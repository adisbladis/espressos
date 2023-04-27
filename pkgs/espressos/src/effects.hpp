#pragma once

// Provides a mechanism to watch variables emited by functions and
// run a callback function on change.
class Effects {
private:
  std::vector<std::function<void()>> effects;

public:
  template <typename T>
  void createEffect(std::function<T()> cause, std::function<void(T)> effect) {
    static T value = cause();
    effect(value);

    effects.push_back([cause, effect]() {
      T v = cause();
      if (value != v) {
        value = v;
        effect(v);
      }
    });
  };

  void loop() {
    for (auto e : effects) {
      e();
    }
  };
};
