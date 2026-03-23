// Copyright (c) 2020-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//
#pragma once

#ifndef USD_CONTEXT_INCLUDES
#    error "Please include UsdContextIncludes.h before including this header or in pre-compiled header."
#endif


#include <carb/events/EventsUtils.h>
#include <omni/usd/UsdContextOverrides.h>

#include "Api.h"
#include <omni/usd/UsdTypes.h>
#include <omni/usd/ViewportTypes.h>
#include <omni/usd/IUsdMutex.h>
#include <omni/usd-abi/IPathAbi.h>
#include <rtx/hydra/FrameIdentifier.h>
#include <rtx/hydra/EngineRenderFlags.h>

#include <omni/fabric/core/FabricRenderingView.h>
#include <omni/fabric/SimStageWithHistory.h>

#include <pxr/base/tf/warning.h>

#include <memory>


PXR_NAMESPACE_OPEN_SCOPE
class UsdGeomBBoxCache;
PXR_NAMESPACE_CLOSE_SCOPE

namespace carb
{
namespace renderer
{
struct Renderer;
struct Context;
enum class FrameDataType;
}
namespace scenerenderer
{
struct Scene;
typedef struct Scene* SceneId;
enum class CameraFit;
struct SceneRenderer;
struct Context;
}
namespace settings
{
struct ISettings;
}
}

namespace rtx
{
namespace resourcemanager
{
// TODO: This must stay in sync with the actual typedef of SyncScopeId
typedef uint32_t SyncScopeId;
}
}

namespace omni
{

template <class T>
class vector;

namespace timeline
{
class Timeline;
using TimelinePtr = std::shared_ptr<Timeline>;
};

namespace usd
{
typedef std::unordered_set<PXR_NS::SdfPath, PXR_NS::SdfPath::Hash> SdfPathUSet;

class Selection;
struct Layers;
class UsdManager;

namespace hydra
{
class IHydraEngine;
struct IHydraSceneDelegate;
struct CameraSettings;
struct ViewportHydraRenderResults;
struct OpaqueSharedHydraEngineContext;
typedef struct OpaqueSharedHydraEngineContext* OpaqueSharedHydraEngineContextPtr;
struct HydraEngineContextConfig;
struct EngineCreationConfig;
enum class EngineCreationFlags : uint32_t;

OMNI_DECLARE_INTERFACE(IViewOverrideBase);
}

namespace audio
{
class AudioManager;
}

struct HydraEngineDesc
{
    uint32_t tickRate;
    const char* engineName;
    const char* threadName;
    uint32_t currentHydraEngineIdx;
    uint32_t deviceMask;
    hydra::EngineCreationFlags flags;
};

/**
 * @brief Specifies the initial set of prims to load when opening a UsdStage.
 * @see UsdContext::openStage
 */
enum class UsdContextInitialLoadSet
{
    /// Load all payloads.
    eLoadAll,

    /// Unload all payloads.
    eLoadNone,
};

class UsdContext
{
public:
    // TODO: getContext needs to me removed, but it's called 160 times across 33 files.
    // Keep stub that invokes UsdManager::getContext for now to reduce MR
    /**
     * Gets a UsdContext.
     *
     * @param name of the UsdContext to get. The default context is named with empty string "".
     */
    OMNI_USD_API static UsdContext* getContext(const std::string& name = "");

    OMNI_USD_API void runRenderRunLoop(const hydra::FrameIdentifier& frameIdentifier,
                                       const hydra::EngineRenderFlags& renderFlags = hydra::EngineRenderFlags::eEmpty);

    OMNI_USD_API void checkHydraRenderFinished();

    OMNI_USD_API void releaseRenderResult(size_t frameNumber, omni::usd::PathH renderProductPath);

    /**
     * Adds a hydra engine with associated renderer to context. The syncScopes determines
     * which rendering thread the hydra engine should run on. It should correspond
     * to the syncScope that was passed into the IHydraEngine during creation
     *
     * @return true if hydra engines are created, false otherwise
     */
    OMNI_USD_API bool addHydraEngine(const char* name,
                                     omni::usd::hydra::IHydraEngine* hydraEngine,
                                     omni::usd::hydra::OpaqueSharedHydraEngineContextPtr ctx,
                                     const omni::usd::hydra::HydraEngineContextConfig& config,
                                     bool defer);

    /**
     * Destroys all hydra engines
     */
    OMNI_USD_API void removeAllHydraEngines();

