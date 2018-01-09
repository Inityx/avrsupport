#ifndef AVRSUPPORT_SERIAL_UART_H
#define AVRSUPPORT_SERIAL_UART_H

#include <portlib/register.hpp>

namespace AvrSupport::Serial {
    struct Uart {
        Register8
            data;
    };
}
#endif
