// Copyright (c) 2019-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief omni.ext interface definition file
#pragma once

#include "../../carb/Interface.h"

namespace omni
{

//! Namespace for Omniverse Extension system
namespace ext
{

/**
 * Extension plugin interface.
 *
 * Interface for writing simple C++ plugins used by extension system.
 * When extension loads a plugin with this interface it gets automatically acquired and \ref IExt::onStartup() is
 * called. \ref IExt::onShutdown() is called when extension gets shutdown.
 *
 * @see ExtensionManager
 */
class IExt
{
public:
    CARB_PLUGIN_INTERFACE("omni::ext::IExt", 0, 1);

    /**
     * Called by the Extension Manager when the extension is being started.
     *
     * @param extId Unique extension id is passed in. It can be used to query more extension info from extension
     * manager.
     */
    virtual void onStartup(const char* extId) = 0;

    /**
     * Called by the Extension Manager when the extension is shutdown.
     */
    virtual void onShutdown() = 0;
};

} // namespace ext
} // namespace omni
