#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <stdint.h>

namespace AVRSupport::Peripheral {
    struct RotaryEncoder {
        struct QuaternaryStream {
            uint8_t stream;
            
            constexpr QuaternaryStream() : stream(0) {}
            bool operator==(uint8_t const rhs) const { return stream == rhs; }

            void shift_in(uint8_t const state) { stream = stream<<2 | state; }
            void unshift() { stream >>= 2; }
            uint8_t previous()    const { return (stream & 0b0011);    }
            uint8_t penultimate() const { return (stream & 0b1100)>>2; }
            void clear() { stream = 0; }
        };
        
        QuaternaryStream stream;

        bool turned_right() const { return stream == 0b00'10'01'00; }
        bool turned_left()  const { return stream == 0b00'01'10'00; }

        bool shift_mid_state(bool a, bool b, uint8_t join) {
                if (b) return false; // Ignore
                if (!a) return true; // Accept ground
                if (stream.penultimate() == join) {
                    // Reject backwards partial
                    stream.unshift();
                    return false;
                }
                return true; // Accept middle transition
        }

        void update(bool a, bool b) {
            uint8_t join = a<<1|b;

            switch (stream.previous()) {
                case 0b00: if (a == b)                       return; break;
                case 0b01: if (!shift_mid_state(a, b, join)) return; break;
                case 0b10: if (!shift_mid_state(b, a, join)) return; break;
                default: return;
            }

            stream.shift_in(join);
        }

        void clear() { stream.clear(); }
    };
}

#endif
