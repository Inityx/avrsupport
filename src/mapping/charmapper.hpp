#ifndef CHARMAPPER_H
#define CHARMAPPER_H

#include <string.h>
#include <utility/iterator.hpp>

namespace AvrSupport::Mapping {
    /**
     * A base iterator adapter for mapping C strings.
     * `Map` must implement `Map::Encoding Map::ascii(char const)`.
     * @tparam Map A struct that statically maps characters to another encoding.
     * @see SevenSegmentMap
     */
    template<typename Map>
    struct CharMapper {
        using InputType = char const;

        /// A C string iterator that maps encodings on dereference.
        struct Iter : public Utility::Iterator<InputType> {
            /// Dereference
            typename Map::Encoding operator* () const {
                return Map::ascii(*address);
            }
        };

        InputType * string;

        constexpr CharMapper(InputType * const string) : string{string} {}

        Iter begin() { return Iter{string}; }
        Iter end()   { return Iter{strchr(string, '\0')}; }
    };
}

#endif