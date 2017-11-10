#ifndef DIGITALPIN_H
#define DIGITALPIN_H

#include "portlib.hpp"

namespace AVRSupport::PortLib {
    struct DigitalPin {
    protected:
        // Members
        DigitalPort &port;
        uint8_t bitmask;

    public:
        // Constructors
        constexpr DigitalPin(
            DigitalPort &port,
            PinIndex const index
        ) :
            port(port),
            bitmask(1<<index)
        {}

        // Accessors
        inline bool get() const { return (port.pinx & bitmask) != 0; }

        inline void set_out()  { port.ddrx  |=  bitmask; }
        inline void set_in()   { port.ddrx  &= ~bitmask; }
        inline void set_high() { port.portx |=  bitmask; }
        inline void set_low()  { port.portx &= ~bitmask; }
        inline void toggle()   { port.portx ^=  bitmask; }

        inline void set(bool rhs) {
            if (rhs) set_high();
            else     set_low ();
        }
    };
}

#endif
