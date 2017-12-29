#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>

namespace AvrSupport::Utility {
    using array_size_t = uint16_t;

    /// A `std::array` replacement
    template<typename Type, array_size_t SIZE>
    struct Array {
        using ref          = Type &;
        using ref_const    = Type const &;
        using iter         = Type *;
        using iter_const   = Type const*;

        Type _storage[SIZE ? SIZE : 1]; // Allow zero-sized arrays

        constexpr iter       begin()       { return &_storage[0]; }
        constexpr iter_const begin() const { return &_storage[0]; }
        constexpr iter       end()         { return &_storage[SIZE]; }
        constexpr iter_const end()   const { return &_storage[SIZE]; }
        
        constexpr array_size_t size()     const { return SIZE; }
        constexpr array_size_t max_size() const { return SIZE; }
        constexpr bool         empty()    const { return SIZE == 0; }
        
        constexpr ref       operator[](array_size_t const i)       { return _storage[i]; }
        constexpr ref_const operator[](array_size_t const i) const { return _storage[i]; }

        constexpr bool operator==(Array<Type, SIZE> const & rhs) {
            for (
                auto
                    curr_this =     begin(),
                    curr_rhs  = rhs.begin(),
                    end_this  =     end(),
                    end_rhs   = rhs.end();
                curr_this != end_this;
                ++curr_this, ++curr_rhs
            ) {
                if (*curr_this != *curr_rhs) return false;
            }

            return true;
        }
        constexpr bool operator!=(Array<Type, SIZE> const & rhs) {
            return !(operator==(rhs));
        }
    };
}

#endif
