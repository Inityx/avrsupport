#ifndef AVRSUPPORT_PORTLIB_DIGITALPORT_H
#define AVRSUPPORT_PORTLIB_DIGITALPORT_H

#include <portlib/register.hpp>

namespace AvrSupport::PortLib {
    using PinIndex = uint8_t; ///< Type for indexing pins

    /// I/O for a full digital port
    struct DigitalPort {
        Register8 pinx, portx, ddrx;

        DigitalPort(
            Register8 pinx,
            Register8 portx,
            Register8 ddrx
        ) :
            pinx{pinx},
            portx{portx},
            ddrx{ddrx}
        {}

        uint8_t get() const { return pinx; }
        bool get(PinIndex const index) { return pinx & (1<<index); }

        DigitalPort & set_out() { ddrx = 0xFF; return *this; }
        DigitalPort & set_in()  { ddrx = 0x00; return *this; }
        DigitalPort & set(uint8_t states) { portx = states; return *this; }

        DigitalPort & set_out (PinIndex const index) { ddrx  |=  (1<<index); return *this; }
        DigitalPort & set_in  (PinIndex const index) { ddrx  &= ~(1<<index); return *this; }
        DigitalPort & set_high(PinIndex const index) { portx |=  (1<<index); return *this; }
        DigitalPort & set_low (PinIndex const index) { portx &= ~(1<<index); return *this; }
        DigitalPort & toggle  (PinIndex const index) { portx ^=  (1<<index); return *this; }

        DigitalPort & set_at(PinIndex const index, bool state) {
            state ? set_high(index) : set_low(index);
            return *this;
        }
    };
}

#endif
