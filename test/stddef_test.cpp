#include <cassert>

#include <utility/stddef.hpp>

using namespace avrsupport::utility;

template<typename Enum>
constexpr void a_b(Enum const e, bool const b) {
    assert(static_cast<bool>(e) == b);
    assert(static_cast<Enum>(b) == e);
}

int main() {
    a_b(LogicLevel::LOW, false);
    a_b(LogicLevel::HIGH, true);

    a_b(IoDirection::IN, false);
    a_b(IoDirection::OUT, true);
    
    a_b(Endian::BIG, false);
    a_b(Endian::LITTLE, true);
}
