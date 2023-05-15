#include "signals.hpp"

Signal<bool> MachineSignals::solenoid(false);
Signal<std::uint16_t> MachineSignals::setpoint(0);
Signal<unsigned long> MachineSignals::timestamp(0);
Signal<std::uint16_t> MachineSignals::pressure(0);
Signal<std::uint16_t> MachineSignals::temp(0);
Signal<unsigned long>
    MachineSignals::stateUpdateInterval(STATE_UPDATE_INTERVAL);
Signal<PumpTarget> MachineSignals::pump((PumpTarget){PumpMode::POWER, 0});
Signal<MachineMode> MachineSignals::mode(MachineMode::OFF);
Signal<unsigned long> MachineSignals::shotStartTime(0);
Signal<unsigned long> MachineSignals::shotStopTime(0);

class MachineSignalsListening : public MachineSignals { };

FSM_INITIAL_STATE(MachineSignals, MachineSignalsListening)
