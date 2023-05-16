#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

// Wraps a value and calls side effects on update
template <typename T> class Signal {
private:
  std::vector<std::function<void(T)>> effects;
  T value;

public:
  Signal(){};
  Signal(const T value) : value(value){};

  bool operator==(const T &rhs) { return value == rhs; }
  bool operator!=(const T &rhs) { return value != rhs; }

  T get() { return value; }

  void operator=(const T &rhs) {
    if (value == rhs) {
      return;
    }

    value = rhs;

    for (const auto &e : effects) {
      e(value);
    }
  }

  void createEffect(std::function<void(T)> effect, bool eager) {
    effects.push_back(effect);
    if (eager) {
      effect(value);
    }
  };

  void createEffect(std::function<void(T)> effect) {
    createEffect(effect, true);
  };
};
