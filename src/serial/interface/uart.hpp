#ifndef AVRSUPPORT_SERIAL_INTERFACE_UART_H
#define AVRSUPPORT_SERIAL_INTERFACE_UART_H

namespace avrsupport::serial::interface {
    template<typename SelfClass>
    struct Uart {
        enum struct StopBits : uint8_t {
            one = 1,
            two = 2,
        };

        virtual SelfClass & set_stop_bits(StopBits const) = 0;

        virtual uint8_t     read_byte()               = 0;
        virtual SelfClass & write_byte(uint8_t const) = 0;
    };
}

#endif
