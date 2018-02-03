#include <utility/iterator.hpp>
#include <utility/array.hpp>

#include <cstdio>
#include <cassert>
#include <vector>
#include <cstdint>

using namespace AvrSupport::Utility;

constexpr int sum(int const start, int const end) {
    int accum{0};
    for(auto value : Range<int>{start, end})
        accum += value;
    return accum;
}

void constexpr_test() {
    static_assert(sum(3, 10) == 42);
}

void range_test() {
    std::vector< int8_t> acc_signed;
    std::vector<uint8_t> acc_unsigned;
    std::vector<uint8_t> correct_default    { 0, 1, 2, 3, 4 };
    std::vector< int8_t> correct_negative   { 0, -1, -2, -3, -4 };
    std::vector<uint8_t> correct_step_3     { 1, 4, 7, 10, 13 };
    std::vector<uint8_t> correct_bw_default { 5, 4, 3, 2, 1 };
    std::vector< int8_t> correct_bw_step    { 9, 6, 3, 0 };

    for(auto value : Range<uint8_t>{0, 0})      acc_unsigned.push_back(value);
    assert(acc_unsigned.empty());

    acc_unsigned.clear();
    for(auto value : Range<uint8_t>{0, 5})      acc_unsigned.push_back(value);
    assert(acc_unsigned == correct_default);

    acc_unsigned.clear();
    for(auto value : Range<uint8_t>{5})         acc_unsigned.push_back(value);
    assert(acc_unsigned == correct_default);

    acc_signed.clear();
    for(auto value : Range< int8_t, -1>{-5})    acc_signed.push_back(value);
    assert(acc_signed == correct_negative);
    
    acc_unsigned.clear();
    for(auto value : Range<uint8_t, 3>{1, 16})  acc_unsigned.push_back(value);
    assert(acc_unsigned == correct_step_3);

    acc_unsigned.clear();
    for(auto value : Range<uint8_t, 3>{1, 18})  acc_unsigned.push_back(value);
    assert(acc_unsigned == correct_step_3);

    acc_unsigned.clear();
    for(auto value : Range<uint8_t, -1>{5, 0})  acc_unsigned.push_back(value);
    assert(acc_unsigned == correct_bw_default);

    acc_signed.clear();
    for(auto value : Range< int8_t, -3>{9, -4}) acc_signed.push_back(value);
    assert(acc_signed == correct_bw_step);
}


void subslice_test() {
    constexpr static Array<uint8_t, 7> const source { 2, 5, 6, 7, 6, 4, 3 };
    constexpr Subslice const
        first_five  {source, 5},
        middle_three{source, 3, 2};
    
    static_assert(first_five  .begin() == &source[0]);
    static_assert(first_five  .end  () == &source[5]);
    static_assert(middle_three.begin() == &source[2]);
    static_assert(middle_three.end  () == &source[5]);
}

int main() {
    range_test();
    constexpr_test();
    subslice_test();
    return 0;
}
