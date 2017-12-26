#ifndef PORTLIB_H
#define PORTLIB_H

#include <stdint.h>

namespace AvrSupport::PortLib {
    /// Generic variable-size register type
    template<typename RegisterSize>
    using Register = volatile RegisterSize &;

    using Register8 = Register<uint8_t>;   ///< 8-bit register
    using Register16 = Register<uint16_t>; ///< 16-bit register
}

#endif
