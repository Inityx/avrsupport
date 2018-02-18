#include <utility/bytewise.hpp>
#include <utility/array.hpp>
#include <utility/stddef.hpp>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

#include <cstdio>

using namespace AvrSupport;
using Utility::Array;
using Utility::Bytewise;
using Utility::BytewiseConst;
using Utility::Endian;

template<Utility::avr_size_t COUNT> using Bytes    = const Array<uint8_t, COUNT>;
template<Utility::avr_size_t COUNT> using BytesMut =       Array<uint8_t, COUNT>;

void scramble(uint8_t & byte, uint8_t & prev) {
    uint8_t temp = byte;
    byte += prev;
    prev = temp;
}

template<typename Correct, typename Tested>
bool compare(Correct correct, Tested tested) {
    for (Utility::avr_size_t i{0}; i<correct.size(); i++)
        if (correct[i] != tested[i])
            return false;

    return true;
}

void array_const_big_endian() {
    using each_byte = BytewiseConst<Bytes<5>, Endian::big>;
    Bytes<5> array{ 5, 4, 2, 3, 1 }, correct{array};
    std::vector<uint8_t> accum;

    for (uint8_t const & byte : each_byte{array}) accum.push_back(byte);
    
    assert(compare(correct, accum));
}
void array_const_little_endian() {
    using each_byte = BytewiseConst<Bytes<5>, Endian::little>;
    Bytes<5> array{ 5, 4, 2, 3, 1 }, correct{ 1, 3, 2, 4, 5};
    std::vector<uint8_t> accum;

    for (uint8_t const & byte : each_byte{array}) accum.push_back(byte);

    assert(compare(correct, accum));
}
void array_mut_big_endian() {
    using each_byte = Bytewise<BytesMut<5>, Endian::big>;
    BytesMut<5> array{ 5, 4, 2, 3, 1 };
    Bytes<5>  correct{ 5, 9, 6, 5, 4 };
    uint8_t prev{0};

    for (auto & byte : each_byte{array}) scramble(byte, prev);

    assert(compare(correct, array));
}
void array_mut_little_endian() {
    using each_byte = Bytewise<BytesMut<5>, Endian::little>;
    BytesMut<5> array{ 5, 4, 2, 3, 1 };
    Bytes<5>  correct{ 9, 6, 5, 4, 1 };
    uint8_t prev{0};

    for (auto & byte : each_byte{array}) scramble(byte, prev);

    assert(compare(correct, array));
}

void primitive_const_big_endian() {
    using each_byte = BytewiseConst<uint32_t, Endian::big>;
    uint32_t source{0xAB'CD'EF'00};
    Bytes<4> correct{ 0x00, 0xEF, 0xCD, 0xAB };
    std::vector<uint8_t> accum;
    
    for (uint8_t const & byte : each_byte{source}) accum.push_back(byte);
    
    assert(compare(correct, accum));
}
void primitive_const_little_endian() {
    using each_byte = BytewiseConst<uint32_t, Endian::little>;
    uint32_t source{0xAB'CD'EF'00};
    Bytes<4> correct{ 0xAB, 0xCD, 0xEF, 0x00 };
    std::vector<uint8_t> accum;
    
    for (uint8_t const & byte : each_byte{source}) accum.push_back(byte);
    
    assert(compare(correct, accum));
}
void primitive_mut_big_endian() {
    using each_byte = Bytewise<uint32_t, Endian::big>;
    std::vector<uint8_t> source{ 0xAB, 0xCD, 0xEF, 0x00 };
    uint32_t correct{0xAB'CD'EF'00}, dest{0};
    
    for (uint8_t & byte : each_byte{dest}) {
        byte = source.back();
        source.pop_back();
    }
    
    assert(dest == correct);
}
void primitive_mut_little_endian() {
    using each_byte = Bytewise<uint32_t, Endian::little>;
    std::vector<uint8_t> source{ 0xAB, 0xCD, 0xEF, 0x00 };
    uint32_t correct{0x00'EF'CD'AB}, dest{0};

    for (uint8_t & byte : each_byte{dest}) {
        byte = source.back();
        source.pop_back();
    }
    assert(dest == correct);
}

int main() { 
    array_const_big_endian();
    array_const_little_endian();
    array_mut_big_endian();
    array_mut_little_endian();

    primitive_const_big_endian();
    primitive_const_little_endian();
    primitive_mut_big_endian();
    primitive_mut_little_endian();

    return 0;
}
