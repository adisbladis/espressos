import type { Component } from "solid-js";
import { onCleanup } from "solid-js";

import { APIClient } from "./api";
import { Event, LogMessage_LogLevel } from "./api/api";
import { state, setState } from "./state";
import { config, setConfig } from "./config";

import Chart from "./chart";

const client = new APIClient(
  (window.location.protocol === "https:" ? "wss:" : "ws:") +
    "//" +
    window.location.host +
    "/api",
);

client.onEvent("*", (event: Event) => {
  switch (event.eventOneof.$case) {
    case "stateUpdate":
      setState(event.eventOneof.stateUpdate);
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

  return (
    <>
      <div class="flex">
        <div class="flex-auto">
          <Chart />
        </div>

        <div class="flex-col">
          <div>
            <button
              class="btn m-1 btn-lg"
              onClick={() =>
                state.isBrewing ? client.stopBrew() : client.startBrew()
              }
            >
              ☕
            </button>

            <button
              class="btn m-1 btn-lg"
              onClick={() =>
                state.isPumping ? client.stopPump() : client.startPump()
              }
            >
              💧
            </button>
          </div>

          <div>
            <button
              class="btn m-1 btn-lg"
              onClick={() =>
                state.isSteaming ? client.stopSteam() : client.startSteam()
              }
            >
              ☁️
            </button>

            <button
              class="btn m-1 btn-lg"
              onClick={() =>
                state.isOn ? client.powerOff() : client.powerOn()
              }
            >
              ⚡
            </button>
          </div>

          <div>
            <div class="card  bg-base-100 shadow-xl">
              <div class="card-body">

                <div class="flex flex-row-reverse">
                  <label for="state-store-modal">ℹ️</label>
                  <label for="config-store-modal">⚙️</label>
                </div>

                <input type="checkbox" id="state-store-modal" class="modal-toggle" />
                <div class="modal">
                  <div class="modal-box">
                    <h3 class="font-bold text-lg">Current state:</h3>
                    <pre class="py-4">
                      {JSON.stringify(state, null, 2)}
                    </pre>
                    <div class="modal-action">
                      <label for="state-store-modal" class="btn">Close</label>
                    </div>
                  </div>
                </div>

                <input type="checkbox" id="config-store-modal" class="modal-toggle" />
                <div class="modal">
                  <div class="modal-box">
                    <h3 class="font-bold text-lg">Current config:</h3>
                    <pre class="py-4">
                      {JSON.stringify(config, null, 2)}
                    </pre>
                    <div class="modal-action">
                      <label for="config-store-modal" class="btn">Close</label>
                    </div>
                  </div>
                </div>

                <ul>

                  <li>
                    Mode: {((): string => {
                        if (!state.isOn) {
                          return "☠️"
                        }

                        if (state.isBrewing) {
                          return "☕"
                        }

                        if (state.isPumping) {
                          return "💧"
                        }

                        if (state.isSteaming) {
                          return "☁️"
                        }

                        return "⚡"
                    })()}
                  </li>

                  <li>
                    Temp:{" "}
                    {((): string => {
                      switch (state.boilerTemp.valueOrError.$case) {
                        case "value":
                          return `${state.boilerTemp.valueOrError.value.toFixed(
                            2,
                          )}°C`;
                        case "error":
                          return "ERR";
                      }
                    })()}
                  </li>

                  <li>
                    Pressure:{" "}
                    {((): string => {
                      switch (state.pressure.valueOrError.$case) {
                        case "value":
                          return `${state.pressure.valueOrError.value.toFixed(
                            2,
                          )} bar`;
                        case "error":
                          return "ERR";
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
