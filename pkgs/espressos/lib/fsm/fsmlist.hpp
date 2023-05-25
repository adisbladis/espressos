#pragma once

#include <tinyfsm.hpp>

// On the RPi Pico W & friends using pico-sdk we might be running the state
// transition on another core. Therefore we use a recursive mutex where each
// core is considered an owner.
#if defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED)
#include <pico/mutex.h>
auto_init_recursive_mutex(send_event_mutex);
#endif

#include "backflush.hpp"
#include "brew.hpp"
#include "machine.hpp"
#include "pump.hpp"
#include "rinse.hpp"
#include "signals.hpp"
#include "steam.hpp"

using fsm_list =
    tinyfsm::FsmList<MachineState, BackflushState, BrewState, RinseState,
                     MachineSignals, PumpState, SteamState>;

// Dispatch event to all fsm_list classes
template <typename E> void send_event(E const &event) {
#if defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED)
  recursive_mutex_enter_blocking(&send_event_mutex);
#endif

  fsm_list::template dispatch<E>(event);

#if defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED)
  recursive_mutex_exit(&send_event_mutex);
#endif
}
