//
//
// File - Iamb/Core:
//
//      Implementation of the Fixedpoint class and related support types.
//
//
// MIT License
//
// Copyright (c) 2017-2021 Martin Jay McKee
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//

#ifndef IAMB_CORE_H
#define IAMB_CORE_H

#include <cmath>
#include <type_traits>
#include <utility>

#include <stdint.h>

#include "meta.h"

namespace iamb
{

namespace internal
{
template<typename Storage, long Offset, bool left>
struct ShiftImpl // Default to Left Shift (Offset is positive)
{
        using storage_t = Storage;
        static constexpr long offset = Offset;

        storage_t calc(const storage_t& _v) { return _v << Offset; }
};
template<typename Storage, long Offset>
struct ShiftImpl<Storage, Offset, false> // Right Shift (Offset is negative)
{
        using storage_t = Storage;
        static constexpr long offset = Offset;

        storage_t calc(const storage_t& _v) { return _v >> -Offset; }
};
template<long offset, typename Value>
Value shift(const Value& _value) {
    return ShiftImpl<Value, offset, (offset>=0)>::calc(_value);
}
// TODO: NEED TO IMPLMEMENT MY OWN std::declval
} /*namespace internal*/

namespace meta
{
template<size_t Power, typename Value>
constexpr Value raise_to_nth(const Value& v) {
    Value prod(v);
    for(size_t i=0; i<Power-1; ++i) prod *= v;
    return prod;
}

template<class Storage>
struct StorageSize
{
  static constexpr size_t bits = 8 * sizeof(Storage);
};
} /*namespace meta*/

struct NumCode {
    enum type_t {
        Normal,
        PositiveInfinity,
        NegativeInfinity,
        NaN
    };
};

struct Flags
{
	enum flags_t {
		None,
		NoUnderflow,
		NoOverflow
	};
};

struct OverflowHandling
{
  enum overflow_t {
    Wrapping,
    Saturating,
    Invalid
  };
};

// TODO: THIS SHOULD BE OPTIMIZED AND THE API FINALIZED BEFORE IT IS PUSHED THROUGH THE BULK OF THE CODE
struct FixedPointErrors
{
    FixedPointErrors()
        : code(NumCode::Normal), overflow(false), underflow(false), divisionByZero(false), invalidArgument(false) {}

    FixedPointErrors(const FixedPointErrors& _other)
        : code(_other.code), overflow(_other.overflow), underflow(_other.underflow), divisionByZero(_other.divisionByZero), invalidArgument(_other.invalidArgument) {}

    bool ok() const { return (code==NumCode::Normal) && !overflow & !underflow && !divisionByZero && !invalidArgument; }

    NumCode::type_t code;
    bool overflow;
    bool underflow;
    bool divisionByZero;
    bool invalidArgument;
};

template <typename Return>
struct FixedPointReturn
{
    using return_t = Return;

    FixedPointReturn(return_t _val, const FixedPointErrors& _err = FixedPointErrors()) : val(_val), err(_err){}

    operator return_t () { return val; }
    operator return_t () const { return val; }

    return_t operator = (return_t _other) {
        val = _other;
        return val;
    }

    bool valid() const { return err.ok(); }

    return_t val;
    FixedPointErrors err;
};

namespace internal
{

template<class Storage, Storage ValueMask, bool doMask>
struct MaskStorage
{
  static constexpr Storage exec(const Storage& _val) { return _val; }
};

template<class Storage, Storage ValueMask>
struct MaskStorage<Storage, ValueMask, true>
{
  static constexpr Storage exec(const Storage& _val) { return ValueMask & _val; }
};

template<class Storage, size_t TotalBits, bool doFill>
struct FillNegative
{
  static constexpr Storage exec(const Storage& _val) { return _val; }
};

template<class Storage, size_t TotalBits>
struct FillNegative<Storage, TotalBits, true>
{
  static constexpr size_t storage_bits{8*sizeof(Storage)};
  static constexpr size_t upper_bits{storage_bits - TotalBits};
  static constexpr Storage negativeBit{1<<(TotalBits-1)};
  static constexpr Storage negativeMask{static_cast<Storage>(((1<<upper_bits)-1)<<TotalBits)};

