#ifndef BYTEWISE_H
#define BYTEWISE_H

#include <stdint.h>
#include <utility/iterator.hpp>

/// %Bytewise operation helper classes
namespace AvrSupport::Utility::Bytewise {
    /**
     * A big endian bytewise iterator adapter.
     * 
     * This adapter yields an iterator over each byte of a struct or primitive
     * going forwards, as would be appropriate for big-endian architectures or
     * contiguous data structures.
     * 
     * @tparam SourceType The datatype to be iterated over
     * @tparam ByteType   The type of byte to yield (e.g. char, uint8_t const, etc.)
     */
    template<typename SourceType, typename ByteType = uint8_t>
    struct BigEndian {
        static_assert(sizeof(ByteType) == 1, "The ByteType must be one byte.");

        /** A big endian (forwards) bytewise iterator. */
        struct Iter : public Utility::Iterator<ByteType> {};

        SourceType & target;

        constexpr BigEndian(SourceType & target) : target{target} {}

        // constexpr functions cannot have reinterpret_cast
        Iter begin() const { return Iter{reinterpret_cast<ByteType *>(&target    )}; }
        Iter end()   const { return Iter{reinterpret_cast<ByteType *>(&target + 1)}; }
    };

    /**
     * A little endian bytewise iterator adapter.
     * 
     * This adapter yields an iterator over each byte of a struct or primitive
     * going backwards, as would be appropriate for little-endian architectures.
     * 
     * @tparam SourceType The datatype to be iterated over
     * @tparam ByteType   The type of byte to yield (e.g. char, uint8_t const, etc.)
     */
    template<typename SourceType, typename ByteType = uint8_t>
    struct LittleEndian {
        static_assert(sizeof(ByteType) == 1, "The ByteType must be one byte.");

        /** A little endian (backwards) bytewise iterator. */
        struct Iter : public Utility::Iterator<ByteType> {
            constexpr Iter & operator++()                       { this->address--;      return *this; }
            constexpr Iter & operator--()                       { this->address++;      return *this; }
            constexpr Iter & operator+=(size_t const rhs)       { this->address -= rhs; return *this; }
            constexpr Iter & operator-=(size_t const rhs)       { this->address += rhs; return *this; }
            constexpr Iter   operator++(int)                    { return Iter{ this->address-- }; }
            constexpr Iter   operator--(int)                    { return Iter{ this->address++ }; }
            constexpr Iter   operator+ (size_t const rhs) const { return Iter{ this->address - rhs }; }
            constexpr Iter   operator- (size_t const rhs) const { return Iter{ this->address + rhs }; }
        };

        SourceType & target;

        constexpr LittleEndian(SourceType & target) : target{target} {}

        // constexpr functions cannot have reinterpret_cast
        Iter begin() const { return Iter{reinterpret_cast<ByteType *>(&target + 1) - 1}; }
        Iter end()   const { return Iter{reinterpret_cast<ByteType *>(&target    ) - 1}; }
    };

    template<typename SourceType> using BigEndianConst    = BigEndian   <SourceType, uint8_t const>;
    template<typename SourceType> using LittleEndianConst = LittleEndian<SourceType, uint8_t const>;
}

#endif
