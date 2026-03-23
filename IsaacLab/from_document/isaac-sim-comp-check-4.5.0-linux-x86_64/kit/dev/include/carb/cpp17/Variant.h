// Copyright (c) 2023-2024, NVIDIA CORPORATION. All rights reserved.
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

#include "../cpp/Variant.h"

#include <cstddef>

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp17
{

using ::carb::cpp::bad_variant_access;
using ::carb::cpp::get;
using ::carb::cpp::get_if;
using ::carb::cpp::holds_alternative;
using ::carb::cpp::monostate;
using ::carb::cpp::variant;
using ::carb::cpp::variant_alternative;
using ::carb::cpp::variant_alternative_t;
using ::carb::cpp::variant_npos;
using ::carb::cpp::visit;

} // namespace cpp17
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    using my_variant = carb::cpp17::variant<int, float>;
    static_assert(std::is_same<int, carb::cpp17::variant_alternative<0, my_variant>::type>::value, "1");
    static_assert(std::is_same<float, carb::cpp17::variant_alternative_t<1, my_variant>>::value, "2");
    static_assert(carb::cpp17::variant_npos == size_t(-1), "3");
    static_assert(sizeof(carb::cpp17::bad_variant_access), "4");
    static_assert(!std::is_same<void, decltype(carb::cpp17::get<int>(std::declval<my_variant>()))>::value, "5");
    static_assert(!std::is_same<void, decltype(carb::cpp17::get<0>(std::declval<my_variant>()))>::value, "6");
    static_assert(!std::is_same<void, decltype(carb::cpp17::get_if<int>(std::declval<my_variant*>()))>::value, "7");
    static_assert(!std::is_same<void, decltype(carb::cpp17::get_if<0>(std::declval<my_variant*>()))>::value, "8");
    static_assert(
        std::is_same<bool, decltype(carb::cpp17::holds_alternative<int>(std::declval<my_variant>()))>::value, "9");
    static_assert(std::is_class<carb::cpp17::monostate>::value, "10");
    struct my_visitor
    {
        void operator()(int)
        {
        }
        void operator()(float)
        {
        }
    };
    static_assert(std::is_function<decltype(carb::cpp17::visit<my_visitor, my_variant>)>::value, "11");
});
