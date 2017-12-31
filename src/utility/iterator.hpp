#ifndef ITERATOR_H
#define ITERATOR_H

#include <stddef.h>
#include <utility/arithmetic.hpp>

namespace AvrSupport::Utility {
    // TODO: Unit test

    /// A pointer-based iterator type.
    template<typename Type>
    struct Iterator {
        using SelfClass = Iterator<Type>;

    protected:
        Type * address;

    public:
        constexpr Iterator(Type * const target) : address(target) {}

        constexpr Type & operator* () const { return *address; } ///< Dereference
        constexpr operator Type*()    const { return  address; } ///< Pointer coercion
        constexpr bool operator==(SelfClass const & rhs) const { return address == rhs.address; }
        constexpr bool operator!=(SelfClass const & rhs) const { return address != rhs.address; }
        
        constexpr SelfClass & operator++()                       { address++;      return *this; }
        constexpr SelfClass & operator--()                       { address--;      return *this; }
        constexpr SelfClass & operator+=(size_t const rhs)       { address += rhs; return *this; }
        constexpr SelfClass & operator-=(size_t const rhs)       { address -= rhs; return *this; }
        constexpr SelfClass   operator++(int)                    { return SelfClass{ address++ }; }
        constexpr SelfClass   operator--(int)                    { return SelfClass{ address-- }; }
        constexpr SelfClass   operator+ (size_t const rhs) const { return SelfClass{ address + rhs }; }
        constexpr SelfClass   operator- (size_t const rhs) const { return SelfClass{ address - rhs }; }
    };

    /**
     * A value-based iterator type.
     * @tparam Type The integral type to iterate with
     * @tparam STEP The amount to increment by
     */
    template <typename Type, auto STEP = 1>
    struct ValueIterator {
        using SelfClass = ValueIterator<Type, STEP>;

    protected:
        Type value;

    public:
        constexpr ValueIterator(Type const & value) : value{value} {}

        constexpr Type operator*() const { return value; } ///< Dereference
        constexpr operator Type()  const { return value; } ///< Type coercion

        constexpr bool operator==(SelfClass const & rhs) const {
            if constexpr (abs(STEP) == 1)
                return value == rhs.value;
            else
                return Arithmetic::abs_diff(value, rhs.value) < abs(STEP);
        }
        constexpr bool operator!=(SelfClass const & rhs) const {
            if constexpr (abs(STEP) == 1)
                return value != rhs.value;
            else
                return Arithmetic::abs_diff(value, rhs.value) >= abs(STEP);
        }

        constexpr SelfClass & operator++()                     { value += STEP; return *this; }
        constexpr SelfClass & operator--()                     { value -= STEP; return *this; }
        constexpr SelfClass & operator+=(Type const rhs)       { value += rhs * STEP;  return *this; }
        constexpr SelfClass & operator-=(Type const rhs)       { value -= rhs * STEP;  return *this; }
        constexpr SelfClass   operator++(int)                  { return SelfClass{ value += STEP }; }
        constexpr SelfClass   operator--(int)                  { return SelfClass{ value -= STEP }; }
        constexpr SelfClass   operator+ (Type const rhs) const { return SelfClass{ value + (rhs * STEP) }; }
        constexpr SelfClass   operator- (Type const rhs) const { return SelfClass{ value - (rhs * STEP) }; }
    };

    /**
     * An iterable numeric range, `[first, last)`.
     * When `abs(step)` is not 1, `last` is an outer bound.
     * @tparam Type The type to yield
     * @tparam STEP The step size (negative for backwards iteration)
     */
    template <typename Type, auto STEP = 1>
    struct Range {
    public:
        Type const first, last;

        /// Steps from `first` to `last` by `STEP`.
        constexpr Range(Type first, Type last) : first{first}, last{last} {}

        /// Steps from 0 to `last` by `STEP`.
        constexpr Range(Type last) : first{0}, last{last} {}
        
        constexpr ValueIterator<Type, STEP> begin() const { return ValueIterator<Type, STEP>{first}; }
        constexpr ValueIterator<Type, STEP> end()   const { return ValueIterator<Type, STEP>{last}; }
    };
}
#endif