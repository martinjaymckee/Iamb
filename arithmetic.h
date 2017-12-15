//
//
// File - Iamb/Arithmetic:
//
//      Implementation of the Fixedpoint arithmetic functions.
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

#ifndef IAMB_ARITHMETIC_H
#define IAMB_ARITHMETIC_H

#include "Core"

namespace iamb
{

//
// Basic Arithmetical Operations
//
template<class _storage_type, uint8_t _frac_bits, class _calc_type>
FixedPoint<_storage_type, _frac_bits, _calc_type> operator - (const FixedPoint<_storage_type, _frac_bits, _calc_type>& _a) {
    static const FixedPoint<_storage_type, _frac_bits, _calc_type> neg_one(-1);
    return neg_one * _a;
}

template<class _storage_type, uint8_t _frac_bits, class _calc_type, class S, uint8_t F, class C>
FixedPoint<_storage_type, _frac_bits, _calc_type> operator + (const FixedPoint<_storage_type, _frac_bits, _calc_type>& _a, const FixedPoint<S, F, C>& _b) {
    FixedPoint<_storage_type> temp = _a;
    temp += _b;
    return temp;
}

template<class _storage_type, uint8_t _frac_bits, class _calc_type, class S, uint8_t F, class C>
FixedPoint<_storage_type, _frac_bits, _calc_type> operator - (const FixedPoint<_storage_type, _frac_bits, _calc_type>& _a, const FixedPoint<S, F, C>& _b) {
    FixedPoint<_storage_type, _frac_bits, _calc_type> temp = _a;
    temp -= _b;
    return temp;
}

template<class _storage_type, uint8_t _frac_bits, class _calc_type, class S, uint8_t F, class C>
FixedPoint<_storage_type, _frac_bits, _calc_type> operator * (const FixedPoint<_storage_type, _frac_bits, _calc_type>& _a, const FixedPoint<S, F, C>& _b) {
    FixedPoint<_storage_type, _frac_bits, _calc_type> temp = _a;
    temp *= _b;
    return temp;
}

template<class _storage_type, uint8_t _frac_bits, class _calc_type, class S, uint8_t F, class C>
FixedPoint<_storage_type, _frac_bits, _calc_type> operator / (const FixedPoint<_storage_type, _frac_bits, _calc_type>& _a, const FixedPoint<S, F, C>& _b) {
    FixedPoint<_storage_type, _frac_bits, _calc_type> temp = _a;
    temp /= _b;
    return temp;
}


template<uint8_t N>
struct Compare
{
    template<class S, uint8_t F, class C>
    static bool equalTo(const FixedPoint<S, F, C>& _a, const FixedPoint<S, F, C>& _b) {
        typedef FixedPoint<S, F, C> value_t;
        typedef typename value_t::storage_t storage_t;
        typedef typename value_t::calc_t calc_t;

        static const storage_t mask = ((calc_t(1UL)<<N)-1UL)<<(value_t::TOTAL_BITS-N);
        return (_a.storage() & mask) == (_b.storage() & mask);
    }

};

//  Reciprocal
template<class _storage_type, uint8_t _frac_bits, class _calc_type>
FixedPoint<_storage_type, _frac_bits, _calc_type> reciprocal(const FixedPoint<_storage_type, _frac_bits, _calc_type>& _a) {
    static const FixedPoint<_storage_type, _frac_bits, _calc_type> one(1);
    return one/_a;
}

} /*namespace iamb*/

#endif /*IAMB_ARITHMETIC_H*/
