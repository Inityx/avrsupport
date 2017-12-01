#ifndef ATTINY84_H
#define ATTINY84_H

#include <stdint.h>
#include <avr/io.h>

#include "../portlib/digitalport.hpp"
#include "../portlib/timer.hpp"

namespace AVRSupport {
    using PortLib::DigitalPort;
    using PortLib::Timer;

    using Timer0 = Timer<uint8_t>;
    using Timer1 = Timer<uint16_t>;

    DigitalPort port_a{ PINA, PORTA, DDRA };
    DigitalPort port_b{ PINB, PORTB, DDRB };

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
