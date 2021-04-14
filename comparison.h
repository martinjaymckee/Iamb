//
//
// File - Iamb/comparison.h:
//
//      Implementation of the iamb::FixedPoint comparison functions.
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

#ifndef IAMB_COMPARISON_H
#define IAMB_COMPARISON_H

namespace iamb
{

// TODO: NEED TO GET THESE WORKING PROPERLY AS THIS ONLY WORKS FOR IDENTICAL FIXEDPOINT TYPES

//
// Comparison Operators
//
//  Equality
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr bool operator == (
        const FixedPoint<S1, F1, T1, C1, O1>& _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b
) {
    return _a.storage() == _b.storage();
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator == (
        const FixedPoint<S, F, T, C, O>& _a,
        const V& _b
) {
    return _a.storage() == FixedPoint<S, F, T, C, O>(_b).storage();
}

template<
  typename V, typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator == (
        const V& _a,
        const FixedPoint<S, F, T, C, O>& _b
) {
    return FixedPoint<S, F, T, C, O>(_a).storage() == _b.storage();
}

//  Inequality
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr bool operator != (
        const FixedPoint<S1, F1, T1, C1, O1>& _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b
) {
    return _a.storage() != _b.storage();
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator != (
        const FixedPoint<S, F, T, C, O>& _a,
        const V& _b
) {
    return _a.storage() != FixedPoint<S, F, T, C, O>(_b).storage();
}

template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator != (
        const V& _a,
        const FixedPoint<S, F, T, C, O>& _b
) {
    return FixedPoint<S, F, T, C, O>(_a).storage() != _b.storage();
}

//  Greater Than
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr bool operator > (
        const FixedPoint<S1, F1, T1, C1, O1>& _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b
) {
    return _a.storage() > _b.storage();
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator > (
        const FixedPoint<S, F, T, C, O>& _a,
        const V& _b
) {
    return _a.storage() > FixedPoint<S, F, T, C, O>(_b).storage();
}

template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator > (
        const V& _a,
        const FixedPoint<S, F, T, C, O>& _b
) {
    return FixedPoint<S, F, T, C, O>(_a).storage() > _b.storage();
}

//  Greater Than or Equal
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr bool operator >= (
        const FixedPoint<S1, F1, T1, C1, O1>& _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b
) {
    return _a.storage() >= _b.storage();
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator >= (
        const FixedPoint<S, F, T, C, O>& _a,
        const V& _b
) {
    return _a.storage() >= FixedPoint<S, F, T, C, O>(_b).storage();
}

template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator >= (
        const V& _a,
        const FixedPoint<S, F, T, C, O>& _b
) {
    return FixedPoint<S, F, T, C, O>(_a).storage() >= _b.storage();
}

//  Less Than
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr bool operator < (
        const FixedPoint<S1, F1, T1, C1, O1>& _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b
) {
    return _a.storage() < _b.storage();
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator < (
        const FixedPoint<S, F, T, C, O>& _a,
        const V& _b
) {
    return _a.storage() < FixedPoint<S, F, T, C, O>(_b).storage();
}

template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator < (
        const V& _a,
        const FixedPoint<S, F, T, C, O>& _b
) {
    return FixedPoint<S, F, T, C, O>(_a).storage() < _b.storage();
}

//  Less Than or Equal
//      --> Symmetric
template<
  typename S1, size_t F1, size_t T1, typename C1, OverflowHandling::overflow_t O1,
  typename S2, size_t F2, size_t T2, typename C2, OverflowHandling::overflow_t O2
>
constexpr bool operator <= (
        const FixedPoint<S1, F1, T1, C1, O1>& _a,
        const FixedPoint<S2, F2, T2, C2, O2>& _b
) {
    return _a.storage() <= _b.storage();
}

//      --> With Conversion
template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator <= (
        const FixedPoint<S, F, T, C, O>& _a,
        const V& _b
) {
    return _a.storage() <= FixedPoint<S, F, T, C, O>(_b).storage();
}

template<
  typename S, size_t F, size_t T, typename C, OverflowHandling::overflow_t O,
  typename V, typename = decltype(static_cast<C>(std::declval<V>()))
>
constexpr bool operator <= (
        const V& _a,
        const FixedPoint<S, F, T, C, O>& _b
) {
    return FixedPoint<S, F, T, C, O>(_a).storage() <= _b.storage();
}

} /*namespace iamb*/

#endif /*IAMB_COMPARISON_H*/
