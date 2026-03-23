// Copyright (c) 2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

#pragma once

//! \cond DEV
//! \file
//! \brief Contains the \c ParamPack helper type.

namespace omni
{
namespace detail
{

//! Wrapper type containing a pack of template parameters. This is only useful in metaprogramming.
template <typename... Args>
struct ParamPack
{
};

} // namespace detail
} // namespace omni

//! \endcond
