#include <utility/iterator.hpp>

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

void test_constexpr() {
    static_assert(sum(3, 10) == 42);
}

void test_range() {
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

int main() {
    test_range();
    test_constexpr();
    return 0;
}