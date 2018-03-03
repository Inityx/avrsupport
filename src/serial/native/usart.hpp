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
        static Utility::avr_size_t const CHAR_SIZE_LOW_OFFSET{1};
        static uint8_t             const CHAR_SIZE_LOW_MASK {0b011};
        static uint8_t             const CHAR_SIZE_HIGH_MASK{0b100};

        // Registers
        PortLib::Register8
            data,
            control_a,
            control_b,
            control_c,
            baud_l,
            baud_h;

        // Internal methods
        template<typename MaskEnum> void set_config(
            PortLib::Register8 & reg,
            MaskEnum const mask,
            bool const active
        ) {
            if (active) reg |=  static_cast<uint8_t>(mask);
            else        reg &= ~static_cast<uint8_t>(mask);
        }

        template<typename MaskEnum> bool get_status(
            PortLib::Register8 & reg,
            MaskEnum const mask
        ) {
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

        Usart & set_stop_bits(StopBits const bits) {
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
        bool is_writable()      { return get_status(control_a, ControlAMask::data_reg_empty  ); }
        bool is_data_received() { return get_status(control_a, ControlAMask::receive_complete); }
        bool is_frame_error()   { return get_status(control_a, ControlAMask::frame_error     ); }
        bool is_data_overrun()  { return get_status(control_a, ControlAMask::data_overrun    ); }
        bool is_parity_error()  { return get_status(control_a, ControlAMask::parity_error    ); }

        // Usage methods
        uint8_t read_byte() {
            while (!is_data_received());
            return data;
        }

        Usart & write_byte(uint8_t const value) {
            data = value;
            return *this;
        }

        Usart & sync_write_byte(uint8_t const value) {
            while (!is_writable());
            return write_byte(value);
        }
    };
}
#endif
