#ifndef CHARMAPPER_H
#define CHARMAPPER_H

#include <string.h>
#include <utility/iterator.hpp>

namespace AvrSupport::Mapping {
    /**
     * A helper for mapping strings.
     * @tparam Map A namespace or struct implementing the `ascii` static method,
     *             which takes a `char` and returns a `Map::OutputType`.
     */
    template<typename Map>
    struct CharMapper {
        using InputType = char const;

        struct Iter : public Utility::Iterator<InputType> {
            InputType & operator*() = delete;

            /// Dereference
            typename Map::OutputType operator* () const {
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