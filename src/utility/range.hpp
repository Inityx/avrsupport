#ifndef RANGE_H
#define RANGE_H

#include <stdlib.h>

#include <utility/array.hpp>

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
     * An iterable range; [first, last).
     * `last` is an outer bound when `abs(step)` is not 1.
     * @tparam Type The type to yield (default uint8_t)
     */
    template <typename Type = uint8_t>
    struct Iterable {
        /// A value-based iterator
        struct Iter {
            Type value, step;

            Type operator*() { return value; } ///< Dereference
            operator Type()  { return value; } ///< Type coercion

            bool operator==(Iter const & rhs) const {
                if (abs(step) == 1) return value == rhs.value;
                return abs(value - rhs.value) < abs(step);
            }
            bool operator!=(Iter const & rhs) const {
                if (abs(step) == 1) return value != rhs.value;
                return abs(value - rhs.value) >= abs(step);
            }

            Iter & operator++()                     { value += step; return *this; }
            Iter & operator--()                     { value -= step; return *this; }
            Iter & operator+=(Type const rhs)       { value += rhs;  return *this; }
            Iter & operator-=(Type const rhs)       { value -= rhs;  return *this; }
            Iter   operator++(int)                  { return Iter{ value += step }; }
            Iter   operator--(int)                  { return Iter{ value -= step }; }
            Iter   operator+ (Type const rhs) const { return Iter{ value + rhs }; }
            Iter   operator- (Type const rhs) const { return Iter{ value - rhs }; }
        };

        Type first, last, step;

        /**
         * Complete constructor.
         * Arguments may be positive or negative.
         */
        constexpr Iterable(Type first, Type last, Type step)
            : first{first}, last{last}, step{step} {}

        /**
         * Automatic step constructor.
         * Step is 1 or -1 depending on argument ordering.
         */
        constexpr Iterable(Type first, Type last)
            : first{first}, last{last}, step{(first <= last) ? 1 : -1} {}
        
        /**
         * Automatic range constructor.
         * Steps from 0 to `last` by 1 or -1.
         */
        constexpr Iterable(Type last)
            : first{0}, last{last}, step{(first <= last) ? 1 : -1} {}
        
        Iter begin() { return Iter{first, step}; }
        Iter end()   { return Iter{last,  step}; }
    };
}

#endif