#ifndef DIGITALPORT_H
#define DIGITALPORT_H

#include <portlib/register.hpp>

namespace AvrSupport::PortLib {
    using PinIndex = uint8_t; ///< Type for indexing pins

    /// I/O for a full digital port
    struct DigitalPort {
        Register8 pinx, portx, ddrx;

        constexpr DigitalPort(
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

        void set_out() { ddrx = 0xFF; }
        void set_in()  { ddrx = 0x00; }
        void set(uint8_t states) { portx = states; }

        void set_out (PinIndex const index) { ddrx  |=  (1<<index); }
        void set_in  (PinIndex const index) { ddrx  &= ~(1<<index); }
        void set_high(PinIndex const index) { portx |=  (1<<index); }
        void set_low (PinIndex const index) { portx &= ~(1<<index); }
        void toggle  (PinIndex const index) { portx ^=  (1<<index); }

        void set_at(PinIndex const index, bool state) {
            state ? set_high(index) : set_low(index);
        }
    };
}

#endif