  static constexpr Storage exec(const Storage& _val) {
    return _val | (((_val & negativeBit) != 0) ? negativeMask : 0);
  }
};

//
// Sign Check Implementations
//
template<class Value, size_t Whole, size_t Fractional, size_t Offset = 0>
struct SignImpl
{
  using value_t = Value;
  static constexpr bool is_signed{std::is_signed<value_t>::value};
  static constexpr value_t value_mask{static_cast<value_t>(((1<<(Whole+Fractional))-1)<<Offset)};
  static constexpr value_t sign_bit{static_cast<value_t>(1<<(Whole+Fractional+Offset-1))};

  static constexpr bool negative(const value_t& _val) {
    return is_signed && ((_val&sign_bit) != 0);
  }

  static constexpr bool nonnegative(const value_t& _val) {
    return (is_signed ? ((_val&sign_bit) == 0) : true );
  }

  static constexpr bool positive(const value_t& _val) {
    const value_t val{static_cast<value_t>(value_mask & _val)};
    return (is_signed ? ((val&sign_bit) == 0) : true ) && (val != 0);
  }

  static constexpr bool zero(const value_t& _val) {
    return (_val&value_mask) == 0;
  }
};


//
// Addition Implementations with overflow handling
//
//  Default Addition Implementation
template<bool Signed, OverflowHandling::overflow_t OverflowHandlingFlag>
struct AdditionImpl
{
  template<class Value, size_t Whole, size_t Fractional>
  static constexpr Value exec(const Value& _a, const Value& _b) {
    return _a + _b;
  }
};

//  Signed Saturating Addition Implementation
template<>
struct AdditionImpl<true, OverflowHandling::Saturating>
{
  template<class Value, size_t Whole, size_t Fractional>
  static constexpr Value exec(const Value& _a, const Value& _b) {
    using sign_t = SignImpl<Value, Whole, Fractional>;
    static constexpr Value minimum{static_cast<Value>(1<<(Whole+Fractional-1))};
    static constexpr Value maximum{static_cast<Value>(minimum-1)};

    if(sign_t::nonnegative(_a)) {
      if(sign_t::nonnegative(_b)) { // Both Non-negative (positive overflow possible)
        if((maximum - _a) < _b) return maximum;
      }
    } else {
      if(sign_t::negative(_b)) { // Both Negative (negative overflow possible)
        if( ((_a + _b) & sign_t::sign_bit) == 0) {
          return minimum;
        }
      }
    }

    return _a + _b; // Alternate signs (overflow impossible)
  }
};

//  Unsigned Saturating Addition Implementation
template<>
struct AdditionImpl<false, OverflowHandling::Saturating>
{
  template<class Value, size_t Whole, size_t Fractional>
  static constexpr Value exec(const Value& _a, const Value& _b) {
    static constexpr Value maximum{static_cast<Value>((1<<(Whole+Fractional))-1)};
    if((maximum - _a) < _b) return maximum;
    return _a + _b;
  }
};

//
// Subtraction Implementations with overflow handling
//
//  Default Subtraction Implementation
template<bool Signed, OverflowHandling::overflow_t OverflowHandlingFlag>
struct SubtractionImpl
{
  template<class Value, size_t Whole, size_t Fractional>
  static constexpr Value exec(const Value& _a, const Value& _b) {
    return _a - _b;
  }
};

//  Signed Saturating Subtraction Implementation
template<>
struct SubtractionImpl<true, OverflowHandling::Saturating>
{
  template<class Value, size_t Whole, size_t Fractional>
  static constexpr Value exec(const Value& _a, const Value& _b) {
    return _a - _b;
  }
};

//  Unsigned Saturating Subtraction Implementation
template<>
struct SubtractionImpl<false, OverflowHandling::Saturating>
{
  template<class Value, size_t Whole, size_t Fractional>
  static constexpr Value exec(const Value& _a, const Value& _b) {
    return _a - _b;
  }
};
} /*namespace internal*/

template<
  class Store = int32_t,
  size_t Fractional = 16,
  size_t Total = meta::StorageSize<Store>::bits,
  typename Calc = int64_t,
  OverflowHandling::overflow_t OverflowHandlingFlag = OverflowHandling::Wrapping
>
class FixedPoint
{
    public:
        using storage_t = Store;
        using calc_t = Calc;
        using ref_t =  FixedPoint<Store, Fractional, Total, Calc, OverflowHandlingFlag>;

