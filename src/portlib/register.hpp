#ifndef AVRSUPPORT_PORTLIB_REGISTER_H
#define AVRSUPPORT_PORTLIB_REGISTER_H

#include <stdint.h>

namespace avrsupport::portlib {
    /// Generic variable-size register type
    template<typename RegisterSize>
    using Register = volatile RegisterSize &;

    using Register8 = Register<uint8_t>;   ///< 8-bit register
    using Register16 = Register<uint16_t>; ///< 16-bit register
}

#endif