    /**
     * Controls two things:
     *  1. Specifies the default hydra engine to use with hydraEngine = null in CreateViewport
     *  2. [TODO] Controls the hydra engine used for selection and picking, this feels wrong
     *     and instead the picking api should include a hydra engine, and if non specified
     *     use the default. This API should instead be setDefaultHydraEngine
     *
     */
    OMNI_USD_API void setActiveHydraEngine(const char* name);

    /**
     *  Return the name of active hydra engine
     */
    OMNI_USD_API const char* getActiveHydraEngineName(void);

    /**
     * Adds a hydra scene delegate with associated input data to the context
     *
     * @param name should be unique, and will be used for identifying the scene delegate in
     * future calls. If the name is not unique, the add call will fail.
     * @param hydraEngine a string representing which hydra engine this delegate should be
     * added for. For instance, "rtx"
     * @param sceneDelegate the new scene delegate interface to add
     * @param delegateCreateParam a parameter to pass to the sceneDelegate's Create function
     *
     * @return true if successfully added, false otherwise
     */
    OMNI_USD_API bool addHydraSceneDelegate(const char* name,
                                            const char* hydraEngine,
                                            const omni::usd::hydra::IHydraSceneDelegate& sceneDelegate,
                                            const char* delegateCreateParam);

    /**
     * Removes a hydra scene delegate
     *
     * @param name should match the name passed to addHydraSceneDelegate
     *
     * @return true if successfully removed, false otherwise
     */
    OMNI_USD_API bool removeHydraSceneDelegate(const char* name);

    /**
     * Sets a global tansform for all content in a named hydra scene delegate
     *
     * @param name should match the name passed to addHydraSceneDelegate
     * @param xform a transform to apply to everything in the delegate
     *
     * @return true if successfully removed, false otherwise
     */
    OMNI_USD_API bool setHydraSceneDelegateRootTransform(const char* name, const PXR_NS::GfMatrix4d& xform);

    /**
     * Gets a named attribute of a prim from a named hydra scene delegate
     *
     * @param name should match the name passed to addHydraSceneDelegate
     * @param primPath is the path to the prim containing the attribute
     * @param attributeName name of the attribute
     * @param attributeValue a VtValue to populate with the value of the attribute
     *
     * @return true if successfully accessed the scene delegate, false otherwise
     */
    OMNI_USD_API bool getHydraSceneDelegateAttribute(const char* name,
                                                     const PXR_NS::SdfPath& primPath,
                                                     const PXR_NS::TfToken& attribueName,
                                                     PXR_NS::VtValue& attributeValue);

    /**
     * Computs a world bounding box for all content in a named hydra scene delegate
     *
     * @param name should match the name passed to addHydraSceneDelegate
     * @param bbox is an array of 12 elements (4 x vector3, min/max bounds, center, size)
     */
    OMNI_USD_API void computeHydraSceneDelegateWorldBoundingBox(const char* name, double* bbox);

    /**
     * Returns true if the Fabric scene delegate is in use. This changes
     * behavior in Fabric population, StageReaderWriter copies to the ring
     * buffer, and Fabric USD notice handling
     *
     * This is the value of the "/app/useFabricSceneDelegate" carb setting
     * when the stage is first loaded, and remains constant until the
     * next stage load to avoid bad behavior with already allocated scene
     * delegates and hydra engines.
     */
    OMNI_USD_API bool useFabricSceneDelegate() const;

    /**
     * Checks if any Hydra Renderer supports MDL
     *
     * @return true if a Hydra renderer supports MDL, false otherwise.
     */
    OMNI_USD_API bool hasMDLHydraRenderer() const;

    /**
     * Returns the RTX SceneRenderer if a "rtx" hydra engine
     * exists, otherwise nullptr. Note, tighly coupled with getScene()
     *
     * [TODO] Remove this API
     * Used for PrimitiveListDrawing
     * Trigger "compile shaders" for F9
     * MaterialWatcher for getNeurayDbScopeName()
     * Kit.Legacy Editor python bindings for get_current_renderer_status
     *
     */
    OMNI_USD_API carb::scenerenderer::SceneRenderer* getSceneRenderer() const;

    /**
     * Returns the RTX SceneRenderer Context if a "rtx" hydra engine
     * exists, otherwise nullptr
     *
     */
    OMNI_USD_API carb::scenerenderer::Context* getSceneRendererContext() const;

    /**
     * Update function to be called once every frame.
     *
     * @param elapsedTime Elapsed time in seconds since last update call.
     * @return true if omniverse has new update during this frame.
     */
    OMNI_USD_API bool update(float elapsedTime);

