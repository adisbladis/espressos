import { v4 as uuidv4, stringify as uuidStringify } from "uuid";

import {
  Command,
  PowerOn,
  PowerOff,
  StartBrew,
  StopBrew,
  StartPump,
  StopPump,
  StartSteam,
  StopSteam,
  Event,
  BrewTarget,
  BrewTargetMode,
  SetpointSet,
} from "./proto/api";

const mkRequestId = (): Uint8Array => new Uint8Array(uuidv4(null, []));

// A promise wrapper that can be resolved externally
class PromiseExt<T> {
  resolve!: (value: T | PromiseLike<T>) => void;
  reject!: (reason?: any) => void;
  promise: Promise<T>;

  constructor(timeout: number) {
    this.promise = new Promise<T>((resolve, reject) => {
      this.resolve = resolve;
      this.reject = reject;

      if (timeout) {
        setTimeout(() => reject(new Error("Request timed out")), timeout);
      }
    });
  }
}

export class APIClient {
  readonly socket: WebSocket;
  private handlers: Map<string, Array<(event: Event) => void>>;

  // UUID -> Promise
  private requests: Map<string, PromiseExt<void>>;

  constructor(url: string) {
    this.socket = new WebSocket(url);
    this.handlers = new Map();
    this.requests = new Map();

    this.socket.onopen = () => {
      console.log("connected to server", this);
    };

    this.socket.onmessage = async (event) => {
      const msg = Event.decode(new Uint8Array(await event.data.arrayBuffer()));

      // Dispatch to event handlers
      for (const key of ["*", msg.eventOneof.$case]) {
        const handlersArr: Array<(event: Event) => void> =
          this.handlers.get(key);
        if (!handlersArr) {
          continue;
        }

        for (const cb of handlersArr) {
          cb(msg);
        }
      }

      // Resolve response promise
      if (msg.eventOneof.$case == "resp") {
        const resp = msg.eventOneof.resp;

        const result = this.requests.get(uuidStringify(resp.requestId));
        if (result !== undefined) {
          switch (resp.respOneof.$case) {
            case "OK":
              result.resolve();
              break;
            case "error":
              result.reject(resp.respOneof.error);
              break;
          }
        }
      }
    };

    this.socket.onclose = (event) => {
      console.log("Disconnected!", event);
    };
  }

  close(): void {
    this.socket.close();
  }

  onEvent(kind: string, cb: (event: Event) => void): void {
    let handlersArr: Array<(event: Event) => void>;

    if (this.handlers.has(kind)) {
      handlersArr = this.handlers.get(kind);
    } else {
      handlersArr = [];
      this.handlers.set(kind, handlersArr);
    }

    handlersArr.push(cb);
  }

  private async sendCommand(msg: Command): Promise<void> {
    if (this.socket.readyState != WebSocket.OPEN) {
      throw new Error(`socket not connected, status ${this.socket.readyState}`);
    }

    if (!msg.requestId) {
      throw new Error("Message missing requestId");
    }

    const bytes = Command.encode(msg).finish();

    // Register request in the request map so we can route the response
    const uuidString = uuidStringify(msg.requestId);

    const promiseExt = new PromiseExt<void>(1000); // Timeout 1s
    this.requests.set(uuidString, promiseExt);

    console.log("Sending command", msg.commandOneof.$case, msg);
    this.socket.send(bytes);

    try {
      await promiseExt.promise;
    } catch (e) {
      console.error(e);
    }

    this.requests.delete(uuidString);
  }

  async powerOn(setpoint: number): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: {
        $case: "powerOn",
        powerOn: <PowerOn>{
          setpoint: setpoint,
        },
      },
    });
  }

  async powerOff(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "powerOff", powerOff: <PowerOff>{} },
    });
  }

  async startBrew(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "startBrew", startBrew: <StartBrew>{} },
    });
  }

  async stopBrew(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "stopBrew", stopBrew: <StopBrew>{} },
    });
  }

  async startPump(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "startPump", startPump: <StartPump>{} },
    });
  }

  async stopPump(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "stopPump", stopPump: <StopPump>{} },
    });
  }

  async startSteam(setpoint: number): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: {
        $case: "startSteam",
        startSteam: <StartSteam>{
          setpoint: setpoint,
        },
      },
    });
  }

  async stopSteam(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "stopSteam", stopSteam: <StopSteam>{} },
    });
  }

  async backflushStart(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: {
        $case: "backflushStart",
        backflushStart: <BackflushStart>{},
      },
    });
  }

  async backflushStop(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: {
        $case: "backflushStop",
        backflushStop: <BackflushStop>{},
      },
    });
  }

  async rinseStart(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "rinseStart", rinseStart: <RinseStart>{} },
    });
  }

  async rinseStop(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "rinseStop", rinseStop: <RinseStop>{} },
    });
  }

  async targetSetPower(power: number): Promise<void> {
    if (power < 0 || power > 65535) {
      throw new Error("Power value out of bounds");
    }

    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: {
        $case: "brewTargetSet",
        brewTargetSet: <BrewTarget>{
          mode: BrewTargetMode.POWER,
          value: power,
        },
      },
    });
  }

  async setpointSet(setpoint: number): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: {
        $case: "setpointSet",
        setpointSet: <SetpointSet>{
          setpoint: setpoint,
        },
      },
    });
  }

  async targetSetPressure(pressure: number): Promise<void> {
    if (pressure < 0 || pressure > 65535) {
      throw new Error("Power value out of bounds");
    }

    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: {
        $case: "brewTargetSet",
        brewTargetSet: <BrewTarget>{
          mode: BrewTargetMode.PRESSURE,
          value: pressure,
        },
      },
    });
  }
}
