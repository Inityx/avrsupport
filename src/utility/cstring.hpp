#ifndef C_STRING_H
#define C_STRING_H

#include <stddef.h>

/// Utilities for working with C strings, mostly for constexpr correctness
namespace AvrSupport::Utility::CString {
    // Max length must be defined for to prevent compile-time infinite loops
    // Who would ever even want a string this long on an AVR chip
    static size_t const MAX_LENGTH = 4096;

    struct StringStat {
        size_t length{0};
        char const * end;

        constexpr StringStat(char const * const string) : end{string} {
            while(*end != '\0' && length <= MAX_LENGTH)
                end++, length++;
        }
    };

    constexpr size_t length(char const * const string) {
        return StringStat{string}.length;
    }
    
    constexpr char const * end(char const * const string) {
        return StringStat{string}.end;
    }
}

#endif