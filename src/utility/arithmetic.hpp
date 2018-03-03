#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <utility/array.hpp>
#include <utility/stddef.hpp>

/// %arithmetic helpers
namespace avrsupport::utility::arithmetic {
    /// Inclusive range check
    template<typename Type>
    constexpr bool within(Type const & value, Array<Type, 2> const range) {
        return (
            value >= range[0] &&
            value <= range[1]
        );
    }

    /// Exclusive range check
    template<typename Type>
    constexpr bool between(Type const & value, Array<Type, 2> const range) {
        return (
            value > range[0] &&
            value < range[1]
        );
    }

    /// Reference-based minimum, default lhs
    template<typename Type>
    constexpr Type & min(Type & lhs, Type & rhs) {
        return lhs <= rhs ? lhs : rhs;
    }
    
    /// Reference-based maximum, default lhs
    template<typename Type>
    constexpr Type & max(Type & lhs, Type & rhs) {
        return lhs >= rhs ? lhs : rhs;
    }

    /// Absolute value
    template<typename Type>
    constexpr Type abs(Type value) {
        return value >= 0 ? value : 0 - value;
    }

    /// Unambiguous non-coercive absolute difference (1-dimensional distance)
    template<typename Type>
    constexpr Type abs_diff(Type const & a, Type const & b) {
        return a >= b ? a - b : b - a;
    }

    /// Circular index incrementor
    template<avr_size_t LIMIT, typename IndexType>
    constexpr void circular_increment_index(IndexType & index) {
        index++;
        if (index >= LIMIT) index = 0;
    }

    /// Circular iterator incrementor
    template<typename Iterable, typename Iterator>
    constexpr void circular_increment_iterator(
        Iterable & iterable,
        Iterator & iterator
    ) {
        iterator++;
        if (iterator == iterable.end())
            iterator = iterable.begin();
    }

}

#endif
