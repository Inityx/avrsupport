#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include <limits.h>

#include "portlib/digitalpin.hpp"

#include "utility/array.hpp"
#include "utility/bytewise.hpp"

namespace AvrSupport::Peripheral {
    template<
        PortLib::DigitalPin * const data,
        PortLib::DigitalPin * const shift_clock,
        PortLib::DigitalPin * const storage_clock = nullptr,
        PortLib::DigitalPin * const master_reset = nullptr,
        PortLib::DigitalPin * const output_enable = nullptr
    >
    struct ShiftRegister {
        static_assert(data, "Data pin must not be null.");
        static_assert(shift_clock, "Shift clock pin must not be null.");

        void enable() {
            if constexpr (output_enable)
                output_enable->set_low();
        }
        void disable() {
            if constexpr (output_enable)
                output_enable->set_high();
        }
        void latch() {
            if constexpr (storage_clock) {
                storage_clock->set_high();
                storage_clock->set_low();
            }
        }
        void clear() {
            if constexpr (master_reset) {
                master_reset->set_low();
                master_reset->set_high();
                latch();
            }
        }
        void initialize() {
            shift_clock->set_low();
            enable();
            clear();
        }

        void shift_bit_unlatched(bool value) {
            data->set(value);
            shift_clock->set_high();
            shift_clock->set_low();
        }
        void shift_up_unlatched(uint8_t value) {
            for(uint8_t mask{1}; mask; mask <<= 1)
                shift_bit_unlatched(value & mask);
        }
        void shift_down_unlatched(uint8_t value) {
            for(uint8_t mask{0b1000'0000}; mask; mask >>= 1)
                shift_bit_unlatched(value & mask);
        }

        void shift_bit(bool value) {
            shift_bit_unlatched(value);
            latch();
        }
        void shift_up(uint8_t value) {
            shift_up_unlatched(value);
            latch();
        }
        void shift_down(uint8_t value) {
            shift_down_unlatched(value);
            latch();
        }

        template<typename Type>
        void shift_up_big_endian(Type const & value) {
            using each_byte = Utility::Bytewise::BigEndianConst<Type>;
            for (uint8_t byte : each_byte{value})
                shift_up_unlatched(byte);
            latch();
        }
        template<typename Type>
        void shift_down_big_endian(Type const & value) {
            using each_byte = Utility::Bytewise::BigEndianConst<Type>;
            for (uint8_t byte : each_byte{value})
                shift_down_unlatched(byte);
            latch();
        }
        template<typename Type>
        void shift_up_little_endian(Type const & value) {
            using each_byte = Utility::Bytewise::LittleEndianConst<Type>;
            for (uint8_t byte : each_byte{value})
                shift_up_unlatched(byte);
            latch();
        }
        template<typename Type>
        void shift_down_little_endian(Type const & value) {
            using each_byte = Utility::Bytewise::LittleEndianConst<Type>;
            for (uint8_t byte : each_byte{value})
                shift_down_unlatched(byte);
            latch();
        }
    };
}

#endif