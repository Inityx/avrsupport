#ifndef AVRSUPPORT_USI_BASE_H
#define AVRSUPPORT_USI_BASE_H

#include <portlib/register.hpp>

namespace AvrSupport::Usi {
    /** A base Universal %Serial Interface (USI) driver.
     * This class is used to implement other serial interfaces.
     * @tparam SelfClass The derived class ([CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern))
     */
    template<typename SelfClass>
    struct Base {
    private:
        enum struct ControlMask : uint8_t {
            start_cond_int = 0b10'00'00'00, ///< Start condition interrupt enable
            overflow_int   = 0b01'00'00'00, ///< Counter overflow interrupt enable
            wire_mode      = 0b00'11'00'00, ///< Wire mode select (Usi::WireMode)
            clock_source   = 0b00'00'11'00, ///< Clock source select (Usi::ClockSource)
            clock_strobe   = 0b00'00'00'10, ///< Clock strobe
            toggle_clock   = 0b00'00'00'01, ///< Toggle clock port pin
        };

        static uint8_t const COUNTER_MASK{0b1111};

        PortLib::Register8
            data,
            buffer,
            status,
            control;
        
        SelfClass * this_derived() { return static_cast<SelfClass *>(this); }

    protected:
        enum struct StatusMask : uint8_t {
            start_cond_interrupt = 0b1000'0000,
            overflow_interrupt   = 0b0100'0000,
            stop_cond            = 0b0010'0000,
            output_collision     = 0b0001'0000,
        };

        enum struct WireMode : uint8_t {
            none           = 0b00'00'00'00, ///< None
            three_wire     = 0b00'01'00'00, ///< Three wire mode
            two_wire       = 0b00'10'00'00, ///< Two wire mode
            two_wire_low   = 0b00'11'00'00, ///< Two wire mode w/ SCL pin low on overflow
        };
        uint8_t get_counter_value() const { return status & COUNTER_MASK; }
        
        bool get_status(StatusMask const selection) const {
            return status & static_cast<uint8_t>(selection);
        }

        SelfClass & set_wire_mode(WireMode mode) {
            control &= ~static_cast<uint8_t>(ControlMask::wire_mode);
            control |=  static_cast<uint8_t>(mode);
            return *this_derived();
        }

        SelfClass & strobe_clock() {
            control &= static_cast<uint8_t>(ControlMask::clock_strobe);
            return *this_derived();
        }

    public:
        enum struct ClockSource : uint8_t {
            none_software  = 0b00'00'00'00, ///< None or software strobe
            compare_match  = 0b00'00'01'00, ///< Timer/Counter0 compare match
            positive_edge  = 0b00'00'10'00, ///< Positive edge
            negavive_edge  = 0b00'00'11'00, ///< Negative edge
        };

        Base(
            PortLib::Register8 usidr,
            PortLib::Register8 usibr,
            PortLib::Register8 usisr,
            PortLib::Register8 usicr
        ) :
            data   {usidr},
            buffer {usibr},
            status {usisr},
            control{usicr}
        {}

        SelfClass & set_clock_source(ClockSource source) {
            control &= ~static_cast<uint8_t>(ControlMask::clock_source);
            control |=  static_cast<uint8_t>(source);
            return *this_derived();
        }
    };
}

#endif
