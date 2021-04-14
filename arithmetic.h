//
//
// File - Iamb/arithmetic.h:
//
//      Implementation of the FixedPoint arithmetic functions.
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

#include "core.h"

namespace iamb
{

//
// Basic Arithmetic Operations
//
//  Negation
template<typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O>
constexpr FixedPoint<S, F, T, C, O> operator - (FixedPoint<S, F, T, C, O> _a) {
    -_a;
    return _a;
}

//  Addition
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr FixedPoint<S1, F1, T1, C1, O1> operator + (
        FixedPoint<S1, F1, T1, C1, O1> _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b)
{
    _a += _b;
    return _a;
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr FixedPoint<S, F, T, C, O> operator + (FixedPoint<S, F, T, C, O> _a, const V& _b) {
    const FixedPoint<S, F, T, C, O> b(_b);
    _a += b;
    return _a;
}
template<
  typename V,
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr FixedPoint<S, F, T, C, O> operator + (const V& _a, const FixedPoint<S, F, T, C, O>& _b) {
    FixedPoint<S, F, T, C, O> temp(_a);
    temp += _b;
    return temp;
}


//  Subtraction
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr FixedPoint<S1, F1, T1, C1, O1> operator - (
        FixedPoint<S1, F1, T1, C1, O1> _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b)
{
    _a -= _b;
    return _a;
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr FixedPoint<S, F, T, C, O> operator - (FixedPoint<S, F, T, C, O> _a, const V& _b) {
    const FixedPoint<S, F, T, C, O> b(_b);
    _a -= b;
    return _a;
}
template<
  typename V,
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr FixedPoint<S, F, T, C, O> operator - (const V& _a, const FixedPoint<S, F, T, C, O>& _b) {
    FixedPoint<S, F, T, C, O> temp(_a);
    temp -= _b;
    return temp;
}

//  Multiplication
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr FixedPoint<S1, F1, T1, C1, O1> operator * (
        FixedPoint<S1, F1, T1, C1, O1> _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b)
{
    _a *= _b;
    return _a;
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr FixedPoint<S, F, T, C, O> operator * (FixedPoint<S, F, T, C, O> _a, const V& _b) {
    const FixedPoint<S, F, T, C, O> b(_b);
    _a *= b;
    return _a;
}

template<
  typename V,
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr FixedPoint<S, F, T, C, O> operator * (const V& _a, const FixedPoint<S, F, T, C, O>& _b) {
    FixedPoint<S, F, T, C, O> temp(_a);
    temp *= _b;
    return temp;
}


//  Division
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr FixedPoint<S1, F1, T1, C1, O1> operator / (
        FixedPoint<S1, F1, T1, C1, O1> _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b)
{
    _a /= _b;
    return _a;
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr FixedPoint<S, F, T, C, O> operator / (FixedPoint<S, F, T, C, O> _a, const V& _b) {
    const FixedPoint<S, F, T, C, O> b(_b);
    _a /= b;
    return _a;
}
template<
  typename V,
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr FixedPoint<S, F, T, C, O> operator / (const V& _a, const FixedPoint<S, F, T, C, O>& _b) {
    FixedPoint<S, F, T, C, O> temp(_a);
    temp /= _b;
    return temp;
}

//
// Stream Output
//  -- Note: At the moment this is just formatting through the built-in double type
//
template<typename Stream, typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O>
constexpr Stream& operator << (Stream& _stream, const iamb::FixedPoint<S, F, T, C, O>& _v) {
    _stream << static_cast<double>(_v);
    return _stream;

}

/*
//
// Safe Value Comparisons
//
template<size_t N> // TODO: FIGURE OUT WHAT THE HECK THIS IS DOING!
struct Compare
{
    template<typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O>
    static bool equalTo(const FixedPoint<S, F, T, C, O>& _a, const FixedPoint<S, F, T, C, O>& _b) {
        using value_t = FixedPoint<S, F, T, C, O>;
        using storage_t = typename value_t::storage_t;
        using calc_t = typename value_t::calc_t;

        static constexpr storage_t mask = ((calc_t(1UL)<<N)-1UL)<<(value_t::totalBits-N);
        return (_a.storage() & mask) == (_b.storage() & mask);
    }
};
*/

//  Reciprocal
template<typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O>
constexpr FixedPoint<S, F, T, C, O> reciprocal(const FixedPoint<S, F, T, C, O>& _a) {
    static constexpr FixedPoint<S, F, T, C, O> one(1);
    return one/_a;
}
} /*namespace iamb*/

#endif /*IAMB_ARITHMETIC_H*/
