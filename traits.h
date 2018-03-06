#ifndef IAMB_TRAITS_H
#define IAMB_TRAITS_H

#include <type_traits>

#include "core.h"


namespace iamb
{
template<typename T>
struct is_fixed_point
{
        static constexpr bool value = false;
};

template<typename S, size_t F, typename C>
struct is_fixed_point<iamb::FixedPoint<S, F, C>>
{
        static constexpr bool value = true;
};
} /*namespace iamb*/

namespace std {

  // TODO: THESE SHOULD CHECK FOR CONVERTIBILITY OF THE VALUE TYPE TO A FIXEDPOINT
template<typename S1, size_t F1, typename C1, typename S2, size_t F2, typename C2>
struct common_type<iamb::FixedPoint<S1, F1, C1>, iamb::FixedPoint<S2, F2, C2>> {
        using type = iamb::FixedPoint<
            std::common_type_t<S1, S2>,
            F1, // TODO: MAKE THIS THE MINIMUM OF THE TWO VALUES?
            std::common_type_t<C1, C2>>;
};

template<typename S, size_t F, typename C, typename V>
struct common_type<iamb::FixedPoint<S, F, C>, V> {
        using type = std::enable_if_t<!iamb::is_fixed_point<V>::value, iamb::FixedPoint<S, F, C>>;
};

template<typename V, typename S, size_t F, typename C>
struct common_type<V, iamb::FixedPoint<S, F, C>> {
        using type = std::enable_if_t<!iamb::is_fixed_point<V>::value, iamb::FixedPoint<S, F, C>>;
};

template<typename S, size_t F, typename C>
struct is_integral<iamb::FixedPoint<S, F, C>> {
        static constexpr bool value = false;
};

template<typename S, size_t F, typename C>
struct is_floating_point<iamb::FixedPoint<S, F, C>> {
        static constexpr bool value = false;
};

} /*namespace std*/

#endif /*IAMB_TRAITS_H*/

