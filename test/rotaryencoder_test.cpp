#include <cassert>
#include <array>

#include "peripheral/rotaryencoder.hpp"

using Rotary = AVRSupport::Peripheral::RotaryEncoder;

template<size_t count>
using Actions = std::array<std::array<int, 2>, count>;

template<size_t count>
void rotary_apply(
    Rotary &rotary,
    Actions<count> &actions
) {
    for(auto &a : actions) rotary.update(a[0], a[1]);
}

template<typename Func>
void times_do(int count, Func func) {
    for(int i{0}; i<count; i++) func();
}

bool left_turn() {
    Rotary rotary;
    Actions<5> actions { 0,0, 0,1, 1,1, 1,0, 0,0 };

    rotary_apply(rotary, actions);
    return (
        rotary.turned_left() &&
        !rotary.turned_right()
    );
}

bool right_turn() {
    Rotary rotary;
    Actions<5> actions { 0,0, 1,0, 1,1, 0,1, 0,0 };

    rotary_apply(rotary, actions);
    return (
        rotary.turned_right() &&
        !rotary.turned_left()
    );
}

bool deduplicate() {
    Rotary rotary;
    times_do(10, [&](){ rotary.update(0,0); });
    times_do(11, [&](){ rotary.update(0,1); });
    times_do(9,  [&](){ rotary.update(1,1); });
    times_do(14, [&](){ rotary.update(1,0); });
    times_do(10, [&](){ rotary.update(0,0); });

    return (
        rotary.turned_left() &&
        !rotary.turned_right()
    );
}

bool no_false_positive() {
    Rotary rotary;
    Actions<5> actions = {
         0, 0,
         0, 1,
         1, 1,
         0, 1,
         0, 0
    };

    rotary_apply(rotary, actions);
    return (
        !rotary.turned_left() &&
        !rotary.turned_right()
    );
}

bool no_false_negative_1() {
    Rotary rotary;
    Actions<9> actions {
        0,0,
        0,1,
        1,1,
        0,1,
        1,1,
        1,0,
        1,1,
        1,0,
        0,0
    };

    rotary_apply(rotary, actions);
    return(rotary.turned_left());
}

bool no_false_negative_2() {
    Rotary rotary;
    Actions<9> actions = {
        0, 0,
        0, 1,
        1, 1,
        1, 0,
        1, 1,
        0, 1,
        1, 1,
        1, 0,
        0, 0
    };

    rotary_apply(rotary, actions);
    return(rotary.turned_left());
}

int main() {
    assert(deduplicate());
    assert(no_false_positive());
    assert(no_false_negative_1());
    assert(no_false_negative_2());
}

