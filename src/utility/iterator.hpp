#ifndef AVRSUPPORT_UTILITY_ITERATOR_H
#define AVRSUPPORT_UTILITY_ITERATOR_H

#include <utility/arithmetic.hpp>
#include <utility/stddef.hpp>

namespace avrsupport::utility {
    /// Iterator types
    namespace iterator {
        /** A base iterator.
         * @tparam StateType The type of state to hold
         * @tparam SelfClass The derived class type
         */
        template<typename StateType, typename SelfClass>
        struct BaseIter {
        protected:
            StateType state;
            constexpr SelfClass * derived_this() {
                return static_cast<SelfClass *>(this);
            }

        public:
            constexpr BaseIter(StateType state) : state{state} {}

            constexpr bool operator==(SelfClass const & rhs) const { return state == rhs.state; }
            constexpr bool operator!=(SelfClass const & rhs) const { return state != rhs.state; }

            constexpr SelfClass & operator++() { state++; return *derived_this(); }
            constexpr SelfClass & operator--() { state--; return *derived_this(); }
            constexpr SelfClass   operator++(int) { return SelfClass{ state++ }; }
            constexpr SelfClass   operator--(int) { return SelfClass{ state-- }; }
        };

        /** A base pointer-based iterator.
         * @tparam Type The type yielded on dereference
         * @tparam SelfClass The derived class type
         */
        template<typename Type, typename SelfClass>
        struct PointerIter : public BaseIter<Type *, SelfClass> {
            constexpr PointerIter(Type * pointer) : BaseIter<Type *, SelfClass>{pointer} {}

            constexpr Type & operator* () const { return *this->state; } ///< Dereference
            constexpr operator Type*()    const { return  this->state; } ///< Pointer coercion
            
            constexpr SelfClass & operator+=(avr_size_t const rhs)       { this->state += rhs; return *this; }
            constexpr SelfClass & operator-=(avr_size_t const rhs)       { this->state -= rhs; return *this; }
            constexpr SelfClass   operator+ (avr_size_t const rhs) const { return SelfClass{ this->state + rhs }; }
            constexpr SelfClass   operator- (avr_size_t const rhs) const { return SelfClass{ this->state - rhs }; }
        };

        /** A value-based iterator.
         * @tparam Type The integral type to iterate with
         * @tparam STEP The amount to increment by
         */
        template <typename Type, auto STEP = 1>
        struct ValueIter : public BaseIter<Type, ValueIter<Type, STEP>> {
            using SelfClass = ValueIter<Type, STEP>;

        public:
            constexpr ValueIter(Type const & value) : BaseIter<Type, SelfClass>{value} {}

            constexpr Type operator*() const { return this->state; } ///< Dereference
            constexpr operator Type()  const { return this->state; } ///< Type coercion

            constexpr bool operator==(SelfClass const & rhs) const {
                if constexpr (arithmetic::abs(STEP) == 1)
                    return this->state == rhs.state;
                else
                    return arithmetic::abs_diff(this->state, rhs.state) < arithmetic::abs(STEP);
            }
            constexpr bool operator!=(SelfClass const & rhs) const {
                if constexpr (arithmetic::abs(STEP) == 1)
                    return this->state != rhs.state;
                else
                    return arithmetic::abs_diff(this->state, rhs.state) >= arithmetic::abs(STEP);
            }

            constexpr SelfClass & operator++()                     { this->state += STEP; return *this; }
            constexpr SelfClass & operator--()                     { this->state -= STEP; return *this; }
            constexpr SelfClass & operator+=(Type const rhs)       { this->state += rhs * STEP;  return *this; }
            constexpr SelfClass & operator-=(Type const rhs)       { this->state -= rhs * STEP;  return *this; }
            constexpr SelfClass   operator++(int)                  { return SelfClass{ this->state += STEP }; }
            constexpr SelfClass   operator--(int)                  { return SelfClass{ this->state -= STEP }; }
            constexpr SelfClass   operator+ (Type const rhs) const { return SelfClass{ this->state + (rhs * STEP) }; }
            constexpr SelfClass   operator- (Type const rhs) const { return SelfClass{ this->state - (rhs * STEP) }; }
        };
    }

    /** An iterable numeric range, `[first, last)`.
     * When `abs(step)` is not 1, `last` is an outer bound.
     * @tparam Type The type to yield
     * @tparam STEP The step size (negative for backwards iteration)
     */
    template <typename Type, auto STEP = 1>
    struct Range {
        using Iter = iterator::ValueIter<Type, STEP>;
        Type const first, last;

        /// Steps from `first` to `last` by `STEP`.
        constexpr Range(Type first, Type last) : first{first}, last{last} {}

        /// Steps from 0 to `last` by `STEP`.
        constexpr Range(Type last) : first{0}, last{last} {}
        
        constexpr Iter begin() const { return Iter{first}; }
        constexpr Iter end()   const { return Iter{last}; }
    };

    template<typename SourceType>
    struct Subslice {
        SourceType const & source;
        avr_size_t const count, start;

        constexpr Subslice(
            SourceType const & source,
            avr_size_t const count,
            avr_size_t const start = 0
        ) :
            source{source},
            count{count},
            start{start}
        {}
        
        constexpr auto begin() const { return source.begin() + start; }
        constexpr auto end()   const { return source.begin() + (start + count); }
    };
}
#endif
