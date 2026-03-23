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

#include "../cpp/TypeTraits.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
//! (Deprecated) Namespace for C++17 features using C++14 semantics. Use \ref carb::cpp instead.
namespace cpp17
{

using ::carb::cpp::bool_constant;
using ::carb::cpp::conjunction;
using ::carb::cpp::disjunction;
using ::carb::cpp::invoke_result;
using ::carb::cpp::invoke_result_t;
using ::carb::cpp::is_convertible;
using ::carb::cpp::is_invocable;
using ::carb::cpp::is_invocable_r;
using ::carb::cpp::is_nothrow_invocable;
using ::carb::cpp::is_nothrow_invocable_r;
using ::carb::cpp::is_nothrow_swappable;
using ::carb::cpp::is_nothrow_swappable_with;
using ::carb::cpp::is_swappable;
using ::carb::cpp::is_swappable_with;
using ::carb::cpp::is_void;
using ::carb::cpp::negation;
using ::carb::cpp::void_t;

} // namespace cpp17
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    using True = carb::cpp17::bool_constant<true>;
    using False = carb::cpp17::bool_constant<false>;
    static_assert(carb::cpp17::conjunction<True, True>::value, "1");
    static_assert(carb::cpp17::disjunction<True, False>::value, "2");
    auto foo = []() noexcept { return true; };
    static_assert(std::is_same<carb::cpp17::invoke_result<decltype(foo)>::type, bool>::value, "3");
    static_assert(std::is_same<carb::cpp17::invoke_result_t<decltype(foo)>, bool>::value, "4");
    static_assert(carb::cpp17::is_convertible<bool, int>::value, "5");
    static_assert(carb::cpp17::is_invocable<decltype(foo)>::value, "6");
    static_assert(carb::cpp17::is_invocable_r<bool, decltype(foo)>::value, "7");
    static_assert(carb::cpp17::is_nothrow_invocable<decltype(foo)>::value, "9");
    static_assert(carb::cpp17::is_nothrow_invocable_r<bool, decltype(foo)>::value, "10");
    static_assert(std::is_class<carb::cpp17::is_nothrow_swappable<int>>::value, "11");
    static_assert(std::is_class<carb::cpp17::is_nothrow_swappable_with<int, int>>::value, "12");
    static_assert(std::is_class<carb::cpp17::is_swappable<int>>::value, "13");
    static_assert(std::is_class<carb::cpp17::is_swappable_with<int, int>>::value, "14");
    static_assert(carb::cpp17::is_void<void>::value, "15");
    static_assert(carb::cpp17::negation<False>::value, "16");
});
