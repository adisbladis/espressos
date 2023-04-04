import { v4 as uuidv4 } from 'uuid';

import {
  Command,
  PowerOn,
  PowerOff,
  StartBrew,
  StopBrew,
  StartSteam,
  StopSteam,
  SetSteamSetPoint,
  SetBoilerSetPoint,
} from './api';

export class APIClient {
  readonly socket: WebSocket

  constructor(url: string) {
    this.socket = new WebSocket(url);

    this.socket.onopen = (event) => {
      console.log("connected to server", event);
    }

    this.socket.onmessage = (event) => {
      console.log("got message ", event.data);
    }

    this.socket.onclose = (event) => {
      console.log('Disconnected!', event);
    };
  }

  close(): void {
    this.socket.close()
  }

  private async sendCommand(msg: Command): Promise<void> {
    if (this.socket.readyState != WebSocket.OPEN) {
      throw new Error(`socket not connected, status ${this.socket.readyState}`)
    }

    const bytes = Command.encode(msg).finish()
    this.socket.send(bytes)
  }

  async powerOn(): Promise<void> {
    await this.sendCommand({
      requestId: uuidv4(),
      powerOn: <PowerOn>{},
    })
  }

  async powerOff(): Promise<void> {
    await this.sendCommand({
      requestId: uuidv4(),
      powerOn: <PowerOff>{},
    })
  }

  async startBrew(): Promise<void> {
    await this.sendCommand({
      requestId: uuidv4(),
      startBrew: <StartBrew>{},
    })
  }

  async stopBrew(): Promise<void> {
    await this.sendCommand({
      requestId: uuidv4(),
      stopBrew: <StopBrew>{},
    })
  }

  async startSteam(): Promise<void> {
    await this.sendCommand({
      requestId: uuidv4(),
      startSteam: <StartSteam>{},
    })
  }

  async stopSteam(): Promise<void>{
    await this.sendCommand({
      requestId: uuidv4(),
      stopSteam: <StopSteam>{},
    })
  }

  async setSteamSetPoint(): Promise<void>{
    await this.sendCommand({
      requestId: uuidv4(),
      setSteamSetPoint: <SetSteamSetPoint>{},
    })
  }

  async setBoilerSetPoint(): Promise<void>{
    await this.sendCommand({
      requestId: uuidv4(),
      setBoilerSetPoint: <SetBoilerSetPoint>{},
    })
  }

}
