import { createStore } from "solid-js/store";
import { Config, BoilerConfig, PIDTunings } from "./proto/config";

export const [config, setConfig] = createStore(
  Config.create({
    boiler: BoilerConfig.create({
      PID: PIDTunings.create(),
    }),
  }),
);
