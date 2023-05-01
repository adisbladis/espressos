import { v4 as uuidv4 } from 'uuid';

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
} from './api';

const mkRequestId = (): Uint8Array => new Uint8Array(uuidv4(null, []))

export class APIClient {
  readonly socket: WebSocket;
  private handlers: Map<string, Array<(event: Event) => void>>;

  constructor(url: string) {
    this.socket = new WebSocket(url);
    this.handlers = new Map();

    this.socket.onopen = () => {
      console.log("connected to server", this);
    }

    this.socket.onmessage = async (event) => {
      const msg = Event.decode(new Uint8Array(await event.data.arrayBuffer()));

      for (const key of ["*", msg.eventOneof.$case]) {
        const handlersArr: Array<(event: Event) => void> = this.handlers.get(key)
        if (!handlersArr) {
          continue
        }

        for (const cb of handlersArr) {
          cb(msg)
        }
      }
    }

    this.socket.onclose = (event) => {
      console.log('Disconnected!', event);
    };
  }

  close(): void {
    this.socket.close()
  }

  onEvent(kind: string, cb: (event: Event) => void): void {
    let handlersArr: Array<(event: Event) => void>

    if (this.handlers.has(kind)) {
      handlersArr = this.handlers.get(kind)
    } else {
      handlersArr = []
      this.handlers.set(kind, handlersArr)
    }

    handlersArr.push(cb)
  }

  private async sendCommand(msg: Command): Promise<void> {
    if (this.socket.readyState != WebSocket.OPEN) {
      throw new Error(`socket not connected, status ${this.socket.readyState}`)
    }

    console.log("Sending command", msg)

    const bytes = Command.encode(msg).finish()
    this.socket.send(bytes)
  }

  async powerOn(setpoint: number): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "powerOn", powerOn: <PowerOn>{
        setpoint: setpoint,
      } },
    })
  }

  async powerOff(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "powerOff", powerOff: <PowerOff>{} },
    })
  }

  async startBrew(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "startBrew", startBrew: <StartBrew>{} },
    })
  }

  async stopBrew(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "stopBrew", stopBrew: <StopBrew>{} },
    })
  }

  async startPump(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "startPump", startPump: <StartPump>{} },
    })
  }

  async stopPump(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "stopPump", stopPump: <StopPump>{} },
    })
  }

  async startSteam(setpoint: number): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "startSteam", startSteam: <StartSteam>{
        setpoint: setpoint,
      } },
    })
  }

  async stopSteam(): Promise<void>{
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "stopSteam", stopSteam: <StopSteam>{} },
    })
  }

  async backflushStart(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "backflushStart", backflushStart: <BackflushStart>{} },
    })
  }

  async backflushStop(): Promise<void>{
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "backflushStop", backflushStop: <BackflushStop>{} },
    })
  }

  async rinseStart(): Promise<void> {
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "rinseStart", rinseStart: <RinseStart>{} },
    })
  }

  async rinseStop(): Promise<void>{
    await this.sendCommand({
      requestId: mkRequestId(),
      commandOneof: { $case: "rinseStop", rinseStop: <RinseStop>{} },
    })
  }
}
