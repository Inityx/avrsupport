#ifndef AVRSUPPORT_PORTLIB_TIMER_H
#define AVRSUPPORT_PORTLIB_TIMER_H

#include <stdint.h>
#include <portlib/register.hpp>

namespace avrsupport::portlib {
    /**
     * Built-in timer driver
     * @tparam TimerSize %Timer counter register type (e.g. uint8_t)
     */
    template<typename TimerSize>
    struct Timer {
        using RegisterT = Register<TimerSize>;
        using SelfClass = Timer<TimerSize>;

        /// %Timer prescale type
        enum struct Prescale : uint8_t {
            OFF = 0x00,
            D1,
            D8,
            D64,
            D256,
            D1024,
            T0_FALLING_EDGE,
            T0_RISING_EDGE
        };
        
        /// %Timer mode
        enum struct Mode : uint8_t {
            NORMAL = 0x00,
            PWM_PHASE_CORRECT,
            CLEAR_ON_COMPARE,
            FAST_PWM
        };

        /// Interrupt trigger selection
        enum struct Trigger : uint8_t {
            OVERFLOW  = 0b0000'0001,
            COMPARE_A = 0b0000'0010,
            COMPARE_B = 0b0000'0100
        };
            
    private:
        static uint8_t const PRESCALE_BITMASK{0b111};
        static uint8_t const MODE_BITMASK{0b11};

        RegisterT
            timer_counter,
            compare_a,
            compare_b;

        Register8
            config_a, config_b,
            interrupt_mask,
            interrupt_flag;
        
    public:
        Timer(
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

        SelfClass & set_timer_length(TimerSize duration) {
            compare_a = duration;
            return *this;
        }

        SelfClass & set_prescale(Prescale scale) {
            config_b &= ~PRESCALE_BITMASK;
            config_b |= static_cast<uint8_t>(scale);
            return *this;
        }

        SelfClass & set_mode(Mode mode) {
            config_a &= ~MODE_BITMASK;
            config_a |= static_cast<uint8_t>(mode);
            return *this;
        }
        
        SelfClass & enable_interrupt(Trigger trigger) {
            interrupt_mask = static_cast<uint8_t>(trigger);
            return *this;
        }
    };
}

#endif
