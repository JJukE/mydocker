// Copyright (c) 2023-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! \brief C++14-compatible implementation of select functionality from C++ `<numeric>` library.
#pragma once

#include "TypeTraits.h"

namespace carb
{
namespace cpp
{

//! \cond DEV
namespace detail
{

template <class Signed, std::enable_if_t<std::is_signed<Signed>::value, bool> = false>
constexpr inline std::make_unsigned_t<Signed> abs(const Signed val) noexcept
{
    using Unsigned = std::make_unsigned_t<Signed>;
    if (val < 0)
        return Unsigned(0) - Unsigned(val);
    return Unsigned(val);
}

template <class Unsigned, std::enable_if_t<std::is_unsigned<Unsigned>::value, bool> = false>
constexpr inline Unsigned abs(const Unsigned val) noexcept
{
    return val;
}

template <class Unsigned>
constexpr inline unsigned long bitscan_forward(Unsigned mask) noexcept
{
    // Since carb::cpp::countr_zero isn't constexpr...
    static_assert(std::is_unsigned<Unsigned>::value, "Must be an unsigned value");

    unsigned long count = 0;
    if (mask != 0)
    {
        while ((mask & 1u) == 0)
        {
            mask >>= 1;
            ++count;
        }
    }
    return count;
}

template <class T>
using NotBoolIntegral =
    ::carb::cpp::bool_constant<std::is_integral<T>::value && !std::is_same<std::remove_cv_t<T>, bool>::value>;

} // namespace detail
//! \endcond

/**
 * Computes the greatest common divisor of two integers.
 *
 * If either `M` or `N` is not an integer type, or if either is (possibly cv-qualified) `bool`, the program is ill-
 * formed. If either `|m|` or `|n|` is not representable as a value of type `std::common_type_t<M, N>`, the behavior is
 * undefined.
 * @param m Integer value
 * @param n Integer value
 * @returns If both @p m and @p n are 0, returns 0; otherwise returns the greatest common divisor of `|m|` and `|n|`.
 */
template <class M, class N>
constexpr inline std::common_type_t<M, N> gcd(M m, N n) noexcept /*strengthened*/
{
    static_assert(::carb::cpp::detail::NotBoolIntegral<M>::value && ::carb::cpp::detail::NotBoolIntegral<N>::value,
                  "Requires non-bool integral");
    using Common = std::common_type_t<M, N>;
    using Unsigned = std::make_unsigned_t<Common>;

    Unsigned am = ::carb::cpp::detail::abs(m);
    Unsigned an = ::carb::cpp::detail::abs(n);

    if (am == 0)
        return Common(an);
    if (an == 0)
        return Common(am);

    const auto trailingZerosM = ::carb::cpp::detail::bitscan_forward(am);
    const auto common2s = carb_min(trailingZerosM, ::carb::cpp::detail::bitscan_forward(an));
    an >>= common2s;
    am >>= trailingZerosM;
    do
    {
        an >>= ::carb::cpp::detail::bitscan_forward(an);
        if (am > an)
        {
            Unsigned temp = am;
            am = an;
            an = temp;
        }
        an -= am;
    } while (an != 0u);

    return Common(am << common2s);
}

} // namespace cpp
} // namespace carb
