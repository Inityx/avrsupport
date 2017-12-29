#ifndef ITERATOR_H
#define ITERATOR_H

#include <stddef.h>

namespace AvrSupport::Utility {
    /// Circular index incrementor
    template<size_t LIMIT, typename IndexType>
    void circular_increment_index(IndexType & index) {
        index++;
        if (index >= LIMIT) index = 0;
    }

    /// Circular iterator incrementor
    template<typename Iterable, typename Iterator>
    void circular_increment_iterator(
        Iterable & iterable,
        Iterator & iterator
    ) {
        iterator++;
        if (iterator == iterable.end())
            iterator = iterable.begin();
    }

    // TODO: Unit test

    /// A pointer-based iterator type.
    template<typename Type>
    struct Iterator {
    protected:
        Type * address;

    public:
        constexpr Iterator(Type * const target) : address(target) {}

        Type & operator* () const { return *address; } ///< Dereference
        operator Type*()    const { return  address; } ///< Pointer coercion
        bool operator==(Iterator const & rhs) const { return address == rhs.address; }
        bool operator!=(Iterator const & rhs) const { return address != rhs.address; }
        
        Iterator & operator++()                       { address++;      return *this; }
        Iterator & operator--()                       { address--;      return *this; }
        Iterator & operator+=(size_t const rhs)       { address += rhs; return *this; }
        Iterator & operator-=(size_t const rhs)       { address -= rhs; return *this; }
        Iterator   operator++(int)                    { return Iterator{ address++ }; }
        Iterator   operator--(int)                    { return Iterator{ address-- }; }
        Iterator   operator+ (size_t const rhs) const { return Iterator{ address + rhs }; }
        Iterator   operator- (size_t const rhs) const { return Iterator{ address - rhs }; }
    };

    /// A value-based iterator type
    template <typename Type>
    struct ValueIterator {
    protected:
        Type value, step;

    public:
        constexpr ValueIterator(Type value, Type step) : value{value}, step{step} {}

        Type operator*() { return value; } ///< Dereference
        operator Type()  { return value; } ///< Type coercion

        bool operator==(ValueIterator const & rhs) const {
            if (abs(step) == 1) return value == rhs.value;
            return abs(value - rhs.value) < abs(step);
        }
        bool operator!=(ValueIterator const & rhs) const {
            if (abs(step) == 1) return value != rhs.value;
            return abs(value - rhs.value) >= abs(step);
        }

        ValueIterator & operator++()                     { value += step; return *this; }
        ValueIterator & operator--()                     { value -= step; return *this; }
        ValueIterator & operator+=(Type const rhs)       { value += rhs;  return *this; }
        ValueIterator & operator-=(Type const rhs)       { value -= rhs;  return *this; }
        ValueIterator   operator++(int)                  { return ValueIterator{ value += step }; }
        ValueIterator   operator--(int)                  { return ValueIterator{ value -= step }; }
        ValueIterator   operator+ (Type const rhs) const { return ValueIterator{ value + rhs }; }
        ValueIterator   operator- (Type const rhs) const { return ValueIterator{ value - rhs }; }
    };
}
#endif