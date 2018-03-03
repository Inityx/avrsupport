#ifndef AVRSUPPORT_UTILITY_BYTEWISE_H
#define AVRSUPPORT_UTILITY_BYTEWISE_H

#include <stdint.h>

#include <utility/iterator.hpp>
#include <utility/stddef.hpp>

/// %Bytewise iteration helpers
namespace avrsupport::utility {
    /** A bytewise iterator adapter.
     * This adapter yields an iterator over each byte of a struct or primitive.
     * @tparam SourceType The datatype to be iterated over
     * @tparam ENDIAN     The endianness of the iterator
     * @tparam ByteType   The type of byte to yield (e.g. uint8_t const)
     */
    template<typename SourceType, Endian ENDIAN, typename ByteType = uint8_t>
    struct Bytewise {
    private:
        static_assert(sizeof(ByteType) == 1, "The ByteType must be one byte.");
        constexpr static bool const FORWARDS{ENDIAN == Endian::big};
        constexpr static avr_ptrdiff_t const
            STRUCT_BEGIN_OFFSET{FORWARDS ? 0 : 1},
            STRUCT_END_OFFSET  {FORWARDS ? 1 : 0},
            BYTE_OFFSET        {FORWARDS ? 0 : -1};
    
        SourceType & target;

    public:
        struct Iter : public BasePointerIterator<ByteType, Iter> {
            constexpr static avr_ptrdiff_t const INCREMENT{FORWARDS ? 1 : -1};

            constexpr Iter & operator++()                           { this->state += INCREMENT      ; return *this; }
            constexpr Iter & operator--()                           { this->state -= INCREMENT      ; return *this; }
            constexpr Iter & operator+=(avr_size_t const rhs)       { this->state += INCREMENT * rhs; return *this; }
            constexpr Iter & operator-=(avr_size_t const rhs)       { this->state -= INCREMENT * rhs; return *this; }
            constexpr Iter   operator++(int)                        { return Iter{ this->state += INCREMENT       }; }
            constexpr Iter   operator--(int)                        { return Iter{ this->state -= INCREMENT       }; }
            constexpr Iter   operator+ (avr_size_t const rhs) const { return Iter{ this->state +  INCREMENT * rhs }; }
            constexpr Iter   operator- (avr_size_t const rhs) const { return Iter{ this->state -  INCREMENT * rhs }; }
        };

        constexpr Bytewise(SourceType & target) : target{target} {}

        // constexpr functions cannot have reinterpret_cast
        Iter begin() const {
            SourceType * const begin_source{&target + STRUCT_BEGIN_OFFSET};
            ByteType   * const begin_byte  {reinterpret_cast<ByteType *>(begin_source) + BYTE_OFFSET};
            return Iter{begin_byte};
        }
        Iter end()   const {
            SourceType * const end_source  {&target + STRUCT_END_OFFSET};
            ByteType   * const end_byte    {reinterpret_cast<ByteType *>(end_source  ) + BYTE_OFFSET};
            return Iter{end_byte};
        }
    };

    template<typename SourceType, Endian ENDIAN>
    using BytewiseConst = Bytewise<SourceType, ENDIAN, uint8_t const>;
}

#endif
