//
//
// File - Iamb/elementary.h:
//
//      Implementation of the elementary functions applied to Fixedpoint values.
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

#ifndef IAMB_ELEMENTARY_H
#define IAMB_ELEMENTARY_H

#include "core.h"
#include "arithmetic.h"

//
// NOTE: GOOD REFERENCES FOR SIN/COS, ATAN2, ASIN/ACOS INCLUDE:
// http://www.olliw.eu/2014/fast-functions/
//
//
namespace iamb
{
//
// Elementary Functions
//

//	Absolute Value
template<class S, size_t F, class C>
FixedPoint<S, F, C> abs( const FixedPoint<S, F, C>& _val) {
    typedef FixedPoint<S, F, C> value_t;
    if(_val.storage() < 0) return value_t::Storage(static_cast<S>(-1) * _val.storage());
    return _val;
}

template<typename T> struct TD;
//	Square Root Function using the Babylonian Method
template<class S, size_t F, class C, size_t Nlimit=10>
FixedPoint<S, F, C> sqrt(const FixedPoint<S, F, C>& _val) {
    typedef FixedPoint<S, F, C> value_t;
    static const value_t half(0.5);
	value_t Xn = _val;
    value_t Xn_last(Xn);

    for(size_t iter = 0; iter < Nlimit; ++iter ) {
		Xn = half * (Xn + (_val / Xn));
        if(Xn.storage() == Xn_last.storage()) break;
        Xn_last = Xn;
	}

	return Xn;
}

//	Inverse Square-Root Function
template<class S, size_t F, class C>
FixedPoint<S, F, C> invSqrt( const FixedPoint<S, F, C>& _val ) {
    typedef FixedPoint<S, F, C> value_t;
	value_t Xn = _val;

	// TODO: Implement inverse square root
	return Xn;
}

//	Log base 2
//      Note: This function is based upon code on github by dmoulding at https://github.com/dmoulding/log2fix
//          the code is, in turn, based upon the algorithm for a binary log2 found in "A fast binary logarithm algorithm" by Clay S. Turner
//
template<class S, size_t F, class C>
FixedPointReturn<FixedPoint<S, F, C> > log2( const FixedPoint<S, F, C>& _val) {
    typedef FixedPoint<S, F, C> value_t;
    typedef typename value_t::storage_t storage_t;
    typedef typename value_t::calc_t calc_t;
    typedef FixedPointReturn<value_t> return_t;
    FixedPointErrors err;

    storage_t b = 1UL << (value_t::FRAC_BITS-1UL);
    storage_t y = 0;
    storage_t x = _val.storage(); // This is a hack... it would be better to make these friend functions....

    if(x == 0) { // Return -inf for an argument of zero
        err.invalidArgument = true;
        err.code = NumCode::NegativeInfinity;
        y = 0;
    } else if(x < 0){ // Return NaN for
        err.invalidArgument = true;
        err.code = NumCode::NaN;
        y = 0;
    } else {
        //
        // Do range reduction to [1.0, 2.0]
        //
        static const storage_t upper = 2<<value_t::FRAC_BITS;
        static const storage_t lower = 1<<value_t::FRAC_BITS;

        while(x < lower) {
            x <<= 1;
            y -= lower;
        }

        while(x >= upper) {
            x >>= 1;
            y += lower;
        }

        //
        // Calculate the final reduced range log2
        //
        calc_t z = x;

        for (size_t i = 0; i < value_t::FRAC_BITS; ++i) {
            z = (z * z) >> value_t::FRAC_BITS;
            if(z == 0) {
                break;
            } else if (z >= upper) {
                z >>= 1;
                y += b;
            }

            b >>= 1;
        }
    }

    return return_t(value_t::Storage(y), err);
}

//	Log base e
template<class S, size_t F, class C>
FixedPointReturn<FixedPoint<S, F, C> > ln( const FixedPoint<S, F, C>& _val) {
    typedef FixedPoint<S, F, C> value_t;
    typedef FixedPointReturn<value_t> return_t;

    static const value_t scale(0.69314718056); // ln(2)
    const return_t result = log2(_val);
    if(!result.err.ok()) return result;
    return return_t(scale*result.val, result.err);
}

//  Log base 10
template<class S, size_t F, class C>
FixedPointReturn<FixedPoint<S, F, C> > log10( const FixedPoint<S, F, C>& _val) {
    typedef FixedPoint<S, F, C> value_t;
    typedef FixedPointReturn<value_t> return_t;

    static const value_t scale(.301029995664); // log10(2)
    const return_t result = log2(_val);
    if(!result.err.ok()) return result;
    return return_t(scale*result.val, result.err);
}

//	2^x
template<class S, size_t F, class C>
FixedPointReturn<FixedPoint<S, F, C> > exp2( const FixedPoint<S, F, C>& _x) {
    typedef FixedPoint<S, F, C> value_t;
    typedef typename value_t::storage_t storage_t;
    typedef FixedPointReturn<value_t> return_t;
    FixedPointErrors err;

    const bool negative_exp = _x.isNegative();
    const value_t exp = abs(_x);
    const storage_t i = exp.integer().storage() >> value_t::FRAC_BITS;

    // Calculate the integer power portion of the exponent
    const value_t i_part(1<<i);


    // Calculate the fractional power portion of the exponent
    //  This is using a standard taylor series expansion... this can likely be optimized
    static const value_t ln2(0.69314718056); // ln(2)
    value_t x(ln2*exp.fractional());
    value_t z(x);
    value_t f_part = 1 + z;
    storage_t div = 1;

    size_t term = 2;
    while(true) {
        div *= term;
        z *= x;
        const value_t inc = value_t::IntDiv(z,div);
        if(inc.isZero()) break;
        f_part += inc;
        ++term;
    }

    // Multiply integer and fractional parts, and return
    const value_t y(i_part*f_part);
    return return_t(negative_exp ? reciprocal(y) : y, err);
}

//	e^x
template<class S, size_t F, class C>
FixedPointReturn<FixedPoint<S, F, C> > exp( const FixedPoint<S, F, C>& _val) {
    typedef FixedPoint<S, F, C> value_t;

    static const value_t scale(1.44269504089); // log2(e)
    return log2(scale*_val);
}

//  10^x
template<class S, size_t F, class C>
FixedPointReturn<FixedPoint<S, F, C> > exp10( const FixedPoint<S, F, C>& _val) {
    typedef FixedPoint<S, F, C> value_t;

    static const value_t scale(3.32192809489); // log2(10)
    return log2(scale*_val);
}

//	x^y
template<class S1, size_t F1, class C1, class S2, size_t F2, class C2>
FixedPointReturn<FixedPoint<S1, F1, C1> > pow( const FixedPoint<S1, F1, C1>& _x, const FixedPoint<S2, F2, C2>& _y) {
    using value_t = FixedPoint<S1, F1, C1>;
    using return_t = FixedPointReturn<value_t>;

    const return_t result = log2(_x);
    if(!result.err.ok()) return result;
    return exp2(_y * result.val);
}

//
// Trigonometric Functions
//
//  acos
template<class S, size_t F, class C>
FixedPointReturn<FixedPoint<S, F, C> > acos( const FixedPoint<S, F, C>& _val) {
    using value_t = FixedPoint<S, F, C>;
    static constexpr value_t one(1);
    static constexpr value_t two(2);
    static constexpr value_t pi(3.14159265358979);
    static constexpr value_t a(-0.0187293);
    static constexpr value_t b(0.0742610);
    static constexpr value_t c(-0.2121144);
    static constexpr value_t d(pi/2); // 1.5707288
    const value_t negate((_val.isNegative() ? 1 : 0));
    value_t v(a*_val);
    v += b;
    v *= _val;
    v += c;
    v *= sqrt(one-_val);
    v -= two * negate * v;
    return (negate * pi) + v;
}
} /*namespace iamb*/

#endif /*IAMB_ELEMENTARY_H*/
