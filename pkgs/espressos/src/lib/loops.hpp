#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

// Provides a container for functions to run in the loop() cycle
class Loops {
private:
  std::vector<std::function<void()>> funcs;

public:
  void addFunc(std::function<void()> func) { funcs.push_back(func); }

  void loop() {
    for (auto fn : funcs) {
      fn();
    }
  };
};
