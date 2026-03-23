// Copyright (c) 2022-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file NodePartition.h
//!
//! @brief Defines omni::graph::exec::unstable::NodePartition.
#pragma once

#include <omni/graph/exec/unstable/Span.h>

namespace omni
{
namespace graph
{
namespace exec
{
namespace unstable
{

// forward declarations
class INode;

//! Type definition used to pass node partitions in the ABI.
using NodePartition = omni::graph::exec::unstable::Span<INode* const>;

} // namespace unstable
} // namespace exec
} // namespace graph
} // namespace omni
