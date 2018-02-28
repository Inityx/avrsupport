#ifndef AVRSUPPORT_SERIAL_NATIVE_SPI_H
#define AVRSUPPORT_SERIAL_NATIVE_SPI_H

#include <portlib/register.hpp>
#include <utility/bytewise.hpp>

namespace AvrSupport::Serial::Native {
    /// Native %Serial %Peripheral Interface (SPI) driver
    /// @see Usi::Spi
    struct Spi : Interface::Spi<Spi> {
    private:
        enum struct ControlMask : uint8_t {
            spi_irq_enable = 0b1000'0000,
            spi_enable     = 0b0100'0000,
            data_order     = 0b0010'0000,
            role_select    = 0b0001'0000,
            clock_polarity = 0b0000'1000,
            clock_phase    = 0b0000'0100,
            clock_base     = 0b0000'0011,
        };
        enum struct StatusMask : uint8_t {
            spi_irq        = 0b1000'0000,
            collision      = 0b0100'0000,
            double_speed   = 0b0000'0001,
        };

        PortLib::Register8
            control,
            status,
            data;

    public:        
        enum Role : uint8_t { master, slave };

        constexpr Spi(
            PortLib::Register8 spcrn,
            PortLib::Register8 spsrn,
            PortLib::Register8 spdrn
        ) :
            control{spcrn},
            status{spsrn},
            data{spdrn}
        {}

        /** Set SPI baud rate.
         * Set baud rate = clock / 2^(exponent), 1 <= exponent <= 8
         */
        Spi & set_rate_divisor(uint8_t exponent) {
            exponent -= 1;
            control &= ~static_cast<uint8_t>(ControlMask::clock_base);
            control |= exponent>>1;
            status  &= ~static_cast<uint8_t>(StatusMask::double_speed);
            status  |= ~exponent & 0b1;
            return *this;
        }
    };
}

#endif
