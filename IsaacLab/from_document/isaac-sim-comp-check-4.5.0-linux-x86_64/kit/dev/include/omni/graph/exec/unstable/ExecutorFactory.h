// Copyright (c) 2022-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file ExecutorFactory.h
//!
//! @brief Declares @ref omni::graph::exec::unstable::ExecutorFactory
#pragma once

#include <omni/graph/exec/unstable/IExecutor.h>

#include <functional>

namespace omni
{
namespace graph
{
namespace exec
{
namespace unstable
{

class ExecutionTask;
class ITopology;

//! Factory owned by a node graph definition used to instantiate an executor to generate work with the graph definition.
//!
//! The given topology is the topology that generated the work causing the executor to be created.  It must not be @c
//! nullptr.
//!
//! The given task describes the path of the node to execute.
//!
//! A valid pointer is always returned.
using ExecutorFactory =
    std::function<omni::core::ObjectPtr<IExecutor>(omni::core::ObjectParam<ITopology>, const ExecutionTask&)>;

} // namespace unstable
} // namespace exec
} // namespace graph
} // namespace omni
