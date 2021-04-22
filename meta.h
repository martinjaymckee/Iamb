#ifndef IAMB_META_H
#define IAMB_META_H

#include <stdint.h>

namespace iamb
{
namespace meta
{
namespace internal
{
template<bool Signed, bool gtEight, bool gtSixteen, bool gtThirtytwo, bool gtSixtyfour>
struct IambTypesImpl
{
  using type = void;
};

//
// Unsigned Types
//
template<>
struct IambTypesImpl<false, false, false, false, false> { using type = uint8_t; };

template<>
struct IambTypesImpl<false, true, false, false, false> { using type = uint16_t; };

template<>
struct IambTypesImpl<false, true, true, false, false> { using type = uint32_t; };

template<>
struct IambTypesImpl<false, true, true, true, false> { using type = uint64_t; };

template<>
struct IambTypesImpl<false, true, true, true, true> { using type = void; };

//
// Signed Types
//
template<>
struct IambTypesImpl<true, false, false, false, false> { using type = int8_t; };

template<>
struct IambTypesImpl<true, true, false, false, false> { using type = int16_t; };

template<>
struct IambTypesImpl<true, true, true, false, false> { using type = int32_t; };

template<>
struct IambTypesImpl<true, true, true, true, false> { using type = int64_t; };

template<>
struct IambTypesImpl<true, true, true, true, true> { using type = void; };

} /* namespace internal */

template<bool Signed, size_t Total>
struct IambTypes
{
  using type = typename internal::IambTypesImpl<
      Signed,
      (Total > 8),
      (Total > 16),
      (Total > 32),
      (Total > 64)
    >::type;
};

} /* namespace meta */
} /* namespace iamb*/

#endif /* IAMB_META_H */
