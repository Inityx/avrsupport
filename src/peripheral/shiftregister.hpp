#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include <portlib/digitalpin.hpp>

#include <utility/array.hpp>
#include <utility/bytewise.hpp>

namespace AvrSupport::Peripheral {
    /**
     * A latching or non-latching shift register driver.
     * 
     * Data and shift clock are the only required pins; any pins passed
     * `nullptr` will have their functionality omitted. Simply omit
     * `storage_clock` or pass it `nullptr` for use with non-latching shift
     * registers.
     * 
     * @tparam data          Data pin
     * @tparam shift_clock   Shift clock pin
     * @tparam storage_clock Storage clock (latch) pin
     * @tparam master_reset  Master reset pin
     * @tparam output_enable Output enable pin
     */
    template<
        PortLib::DigitalPin * const data,
        PortLib::DigitalPin * const shift_clock,
        PortLib::DigitalPin * const storage_clock = nullptr,
        PortLib::DigitalPin * const master_reset = nullptr,
        PortLib::DigitalPin * const output_enable = nullptr
    >
    struct ShiftRegister {
        static_assert(data,        "Data pin must not be null.");
        static_assert(shift_clock, "Shift clock pin must not be null.");

        /// Enable output (`output_enable` pin).
        void enable() {
            if constexpr (output_enable)
                output_enable->set_low();
        }
        /// Disable output (`output_enable` pin).
        void disable() {
            if constexpr (output_enable)
                output_enable->set_high();
        }
        /// Pulse latch (`storage_clock` pin).
        void latch() {
            if constexpr (storage_clock) {
                storage_clock->set_high();
                storage_clock->set_low();
            }
        }
        /// Clear pins (`master_reset` pin) and pulse latch.
        void clear() {
            if constexpr (master_reset) {
                master_reset->set_low();
                master_reset->set_high();
                latch();
            }
        }
        /// Set shift clock low, enable output, clear all pins, and pulse latch.
        void initialize() {
            shift_clock->set_low();
            enable();
            clear();
        }

        /// Shift in a bit without pulsing latch.
        void shift_bit_unlatched(bool const bit) {
            data->set(bit);
            shift_clock->set_high();
            shift_clock->set_low();
        }
        /// Shift in a byte LSB-first without pulsing latch.
        void shift_up_unlatched(uint8_t const byte) {
            for(uint8_t mask{1}; mask; mask <<= 1)
                shift_bit_unlatched(byte & mask);
        }
        /// Shift in a byte MSB-first without pulsing latch.
        void shift_down_unlatched(uint8_t const byte) {
            for(uint8_t mask{0b1000'0000}; mask; mask >>= 1)
                shift_bit_unlatched(byte & mask);
        }

        /// Shift in a bit and pulse latch.
        void shift_bit(bool bit) {
            shift_bit_unlatched(bit);
            latch();
        }
        /// Shift in a byte LSB-first and pulse latch.
        void shift_up(uint8_t byte) {
            shift_up_unlatched(byte);
            latch();
        }
        /// Shift in a byte MSB-first and pulse latch.
        void shift_down(uint8_t byte) {
            shift_down_unlatched(byte);
            latch();
        }

        /// Shift in a struct or primitive, bytes in increasing order, LSB first.
        template<typename Type>
        void shift_up_big_endian(Type const & value) {
            using each_byte = Utility::Bytewise::BigEndianConst<Type>;

            for (uint8_t byte : each_byte{value})
                shift_up_unlatched(byte);
            
            latch();
        }
        /// Shift in a struct or primitive, bytes in increasing order, MSB first.
        template<typename Type>
        void shift_down_big_endian(Type const & value) {
            using each_byte = Utility::Bytewise::BigEndianConst<Type>;

            for (uint8_t byte : each_byte{value})
                shift_down_unlatched(byte);
            
            latch();
        }
        /// Shift in a struct or value, bytes in decreasing order, LSB first.
        template<typename Type>
        void shift_up_little_endian(Type const & value) {
            using each_byte = Utility::Bytewise::LittleEndianConst<Type>;

            for (uint8_t byte : each_byte{value})
                shift_up_unlatched(byte);

            latch();
        }
        /// Shift in a struct or value, bytes in decreasing order, MSB first.
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
