#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <stdlib.h>

#include <utility/array.hpp>

/// %Arithmetic helpers
namespace AvrSupport::Utility::Arithmetic {
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

    /// Unambiguous non-coercive absolute difference (1-dimensional distance)
    template<typename Type>
    constexpr Type abs_diff(Type const & a, Type const & b) {
        return a >= b ? a - b : b - a;
    }

    /// Circular index incrementor
    template<size_t LIMIT, typename IndexType>
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
