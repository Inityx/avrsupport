#ifndef AVRSUPPORT_PORTLIB_DIGITALPORT_H
#define AVRSUPPORT_PORTLIB_DIGITALPORT_H

#include <portlib/register.hpp>
#include <utility/stddef.hpp>

namespace avrsupport::portlib {
    /// I/O for a full digital port
    struct DigitalPort {
        using PinIndex = uint8_t; ///< Type for indexing pins

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

        DigitalPort & set_at(PinIndex const index, bool const level) {
            level ? set_high(index) : set_low(index);
            return *this;
        }

        DigitalPort & set_at(PinIndex const index, utility::LogicLevel const level) {
            return set_at(index, static_cast<bool>(level));
        }
    };
}

#endif
