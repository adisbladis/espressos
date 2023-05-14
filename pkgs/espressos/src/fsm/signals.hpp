#pragma once

#include <cstdint>

#include "../lib/signal.hpp"
#include "../proto/api.h"
#include "pump.hpp"

// Signals are sideeffectful singleton values shared by all state machines
namespace MachineSignals {
extern Signal<bool> solenoid;

// Boiler setpoint
extern Signal<std::uint16_t> setpoint;

// Current FSM timestamp
extern Signal<unsigned long> timestamp;

// Current FSM pressure
extern Signal<std::uint16_t> pressure;

// Current FSM temp
extern Signal<std::uint16_t> temp;

// How often to send state updates
extern Signal<unsigned long> stateUpdateInterval;

// Pump target
extern Signal<PumpTarget> pump;

// Overall mode
extern Signal<MachineMode> mode;

// Shot timer
extern Signal<unsigned long> shotStartTime;
extern Signal<unsigned long> shotStopTime;

} // namespace MachineSignals
