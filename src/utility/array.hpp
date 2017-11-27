#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>

namespace AVRSupport::Utility {
    template<typename Type, uint8_t Size>
    struct Array {
        using size_type       = uint8_t;
        using reference       = Type &;
        using reference_const = Type const &;
        using iterator        = Type *;
        using iterator_const  = Type const*;

        Type _storage[Size ? Size : 1]; // Allow zero-sized arrays

        // Iterators
        iterator       begin()       { return &_storage[0]; }
        iterator_const begin() const { return &_storage[0]; }
        iterator       end()         { return &_storage[Size]; }
        iterator_const end()   const { return &_storage[Size]; }
        
        // Capacity
        constexpr size_type size()     const { return Size; }
        constexpr size_type max_size() const { return Size; }
        constexpr bool      empty()    const { return Size == 0; }
        
        // Accessors
        reference       operator[](size_type const i)       { return _storage[i]; }
        reference_const operator[](size_type const i) const { return _storage[i]; }
    };

    // Comparisons
    template<typename Type, uint8_t Size>
    inline bool operator==(
        Array<Type, Size> const & l,
        Array<Type, Size> const & r
    ) {
        for(
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

    template<typename Type, uint8_t Size>
    inline bool operator!=(
        Array<Type, Size> const & l,
        Array<Type, Size> const & r
    ) {
        return !(l == r);
    }
}

#endif
