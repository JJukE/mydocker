// Copyright (c) 2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief Implementation of \ref omni::span
#pragma once

#include "../carb/cpp/Span.h"

namespace omni
{

//! @copydoc carb::cpp::dynamic_extent
constexpr auto dynamic_extent = carb::cpp::dynamic_extent;

//! @copydoc carb::cpp::span
template <class T, size_t Extent = dynamic_extent>
using span = carb::cpp::span<T, Extent>;

} // namespace omni
