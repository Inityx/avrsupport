#include <utility/bytewise.hpp>
#include <utility/array.hpp>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

#include <cstdio>

using namespace AvrSupport::Utility;

template<size_t COUNT> using Bytes    = const Array<uint8_t, COUNT>;
template<size_t COUNT> using BytesMut =       Array<uint8_t, COUNT>;

void scramble(uint8_t & byte, uint8_t & prev) {
    uint8_t temp = byte;
    byte += prev;
    prev = temp;
}

template<typename Correct, typename Tested>
bool compare(Correct correct, Tested tested) {
    for (size_t i{0}; i<correct.size(); i++)
        if (correct[i] != tested[i])
            return false;

    return true;
}

void array_const_big_endian() {
    using each_byte = Bytewise::BigEndianConst<Bytes<5>>;
    Bytes<5> array{ 5, 4, 2, 3, 1 }, correct{array};
    std::vector<uint8_t> accum;

    for (uint8_t const & byte : each_byte{array}) accum.push_back(byte);
    
    assert(compare(correct, accum));
}
void array_const_little_endian() {
    using each_byte = Bytewise::LittleEndianConst<Bytes<5>>;
    Bytes<5> array{ 5, 4, 2, 3, 1 }, correct{ 1, 3, 2, 4, 5};
    std::vector<uint8_t> accum;

    for (uint8_t const & byte : each_byte{array}) accum.push_back(byte);

    assert(compare(correct, accum));
}
void array_mut_big_endian() {
    using each_byte = Bytewise::BigEndian<BytesMut<5>>;
    BytesMut<5> array{ 5, 4, 2, 3, 1 };
    Bytes<5>  correct{ 5, 9, 6, 5, 4 };
    uint8_t prev{0};

    for (auto & byte : each_byte{array}) scramble(byte, prev);

    assert(compare(correct, array));
}
void array_mut_little_endian() {
    using each_byte = Bytewise::LittleEndian<BytesMut<5>>;
    BytesMut<5> array{ 5, 4, 2, 3, 1 };
    Bytes<5>  correct{ 9, 6, 5, 4, 1 };
    uint8_t prev{0};

    for (auto & byte : each_byte{array}) scramble(byte, prev);

    assert(compare(correct, array));
}

void primitive_const_big_endian() {
    using each_byte = Bytewise::BigEndianConst<uint32_t>;
    uint32_t source{0xAB'CD'EF'00};
    Bytes<4> correct{ 0x00, 0xEF, 0xCD, 0xAB };
    std::vector<uint8_t> accum;
    
    for (uint8_t const & byte : each_byte{source}) accum.push_back(byte);
    
    assert(compare(correct, accum));
}
void primitive_const_little_endian() {
    using each_byte = Bytewise::LittleEndianConst<uint32_t>;
    uint32_t source{0xAB'CD'EF'00};
    Bytes<4> correct{ 0xAB, 0xCD, 0xEF, 0x00 };
    std::vector<uint8_t> accum;
    
    for (uint8_t const & byte : each_byte{source}) accum.push_back(byte);
    
    assert(compare(correct, accum));
}
void primitive_mut_big_endian() {
    using each_byte = Bytewise::BigEndian<uint32_t>;
    std::vector<uint8_t> source{ 0xAB, 0xCD, 0xEF, 0x00 };
    uint32_t correct{0xAB'CD'EF'00}, dest{0};
    
    for (uint8_t & byte : each_byte{dest}) {
        byte = source.back();
        source.pop_back();
    }
    
    assert(dest == correct);
}
void primitive_mut_little_endian() {
    using each_byte = Bytewise::LittleEndian<uint32_t>;
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
