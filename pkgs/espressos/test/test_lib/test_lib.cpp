#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include "lib/effects.hpp"
#include "lib/map.hpp"

TEST_CASE("map") { CHECK(map<long>(512, 0, 1024, 0, 2048) == 1024); }

TEST_SUITE("effects") {
  Effects e;
  static int input = 1;
  static int output;

  TEST_CASE("create effect") {
    e.createEffect<int>([]() { return input; },
                        [](int value) { output = value; });
  };

  TEST_CASE("basic execution") {
    e.loop();
    CHECK(output == input);
  }

  TEST_CASE("did update") {
    input = 2;
    e.loop();
    CHECK(output == input);
  }

  static bool triggered = false;
  TEST_CASE("did run onTriggered") {
    e.onTriggered([]() { triggered = true; });
    input = 3;
    e.loop();
    CHECK(output == input);
    CHECK(triggered);
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
