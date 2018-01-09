#ifndef AVRSUPPORT_SERIAL_SPI_H
#define AVRSUPPORT_SERIAL_SPI_H

#include <portlib/register.hpp>

namespace AvrSupport::Serial {
    /// Native %Serial %Peripheral Interface (SPI) driver
    /// @see Usi::Spi
    struct Spi{
        Register8
            data,
            control,
            status;
    };
}

#endif
