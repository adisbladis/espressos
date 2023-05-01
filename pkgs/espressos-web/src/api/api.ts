/* eslint-disable */
import * as _m0 from "protobufjs/minimal";
import { Config } from "./config";

export const protobufPackage = "";

export enum MachineMode {
  UNKNOWN = 0,
  PANIC = 1,
  OFF = 2,
  IDLE = 3,
  BREWING = 4,
  BACKFLUSHING = 5,
  PUMPING = 6,
  STEAMING = 7,
  UNRECOGNIZED = -1,
}

export function machineModeFromJSON(object: any): MachineMode {
  switch (object) {
    case 0:
    case "UNKNOWN":
      return MachineMode.UNKNOWN;
    case 1:
    case "PANIC":
      return MachineMode.PANIC;
    case 2:
    case "OFF":
      return MachineMode.OFF;
    case 3:
    case "IDLE":
      return MachineMode.IDLE;
    case 4:
    case "BREWING":
      return MachineMode.BREWING;
    case 5:
    case "BACKFLUSHING":
      return MachineMode.BACKFLUSHING;
    case 6:
    case "PUMPING":
      return MachineMode.PUMPING;
    case 7:
    case "STEAMING":
      return MachineMode.STEAMING;
    case -1:
    case "UNRECOGNIZED":
    default:
      return MachineMode.UNRECOGNIZED;
  }
}

export function machineModeToJSON(object: MachineMode): string {
  switch (object) {
    case MachineMode.UNKNOWN:
      return "UNKNOWN";
    case MachineMode.PANIC:
      return "PANIC";
    case MachineMode.OFF:
      return "OFF";
    case MachineMode.IDLE:
      return "IDLE";
    case MachineMode.BREWING:
      return "BREWING";
    case MachineMode.BACKFLUSHING:
      return "BACKFLUSHING";
    case MachineMode.PUMPING:
      return "PUMPING";
    case MachineMode.STEAMING:
      return "STEAMING";
    case MachineMode.UNRECOGNIZED:
    default:
      return "UNRECOGNIZED";
  }
}

export interface PowerOn {
  /**
   * Setpoint represented as a hundredth of a degree celsius
   * i.e. you'll get the real decimal representation by multiplying
   * this number by 100.
   */
  setpoint: number;
}

export interface PowerOff {
}

export interface StartBrew {
}

export interface StopBrew {
}

export interface StartPump {
}

export interface StopPump {
}

export interface StartSteam {
  /**
   * Setpoint represented as a hundredth of a degree celsius
   * i.e. you'll get the real decimal representation by multiplying
   * this number by 100.
   */
  setpoint: number;
}

export interface StopSteam {
}

export interface BackflushStart {
}

export interface BackflushStop {
}

export interface RinseStart {
}

export interface RinseStop {
}

export interface Command {
  /**
   * A unique client-generated request identifier.
   * This will be embedded into a command response coming from the server
   * so the client can route the response appropriately.
   */
  requestId: Uint8Array;
  commandOneof?:
    | { $case: "powerOn"; powerOn: PowerOn }
    | { $case: "powerOff"; powerOff: PowerOff }
    | { $case: "startBrew"; startBrew: StartBrew }
    | { $case: "stopBrew"; stopBrew: StopBrew }
    | { $case: "startPump"; startPump: StartPump }
    | { $case: "stopPump"; stopPump: StopPump }
    | { $case: "startSteam"; startSteam: StartSteam }
    | { $case: "stopSteam"; stopSteam: StopSteam }
    | { $case: "config"; config: Config }
    | { $case: "backflushStart"; backflushStart: BackflushStart }
    | { $case: "backflushStop"; backflushStop: BackflushStop }
    | { $case: "rinseStart"; rinseStart: RinseStart }
    | { $case: "rinseStop"; rinseStop: RinseStop };
}

/** An envelope for an arbitrary sensor that returns a float with an optional error */
export interface FloatSensorReading {
  valueOrError?: { $case: "value"; value: number } | { $case: "error"; error: string };
}

/**
 * The shot timer requires the caller to do a bit of maths to get the actual
 * shot time in milliseconds:
 *
 * if (StateUpdate.mode == MachineMode.BREWING) {
 *   return (StateUpdate.millis - ShotTimer.start)
 * } else {
 *   return (ShotTimer.stop - ShotTimer.start)
 * }
 *
 * Start/Stop could also be used together with StateUpdate.millis to establish
 * _when_ the last shot was pulled.
 */
export interface ShotTimer {
  start: number;
  stop: number;
}

export interface StateUpdate {
  mode: MachineMode;
  boilerTemp: FloatSensorReading | undefined;
  pressure: FloatSensorReading | undefined;
  setpoint: number;
  /**
   * Current time of machine since startup in milliseconds
   * as returned by the Arduino function millis().
   *
   * This is intended to be used for a client to synchronise time with
   * the server so it can make sense of things like shot timers
   * which aren't expressed as a straight up count but as timestamps.
   *
   * This number can also be used to display the uptime of the machine.
   */
  millis: number;
  shotTimer: ShotTimer | undefined;
}

export interface LogMessage {
  logLevel: LogMessage_LogLevel;
  msg: string;
}

