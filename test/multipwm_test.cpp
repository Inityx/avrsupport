#include <cassert>

#include "emulated/multipwm.hpp"

using namespace AvrSupport;

using Utility::Array;
using PortLib::DigitalPort;
using Emulated::MultiPWM;

volatile uint8_t PORTA{0x00};
volatile uint8_t PINA{0x00};
volatile uint8_t DDRA{0x00};

int main() {
    Array<uint8_t, 3> PINS { 0, 1, 2 };
    Array<uint8_t, 3> LEVELS { 0, 2, 4 };

    DigitalPort port_a{ PINA, PORTA, DDRA };
    MultiPWM<3, 1> pwm { PINS, LEVELS, port_a };
        
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

    return 0;
}
