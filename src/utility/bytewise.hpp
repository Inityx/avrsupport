#ifndef BYTEWISE_H
#define BYTEWISE_H

#include <stdint.h>

/// %Bytewise operation helper classes
namespace AvrSupport::Utility::Bytewise {
    using bytewise_size_t = uint16_t;

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
        struct Iter {
            ByteType * address;
            
            ByteType & operator* () { return *address; } ///< Dereference
            operator ByteType*()    { return  address; } ///< Pointer coercion
            bool operator==(Iter const & rhs) { return address == rhs.address; }
            bool operator!=(Iter const & rhs) { return address != rhs.address; }
            
            Iter & operator++()                    { address++;      return *this; }
            Iter & operator--()                    { address--;      return *this; }
            Iter & operator+=(bytewise_size_t rhs) { address += rhs; return *this; }
            Iter & operator-=(bytewise_size_t rhs) { address -= rhs; return *this; }
            Iter   operator++(int)                 { return Iter{ address++ }; }
            Iter   operator--(int)                 { return Iter{ address-- }; }
            Iter   operator+ (bytewise_size_t rhs) { return Iter{ address + rhs }; }
            Iter   operator- (bytewise_size_t rhs) { return Iter{ address - rhs }; }
        };

        SourceType & target;

        constexpr BigEndian(SourceType & target) : target{target} {}
        Iter begin() { return Iter{reinterpret_cast<ByteType *>(&target    )}; }
        Iter end()   { return Iter{reinterpret_cast<ByteType *>(&target + 1)}; }
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

        /** A little endian (backwards) backwardsiterator. */
        struct Iter : public BigEndian<SourceType, ByteType>::Iter {
            Iter & operator++()                    { this->address--;      return *this; }
            Iter & operator--()                    { this->address++;      return *this; }
            Iter & operator+=(bytewise_size_t rhs) { this->address -= rhs; return *this; }
            Iter & operator-=(bytewise_size_t rhs) { this->address += rhs; return *this; }
            Iter   operator++(int)                 { return Iter{ this->address-- }; }
            Iter   operator--(int)                 { return Iter{ this->address++ }; }
            Iter   operator+ (bytewise_size_t rhs) { return Iter{ this->address - rhs }; }
            Iter   operator- (bytewise_size_t rhs) { return Iter{ this->address + rhs }; }
        };

        SourceType & target;

        constexpr LittleEndian(SourceType & target) : target{target} {}
        Iter begin() { return Iter{reinterpret_cast<ByteType *>(&target + 1) - 1}; }
        Iter end()   { return Iter{reinterpret_cast<ByteType *>(&target    ) - 1}; }
    };

    template<typename SourceType> using BigEndianConst    = BigEndian   <SourceType, uint8_t const>;
    template<typename SourceType> using LittleEndianConst = LittleEndian<SourceType, uint8_t const>;
}

#endif
