#pragma once

#include "backflush.hpp"
#include "machine.hpp"
#include <tinyfsm.hpp>

using fsm_list = tinyfsm::FsmList<MachineState, BackflushState>;

// Dispatch event to all fsm_list classes
template <typename E> void send_event(E const &event) {
  fsm_list::template dispatch<E>(event);
}
