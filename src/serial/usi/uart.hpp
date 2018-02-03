#ifndef AVRSUPPORT_SERIAL_USI_UART_H
#define AVRSUPPORT_SERIAL_USI_UART_H

#include <portlib/register.hpp>
#include <serial/usi/base.hpp>

namespace AvrSupport::Serial::Usi {
    /// USI-powered Universal Async Receiver/Transmitter (UART) driver
    /// @see Serial::Uart
    struct Uart : public Usi::Base<Uart> {
        Uart(
            PortLib::Register8 usidr,
            PortLib::Register8 usibr,
            PortLib::Register8 usisr,
            PortLib::Register8 usicr
        ) :
            Usi::Base<Uart>{usidr, usibr, usisr, usicr}
        {}
    };
}

#endif
