#ifndef AVRSUPPORT_PORTLIB_DIGITALPIN_H
#define AVRSUPPORT_PORTLIB_DIGITALPIN_H

#include <portlib/register.hpp>
#include <portlib/digitalport.hpp>
#include <utility/stddef.hpp>

namespace AvrSupport::PortLib {
    /// I/O for a single digital pin
    struct DigitalPin {
    protected:
        DigitalPort &port;
        uint8_t const bitmask;

    public:
        DigitalPin(
            DigitalPort &port,
            DigitalPort::PinIndex const index,
            Utility::IoDirection const direction = Utility::IoDirection::in,
            Utility::LogicLevel const level = Utility::LogicLevel::low
        ) :
            port{port},
            bitmask{static_cast<uint8_t>(1<<index)}
        {
            static_cast<bool>(direction) ? set_out() : set_in();
            set(level);
        }

        Utility::LogicLevel get() const {
            return static_cast<Utility::LogicLevel>((port.pinx & bitmask) != 0);
        }

        DigitalPin & set_out()  { port.ddrx  |=  bitmask; return *this; }
        DigitalPin & set_in()   { port.ddrx  &= ~bitmask; return *this; }
        DigitalPin & set_high() { port.portx |=  bitmask; return *this; }
        DigitalPin & set_low()  { port.portx &= ~bitmask; return *this; }
        DigitalPin & toggle()   { port.portx ^=  bitmask; return *this; }

        DigitalPin & set(bool const level) {
            level ? set_high() : set_low();
            return *this;
        }

        DigitalPin & set(Utility::LogicLevel const level) {
            return set(static_cast<bool>(level));
        }
    };
}

#endif
