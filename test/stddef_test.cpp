#include <cassert>

#include <utility/stddef.hpp>

using namespace avrsupport::utility;

template<typename Enum>
constexpr void a_b(Enum const e, bool const b) {
    assert(static_cast<bool>(e) == b);
    assert(static_cast<Enum>(b) == e);
}

int main() {
    a_b(LogicLevel::low, false);
    a_b(LogicLevel::high, true);

    a_b(IoDirection::in, false);
    a_b(IoDirection::out, true);
    
    a_b(Endian::big, false);
    a_b(Endian::little, true);
}
