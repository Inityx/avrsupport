#ifndef DIGITALPIN_H
#define DIGITALPIN_H

#include <portlib/register.hpp>
#include <portlib/digitalport.hpp>

namespace AvrSupport::PortLib {
    /// I/O for a single digital pin
    struct DigitalPin {
    protected:
        DigitalPort &port;
        uint8_t bitmask;

    public:
        constexpr DigitalPin(
            DigitalPort &port,
            PinIndex const index
        ) :
            port{port},
            bitmask{static_cast<uint8_t>(1<<index)}
        {}

        bool get() const { return (port.pinx & bitmask) != 0; }

        void set_out()  { port.ddrx  |=  bitmask; }
        void set_in()   { port.ddrx  &= ~bitmask; }
        void set_high() { port.portx |=  bitmask; }
        void set_low()  { port.portx &= ~bitmask; }
        void toggle()   { port.portx ^=  bitmask; }

        void set(bool state) {
            if (state) set_high();
            else       set_low ();
        }
    };
}

#endif
