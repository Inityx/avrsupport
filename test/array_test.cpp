#include "utility/array.hpp"
#include <cassert>

using AVRSupport::Utility::Array;

int main() {
    Array<int, 5> a = {{ 1, 2, 3, 4, 5 }};
    Array<int, 5> b = {{ 1, 2, 3, 4, 5 }};

    assert(a[3] == 4);
    for(auto elem : a) assert(elem > 0);
    assert(a == b);
}
