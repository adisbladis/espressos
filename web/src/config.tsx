import { createStore } from "solid-js/store";
import {
  Config,
  BoilerConfig,
  PIDTunings,
} from './api/config';

export const [ config, setConfig ] = createStore(Config.create({
  boiler: BoilerConfig.create({
    PID: PIDTunings.create(),
  }),
}));
