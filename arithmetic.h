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
template<typename S, size_t F, typename C>
constexpr FixedPoint<S, F, C> operator - (const FixedPoint<S, F, C>& _a) {
    constexpr FixedPoint<S, F, C> neg_one(-1);
    return neg_one * _a;
}

//  Addition
//      --> Symmetric
template<typename S1, size_t F1, typename C1, typename S2, size_t F2, typename C2>
constexpr FixedPoint<S1, F1, C1> operator + (
        const FixedPoint<S1, F1, C1>& _a,
        const FixedPoint<S2, F2, C2>& _b)
{
    FixedPoint<S1, F1, C1> temp(_a);
    temp += _b;
    return temp;
}

//      --> With Conversion
template<typename S, size_t F, typename C, typename V,
         typename = decltype(static_cast<C>(std::declval<V>()))>
constexpr FixedPoint<S, F, C> operator + (const FixedPoint<S, F, C>& _a, const V& _b) {
    const FixedPoint<S, F, C> b(_b);
    FixedPoint<S> temp(_a);
    temp += b;
    return temp;
}
template<typename V, typename S, size_t F, typename C,
         typename = decltype(static_cast<C>(std::declval<V>()))>
constexpr FixedPoint<S, F, C> operator + (const V& _a, const FixedPoint<S, F, C>& _b) {
    FixedPoint<S, F, C> temp(_a);
    temp += _b;
    return temp;
}


//  Subtraction
//      --> Symmetric
template<typename S1, size_t F1, typename C1, typename S2, size_t F2, typename C2>
constexpr FixedPoint<S1, F1, C1> operator - (
        const FixedPoint<S1, F1, C1>& _a,
        const FixedPoint<S2, F2, C2>& _b)
{
    FixedPoint<S1, F1, C1> temp(_a);
    temp -= _b;
    return temp;
}

//      --> With Conversion
template<typename S, size_t F, typename C, typename V,
         typename = decltype(static_cast<C>(std::declval<V>()))>
constexpr FixedPoint<S, F, C> operator - (const FixedPoint<S, F, C>& _a, const V& _b) {
    const FixedPoint<S, F, C> b(_b);
    FixedPoint<S> temp(_a);
    temp -= b;
    return temp;
}
template<typename V, typename S, size_t F, typename C,
         typename = decltype(static_cast<C>(std::declval<V>()))>
constexpr FixedPoint<S, F, C> operator - (const V& _a, const FixedPoint<S, F, C>& _b) {
    FixedPoint<S, F, C> temp(_a);
    temp -= _b;
    return temp;
}

//  Multiplication
//      --> Symmetric
template<typename S1, size_t F1, typename C1, typename S2, size_t F2, typename C2>
constexpr FixedPoint<S1, F1, C1> operator * (
        const FixedPoint<S1, F1, C1>& _a,
        const FixedPoint<S2, F2, C2>& _b)
{
    FixedPoint<S1, F1, C1> temp(_a);
    temp *= _b;
    return temp;
}

//      --> With Conversion
template<typename S, size_t F, typename C, typename V,
         typename = decltype(static_cast<C>(std::declval<V>()))>
constexpr FixedPoint<S, F, C> operator * (const FixedPoint<S, F, C>& _a, const V& _b) {
    const FixedPoint<S, F, C> b(_b);
    FixedPoint<S> temp(_a);
    temp *= b;
    return temp;
}
template<typename V, typename S, size_t F, typename C,
         typename = decltype(static_cast<C>(std::declval<V>()))>
constexpr FixedPoint<S, F, C> operator * (const V& _a, const FixedPoint<S, F, C>& _b) {
    FixedPoint<S, F, C> temp(_a);
    temp *= _b;
    return temp;
}

//  Division
//      --> Symmetric
template<typename S1, size_t F1, typename C1, typename S2, size_t F2, typename C2>
constexpr FixedPoint<S1, F1, C1> operator / (
        const FixedPoint<S1, F1, C1>& _a,
        const FixedPoint<S2, F2, C2>& _b)
{
    FixedPoint<S1, F1, C1> temp(_a);
    temp /= _b;
    return temp;
}

//      --> With Conversion
template<typename S, size_t F, typename C, typename V,
         typename = decltype(static_cast<C>(std::declval<V>()))>
constexpr FixedPoint<S, F, C> operator / (const FixedPoint<S, F, C>& _a, const V& _b) {
    const FixedPoint<S, F, C> b(_b);
    FixedPoint<S> temp(_a);
    temp /= b;
    return temp;
}
template<typename V, typename S, size_t F, typename C,
         typename = decltype(static_cast<C>(std::declval<V>()))>
constexpr FixedPoint<S, F, C> operator / (const V& _a, const FixedPoint<S, F, C>& _b) {
    FixedPoint<S, F, C> temp(_a);
    temp /= _b;
    return temp;
}

//
// Stream Output
//  -- Note: At the moment this is just formatting through the built-in double type
//
template<typename Stream, typename S, size_t F, typename C>
constexpr Stream& operator << (Stream& _stream, const iamb::FixedPoint<S, F, C>& _v) {
    _stream << static_cast<double>(_v);
    return _stream;

}

//
// Safe Value Comparisons
//
template<size_t N> // TODO: FIGURE OUT WHAT THE HECK THIS IS DOING!
struct Compare
{
    template<typename S, size_t F, typename C>
    static bool equalTo(const FixedPoint<S, F, C>& _a, const FixedPoint<S, F, C>& _b) {
        using value_t = FixedPoint<S, F, C>;
        using storage_t = typename value_t::storage_t;
        using calc_t = typename value_t::calc_t;

        static constexpr storage_t mask = ((calc_t(1UL)<<N)-1UL)<<(value_t::totalBits-N);
        return (_a.storage() & mask) == (_b.storage() & mask);
    }
};

//  Reciprocal
template<typename S, size_t F, typename C>
constexpr FixedPoint<S, F, C> reciprocal(const FixedPoint<S, F, C>& _a) {
    constexpr FixedPoint<S, F, C> one(1);
    return one/_a;
}
} /*namespace iamb*/

#endif /*IAMB_ARITHMETIC_H*/
