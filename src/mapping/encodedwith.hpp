#ifndef AVRSUPPORT_MAPPING_ENCODEDWITH_H
#define AVRSUPPORT_MAPPING_ENCODEDWITH_H

#include <assert.h>
#include <utility/iterator.hpp>
#include <utility/cstring.hpp>
#include <utility/stddef.hpp>

namespace avrsupport::mapping {
    /**
     * A base iterator adapter for encoding C strings.
     * `Map` must define a `Map::Encoding` character type and implement
     * `Map::Encoding Map::ascii(char const)`.
     * @tparam Map A struct that statically maps characters to another encoding.
     * @see SevenSegmentMap
     */
    template<typename Map>
    struct EncodedWith {
        using InputType = char const;
        using Encoding = typename Map::Encoding;

        /// A C string iterator that maps encodings on dereference.
        struct Iter : public utility::BasePointerIterator<InputType, Iter> {
            /// Dereference
            Encoding constexpr operator* () const {
                return Map::ascii(*(this->state));
            }
        };

        InputType * string;

        constexpr EncodedWith(InputType * const string) : string{string} {}

        constexpr Iter begin() const { return Iter{string}; }
        constexpr Iter end()   const { return Iter{utility::cstring::end(string)}; }

        /// `constexpr` string encoding.
        template<utility::avr_size_t COUNT>
        constexpr static utility::Array<Encoding, COUNT> collected(
            InputType string[],
            char const filler = ' '
        ) {
            // String must fit in array
            assert(utility::cstring::length(string) <= COUNT);

            utility::Array<Encoding, COUNT> collector{0};
            Encoding * current{&collector[0]};

            for (Encoding character : EncodedWith<Map>{string})
                *(current++) = character;

            while (current != collector.end())
                *(current++) = filler;

            return collector;
        }
    };
}

#endif
