# PIDController - A C++ PID controller implementation

This started it's life as [Arduino PID Library - Version 1.2.1](https://github.com/br3ttb/Arduino-PID-Library) by Brett Beauregard but has been forked and undergone significant API changes.

## Changes from upstream
- Internal timestamp calculation has been removed.

This removes the dependency on millis() and the Arduino standard library, meaning that this library can easily be ported to other platforms.

- C-style preprocessor `#DEFINE`'s have been replaced with enums.

- Code has been formatted with clang-format.

- C-style casts have been replaced with `static_cast` for better type safety.

- A test suite has been added.

- Types are now using C++ templates instead of being hard coded. Users can decide on how much precision and what types to store themselves.

## Resources
For an ultra-detailed explanation of why the code is the way it is, please visit http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

## License
This Library is licensed under the MIT License
