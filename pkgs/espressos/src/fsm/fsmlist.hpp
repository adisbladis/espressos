#pragma once

#include "machine.hpp"
#include <tinyfsm.hpp>

using fsm_list = tinyfsm::FsmList<MachineState>;

// Dispatch event to all fsm_list classes
template <typename E> void send_event(E const &event) {
  fsm_list::template dispatch<E>(event);
}
