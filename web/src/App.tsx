import type { Component } from 'solid-js';
import { onCleanup } from "solid-js";

import {
  APIClient,
} from './api'
import {
  Event,
} from './api/api'
import {
  state,
  setState,
} from './state'

const client = new APIClient("ws://10.1.1.137");

client.onEvent("stateUpdate", (event: Event) => {
  setState(event.eventOneof.stateUpdate)
})

const App: Component = () => {
  onCleanup(() => {
    client.close()
  });

  return (
    <>
      <div style="margin: 10px;">

        <p style="margin: 10px;"></p>

        <button class="btn btn-lg" onClick={(() => state.isBrewing ? client.stopBrew() : client.startBrew())}>
          ☕
        </button>

        <p style="margin: 10px;"></p>

        <button class="btn btn-lg" onClick={(() => state.isPumping ? client.stopPump() : client.startPump())}>
          💧
        </button>

        <span style="margin-left: 10px" />

        <button class="btn btn-lg" onClick={(() => state.isOn ? client.powerOff() : client.powerOn())}>
          ⚡
        </button>

        <p style="margin: 10px;"></p>

        <button class="btn btn-lg" onClick={(() => state.isSteaming ? client.stopSteam() : client.startSteam())}>
          ☁️
        </button>

        <p style="margin: 10px;">{((): string => {
            switch (state.boilerTemp.valueOrError.$case) {
              case "value":
                return `${state.boilerTemp.valueOrError.value.toFixed(2)}°C`
              case "error":
                return "ERR"
            }
        })()}</p>

      </div>

    </>
  );
};

export default App;
