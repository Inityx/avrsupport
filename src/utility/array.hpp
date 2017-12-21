#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>

namespace AvrSupport::Utility {
    using array_size_t = uint16_t;

    template<typename Type, array_size_t SIZE>
    struct Array {
        using ref          = Type &;
        using ref_const    = Type const &;
        using iter         = Type *;
        using iter_const   = Type const*;

        Type _storage[SIZE ? SIZE : 1]; // Allow zero-sized arrays

        // Iterators
        iter       begin()       { return &_storage[0]; }
        iter_const begin() const { return &_storage[0]; }
        iter       end()         { return &_storage[SIZE]; }
        iter_const end()   const { return &_storage[SIZE]; }
        
        // Capacity
        constexpr array_size_t size()     const { return SIZE; }
        constexpr array_size_t max_size() const { return SIZE; }
        constexpr bool         empty()    const { return SIZE == 0; }
        
        // Accessors
        ref       operator[](array_size_t const i)       { return _storage[i]; }
        ref_const operator[](array_size_t const i) const { return _storage[i]; }
    };

    // Comparisons
    template<typename Type, array_size_t SIZE>
    bool operator==(
        Array<Type, SIZE> const & l,
        Array<Type, SIZE> const & r
    ) {
        for (
            auto lb = l.begin(),
                 rb = r.begin(),
                 le = l.end(),
                 re = r.end();
            lb != le;
            ++lb, ++rb
        ) {
            if (*lb != *rb) return false;
        }

        return true;
    }

    template<typename Type, array_size_t SIZE>
    bool operator!=(
        Array<Type, SIZE> const & l,
        Array<Type, SIZE> const & r
    ) {
        return !(l == r);
    }
}

#endif