export enum LogMessage_LogLevel {
  ERROR = 0,
  INFO = 1,
  DEBUG = 2,
  UNRECOGNIZED = -1,
}

export function logMessage_LogLevelFromJSON(object: any): LogMessage_LogLevel {
  switch (object) {
    case 0:
    case "ERROR":
      return LogMessage_LogLevel.ERROR;
    case 1:
    case "INFO":
      return LogMessage_LogLevel.INFO;
    case 2:
    case "DEBUG":
      return LogMessage_LogLevel.DEBUG;
    case -1:
    case "UNRECOGNIZED":
    default:
      return LogMessage_LogLevel.UNRECOGNIZED;
  }
}

export function logMessage_LogLevelToJSON(object: LogMessage_LogLevel): string {
  switch (object) {
    case LogMessage_LogLevel.ERROR:
      return "ERROR";
    case LogMessage_LogLevel.INFO:
      return "INFO";
    case LogMessage_LogLevel.DEBUG:
      return "DEBUG";
    case LogMessage_LogLevel.UNRECOGNIZED:
    default:
      return "UNRECOGNIZED";
  }
}

export interface Event {
  /** If this event was in response to a request id it will contain one, otherwise empty */
  requestId: Uint8Array;
  eventOneof?: { $case: "stateUpdate"; stateUpdate: StateUpdate } | { $case: "config"; config: Config } | {
    $case: "log";
    log: LogMessage;
  };
}

function createBasePowerOn(): PowerOn {
  return { setpoint: 0 };
}

export const PowerOn = {
  encode(message: PowerOn, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.setpoint !== 0) {
      writer.uint32(8).int32(message.setpoint);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): PowerOn {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBasePowerOn();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 8) {
            break;
          }

          message.setpoint = reader.int32();
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): PowerOn {
    return { setpoint: isSet(object.setpoint) ? Number(object.setpoint) : 0 };
  },

  toJSON(message: PowerOn): unknown {
    const obj: any = {};
    message.setpoint !== undefined && (obj.setpoint = Math.round(message.setpoint));
    return obj;
  },

  create<I extends Exact<DeepPartial<PowerOn>, I>>(base?: I): PowerOn {
    return PowerOn.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<PowerOn>, I>>(object: I): PowerOn {
    const message = createBasePowerOn();
    message.setpoint = object.setpoint ?? 0;
    return message;
  },
};

function createBasePowerOff(): PowerOff {
  return {};
}

