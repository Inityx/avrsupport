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
            uint8_t previous() const { return stream & 0b11; }
        };
        
        QuaternaryStream stream;

        bool turned_cw()  const { return stream == 0b00'10'01'00; }
        bool turned_ccw() const { return stream == 0b00'01'10'00; }

        void update(bool a, bool b) {
            switch (stream.previous()) {
                case 0b00: if (a == b) return;
                case 0b01: if (b)      return;
                case 0b10: if (a)      return;
                default: return;
            }

            stream.shift_in(a<<1|b);
        }
    };
}

#endif
