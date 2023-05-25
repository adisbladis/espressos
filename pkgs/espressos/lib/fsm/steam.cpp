#include <tinyfsm.hpp>

#include <EventLoop.hpp>
#include <Time.hpp>

#include "brew.hpp"
#include "events.hpp"
#include "fsmlist.hpp"
#include "signals.hpp"

class SteamActive;

class SteamDone : public SteamState {
  void react(SteamStartingEvent const &e) override { transit<SteamActive>(); }
};

class SteamActive : public SteamState {
  void entry() override {
    MachineSignals::setpoint = MachineSignals::config.get().get_steamSetPoint();

    timeout = getEventLoop().setTimeout(STEAM_TIMEOUT,
                                        []() { send_event(SteamStopEvent()); });
  };

  void exit() override {
    MachineSignals::setpoint = MachineSignals::config.get().get_setpoint();
    timeout->cancel();
  };

protected:
  static Timeout_t timeout;
};

void SteamState::react(SteamStoppingEvent const &e) { transit<SteamDone>(); };

Timeout_t SteamActive::timeout = getEventLoop().setTimeout(0, []() {});

FSM_INITIAL_STATE(SteamState, SteamDone)