export const PowerOff = {
  encode(_: PowerOff, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): PowerOff {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBasePowerOff();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): PowerOff {
    return {};
  },

  toJSON(_: PowerOff): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<PowerOff>, I>>(base?: I): PowerOff {
    return PowerOff.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<PowerOff>, I>>(_: I): PowerOff {
    const message = createBasePowerOff();
    return message;
  },
};

function createBaseStartBrew(): StartBrew {
  return {};
}

export const StartBrew = {
  encode(_: StartBrew, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): StartBrew {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseStartBrew();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): StartBrew {
    return {};
  },

  toJSON(_: StartBrew): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<StartBrew>, I>>(base?: I): StartBrew {
    return StartBrew.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<StartBrew>, I>>(_: I): StartBrew {
    const message = createBaseStartBrew();
    return message;
  },
};

function createBaseStopBrew(): StopBrew {
  return {};
}

export const StopBrew = {
  encode(_: StopBrew, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): StopBrew {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseStopBrew();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): StopBrew {
    return {};
  },

  toJSON(_: StopBrew): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<StopBrew>, I>>(base?: I): StopBrew {
    return StopBrew.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<StopBrew>, I>>(_: I): StopBrew {
    const message = createBaseStopBrew();
    return message;
  },
};

function createBaseStartPump(): StartPump {
  return {};
}

export const StartPump = {
  encode(_: StartPump, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): StartPump {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseStartPump();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): StartPump {
    return {};
  },

  toJSON(_: StartPump): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<StartPump>, I>>(base?: I): StartPump {
    return StartPump.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<StartPump>, I>>(_: I): StartPump {
    const message = createBaseStartPump();
    return message;
  },
};

function createBaseStopPump(): StopPump {
  return {};
}

export const StopPump = {
  encode(_: StopPump, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): StopPump {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseStopPump();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): StopPump {
    return {};
  },

  toJSON(_: StopPump): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<StopPump>, I>>(base?: I): StopPump {
    return StopPump.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<StopPump>, I>>(_: I): StopPump {
    const message = createBaseStopPump();
    return message;
  },
};

function createBaseStartSteam(): StartSteam {
  return { setpoint: 0 };
}

export const StartSteam = {
  encode(message: StartSteam, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.setpoint !== 0) {
      writer.uint32(8).int32(message.setpoint);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): StartSteam {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseStartSteam();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 8) {
            break;
          }

          message.setpoint = reader.int32();
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): StartSteam {
    return { setpoint: isSet(object.setpoint) ? Number(object.setpoint) : 0 };
  },

  toJSON(message: StartSteam): unknown {
    const obj: any = {};
    message.setpoint !== undefined && (obj.setpoint = Math.round(message.setpoint));
    return obj;
  },

  create<I extends Exact<DeepPartial<StartSteam>, I>>(base?: I): StartSteam {
    return StartSteam.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<StartSteam>, I>>(object: I): StartSteam {
    const message = createBaseStartSteam();
    message.setpoint = object.setpoint ?? 0;
    return message;
  },
};

function createBaseStopSteam(): StopSteam {
  return {};
}

export const StopSteam = {
  encode(_: StopSteam, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): StopSteam {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseStopSteam();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): StopSteam {
    return {};
  },

  toJSON(_: StopSteam): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<StopSteam>, I>>(base?: I): StopSteam {
    return StopSteam.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<StopSteam>, I>>(_: I): StopSteam {
    const message = createBaseStopSteam();
    return message;
  },
};

function createBaseBackflushStart(): BackflushStart {
  return {};
}

export const BackflushStart = {
  encode(_: BackflushStart, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): BackflushStart {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseBackflushStart();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): BackflushStart {
    return {};
  },

  toJSON(_: BackflushStart): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<BackflushStart>, I>>(base?: I): BackflushStart {
    return BackflushStart.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<BackflushStart>, I>>(_: I): BackflushStart {
    const message = createBaseBackflushStart();
    return message;
  },
};

function createBaseBackflushStop(): BackflushStop {
  return {};
}

export const BackflushStop = {
  encode(_: BackflushStop, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): BackflushStop {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseBackflushStop();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): BackflushStop {
    return {};
  },

  toJSON(_: BackflushStop): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<BackflushStop>, I>>(base?: I): BackflushStop {
    return BackflushStop.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<BackflushStop>, I>>(_: I): BackflushStop {
    const message = createBaseBackflushStop();
    return message;
  },
};

function createBaseRinseStart(): RinseStart {
  return {};
}

export const RinseStart = {
  encode(_: RinseStart, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): RinseStart {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseRinseStart();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): RinseStart {
    return {};
  },

  toJSON(_: RinseStart): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<RinseStart>, I>>(base?: I): RinseStart {
    return RinseStart.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<RinseStart>, I>>(_: I): RinseStart {
    const message = createBaseRinseStart();
    return message;
  },
};

function createBaseRinseStop(): RinseStop {
  return {};
}

export const RinseStop = {
  encode(_: RinseStop, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): RinseStop {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseRinseStop();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(_: any): RinseStop {
    return {};
  },

  toJSON(_: RinseStop): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<RinseStop>, I>>(base?: I): RinseStop {
    return RinseStop.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<RinseStop>, I>>(_: I): RinseStop {
    const message = createBaseRinseStop();
    return message;
  },
};

function createBaseCommand(): Command {
  return { requestId: new Uint8Array(), commandOneof: undefined };
}

export const Command = {
  encode(message: Command, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.requestId.length !== 0) {
      writer.uint32(10).bytes(message.requestId);
    }
    switch (message.commandOneof?.$case) {
      case "powerOn":
        PowerOn.encode(message.commandOneof.powerOn, writer.uint32(18).fork()).ldelim();
        break;
      case "powerOff":
        PowerOff.encode(message.commandOneof.powerOff, writer.uint32(26).fork()).ldelim();
        break;
      case "startBrew":
        StartBrew.encode(message.commandOneof.startBrew, writer.uint32(34).fork()).ldelim();
        break;
      case "stopBrew":
        StopBrew.encode(message.commandOneof.stopBrew, writer.uint32(42).fork()).ldelim();
        break;
      case "startPump":
        StartPump.encode(message.commandOneof.startPump, writer.uint32(50).fork()).ldelim();
        break;
      case "stopPump":
        StopPump.encode(message.commandOneof.stopPump, writer.uint32(58).fork()).ldelim();
        break;
      case "startSteam":
        StartSteam.encode(message.commandOneof.startSteam, writer.uint32(66).fork()).ldelim();
        break;
      case "stopSteam":
        StopSteam.encode(message.commandOneof.stopSteam, writer.uint32(74).fork()).ldelim();
        break;
      case "config":
        Config.encode(message.commandOneof.config, writer.uint32(82).fork()).ldelim();
        break;
      case "backflushStart":
        BackflushStart.encode(message.commandOneof.backflushStart, writer.uint32(90).fork()).ldelim();
        break;
      case "backflushStop":
        BackflushStop.encode(message.commandOneof.backflushStop, writer.uint32(98).fork()).ldelim();
        break;
      case "rinseStart":
        RinseStart.encode(message.commandOneof.rinseStart, writer.uint32(106).fork()).ldelim();
        break;
      case "rinseStop":
        RinseStop.encode(message.commandOneof.rinseStop, writer.uint32(114).fork()).ldelim();
        break;
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): Command {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseCommand();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 10) {
            break;
          }

          message.requestId = reader.bytes();
          continue;
        case 2:
          if (tag != 18) {
            break;
          }

          message.commandOneof = { $case: "powerOn", powerOn: PowerOn.decode(reader, reader.uint32()) };
          continue;
        case 3:
          if (tag != 26) {
            break;
          }

          message.commandOneof = { $case: "powerOff", powerOff: PowerOff.decode(reader, reader.uint32()) };
          continue;
        case 4:
          if (tag != 34) {
            break;
          }

          message.commandOneof = { $case: "startBrew", startBrew: StartBrew.decode(reader, reader.uint32()) };
          continue;
        case 5:
          if (tag != 42) {
            break;
          }

          message.commandOneof = { $case: "stopBrew", stopBrew: StopBrew.decode(reader, reader.uint32()) };
          continue;
        case 6:
          if (tag != 50) {
            break;
          }

          message.commandOneof = { $case: "startPump", startPump: StartPump.decode(reader, reader.uint32()) };
          continue;
        case 7:
          if (tag != 58) {
            break;
          }

          message.commandOneof = { $case: "stopPump", stopPump: StopPump.decode(reader, reader.uint32()) };
          continue;
        case 8:
          if (tag != 66) {
            break;
          }

          message.commandOneof = { $case: "startSteam", startSteam: StartSteam.decode(reader, reader.uint32()) };
          continue;
        case 9:
          if (tag != 74) {
            break;
          }

          message.commandOneof = { $case: "stopSteam", stopSteam: StopSteam.decode(reader, reader.uint32()) };
          continue;
        case 10:
          if (tag != 82) {
            break;
          }

          message.commandOneof = { $case: "config", config: Config.decode(reader, reader.uint32()) };
          continue;
        case 11:
          if (tag != 90) {
            break;
          }

          message.commandOneof = {
            $case: "backflushStart",
            backflushStart: BackflushStart.decode(reader, reader.uint32()),
          };
          continue;
        case 12:
          if (tag != 98) {
            break;
          }

          message.commandOneof = {
            $case: "backflushStop",
            backflushStop: BackflushStop.decode(reader, reader.uint32()),
          };
          continue;
        case 13:
          if (tag != 106) {
            break;
          }

          message.commandOneof = { $case: "rinseStart", rinseStart: RinseStart.decode(reader, reader.uint32()) };
          continue;
        case 14:
          if (tag != 114) {
            break;
          }

          message.commandOneof = { $case: "rinseStop", rinseStop: RinseStop.decode(reader, reader.uint32()) };
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): Command {
    return {
      requestId: isSet(object.requestId) ? bytesFromBase64(object.requestId) : new Uint8Array(),
      commandOneof: isSet(object.powerOn)
        ? { $case: "powerOn", powerOn: PowerOn.fromJSON(object.powerOn) }
        : isSet(object.powerOff)
        ? { $case: "powerOff", powerOff: PowerOff.fromJSON(object.powerOff) }
        : isSet(object.startBrew)
        ? { $case: "startBrew", startBrew: StartBrew.fromJSON(object.startBrew) }
        : isSet(object.stopBrew)
        ? { $case: "stopBrew", stopBrew: StopBrew.fromJSON(object.stopBrew) }
        : isSet(object.startPump)
        ? { $case: "startPump", startPump: StartPump.fromJSON(object.startPump) }
        : isSet(object.stopPump)
        ? { $case: "stopPump", stopPump: StopPump.fromJSON(object.stopPump) }
        : isSet(object.startSteam)
        ? { $case: "startSteam", startSteam: StartSteam.fromJSON(object.startSteam) }
        : isSet(object.stopSteam)
        ? { $case: "stopSteam", stopSteam: StopSteam.fromJSON(object.stopSteam) }
        : isSet(object.config)
        ? { $case: "config", config: Config.fromJSON(object.config) }
        : isSet(object.backflushStart)
        ? { $case: "backflushStart", backflushStart: BackflushStart.fromJSON(object.backflushStart) }
        : isSet(object.backflushStop)
        ? { $case: "backflushStop", backflushStop: BackflushStop.fromJSON(object.backflushStop) }
        : isSet(object.rinseStart)
        ? { $case: "rinseStart", rinseStart: RinseStart.fromJSON(object.rinseStart) }
        : isSet(object.rinseStop)
        ? { $case: "rinseStop", rinseStop: RinseStop.fromJSON(object.rinseStop) }
        : undefined,
    };
  },

  toJSON(message: Command): unknown {
    const obj: any = {};
    message.requestId !== undefined &&
      (obj.requestId = base64FromBytes(message.requestId !== undefined ? message.requestId : new Uint8Array()));
    message.commandOneof?.$case === "powerOn" &&
      (obj.powerOn = message.commandOneof?.powerOn ? PowerOn.toJSON(message.commandOneof?.powerOn) : undefined);
    message.commandOneof?.$case === "powerOff" &&
      (obj.powerOff = message.commandOneof?.powerOff ? PowerOff.toJSON(message.commandOneof?.powerOff) : undefined);
    message.commandOneof?.$case === "startBrew" &&
      (obj.startBrew = message.commandOneof?.startBrew ? StartBrew.toJSON(message.commandOneof?.startBrew) : undefined);
    message.commandOneof?.$case === "stopBrew" &&
      (obj.stopBrew = message.commandOneof?.stopBrew ? StopBrew.toJSON(message.commandOneof?.stopBrew) : undefined);
    message.commandOneof?.$case === "startPump" &&
      (obj.startPump = message.commandOneof?.startPump ? StartPump.toJSON(message.commandOneof?.startPump) : undefined);
    message.commandOneof?.$case === "stopPump" &&
      (obj.stopPump = message.commandOneof?.stopPump ? StopPump.toJSON(message.commandOneof?.stopPump) : undefined);
    message.commandOneof?.$case === "startSteam" && (obj.startSteam = message.commandOneof?.startSteam
      ? StartSteam.toJSON(message.commandOneof?.startSteam)
      : undefined);
    message.commandOneof?.$case === "stopSteam" &&
      (obj.stopSteam = message.commandOneof?.stopSteam ? StopSteam.toJSON(message.commandOneof?.stopSteam) : undefined);
    message.commandOneof?.$case === "config" &&
      (obj.config = message.commandOneof?.config ? Config.toJSON(message.commandOneof?.config) : undefined);
    message.commandOneof?.$case === "backflushStart" && (obj.backflushStart = message.commandOneof?.backflushStart
      ? BackflushStart.toJSON(message.commandOneof?.backflushStart)
      : undefined);
    message.commandOneof?.$case === "backflushStop" && (obj.backflushStop = message.commandOneof?.backflushStop
      ? BackflushStop.toJSON(message.commandOneof?.backflushStop)
      : undefined);
    message.commandOneof?.$case === "rinseStart" && (obj.rinseStart = message.commandOneof?.rinseStart
      ? RinseStart.toJSON(message.commandOneof?.rinseStart)
      : undefined);
    message.commandOneof?.$case === "rinseStop" &&
      (obj.rinseStop = message.commandOneof?.rinseStop ? RinseStop.toJSON(message.commandOneof?.rinseStop) : undefined);
    return obj;
  },

  create<I extends Exact<DeepPartial<Command>, I>>(base?: I): Command {
    return Command.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<Command>, I>>(object: I): Command {
    const message = createBaseCommand();
    message.requestId = object.requestId ?? new Uint8Array();
    if (
      object.commandOneof?.$case === "powerOn" &&
      object.commandOneof?.powerOn !== undefined &&
      object.commandOneof?.powerOn !== null
    ) {
      message.commandOneof = { $case: "powerOn", powerOn: PowerOn.fromPartial(object.commandOneof.powerOn) };
    }
    if (
      object.commandOneof?.$case === "powerOff" &&
      object.commandOneof?.powerOff !== undefined &&
      object.commandOneof?.powerOff !== null
    ) {
      message.commandOneof = { $case: "powerOff", powerOff: PowerOff.fromPartial(object.commandOneof.powerOff) };
    }
    if (
      object.commandOneof?.$case === "startBrew" &&
      object.commandOneof?.startBrew !== undefined &&
      object.commandOneof?.startBrew !== null
    ) {
      message.commandOneof = { $case: "startBrew", startBrew: StartBrew.fromPartial(object.commandOneof.startBrew) };
    }
    if (
      object.commandOneof?.$case === "stopBrew" &&
      object.commandOneof?.stopBrew !== undefined &&
      object.commandOneof?.stopBrew !== null
    ) {
      message.commandOneof = { $case: "stopBrew", stopBrew: StopBrew.fromPartial(object.commandOneof.stopBrew) };
    }
    if (
      object.commandOneof?.$case === "startPump" &&
      object.commandOneof?.startPump !== undefined &&
      object.commandOneof?.startPump !== null
    ) {
      message.commandOneof = { $case: "startPump", startPump: StartPump.fromPartial(object.commandOneof.startPump) };
    }
    if (
      object.commandOneof?.$case === "stopPump" &&
      object.commandOneof?.stopPump !== undefined &&
      object.commandOneof?.stopPump !== null
    ) {
      message.commandOneof = { $case: "stopPump", stopPump: StopPump.fromPartial(object.commandOneof.stopPump) };
    }
    if (
      object.commandOneof?.$case === "startSteam" &&
      object.commandOneof?.startSteam !== undefined &&
      object.commandOneof?.startSteam !== null
    ) {
      message.commandOneof = {
        $case: "startSteam",
        startSteam: StartSteam.fromPartial(object.commandOneof.startSteam),
      };
    }
    if (
      object.commandOneof?.$case === "stopSteam" &&
      object.commandOneof?.stopSteam !== undefined &&
      object.commandOneof?.stopSteam !== null
    ) {
      message.commandOneof = { $case: "stopSteam", stopSteam: StopSteam.fromPartial(object.commandOneof.stopSteam) };
    }
    if (
      object.commandOneof?.$case === "config" &&
      object.commandOneof?.config !== undefined &&
      object.commandOneof?.config !== null
    ) {
      message.commandOneof = { $case: "config", config: Config.fromPartial(object.commandOneof.config) };
    }
    if (
      object.commandOneof?.$case === "backflushStart" &&
      object.commandOneof?.backflushStart !== undefined &&
      object.commandOneof?.backflushStart !== null
    ) {
      message.commandOneof = {
        $case: "backflushStart",
        backflushStart: BackflushStart.fromPartial(object.commandOneof.backflushStart),
      };
    }
    if (
      object.commandOneof?.$case === "backflushStop" &&
      object.commandOneof?.backflushStop !== undefined &&
      object.commandOneof?.backflushStop !== null
    ) {
      message.commandOneof = {
        $case: "backflushStop",
        backflushStop: BackflushStop.fromPartial(object.commandOneof.backflushStop),
      };
    }
    if (
      object.commandOneof?.$case === "rinseStart" &&
      object.commandOneof?.rinseStart !== undefined &&
      object.commandOneof?.rinseStart !== null
    ) {
      message.commandOneof = {
        $case: "rinseStart",
        rinseStart: RinseStart.fromPartial(object.commandOneof.rinseStart),
      };
    }
    if (
      object.commandOneof?.$case === "rinseStop" &&
      object.commandOneof?.rinseStop !== undefined &&
      object.commandOneof?.rinseStop !== null
    ) {
      message.commandOneof = { $case: "rinseStop", rinseStop: RinseStop.fromPartial(object.commandOneof.rinseStop) };
    }
    return message;
  },
};

function createBaseFloatSensorReading(): FloatSensorReading {
  return { valueOrError: undefined };
}

export const FloatSensorReading = {
  encode(message: FloatSensorReading, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    switch (message.valueOrError?.$case) {
      case "value":
        writer.uint32(9).double(message.valueOrError.value);
        break;
      case "error":
        writer.uint32(18).string(message.valueOrError.error);
        break;
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): FloatSensorReading {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseFloatSensorReading();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 9) {
            break;
          }

          message.valueOrError = { $case: "value", value: reader.double() };
          continue;
        case 2:
          if (tag != 18) {
            break;
          }

          message.valueOrError = { $case: "error", error: reader.string() };
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): FloatSensorReading {
    return {
      valueOrError: isSet(object.value)
        ? { $case: "value", value: Number(object.value) }
        : isSet(object.error)
        ? { $case: "error", error: String(object.error) }
        : undefined,
    };
  },

  toJSON(message: FloatSensorReading): unknown {
    const obj: any = {};
    message.valueOrError?.$case === "value" && (obj.value = message.valueOrError?.value);
    message.valueOrError?.$case === "error" && (obj.error = message.valueOrError?.error);
    return obj;
  },

  create<I extends Exact<DeepPartial<FloatSensorReading>, I>>(base?: I): FloatSensorReading {
    return FloatSensorReading.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<FloatSensorReading>, I>>(object: I): FloatSensorReading {
    const message = createBaseFloatSensorReading();
    if (
      object.valueOrError?.$case === "value" &&
      object.valueOrError?.value !== undefined &&
      object.valueOrError?.value !== null
    ) {
      message.valueOrError = { $case: "value", value: object.valueOrError.value };
    }
    if (
      object.valueOrError?.$case === "error" &&
      object.valueOrError?.error !== undefined &&
      object.valueOrError?.error !== null
    ) {
      message.valueOrError = { $case: "error", error: object.valueOrError.error };
    }
    return message;
  },
};

function createBaseShotTimer(): ShotTimer {
  return { start: 0, stop: 0 };
}

export const ShotTimer = {
  encode(message: ShotTimer, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.start !== 0) {
      writer.uint32(8).uint32(message.start);
    }
    if (message.stop !== 0) {
      writer.uint32(16).uint32(message.stop);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): ShotTimer {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseShotTimer();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 8) {
            break;
          }

          message.start = reader.uint32();
          continue;
        case 2:
          if (tag != 16) {
            break;
          }

          message.stop = reader.uint32();
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): ShotTimer {
    return {
      start: isSet(object.start) ? Number(object.start) : 0,
      stop: isSet(object.stop) ? Number(object.stop) : 0,
    };
  },

  toJSON(message: ShotTimer): unknown {
    const obj: any = {};
    message.start !== undefined && (obj.start = Math.round(message.start));
    message.stop !== undefined && (obj.stop = Math.round(message.stop));
    return obj;
  },

  create<I extends Exact<DeepPartial<ShotTimer>, I>>(base?: I): ShotTimer {
    return ShotTimer.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<ShotTimer>, I>>(object: I): ShotTimer {
    const message = createBaseShotTimer();
    message.start = object.start ?? 0;
    message.stop = object.stop ?? 0;
    return message;
  },
};

function createBaseStateUpdate(): StateUpdate {
  return { mode: 0, boilerTemp: undefined, pressure: undefined, setpoint: 0, millis: 0, shotTimer: undefined };
}

export const StateUpdate = {
  encode(message: StateUpdate, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.mode !== 0) {
      writer.uint32(8).int32(message.mode);
    }
    if (message.boilerTemp !== undefined) {
      FloatSensorReading.encode(message.boilerTemp, writer.uint32(18).fork()).ldelim();
    }
    if (message.pressure !== undefined) {
      FloatSensorReading.encode(message.pressure, writer.uint32(26).fork()).ldelim();
    }
    if (message.setpoint !== 0) {
      writer.uint32(32).int32(message.setpoint);
    }
    if (message.millis !== 0) {
      writer.uint32(40).uint32(message.millis);
    }
    if (message.shotTimer !== undefined) {
      ShotTimer.encode(message.shotTimer, writer.uint32(50).fork()).ldelim();
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): StateUpdate {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseStateUpdate();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 8) {
            break;
          }

          message.mode = reader.int32() as any;
          continue;
        case 2:
          if (tag != 18) {
            break;
          }

          message.boilerTemp = FloatSensorReading.decode(reader, reader.uint32());
          continue;
        case 3:
          if (tag != 26) {
            break;
          }

          message.pressure = FloatSensorReading.decode(reader, reader.uint32());
          continue;
        case 4:
          if (tag != 32) {
            break;
          }

          message.setpoint = reader.int32();
          continue;
        case 5:
          if (tag != 40) {
            break;
          }

          message.millis = reader.uint32();
          continue;
        case 6:
          if (tag != 50) {
            break;
          }

          message.shotTimer = ShotTimer.decode(reader, reader.uint32());
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): StateUpdate {
    return {
      mode: isSet(object.mode) ? machineModeFromJSON(object.mode) : 0,
      boilerTemp: isSet(object.boilerTemp) ? FloatSensorReading.fromJSON(object.boilerTemp) : undefined,
      pressure: isSet(object.pressure) ? FloatSensorReading.fromJSON(object.pressure) : undefined,
      setpoint: isSet(object.setpoint) ? Number(object.setpoint) : 0,
      millis: isSet(object.millis) ? Number(object.millis) : 0,
      shotTimer: isSet(object.shotTimer) ? ShotTimer.fromJSON(object.shotTimer) : undefined,
    };
  },

  toJSON(message: StateUpdate): unknown {
    const obj: any = {};
    message.mode !== undefined && (obj.mode = machineModeToJSON(message.mode));
    message.boilerTemp !== undefined &&
      (obj.boilerTemp = message.boilerTemp ? FloatSensorReading.toJSON(message.boilerTemp) : undefined);
    message.pressure !== undefined &&
      (obj.pressure = message.pressure ? FloatSensorReading.toJSON(message.pressure) : undefined);
    message.setpoint !== undefined && (obj.setpoint = Math.round(message.setpoint));
    message.millis !== undefined && (obj.millis = Math.round(message.millis));
    message.shotTimer !== undefined &&
      (obj.shotTimer = message.shotTimer ? ShotTimer.toJSON(message.shotTimer) : undefined);
    return obj;
  },

  create<I extends Exact<DeepPartial<StateUpdate>, I>>(base?: I): StateUpdate {
    return StateUpdate.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<StateUpdate>, I>>(object: I): StateUpdate {
    const message = createBaseStateUpdate();
    message.mode = object.mode ?? 0;
    message.boilerTemp = (object.boilerTemp !== undefined && object.boilerTemp !== null)
      ? FloatSensorReading.fromPartial(object.boilerTemp)
      : undefined;
    message.pressure = (object.pressure !== undefined && object.pressure !== null)
      ? FloatSensorReading.fromPartial(object.pressure)
      : undefined;
    message.setpoint = object.setpoint ?? 0;
    message.millis = object.millis ?? 0;
    message.shotTimer = (object.shotTimer !== undefined && object.shotTimer !== null)
      ? ShotTimer.fromPartial(object.shotTimer)
      : undefined;
    return message;
  },
};

function createBaseLogMessage(): LogMessage {
  return { logLevel: 0, msg: "" };
}

export const LogMessage = {
  encode(message: LogMessage, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.logLevel !== 0) {
      writer.uint32(8).int32(message.logLevel);
    }
    if (message.msg !== "") {
      writer.uint32(18).string(message.msg);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): LogMessage {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseLogMessage();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 8) {
            break;
          }

          message.logLevel = reader.int32() as any;
          continue;
        case 2:
          if (tag != 18) {
            break;
          }

          message.msg = reader.string();
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): LogMessage {
    return {
      logLevel: isSet(object.logLevel) ? logMessage_LogLevelFromJSON(object.logLevel) : 0,
      msg: isSet(object.msg) ? String(object.msg) : "",
    };
  },

  toJSON(message: LogMessage): unknown {
    const obj: any = {};
    message.logLevel !== undefined && (obj.logLevel = logMessage_LogLevelToJSON(message.logLevel));
    message.msg !== undefined && (obj.msg = message.msg);
    return obj;
  },

  create<I extends Exact<DeepPartial<LogMessage>, I>>(base?: I): LogMessage {
    return LogMessage.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<LogMessage>, I>>(object: I): LogMessage {
    const message = createBaseLogMessage();
    message.logLevel = object.logLevel ?? 0;
    message.msg = object.msg ?? "";
    return message;
  },
};

function createBaseEvent(): Event {
  return { requestId: new Uint8Array(), eventOneof: undefined };
}

export const Event = {
  encode(message: Event, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.requestId.length !== 0) {
      writer.uint32(10).bytes(message.requestId);
    }
    switch (message.eventOneof?.$case) {
      case "stateUpdate":
        StateUpdate.encode(message.eventOneof.stateUpdate, writer.uint32(18).fork()).ldelim();
        break;
      case "config":
        Config.encode(message.eventOneof.config, writer.uint32(26).fork()).ldelim();
        break;
      case "log":
        LogMessage.encode(message.eventOneof.log, writer.uint32(34).fork()).ldelim();
        break;
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): Event {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseEvent();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 10) {
            break;
          }

          message.requestId = reader.bytes();
          continue;
        case 2:
          if (tag != 18) {
            break;
          }

          message.eventOneof = { $case: "stateUpdate", stateUpdate: StateUpdate.decode(reader, reader.uint32()) };
          continue;
        case 3:
          if (tag != 26) {
            break;
          }

          message.eventOneof = { $case: "config", config: Config.decode(reader, reader.uint32()) };
          continue;
        case 4:
          if (tag != 34) {
            break;
          }

          message.eventOneof = { $case: "log", log: LogMessage.decode(reader, reader.uint32()) };
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): Event {
    return {
      requestId: isSet(object.requestId) ? bytesFromBase64(object.requestId) : new Uint8Array(),
      eventOneof: isSet(object.stateUpdate)
        ? { $case: "stateUpdate", stateUpdate: StateUpdate.fromJSON(object.stateUpdate) }
        : isSet(object.config)
        ? { $case: "config", config: Config.fromJSON(object.config) }
        : isSet(object.log)
        ? { $case: "log", log: LogMessage.fromJSON(object.log) }
        : undefined,
    };
  },

  toJSON(message: Event): unknown {
    const obj: any = {};
    message.requestId !== undefined &&
      (obj.requestId = base64FromBytes(message.requestId !== undefined ? message.requestId : new Uint8Array()));
    message.eventOneof?.$case === "stateUpdate" && (obj.stateUpdate = message.eventOneof?.stateUpdate
      ? StateUpdate.toJSON(message.eventOneof?.stateUpdate)
      : undefined);
    message.eventOneof?.$case === "config" &&
      (obj.config = message.eventOneof?.config ? Config.toJSON(message.eventOneof?.config) : undefined);
    message.eventOneof?.$case === "log" &&
      (obj.log = message.eventOneof?.log ? LogMessage.toJSON(message.eventOneof?.log) : undefined);
    return obj;
  },

  create<I extends Exact<DeepPartial<Event>, I>>(base?: I): Event {
    return Event.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<Event>, I>>(object: I): Event {
    const message = createBaseEvent();
    message.requestId = object.requestId ?? new Uint8Array();
    if (
      object.eventOneof?.$case === "stateUpdate" &&
      object.eventOneof?.stateUpdate !== undefined &&
      object.eventOneof?.stateUpdate !== null
    ) {
      message.eventOneof = {
        $case: "stateUpdate",
        stateUpdate: StateUpdate.fromPartial(object.eventOneof.stateUpdate),
      };
    }
    if (
      object.eventOneof?.$case === "config" &&
      object.eventOneof?.config !== undefined &&
      object.eventOneof?.config !== null
    ) {
      message.eventOneof = { $case: "config", config: Config.fromPartial(object.eventOneof.config) };
    }
    if (object.eventOneof?.$case === "log" && object.eventOneof?.log !== undefined && object.eventOneof?.log !== null) {
      message.eventOneof = { $case: "log", log: LogMessage.fromPartial(object.eventOneof.log) };
    }
    return message;
  },
};

declare var self: any | undefined;
declare var window: any | undefined;
declare var global: any | undefined;
var tsProtoGlobalThis: any = (() => {
  if (typeof globalThis !== "undefined") {
    return globalThis;
  }
  if (typeof self !== "undefined") {
    return self;
  }
  if (typeof window !== "undefined") {
    return window;
  }
  if (typeof global !== "undefined") {
    return global;
  }
  throw "Unable to locate global object";
})();

function bytesFromBase64(b64: string): Uint8Array {
  if (tsProtoGlobalThis.Buffer) {
    return Uint8Array.from(tsProtoGlobalThis.Buffer.from(b64, "base64"));
  } else {
    const bin = tsProtoGlobalThis.atob(b64);
    const arr = new Uint8Array(bin.length);
    for (let i = 0; i < bin.length; ++i) {
      arr[i] = bin.charCodeAt(i);
    }
    return arr;
  }
}

function base64FromBytes(arr: Uint8Array): string {
  if (tsProtoGlobalThis.Buffer) {
    return tsProtoGlobalThis.Buffer.from(arr).toString("base64");
  } else {
    const bin: string[] = [];
    arr.forEach((byte) => {
      bin.push(String.fromCharCode(byte));
    });
    return tsProtoGlobalThis.btoa(bin.join(""));
  }
}

type Builtin = Date | Function | Uint8Array | string | number | boolean | undefined;

export type DeepPartial<T> = T extends Builtin ? T
  : T extends Array<infer U> ? Array<DeepPartial<U>> : T extends ReadonlyArray<infer U> ? ReadonlyArray<DeepPartial<U>>
  : T extends { $case: string } ? { [K in keyof Omit<T, "$case">]?: DeepPartial<T[K]> } & { $case: T["$case"] }
  : T extends {} ? { [K in keyof T]?: DeepPartial<T[K]> }
  : Partial<T>;

type KeysOfUnion<T> = T extends T ? keyof T : never;
export type Exact<P, I extends P> = P extends Builtin ? P
  : P & { [K in keyof P]: Exact<P[K], I[K]> } & { [K in Exclude<keyof I, KeysOfUnion<P>>]: never };

function isSet(value: any): boolean {
  return value !== null && value !== undefined;
}
