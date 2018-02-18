#ifndef AVRSUPPORT_UTILTY_STDDEF_H
#define AVRSUPPORT_UTILTY_STDDEF_H

#include <stdint.h>

namespace AvrSupport::Utility {
    using avr_ptrdiff_t = int16_t;
    using avr_size_t = uint16_t;

    enum struct LogicLevel : bool { low, high };
    enum struct IoDirection : bool { in, out };
    enum struct Endian : bool { big, little };
}

#endif
