#ifndef BYTEWISE_H
#define BYTEWISE_H

#include <stdint.h>

namespace AvrSupport::Utility::Bytewise {
    // Bytewise iterators
    template<typename Type>
    constexpr uint8_t const * iter_begin(Type const & value) {
        return reinterpret_cast<uint8_t const *>(&value);
    }
    
    template<typename Type>
    constexpr uint8_t       * iter_begin(Type       & value) {
        return reinterpret_cast<uint8_t       *>(&value);
    }
    
    template<typename Type>
    constexpr uint8_t const * iter_end  (Type const & value) {
        return reinterpret_cast<uint8_t const *>(&value + 1);
    }

    template<typename Type>
    constexpr uint8_t       * iter_end  (Type       & value) {
        return reinterpret_cast<uint8_t       *>(&value + 1);
    }

    // Bytewise foreach
    template<typename Type, typename Func>
    void for_each_big_endian(Type       & value, Func callback) {
        uint8_t       *       current{iter_begin(value)};
        uint8_t const * const end    {iter_end  (value)};
        
        for (; current != end; current++)
            callback(*current);
    }
    template<typename Type, typename Func>
    void for_each_big_endian(Type const & value, Func callback) {
        uint8_t const *       current{iter_begin(value)};
        uint8_t const * const end    {iter_end  (value)};
        
        for (; current != end; current++)
            callback(*current);
    }

    template<typename Type, typename Func>
    void for_each_little_endian(Type       & value, Func callback) {
        uint8_t       *       current{iter_end  (value) - 1};
        uint8_t const * const begin  {iter_begin(value) - 1};
        
        for (; current != begin; current--)
            callback(*current);
    }
    template<typename Type, typename Func>
    void for_each_little_endian(Type const & value, Func callback) {
        uint8_t const *       current{iter_end  (value) - 1};
        uint8_t const * const begin  {iter_begin(value) - 1};
        
        for (; current != begin; current--)
            callback(*current);
    }
}

#endif