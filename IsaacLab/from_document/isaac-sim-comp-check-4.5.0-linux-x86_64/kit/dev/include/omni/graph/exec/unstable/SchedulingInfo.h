// Copyright (c) 2022-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file SchedulingInfo.h
//!
//! @brief Defines omni::graph::exec::unstable::SchedulingInfo.
#pragma once

namespace omni
{
namespace graph
{
namespace exec
{
namespace unstable
{

//! Constraints to be fulfilled by the scheduler when dispatching a task.
//!
//! See @rstref{Execution Concepts <ef_execution_concepts>} for an in-depth guide on how this object is used during
//! execution.
enum class SchedulingInfo
{
    eSerial, //!< Execution of a task should be serialized globally. No other serial task should be running.
    eParallel, //!< Execution of a task can be done safely in parallel. Parallel tasks can run together with serial.
    eIsolate, //!< Execution of a task has to be done in isolation. No other tasks can run concurrently.
    eSchedulerBypass //!< Execution of a task should bypass the scheduler. Either to avoid overhead for lightweight
                     //!< tasks or to serialize within a thread generating the work.
};

} // namespace unstable
} // namespace exec
} // namespace graph
} // namespace omni
