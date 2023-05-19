import type { Component } from "solid-js";
import { onCleanup } from "solid-js";
import { createStore } from "solid-js/store";
import { FloatSensorReading, Uint32Result } from "./api/api";
import { state } from "./state";
import { SolidChart, SolidChartProps } from "./SolidChart";

const chartUpdateInterval = 100; // In ms
const chartLength = 90; // In seconds

const Chart: Component = () => {
  const chartArraySize = chartLength * (1000 / chartUpdateInterval);

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
              if ((i + 1) % chartUpdateInterval == 0) {
                return (
                  chartLength - (i + 1) / (1000 / chartUpdateInterval) + "s"
                );
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
          },
        },
        yPressure: {
          suggestedMax: 11, // 11 bars, but scalable
          beginAtZero: true,
          type: "linear",
          position: "right",
          ticks: {
            callback: (value: number): string => value + " bar",
          },
        },
      },

      plugins: {
        legend: {
          position: "bottom",
        },
      },
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
  ];

  const interval = setInterval(() => {
    for (let i = 0; i < chart.data.datasets.length; i++) {
      const dataset = chart.data.datasets[i];
      const data = [...dataset.data];
      data.shift();

      let value = -1;

      switch (dataset.label) {
        case "Temp":
          value = state.boilerTemp / 100;
          break;
        case "Pressure":
          value = state.pressure / 1000;
          break;
      }

      data.push(value);

      setChart("data", "datasets", i, "data", data);
    }
  }, chartUpdateInterval);

  onCleanup(() => {
    clearInterval(interval);
  });

  setChart("data", "datasets", datasets);

  return (
    <SolidChart
      {...chart}
      canvasOptions={{
        width: 100,
        height: 100,
      }}
    />
  );
};

export default Chart;
