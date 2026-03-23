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

#include "../cpp/Utility.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp17
{

using ::carb::cpp::in_place;
using ::carb::cpp::in_place_index;
using ::carb::cpp::in_place_index_t;
using ::carb::cpp::in_place_t;
using ::carb::cpp::in_place_type;
using ::carb::cpp::in_place_type_t;

} // namespace cpp17
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    static_assert(sizeof(carb::cpp17::in_place), "");
    static_assert(sizeof(carb::cpp17::in_place_index<0>), "");
    static_assert(sizeof(carb::cpp17::in_place_index_t<0>), "");
    static_assert(sizeof(carb::cpp17::in_place_t), "");
    static_assert(sizeof(carb::cpp17::in_place_type<int>), "");
    static_assert(sizeof(carb::cpp17::in_place_type_t<int>), "");
});
