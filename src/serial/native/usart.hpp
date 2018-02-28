#ifndef AVRSUPPORT_SERIAL_NATIVE_USART_H
#define AVRSUPPORT_SERIAL_NATIVE_USART_H

#include <stdint.h>
#include <stddef.h>

#include <portlib/register.hpp>
#include <serial/interface/uart.hpp>
#include <utility/stddef.hpp>

namespace AvrSupport::Serial::Native {
    struct Usart : public Interface::Uart<Usart> {
    private:
        enum class ControlAMask : uint8_t {
            receive_complete  = 0b1000'0000,
            transmit_complete = 0b0100'0000,
            data_reg_empty    = 0b0010'0000,
            frame_error       = 0b0001'0000,
            data_overrun      = 0b0000'1000,
            parity_error      = 0b0000'0100,
            double_speed      = 0b0000'0010,
            multi_processor   = 0b0000'0001,
        };
        enum class ControlBMask : uint8_t {
            rx_complete_irq_enable = 0b1000'0000,
            tx_complete_irq_enable = 0b0100'0000,
            data_empty_irq_enable  = 0b0010'0000,
            rx_enable              = 0b0001'0000,
            tx_enable              = 0b0000'1000,
            char_size_high         = 0b0000'0100,
            ninth_rx_bit           = 0b0000'0010,
            ninth_tx_bit           = 0b0000'0001,
        };
        enum class ControlCMask : uint8_t {
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
            control_a,
            control_b,
            control_c,
            baud_l,
            baud_h;

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
            control_b &= ~static_cast<uint8_t>(ControlBMask::char_size_high);
            control_c &= ~static_cast<uint8_t>(ControlCMask::char_size_low);
            control_b |= high_bit;
            control_c |= low_bits;
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
            data     {udrn  },
            control_a{ucsrna},
            control_b{ucsrnb},
            control_c{ucsrnc},
            baud_l   {ubrrnl},
            baud_h   {ubrrnh}
        {}

        Usart & set_baud_rate(uint32_t const rate, uint32_t const f_cpu) {
            uint16_t baud_setting = (f_cpu / 16) / rate;
            baud_l = static_cast<uint8_t>(baud_setting & 0xFF);
            baud_h = static_cast<uint8_t>(baud_setting >> 8);
            return *this;
        }

        template<typename MaskEnum>
        void set_config(
            PortLib::Register8 & reg,
            MaskEnum const mask,
            bool const active
        ) {
            if (active) reg |=  static_cast<uint8_t>(mask);
            else        reg &= ~static_cast<uint8_t>(mask);
        }

        Usart & set_tx_rx(bool const tx_active, bool const rx_active) {
            set_config(control_b, ControlBMask::tx_enable, tx_active);
            set_config(control_b, ControlBMask::rx_enable, rx_active);
            
            // Required?
            asm("nop");
            asm("nop");

            return *this;
        }

        bool is_writing() {
            return !(control_a & static_cast<uint8_t>(ControlAMask::data_reg_empty));
        }

        uint8_t read_byte() {
            while (!(control_a & static_cast<uint8_t>(ControlAMask::receive_complete)));
            return data;
        }

        Usart & write_byte(uint8_t const value) {
            data = value;
            return *this;
        }

        Usart & sync_write_byte(uint8_t const value) {
            while (is_writing());
            return write_byte(value);
        }
    };
}
#endif
