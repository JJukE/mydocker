// Copyright (c) 2022-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//
#pragma once

#include <carb/logging/Log.h>
#include <omni/kit/renderer/IRenderer.h>
#include <rtx/resourcemanager/ResourceManager.h>

namespace
{
using namespace omni::kit::renderer;
using namespace rtx::resourcemanager;

static void* acquireGpuPointerReference(IRenderer::ResourceManagerState state, RpResource& rpRsrc, const carb::graphics::TextureDesc** texDesc)
{
    if (!state.manager || !state.manager)
    {
        CARB_LOG_ERROR("ResourceManagerState doesn't have a ResourceManager or its Context");
        return nullptr;
    }
    if (texDesc)
    {
        *texDesc = state.manager->getTextureDesc(*state.context, &rpRsrc);
        if (!*texDesc)
        {
            CARB_LOG_ERROR("ResourceManager returned a null TextureDesc for the RpResource");
            return nullptr;
        }
    }
    auto texH = state.manager->getTextureHandle(*state.context, &rpRsrc);
    if (!texH.ptr)
    {
        CARB_LOG_ERROR("ResourceManager could not retrieve a TextureHandle for the RpResource");
        return nullptr;
    }

    state.manager->acquireResource(rpRsrc);
    return texH.ptr;
}

static bool releaseGpuPointerReference(IRenderer::ResourceManagerState state, RpResource& rpRsrc)
{
    if (state.manager)
    {
        state.manager->releaseResource(rpRsrc);
        return true;
    }
    return false;
}

}
