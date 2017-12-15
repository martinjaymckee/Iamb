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
#include <stdint.h>

#include "meta.h"

//#include <iostream>
//using namespace std;

namespace iamb
{

namespace NumCode {
enum Type {
    Normal,
    PositiveInfinity,
    NegativeInfinity,
    NaN
};
}

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

    NumCode::Type code;
    bool overflow;
    bool underflow;
    bool divisionByZero;
    bool invalidArgument;
};

template <class _return_type>
struct FixedPointReturn
{
    typedef _return_type return_t;

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
template<class _storage_type = int32_t, uint8_t _frac_bits = 16, class _calc_type = int64_t>
class FixedPoint
{
    public: // TODO: MAKE THE EXTERNAL FUNCTIONS FRIEND FUNCTIONS! THIS SHOULD REALLY BE PROTECTED....
        typedef _storage_type storage_t;
        typedef _calc_type calc_t;
        typedef FixedPoint<_storage_type, _frac_bits, _calc_type> ref_t;

        static const uint8_t TOTAL_BITS = 8*sizeof(storage_t);
        static const uint8_t FRAC_BITS = _frac_bits;
        static const uint8_t WHOLE_BITS = TOTAL_BITS-FRAC_BITS;
        static const storage_t INTEGER_MASK = ((1<<WHOLE_BITS) - 1) << FRAC_BITS;
        static const storage_t FRACTIONAL_MASK = (1<<FRAC_BITS) - 1;

    public:
        template<class _other_storage_type, uint8_t _other_frac_bits, class _other_calc_type>
        friend class FixedPoint;

        //
        // Construction
        //

        //  Default Constructor -- Zero Value
        FixedPoint() : storage_(0) {}

        //	Constructor from Double
        FixedPoint( const double& _value, const Flags::flags_t& _flags = Flags::NONE) {
        	const storage_t data = static_cast<storage_t>(_value*pow(2.0,FRAC_BITS)); // TODO: THIS NEEDS TO ROUND TO THE NEAREST FRACTION TOO
        	storage_ = processFlags(data, _value < 0.0, _flags);
        }

        //	Conversion Constructor
        template<class _other_storage_type, uint8_t _other_frac_bits, class _other_calc_type>
        FixedPoint( const FixedPoint<_other_storage_type, _other_frac_bits, _other_calc_type>& _other ) {

        	if(_other_frac_bits == FRAC_BITS) {
        		storage_ = _other.storage_;
        	} else {
        		static const storage_t neg_one(-1);
        		const bool invert = ( _other.storage_ < 0 );
        		storage_t value = invert?(neg_one*static_cast<storage_t>(_other.storage_)):static_cast<storage_t>(_other.storage_);

        		value = Shift<storage_t, (static_cast<int16_t>(_other_frac_bits) - static_cast<int16_t>(FRAC_BITS))>::exec(value);

        		storage_ = invert ? (neg_one*value) : value;
        	}
        }

        //	Integer Scaled Static Factory
        static ref_t IntDiv(const storage_t& _value, const storage_t& _div, const Flags::flags_t& _flags = Flags::NONE) {
        	ref_t value;

        	// Calculate the whole number bits
        	storage_t data = (_value / _div) << FRAC_BITS;

        	// Calculate the fractional number bits
            static const storage_t frac_max = static_cast<storage_t>(1)<<FRAC_BITS;
        	data |= static_cast<calc_t>((frac_max * static_cast<calc_t>(_value%_div)) / _div);

        	value.storage_ = processFlags(data, _value < 0, _flags);
        	return value;
        }

        //	Integer Divison Static Factory
        static ref_t IntDiv(const ref_t& _value, const storage_t& _div, const Flags::flags_t& _flags = Flags::NONE) {
            ref_t value;
            const storage_t data = static_cast<storage_t>(static_cast<calc_t>(_value.storage_) / static_cast<calc_t>(_div));
            value.storage_ = processFlags(data, _value.isNegative(), _flags);
            return value;
        }

        //	Direct Storage Set Static Factory
        static ref_t Storage(const storage_t& _value) {
            ref_t value;
            value.storage_ = _value;
            return value;
        }

        //
        // Access Operations
        //
        ref_t integer() const { return Storage(storage_&INTEGER_MASK); }
        ref_t fractional() const { return Storage(storage_&FRACTIONAL_MASK); }
        bool isPositive() const { return storage_ > 0; }
        bool isNegative() const { return storage_ < 0; }
        bool isZero() const { return storage_ == 0; }
        storage_t asInt() const { return (storage_&INTEGER_MASK) >> FRAC_BITS; }

        //
        // Operators
        //
        //		TODO: These should be modified so that they can be used on different FixedPoint classes....
        //

        //	Assignment -- Need to correct the storage....
        ref_t& operator = ( const ref_t& _other ) {
            storage_ = _other.storage_;
            return *this;
        }


        //	Direct access to storage value
        storage_t storage() const { return storage_; }

        //	Conversion to Double
        operator double() const { return static_cast<double>(storage_) / pow(2.0,FRAC_BITS); }

        //	Addition-Assignment
        ref_t& operator += (const ref_t& _other) {
            storage_ += _other.storage_;
            return *this;
        }

        //	Subtraction-Assignment
        ref_t& operator -= (const ref_t& _other) {
            storage_ -= _other.storage_;
            return *this;
        }

        //	Multiply-Assignment
        ref_t& operator *= (const ref_t& _other) {
            storage_ = static_cast<storage_t>( ( (static_cast<calc_t>(storage_) * _other.storage_) / (1<<FRAC_BITS) ) );
            return *this;
        }

        //	Division-Assignment
        ref_t& operator /= (const ref_t& _other) {
            storage_ = static_cast<storage_t>( (static_cast<calc_t>(storage_) * (1<<FRAC_BITS) ) / _other.storage_ );
            return *this;
        }

        //
        //	Comparison Operators
        //
        //		TODO: These should be modified so that they can be used on different FixedPoint classes....
        //
        bool operator == (const ref_t& _other) const {
        	return storage_ == _other.storage_;
        }

        bool operator != (const ref_t& _other) const {
        	return !this->operator ==(_other);
        }

        bool operator >= (const ref_t& _other) const {
        	return storage_ >= _other.storage_;
        }

        bool operator < (const ref_t& _other) const {
        	return !this->operator >= (_other);
        }

        bool operator <= (const ref_t& _other) const {
        	return storage_ <= _other.storage_;
        }

        bool operator > (const ref_t& _other) const {
        	return !this->operator <= (_other);
        }

    protected:
        // TODO: THIS NEEDS TO BE HANDLED BETTER TO MAKE SURE THAT IT USES ALL OF THE AVAILBLE FLAGS
        static storage_t processFlags(const storage_t& _storage, const bool& _is_negative, const Flags::flags_t& _flags ) {
        	if( _flags == Flags::NO_UNDERFLOW) return (_storage != 0) ? _storage : (_is_negative ? 0xFFFFFFFF : 0x00000001);
        	return _storage;
        }

    private:
        storage_t storage_;
};
} /* namespace iamb */

#endif /* IAMB_CORE_H */