    /**
     * Creates a new USD stage. This is an asynchronous call.
     *
     * @param fn the callback function when stage is created or fails to create.
     */
    OMNI_USD_API bool newStage(const OnStageResultFn& resultFn);

    /**
     * Synchronous version of @see newStage(const OnStageResultFn& resultFn);
     */
    OMNI_USD_API bool newStage();

    /**
     * Attaches an opened USD stage. This is an asynchronous call.
     *
     * @param stageId The stage id saved into UsdUtilsStageCache.
     * @param fn The callback function when stage is attached successfully or fails to attach.
     */
    OMNI_USD_API bool attachStage(long int stageId, const OnStageResultFn& resultFn);

    /**
     * Opens a existing USD stage. This is an asynchronous call.
     *
     * @param url The file path. For Omniverse file, you must connect to Omniverse first and pass the url with prefix
     * "omniverse:".
     * @param fn The callback function when stage is opened or fails to open.
     * @param loadSet Specifies the initial set of prims to load when opening a UsdStage.
     */
    OMNI_USD_API bool openStage(const char* fileUrl,
                                const OnStageResultFn& resultFn,
                                UsdContextInitialLoadSet loadSet = UsdContextInitialLoadSet::eLoadAll,
                                omni::fabric::SampleIndex initial = omni::fabric::kEmptySampleIndex);

    /**
     * Synchronous version of @see openStage(const char* fileUrl, const OnStageResultFn& resultFn);
     */
    OMNI_USD_API bool openStage(const char* fileUrl,
                                UsdContextInitialLoadSet loadSet = UsdContextInitialLoadSet::eLoadAll,
                                omni::fabric::SampleIndex initial = omni::fabric::kEmptySampleIndex);

    /**
     * Reopens current USD stage. This is an asynchronous call.
     *
     * @param fn The callback function when stage is reopened or fails to reopen.
     */
    OMNI_USD_API bool reopenStage(const OnStageResultFn& resultFn,
                                  UsdContextInitialLoadSet loadSet = UsdContextInitialLoadSet::eLoadAll,
                                  omni::fabric::SampleIndex initial = omni::fabric::kEmptySampleIndex);

    /**
     * Synchronous version of @see reopenStage(const OnStageResultFn& resultFn);
     */
    OMNI_USD_API bool reopenStage(UsdContextInitialLoadSet loadSet = UsdContextInitialLoadSet::eLoadAll,
                                  omni::fabric::SampleIndex initial = omni::fabric::kEmptySampleIndex);

    /**
     * Close current USD stage. This is an asynchronous call.
     *
     * @param fn The callback function when stage is closed for fails to close.
     */
    OMNI_USD_API bool closeStage(const OnStageResultFn& resultFn);

    /**
     * Synchronous version of @see closeStage(const OnStageResultFn& resultFn);
     */
    OMNI_USD_API bool closeStage();

    /**
     * Saves specific layer only. This is an asynchronous call.
     *
     * @param layerIdentifier The layer to save.
     * @param fn The callback function when stage is saved or fails to save.
     * @return true if it's saved successfully.
     */
    OMNI_USD_API bool saveLayer(const std::string& layerIdentifier, const OnLayersSavedResultFn& resultFn);

    /**
     * Synchronous version of @see saveLayer(const std::string& layerIdentifier, const OnStageResultFn& resultFn);
     */
    OMNI_USD_API bool saveLayer(const std::string& layerIdentifier);

    /**
     * Saves specific layers only. This is an asynchronous call.
     *
     * @param newRootLayerPath If it's not empty, it means to do save root layer
     * to new place. This will trigger stage reload to open new root layer.
     * @param layerIdentifiers The layers to save. It will save all dirty changes of them.
     * @param fn The callback function when stage is saved or fails to save.
     * @return true if it's saved successfully.
     */
    OMNI_USD_API bool saveLayers(const std::string& newRootLayerPath,
                                 const std::vector<std::string>& layerIdentifiers,
                                 const OnLayersSavedResultFn& resultFn);

    /**
     * Synchronous version of @see saveLayers
     */
    OMNI_USD_API bool saveLayers(const std::string& newRootLayerPath, const std::vector<std::string>& layerIdentifiers);

    /**
     * Saves current USD stage. This is an asynchronous call.
     *
     * @param fn The callback function when stage is saved or fails to save.
     */
    OMNI_USD_API bool saveStage(const OnLayersSavedResultFn& resultFn);

    /**
     * Synchronous version of @see saveStage(const OnStageResultFn& resultFn);
     */
    OMNI_USD_API bool saveStage();

