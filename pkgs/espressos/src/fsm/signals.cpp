#include "signals.hpp"

namespace MachineSignals {
Signal<bool> solenoid(false);
Signal<std::uint16_t> setpoint(0);
Signal<unsigned long> timestamp(0);
Signal<std::uint16_t> pressure(0);
Signal<std::uint16_t> temp(0);
Signal<unsigned long> stateUpdateInterval(STATE_UPDATE_INTERVAL);
Signal<PumpTarget> pump((PumpTarget){PumpMode::POWER, 0});
Signal<MachineMode> mode(MachineMode::OFF);
Signal<unsigned long> shotStartTime(0);
Signal<unsigned long> shotStopTime(0);
} // namespace MachineSignals
