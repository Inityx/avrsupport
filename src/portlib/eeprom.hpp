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
        
        bool is_writing() { return control & ControlMask::write; }

        uint8_t read_byte(eeprom_size_t const location) {
            address = location;
            control |= ControlMask::read;
            return data;
        }

        void write_byte(eeprom_size_t const location, uint8_t const value) {
            address = location;
            data = value;
            control |= ControlMask::master_write;
            control |= ControlMask::write;
        }
    };
    
    template<eeprom_size_t EEPROM_SIZE>
    struct BufferEeprom : public Eeprom<EEPROM_SIZE> {
        using Eeprom = Eeprom<EEPROM_SIZE>;

        BufferEeprom(
            Register8 eedr,
            Register8 eecr,
            Register<eeprom_size_t> eear
        ) :
            Eeprom{eedr, eecr, eear}
        {}

        template<typename ReadType>
        void sync_read(eeprom_size_t location, ReadType & dest) {
            auto dest_byte = static_cast<uint8_t *>(&dest);

            while (Eeprom::is_writing());

            for (uint8_t _{0}; _ < sizeof(ReadType); _++) {
                *dest_byte = Eeprom::read_byte(location);
                location++;
                dest_byte++;
            }
        }
        
        template<typename WriteType>
        void sync_write(eeprom_size_t location, WriteType const & source) {
            auto source_byte = static_cast <uint8_t const *>(&source);

            for (uint8_t _{0}; _ < sizeof(WriteType); _++) {
                while (Eeprom::is_writing());

                Eeprom::write_byte(location, *source_byte); 
                location++;
                source_byte++;
            }
        }
    };
    
    template<eeprom_size_t EEPROM_SIZE, typename StorageType>
    struct ValueEeprom : public BufferEeprom<EEPROM_SIZE> {
        using BufferEeprom = BufferEeprom<EEPROM_SIZE>;

        StorageType *location;

        ValueEeprom(
            Register8 eedr,
            Register8 eecr,
            Register<eeprom_size_t> eear
        ) :
            BufferEeprom{eedr, eecr, eear},
            location{0}
        {}

        void sync_read(StorageType & dest) {
            BufferEeprom::template sync_read<StorageType>(location, dest);
        }

        void sync_write(StorageType const & source) {
            // Wear levelling
            location++;
            if (location + 1 > EEPROM_SIZE) location = 0;
            
            BufferEeprom::template sync_write<StorageType>(location, source);
        }
    };
}

#endif
