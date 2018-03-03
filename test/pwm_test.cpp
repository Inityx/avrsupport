#include <cassert>
#include <cstdio>

#include <portlib/digitalport.hpp>
#include <portlib/digitalpin.hpp>

#include <emulated/pwm.hpp>

using namespace avrsupport;

using portlib::DigitalPort;
using portlib::DigitalPin;
using emulated::Pwm;

volatile uint8_t
    PORTA{0x00},
    PINA{0x00},
    DDRA{0x00};

DigitalPort port_a{PINA, PORTA, DDRA};
DigitalPin pwm_pin{port_a, 0};

Pwm pwm{pwm_pin, 2};

int main() {
    pwm.step(); // 0 -> high
    assert(PORTA == 0b1);
    pwm.step(); // 1 -> high
    pwm.step(); // 2 -> high
    assert(PORTA == 0b1);
    pwm.step(); // 3 -> low
    assert(PORTA == 0b0);
}
