#ifndef AVRSUPPORT_SERIAL_USI_SPI_H
#define AVRSUPPORT_SERIAL_USI_SPI_H

#include <portlib/register.hpp>
#include <serial/usi/base.hpp>

namespace AvrSupport::Serial::Usi {
    /// USI-powered %Serial %Peripheral Interface (SPI) driver
    /// @see Serial::Native::Spi
    struct Spi : public Interface::Spi<Spi>, public Usi::Base<Spi> {
        Spi(
            PortLib::Register8 usidr,
            PortLib::Register8 usibr,
            PortLib::Register8 usisr,
            PortLib::Register8 usicr
        ) :
            Usi::Base<Spi>{usidr, usibr, usisr, usicr}
        {}

        Spi & initialize() {
            set_wire_mode(WireMode::three_wire);
            return *this;
        }

        template<typename Type>
        Type swap(Type const value) {
            // TODO
            return Type(0);
        }

        template<typename Type>
        void read(Type & value) {
            return value = swap_transact_reference(Type(0));
        }

        template<typename Type>
        Type read_dyn_n() {
        }

    };
}

#endif
