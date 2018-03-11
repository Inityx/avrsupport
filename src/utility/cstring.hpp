#ifndef AVRSUPPORT_UTILITY_CSTRING_H
#define AVRSUPPORT_UTILITY_CSTRING_H

#include <utility/iterator.hpp>
#include <utility/stddef.hpp>

/// C String helpers
namespace avrsupport::utility::cstring {
    // Max length must be defined for to prevent compile-time infinite loops
    // Who would ever even want a string this long on an AVR chip
    static avr_size_t const MAX_LENGTH = 1024 * 16;

    struct StringStat {
        avr_size_t length{0};
        char const * end;

        constexpr StringStat(char const * const string) : end{string} {
            while(*end != '\0' && length <= MAX_LENGTH)
                end++, length++;
        }
    };

    constexpr avr_size_t length(char const * const string) {
        return StringStat{string}.length;
    }
    
    constexpr char const * end(char const * const string) {
        return StringStat{string}.end;
    }

    template<typename CharType>
    struct Iter : public iterator::PointerIter<CharType, Iter<CharType>> {
        constexpr Iter(CharType * const string) :
            iterator::PointerIter<CharType, Iter<CharType>>{string} {}
    };

    template<typename CharType = char, typename Iter = Iter<CharType>>
    struct Chars {
        CharType * const string;

        constexpr Chars(CharType * const string) : string{string} {}
        constexpr Iter begin() const { return Iter{string}; }
        constexpr Iter end()   const { return Iter{cstring::end(string)}; }
    };
}

#endif
