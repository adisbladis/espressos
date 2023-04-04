/* eslint-disable */
import * as _m0 from "protobufjs/minimal";

export const protobufPackage = "";

export interface PowerOn {
}

export interface PowerOff {
}

export interface StartBrew {
}

export interface StopBrew {
}

export interface StartSteam {
}

export interface StopSteam {
}

export interface SetSteamSetPoint {
}

export interface SetBoilerSetPoint {
}

export interface Command {
  /**
   * A unique client-generated request identifier.
   * This will be embedded into a command response coming from the server
   * so the client can route the response appropriately.
   */
  requestId: string;
  powerOn?: PowerOn | undefined;
  powerOff?: PowerOff | undefined;
  startBrew?: StartBrew | undefined;
  stopBrew?: StopBrew | undefined;
  startSteam?: StartSteam | undefined;
  stopSteam?: StopSteam | undefined;
  setSteamSetPoint?: SetSteamSetPoint | undefined;
  setBoilerSetPoint?: SetBoilerSetPoint | undefined;
}

function createBasePowerOn(): PowerOn {
  return {};
}

export const PowerOn = {
  encode(_: PowerOn, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): PowerOn {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBasePowerOn();
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

  fromJSON(_: any): PowerOn {
    return {};
  },

  toJSON(_: PowerOn): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<PowerOn>, I>>(base?: I): PowerOn {
    return PowerOn.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<PowerOn>, I>>(_: I): PowerOn {
    const message = createBasePowerOn();
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

function createBaseStartSteam(): StartSteam {
  return {};
}

export const StartSteam = {
  encode(_: StartSteam, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): StartSteam {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseStartSteam();
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

  fromJSON(_: any): StartSteam {
    return {};
  },

  toJSON(_: StartSteam): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<StartSteam>, I>>(base?: I): StartSteam {
    return StartSteam.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<StartSteam>, I>>(_: I): StartSteam {
    const message = createBaseStartSteam();
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

function createBaseSetSteamSetPoint(): SetSteamSetPoint {
  return {};
}

export const SetSteamSetPoint = {
  encode(_: SetSteamSetPoint, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): SetSteamSetPoint {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseSetSteamSetPoint();
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

  fromJSON(_: any): SetSteamSetPoint {
    return {};
  },

  toJSON(_: SetSteamSetPoint): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<SetSteamSetPoint>, I>>(base?: I): SetSteamSetPoint {
    return SetSteamSetPoint.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<SetSteamSetPoint>, I>>(_: I): SetSteamSetPoint {
    const message = createBaseSetSteamSetPoint();
    return message;
  },
};

function createBaseSetBoilerSetPoint(): SetBoilerSetPoint {
  return {};
}

export const SetBoilerSetPoint = {
  encode(_: SetBoilerSetPoint, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): SetBoilerSetPoint {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseSetBoilerSetPoint();
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

  fromJSON(_: any): SetBoilerSetPoint {
    return {};
  },

  toJSON(_: SetBoilerSetPoint): unknown {
    const obj: any = {};
    return obj;
  },

  create<I extends Exact<DeepPartial<SetBoilerSetPoint>, I>>(base?: I): SetBoilerSetPoint {
    return SetBoilerSetPoint.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<SetBoilerSetPoint>, I>>(_: I): SetBoilerSetPoint {
    const message = createBaseSetBoilerSetPoint();
    return message;
  },
};

function createBaseCommand(): Command {
  return {
    requestId: "",
    powerOn: undefined,
    powerOff: undefined,
    startBrew: undefined,
    stopBrew: undefined,
    startSteam: undefined,
    stopSteam: undefined,
    setSteamSetPoint: undefined,
    setBoilerSetPoint: undefined,
  };
}

export const Command = {
  encode(message: Command, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.requestId !== "") {
      writer.uint32(10).string(message.requestId);
    }
    if (message.powerOn !== undefined) {
      PowerOn.encode(message.powerOn, writer.uint32(18).fork()).ldelim();
    }
    if (message.powerOff !== undefined) {
      PowerOff.encode(message.powerOff, writer.uint32(26).fork()).ldelim();
    }
    if (message.startBrew !== undefined) {
      StartBrew.encode(message.startBrew, writer.uint32(34).fork()).ldelim();
    }
    if (message.stopBrew !== undefined) {
      StopBrew.encode(message.stopBrew, writer.uint32(42).fork()).ldelim();
    }
    if (message.startSteam !== undefined) {
      StartSteam.encode(message.startSteam, writer.uint32(50).fork()).ldelim();
    }
    if (message.stopSteam !== undefined) {
      StopSteam.encode(message.stopSteam, writer.uint32(58).fork()).ldelim();
    }
    if (message.setSteamSetPoint !== undefined) {
      SetSteamSetPoint.encode(message.setSteamSetPoint, writer.uint32(66).fork()).ldelim();
    }
    if (message.setBoilerSetPoint !== undefined) {
      SetBoilerSetPoint.encode(message.setBoilerSetPoint, writer.uint32(74).fork()).ldelim();
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

          message.requestId = reader.string();
          continue;
        case 2:
          if (tag != 18) {
            break;
          }

          message.powerOn = PowerOn.decode(reader, reader.uint32());
          continue;
        case 3:
          if (tag != 26) {
            break;
          }

          message.powerOff = PowerOff.decode(reader, reader.uint32());
          continue;
        case 4:
          if (tag != 34) {
            break;
          }

          message.startBrew = StartBrew.decode(reader, reader.uint32());
          continue;
        case 5:
          if (tag != 42) {
            break;
          }

          message.stopBrew = StopBrew.decode(reader, reader.uint32());
          continue;
        case 6:
          if (tag != 50) {
            break;
          }

          message.startSteam = StartSteam.decode(reader, reader.uint32());
          continue;
        case 7:
          if (tag != 58) {
            break;
          }

          message.stopSteam = StopSteam.decode(reader, reader.uint32());
          continue;
        case 8:
          if (tag != 66) {
            break;
          }

          message.setSteamSetPoint = SetSteamSetPoint.decode(reader, reader.uint32());
          continue;
        case 9:
          if (tag != 74) {
            break;
          }

          message.setBoilerSetPoint = SetBoilerSetPoint.decode(reader, reader.uint32());
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
      requestId: isSet(object.requestId) ? String(object.requestId) : "",
      powerOn: isSet(object.powerOn) ? PowerOn.fromJSON(object.powerOn) : undefined,
      powerOff: isSet(object.powerOff) ? PowerOff.fromJSON(object.powerOff) : undefined,
      startBrew: isSet(object.startBrew) ? StartBrew.fromJSON(object.startBrew) : undefined,
      stopBrew: isSet(object.stopBrew) ? StopBrew.fromJSON(object.stopBrew) : undefined,
      startSteam: isSet(object.startSteam) ? StartSteam.fromJSON(object.startSteam) : undefined,
      stopSteam: isSet(object.stopSteam) ? StopSteam.fromJSON(object.stopSteam) : undefined,
      setSteamSetPoint: isSet(object.setSteamSetPoint) ? SetSteamSetPoint.fromJSON(object.setSteamSetPoint) : undefined,
      setBoilerSetPoint: isSet(object.setBoilerSetPoint)
        ? SetBoilerSetPoint.fromJSON(object.setBoilerSetPoint)
        : undefined,
    };
  },

  toJSON(message: Command): unknown {
    const obj: any = {};
    message.requestId !== undefined && (obj.requestId = message.requestId);
    message.powerOn !== undefined && (obj.powerOn = message.powerOn ? PowerOn.toJSON(message.powerOn) : undefined);
    message.powerOff !== undefined && (obj.powerOff = message.powerOff ? PowerOff.toJSON(message.powerOff) : undefined);
    message.startBrew !== undefined &&
      (obj.startBrew = message.startBrew ? StartBrew.toJSON(message.startBrew) : undefined);
    message.stopBrew !== undefined && (obj.stopBrew = message.stopBrew ? StopBrew.toJSON(message.stopBrew) : undefined);
    message.startSteam !== undefined &&
      (obj.startSteam = message.startSteam ? StartSteam.toJSON(message.startSteam) : undefined);
    message.stopSteam !== undefined &&
      (obj.stopSteam = message.stopSteam ? StopSteam.toJSON(message.stopSteam) : undefined);
    message.setSteamSetPoint !== undefined &&
      (obj.setSteamSetPoint = message.setSteamSetPoint ? SetSteamSetPoint.toJSON(message.setSteamSetPoint) : undefined);
    message.setBoilerSetPoint !== undefined && (obj.setBoilerSetPoint = message.setBoilerSetPoint
      ? SetBoilerSetPoint.toJSON(message.setBoilerSetPoint)
      : undefined);
    return obj;
  },

  create<I extends Exact<DeepPartial<Command>, I>>(base?: I): Command {
    return Command.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<Command>, I>>(object: I): Command {
    const message = createBaseCommand();
    message.requestId = object.requestId ?? "";
    message.powerOn = (object.powerOn !== undefined && object.powerOn !== null)
      ? PowerOn.fromPartial(object.powerOn)
      : undefined;
    message.powerOff = (object.powerOff !== undefined && object.powerOff !== null)
      ? PowerOff.fromPartial(object.powerOff)
      : undefined;
    message.startBrew = (object.startBrew !== undefined && object.startBrew !== null)
      ? StartBrew.fromPartial(object.startBrew)
      : undefined;
    message.stopBrew = (object.stopBrew !== undefined && object.stopBrew !== null)
      ? StopBrew.fromPartial(object.stopBrew)
      : undefined;
    message.startSteam = (object.startSteam !== undefined && object.startSteam !== null)
      ? StartSteam.fromPartial(object.startSteam)
      : undefined;
    message.stopSteam = (object.stopSteam !== undefined && object.stopSteam !== null)
      ? StopSteam.fromPartial(object.stopSteam)
      : undefined;
    message.setSteamSetPoint = (object.setSteamSetPoint !== undefined && object.setSteamSetPoint !== null)
      ? SetSteamSetPoint.fromPartial(object.setSteamSetPoint)
      : undefined;
    message.setBoilerSetPoint = (object.setBoilerSetPoint !== undefined && object.setBoilerSetPoint !== null)
      ? SetBoilerSetPoint.fromPartial(object.setBoilerSetPoint)
      : undefined;
    return message;
  },
};

type Builtin = Date | Function | Uint8Array | string | number | boolean | undefined;

export type DeepPartial<T> = T extends Builtin ? T
  : T extends Array<infer U> ? Array<DeepPartial<U>> : T extends ReadonlyArray<infer U> ? ReadonlyArray<DeepPartial<U>>
  : T extends {} ? { [K in keyof T]?: DeepPartial<T[K]> }
  : Partial<T>;

type KeysOfUnion<T> = T extends T ? keyof T : never;
export type Exact<P, I extends P> = P extends Builtin ? P
  : P & { [K in keyof P]: Exact<P[K], I[K]> } & { [K in Exclude<keyof I, KeysOfUnion<P>>]: never };

function isSet(value: any): boolean {
  return value !== null && value !== undefined;
}
