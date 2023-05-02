#pragma once

#include <tinyfsm.hpp>

#include "backflush.hpp"
#include "brew.hpp"
#include "machine.hpp"
#include "rinse.hpp"

using fsm_list =
    tinyfsm::FsmList<MachineState, BackflushState, BrewState, RinseState>;

// Dispatch event to all fsm_list classes
template <typename E> void send_event(E const &event) {
  fsm_list::template dispatch<E>(event);
}
