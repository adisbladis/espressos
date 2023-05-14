#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include "lib/map.hpp"
#include "lib/signal.hpp"

TEST_CASE("map") { CHECK(map<long>(512, 0, 1024, 0, 2048) == 1024); }

TEST_SUITE("signal") {
  static Signal<int> input(1);
  static int output;

  TEST_CASE("constructor") { CHECK(input.get() == 1); };

  TEST_CASE("create effect") {
    input.createEffect([](int value) { output = value; });
    CHECK(input == output);
  };

  TEST_CASE("did update") {
    input = 2;
    CHECK(input == output);
  }
}

int main(int argc, char **argv) {
  doctest::Context context;

  // PlatformIO required options
  context.setOption("success", true);     // Report successful tests
  context.setOption("no-exitcode", true); // No non-zero exit on fail

  context.applyCommandLine(argc, argv);
  return context.run();
}
