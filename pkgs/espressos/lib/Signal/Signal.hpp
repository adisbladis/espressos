#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

// Wraps a value and calls side effects on update
template <typename T> class Signal {
private:
  std::vector<std::function<void(const T &)>> effects;
  T value;

public:
  Signal(){};
  Signal(const T value) : value(value){};

  bool operator==(const T &rhs) { return value == rhs; }
  bool operator!=(const T &rhs) { return value != rhs; }

  T get() { return value; }
  T &get_mut() { return value; }

  void operator=(const T &rhs) {
    if (value == rhs) {
      return;
    }

    value = rhs;

    for (const auto &e : effects) {
      e(value);
    }
  }

  void createEffect(std::function<void(const T &)> effect, bool eager) {
    effects.push_back(effect);
    if (eager) {
      effect(value);
    }
  };

  void createEffect(std::function<void(const T &)> effect) {
    createEffect(effect, true);
  };
};