    /**
     * Saves current USD stage to a different location. This is an asynchronous call.
     *
     * @param url The new location to save the USD stage.
     * @param fn The callback function when stage is saved or fails to save.
     */
    OMNI_USD_API bool saveAsStage(const char* url, const OnLayersSavedResultFn& resultFn);

    /**
     * Synchronous version of @see saveAsStage(const char* url, const OnStageResultFn& resultFn);
     */
    OMNI_USD_API bool saveAsStage(const char* url);

    /**
     * Exports current USD stage to a different location. This is an asynchronous call.
     * It will composite current stage into a single flattened layer.
     *
     * @param url The new location to save the USD stage.
     * @param fn The callback function when stage is saved or fails to save.
     */
    OMNI_USD_API bool exportAsStage(const char* url, const OnStageResultFn& resultFn) const;

    /**
     * Synchronous version of @see exportAsStage(const char* url, const OnStageResultFn& resultFn) const;
     */
    OMNI_USD_API bool exportAsStage(const char* url) const;

    /**
     * Checks if currently opened stage is created by calling @ref newStage.
     *
     * @return true if current stage is a new stage.
     */
    OMNI_USD_API bool isNewStage() const;

    /**
     * Checks if it's safe to close stage at calling time.
     *
     * @return true if it is safe to close the stage (when current stage is fully opened).
     *     false if it's unsafe to close current stage (when current stage is still being opened or closed).
     */
    OMNI_USD_API bool canCloseStage() const;

    /**
     * Checks if a USD stage is opened and in a savable stage (not opening/closing in progress).
     *
     * @return true if the stage is opened and savable, false is no stage is opened or opened stage is not savable.
     */
    OMNI_USD_API bool canSaveStage() const;

    /**
     * Checks if it's safe to open a different stage at calling time.
     *
     * @return true if it is safe to open a different stage (when current stage is fully opened or closed).
     *     false if it's unsafe to open different stage (when current stage is still being opened or closed).
     */
    OMNI_USD_API bool canOpenStage() const;

    /**
     * Checks if there is enough permissions to save the stage at calling time.
     *
     * @return true if it is possible to save the stage.
     *     false if it's not possible to save current stage.
     */
    OMNI_USD_API bool isWritable() const;

    /**
     * Gets the state of current stage.
     *
     * @return state of current stage.
     */
    OMNI_USD_API omni::usd::StageState getStageState() const;

    /**
     * Gets UsdStage.
     *
     * @return UsdStageWeakPtr of current stage.
     */
    OMNI_USD_API PXR_NS::UsdStageWeakPtr getStage() const;

    /**
     * Gets UsdStage id.
     *
     * @return id of current stage.
     */
    OMNI_USD_API long int getStageId() const;

    /**
     * Gets the url of current stage.
     *
     * @return url of current stage.
     */
    OMNI_USD_API const std::string& getStageUrl() const;

    /**
     * Checks if current stage is dirty.
     *
     * @return true if current stage is dirty.
     */
    OMNI_USD_API bool hasPendingEdit() const;

    /**
     * Marks the edits state of current opened stage. It means changes are
     * pending to be saved if you set it to true, or false otherwise. This will
     * disgard the real state of opened stage. For example, if the opened stage
     * has real changes to be saved, hasPendingEdit() will still return false if
     * you set it to false.
     *
     * @param edit true to set pending edits state, false to unset pending edits state
     */
    OMNI_USD_API void setPendingEdit(bool edit);

    /**
     * Gets the carb::events::IEventStreamPtr for StageEvent.
     *
     * @return the carb::events::IEventStream for StageEvent.
     */
    OMNI_USD_API carb::events::IEventStreamPtr getStageEventStream();

    /**
     * Gets the carb::events::IEventStreamPtr for RenderingEvent.
     *
     * @return the carb::events::IEventStream for RenderingEvent.
     */
    OMNI_USD_API carb::events::IEventStreamPtr getRenderingEventStream();

    /**
     * Sends a StageEvent to UsdStageEvent stream.
     * It chooses to send event sync/async based on syncUsdLoads option.
     *
     * @param event The event to be sent.
     * @param blockingAsync If the event is sent as async, true to wait on the event until it has been processed by all
     * subscribers. false to return immediately.
     */
    OMNI_USD_API void sendEvent(carb::events::IEventPtr& event, bool blockingAsync = false);

    /**
     * Gets if the current stage is opened from Omniverse.
     *
     * @return true if the stage is opened from Omniverse.
     */
    OMNI_USD_API bool isOmniStage() const;

    /**
     * Saves render settings to current opened USD stage.
     */
    OMNI_USD_API void saveRenderSettingsToCurrentStage();

