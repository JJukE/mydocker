// Copyright (c) 2022-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file ITbbSchedulerState.h
//!
//! @brief Defines @ref omni::kit::exec::core::unstable::ITbbSchedulerState.
#pragma once

#include <omni/graph/exec/unstable/IBase.h>

namespace omni
{
namespace kit
{
namespace exec
{
namespace core
{
namespace unstable
{

struct TbbSchedulerState;

// forward declarations needed by interface declaration
class ITbbSchedulerState;
class ITbbSchedulerState_abi;

//! Returns a global scheduler state based on TBB.
//!
//! This object is a singleton.  Access it with @ref omni::kit::exec::core::unstable::getTbbSchedulerState().
//!
//! Use of this object should be transparent to the user as it is an implementation detail of
//! @ref omni::kit::exec::core::unstable::ParallelSpawner.
//!
//! Temporary interface.  Will be replaced with something more generic.
class ITbbSchedulerState_abi
    : public omni::core::Inherits<graph::exec::unstable::IBase, OMNI_TYPE_ID("omni.kit.exec.core.unstable.ITbbSchedulerState")>
{
protected:
    //! Returns the needed data to access the serial task queue.
    virtual TbbSchedulerState* getState_abi() noexcept = 0;
};

//! Returns the singleton @ref omni::kit::exec::core::unstable::ITbbSchedulerState.
//!
//! May return @c nullptr if the *omni.kit.exec.core* extension has not been loaded.
//!
//! The returned pointer does not have @ref omni::core::IObject::acquire() called on it.
inline ITbbSchedulerState* getTbbSchedulerState() noexcept;

} // namespace unstable
} // namespace core
} // namespace exec
} // namespace kit
} // namespace omni

// generated API declaration
#define OMNI_BIND_INCLUDE_INTERFACE_DECL
#include <omni/kit/exec/core/unstable/ITbbSchedulerState.gen.h>

//! @copydoc omni::kit::exec::core::unstable::ITbbSchedulerState_abi
class omni::kit::exec::core::unstable::ITbbSchedulerState
    : public omni::core::Generated<omni::kit::exec::core::unstable::ITbbSchedulerState_abi>
{
};

inline omni::kit::exec::core::unstable::ITbbSchedulerState* omni::kit::exec::core::unstable::getTbbSchedulerState() noexcept
{
    // createType() always calls acquire() and returns an ObjectPtr to make sure release() is called. we don't want to
    // hold a ref here to avoid static destruction issues. here we allow the returned ObjectPtr to destruct (after
    // calling get()) to release our ref. we know the DLL in which the singleton was created is maintaining a ref and
    // will keep the singleton alive for the lifetime of the DLL.
    static auto sSingleton = omni::core::createType<ITbbSchedulerState>().get();
    return sSingleton;
}

// generated API implementation
#define OMNI_BIND_INCLUDE_INTERFACE_IMPL
#include <omni/kit/exec/core/unstable/ITbbSchedulerState.gen.h>
