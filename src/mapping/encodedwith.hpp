#ifndef AVRSUPPORT_MAPPING_ENCODEDWITH_H
#define AVRSUPPORT_MAPPING_ENCODEDWITH_H

#include <assert.h>
#include <utility/iterator.hpp>
#include <utility/cstring.hpp>
#include <utility/stddef.hpp>

namespace avrsupport::mapping {
    /// A C string iterator that maps encodings on dereference.
    template<typename Map>
    struct StringMapper : public utility::iterator::PointerIter<char const, StringMapper<Map>> {
        typename Map::Encoding constexpr operator* () const {
            return Map::ascii(*(this->state));
        }
    };

    /**
     * A base iterator adapter for encoding C strings.
     * `Map` must define a `Map::Encoding` character type and implement
     * `Map::Encoding Map::ascii(char const)`.
     * @tparam Map A struct that statically maps characters to another encoding.
     * @see SevenSegmentMap
     */
    template<typename Map>
    struct EncodedWith : utility::CStringChars<char const, StringMapper<Map>> {
        constexpr EncodedWith(char const * const string) :
            utility::CStringChars<char const, StringMapper<Map>>{string} {}

        /// `constexpr` string encoding.
        template<utility::avr_size_t COUNT>
        constexpr static utility::Array<typename Map::Encoding, COUNT> collected(
            char const * const string,
            char const filler = ' '
        ) {
            // String must fit in array
            assert(utility::cstring::length(string) <= COUNT);

            utility::Array<typename Map::Encoding, COUNT> collector{0};
            typename Map::Encoding * current{&collector[0]};

            for (typename Map::Encoding character : EncodedWith<Map>{string})
                *(current++) = character;

            while (current != collector.end())
                *(current++) = filler;

            return collector;
        }
    };
}

#endif
