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

#include "../cpp/Bit.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp20
{

using ::carb::cpp::bit_cast;
using ::carb::cpp::bit_ceil;
using ::carb::cpp::bit_floor;
using ::carb::cpp::countl_zero;
using ::carb::cpp::countr_zero;
using ::carb::cpp::endian;
using ::carb::cpp::has_single_bit;
using ::carb::cpp::popcount;

} // namespace cpp20
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    static_assert(std::is_function<decltype(carb::cpp20::bit_cast<int, unsigned>)>::value, "1");
    static_assert(std::is_function<decltype(carb::cpp20::bit_ceil<unsigned>)>::value, "2");
    static_assert(std::is_function<decltype(carb::cpp20::bit_floor<unsigned>)>::value, "3");
    static_assert(std::is_function<decltype(carb::cpp20::countl_zero<unsigned>)>::value, "4");
    static_assert(std::is_function<decltype(carb::cpp20::countr_zero<unsigned>)>::value, "5");
    static_assert(std::is_enum<carb::cpp20::endian>::value, "6");
    static_assert(std::is_function<decltype(carb::cpp20::has_single_bit<unsigned>)>::value, "7");
    static_assert(std::is_function<decltype(carb::cpp20::popcount<unsigned>)>::value, "8");
});
