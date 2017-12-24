#ifndef DIGITALPORT_H
#define DIGITALPORT_H

#include <portlib/register.hpp>

namespace AvrSupport::PortLib {
    typedef uint8_t PinIndex;

    struct DigitalPort {
        // Members
        Register8 pinx, portx,  ddrx;

        // Constructors
        constexpr DigitalPort(
            Register8 pinx,
            Register8 portx,
            Register8 ddrx
        ) :
            pinx{pinx},
            portx{portx},
            ddrx{ddrx}
        {}

        // Accessors
        uint8_t get() const { return pinx; }
        bool get(PinIndex const index) { return pinx & (1<<index); }

        void set(uint8_t rhs) { portx = rhs; }
        void set_out()        { ddrx = 0xFF; }
        void set_in()         { ddrx = 0x00; }

        void set_out (PinIndex const index) { ddrx  |=  (1<<index); }
        void set_in  (PinIndex const index) { ddrx  &= ~(1<<index); }
        void set_high(PinIndex const index) { portx |=  (1<<index); }
        void set_low (PinIndex const index) { portx &= ~(1<<index); }
        void toggle  (PinIndex const index) { portx ^=  (1<<index); }

        void set(PinIndex const index, bool rhs) {
            if (rhs) set_high(index);
            else     set_low (index);
        }
    };
}

#endif
