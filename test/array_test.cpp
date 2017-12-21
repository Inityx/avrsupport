#include "utility/array.hpp"
#include <cassert>
#include <cstdint>

using AvrSupport::Utility::Array;

template<typename T, AvrSupport::Utility::array_size_t COUNT>
bool arg_value_test(
    Array<T, COUNT> const custom,
    T const * native
) {
    for(uint8_t i{0}; i<COUNT; i++)
        if (custom[i] != native[i])
            return false;
    return true;
}

template<typename T, AvrSupport::Utility::array_size_t COUNT>
bool arg_reference_test(
    Array<T, COUNT> const & custom,
    T const * native
) {
    for(uint8_t i{0}; i<COUNT; i++)
        if (custom[i] != native[i])
            return false;
    return true;
}

int main() {
    int native[5] = { 1, 2, 3, 4, 5 };
    Array<int, 5> a = { 1, 2, 3, 4, 5 };
    Array<int, 5> b = { 1, 2, 3, 4, 5 };
    Array<int, 5> c = { 1, 2, 3, 4, 6 };
    Array<int, 5> d { a };

    // Test index operator
    assert(a[3] == 4);

    // Test equality operator
    assert(a == b);
    assert(a != c);
    
    // Test copy constructor
    assert(a == d);

    // Test iteration
    uint8_t i{1};
    for(auto elem : a) {
        assert(elem == i);
        i++;
    }

    // Test argument passing
    assert(arg_value_test(a, native));
    assert(arg_reference_test(a, native));
}