    /**
     * Loads render settings from stage.
     *
     */
    OMNI_USD_API void loadRenderSettingsFromStage();
    OMNI_USD_API void loadRenderSettingsFromStage(long int legacyAPIDoNotUSe);


    /**
     * Gets the picked position in world space since last picking request.
     *
     * @param outWorldPos The picked world space position.
     * @return true if the result is valid, false if result is not valid.
     */
    OMNI_USD_API bool getPickedWorldPos(ViewportHandle handle, carb::Double3& outWorldPos);

    /**
     * Gets the picked geometry hit data - position in world space and normal since last picking request.
     *
     * @param outWorldPos The picked world space position.
     * @param outNormal The picked normal.
     * @return true if the result is valid, false if result is not valid.
     */
    OMNI_USD_API bool getPickedGeometryHit(ViewportHandle handle, carb::Double3& outWorldPos, carb::Float3& outNormal);

    /**
     * Gets the AABB of a prim.
     *
     * @param prim The prim to get AABB from.
     * @return The AABB of the prim.
     */
    OMNI_USD_API PXR_NS::GfRange3d computePrimWorldBoundingBox(const pxr::UsdPrim& prim);

    /**
     * Gets the AABB of a path as carb::Doubles3 (primarily for Python).
     *
     * @param path The path to get AABB from.
     * @param aabbMin Where to store the min-extents.
     * @param aabbMax Where to store the max-extents.
     */
    OMNI_USD_API void computePathWorldBoundingBox(const std::string& path, carb::Double3& aabbMin, carb::Double3& aabbMax);

    /**
     * Gets the GfMatrix4 of a prim as stored in the current bbox cache.
     *
     * @param prim The prim to get the transform of.
     * @return The world-space GfMatrix4 of the prim.
     */
    OMNI_USD_API PXR_NS::GfMatrix4d computePrimWorldTransform(const pxr::UsdPrim& prim);

    /**
     * Gets the GfMatrix4 of a path as stored in the current bbox cache.
     *
     * @param path The path to a prim to get the transform of.
     * @return The world-space GfMatrix4 of the prim at the path.
     */
    OMNI_USD_API void computePrimWorldTransform(const std::string& path, std::array<double, 16>& flattened);

    /**
     * Gets the stage loading status.
     *
     * @param The message of current stage loading.
     * @param filesLoaded Number of files already loaded.
     * @param totalFiles Total number of files to be loaded.
     */
    OMNI_USD_API void getStageLoadingStatus(std::string& message, int32_t& filesLoaded, int32_t& totalFiles);

    /**
     * Gets the status of all stage streaming systems.
     *
     * @param isBusy whether any of the streaming systems is busy.
     */
    OMNI_USD_API void getStageStreamingStatus(bool& isBusy);

    /**
     * Resets all accumulation state inside renderers so that rendering n frames after this point is deterministic independent of prior frame-history
     */
    OMNI_USD_API void resetRendererAccumulation();

    /**
     * Returns all renderable paths for given prim path and its descendatns.
     * Renderable paths are prims of type instancer or geometry.
     *
     * @param primPath Prim path
     * @param renderablePathSet An unordered output path set.
     */
    OMNI_USD_API void getRenderablePaths(PXR_NS::SdfPath primPath, SdfPathUSet& unOrderedRenderablePathSet);

    /**
     * Returns all renderable paths for given prim path and its descendatns.
     * Renderable paths are prims of type instancer or geometry.
     *
     * @param primPath Prim path
     * @param renderablePathSet An ordered output path set.
     */
    template <typename T>
    OMNI_USD_API void getRenderablePaths(PXR_NS::SdfPath primPath, T& renderablePathSet);

    /**
     * Sets the mouse pickable state of a prim.
     *
     * @param primPath The path of the prim to set pickable state.
     * @param isPickable true if to set the prim to be pickable, false to set to unpickable.
     */
    OMNI_USD_API void setPickable(const char* primPath, bool isPickable);

    /**
     * Returns the SceneId from the RTX SceneRenderer if a "rtx" hydra engine
     * exists. Note, usage coupled with getSceneRenderer()
     *
     * @return carb::scenerenderer::SceneId of current scene.
     */
    OMNI_USD_API carb::scenerenderer::SceneId getRendererScene();

    /**
     * Stops all async hydra rendering. Waits for completion of in-flight tasks
     * then requests RunLoops for rendering to exit
     */
    OMNI_USD_API void stopAsyncRendering();

