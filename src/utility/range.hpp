#ifndef RANGE_H
#define RANGE_H

#include <stdlib.h>

#include <utility/array.hpp>
#include <utility/iterator.hpp>

/// Numerical range helpers
namespace AvrSupport::Utility::Range {
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

    /**
     * An iterable numeric range, `[first, last)`.
     * When `abs(step)` is not 1, `last` is an outer bound.
     * @tparam Type The type to yield (default uint8_t)
     */
    template <typename Type = uint8_t>
    struct Iterable {
    private:
        constexpr static Type auto_step(Type const first, Type const last) {
            return Type(first <= last ? 1 : -1);
        }

    public:
        Type first, last, step;

        /// Steps from `first` to `last` by `step`.
        constexpr Iterable(Type first, Type last, Type step)
            : first{first}, last{last}, step{step}
        {}

        /// Steps from `first` to `last` by 1 (or -1).
        constexpr Iterable(Type first, Type last)
            : first{first}, last{last}, step{auto_step(first, last)}
        {}
        
        /// Steps from 0 to `last` by 1 (or -1).
        constexpr Iterable(Type last)
            : first{0}, last{last}, step{auto_step(first, last)}
        {}
        
        ValueIterator<Type> begin() { return ValueIterator{first, step}; }
        ValueIterator<Type> end()   { return ValueIterator{last,  step}; }
    };
}

#endif