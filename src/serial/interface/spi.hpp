#ifndef AVRSUPPORT_SERIAL_INTERFACE_SPI
#define AVRSUPPORT_SERIAL_INTERFACE_SPI

#include <utility/array.hpp>
#include <utility/iterator.hpp>
#include <utility/stddef.hpp>

namespace avrsupport::serial::interface {
    template<typename SelfClass>
    struct Spi {
        // Virtual methods
        virtual uint8_t     read_byte     (                     ) = 0;
        virtual uint8_t     swap_byte     (uint8_t const & value) = 0;
        virtual SelfClass & write_byte    (uint8_t const & value) = 0;
        virtual SelfClass & swap_byte_into(uint8_t       & value) = 0;
        virtual bool        byte_ready    (                     ) = 0;

        template<utility::avr_size_t BUFFER_SIZE>
        SelfClass & read_bytes_into(
            utility::Array<uint8_t, BUFFER_SIZE> & buffer,
            utility::avr_size_t const count
        ) {
            using utility::Subslice;
            auto const min_size = utility::arithmetic::min<utility::avr_size_t const>;

            for (auto & byte : Subslice{buffer, min_size(count, BUFFER_SIZE)}) {
                while(!byte_ready());
                byte = read_byte();
            }
            
            return byte_ready;
        }

        template<utility::avr_size_t BUFFER_SIZE>
        bool read_string(utility::Array<char, BUFFER_SIZE> & buffer) {
            using utility::Subslice;

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
