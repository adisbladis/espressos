import type { Component } from 'solid-js';
import { createSignal, onCleanup } from "solid-js";

import {
  APIClient
} from './api'

const client = new APIClient("ws://10.1.1.137");

const App: Component = () => {
  const [boilerTemp, setBoilerTemp] = createSignal(0);

  // TODO: Replace with monitor API once it's working
  const timer = setInterval(() => {
    setBoilerTemp(boilerTemp() + 1);
  }, 1000);
  onCleanup(() => {
    clearInterval(timer);
    client.close()
  });

  const [brewing, setBrewing] = createSignal(false);

  return (
    <>

      <div style="margin: 10px;">

        <p style="margin: 10px;"></p>

        <button class="btn btn-lg" onClick={(async () => {
          if (!brewing()) {
            setBrewing(true) // TODO: Message based
            client.startBrew()
          } else {
            setBrewing(false) // TODO: Message based
            client.stopBrew()
          }
        })}>
          ☕
        </button>

        <p style="margin: 10px;"></p>

        <button class="btn btn-lg" onClick={(async () => {
        })}>
          💧
        </button>

        <span style="margin-left: 10px" />

        <button class="btn btn-lg" onClick={(async () => {
        })}>
          ⚡
        </button>

        <p style="margin: 10px;"></p>

        <button class="btn btn-lg" onClick={(async () => {
        })}>
          ☁️
        </button>

        <p style="margin: 10px;">{boilerTemp()}°C</p>

      </div>

    </>
  );
};

export default App;
