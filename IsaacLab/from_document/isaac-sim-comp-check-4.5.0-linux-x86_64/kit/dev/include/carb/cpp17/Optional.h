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

#include "../cpp/Optional.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp17
{

using ::carb::cpp::bad_optional_access;
using ::carb::cpp::make_optional;
using ::carb::cpp::nullopt;
using ::carb::cpp::nullopt_t;
using ::carb::cpp::optional;

} // namespace cpp17
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    carb::cpp17::optional<int> opt{ carb::cpp17::nullopt };
    carb::cpp17::optional<int> opt2 = carb::cpp17::make_optional<int>(5);
    CARB_UNUSED(opt, opt2);
    static_assert(sizeof(carb::cpp17::nullopt_t), "");
    static_assert(sizeof(carb::cpp17::bad_optional_access), "");
});
