// Copyright (c) 2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief Implementation details
#pragma once

#include <type_traits>

namespace carb
{
namespace cpp
{

//! \cond DEV
namespace detail
{

struct NontrivialDummyType
{
    constexpr NontrivialDummyType() noexcept
    {
    }
};
static_assert(!std::is_trivially_default_constructible<NontrivialDummyType>::value, "Invalid assumption");

} // namespace detail
//! \endcond

} // namespace cpp
} // namespace carb
