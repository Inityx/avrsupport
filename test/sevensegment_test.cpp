#include <cassert>
#include <mapping/sevensegment.hpp>

using namespace avrsupport;
using namespace mapping;
using utility::Array;
using SSM = SevenSegmentMap;

void map_digits_test() {
    static_assert(SSM::digit(0x8      ) == 0b0'1111111);
    static_assert(SSM::digit(0xF      ) == 0b0'1000111);
    static_assert(SSM::digit(0x1, true) == 0b1'0110000);
}

bool constexpr ascii_caps_test() {
    for (char c{'a'}, C{'A'}; c < 'z'+1; c++, C++) {
        if (SSM::ascii(c) != SSM::ascii(C))
            return false;
    }
    return true;
}

void map_ascii_test() {
    static_assert(SSM::ascii('a') == 0b0'1110111);
    static_assert(SSM::ascii('z') == 0b0'1101101);

    static_assert(ascii_caps_test());
}

void mapper_string_test() {
    constexpr Array<SSM::Encoding, 9> const DEADBEEF {
        0b0'0111101, // d
        0b0'1001111, // E
        0b0'1110111, // A
        0b0'0111101, // d
        0b0'0011111, // b
        0b0'1001111, // E
        0b0'1001111, // E
        0b0'1000111, // F
        0b1'0000000, // .
    };
    constexpr Array<SSM::Encoding, 9> const RESULT{
        SevenSegmentFrom::collected<9>("DeAdBeEf.")
    };
    static_assert(RESULT == DEADBEEF);
}

int main() {
    map_digits_test();
    map_ascii_test();
    mapper_string_test();
}
