#ifndef AVRSUPPORT_SERIAL_USI_TWI_H
#define AVRSUPPORT_SERIAL_USI_TWI_H

#include <portlib/register.hpp>
#include <serial/usi/base.hpp>

namespace AvrSupport::Serial::Usi {
    /// USI-powered Two-Wire Interface (TWI/I2C) driver
    struct Twi : public Usi::Base<Twi> {
        Twi(
            PortLib::Register8 usidr,
            PortLib::Register8 usibr,
            PortLib::Register8 usisr,
            PortLib::Register8 usicr
        ) :
            Usi::Base<Twi>{usidr, usibr, usisr, usicr}
        {}

        Twi & initialize() {
            set_wire_mode(WireMode::two_wire);
            return *this;
        }
    };
}

#endif
