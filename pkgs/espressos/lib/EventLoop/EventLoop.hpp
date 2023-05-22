#pragma once

#include <Timers.hpp>

// Timers are essentially an event loop (albeit without network/file io)
// I'd like to acknowedlge that
Timers &getEventLoop();
