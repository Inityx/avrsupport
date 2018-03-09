#ifndef AVRSUPPORT_PORTLIB_INTERRUPT_H
#define AVRSUPPORT_PORTLIB_INTERRUPT_H

#ifndef __AVR
    #warning "__AVR not defined, InterruptGuard disabled"
#endif

namespace avrsupport::portlib {
    /// `cli()` when constructed, `sei()` when destructed;
    /// no-op when non-AVR platform.
    struct InterruptGuard {
        InterruptGuard() {
#ifdef __AVR
            asm("cli");
#endif
        }

        ~InterruptGuard() {
#ifdef __AVR
            asm("sei");
#endif
        }
    };
}

#endif
