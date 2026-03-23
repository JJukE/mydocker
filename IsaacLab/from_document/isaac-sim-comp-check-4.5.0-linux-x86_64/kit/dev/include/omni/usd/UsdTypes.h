// Copyright (c) 2018-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//
#pragma once

#include <omni/kit/KitTypes.h>

#include <functional>

namespace omni
{
namespace usd
{

typedef uint64_t SubscriptionId;

/**
 * Defines the USD state types.
 */
enum class StageState
{
    /// USD is closed/unopened.
    eClosed,

    /// USD is opening.
    eOpening,

    /// USD is opened.
    eOpened,

    /// USD is closing.
    eClosing
};

/**
 * Defines the usd event types.
 */
enum class StageEventType
{
    /// USD file saved.
    eSaved,

    /// Failed to save USD.
    eSaveFailed,

    /// USD stage is opening.
    eOpening,

    /// USD stage is opened successfully.
    eOpened,

    /// USD stage failed to open.
    eOpenFailed,

    /// USD stage is about to close. This is a good opportunity to shutdown anything depends on USD stage.
    eClosing,

    /// USD stage is fully closed.
    eClosed,

    /// USD Prim selection has changed.
    eSelectionChanged,

    /// Current batch of async asset loading has been completed.
    eAssetsLoaded,

    /// Current batch of async asset loading has been aborted.
    eAssetsLoadAborted,

    /// Started or stopped tracking (hovering) on a gizmo
    eGizmoTrackingChanged,

    /// MDL parameter is loaded for a MDL UsdShadeShader.
    eMdlParamLoaded,

    /// Stage settings have loaded
    eSettingsLoaded,

    /// Stage settings are being saved
    eSettingsSaving,

    /// OmniGraph play has started
    eOmniGraphStartPlay,

    /// OmniGraph play has stopped
    eOmniGraphStopPlay,

    /// Simulation play has started
    eSimulationStartPlay,

    /// Simulation play has stopped
    eSimulationStopPlay,

    /// Animation playback has started
    eAnimationStartPlay,

    /// Animation playback has stopped
    eAnimationStopPlay,

    /// Dirty state of USD stage has changed. Dirty state means if it has unsaved changes or not.
    eDirtyStateChanged,

    /// A new batch of async asset loading has started.
    eAssetsLoading,

    /// Number of active lights in the scene has changed. This signal should be triggered
    /// every time a scene has been loaded or number of lights has been changed. A few
    /// features, for instance view lighting mode, need to detect when a number of active
    /// lights becomes zero / become non-zero.
    eActiveLightsCountChanged,

    /// USD stage hierarchy has changed.
    eHierarchyChanged,

    /// Fabric Scene Delegate sends this when starting to stream rprims.
    eHydraGeoStreamingStarted,

    /// Fabric Scene Delegate sends this when stopping to stream rprims.
    eHydraGeoStreamingStopped,

    /// Fabric Scene Delegate sends this when geometry streaming stops loading more geometry because of insufficient device memory
    eHydraGeoStreamingStoppedNotEnoughMem,

    /// Fabric Scene Delegate sends this when stopping to stream rprims because of the limit set by the user.
    eHydraGeoStreamingStoppedAtLimit,

    /// Starting to save stage.
    eSaving,
};

enum class StageRenderingEventType
{
    /// New frame available for Viewport, params are ViewportHandle, FrameNo, RenderResults
    eNewFrame,

    /// Frames complete for a single hydra engine render() invocation.
    /// Payload is {
    ///     render_results: [ { viewport_handle: ViewportHandle, product: HydraRenderProduct*, subframe_count: int32_t } ],
    ///     average_frame_time_ns: float,
    ///     swh_frame_number: uint64_t,
    /// }
    eHydraEngineFramesComplete,

    /// Frames added to the GPU queue for a single hydra engine.
    /// This event signifies that the frame is scheduled for GPU rendering and has not been rendered yet.
    /// The payload structure is the same as eHydraEngineFramesComplete.
    eHydraEngineFramesAdded,

    /// Recording of the frame is done, but has not been submitted for execution yet.
    /// Payload is {
    ///     results: HydraRenderResults*
    ///     product_path_handle: usd::PathH
    ///     rendergraph: gpu::RenderGraph::RenderGraph*
    /// }
    /// This event is never push()ed; it is only ever dispatch()ed. This means that push subscriptions will never be
    /// triggered. This also means that it can execute out of order with respect to other events in the same stream; A
    /// OnEventFunction for this function can not rely on the execution order of other events in the same stream.
    /// Execution of this event will block the rendering thread - it should be used only to add operations to the end of
    /// the RenderGraph, after rendering has added all operations, but before it is submitted.
    eRendererRecordingComplete,
};

/**
* @brief Stage result callback.
*
* @see UsdContext::newStage, UsdContext::openStage, UsdContext::closeStage, UsdContext::attachStage
*/
using OnStageResultFn = std::function<void(bool result, const char* err)>;

/**
* @brief Stage save callback.
*
* @see UsdContext::saveStage, UsdContext::saveAsStage, UsdContext::saveLayers
*/
using OnLayersSavedResultFn =
    std::function<void(bool result, const char* err, const std::vector<std::string>& savedLayers)>;


/**
 * Internal.
 */
using OnPickingCompleteFn = std::function<void(const char* path, const carb::Double3* worldPos)>;

}
}
