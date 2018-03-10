#ifndef AVRSUPPORT_SERIAL_USART_SPI_H
#define AVRSUPPORT_SERIAL_USART_SPI_H

#include <portlib/register.hpp>
#include <serial/interface/spi.hpp>
#include <utility/bytewise.hpp>

namespace avrsupport::serial::usart {
    /// native %serial %Peripheral interface (SPI) driver
    /// @see usi::Spi
    struct Spi : interface::Spi<Spi> {
    private:
        enum struct ControlMask : uint8_t {
            SPI_IRQ_ENABLE = 0b1000'0000,
            SPI_ENABLE     = 0b0100'0000,
            DATA_ORDER     = 0b0010'0000,
            ROLE_SELECT    = 0b0001'0000,
            CLOCK_POLARITY = 0b0000'1000,
            CLOCK_PHASE    = 0b0000'0100,
            CLOCK_BASE     = 0b0000'0011,
        };
        enum struct StatusMask : uint8_t {
            SPI_IRQ        = 0b1000'0000,
            COLLISION      = 0b0100'0000,
            DOUBLE_SPEED   = 0b0000'0001,
        };

        portlib::Register8
            control,
            status,
            data;

    public:        
        enum Role : uint8_t { master, slave };

        constexpr Spi(
            portlib::Register8 spcrn,
            portlib::Register8 spsrn,
            portlib::Register8 spdrn
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
            control &= ~static_cast<uint8_t>(ControlMask::CLOCK_BASE);
            control |= exponent>>1;
            status  &= ~static_cast<uint8_t>(StatusMask::DOUBLE_SPEED);
            status  |= ~exponent & 0b1;
            return *this;
        }
    };
}

#endif
