#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

struct Timer {
  unsigned long last;
  unsigned long interval;
  std::function<void(unsigned long)> callback;
};

struct Timeout {
  unsigned long at;
  std::function<void()> callback;
};

// Provides a way to run a function at an interval
class Timers {
private:
  std::vector<std::function<void(unsigned long)>> funcs;
  std::set<std::shared_ptr<Timeout>> timeouts;
  unsigned long lastNow;

public:
  Timers() : lastNow(0){};

  std::shared_ptr<Timer> createInterval(unsigned long interval,
                                        std::function<void()> callback) {
    return createInterval(interval, [=](unsigned long) { callback(); });
  };

  // Create a timer that's called at a certain interval.
  // If the interval is 0 it will be called on every loop cycle.
  std::shared_ptr<Timer>
  createInterval(unsigned long interval,
                 std::function<void(unsigned long)> callback) {
    std::shared_ptr<Timer> timer =
        std::make_shared<Timer>((Timer){0, interval, callback});

    funcs.push_back([=](unsigned long now) mutable {
      if (now < (timer->last + timer->interval)) {
        return;
      }

      timer->callback(now);

      timer->last = now;
    });

    return timer;
  };

  std::shared_ptr<Timeout> setTimeout(unsigned long timeoutMs,
                                      std::function<void()> callback) {
    unsigned long at = lastNow + timeoutMs;

    std::shared_ptr<Timeout> timeout =
        std::make_shared<Timeout>((Timeout){at, callback});

    timeouts.insert(timeout);

    return timeout;
  };

  void loop(unsigned long now) {
    lastNow = now;

    // Run intervals
    for (auto cb : funcs) {
      cb(now);
    }

    // Run timeouts
    if (timeouts.size() > 0) {
      std::vector<std::shared_ptr<Timeout>> triggeredTimeouts;

      for (auto timeout : timeouts) {
        if (now >= timeout->at) {
          timeout->callback();
          triggeredTimeouts.push_back(timeout);
        };
      }

      for (auto timeout : triggeredTimeouts) {
        timeouts.erase(timeout);
      }
    }
  };
};
