// Copyright (c) 2023-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief Implementation of a clock based on the CPU time-stamp counter
#pragma once

#include "../clock/TscClock.h"

namespace carb
{
namespace time
{
//! Deprecated name for carb::clock::tsc_clock;
using carb::clock::tsc_clock;
//! \cond DEV
namespace detail
{
using namespace carb::clock::detail;
}
//! \endcond
} // namespace time
} // namespace carb
