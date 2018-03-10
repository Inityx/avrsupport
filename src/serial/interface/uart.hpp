#ifndef AVRSUPPORT_SERIAL_INTERFACE_UART_H
#define AVRSUPPORT_SERIAL_INTERFACE_UART_H

#include <utility/iterator.hpp>

namespace avrsupport::serial::interface {
    template<typename SelfClass>
    struct Uart {
        enum struct StopBits : uint8_t {
            ONE = 1,
            TWO = 2,
        };

        virtual SelfClass & set_stop_bits(StopBits const)       = 0;
        virtual bool        is_writable()                 const = 0;
        virtual uint8_t     read_byte()                   const = 0;
        virtual SelfClass & write_byte(uint8_t const)           = 0;

        SelfClass & sync_write_byte(uint8_t const value) {
            while (!is_writable());
            write_byte(value);
        }

        SelfClass & sync_write_string(char const * string) {
            for (auto c : utility::CStringChars<char const>{string})
                sync_write_byte(c);
        }
    };
}

#endif
