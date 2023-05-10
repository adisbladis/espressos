import type { Component } from "solid-js";
import { onCleanup, createMemo } from "solid-js";
import { produce } from "solid-js/store";

import { APIClient } from "./api";
import {
  BrewTargetMode,
  Event,
  LogMessage_LogLevel,
  MachineMode,
} from "./proto/api";
import { state, setState } from "./state";
import { config, setConfig } from "./config";

import Chart from "./chart";

const Symbols = {
  UNKNOWN: "❓",
  PANIC: "❗",
  OFF: "😴",
  POWER: "⚡",
  BREWING: "☕",
  PUMPING: "💧",
  STEAMING: "☁️",
  SETTINGS: "⚙️",
  INFO: "ℹ️",
  BACKFLUSHING: "🧹",
  RINSING: "💦",
};

const client = new APIClient(
  (window.location.protocol === "https:" ? "wss:" : "ws:") +
    "//" +
    window.location.host +
    "/api",
);

// Allows to set a value with the setter being called at most once per maxInterval
function createPacedSetter<Type>(interval: number, cb: (value: Type) => void) {
  let next: number = performance.now();
  let value: Type;

  return (newValue: Type) => {
    const now = performance.now();

    value = newValue;

    // Schedule timer
    if (now >= next) {
      next = now + interval;
      setTimeout(() => {
        cb(value);
      }, interval);
    }
  };
}

client.onEvent("*", (event: Event) => {
  switch (event.eventOneof.$case) {
    case "stateUpdate":
      setState(event.eventOneof.stateUpdate);

      const temp = event.eventOneof.stateUpdate.boilerTemp.result;
      if (temp.$case == "error") {
        console.error(temp);
      }

      const pressure = event.eventOneof.stateUpdate.pressure.result;
      if (pressure.$case == "error") {
        console.error(pressure);
      }

      break;
    case "config":
      setConfig(event.eventOneof.config);
      break;
  }
});

client.onEvent("log", (event: Event) => {
  switch (event.eventOneof.$case) {
    case "log":
      let { logLevel, msg } = event.eventOneof.log; // eslint-disable-line

      msg = "espressos: " + msg;

      switch (logLevel) {
        case LogMessage_LogLevel.ERROR:
          console.error(msg);
          break;
        case LogMessage_LogLevel.INFO:
          console.info(msg);
          break;
        case LogMessage_LogLevel.DEBUG:
          console.debug(msg);
          break;
        default:
          console.error(`Unknown log level ${logLevel}`, event);
          break;
      }
      break;
  }
});

