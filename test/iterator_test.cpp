#include <utility/iterator.hpp>

#include <cassert>
#include <cstdint>

using namespace AvrSupport::Utility;

void circular_inc_test() {
    uint8_t val{0};
    auto bump_3 = circular_increment_index<3, decltype(val)>;

    bump_3(val);
    assert(val == 1);
    bump_3(val);
    assert(val == 2);
    bump_3(val);
    assert(val == 0);
}

int main() {
    circular_inc_test();   
    return 0;
}