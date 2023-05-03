#pragma once

typedef std::function<void()> Callback_t;

class IntervalCallback {
private:
  unsigned long interval;
  unsigned long lastCall;
  Callback_t callback;

public:
  IntervalCallback(unsigned long interval, Callback_t callback)
      : interval(interval), callback(callback){};
  IntervalCallback(unsigned long interval) : interval(interval){};

  void setInterval(unsigned long newInterval) { interval = newInterval; }

  void setCallback(Callback_t callback) { this->callback = callback; }

  void reset() { lastCall = 0; }

  void loop(unsigned long now) {
    if (now >= lastCall + interval) {
      if (callback != NULL) {
        callback();
      }

      lastCall = now;
    }
  }
};
