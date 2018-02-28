#ifndef AVRSUPPORT_SERIAL_INTERFACE_UART_H
#define AVRSUPPORT_SERIAL_INTERFACE_UART_H

namespace AvrSupport::Serial::Interface {
    template<typename SelfClass>
    struct Uart {
       virtual uint8_t     read_byte()               = 0;
       virtual SelfClass & write_byte(uint8_t const) = 0;
    };
}

#endif
