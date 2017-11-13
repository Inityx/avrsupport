#ifndef ATTINY84_H
#define ATTINY84_H

#include <stdint.h>
#include <avr/io.h>

#include "portlib/digitalport.hpp"

namespace AVRSupport {
    using PortLib::DigitalPort;
    using Timer0 = PortLib::Timer<uint8_t>;
    using Timer1 = PortLib::Timer<uint16_t>;

    DigitalPort port_a{ porta, pina, ddra };
    DigitalPort port_b{ portb, pinb, ddrb };

    Timer0 timer_0 {
        TCNT0,
        OCR0A, OCR0B,
        TCCR0A, TCCR0B,
        TIMSK0,
        TIFR0
    };

    Timer1 timer_1 {
        TCNT1,
        OCR1A, OCR1B,
        TCCR1A, TCCR1B,
        TIMSK1,
        TIFR1
    };
}

#endif
