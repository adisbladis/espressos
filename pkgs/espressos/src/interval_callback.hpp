#pragma once

typedef std::function<void()> Callback_t;

class IntervalCallback {
private:
  long interval;
  long lastCall;
  Callback_t callback;

public:
  IntervalCallback(long interval, Callback_t callback)
      : interval(interval), callback(callback){};
  IntervalCallback(long interval) : interval(interval){};

  void setInterval(long newInterval) { interval = newInterval; }

  void setCallback(Callback_t callback) { this->callback = callback; }

  void loop() {
    long now = millis();

    if (now >= lastCall + interval) {
      if (callback != NULL) {
        callback();
      }

      lastCall = millis();
    }
  }
};
