#ifndef PORTLIB_H
#define PORTLIB_H

#include <stdint.h>

namespace AVRSupport::PortLib {
    template<typename RegSize>
    using Register = volatile RegSize &;

    using Register8 = Register<uint8_t>;

    typedef uint8_t PinIndex;
}

#endif
