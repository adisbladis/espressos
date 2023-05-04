import { createStore } from "solid-js/store";
import { StateUpdate } from "./proto/api";

export const [state, setState] = createStore(
  StateUpdate.create({
    boilerTemp: {
      result: {
        $case: "value",
        value: 0,
      },
    },
    pressure: {
      result: {
        $case: "value",
        value: 0,
      },
    },
  }),
);
