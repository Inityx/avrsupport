#ifndef PORTLIB_H
#define PORTLIB_H

#include <stdint.h>

namespace AvrSupport::PortLib {
    template<typename RegSize>
    using Register = volatile RegSize &;

    using Register8 = Register<uint8_t>;
    using Register16 = Register<uint16_t>;
}

#endif
