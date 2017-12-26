#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <portlib/register.hpp>

namespace AvrSupport::PortLib {
    /**
     * Built-in timer driver
     * @tparam TimerSize %Timer counter register type (e.g. uint8_t)
     */
    template<typename TimerSize>
    struct Timer {
        using RegisterT = Register<TimerSize>;

        /// %Timer prescale type
        enum struct Prescale : uint8_t {
            off = 0x00,
            d1,
            d8,
            d64,
            d256,
            d1024,
            t0_falling_edge,
            t0_rising_edge
        };
        
        /// %Timer mode
        enum struct Mode : uint8_t {
            normal = 0x00,
            pwm_phase_correct,
            clear_on_compare,
            fast_pwm
        };

        /// Interrupt trigger selection
        enum struct Trigger {
            overflow  = 0b0000'0001,
            compare_a = 0b0000'0010,
            compare_b = 0b0000'0100
        };
            
    private:
        static uint8_t const PRESCALE_BITMASK{~0b111};
        static uint8_t const MODE_BITMASK{~0b11};

        RegisterT
            timer_counter,
            compare_a,
            compare_b;

        Register8
            config_a, config_b,
            interrupt_mask,
            interrupt_flag;
        
    public:
        constexpr Timer(
            RegisterT TCNTn,
            RegisterT OCRnA,
            RegisterT OCRnB,
            Register8 TCCRnA,
            Register8 TCCRnB,
            Register8 TIMSKn,
            Register8 TIFRn
        ) :
            timer_counter  {TCNTn},
            compare_a      {OCRnA},
            compare_b      {OCRnB},
            config_a       {TCCRnA},
            config_b       {TCCRnB},
            interrupt_mask {TIMSKn},
            interrupt_flag {TIFRn}
        {}

        void set_timer_length(TimerSize duration) {
            compare_a = duration;
        }

        void set_prescale(Prescale scale) {
            config_b &= PRESCALE_BITMASK;
            config_b |= static_cast<uint8_t>(scale);
        }

        void set_mode(Mode mode) {
            config_a &= MODE_BITMASK;
            config_a |= static_cast<uint8_t>(mode);
        }
        
        void enable_interrupt(Trigger trigger) {
            interrupt_mask = static_cast<uint8_t>(trigger);
        }
    };
}

#endif
