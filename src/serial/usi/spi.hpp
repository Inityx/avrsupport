#ifndef AVRSUPPORT_SERIAL_USI_SPI_H
#define AVRSUPPORT_SERIAL_USI_SPI_H

#include <portlib/register.hpp>
#include <serial/interface/spi.hpp>
#include <serial/usi/base.hpp>

namespace avrsupport::serial::usi {
    /// USI-powered Serial Peripheral interface (SPI) driver
    /// @see serial::native::Spi
    struct Spi : public interface::Spi<Spi>, public usi::Base<Spi> {
        Spi(
            portlib::Register8 usidr,
            portlib::Register8 usibr,
            portlib::Register8 usisr,
            portlib::Register8 usicr
        ) :
            usi::Base<Spi>{usidr, usibr, usisr, usicr}
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
