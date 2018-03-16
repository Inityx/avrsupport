[![Build Status](https://travis-ci.org/Inityx/avrsupport.png)](https://travis-ci.org/Inityx/avrsupport)

# AVRSupport
This is a C++17 library for AVR Microcontrollers, designed to provide fast and clean implementations of common objects.

## Including in your project

To include this library in your C++ project, simply add this repository as a git submodule, run `make` in the `avrsupport` directory, and append `-I avrsupport/include` to your compile lines.
You can then `#include <portlib/digitalport.hpp>` to bring in `avrsupport::portlib::DigitalPort` or similar.
Compiled library objects are found in `avrsupport/lib`.

Unfortunately, there is no clean way to have make automatically build subfolders in a managed way; this problem is known as Recursive Make ([Miller, 1997](http://www.lateralt.net/files/auug97.pdf)).
This situation will hopefully become at least less complicated when C++ Modules are standardized.