    /**
     * Currently writing to usd during async rendering can cause a data race.
     * This necessitates the prescence of certain mutex to protect usd data access.
     * This call controls the usage of this mutex and will block until it is safe to begin writes.
     *
     * If a usd write occurs while this is disabled, undefined behavior is likely.
     *
     * Can only be called from the main thread for safety reasons.
     *
     * @note This function is deprecated.
     *
     * @param enabled  if true enables the usage of a mutex to protect usd write operations.
     */
    OMNI_USD_API void enableUsdWrites(bool enabled);

    /**
     * USD is a global shared resource. Invoking enableUsdLocking()
     * lets UsdContext manage the locking of this resource.
     *
     * enableUsdLocking will lock() the resource and manage locking/unlocking
     * going forward
     *
     * disableUsdLokcing() will unlock() the resource and make no subsequent
     * lock/unlock calls
     *
     * WARNING: enableUsdLocking and disableUsdLocking will only work when called from the main thread.
     */
    OMNI_USD_API void enableUsdLocking();
    OMNI_USD_API void disableUsdLocking();
    OMNI_USD_API bool isUsdLockingEnabled();

    /**
     * Returns the status of usd write back.  If not true, writing to usd
     * may cause undefined behavior.
     *
     * @note This function is deprecated.
     *
     * @return true if write back is enabled and false otherwise.
     */
    OMNI_USD_API bool usdWritesEnabled();

    /**
     * Creates a new ViewportHandle for the hydraEngine at the specified tick rate
     * A tickrate of -1 means "render as fast as possible"
     *
     * Will not create a new HydraEngine, a HydraEngine must be available. HydraEngines
     * are added by addHydraEngine()
     *
     * @return ViewportHandle that's used in addRender(), getRenderResults(), destroyViewport()
     */
    OMNI_USD_API ViewportHandle createViewport(const char* hydraEngine,
                                               uint32_t tickrate,
                                               omni::usd::hydra::EngineCreationFlags hydraEngineFlags,
                                               uint32_t deviceMask);

    /**
     * Destroys the ViewportHandle that was created in createViewport
     * then requests RunLoops for rendering to exit
     */
    OMNI_USD_API void destroyViewport(ViewportHandle viewportHandle);

    /**
     * Returns the latest available rendering result. It is assumed main/simulation thread runs
     * at the same rate or faster than hydra engine tick rate. If you query the FPS of the
     * returned value, it will match the hydra engine render rate
     *
     * bBlock == true will trigger hydraRendering() + checkForHydraResults() outside
     * of defined update order in omni::kit::update::UpdateOrderings *if necessary*
     *
     * @return ViewportHydraRenderResults the latest available hydra render result
     */
    OMNI_USD_API const omni::usd::hydra::ViewportHydraRenderResults* getRenderResults(ViewportHandle viewportHandle,
                                                                                      bool bBlock = false);

    /**
     * The ViewportHandle and the RenderProduct USD Prim Path to render
     *
     */
    OMNI_USD_API void addRender(ViewportHandle handle,
                                omni::usd::PathH renderProductPrimPath,
                                const Picking* picking = nullptr);

    /**
     * Gets Selection instance.
     */
    OMNI_USD_API Selection* getSelection();

    /**
     * Gets const Selection instance.
     */
    OMNI_USD_API const Selection* getSelection() const;

    /**
     *  Retrieves the stage audio manager for use in the IStageAudio interface.
     *  @returns The USD context's stage audio manager instance.
     *           This is valid until the USD context is destroyed.
     *  @returns nullptr if the stage audio manager was not loaded or failed
     *           to load.
     */
    OMNI_USD_API audio::AudioManager* getAudioManager() const;

    OMNI_USD_API void enableSaveToRecentFiles();

    OMNI_USD_API void disableSaveToRecentFiles();

    /**
     * By default UsdContext subscribes to IApp for updates. That can be disabled. Temp function until we refactor
     * Editor.cpp
     */
    OMNI_USD_API void toggleAutoUpdate(bool enabled);

    /**
     * Query for instance IDs that are returned to synthetic data
     */
    OMNI_USD_API size_t GetGeometryInstanceId(const PXR_NS::SdfPath& path, uint32_t* instanceList);

    /**
     * Query for geometry IDs that are returned to synthetic data
     */
    OMNI_USD_API size_t GetGeometryId(const PXR_NS::SdfPath& path, uint32_t* geometryList);

    /**
     * Query for mesh instance path that is returned to synthetic data
     */
    OMNI_USD_API const std::string& GetGeometryInstancePath(uint32_t instanceId);

