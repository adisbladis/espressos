#pragma once

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include <events.hpp>
#include <fsmlist.hpp>

#define TEST_MAIN()                                                            \
  int main(int argc, char **argv) {                                            \
    doctest::Context context;                                                  \
    fsm_list::start();                                                         \
    context.setOption("success", true);                                        \
    context.setOption("no-exitcode", true);                                    \
    context.applyCommandLine(argc, argv);                                      \
    return context.run();                                                      \
  }
