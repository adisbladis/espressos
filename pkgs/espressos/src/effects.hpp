#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <iostream>

// Provides a mechanism to watch variables emited by functions and
// run a callback function on change.
class Effects {
private:
  std::vector<std::function<bool()>> effects;
  std::vector<std::function<void()>> onEffectsTriggered;

public:
  template <typename T>
  void createEffect(std::function<T()> signal, std::function<void(T)> effect) {
    return createEffect(signal, effect, true);
  };

  template <typename T>
  void createEffect(std::function<T()> signal, std::function<void(T)> effect,
                    bool doWatch) {
    std::shared_ptr<T> value = std::make_shared<T>(signal());
    effect(*value);

    effects.push_back([=]() mutable {
      T v = signal();
      if (*value != v) {
        *value = v;
        effect(v);
        return doWatch;
      }
      return false;
    });
  };

  void onTriggered(std::function<void()> cb) {
    onEffectsTriggered.push_back(cb);
  };

  void loop() {
    bool effectsTriggered = false;

    for (auto e : effects) {
      if (e()) {
        effectsTriggered = true;
      }
    }

    if (effectsTriggered) {
      for (auto cb : onEffectsTriggered) {
        cb();
      }
    }
  };
};
