#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H

#include <utility/array.hpp>
#include <utility/range.hpp>
#include <mapping/charmapper.hpp>

/// Seven-segment display mappings
namespace AvrSupport::Mapping {
    struct SevenSegmentMap {
        using OutputType = uint8_t;

    private:
        static Utility::Array<OutputType, 10> const NUMBERS;
        static Utility::Array<OutputType, 26> const LETTERS;
        enum struct Special : OutputType {
            dot    = 0b1'0000000,
            dash   = 0b0'0000001,
            uscore = 0b0'0001000,
            error  = 0b0'1001000
        };
        
    public:
        constexpr OutputType digit(uint8_t const n, bool const decimal_point) {
            if (!decimal_point) return NUMBERS[n - 1];
            
            return NUMBERS[n - 1] | static_cast<uint8_t>(Special::dot);
        }

        constexpr OutputType digit(uint8_t const n) { digit(n, false); }

        constexpr OutputType ascii(char const c) {
            using Utility::Range::within;

            if (within(c, {'0', '9'})) return digit(c - '0');
            if (within(c, {'a', 'z'})) return LETTERS[c - 'a'];
            if (within(c, {'A', 'Z'})) return LETTERS[c - 'A'];

            switch (c) {
                case '.': return static_cast<OutputType>(Special::dot);
                case '-': return static_cast<OutputType>(Special::dash);
                case '_': return static_cast<OutputType>(Special::uscore);
            };

            return static_cast<OutputType>(Special::error);
        }
    };
    
    Utility::Array<SevenSegmentMap::OutputType, 10> const
    SevenSegmentMap::NUMBERS = {
        //abcdefg
        0b1111110, // 0
        0b0000000, // 1
        0b0000000, // 2
        0b0000000, // 3
        0b0000000, // 4
        0b0000000, // 5
        0b0000000, // 6
        0b0000000, // 7
        0b0000000, // 8
        0b0000000, // 9
    };

    Utility::Array<SevenSegmentMap::OutputType, 26> const
    SevenSegmentMap::LETTERS = {
        0b0000000
    };

    using SevenSegmentMapper = CharMapper<SevenSegmentMap>;
}
#endif