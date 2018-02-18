#ifndef AVRSUPPORT_SERIAL_INTERFACE_SPI
#define AVRSUPPORT_SERIAL_INTERFACE_SPI

#include <utility/array.hpp>
#include <utility/iterator.hpp>
#include <utility/stddef.hpp>

namespace AvrSupport::Serial::Interface {
    template<typename SelfClass>
    struct Spi {
        virtual SelfClass & initialize() = 0;

        // Virtual methods
        virtual uint8_t     read_byte     (                     ) = 0;
        virtual uint8_t     swap_byte     (uint8_t const & value) = 0;
        virtual SelfClass & write_byte    (uint8_t const & value) = 0;
        virtual SelfClass & swap_byte_into(uint8_t       & value) = 0;
        virtual bool        byte_ready    (                     ) = 0;

        template<Utility::avr_size_t BUFFER_SIZE>
        SelfClass & read_bytes_into(
            Utility::Array<uint8_t, BUFFER_SIZE> & buffer,
            Utility::avr_size_t const count
        ) {
            using Utility::Subslice;
            auto const min_size = Utility::Arithmetic::min<Utility::avr_size_t const>;

            for (auto & byte : Subslice{buffer, min_size(count, BUFFER_SIZE)}) {
                while(!byte_ready());
                byte = read_byte();
            }
            
            return byte_ready;
        }

        template<Utility::avr_size_t BUFFER_SIZE>
        bool read_string(Utility::Array<char, BUFFER_SIZE> & buffer) {
            using Utility::Subslice;

            for (auto & character : Subslice{buffer, BUFFER_SIZE - 1}) {
                while(!byte_ready());
                character = read_byte();
                if (character == '\0')
                    return false;
            }

            buffer[BUFFER_SIZE - 1] = '\0';
            return true;
        }
    };
}

#endif
