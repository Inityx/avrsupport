#ifndef ITERATOR_H
#define ITERATOR_H

#include <stddef.h>

namespace AvrSupport::Utility {

    // TODO: Unit test

    /// A base iterator type.
    template<typename Type>
    struct Iterator {
        Type * address;

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
}
#endif