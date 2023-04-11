/* eslint-disable */
import * as _m0 from "protobufjs/minimal";

export const protobufPackage = "";

/** PID gain tunings. */
export interface PIDTunings {
  /** Proportional gain. */
  P: number;
  /** Integral gain. */
  I: number;
  /** Derivative gain. */
  D: number;
}

/** Hardware configuration for the boiler. */
export interface BoilerConfig {
  /** PID tunings for this boiler. */
  PID:
    | PIDTunings
    | undefined;
  /**
   * Value of Rref resistor.
   * PT100 == 430.0
   * PT1000 == 4300.0
   */
  rref: number;
  /**
   * "Nominal" 0-degrees resistance of the sensor.
   * PT100 == 100.0
   * PT1000 == 1000.0
   */
  rnominal: number;
}

/** Persisted configuration. */
export interface Config {
  boiler: BoilerConfig | undefined;
  setpoint: number;
  steamSetPoint: number;
}

function createBasePIDTunings(): PIDTunings {
  return { P: 0, I: 0, D: 0 };
}

export const PIDTunings = {
  encode(message: PIDTunings, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.P !== 0) {
      writer.uint32(9).double(message.P);
    }
    if (message.I !== 0) {
      writer.uint32(17).double(message.I);
    }
    if (message.D !== 0) {
      writer.uint32(25).double(message.D);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): PIDTunings {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBasePIDTunings();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 9) {
            break;
          }

          message.P = reader.double();
          continue;
        case 2:
          if (tag != 17) {
            break;
          }

          message.I = reader.double();
          continue;
        case 3:
          if (tag != 25) {
            break;
          }

          message.D = reader.double();
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): PIDTunings {
    return {
      P: isSet(object.P) ? Number(object.P) : 0,
      I: isSet(object.I) ? Number(object.I) : 0,
      D: isSet(object.D) ? Number(object.D) : 0,
    };
  },

  toJSON(message: PIDTunings): unknown {
    const obj: any = {};
    message.P !== undefined && (obj.P = message.P);
    message.I !== undefined && (obj.I = message.I);
    message.D !== undefined && (obj.D = message.D);
    return obj;
  },

  create<I extends Exact<DeepPartial<PIDTunings>, I>>(base?: I): PIDTunings {
    return PIDTunings.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<PIDTunings>, I>>(object: I): PIDTunings {
    const message = createBasePIDTunings();
    message.P = object.P ?? 0;
    message.I = object.I ?? 0;
    message.D = object.D ?? 0;
    return message;
  },
};

function createBaseBoilerConfig(): BoilerConfig {
  return { PID: undefined, rref: 0, rnominal: 0 };
}

export const BoilerConfig = {
  encode(message: BoilerConfig, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.PID !== undefined) {
      PIDTunings.encode(message.PID, writer.uint32(10).fork()).ldelim();
    }
    if (message.rref !== 0) {
      writer.uint32(17).double(message.rref);
    }
    if (message.rnominal !== 0) {
      writer.uint32(25).double(message.rnominal);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): BoilerConfig {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseBoilerConfig();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 10) {
            break;
          }

          message.PID = PIDTunings.decode(reader, reader.uint32());
          continue;
        case 2:
          if (tag != 17) {
            break;
          }

          message.rref = reader.double();
          continue;
        case 3:
          if (tag != 25) {
            break;
          }

          message.rnominal = reader.double();
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): BoilerConfig {
    return {
      PID: isSet(object.PID) ? PIDTunings.fromJSON(object.PID) : undefined,
      rref: isSet(object.rref) ? Number(object.rref) : 0,
      rnominal: isSet(object.rnominal) ? Number(object.rnominal) : 0,
    };
  },

  toJSON(message: BoilerConfig): unknown {
    const obj: any = {};
    message.PID !== undefined && (obj.PID = message.PID ? PIDTunings.toJSON(message.PID) : undefined);
    message.rref !== undefined && (obj.rref = message.rref);
    message.rnominal !== undefined && (obj.rnominal = message.rnominal);
    return obj;
  },

  create<I extends Exact<DeepPartial<BoilerConfig>, I>>(base?: I): BoilerConfig {
    return BoilerConfig.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<BoilerConfig>, I>>(object: I): BoilerConfig {
    const message = createBaseBoilerConfig();
    message.PID = (object.PID !== undefined && object.PID !== null) ? PIDTunings.fromPartial(object.PID) : undefined;
    message.rref = object.rref ?? 0;
    message.rnominal = object.rnominal ?? 0;
    return message;
  },
};

function createBaseConfig(): Config {
  return { boiler: undefined, setpoint: 0, steamSetPoint: 0 };
}

export const Config = {
  encode(message: Config, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.boiler !== undefined) {
      BoilerConfig.encode(message.boiler, writer.uint32(10).fork()).ldelim();
    }
    if (message.setpoint !== 0) {
      writer.uint32(17).double(message.setpoint);
    }
    if (message.steamSetPoint !== 0) {
      writer.uint32(25).double(message.steamSetPoint);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): Config {
    const reader = input instanceof _m0.Reader ? input : _m0.Reader.create(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = createBaseConfig();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if (tag != 10) {
            break;
          }

          message.boiler = BoilerConfig.decode(reader, reader.uint32());
          continue;
        case 2:
          if (tag != 17) {
            break;
          }

          message.setpoint = reader.double();
          continue;
        case 3:
          if (tag != 25) {
            break;
          }

          message.steamSetPoint = reader.double();
          continue;
      }
      if ((tag & 7) == 4 || tag == 0) {
        break;
      }
      reader.skipType(tag & 7);
    }
    return message;
  },

  fromJSON(object: any): Config {
    return {
      boiler: isSet(object.boiler) ? BoilerConfig.fromJSON(object.boiler) : undefined,
      setpoint: isSet(object.setpoint) ? Number(object.setpoint) : 0,
      steamSetPoint: isSet(object.steamSetPoint) ? Number(object.steamSetPoint) : 0,
    };
  },

  toJSON(message: Config): unknown {
    const obj: any = {};
    message.boiler !== undefined && (obj.boiler = message.boiler ? BoilerConfig.toJSON(message.boiler) : undefined);
    message.setpoint !== undefined && (obj.setpoint = message.setpoint);
    message.steamSetPoint !== undefined && (obj.steamSetPoint = message.steamSetPoint);
    return obj;
  },

  create<I extends Exact<DeepPartial<Config>, I>>(base?: I): Config {
    return Config.fromPartial(base ?? {});
  },

  fromPartial<I extends Exact<DeepPartial<Config>, I>>(object: I): Config {
    const message = createBaseConfig();
    message.boiler = (object.boiler !== undefined && object.boiler !== null)
      ? BoilerConfig.fromPartial(object.boiler)
      : undefined;
    message.setpoint = object.setpoint ?? 0;
    message.steamSetPoint = object.steamSetPoint ?? 0;
    return message;
  },
};

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
