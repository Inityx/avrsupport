#ifndef AVRSUPPORT_SERIAL_USI_TWI_H
#define AVRSUPPORT_SERIAL_USI_TWI_H

#include <portlib/register.hpp>
#include <serial/usi/base.hpp>

namespace avrsupport::serial::usi {
    /// USI-powered Two-Wire interface (TWI/I2C) driver
    struct Twi : public usi::Base<Twi> {
        Twi(
            portlib::Register8 usidr,
            portlib::Register8 usibr,
            portlib::Register8 usisr,
            portlib::Register8 usicr
        ) :
            usi::Base<Twi>{usidr, usibr, usisr, usicr}
        {}

        Twi & initialize() {
            set_wire_mode(WireMode::two_wire);
            return *this;
        }
    };
}

#endif
