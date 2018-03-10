#ifndef AVRSUPPORT_SERIAL_USART_BASE_H
#define AVRSUPPORT_SERIAL_USART_BASE_H

#include <portlib/register.hpp>
#include <utility/stddef.hpp>

namespace avrsupport::serial::usart {
    struct Base {
    protected:
        // Physical control enums
        enum struct ControlAMask : uint8_t {
            RECEIVE_COMPLETE  = 0b1000'0000,
            TRANSMIT_COMPLETE = 0b0100'0000,
            DATA_REG_EMPTY    = 0b0010'0000,
            FRAME_ERROR       = 0b0001'0000,
            DATA_OVERRUN      = 0b0000'1000,
            PARITY_ERROR      = 0b0000'0100,
            DOUBLE_SPEED      = 0b0000'0010,
            MULTI_PROCESSOR   = 0b0000'0001,
        };
        enum struct ControlBMask : uint8_t {
            RX_DONE_IRQ_ON    = 0b1000'0000,
            TX_DONE_IRQ_ON    = 0b0100'0000,
            DATA_EMPTY_IRQ_ON = 0b0010'0000,
            RX_ENABLE         = 0b0001'0000,
            TX_ENABLE         = 0b0000'1000,
            CHAR_SIZE_HIGH    = 0b0000'0100,
            NINTH_RX_BIT      = 0b0000'0010,
            NINTH_TX_BIT      = 0b0000'0001,
        };
        enum struct ControlCMask : uint8_t {
            USART_MODE     = 0b11'00'0000,
            PARITY_MODE    = 0b00'11'0000,
            STOP_BITS      = 0b00'00'1000,
            CHAR_SIZE_LOW  = 0b00'00'0110,
            DATA_ORDER     = 0b00'00'0100,
            CLOCK_PHASE    = 0b00'00'0010,
            CLOCK_POLARITY = 0b00'00'0001,
        };

        enum struct Mode : uint8_t {
            ASYNC_USART = 0b00'00'0000,
            SYNC_USART  = 0b01'00'0000,
            MASTER_SPI  = 0b11'00'0000,
        };
        enum struct ParityMode : uint8_t {
            DISABLED = 0b00'00'0000,
            EVEN     = 0b00'10'0000,
            ODD      = 0b00'11'0000,
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
