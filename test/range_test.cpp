#include <utility/range.hpp>
#include <utility/array.hpp>

#include <assert.h>
#include <stdio.h>
#include <vector>

using namespace AvrSupport::Utility;

void test_within() {
    Array<int, 2> r{2,4};
    assert(!Range::within(1, r));
    assert( Range::within(2, r));
    assert( Range::within(3, r));
    assert( Range::within(4, r));
    assert(!Range::within(5, r));
}

void test_between() {
    Array<int, 2> r{2,4};
    assert(!Range::between(1, r));
    assert(!Range::between(2, r));
    assert( Range::between(3, r));
    assert(!Range::between(4, r));
    assert(!Range::between(5, r));
}

void test_iterator() {
    std::vector<int> accumulator;
    std::vector<int> correct_default    { 0, 1, 2, 3, 4 };
    std::vector<int> correct_negative   { 0, -1, -2, -3, -4 };
    std::vector<int> correct_step_3     { 1, 4, 7, 10, 13 };
    std::vector<int> correct_bw_default { 5, 4, 3, 2, 1 };
    std::vector<int> correct_bw_step    { 9, 6, 3, 0 };

    for(auto value : Range::Iterable<int>{0, 0})      accumulator.push_back(value);
    assert(accumulator.empty());

    accumulator.clear();
    for(auto value : Range::Iterable<int>{0, 5})      accumulator.push_back(value);
    assert(accumulator == correct_default);

    accumulator.clear();
    for(auto value : Range::Iterable<int>{5})         accumulator.push_back(value);
    assert(accumulator == correct_default);

    accumulator.clear();
    for(auto value : Range::Iterable<int>{-5})        accumulator.push_back(value);
    assert(accumulator == correct_negative);
    
    accumulator.clear();
    for(auto value : Range::Iterable<int>{1, 16, 3})  accumulator.push_back(value);
    assert(accumulator == correct_step_3);

    accumulator.clear();
    for(auto value : Range::Iterable<int>{1, 18, 3})  accumulator.push_back(value);
    assert(accumulator == correct_step_3);

    accumulator.clear();
    for(auto value : Range::Iterable<int>{5, 0})      accumulator.push_back(value);
    assert(accumulator == correct_bw_default);

    accumulator.clear();
    for(auto value : Range::Iterable<int>{9, -4, -3}) accumulator.push_back(value);
    assert(accumulator == correct_bw_step);
}

int main() {
    test_within();
    test_between();
    test_iterator();
}