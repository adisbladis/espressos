#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include "lib/map.hpp"
#include "lib/signal.hpp"
#include "lib/timers.hpp"

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

TEST_SUITE("timers") {

  TEST_CASE("createInterval") {
    Timers timers;
    static int input = 1;
    static int output = 0;

    timers.createInterval(10, []() { output = input; });

    // Check that we didn't trigger eagerly
    CHECK(output == 0);

    // Check that we didn't trigger
    timers.loop(1);
    CHECK(output == 0);

    // Check that we did trigger
    timers.loop(10);
    CHECK(output == 1);
  }

  TEST_CASE("setTimeout") {
    Timers timers;
    static int input = 1;
    static int output = 0;

    timers.setTimeout(10, []() { output = input; });

    // Check that we didn't trigger eagerly
    CHECK(output == 0);

    // Check that we didn't trigger
    timers.loop(1);
    CHECK(output == 0);

    // Check that we did trigger
    timers.loop(10);
    CHECK(output == 1);

    // Check that we didn't trigger again
    input = 2;
    timers.loop(20);
    CHECK(output == 1);
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
