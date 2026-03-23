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

#include "../cpp/Functional.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp17
{

using ::carb::cpp::invoke;
using ::carb::cpp::invoke_r;

} // namespace cpp17
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    extern int foo();
    carb::cpp17::invoke(foo);
    carb::cpp17::invoke_r<int>(foo);
});
