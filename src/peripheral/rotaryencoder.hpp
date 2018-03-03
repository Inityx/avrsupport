#ifndef AVRSUPPORT_PERIPHERAL_ROTARYENCODER_H
#define AVRSUPPORT_PERIPHERAL_ROTARYENCODER_H

#include <stdint.h>

namespace avrsupport::peripheral {
    /// A rotary encoder driver
    struct RotaryEncoder {
    private:
        struct QuaternaryStream {
            uint8_t stream{0};
            
            bool operator==(uint8_t const rhs) const { return stream == rhs; }

            void shift_in(uint8_t const state) { stream = stream<<2 | state; }
            void unshift() { stream >>= 2; }
            uint8_t prev()   const { return stream    & 0b11; }
            uint8_t penult() const { return stream>>2 & 0b11; }
            void clear() { stream = 0; }
        };

        QuaternaryStream stream;

        bool shift_mid_state(bool const a, bool const b, uint8_t const join) {
            if ( b) return false; // Ignore
            if (!a) return true; // Accept ground
            if (stream.penult() != join) return true; // Accept middle transition
            // Reject backwards partial
            stream.unshift();
            return false;
        }

    public:
        bool turned_right() const { return stream == 0b00'10'01'00; }
        bool turned_left()  const { return stream == 0b00'01'10'00; }

        /**
         * Update rotary encoder state.
         * @param a Encoder pin value A
         * @param b Encoder pin value B
         */
        void sample(bool const a, bool const b) {
            uint8_t join = a<<1 | b;

            switch (stream.prev()) {
                case 0b00: if (a == b)                       return; break;
                case 0b01: if (!shift_mid_state(a, b, join)) return; break;
                case 0b10: if (!shift_mid_state(b, a, join)) return; break;
                default: return;
            }

            stream.shift_in(join);
        }

        /// Clear rotary encoder turn status
        void clear() { stream.clear(); }
    };
}

#endif