        static constexpr bool isSigned = std::is_signed<Store>::value;
        static constexpr size_t storageBits = meta::StorageSize<Store>::bits;
        static constexpr size_t totalBits = Total;
        static constexpr size_t unusedBits = storageBits - totalBits;
        static constexpr size_t fractionalBits = Fractional;
        static constexpr size_t wholeBits = totalBits-fractionalBits;

      protected:
        static constexpr storage_t integerMask = ((1<<wholeBits) - 1) << fractionalBits;
        static constexpr storage_t fractionalMask = (1<<fractionalBits) - 1;
        static constexpr storage_t valueMask = static_cast<storage_t>((1ull<<totalBits) - 1);
        using storage_sign_impl_t = internal::SignImpl<storage_t, wholeBits, fractionalBits>;
        using add_impl_t = internal::AdditionImpl<isSigned, OverflowHandlingFlag>;
        using sub_impl_t = internal::SubtractionImpl<isSigned, OverflowHandlingFlag>;

      public:
        //
        // Allow internal access to other overloads of the fixed-point class
        //
        template<class S, size_t F, size_t T, class C, OverflowHandling::overflow_t O>
        friend class FixedPoint;

        //
        // Construction
        //

        //  Default Constructor -- Zero Value
        constexpr FixedPoint() : storage_(0) {}

        //	Value Constructor
        template<typename Value, typename = decltype(static_cast<calc_t>(std::declval<Value>()))>
        constexpr FixedPoint( const Value& _value, const Flags::flags_t& _flags = Flags::None)
            : storage_(
                  processFlags(
                      internal::MaskStorage<
                        storage_t,
                        valueMask,
                        (storageBits>totalBits)
                      >::exec(
                        static_cast<storage_t>(_value*meta::raise_to_nth<fractionalBits, calc_t>(2))
                      ),
                      _value < 0,
                      _flags
                  )
             ) {}

        //	Conversion Constructor
        template<typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O>
        constexpr FixedPoint( const FixedPoint<S, F, T, C, O>& _other ) {
          if(F == fractionalBits) {
        		storage_ = _other.storage_;
        	} else {
            constexpr storage_t neg_one(-1);
            const bool invert = ( _other.storage_ < 0 );
    		    storage_t value = invert?(neg_one*static_cast<storage_t>(_other.storage_)):static_cast<storage_t>(_other.storage_);
            value = internal::shift<(static_cast<int16_t>(F) - static_cast<int16_t>(fractionalBits))>(value);
        		storage_ = internal::MaskStorage<
              storage_t,
              valueMask,
              (storageBits>totalBits)
            >::exec(
              static_cast<storage_t>(invert ? (neg_one*value) : value)
            );
        	}
        }

        //  Conversion Constructor
        template<typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O>
        constexpr FixedPoint(const FixedPointReturn<FixedPoint<S, F, T, C, O>>& _ret) : FixedPoint(_ret.val) {}

