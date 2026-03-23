// Copyright (c) 2022-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file Status.h
//!
//! @brief Defines omni::graph::exec::unstable::Status.
#pragma once

#include <omni/graph/exec/unstable/EnumBitops.h>

#include <cstdint>

namespace omni
{
namespace graph
{
namespace exec
{
namespace unstable
{

//! Return status of all executions.
//!
//! Status objects can be or'd together, allowing a single Status to represent the result of many operations.
//!
//! See @rstref{Error Handling<ef_error_handling>} to understand how this object is used during error handling.
enum class Status : uint32_t
{
    eUnknown = 0, //!< Status is undetermined yet
    eFailure = 1 << 0, //!< Execution failed
    eSkip = 1 << 1, //!< Execution was skipped
    eDeferred = 1 << 2, //!< Execution was deferred to start and/or complete outside of current execution frame
    eSuccess = 1 << 3, //!< Execution was successful
    eInProgress = 1 << 4 //!< Execution is in progress
};

//! Enable bitwise operations on return state.
template <>
struct EnumBitops<Status> : EnumBitops<>::allow_bitops
{
};

} // namespace unstable
} // namespace exec
} // namespace graph
} // namespace omni
