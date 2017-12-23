#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include <limits.h>

#include "portlib/digitalpin.hpp"

#include "utility/array.hpp"
#include "utility/bytewise.hpp"

namespace AvrSupport::Peripheral {
    /**
     * Shift Register
     * 
     * A latching or Non-latching shift register driver. Data and shift clock
     * are the only required pins; any pins passed `nullptr` will have their
     * functionality omitted. Simply omit storage clock or pass it `nullptr`
     * for non-latching shift registers.
     * 
     * @param data
     * @param shift_clock
     * @param storage_clock
     * @param master_reset
     * @param output_enable
     */
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

        /** Enable output. */
        void enable() {
            if constexpr (output_enable)
                output_enable->set_low();
        }
        /** Disable output. */
        void disable() {
            if constexpr (output_enable)
                output_enable->set_high();
        }
        /** Pulse storage clock (latch) pin. */
        void latch() {
            if constexpr (storage_clock) {
                storage_clock->set_high();
                storage_clock->set_low();
            }
        }
        /** Clear pins and pulse latch. */
        void clear() {
            if constexpr (master_reset) {
                master_reset->set_low();
                master_reset->set_high();
                latch();
            }
        }
        /**
         * Initialize shift register driver.
         * Set shift clock low, enable output, clear all pins, and pulse latch.
         */
        void initialize() {
            shift_clock->set_low();
            enable();
            clear();
        }

        /** Shift in a bit without pulsing latch. */
        void shift_bit_unlatched(bool value) {
            data->set(value);
            shift_clock->set_high();
            shift_clock->set_low();
        }
        /** Shift in a byte LSB first without pulsing latch. */
        void shift_up_unlatched(uint8_t value) {
            for(uint8_t mask{1}; mask; mask <<= 1)
                shift_bit_unlatched(value & mask);
        }
        /** Shift in a byte MSB first without pulsing latch. */
        void shift_down_unlatched(uint8_t value) {
            for(uint8_t mask{0b1000'0000}; mask; mask >>= 1)
                shift_bit_unlatched(value & mask);
        }

        /** Shift in a bit and pulse latch. */
        void shift_bit(bool value) {
            shift_bit_unlatched(value);
            latch();
        }
        /** Shift in a byte LSB first and pulse latch. */
        void shift_up(uint8_t value) {
            shift_up_unlatched(value);
            latch();
        }
        /** Shift in a byte MSB first and pulse latch. */
        void shift_down(uint8_t value) {
            shift_down_unlatched(value);
            latch();
        }

        /**
         * Shift LSB first/big endian and pulse latch.
         * Shift a struct or value with bytes in increasing order, LSB first.
         */
        template<typename Type>
        void shift_up_big_endian(Type const & value) {
            using each_byte = Utility::Bytewise::BigEndianConst<Type>;
            for (uint8_t byte : each_byte{value})
                shift_up_unlatched(byte);
            latch();
        }
        /**
         * Shift MSB first/big endian and pulse latch.
         * Shift a struct or value with bytes in increasing order, MSB first.
         */
        template<typename Type>
        void shift_down_big_endian(Type const & value) {
            using each_byte = Utility::Bytewise::BigEndianConst<Type>;
            for (uint8_t byte : each_byte{value})
                shift_down_unlatched(byte);
            latch();
        }
        /**
         * Shift LSB first/little endian and pulse latch.
         * Shift a struct or value with bytes in decreasing order, LSB first.
         */
        template<typename Type>
        void shift_up_little_endian(Type const & value) {
            using each_byte = Utility::Bytewise::LittleEndianConst<Type>;
            for (uint8_t byte : each_byte{value})
                shift_up_unlatched(byte);
            latch();
        }
        /**
         * Shift MSB first/little endian and pulse latch.
         * Shift a struct or value with bytes in decreasing order, MSB first.
         */
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