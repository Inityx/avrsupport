#ifndef AVRSUPPORT_PORTLIB_EEPROM_H
#define AVRSUPPORT_PORTLIB_EEPROM_H

#include <stdint.h>

#include <portlib/register.hpp>
#include <utility/bytewise.hpp>
#include <utility/stddef.hpp>

namespace AvrSupport::PortLib {
    using RegisterE = Register<avr_size_t>;

    /**
     * A base EEPROM driver class.
     * 
     * EEPROM driver class with base methods for viewing the current write
     * status, reading a byte, and writing a byte.
     * 
     * @tparam EEPROM_SIZE Size of the device EEPROM in bytes
     */
    template<avr_size_t EEPROM_SIZE>
    struct Eeprom {
    private:
        enum struct ControlMask : uint8_t {
            read         = 0b0001,
            write        = 0b0010,
            master_write = 0b0100,
            interrupt    = 0b1000
        };

        Register8 data, control;
        RegisterE address;

    public:
        Eeprom(
            Register8 eedr,
            Register8 eecr,
            RegisterE eear
        ) :
            data{eedr},
            control{eecr},
            address{eear}
        {}

        bool is_writing() {
            return control & static_cast<uint8_t>(ControlMask::write);
        }

        uint8_t read_byte(avr_size_t const location) {
            address = location;
            control |= static_cast<uint8_t>(ControlMask::read);
            return data;
        }

        void write_byte(avr_size_t const location, uint8_t const byte) {
            address = location;
            data = byte;
            control |= static_cast<uint8_t>(ControlMask::master_write);
            control |= static_cast<uint8_t>(ControlMask::write);
        }
    };
    
    /**
     * A buffer-oriented EEPROM driver class.
     * 
     * EEPROM driver class that behaves like a buffer, with methods for reading
     * and writing arbitrary POD types and C strings.
     * 
     * @tparam EEPROM_SIZE Size of the device EEPROM in bytes
     */
    template<avr_size_t EEPROM_SIZE>
    struct BufferEeprom : private Eeprom<EEPROM_SIZE> {
    private:
        using BaseEeprom = Eeprom<EEPROM_SIZE>;

    public:
        BufferEeprom(
            Register8 eedr,
            Register8 eecr,
            RegisterE eear
        ) :
            BaseEeprom{eedr, eecr, eear}
        {}

        /**
         * Read struct or primitive synchronously.
         * @param location   EEPROM location
         * @param [out] dest Memory to write into
         */
        template<typename ReadType>
        void sync_read(avr_size_t location, ReadType & dest) {
            using EachByte = Utility::Bytewise<ReadType, Utility::Endian::big>;

            while (BaseEeprom::is_writing());

            for (uint8_t & byte : EachByte{dest}) {
                dest = BaseEeprom::read_byte(location);
                location++;
            }
        }
        
        /// Write struct or primitive synchronously.
        template<typename WriteType>
        void sync_write(avr_size_t location, WriteType const & source) {
            using EachByteConst = Utility::BytewiseConst<WriteType const, Utility::Endian::big>;

            for (uint8_t const & byte : EachByteConst{source}) {
                while (BaseEeprom::is_writing());

                BaseEeprom::write_byte(location, byte); 
                location++;
            }
        }

        /**
         * Write C-style string synchronously.
         * This function does not perform bounds checking on EEPROM addresses.
         * @param location EEPROM location
         * @param source   String to write
         */
        void sync_write_string(avr_size_t location, char const * source) {
            while (true) {
                BaseEeprom::write_byte(location, *source);
                if (!*source) break;
                location++;
                source++;
            }
        }

        /**
         * Read C-style string synchronously.
         * Reads up to `max_length` bytes, or until a null terminator. This
         * function does not add a null terminator if the max length is hit.
         * @param location   EEPROM location
         * @param [out] dest Buffer to write into
         * @param max_length Maximum number of bytes to read
         */
        void sync_read_string(
            avr_size_t location,
            char * dest,
            avr_size_t const max_length
        ) {
            for (avr_size_t _{0}; _ < max_length; _++) {
                *dest = BaseEeprom::read_byte(location);
                if (!*dest) break;
                location++;
                dest++;
            }
        };
    };
    
    /**
     * A value-oriented EEPROM driver class.
     * 
     * EEPROM driver class that behaves like a singular variable, with automatic
     * wear levelling to prevent premature degradation of the EEPROM.
     * 
     * @tparam EEPROM_SIZE Size of the device EEPROM in bytes
     */
    template<avr_size_t EEPROM_SIZE, typename ValueType>
    struct ValueEeprom : private BufferEeprom<EEPROM_SIZE> {
    private:
        using BaseBufferEeprom = BufferEeprom<EEPROM_SIZE>;

        struct Storage {
            bool active; 
            ValueType value;

            constexpr static avr_size_t const VALUE_OFFSET{
                reinterpret_cast<avr_size_t const>(
                    &(static_cast<Storage *>(0)->value)
                )
            };
        };

        avr_size_t location{0};

        void move_to_active() {
            // Increment location until active found
            while(location < EEPROM_SIZE) {
                if (is_active()) return;
                location += sizeof(Storage);
            }

            // Active not found
            location = 0;
        }
        
    public:
        /**
         * Constructor. Automatically finds the previous active location
         * from the last power cycle.
         * @param eedr Data register
         * @param eecr Control register
         * @param eear Address register
         */
        ValueEeprom(
            Register8 eedr,
            Register8 eecr,
            RegisterE eear
        ) :
            BaseBufferEeprom{eedr, eecr, eear}
        {
            move_to_active();
        }

        // Whether or not the current storage location contains active data
        bool is_active() {
            bool active;
            BaseBufferEeprom::sync_read(location, active);
            return active;
        }

        /**
         * Synchronously read value.
         * @param [out] dest Memory to read into
         */
        void sync_read(ValueType & dest) {
            BaseBufferEeprom::sync_read(
                location + Storage::VALUE_OFFSET,
                dest
            );
        }

        /**
         * Synchronously write value with automatic wear-levelling.
         * @param source Value to write
         */
        void sync_write(ValueType const & source) {
            auto prev_location = location;

            // Wear levelling
            location += sizeof(Storage);
            if (location + 1 > EEPROM_SIZE) location = 0;
            
            // Write new value
            Storage const to_write{true, source};
            BaseBufferEeprom::sync_write(location, to_write);

            // Deactivate previous
            BaseBufferEeprom::sync_write(prev_location, false);
        }
    };
}

#endif
