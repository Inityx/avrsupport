#ifndef AVRSUPPORT_SERIAL_USART_BASE_H
#define AVRSUPPORT_SERIAL_USART_BASE_H

#include <portlib/register.hpp>
#include <utility/stddef.hpp>

namespace avrsupport::serial::usart {
    struct Base {
    protected:
        // Physical control enums
        enum struct ControlAMask : uint8_t {
            receive_complete  = 0b1000'0000,
            transmit_complete = 0b0100'0000,
            data_reg_empty    = 0b0010'0000,
            frame_error       = 0b0001'0000,
            data_overrun      = 0b0000'1000,
            parity_error      = 0b0000'0100,
            double_speed      = 0b0000'0010,
            multi_processor   = 0b0000'0001,
        };
        enum struct ControlBMask : uint8_t {
            rx_done_irq_on    = 0b1000'0000,
            tx_done_irq_on    = 0b0100'0000,
            data_empty_irq_on = 0b0010'0000,
            rx_enable         = 0b0001'0000,
            tx_enable         = 0b0000'1000,
            char_size_high    = 0b0000'0100,
            ninth_rx_bit      = 0b0000'0010,
            ninth_tx_bit      = 0b0000'0001,
        };
        enum struct ControlCMask : uint8_t {
            usart_mode     = 0b11'00'0000,
            parity_mode    = 0b00'11'0000,
            stop_bits      = 0b00'00'1000,
            char_size_low  = 0b00'00'0110,
            data_order     = 0b00'00'0100,
            clock_phase    = 0b00'00'0010,
            clock_polarity = 0b00'00'0001,
        };

        enum struct Mode : uint8_t {
            async_usart = 0b00'00'0000,
            sync_usart  = 0b01'00'0000,
            master_spi  = 0b11'00'0000,
        };
        enum struct ParityMode : uint8_t {
            disabled = 0b00'00'0000,
            even     = 0b00'10'0000,
            odd      = 0b00'11'0000,
        };

        // Static constants
        static utility::avr_size_t const CHAR_SIZE_LOW_OFFSET{1};
        static uint8_t             const CHAR_SIZE_LOW_MASK {0b011};
        static uint8_t             const CHAR_SIZE_HIGH_MASK{0b100};

        // Registers
        portlib::Register8
            data,
            control_a,
            control_b,
            control_c,
            baud_l,
            baud_h;

        // Constructor
        constexpr Base(
            portlib::Register8 udrn,
            portlib::Register8 ucsrna,
            portlib::Register8 ucsrnb,
            portlib::Register8 ucsrnc,
            portlib::Register8 ubrrnl,
            portlib::Register8 ubrrnh
        ) :
            data     {udrn  },
            control_a{ucsrna},
            control_b{ucsrnb},
            control_c{ucsrnc},
            baud_l   {ubrrnl},
            baud_h   {ubrrnh}
        {}
    };
}

#endif
