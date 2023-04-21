import type { Component } from 'solid-js';
import { onCleanup } from "solid-js";
import { createStore } from "solid-js/store";

import {
  APIClient,
} from './api'
import {
  Event, FloatSensorReading, LogMessage_LogLevel,
} from './api/api'
import {
  state,
  setState,
} from './state'
import {
  setConfig,
} from './config'

import { SolidChart, SolidChartProps } from './SolidChart';

const client = new APIClient("ws://10.1.1.137");

client.onEvent("*", (event: Event) => {
  switch (event.eventOneof.$case) {
    case "stateUpdate":
      setState(event.eventOneof.stateUpdate)
      break
    case "config":
      setConfig(event.eventOneof.config)
      break
  }
})

client.onEvent("log", (event: Event) => {
  switch (event.eventOneof.$case) {
    case "log":
      let { logLevel, msg } = event.eventOneof.log; // eslint-disable-line

      msg = "espressos: " + msg;

      switch (logLevel) {
        case LogMessage_LogLevel.ERROR:
          console.error(msg);
          break
        case LogMessage_LogLevel.INFO:
          console.error(msg);
          break
        case LogMessage_LogLevel.DEBUG:
          console.debug(msg);
          break
        default:
          console.error(`Unknown log level ${logLevel}`, event);
      }
      break
  }
})

const chartUpdateInterval = 100 // In ms
const chartLength = 90 // In seconds

const App: Component = () => {
  const chartArraySize = chartLength * (1000 / chartUpdateInterval)

  const chartSettings: SolidChartProps = {
    type: "line",
    data: {
      // Empty dummy labels
      labels: Array(chartArraySize).fill(""),
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      animation: false,

      scales: {
        x: {
          ticks: {
            // Only show ticks for every second, not every sample
            // Note that this returns the labels in reverse order
            callback: (i: number): string => {
              // Generate a label for the 0 index
              if (i == 0) {
                return chartLength + "s";
              }

              // Return the second count for every 10 seconds
              if (((i + 1) % chartUpdateInterval) == 0) {
                return chartLength - ((i+1) / (1000 / chartUpdateInterval)) + "s";
              }

              return null;
            },
          },
        },
        y: {
          suggestedMax: 100, // 100 degrees celsius, but allow chart to scale accordingly
          beginAtZero: true,
          type: "linear",
          position: "left",
          ticks: {
            callback: (value: number): string => value + "°C",
          }
        },
        yPressure: {
          suggestedMax: 11, // 11 bars, but scalable
          beginAtZero: true,
          type: "linear",
          position: "right",
          ticks: {
            callback: (value: number): string => value + " bar",
          }
        },
      },


      plugins: {
        legend: {
          position: 'bottom',
        },
      }
    },
  };

  const [chart, setChart] = createStore(chartSettings);

  const datasets = [
    {
      label: "Temp",
      borderColor: "red",
      radius: 0,
      borderWidth: 2,
      tension: 0.3,
      data: Array(chartArraySize).fill(0),
    },
    {
      label: "Pressure",
      borderColor: "green",
      radius: 0,
      borderWidth: 2,
      tension: 0.3,
      data: Array(chartArraySize).fill(0),
      yAxisID: "yPressure",
    },
  ]

  const unpackSensorReading = (x: FloatSensorReading["valueOrError"]): number => {
    switch (x.$case) {
      case "value":
        return x.value
      case "error":
        return -1; // Sentinel error value
      default:
        throw new Error("Case unhandled")
    }
  }

  const interval = setInterval(() => {
    for (let i = 0; i < chart.data.datasets.length; i++) {
      const dataset = chart.data.datasets[i]
      const data = [...dataset.data]
      data.shift()

      let value = -1

      switch (dataset.label) {
        case "Temp":
          value = unpackSensorReading(state.boilerTemp.valueOrError)
          break
        case "Pressure":
          value = unpackSensorReading(state.pressure.valueOrError)
          break
      }

      data.push(value)

      setChart("data", "datasets", i, "data", data)
    }
  }, chartUpdateInterval)

  onCleanup(() => {
    clearInterval(interval)
    client.close()
  });

  setChart("data", "datasets", datasets);

  return (
    <>
      <div class="flex">

        <div class="flex-auto">
          <SolidChart
            {...chart}
            canvasOptions={{
              width: 300,
              height: 600,
            }}
    />
        </div>

        <div class="flex-col">
          <div>
            <button class="btn m-1 btn-lg" onClick={(() => state.isBrewing ? client.stopBrew() : client.startBrew())}>
              ☕
            </button>

            <button class="btn m-1 btn-lg" onClick={(() => state.isPumping ? client.stopPump() : client.startPump())}>
              💧
            </button>
          </div>

          <div>
            <button class="btn m-1 btn-lg" onClick={(() => state.isSteaming ? client.stopSteam() : client.startSteam())}>
              ☁️
            </button>

            <button class="btn m-1 btn-lg" onClick={(() => state.isOn ? client.powerOff() : client.powerOn())}>
              ⚡
            </button>
          </div>

          <div>

            <div class="card  bg-base-100 shadow-xl">
              <div class="card-body">
                <ul>

                  <li>
                    Temp: {((): string => {
                        switch (state.boilerTemp.valueOrError.$case) {
                          case "value":
                            return `${state.boilerTemp.valueOrError.value.toFixed(2)}°C`
                          case "error":
                            return "ERR"
                        }
                    })()}
                  </li>

                  <li>
                    Pressure: {((): string => {
                        switch (state.pressure.valueOrError.$case) {
                          case "value":
                            return `${state.pressure.valueOrError.value.toFixed(2)} bar`
                          case "error":
                            return "ERR"
                        }
                    })()}
                  </li>

                </ul>
              </div>
            </div>

          </div>

        </div>

      </div>

    </>
  );
};

export default App;
