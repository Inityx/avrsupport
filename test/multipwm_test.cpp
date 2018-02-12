#include <cassert>

#include <portlib/digitalport.hpp>
#include <portlib/digitalpin.hpp>

#include <emulated/multipwm.hpp>

using namespace AvrSupport;

using Utility::Array;
using PortLib::DigitalPort;
using Emulated::MultiPwm;

volatile uint8_t
    PORTA{0x00},
    PINA{0x00},
    DDRA{0x00};

DigitalPort port_a{ PINA, PORTA, DDRA };

int main() {
    Array<uint8_t, 3> const PINS { 0, 1, 2 };
    Array<uint8_t, 3> const LEVELS { 0, 2, 4 };

    MultiPwm<3, 1> pwm { PINS, LEVELS, port_a };

    pwm.set_pins_out();

    assert(DDRA  == 0b111);
    assert(PORTA == 0b000);

    pwm.step();
    pwm.step();

    assert(PORTA == 0b110);

    pwm.step();
    pwm.step();

    assert(PORTA == 0b100);

    pwm.step();
    pwm.step();

    assert(PORTA == 0b000);

    pwm.adjust_up();
    assert(pwm.get_level(0) == 1);
    pwm.select_next();
    pwm.adjust_up();
    assert(pwm.get_level(1) == 3);

    return 0;
}
