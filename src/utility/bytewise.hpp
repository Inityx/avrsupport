#ifndef BYTEWISE_H
#define BYTEWISE_H

#include <stdint.h>

namespace AvrSupport::Utility::Bytewise {
    using bytewise_size_t = uint16_t;

    template<typename ByteType>
    struct BigEndianIter {
        using SelfClass = BigEndianIter<ByteType>;
        ByteType * address;
        
        ByteType & operator* () { return *address; }
        bool operator==(SelfClass const & rhs) { return address == rhs.address; }
        bool operator!=(SelfClass const & rhs) { return address != rhs.address; }
        
        SelfClass & operator++()                    { address++;      return *this; }
        SelfClass & operator--()                    { address--;      return *this; }
        SelfClass & operator+=(bytewise_size_t rhs) { address += rhs; return *this; }
        SelfClass & operator-=(bytewise_size_t rhs) { address -= rhs; return *this; }
        SelfClass   operator++(int)                 { return SelfClass{ address++ }; }
        SelfClass   operator--(int)                 { return SelfClass{ address-- }; }
        SelfClass   operator+ (bytewise_size_t rhs) { return SelfClass{ address + rhs }; }
        SelfClass   operator- (bytewise_size_t rhs) { return SelfClass{ address - rhs }; }
    };

    template<typename ByteType>
    struct LittleEndianIter : public BigEndianIter<ByteType> {
        using SelfClass = LittleEndianIter<ByteType>;
        SelfClass & operator++()                    { this->address--;      return *this; }
        SelfClass & operator--()                    { this->address++;      return *this; }
        SelfClass & operator+=(bytewise_size_t rhs) { this->address -= rhs; return *this; }
        SelfClass & operator-=(bytewise_size_t rhs) { this->address += rhs; return *this; }
        SelfClass   operator++(int)                 { return SelfClass{ this->address-- }; }
        SelfClass   operator--(int)                 { return SelfClass{ this->address++ }; }
        SelfClass   operator+ (bytewise_size_t rhs) { return SelfClass{ this->address - rhs }; }
        SelfClass   operator- (bytewise_size_t rhs) { return SelfClass{ this->address + rhs }; }
    };

    template<typename SourceType, typename ByteType = uint8_t>
    struct BigEndian {
        using IterType = BigEndianIter<ByteType>;
        SourceType & target;

        constexpr BigEndian(SourceType & target) : target{target} {}
        IterType begin() { return IterType{reinterpret_cast<ByteType *>(&target    )}; }
        IterType end()   { return IterType{reinterpret_cast<ByteType *>(&target + 1)}; }
    };

    template<typename SourceType, typename ByteType = uint8_t>
    struct LittleEndian {
        using IterType = LittleEndianIter<ByteType>;
        SourceType & target;

        constexpr LittleEndian(SourceType & target) : target{target} {}
        IterType begin() { return IterType{reinterpret_cast<ByteType *>(&target + 1) - 1}; }
        IterType end()   { return IterType{reinterpret_cast<ByteType *>(&target    ) - 1}; }
    };

    template<typename SourceType> using BigEndianConst    = BigEndian   <SourceType, uint8_t const>;
    template<typename SourceType> using LittleEndianConst = LittleEndian<SourceType, uint8_t const>;
}

#endif