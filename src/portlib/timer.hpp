#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "portlib.hpp"

namespace AvrSupport::PortLib {
    template<typename TimerSize>
    struct Timer {
        using RegisterT = Register<TimerSize>;

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
        
        enum struct Mode : uint8_t {
            normal = 0x00,
            pwm_phase_correct,
            clear_on_compare,
            fast_pwm
        };

        enum struct Trigger {
            overflow  = 0b0000'0001,
            compare_a = 0b0000'0010,
            compare_b = 0b0000'0100
        };
            
    private:
        static uint8_t const PRESCALE_BITMASK{0b1111'1000};
        static uint16_t const MODE_BITMASK{0b0000'0011};

        RegisterT
            timer_counter,  // TCNTn
            compare_a, compare_b; // OCRnA, OCRnB

        Register8
            config_a, config_b,   // TCCRnA, TCCRnB
            interrupt_mask,       // TIMSKn
            interrupt_flag;       // TIFRn
        
    public:
        constexpr Timer(
            RegisterT timer_counter,
            RegisterT compare_a,
            RegisterT compare_b,
            Register8 config_a,
            Register8 config_b,
            Register8 interrupt_mask,
            Register8 interrupt_flag
        ) :
            timer_counter{timer_counter},
            compare_a{compare_a}, compare_b{compare_b},
            config_a{config_a}, config_b{config_b},
            interrupt_mask{interrupt_mask},
            interrupt_flag{interrupt_flag}
        {}

        void set_timer_length(TimerSize duration) {
            compare_a = duration;
        }

        void set_prescale(Prescale scale) {
            config_b &= PRESCALE_BITMASK;
            config_b |= static_cast<uint8_t>(scale);
        }

        void set_mode(Mode mode) {
            config_a &= ~MODE_BITMASK;
            config_a |= static_cast<uint8_t>(mode);
        }
        
        void enable_interrupt(Trigger trigger) {
            interrupt_mask = static_cast<uint8_t>(trigger);
        }
    };
}

#endif
