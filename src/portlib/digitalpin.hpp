#ifndef AVRSUPPORT_PORTLIB_DIGITALPIN_H
#define AVRSUPPORT_PORTLIB_DIGITALPIN_H

#include <portlib/register.hpp>
#include <portlib/digitalport.hpp>

namespace AvrSupport::PortLib {
    /// I/O for a single digital pin
    struct DigitalPin {
    protected:
        DigitalPort &port;
        uint8_t const bitmask;

    public:
        DigitalPin(
            DigitalPort &port,
            PinIndex const index
        ) :
            port{port},
            bitmask{static_cast<uint8_t>(1<<index)}
        {}

        bool get() const { return (port.pinx & bitmask) != 0; }

        DigitalPin & set_out()  { port.ddrx  |=  bitmask; return *this; }
        DigitalPin & set_in()   { port.ddrx  &= ~bitmask; return *this; }
        DigitalPin & set_high() { port.portx |=  bitmask; return *this; }
        DigitalPin & set_low()  { port.portx &= ~bitmask; return *this; }
        DigitalPin & toggle()   { port.portx ^=  bitmask; return *this; }

        DigitalPin & set(bool state) {
            if (state) set_high();
            else       set_low ();
            return *this;
        }
    };
}

#endif
