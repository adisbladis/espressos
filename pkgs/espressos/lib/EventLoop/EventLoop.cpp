#include "EventLoop.hpp"
#include <Timers.hpp>

Timers &getEventLoop() {
  static Timers timers;
  return timers;
}
