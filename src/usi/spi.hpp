#ifndef AVRSUPPORT_USI_SPI_H
#define AVRSUPPORT_USI_SPI_H

#include <portlib/register.hpp>
#include <usi/base.hpp>

namespace AvrSupport::Usi {
    /// USI-powered %Serial %Peripheral Interface (SPI) driver
    /// @see Serial::Spi
    struct Spi : public Usi::Base<Spi> {
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

        // Asynchronous has risk of modification while sending
        template<typename Type>
        Type swap_transact(Type const & value);

        template<typename Type>
        void swap_in_place(Type & buffer = Type(0));

        template<typename Type = uint16_t>
        void read(Type & value) { return value = swap_transact(Type(0)); }

        template<typename Type = uint16_t>
        Type read_n() {
        }

        template<typename Type>
        void write(Type & value) { swap(value); }
    };
}

#endif
