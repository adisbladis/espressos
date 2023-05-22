#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include <Time.hpp>

struct Timer {
  Timestamp_t last;
  Timestamp_t interval;
  std::function<void(Timestamp_t)> callback;
};

struct Timeout {
  Timestamp_t at;
  std::function<void()> callback;
  std::function<void()> cancel;
};

typedef std::shared_ptr<Timeout> Timeout_t;

// Provides a way to run a function at an interval
class Timers {
private:
  std::vector<std::function<void(Timestamp_t)>> funcs;
  std::set<Timeout_t> timeouts;
  Timestamp_t lastNow;

public:
  Timers() : lastNow(0){};

  std::shared_ptr<Timer> createInterval(Timestamp_t interval,
                                        std::function<void()> callback) {
    return createInterval(interval, [=](Timestamp_t) { callback(); });
  };

  // Create a timer that's called at a certain interval.
  // If the interval is 0 it will be called on every loop cycle.
  std::shared_ptr<Timer>
  createInterval(Timestamp_t interval,
                 std::function<void(Timestamp_t)> callback) {
    std::shared_ptr<Timer> timer =
        std::make_shared<Timer>((Timer){0, interval, callback});

    funcs.push_back([=](Timestamp_t now) mutable {
      if (now < (timer->last + timer->interval)) {
        return;
      }

      timer->callback(now);

      timer->last = now;
    });

    return timer;
  };

  Timeout_t setTimeout(Timestamp_t timeoutMs, std::function<void()> callback) {
    Timestamp_t at = lastNow + timeoutMs;

    Timeout_t timeout = std::make_shared<Timeout>((Timeout){
        at, callback, [this, &timeout]() { timeouts.erase(timeout); }});

    timeouts.insert(timeout);

    return timeout;
  };

  void loop(Timestamp_t now) {
    lastNow = now;

    // Run intervals
    for (const auto &cb : funcs) {
      cb(now);
    }

    // Run timeouts
    if (timeouts.size() > 0) {
      std::vector<Timeout_t> triggeredTimeouts;

      for (const auto &timeout : timeouts) {
        if (now >= timeout->at) {
          timeout->callback();
          triggeredTimeouts.push_back(timeout);
        };
      }

      for (const auto &timeout : triggeredTimeouts) {
        timeouts.erase(timeout);
      }
    }
  };
};
