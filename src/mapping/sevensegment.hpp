#ifndef AVRSUPPORT_MAPPING_SEVENSEGMENT_H
#define AVRSUPPORT_MAPPING_SEVENSEGMENT_H

#include <stdint.h>

#include <mapping/encodedwith.hpp>
#include <utility/array.hpp>
#include <utility/arithmetic.hpp>
#include <utility/stddef.hpp>

namespace avrsupport::mapping {
    /// Seven-segment display character mappings
    struct SevenSegmentMap {
        using Encoding = uint8_t; ///< Segments `.abcdefg`

    private:
        constexpr static utility::avr_ptrdiff_t ALPHA_OFFSET = 10;
        constexpr static utility::Array<Encoding, 36> const CHARS{
            //. abcdefg
            0b0'1111110, // 0
            0b0'0110000, // 1
            0b0'1101101, // 2
            0b0'1111001, // 3
            0b0'0110011, // 4
            0b0'1011011, // 5
            0b0'1011111, // 6
            0b0'1110000, // 7
            0b0'1111111, // 8
            0b0'1111011, // 9

            0b0'1110111, // A
            0b0'0011111, // B
            0b0'1001110, // C
            0b0'0111101, // D
            0b0'1001111, // E
            0b0'1000111, // F
            
            0b0'1011111, // G
            0b0'0110111, // H
            0b0'0110000, // I
            0b0'0111100, // J
            0b0'1010111, // K
            0b0'0001110, // L
            0b0'1010101, // M
            0b0'0010101, // N
            0b0'0011101, // O
            0b0'1100111, // P
            0b0'1110011, // Q
            0b0'0000101, // R
            0b0'1011011, // S
            0b0'0001111, // T
            0b0'0111110, // U
            0b0'0011100, // V
            0b0'0101011, // W
            0b0'1011001, // X
            0b0'0111011, // Y
            0b0'1101101, // Z
        };
        enum struct Special : Encoding {
            //         . abcdefg
            SPACE  = 0b0'0000000,
            DOT    = 0b1'0000000,
            DASH   = 0b0'0000001,
            USCORE = 0b0'0001000,
            ERROR  = 0b0'1001000
        };
        
    public:
        constexpr static Encoding digit(
            uint8_t const n,
            bool const decimal_point = false
        ) {
            if (!decimal_point) return CHARS[n];
            
            return CHARS[n] | static_cast<uint8_t>(Special::DOT);
        }

        constexpr static Encoding ascii(char const c) {
            using utility::arithmetic::within;

            if (within(c, {'0', '9'})) return digit(c - '0');
            if (within(c, {'a', 'z'})) return CHARS[c - 'a' + ALPHA_OFFSET];
            if (within(c, {'A', 'Z'})) return CHARS[c - 'A' + ALPHA_OFFSET];

            switch (c) {
                case ' ': return static_cast<Encoding>(Special::SPACE);
                case '.': return static_cast<Encoding>(Special::DOT);
                case '-': return static_cast<Encoding>(Special::DASH);
                case '_': return static_cast<Encoding>(Special::USCORE);
            };

            return static_cast<Encoding>(Special::ERROR);
        }
    };
    
    using SevenSegmentFrom = EncodedWith<SevenSegmentMap>;
}
#endif
