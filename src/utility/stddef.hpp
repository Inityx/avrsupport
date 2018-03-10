#ifndef AVRSUPPORT_UTILTY_STDDEF_H
#define AVRSUPPORT_UTILTY_STDDEF_H

#include <stdint.h>

namespace avrsupport::utility {
    using avr_ptrdiff_t = int16_t;
    using avr_size_t = uint16_t;

    enum struct LogicLevel : bool { LOW, HIGH };
    enum struct IoDirection : bool { IN, OUT };
    enum struct Endian : bool { BIG, LITTLE };
}

#endif
