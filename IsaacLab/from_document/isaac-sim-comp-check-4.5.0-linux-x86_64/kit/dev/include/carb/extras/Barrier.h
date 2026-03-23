// Copyright (c) 2019-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

#pragma once
#include "../cpp/Barrier.h"

namespace carb
{
namespace extras
{

template <class CompletionFunction = carb::cpp::detail::NullFunction>
class CARB_DEPRECATED("Deprecated: carb::extras::binary_semaphore has moved to carb::cpp::binary_semaphore") barrier
    : public carb::cpp::barrier<CompletionFunction>
{
public:
    constexpr explicit barrier(ptrdiff_t desired, CompletionFunction f = CompletionFunction())
        : carb::cpp::barrier<CompletionFunction>(desired, std::move(f))
    {
    }
};

} // namespace extras
} // namespace carb
