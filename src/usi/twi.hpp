#ifndef USI_TWI_H
#define USI_TWI_H

#include <portlib/register.hpp>
#include <usi/base.hpp>

namespace AvrSupport::Usi {
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
