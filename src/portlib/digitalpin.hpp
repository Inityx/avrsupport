#ifndef DIGITALPIN_H
#define DIGITALPIN_H

#include "portlib.hpp"
#include "digitalport.hpp"

namespace AvrSupport::PortLib {
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
            port{port},
            bitmask{1<<index}
        {}

        // Accessors
        bool get() const { return (port.pinx & bitmask) != 0; }

        void set_out()  { port.ddrx  |=  bitmask; }
        void set_in()   { port.ddrx  &= ~bitmask; }
        void set_high() { port.portx |=  bitmask; }
        void set_low()  { port.portx &= ~bitmask; }
        void toggle()   { port.portx ^=  bitmask; }

        void set(bool rhs) {
            if (rhs) set_high();
            else     set_low ();
        }
    };
}

#endif
