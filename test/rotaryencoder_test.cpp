#include <cassert>

#include "peripheral/rotaryencoder.hpp"

using AVRSupport::Peripheral::RotaryEncoder;

RotaryEncoder rotary;

int main() {
    rotary.update(0,0);
    rotary.update(0,1);
    rotary.update(1,1);
    rotary.update(0,1);
    rotary.update(1,1);
    rotary.update(1,0);
    rotary.update(1,1);
    rotary.update(1,0);
    rotary.update(0,0);

    assert(rotary.turned_left());
}