        //	Integer Scaled Static Factory
        static constexpr ref_t IntDiv(const storage_t& _value, const storage_t& _div, const Flags::flags_t& _flags = Flags::None) {
        	ref_t value;

        	// Calculate the whole number bits
          storage_t data = (_value / _div) << fractionalBits;

        	// Calculate the fractional number bits
          constexpr storage_t frac_max = static_cast<storage_t>(1)<<fractionalBits;
        	data |= static_cast<calc_t>((frac_max * static_cast<calc_t>(_value%_div)) / _div);

        	value.storage_ = internal::MaskStorage<
            storage_t,
            valueMask,
            (storageBits>totalBits)
          >::exec(
            processFlags(data, _value < 0, _flags)
          );
        	return value;
        }

        //	Integer Divison Static Factory
        static constexpr ref_t IntDiv(const ref_t& _value, const storage_t& _div, const Flags::flags_t& _flags = Flags::None) {
            ref_t value;
            const storage_t data = static_cast<storage_t>(static_cast<calc_t>(_value.storage_) / static_cast<calc_t>(_div));
            value.storage_ = internal::MaskStorage<
              storage_t,
              valueMask,
              (storageBits>totalBits)
            >::exec(
              processFlags(data, _value.isNegative(), _flags)
            );
            return value;
        }

        //	Direct Storage Set Static Factory
        static constexpr ref_t Storage(storage_t _value) {
            ref_t value;
            value.storage_ = internal::MaskStorage<
              storage_t,
              valueMask,
              (storageBits>totalBits)
            >::exec(
              _value
            );
            return value;
        }

        //
        // Access Operations
        //
        constexpr ref_t integer() const { return Storage(storage_&integerMask); }
        constexpr ref_t fractional() const { return Storage(storage_&fractionalMask); }
        constexpr bool isPositive() const { return storage_sign_impl_t::positive(storage_); }
        constexpr bool isNegative() const { return storage_sign_impl_t::negative(storage_); }
        constexpr bool isNonnegative() const { return storage_sign_impl_t::nonnegative(storage_); }
        constexpr bool isZero() const { return storage_sign_impl_t::zero(storage_); }

        //
        // Conversion Operators
        //
        //  Integral Conversion
        template<typename V = storage_t,
                 typename = std::enable_if_t<std::is_integral<V>::value, V>>
        constexpr operator V () const {
            return (storage_&integerMask) >> fractionalBits;
        }

        //  Floating-Point Conversion
        template<typename V1 = double, typename V2 = V1,
                 typename = std::enable_if_t<!std::is_integral<V2>::value, V1>>
        constexpr operator V1 () const {
            const storage_t value = internal::FillNegative<storage_t, totalBits, (storageBits>totalBits)>::exec(storage_);
            return static_cast<V1>(value) / meta::raise_to_nth<fractionalBits, V1>(2);
        }

        //
        // Operators
        //

        //	Assignment
        template<typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O>
        ref_t& operator = ( const FixedPoint<S, F, T, C, O>& _other ) {
            if(F == fractionalBits) { // TODO: ASSUMES THAT THE REMAINDER WILL BE ELIDED... CHECK THIS FOR OPTIMIZATION
                storage_ = _other.storage_;
            } else {
                constexpr storage_t neg_one(-1);
                const bool invert = ( _other.storage_ < 0 );
                storage_t value = invert ? (neg_one*static_cast<storage_t>(_other.storage_)) : static_cast<storage_t>(_other.storage_);
                value = internal::shift<(static_cast<int16_t>(F) - static_cast<int16_t>(fractionalBits))>(value);

                storage_ = internal::MaskStorage<
                  storage_t,
                  valueMask,
                  (storageBits>totalBits)
                >::exec(
                  invert ? (neg_one*value) : value
                );
            }
            return *this;
        }

        template<typename V>
        ref_t& operator = ( const V& _v ) {
            *this = ref_t(_v);
            return *this;
        }

        //	Direct access to storage value
        constexpr storage_t storage() const { return storage_; }

