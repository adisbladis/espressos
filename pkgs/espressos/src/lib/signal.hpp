#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <iostream>

// Wraps a value and calls side effects on update
template <typename T> class Signal {
private:
  std::vector<std::function<void(T)>> effects;
  T value;

public:
  Signal(const T value) : value(value){};

  bool operator==(const T &rhs) { return value == rhs; }
  bool operator!=(const T &rhs) { return value != rhs; }

  void operator=(const T &newValue) {
    if (value == newValue) {
      return;
    }

    value = newValue;

    for (auto e : effects) {
      e(value);
    }
  }

  void createEffect(std::function<void(T)> effect) {
    effects.push_back(effect);
    effect(value);
  };
};
