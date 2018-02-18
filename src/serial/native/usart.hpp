#ifndef AVRSUPPORT_SERIAL_NATIVE_USART_H
#define AVRSUPPORT_SERIAL_NATIVE_USART_H

#include <stdint.h>
#include <stddef.h>

#include <portlib/register.hpp>

#include <utility/stddef.hpp>

namespace AvrSupport::Serial::Native {
    struct Usart {
    private:
        enum class ControlStatusAMask : uint8_t {
            receive_complete  = 0b1000'0000,
            transmit_complete = 0b0100'0000,
            data_reg_empty    = 0b0010'0000,
            frame_error       = 0b0001'0000,
            data_overrun      = 0b0000'1000,
            parity_error      = 0b0000'0100,
            double_speed      = 0b0000'0010,
            multi_processor   = 0b0000'0001,
        };
        enum class ControlStatusBMask : uint8_t {
            rx_complete_irq_enable = 0b1000'0000,
            tx_complete_irq_enable = 0b0100'0000,
            data_empty_irq_enable  = 0b0010'0000,
            rx_enable              = 0b0001'0000,
            tx_enable              = 0b0000'1000,
            char_size_high         = 0b0000'0100,
            ninth_rx_bit           = 0b0000'0010,
            ninth_tx_bit           = 0b0000'0001,
        };
        enum class ControlStatusCMask : uint8_t {
            usart_mode     = 0b11'00'0000,
            parity_mode    = 0b00'11'0000,
            stop_bits      = 0b00'00'1000,
            char_size_low  = 0b00'00'0110,
            data_order     = 0b00'00'0100,
            clock_phase    = 0b00'00'0010,
            clock_polarity = 0b00'00'0001,
        };

        static Utility::avr_size_t const CHAR_SIZE_LOW_OFFSET{1};
        static uint8_t         const CHAR_SIZE_LOW_MASK {0b011};
        static uint8_t         const CHAR_SIZE_HIGH_MASK{0b100};
        enum class Mode : uint8_t {
            async_usart = 0b00'00'0000,
            sync_usart  = 0b01'00'0000,
            master_spi  = 0b11'00'0000,
        };
        enum class ParityMode : uint8_t {
            disabled = 0b00'00'0000,
            even     = 0b00'10'0000,
            odd      = 0b00'11'0000,
        };

        PortLib::Register8
            data,
            control_status_a,
            control_status_b,
            control_status_c,
            baud_rate_low,
            baud_rate_high;

    public:
        enum class CharSize : uint8_t {
            five_bit  = 0b000,
            six_bit   = 0b001,
            seven_bit = 0b010,
            eight_bit = 0b011,
            nine_bit  = 0b111,
        };

        Usart & set_char_size(CharSize size) {
            uint8_t const
                high_bit( static_cast<uint8_t>(size) & CHAR_SIZE_HIGH_MASK),
                low_bits((static_cast<uint8_t>(size) & CHAR_SIZE_LOW_MASK) << CHAR_SIZE_LOW_OFFSET);
            control_status_b &= ~static_cast<uint8_t>(ControlStatusBMask::char_size_high);
            control_status_c &= ~static_cast<uint8_t>(ControlStatusCMask::char_size_low);
            control_status_b |= high_bit;
            control_status_c |= low_bits;
            return *this;
        }

        constexpr Usart(
            PortLib::Register8 udrn,
            PortLib::Register8 ucsrna,
            PortLib::Register8 ucsrnb,
            PortLib::Register8 ucsrnc,
            PortLib::Register8 ubrrnl,
            PortLib::Register8 ubrrnh
        ) :
            data            {udrn  },
            control_status_a{ucsrna},
            control_status_b{ucsrnb},
            control_status_c{ucsrnc},
            baud_rate_low   {ubrrnl},
            baud_rate_high  {ubrrnh}
        {}

        Usart & set_baud_rate(uint16_t rate) {
            baud_rate_low  = rate & 0xFF;
            baud_rate_high = rate >> 8;
            return *this;
        }
    };
}
#endif
