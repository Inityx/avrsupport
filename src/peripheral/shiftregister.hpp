#ifndef AVRSUPPORT_PERIPHERAL_SHIFTREGISTER_H
#define AVRSUPPORT_PERIPHERAL_SHIFTREGISTER_H

#include <portlib/digitalpin.hpp>

#include <utility/array.hpp>
#include <utility/bytewise.hpp>

namespace AvrSupport::Peripheral {
    /// A latching or non-latching parallel-output shift register driver.
    struct ShiftRegister {
    private:
        PortLib::DigitalPin
            & data,
            & shift_clock,
            * const storage_clock,
            * const master_reset,
            * const output_enable;

    public:
        /**
         * Constructor.
         * Any pins passed by pointer are optional; passing `nullptr` omits pin
         * functionality. Omit `storage_clock` for non-latching shift registers.
         */
        ShiftRegister(
            PortLib::DigitalPin & data,
            PortLib::DigitalPin & shift_clock,
            PortLib::DigitalPin * const storage_clock = nullptr,
            PortLib::DigitalPin * const master_reset = nullptr,
            PortLib::DigitalPin * const output_enable = nullptr
        ) : 
            data         {data},
            shift_clock  {shift_clock},
            storage_clock{storage_clock},
            master_reset {master_reset},
            output_enable{output_enable}
        {}

        /// Enable output (`output_enable` pin).
        void enable() {
            if (output_enable)
                output_enable->set_low();
        }
        /// Disable output (`output_enable` pin).
        void disable() {
            if (output_enable)
                output_enable->set_high();
        }
        /// Pulse latch (`storage_clock` pin).
        void latch() {
            if (storage_clock) {
                storage_clock->set_high();
                storage_clock->set_low();
            }
        }
        /// Clear pins (`master_reset` pin) and pulse latch.
        void clear() {
            if (master_reset) {
                master_reset->set_low();
                master_reset->set_high();
                latch();
            }
        }
        /// Set shift clock low, enable output, clear all pins, and pulse latch.
        void initialize() {
            shift_clock.set_low();
            enable();
            clear();
        }

        /// Shift in a bit without pulsing latch.
        void shift_bit_unlatched(bool const bit) {
            data.set(bit);
            shift_clock.set_high();
            shift_clock.set_low();
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

        /// Shift in a struct or primitive, normal byte order, LSB first.
        template<typename Type>
        void shift_up_big_endian(Type const & value) {
            using each_byte = Utility::BytewiseConst<Type const, Utility::Endian::big>;

            for (auto & byte : each_byte{value})
                shift_up_unlatched(byte);
            
            latch();
        }
        /// Shift in a struct or primitive, normal byte order, MSB first.
        template<typename Type>
        void shift_down_big_endian(Type const & value) {
            using each_byte = Utility::BytewiseConst<Type const, Utility::Endian::big>;

            for (auto & byte : each_byte{value})
                shift_down_unlatched(byte);
            
            latch();
        }
        /// Shift in a struct or value, reverse byte order, LSB first.
        template<typename Type>
        void shift_up_little_endian(Type const & value) {
            using each_byte = Utility::BytewiseConst<Type const, Utility::Endian::little>;

            for (auto & byte : each_byte{value})
                shift_up_unlatched(byte);

            latch();
        }
        /// Shift in a struct or value, reverse byte order, MSB first.
        template<typename Type>
        void shift_down_little_endian(Type const & value) {
            using each_byte = Utility::BytewiseConst<Type const, Utility::Endian::little>;

            for (auto & byte : each_byte{value})
                shift_down_unlatched(byte);
            
            latch();
        }
    };
}

#endif
