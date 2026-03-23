// Copyright (c) 2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! \file
//! \brief Redirection for backwards compatibility
#pragma once

#include "../cpp17/TypeTraits.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
//! (Deprecated) Namespace for C++20 features using C++14 semantics. Use \ref carb::cpp instead.
namespace cpp20
{

using ::carb::cpp::is_nothrow_convertible;
using ::carb::cpp::remove_cvref;
using ::carb::cpp::remove_cvref_t;
using ::carb::cpp::type_identity;
using ::carb::cpp::type_identity_t;

} // namespace cpp20
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    static_assert(carb::cpp20::is_nothrow_convertible<bool, int>::value, "1");
    static_assert(std::is_same<int, carb::cpp20::remove_cvref<const int&>::type>::value, "2");
    static_assert(std::is_same<int, carb::cpp20::remove_cvref_t<const int&>>::value, "3");
    static_assert(std::is_same<int, carb::cpp20::type_identity<int>::type>::value, "4");
    static_assert(std::is_same<int, carb::cpp20::type_identity_t<int>>::value, "4");
});
