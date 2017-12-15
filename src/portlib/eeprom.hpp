#ifndef EEPROM_ASYNC_H
#define EEPROM_ASYNC_H

#include <stdint.h>
#include <stddef.h>
#include "portlib.hpp"

namespace AvrSupport::PortLib {
    template<eeprom_size_t EEPROM_SIZE>
    struct Eeprom {
        enum struct ControlMask : uint8_t {
            read         = 0b0001,
            write        = 0b0010,
            master_write = 0b0100,
            interrupt    = 0b1000
        };

        Register8 data, control;         // EEDR, EECR
        Register<eeprom_size_t> address; // EEAR
        
        bool is_writing() {
            return control & static_cast<uint8_t>(ControlMask::write);
        }

        uint8_t read_byte(eeprom_size_t const location) {
            address = location;
            control |= static_cast<uint8_t>(ControlMask::read);
            return data;
        }

        void write_byte(eeprom_size_t const location, uint8_t const value) {
            address = location;
            data = value;
            control |= static_cast<uint8_t>(ControlMask::master_write);
            control |= static_cast<uint8_t>(ControlMask::write);
        }
    };
    
    template<eeprom_size_t EEPROM_SIZE>
    struct BufferEeprom : public Eeprom<EEPROM_SIZE> {
        using BaseEeprom = Eeprom<EEPROM_SIZE>;
        BufferEeprom(
            Register8 eedr,
            Register8 eecr,
            Register<eeprom_size_t> eear
        ) :
            Eeprom<EEPROM_SIZE>{eedr, eecr, eear}
        {}

        template<typename ReadType>
        void sync_read(eeprom_size_t location, ReadType & dest) {
            auto dest_byte = reinterpret_cast<uint8_t *>(&dest);

            while (BaseEeprom::is_writing());

            for (uint8_t _{0}; _ < sizeof(ReadType); _++) {
                *dest_byte = BaseEeprom::read_byte(location);
                location++;
                dest_byte++;
            }
        }
        
        template<typename WriteType>
        void sync_write(eeprom_size_t location, WriteType const & source) {
            auto source_byte = static_cast <uint8_t const *>(&source);

            for (uint8_t _{0}; _ < sizeof(WriteType); _++) {
                while (BaseEeprom::is_writing());

                BaseEeprom::write_byte(location, *source_byte); 
                location++;
                source_byte++;
            }
        }

        void sync_write_string(eeprom_size_t location, char const * source) {
            while (true) {
                BaseEeprom::write_byte(location, *source);
                if (!*source) break;
                location++;
                source++;
            }
        }

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
    
    template<eeprom_size_t EEPROM_SIZE, typename ValueType>
    struct ValueEeprom : public BufferEeprom<EEPROM_SIZE> {
        using BaseBufferEeprom = BufferEeprom<EEPROM_SIZE>;

        struct Storage {
            bool active; 
            ValueType value;

            static eeprom_size_t const VALUE_OFFSET{
                static_cast<eeprom_size_t>(&(static_cast<Storage *>(0)->value))
            };
        };

        eeprom_size_t location;

        ValueEeprom(
            Register8 eedr,
            Register8 eecr,
            Register<eeprom_size_t> eear
        ) :
            BaseBufferEeprom{eedr, eecr, eear},
            location{0}
        {
            move_to_active();
        }

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
        
        void sync_read(ValueType & dest) {
            BaseBufferEeprom::sync_read(
                location + Storage::VALUE_OFFSET,
                dest
            );
        }

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
