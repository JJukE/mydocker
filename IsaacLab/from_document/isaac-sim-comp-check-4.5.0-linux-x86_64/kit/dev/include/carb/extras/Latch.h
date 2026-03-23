// Copyright (c) 2019-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

#pragma once
#include "../cpp/Latch.h"

namespace carb
{
namespace extras
{

class CARB_DEPRECATED("Deprecated: carb::extras::latch has moved to carb::cpp::latch") latch : public carb::cpp::latch
{
public:
    constexpr explicit latch(ptrdiff_t expected) : carb::cpp::latch(expected)
    {
    }
};

} // namespace extras
} // namespace carb
