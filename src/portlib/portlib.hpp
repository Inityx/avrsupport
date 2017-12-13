#ifndef PORTLIB_H
#define PORTLIB_H

#include <stdint.h>

namespace AvrSupport::PortLib {
    template<typename RegSize>
    using Register = volatile RegSize &;

    using Register8 = Register<uint8_t>;
    using Register16 = Register<uint16_t>;

    typedef uint8_t PinIndex;
    typedef uint16_t eeprom_size_t;
}

#endif
