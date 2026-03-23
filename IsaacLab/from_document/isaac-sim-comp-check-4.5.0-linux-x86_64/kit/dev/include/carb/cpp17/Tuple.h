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

#include "../cpp/Tuple.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp17
{

using ::carb::cpp::apply;

}
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({ carb::cpp17::apply([](int) {}, std::make_tuple(5)); });