    /**
     * Query for mesh geometry path that is returned to synthetic data
     */
    OMNI_USD_API const std::string& GetGeometryPath(uint32_t geometryId);

    /**
     * Get unique gizmo id.
     * When it not needed freeGizmoUID should be called so that it can be reused.
     * If it is not called this method return the same uid on next call with given path.
     * @param path Prim path that will be used for prim selection when gizmo uid is picked.
     */
    OMNI_USD_API uint32_t getGizmoUID(const PXR_NS::SdfPath& path);

    /**
     * Allows to notify that gizmo uid is no longer used.
     * @param uid Gizmo unique id.
     */
    OMNI_USD_API void freeGizmoUID(uint32_t uid);

    /**
     * Returns prim path for given gizmo uid.
     * @param uid Gizmo unique id.
     * @return A pointer to path for given uid or nullptr for invalid uid.
     */
    OMNI_USD_API const PXR_NS::SdfPath* getPathForGizmoUID(uint32_t uid);

    /**
     * Register selection group.
     * Note: Supposed that this function called once at app initialization.
     */
    OMNI_USD_API uint8_t registerSelectionGroup();

    /**
     * Setup selection group outline color.
     */
    OMNI_USD_API void setSelectionGroupOutlineColor(uint8_t groupId, const carb::Float4& color);

    /**
     * Setup selection group shade color.
     */
    OMNI_USD_API void setSelectionGroupShadeColor(uint8_t groupId, const carb::Float4& color);

    /**
     * Set selection group for specified primitives path.
     */
    OMNI_USD_API void setSelectionGroup(uint8_t groupId, const std::string& path);

    /**
     * Query for camera settings for the camera prim
     */
    OMNI_USD_API omni::usd::hydra::CameraSettings getCameraSettings(const PXR_NS::SdfPath& path, double aspectRatio);

    /**
     * Set the aperture fit policy
     */
    OMNI_USD_API void setCameraWindowPolicy(carb::scenerenderer::CameraFit policy);

    /**
     * Returns the current frame data based on the requested feature, such as profiler data.
     * If frame is not finished processing the data, the result of the previous frame is returned.
     * For multi-GPU, query the device count and then set deviceIndex to the desired device index.
     *
     * @param dataType    Specified the requested data type to return.
     * @param deviceIndex The index of GPU device to get the frame data from. Set to zero in Single-GPU mode.
     *                    You may query the number of devices with FrameDataType::eGpuProfilerDeviceCount.
     *                    deviceIndex is ignored when the type is set to eGpuProfilerDeviceCount.
     * @param data        A pointer to the returned data. Returns nullptr for failures or eGpuProfilerDeviceCount.
     *                    You may pass nullptr if you only need dataSize.
     * @param dataSize    The size of the returned data in bytes, the number of structures, or device count based on
     *                    the dataType. For strings, it includes the null-termination.
     * @param engineIndex The index of an attached HydraEngine or -1 for the -active- HydraEngine.
     */
    OMNI_USD_API void getFrameData(carb::renderer::FrameDataType dataType,
                                   uint32_t deviceIndex,
                                   void** data,
                                   size_t* dataSize,
                                   uint32_t engineIndex = -1);

    // Exposed to Python (see viewport based api getHydraEngineDesc() below)
    OMNI_USD_API size_t getAttachedHydraEngineCount() const;
    OMNI_USD_API const char* getAttachedHydraEngineName(size_t hydraEngineIdx) const;

    // Returns the HydraEngineDesc describing the hydra engine being used by this viewport
    OMNI_USD_API HydraEngineDesc getHydraEngineDesc(ViewportHandle handle) const;

    /**
     * Allow for viewoverrides to be added through the usd context
     */
    OMNI_USD_API void registerViewOverrideToHydraEngines(omni::usd::hydra::IViewOverrideBase* viewOverride);

    /**
     * Allow for viewoverrides to be removed through the usd context
     */
    OMNI_USD_API void unregisterViewOverrideToHydraEngines(omni::usd::hydra::IViewOverrideBase* viewOverride);

    /**
     * Allow scheduling override to be set for usd context
     */
    OMNI_USD_API void setUsdContextSchedulingOverride(IUsdContextSchedulingOverride* schedulingOverride);

    /**
     * Reset scheduling override
     */
    OMNI_USD_API void resetUsdContextSchedulingOverride();

    /**
     * Retrieves the mutex for `*this`
     */
    OMNI_USD_API IUsdMutex& getMutex();

    /**
     * Stop any picking in flight for a specific View
     */
    OMNI_USD_API void stopAllPickingForView(ViewPickingId pickingId);

