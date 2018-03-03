#include <cassert>
#include <cstddef>
#include <array>

#include <peripheral/rotaryencoder.hpp>

using namespace avrsupport;
using Rotary = peripheral::RotaryEncoder;

template<std::size_t COUNT>
using Actions = std::array<std::array<int, 2> const, COUNT> const;

Actions<5>
    LEFT_TURN   { 0,0, 0,1, 1,1, 1,0, 0,0 },
    RIGHT_TURN  { 0,0, 1,0, 1,1, 0,1, 0,0 },
    FALSE_POS   { 0,0, 0,1, 1,1, 0,1, 0,0 };

Actions<9>
    FALSE_NEG_1 { 0,0, 0,1, 1,1, 0,1, 1,1, 1,0, 1,1, 1,0, 0,0 },
    FALSE_NEG_2 { 0,0, 0,1, 1,1, 1,0, 1,1, 0,1, 1,1, 1,0, 0,0 };

template<std::size_t COUNT>
void rotary_apply(
    Rotary &rotary,
    Actions<COUNT> &actions
) {
    for(auto & a : actions) rotary.sample(a[0], a[1]);
}

template<typename Func>
void times_do(int count, Func func) {
    for(int i{0}; i<count; i++) func();
}

void left_turn() {
    Rotary rotary;
    rotary_apply(rotary, LEFT_TURN);

    assert(rotary.turned_left());
    assert(!rotary.turned_right());
}

void right_turn() {
    Rotary rotary;
    rotary_apply(rotary, RIGHT_TURN);

    assert( rotary.turned_right());
    assert(!rotary.turned_left());
}

void clear() {
    Rotary rotary;
    rotary_apply(rotary, RIGHT_TURN);
    assert(rotary.turned_right());

    rotary.clear();
    assert(!rotary.turned_right());
}

void deduplicate() {
    Rotary rotary;
    times_do(10, [&](){ rotary.sample(0,0); });
    times_do(11, [&](){ rotary.sample(0,1); });
    times_do(9,  [&](){ rotary.sample(1,1); });
    times_do(14, [&](){ rotary.sample(1,0); });
    times_do(10, [&](){ rotary.sample(0,0); });

    assert( rotary.turned_left());
    assert(!rotary.turned_right());
}

void no_false_positive() {
    Rotary rotary;

    rotary_apply(rotary, FALSE_POS);
    assert(!rotary.turned_left());
    assert(!rotary.turned_right());
}

void no_false_negative_1() {
    Rotary rotary;

    rotary_apply(rotary, FALSE_NEG_1);
    assert(rotary.turned_left());
}

void no_false_negative_2() {
    Rotary rotary;

    rotary_apply(rotary, FALSE_NEG_2);
    assert(rotary.turned_left());
}

int main() {
    deduplicate();
    no_false_positive();
    no_false_negative_1();
    no_false_negative_2();
}

