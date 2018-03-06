//
//
// File - Iamb/Core:
//
//      Implementation of the Fixedpoint class and related support types.
//
//
// MIT License
//
// Copyright (c) 2017 Martin Jay McKee
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
		NONE,
		NO_UNDERFLOW,
		NO_OVERFLOW
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

// TODO: WHEN ERROR HANDLING IS ADDED TO THIS CLASS, THE ACTUAL ERROR HANDLING SHOULD BE CONTROLLED BY A TEMPLATE PARAMETER.
template<typename Store = int32_t, size_t Fractional = 16, typename Calc = int64_t>
class FixedPoint
{
    public: // TODO: MAKE THE EXTERNAL FUNCTIONS FRIEND FUNCTIONS! THIS SHOULD REALLY BE PROTECTED....
        using storage_t = Store;
        using calc_t = Calc;
        using ref_t =  FixedPoint<Store, Fractional, Calc>;

        static constexpr size_t totalBits = 8*sizeof(storage_t);
        static constexpr size_t fractionalBits = Fractional;
        static constexpr size_t wholeBits = totalBits-fractionalBits;
        static constexpr storage_t integerMask = ((1<<wholeBits) - 1) << fractionalBits;
        static constexpr storage_t fractionalMask = (1<<fractionalBits) - 1;

        template<class S, size_t F, class C>
        friend class FixedPoint;

        //
        // Construction
        //

        //  Default Constructor -- Zero Value
        constexpr FixedPoint() : storage_(0) {}

        //	Value Constructor
        template<typename Value, typename = decltype(static_cast<calc_t>(std::declval<Value>()))>
        constexpr FixedPoint( const Value& _value, const Flags::flags_t& _flags = Flags::NONE)
            : storage_(
                  processFlags(
                      static_cast<storage_t>(_value*meta::raise_to_nth<fractionalBits, calc_t>(2)),
                      _value < 0,
                      _flags
                  )
             ) {}

        //	Conversion Constructor
        template<typename S, size_t F, typename C>
        constexpr FixedPoint( const FixedPoint<S, F, C>& _other ) {

            if(F == fractionalBits) {
        		storage_ = _other.storage_;
        	} else {
                constexpr storage_t neg_one(-1);
                const bool invert = ( _other.storage_ < 0 );
        		storage_t value = invert?(neg_one*static_cast<storage_t>(_other.storage_)):static_cast<storage_t>(_other.storage_);

                value = internal::shift<(static_cast<int16_t>(F) - static_cast<int16_t>(fractionalBits))>(value);

        		storage_ = invert ? (neg_one*value) : value;
        	}
        }

        template<typename S, size_t F, typename C>
        constexpr FixedPoint(const FixedPointReturn<FixedPoint<S, F, C>>& _ret) : FixedPoint(_ret.val) {}

        //	Integer Scaled Static Factory
        static constexpr ref_t IntDiv(const storage_t& _value, const storage_t& _div, const Flags::flags_t& _flags = Flags::NONE) {
        	ref_t value;

        	// Calculate the whole number bits
            storage_t data = (_value / _div) << fractionalBits;

        	// Calculate the fractional number bits
            constexpr storage_t frac_max = static_cast<storage_t>(1)<<fractionalBits;
        	data |= static_cast<calc_t>((frac_max * static_cast<calc_t>(_value%_div)) / _div);

        	value.storage_ = processFlags(data, _value < 0, _flags);
        	return value;
        }

        //	Integer Divison Static Factory
        static constexpr ref_t IntDiv(const ref_t& _value, const storage_t& _div, const Flags::flags_t& _flags = Flags::NONE) {
            ref_t value;
            const storage_t data = static_cast<storage_t>(static_cast<calc_t>(_value.storage_) / static_cast<calc_t>(_div));
            value.storage_ = processFlags(data, _value.isNegative(), _flags);
            return value;
        }

        //	Direct Storage Set Static Factory
        static constexpr ref_t Storage(const storage_t& _value) {
            ref_t value;
            value.storage_ = _value;
            return value;
        }

        //
        // Access Operations
        //
        constexpr ref_t integer() const { return Storage(storage_&integerMask); }
        constexpr ref_t fractional() const { return Storage(storage_&fractionalMask); }
        constexpr bool isPositive() const { return storage_ > 0; }
        constexpr bool isNegative() const { return storage_ < 0; }
        constexpr bool isZero() const { return storage_ == 0; }

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
            return static_cast<V1>(storage_) / meta::raise_to_nth<fractionalBits, V1>(2);
        }

        //
        // Operators
        //
        //		TODO: These should be modified so that they can be used on different FixedPoint classes....
        //

        //	Assignment
        template<typename S, size_t F, typename C>
        ref_t& operator = ( const FixedPoint<S, F, C>& _other ) {
            if(F == fractionalBits) { // TODO: ASSUMES THAT THE REMAINDER WILL BE ELIDED... CHECK THIS FOR OPTIMIZATION
                storage_ = _other.storage_;
            } else {
                constexpr storage_t neg_one(-1);
                const bool invert = ( _other.storage_ < 0 );
                storage_t value = invert?(neg_one*static_cast<storage_t>(_other.storage_)):static_cast<storage_t>(_other.storage_);

                value = internal::shift<(static_cast<int16_t>(F) - static_cast<int16_t>(fractionalBits))>(value);

                storage_ = invert ? (neg_one*value) : value;
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

        //	Conversion to Floating Point
//        constexpr operator float() const { return static_cast<double>(storage_) / pow(2.0,fractionalBits); }
//        constexpr operator double() const { return static_cast<double>(storage_) / pow(2.0,fractionalBits); }


        //	Addition-Assignment
        constexpr ref_t& operator += (const ref_t& _other) {
            storage_ += _other.storage_;
            return *this;
        }

        //	Subtraction-Assignment
        constexpr ref_t& operator -= (const ref_t& _other) {
            storage_ -= _other.storage_;
            return *this;
        }

        //	Multiply-Assignment
        constexpr ref_t& operator *= (const ref_t& _other) {
            storage_ = static_cast<storage_t>( ( (static_cast<calc_t>(storage_) * _other.storage_) / (1<<fractionalBits) ) );
            return *this;
        }

        //	Division-Assignment
        constexpr ref_t& operator /= (const ref_t& _other) {
            storage_ = static_cast<storage_t>( (static_cast<calc_t>(storage_) * (1<<fractionalBits) ) / _other.storage_ );
            return *this;
        }

    protected:
        // TODO: THIS NEEDS TO BE HANDLED BETTER TO MAKE SURE THAT IT USES ALL OF THE AVAILBLE FLAGS
        static constexpr storage_t processFlags(
                const storage_t& _storage,
                const bool& _is_negative,
                const Flags::flags_t& _flags )
        {
        	if( _flags == Flags::NO_UNDERFLOW) return (_storage != 0) ? _storage : (_is_negative ? 0xFFFFFFFF : 0x00000001);
        	return _storage;
        }

    private:
        storage_t storage_;
};
} /*namespace iamb*/

#endif /* IAMB_CORE_H */
