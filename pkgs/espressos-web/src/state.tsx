import { createStore } from "solid-js/store";
import { StateUpdate } from "./api/api";

export const [state, setState] = createStore(
  StateUpdate.create({
    boilerTemp: {
      valueOrError: {
        $case: "value",
        value: 0,
      },
    },
    pressure: {
      valueOrError: {
        $case: "value",
        value: 0,
      },
    },
  }),
);
