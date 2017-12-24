#ifndef ATTINY84_H
#define ATTINY84_H

#include <stdint.h>
#include <avr/io.h>

#include <portlib/digitalport.hpp>
#include <portlib/timer.hpp>

namespace AvrSupport {
    namespace PortLib {
        using Timer0 = Timer<uint8_t>;
        using Timer1 = Timer<uint16_t>;
    }

    PortLib::DigitalPort port_a{ PINA, PORTA, DDRA };
    PortLib::DigitalPort port_b{ PINB, PORTB, DDRB };

    PortLib::Timer0 timer_0 {
        TCNT0,
        OCR0A, OCR0B,
        TCCR0A, TCCR0B,
        TIMSK0,
        TIFR0
    };

    PortLib::Timer1 timer_1 {
        TCNT1,
        OCR1A, OCR1B,
        TCCR1A, TCCR1B,
        TIMSK1,
        TIFR1
    };
}

#endif
