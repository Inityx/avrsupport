#ifndef AVRSUPPORT_SERIAL_USI_BASE_H
#define AVRSUPPORT_SERIAL_USI_BASE_H

#include <portlib/register.hpp>

namespace avrsupport::serial::usi {
    /** A base Universal %serial interface (USI) driver.
     * This class is used to implement other serial interfaces.
     * @tparam SelfClass The derived class ([CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern))
     */
    template<typename SelfClass>
    struct Base {
    private:
        enum struct ControlMask : uint8_t {
            START_COND_IRQ = 0b10'00'00'00, ///< Start condition interrupt enable
            OVERFLOW_IRQ   = 0b01'00'00'00, ///< Counter overflow interrupt enable
            WIRE_MODE      = 0b00'11'00'00, ///< Wire mode select (usi::WireMode)
            CLOCK_SOURCE   = 0b00'00'11'00, ///< Clock source select (usi::ClockSource)
            CLOCK_STROBE   = 0b00'00'00'10, ///< Clock strobe
            TOGGLE_CLOCK   = 0b00'00'00'01, ///< Toggle clock port pin
        };

        static uint8_t const COUNTER_MASK{0b1111};

        portlib::Register8
            data,
            buffer,
            status,
            control;
        
        SelfClass * this_derived() { return static_cast<SelfClass *>(this); }

    protected:
        enum struct StatusMask : uint8_t {
            START_COND_IRQ = 0b1000'0000,
            OVERFLOW_IRQ   = 0b0100'0000,
            STOP_COND      = 0b0010'0000,
            DATA_COLLISION = 0b0001'0000,
        };

        enum struct WireMode : uint8_t {
            NONE           = 0b00'00'00'00, ///< None
            THREE_WIRE     = 0b00'01'00'00, ///< Three wire mode
            TWO_WIRE       = 0b00'10'00'00, ///< Two wire mode
            TWO_WIRE_LOW   = 0b00'11'00'00, ///< Two wire mode w/ SCL pin low on overflow
        };
        uint8_t get_counter_value() const { return status & COUNTER_MASK; }
        
        bool get_status(StatusMask const selection) const {
            return status & static_cast<uint8_t>(selection);
        }

        SelfClass & set_wire_mode(WireMode mode) {
            control &= ~static_cast<uint8_t>(ControlMask::WIRE_MODE);
            control |=  static_cast<uint8_t>(mode);
            return *this_derived();
        }

        SelfClass & strobe_clock() {
            control &= static_cast<uint8_t>(ControlMask::CLOCK_STROBE);
            return *this_derived();
        }

        Base(
            portlib::Register8 usidr,
            portlib::Register8 usibr,
            portlib::Register8 usisr,
            portlib::Register8 usicr
        ) :
            data   {usidr},
            buffer {usibr},
            status {usisr},
            control{usicr}
        {}

    public:
        enum struct ClockSource : uint8_t {
            NONE_SOFTWARE  = 0b00'00'00'00, ///< None or software strobe
            COMPARE_MATCH  = 0b00'00'01'00, ///< Timer/Counter0 compare match
            POSITIVE_EDGE  = 0b00'00'10'00, ///< Positive edge
            NEGAVIVE_EDGE  = 0b00'00'11'00, ///< Negative edge
        };

        SelfClass & set_clock_source(ClockSource source) {
            control &= ~static_cast<uint8_t>(ControlMask::CLOCK_SOURCE);
            control |=  static_cast<uint8_t>(source);
            return *this_derived();
        }
    };
}

#endif