        //  Negation
        ref_t operator - () {
			constexpr storage_t neg_one(-1);
            storage_ = internal::MaskStorage<
              storage_t,
              valueMask,
              (storageBits>totalBits)
            >::exec(
              neg_one * storage_
            );
            return *this;
        }

        //	Addition-Assignment
        constexpr ref_t& operator += (ref_t _other) {
            storage_ = internal::MaskStorage<
              storage_t,
              valueMask,
              (storageBits>totalBits)
            >::exec(
              add_impl_t::exec<
                storage_t,
                wholeBits,
                fractionalBits
              >(storage_, _other.storage_)
            );
            return *this;
        }

        //	Subtraction-Assignment
        constexpr ref_t& operator -= (ref_t _other) {
            storage_ = internal::MaskStorage<
              storage_t,
              valueMask,
              (storageBits>totalBits)
            >::exec(
              sub_impl_t::exec<
                storage_t,
                wholeBits,
                fractionalBits
              >(storage_, _other.storage_)
            );
            return *this;
        }

        //	Multiply-Assignment
        constexpr ref_t& operator *= (ref_t _other) {
            storage_ = internal::MaskStorage<
              storage_t,
              valueMask,
              (storageBits>totalBits)
            >::exec(
              static_cast<storage_t>( ( (static_cast<calc_t>(storage_) * _other.storage_) / (1<<fractionalBits) ) )
            );

            return *this;
        }

        //	Division-Assignment
        constexpr ref_t& operator /= (ref_t _other) {
            storage_ = internal::MaskStorage<
              storage_t,
              valueMask,
              (storageBits>totalBits)
            >::exec(
              static_cast<storage_t>( (static_cast<calc_t>(storage_) * (1<<fractionalBits) ) / _other.storage_ )
            );

            return *this;
        }

    protected:
        // TODO: THIS NEEDS TO BE HANDLED BETTER TO MAKE SURE THAT IT USES ALL OF THE AVAILBLE FLAGS
        static constexpr storage_t processFlags(
                const storage_t& _storage,
                const bool& _is_negative,
                const Flags::flags_t& _flags )
        {
        	if( _flags == Flags::NoUnderflow) return (_storage != 0) ? _storage : (_is_negative ? 0xFFFFFFFF : 0x00000001);
        	return _storage;
        }

    private:
        storage_t storage_;
};

//
// "Easy" construction aliases
//
template<
  size_t Whole = 16,
  size_t Fractional = 16,
  OverflowHandling::overflow_t OverflowHandlingFlag = OverflowHandling::Wrapping,
  size_t Total = Whole + Fractional
>
using SignedFixedPoint = FixedPoint<
    typename iamb::meta::IambTypes<true, Total>::type,
    Fractional,
    Total,
    typename iamb::meta::IambTypes<true, 2 * Total>::type,
    OverflowHandlingFlag
  >;

template<
  size_t Whole = 16,
  size_t Fractional = 16
>
using SignedSaturatingFixedPoint = SignedFixedPoint<
    Whole,
    Fractional,
    OverflowHandling::Saturating,
	Whole + Fractional
  >;

template<
  size_t Whole = 16,
  size_t Fractional = 16,
  OverflowHandling::overflow_t OverflowHandlingFlag = OverflowHandling::Wrapping,
  size_t Total = Whole + Fractional
>
using UnsignedFixedPoint = FixedPoint<
    typename iamb::meta::IambTypes<false, Total>::type,
    Fractional,
    Total,
    typename iamb::meta::IambTypes<false, 2 * Total>::type,
    OverflowHandlingFlag
  >;

  template<
    size_t Whole = 16,
    size_t Fractional = 16
  >
  using UnsignedSaturatingFixedPoint = UnsignedFixedPoint<
      Whole,
      Fractional,
      OverflowHandling::Saturating,
	  Whole + Fractional
    >;
} /*namespace iamb*/

#endif /* IAMB_CORE_H */