const App: Component = () => {
  onCleanup(() => {
    client.close();
  });

  const modeSymbol = createMemo(() => {
    switch (state.mode) {
      case MachineMode.UNKNOWN:
        return Symbols.UNKNOWN;
      case MachineMode.PANIC:
        return Symbols.PANIC;
      case MachineMode.OFF:
        return Symbols.OFF;
      case MachineMode.IDLE:
        return Symbols.POWER;
      case MachineMode.BREWING:
        return Symbols.BREWING;
      case MachineMode.BACKFLUSHING:
        return Symbols.BACKFLUSHING;
      case MachineMode.PUMPING:
        return Symbols.PUMPING;
      case MachineMode.STEAMING:
        return Symbols.STEAMING;
      case MachineMode.RINSING:
        return Symbols.RINSING;
      default:
        return Symbols.UNKNOWN;
    }
  });

  const shotTimer = createMemo(() => {
    if (!state.shotTimer) {
      return "0s";
    }

    const shotMs =
      state.mode == MachineMode.BREWING
        ? state.millis - state.shotTimer.start
        : state.shotTimer.stop - state.shotTimer.start;
    const minutes = Math.floor(shotMs / 60000);
    const seconds = ((shotMs % 60000) / 1000).toFixed(1);

    const s = minutes ? `${minutes}m` : "";

    return s + (s ? " " : "") + `${seconds}s`;
  });

  // A timer used to pace the setting of pressure
  const setTargetPressure = createPacedSetter(50, (pressure: number) => {
    setState(
      produce((s) => {
        s.brewTarget.mode = BrewTargetMode.PRESSURE;
        s.brewTarget.value = pressure;
      }),
    );

    client.targetSetPressure(pressure);
  });

  return (
    <>
      <div class="flex">
        <div class="flex-col">
          <div>
            <button
              class="btn m-1 btn-lg"
              onClick={() =>
                state.mode != MachineMode.OFF && state.mode != MachineMode.PANIC
                  ? client.powerOff()
                  : client.powerOn(config.setpoint)
              }
            >
              {Symbols.POWER}
            </button>
          </div>

          <div>
            <button
              class="btn m-1 btn-lg"
              onClick={() =>
                state.mode == MachineMode.BREWING
                  ? client.stopBrew()
                  : client.startBrew()
              }
            >
              {Symbols.BREWING}
            </button>
          </div>

          <div>
            <button
              class="btn m-1 btn-lg"
              onClick={() =>
                state.mode == MachineMode.PUMPING
                  ? client.stopPump()
                  : client.startPump()
              }
            >
              {Symbols.PUMPING}
            </button>
          </div>

          <div>
            <button
              class="btn m-1 btn-lg"
              onClick={() =>
                state.mode == MachineMode.STEAMING
                  ? client.stopSteam()
                  : client.startSteam(config.steamSetPoint)
              }
            >
              {Symbols.STEAMING}
            </button>
          </div>

          <div>
            <button
              class="btn m-1 btn-lg"
              onClick={() => client.backflushStart()}
            >
              {Symbols.BACKFLUSHING}
            </button>
          </div>

          <div>
            <button class="btn m-1 btn-lg" onClick={() => client.rinseStart()}>
              {Symbols.RINSING}
            </button>
          </div>
        </div>

        <div class="flex-auto">
          <Chart />
        </div>

        <div class="flex-col">
          <div>
            <div class="card  bg-base-100 shadow-xl">
              <div class="card-body">
                <div>
                  <button
                    class="btn m-1 btn-lg"
                    onClick={() =>
                      client.powerOn((state.setpoint / 100 + 1) * 100)
                    }
                  >
                    ➕
                  </button>

                  {state.setpoint / 100}

                  <button
                    class="btn m-1 btn-lg"
                    onClick={() =>
                      client.powerOn((state.setpoint / 100 - 1) * 100)
                    }
                  >
                    ➖
                  </button>
                </div>
                {state.brewTarget.value / 1000} bar
                <input
                  type="range"
                  min="0"
                  max="10000" /* 10000 mBar */
                  value={state.brewTarget.value}
                  class="range-sm"
                  onInput={(e) => setTargetPressure(Number(e.target.value))}
                />
              </div>
            </div>
          </div>

          <div>
            <div class="card  bg-base-100 shadow-xl">
              <div class="card-body">
                <div class="flex flex-row-reverse">
                  <label for="state-store-modal">{Symbols.INFO}</label>
                  <label for="config-store-modal">{Symbols.SETTINGS}</label>
                </div>

                <input
                  type="checkbox"
                  id="state-store-modal"
                  class="modal-toggle"
                />
                <div class="modal">
                  <div class="modal-box">
                    <h3 class="font-bold text-lg">Current state:</h3>
                    <pre class="py-4">{JSON.stringify(state, null, 2)}</pre>
                    <div class="modal-action">
                      <label for="state-store-modal" class="btn">
                        Close
                      </label>
                    </div>
                  </div>
                </div>

                <input
                  type="checkbox"
                  id="config-store-modal"
                  class="modal-toggle"
                />
                <div class="modal">
                  <div class="modal-box">
                    <h3 class="font-bold text-lg">Current config:</h3>
                    <pre class="py-4">{JSON.stringify(config, null, 2)}</pre>
                    <div class="modal-action">
                      <label for="config-store-modal" class="btn">
                        Close
                      </label>
                    </div>
                  </div>
                </div>

                <ul>
                  <li>Mode: {modeSymbol()}</li>

                  <li>
                    Temp:{" "}
                    {((): string => {
                      switch (state.boilerTemp.result.$case) {
                        case "value":
                          return `${(
                            state.boilerTemp.result.value / 100
                          ).toFixed(2)}°C`;
                        case "error":
                          return "ERR";
                      }
                    })()}
                  </li>

                  <li>
                    Pressure:{" "}
                    {((): string => {
                      switch (state.pressure.result.$case) {
                        case "value":
                          return `${(
                            state.pressure.result.value / 1000
                          ).toFixed(2)} bar`;
                        case "error":
                          return "ERR";
                      }
                    })()}
                  </li>

                  <li>Timer: {shotTimer}</li>
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
