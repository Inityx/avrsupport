#include <utility/arithmetic.hpp>
#include <utility/array.hpp>

#include <cstdint>
#include <assert.h>
#include <vector>

using namespace avrsupport::utility;
using namespace arithmetic;

void within_test() {
    constexpr Array<int32_t, 2> r{2,4};
    static_assert(!within(1, r));
    static_assert( within(2, r));
    static_assert( within(3, r));
    static_assert( within(4, r));
    static_assert(!within(5, r));
}

void between_test() {
    constexpr Array<int32_t, 2> r{2,4};
    static_assert(!between(1, r));
    static_assert(!between(2, r));
    static_assert( between(3, r));
    static_assert(!between(4, r));
    static_assert(!between(5, r));
}

constexpr bool circular_inc() {
    uint8_t val{0};
    auto bump_3 = circular_increment_index<3, decltype(val)>;

    bump_3(val); if (val != 1) return false;
    bump_3(val); if (val != 2) return false;
    bump_3(val); if (val != 0) return false;

    return true;
}

void circular_inc_test() {
    static_assert(circular_inc());
}

int main() {
    within_test();
    between_test();
    circular_inc_test();
}
