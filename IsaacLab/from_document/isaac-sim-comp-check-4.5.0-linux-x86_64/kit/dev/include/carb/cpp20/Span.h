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

#include "TypeTraits.h"
#include "../cpp17/StdDef.h"
#include "../cpp/Span.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp20
{

using ::carb::cpp::data; // From ImplData.h
using ::carb::cpp::dynamic_extent;
using ::carb::cpp::span;

} // namespace cpp20
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    static_assert(std::is_function<decltype(carb::cpp20::data<int, 5>)>::value, "");
    static_assert(carb::cpp20::dynamic_extent == size_t(-1), "");
    static_assert(std::is_class<carb::cpp20::span<int, 5>>::value, "");
});
