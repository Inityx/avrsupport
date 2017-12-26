#ifndef EEPROM_ASYNC_H
#define EEPROM_ASYNC_H

#include <stdint.h>
#include <stddef.h>

#include <portlib/register.hpp>
#include <utility/bytewise.hpp>

namespace AvrSupport::PortLib {
    using eeprom_size_t = uint16_t;

    /**
     * A base EEPROM driver class.
     * 
     * EEPROM driver class with base methods for viewing the current write
     * status, reading a byte, and writing a byte.
     * 
     * @tparam EEPROM_SIZE Size of the device EEPROM in bytes
     */
    template<eeprom_size_t EEPROM_SIZE>
    struct Eeprom {
    private:
        enum struct ControlMask : uint8_t {
            read         = 0b0001,
            write        = 0b0010,
            master_write = 0b0100,
            interrupt    = 0b1000
        };

        Register8 data, control;
        Register<eeprom_size_t> address;

    public:
        /**
         * Constructor.
         * @param EEDR Data register
         * @param EECR Control register
         * @param EEAR Address register
         */
        constexpr Eeprom(
            Register8 EEDR,
            Register8 EECR,
            Register<eeprom_size_t> EEAR
        ) :
            data{EEDR},
            control{EECR},
            address{EEAR}
        {}

        bool is_writing() {
            return control & static_cast<uint8_t>(ControlMask::write);
        }

        uint8_t read_byte(eeprom_size_t const location) {
            address = location;
            control |= static_cast<uint8_t>(ControlMask::read);
            return data;
        }

        void write_byte(eeprom_size_t const location, uint8_t const byte) {
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
    template<eeprom_size_t EEPROM_SIZE>
    struct BufferEeprom : private Eeprom<EEPROM_SIZE> {
    private:
        using BaseEeprom = Eeprom<EEPROM_SIZE>;

    public:
        /**
         * Constructor.
         * @param EEDR Data register
         * @param EECR Control register
         * @param EEAR Address register
         */
        constexpr BufferEeprom(
            Register8 EEDR,
            Register8 EECR,
            Register<eeprom_size_t> EEAR
        ) :
            BaseEeprom{EEDR, EECR, EEAR}
        {}

        /**
         * Read struct or primitive synchronously.
         * @param location   EEPROM location
         * @param [out] dest Memory to write into
         */
        template<typename ReadType>
        void sync_read(eeprom_size_t location, ReadType & dest) {
            using EachByte = Utility::Bytewise::BigEndian<ReadType>;

            while (BaseEeprom::is_writing());

            for (uint8_t & byte : EachByte{dest}) {
                dest = BaseEeprom::read_byte(location);
                location++;
            }
        }
        
        /// Write struct or primitive synchronously.
        template<typename WriteType>
        void sync_write(eeprom_size_t location, WriteType const & source) {
            using EachByteConst = Utility::Bytewise::BigEndianConst<WriteType>;

            for (uint8_t const & byte : EachByteConst{source}) {
                while (BaseEeprom::is_writing());

                BaseEeprom::write_byte(location, byte); 
                location++;
            }
        }

        /// Write C-style string synchronously.
        void sync_write_string(eeprom_size_t location, char const * source) {
            while (true) {
                BaseEeprom::write_byte(location, *source);
                if (!*source) break;
                location++;
                source++;
            }
        }

        /**
         * Read C-style string synchronously.
         * @param location   EEPROM location
         * @param [out] dest Destination buffer
         * @param max_length Maximum number of bytes to read
         */
        void sync_read_string(
            eeprom_size_t location,
            char * dest,
            eeprom_size_t const max_length
        ) {
            for (eeprom_size_t _{0}; _ < max_length; _++) {
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
    template<eeprom_size_t EEPROM_SIZE, typename ValueType>
    struct ValueEeprom : private BufferEeprom<EEPROM_SIZE> {
    private:
        using BaseBufferEeprom = BufferEeprom<EEPROM_SIZE>;

        struct Storage {
            bool active; 
            ValueType value;

            static eeprom_size_t const VALUE_OFFSET{
                static_cast<eeprom_size_t const>(
                    &(static_cast<Storage *>(0)->value)
                )
            };
        };

        eeprom_size_t location;

        void move_to_active() {
            // Increment location until active found
            while(location < EEPROM_SIZE) {
                bool active;
                BaseBufferEeprom::sync_read(location, active);
                if (active) return;
                location += sizeof(Storage);
            }

            // Active not found
            location = 0;
        }
        
    public:
        /**
         * Constructor. Automatically finds the previous active location
         * from the last power cycle.
         * @param EEDR Data register
         * @param EECR Control register
         * @param EEAR Address register
         */
        ValueEeprom(
            Register8 EEDR,
            Register8 EECR,
            Register<eeprom_size_t> EEAR
        ) :
            BaseBufferEeprom{EEDR, EECR, EEAR},
            location{0}
        {
            move_to_active();
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
            // Wear levelling
            location += sizeof(Storage);
            if (location + 1 > EEPROM_SIZE) location = 0;
            
            // Write new value
            BaseBufferEeprom::sync_write(location, { true, source });

            // Deactivate previous
            if (location > 0)
                BaseBufferEeprom::sync_write(location - sizeof(Storage), false);
        }
    };
}

#endif
