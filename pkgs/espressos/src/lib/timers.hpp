#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <iostream>

struct Timer {
  unsigned long last;
  unsigned long interval;
  std::function<void(unsigned long)> callback;
};

// Provides a way to run a function at an interval
class Timers {
private:
  std::vector<std::function<void(unsigned long)>> funcs;

public:
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

  void loop(unsigned long now) {
    for (auto cb : funcs) {
      cb(now);
    }
  };
};
