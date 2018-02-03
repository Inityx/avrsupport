#ifndef ARRAY_H
#define ARRAY_H

#include <utility/stddef.hpp>

namespace AvrSupport::Utility {
    /// A `std::array` replacement
    template<typename Type, avr_size_t SIZE>
    struct Array {
        Type _storage[SIZE ? SIZE : 1]; // Allow zero-sized arrays

        constexpr Type       * begin()       { return &_storage[0]; }
        constexpr Type const * begin() const { return &_storage[0]; }
        constexpr Type       * end()         { return &_storage[SIZE]; }
        constexpr Type const * end()   const { return &_storage[SIZE]; }
        
        constexpr avr_size_t size()     const { return SIZE; }
        constexpr avr_size_t max_size() const { return SIZE; }
        constexpr bool       empty()    const { return SIZE == 0; }
        
        constexpr Type       & operator[](avr_size_t const i)       { return _storage[i]; }
        constexpr Type const & operator[](avr_size_t const i) const { return _storage[i]; }

        constexpr bool operator==(Array<Type, SIZE> const & rhs) const {
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
        constexpr bool operator!=(Array<Type, SIZE> const & rhs) const {
            return !(operator==(rhs));
        }
    };
}

#endif
