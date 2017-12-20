#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include "portlib/digitalpin.hpp"

namespace AvrSupport::Peripheral {
    using PortLib::DigitalPin;

    template<
        DigitalPin * const data,
        DigitalPin * const clock,
        DigitalPin * const latch,
        DigitalPin * const master_reset,
        DigitalPin * const output_enable
    >
    struct ShiftRegister {
        static_assert(data);
        static_assert(clock);

        void enable() {
            if constexpr (output_enable)
                output_enable->set_low();
        }
        void disable() {
            if constexpr (output_enable)
                output_enable->set_high();
        }
        void update_latch() {
            if constexpr (latch) {
                latch->set_high();
                latch->set_low();
            }
        }
        void reset() {
            if constexpr (master_reset) {
                master_reset->set_low();
                master_reset->set_high();
                update_latch();
            }
        }
        void initialize() {
            clock->set_low();
            reset();
            enable();
        }

        void shift_in_unlatched(bool value) {
            data->set(value);
            clock->set_high();
            clock->set_low();
        }
        void shift_in(bool value) {
            shift_in_unlatched(value);
            update_latch();
        }
        void shift_byte_up(uint8_t value) {
            for(uint8_t mask{1}; mask; mask <<= 1)
                shift_in_unlatched(value & mask);
            update_latch();
        }
        void shift_byte_down(uint8_t value) {
            for(uint8_t mask{0b1000'0000}; mask; mask >>= 1)
                shift_in_unlatched(value & mask);
            update_latch();
        }
    };
}

#endif