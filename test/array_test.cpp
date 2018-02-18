#include <utility/array.hpp>
#include <utility/stddef.hpp>

#include <cassert>
#include <cstdint>

using namespace AvrSupport;
using Utility::Array;

template<typename T, Utility::avr_size_t COUNT>
constexpr bool arg_value_test(
    Array<T, COUNT> const custom,
    T const * native
) {
    for(uint8_t i{0}; i<COUNT; i++)
        if (custom[i] != native[i])
            return false;
    return true;
}

template<typename T, Utility::avr_size_t COUNT>
constexpr bool arg_reference_test(
    Array<T, COUNT> const & custom,
    T const * native
) {
    for(uint8_t i{0}; i<COUNT; i++)
        if (custom[i] != native[i])
            return false;
    return true;
}

int main() {
    constexpr int native[5]{ 1, 2, 3, 4, 5 };
    constexpr Array<int, 5> a{ 1, 2, 3, 4, 5 };
    constexpr Array<int, 5> b{ 1, 2, 3, 4, 5 };
    constexpr Array<int, 5> c{ 1, 2, 3, 4, 6 };
    constexpr Array<int, 5> d{ a };

    static_assert(a[3] == 4);
    static_assert(a == b);
    static_assert(a != c);
    static_assert(a == d);

    static_assert(arg_value_test(a, native));
    static_assert(arg_reference_test(a, native));
}
