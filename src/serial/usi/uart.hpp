#ifndef AVRSUPPORT_SERIAL_USI_UART_H
#define AVRSUPPORT_SERIAL_USI_UART_H

#include <portlib/register.hpp>
#include <serial/usi/base.hpp>

namespace avrsupport::serial::usi {
    /// USI-powered Universal Async Receiver/Transmitter (UART) driver
    /// @see Serial::Uart
    struct Uart : public usi::Base<Uart> {
        Uart(
            portlib::Register8 usidr,
            portlib::Register8 usibr,
            portlib::Register8 usisr,
            portlib::Register8 usicr
        ) :
            usi::Base<Uart>{usidr, usibr, usisr, usicr}
        {}
    };
}

#endif