    /**
     * Sets the timeline. This must be called before the context is used, right after its creation.
     */
    OMNI_USD_API void setTimeline(const std::string& name = "");

    /**
     * Retrieves the name of the timeline
     */
    OMNI_USD_API std::string getTimelineName() const;

    /**
     * Retrieves the timeline
     */
    OMNI_USD_API timeline::TimelinePtr getTimeline() const;

    /**
     * Retrieves the name of the context
     */
    OMNI_USD_API std::string getName() const;

    /**
     * Trigger creation of the runloop associated with the given hydraengine configuration
     * (otherwise it will only be created the first time an associated viewport is renderer)
     */
    int32_t getOrCreateRunloopThread(const char* name, const hydra::EngineCreationConfig& engineConfig, bool setWarmup);

    /**
     * Extract the engine warmup config from the settings
     */
    typedef std::unordered_map<uint32_t, hydra::EngineCreationConfig> EngineWarmupConfig;
    static bool getEngineWarmupConfig(const carb::settings::ISettings& settings,
                                      const char* hydraEngineName,
                                      EngineWarmupConfig& config);

    /**
     * Return the default config for hydraengine creation for this engine type, and index
     */
    static hydra::EngineCreationConfig getDefaultEngineCreationConfig(const carb::settings::ISettings& settings,
                                                                      const char* engineName,
                                                                      uint32_t engineIndex);

    /*
     * Opens a existing USD stage with specified session layer. This is an asynchronous call.
     * This funciton can be used to speed up the stage composition to avoid re-composing stage
     * caused by inserting a sublayer under session layer after stage opened.
     *
     * @param url The file path. For Omniverse file, you must connect to Omniverse first and pass the url with prefix
     * "omniverse:".
     * @param sessionLayerUrl. The specified session layer to use. If it's empty or not provided, it will work
     * as the same as openStage. If it's provided but cannot be opened, it will return false.
     * @param fn The callback function when stage is opened or fails to open.
     * @param loadSet Specifies the initial set of prims to load when opening a UsdStage.
     */
    OMNI_USD_API bool openStageWithSessionLayer(const char* fileUrl,
                                                const char* sessionLayerUrl,
                                                const OnStageResultFn& resultFn,
                                                UsdContextInitialLoadSet loadSet = UsdContextInitialLoadSet::eLoadAll);

    /*
     * Tries to cancel save. It only take effects when it's called after receiving event StageEventType::eSaving or
     * StageEventType::eSettingsSaving.
     */
    OMNI_USD_API void tryCancelSave();

    /**
     * Removes a hydra engine and releases IHydraEngine references in the context.
     * Don't call this API directly, call UsdManager::releaseHydraEngine() instead to destroy the engine and its
     * context, which also makes a call to removeHydraEngine().
     */
    OMNI_USD_API bool removeHydraEngine(omni::usd::hydra::OpaqueSharedHydraEngineContextPtr hydraEngineContext);

    /**
     * Returns a vector of asset-open errors that occurred during the last attempted stage load / open.
     * These errors are only captured if:
     * - the stage is opened via UsdContext (ie, they are not collected if a stage is opened directly via the
     *   OpenUSD API)
     * - the stage is not opened via realm using multiple processes
     *   - technically, they are collected on each individual worker opening thread, but they are not gathered
     *     and sent up to triggering process / thread
     *   - we can implement this in the future if needed / desired
     * - the failStageOpenOnAssetErrors setting is on (it defaults to off)
     *   - the full setting path is:
     *         /persistent/app/omni.usd/failStageOpenOnAssetErrors
     */
    OMNI_USD_API const std::vector<PXR_NS::TfWarning>& getAssetOpenErrors() const;


    /**
     * Returns the total number of asset-open errors that occurred during the last attempted stage load / open.
     * This number may be greater than the number captured (ie, getAssetOpenErrors()) if the maximum errors stored
     * is exceeeded - this is a (non-persistent) setting stored in:
     *     /app/omni.usd/maxAssetErrorsStored
     * ...which defaults to 100.
     * See getAssetOpenErrors() for details on the conditions under which these errors are collected.
     */
    OMNI_USD_API int getNumAssetOpenErrors() const;
private:
    /**
     * Constructor.
     *
     * @param name The name of the context.
     */
    UsdContext(const std::string& name);

    /**
     * No copy.
     */
    UsdContext(const UsdContext&) = delete;

    /**
     * No assign.
     */
    UsdContext& operator=(const UsdContext&) = delete;

    /**
     * Destructor.
     */
    ~UsdContext();

    friend UsdManager;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
}
}
