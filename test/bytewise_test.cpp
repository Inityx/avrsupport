#include "utility/bytewise.hpp"
#include "utility/array.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

using namespace AvrSupport::Utility;
template<typename T> using Array5 = Array<T, 5>;

void iter_const() {
    const Array5<uint8_t> array{ 5, 4, 2, 3, 1 };

    uint8_t const
        * begin = Bytewise::iter_begin(array),
        * end   = Bytewise::iter_end(array);

    assert(begin[0] == 5 && begin[1] == 4);
    assert(end == &array[5]);
}

void iter_mut() {
    Array5<uint8_t> array{ 5, 4, 2, 3, 1 };

    uint8_t
        * begin = Bytewise::iter_begin(array),
        * end   = Bytewise::iter_end(array);

    assert(begin == &array[0]);
    assert(end   == &array[5]);

    begin[0]++;
    assert(array[0] == 6);
}

void for_each_const() {
    const Array5<uint8_t> array{ 5, 4, 2, 3, 1 };    
    Array5<char> string;
    
    char * string_iter;
    auto stringifier = [&](uint8_t const byte) {
        *string_iter = static_cast<char>('0' + byte);
        string_iter++;
    };

    string_iter = &string[0];
    Bytewise::for_each_big_endian(array, stringifier);
    assert(
        strncmp(
            reinterpret_cast<char *>(&string),
            "54231",
            sizeof(string)
        ) == 0
    );

    string_iter = &string[0];
    Bytewise::for_each_little_endian(array, stringifier);
    assert(
        strncmp(
            reinterpret_cast<char *>(&string),
            "13245",
            sizeof(string)
        ) == 0
    );
}

void for_each_mut() {
    Array5<uint8_t>
        array1{ 5, 4, 2, 3, 1 },
        array2{ array1 },
        shiftenated_big_endian   { 5, 9, 6, 5, 4 },
        shiftenated_little_endian{ 9, 6, 5, 4, 1 };

    int accumulator;
    auto shiftenator = [&](uint8_t & byte) {
        uint8_t temp = byte;
        byte += accumulator;
        accumulator = temp;
    };

    accumulator = 0;
    Bytewise::for_each_big_endian(array1, shiftenator);
    assert(array1 == shiftenated_big_endian);

    accumulator = 0;
    Bytewise::for_each_little_endian(array2, shiftenator);
    assert(array2 == shiftenated_little_endian);
}

int main() { 
    iter_const();
    iter_mut();
    for_each_const();
    for_each_mut();
    return 0;
}