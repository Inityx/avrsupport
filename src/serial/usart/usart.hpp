#ifndef AVRSUPPORT_SERIAL_USART_USART_H
#define AVRSUPPORT_SERIAL_USART_USART_H

#include <stdint.h>
#include <stddef.h>

#include <portlib/register.hpp>

#include <serial/interface/uart.hpp>
#include <serial/usart/base.hpp>

#include <utility/stddef.hpp>

namespace avrsupport::serial::usart {
    struct Usart : public interface::Uart<Usart>, public usart::Base {
    private:
        // Internal methods
        template<typename MaskEnum> void set_config(
            portlib::Register8 & reg,
            MaskEnum const mask,
            bool const active
        ) {
            if (active) reg |=  static_cast<uint8_t>(mask);
            else        reg &= ~static_cast<uint8_t>(mask);
        }

        template<typename MaskEnum> bool get_status(
            portlib::Register8 & reg,
            MaskEnum const mask
        ) const {
            return reg & static_cast<uint8_t>(mask);
        }

    public:
        // Logical control enums
        enum struct CharSize : uint8_t {
            five_bit  = 0b000,
            six_bit   = 0b001,
            seven_bit = 0b010,
            eight_bit = 0b011,
            nine_bit  = 0b111,
        };

        // Constructors
        constexpr Usart(
            portlib::Register8 udrn,
            portlib::Register8 ucsrna,
            portlib::Register8 ucsrnb,
            portlib::Register8 ucsrnc,
            portlib::Register8 ubrrnl,
            portlib::Register8 ubrrnh
        ) :
            Base {
                udrn,
                ucsrna,
                ucsrnb,
                ucsrnc,
                ubrrnl,
                ubrrnh
            }
        {}

        // Configuration methods
        Usart & set_char_size(CharSize const size) {
            uint8_t const
                size_num = static_cast<uint8_t>(size),
                high_bit( size_num & CHAR_SIZE_HIGH_MASK),
                low_bits((size_num & CHAR_SIZE_LOW_MASK) << CHAR_SIZE_LOW_OFFSET);

            control_b &= ~static_cast<uint8_t>(ControlBMask::char_size_high);
            control_c &= ~static_cast<uint8_t>(ControlCMask::char_size_low);
            control_b |= high_bit;
            control_c |= low_bits;
            return *this;
        }

        Usart & set_stop_bits(StopBits const bits) override {
            bool setting;
            switch (bits) {
                case StopBits::one: setting = false; break;
                case StopBits::two: setting = true;  break;
            }
            set_config(control_c, ControlCMask::stop_bits, setting);
            return *this;
        }

        Usart & set_baud_rate(uint32_t const rate, uint32_t const f_cpu) {
            uint16_t baud_setting = (f_cpu / 16) / rate;
            baud_l = static_cast<uint8_t>(baud_setting & 0xFF);
            baud_h = static_cast<uint8_t>(baud_setting >> 8);
            return *this;
        }

        Usart & set_tx_rx(bool const tx_active, bool const rx_active) {
            set_config(control_b, ControlBMask::tx_enable, tx_active);
            set_config(control_b, ControlBMask::rx_enable, rx_active);
            
            asm("nop"); asm("nop"); // Required?

            return *this;
        }

        // Status methods
        bool is_writable()      const override { return get_status(control_a, ControlAMask::data_reg_empty  ); }
        bool is_data_received() const          { return get_status(control_a, ControlAMask::receive_complete); }
        bool is_frame_error()   const          { return get_status(control_a, ControlAMask::frame_error     ); }
        bool is_data_overrun()  const          { return get_status(control_a, ControlAMask::data_overrun    ); }
        bool is_parity_error()  const          { return get_status(control_a, ControlAMask::parity_error    ); }

        // Usage methods
        uint8_t read_byte() const override {
            while (!is_data_received());
            return data;
        }

        Usart & write_byte(uint8_t const value) override {
            data = value;
            return *this;
        }
    };
}
#endif
