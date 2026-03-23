// Copyright (c) 2020-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//
#pragma once

#include <carb/IObject.h>

#include <gpu/realm/Utils.h>
#include <omni/kit/KitTypes.h>
#include <omni/usd/Api.h>

#include <memory.h>

namespace carb
{
namespace settings
{
struct ISettings;
}
}

namespace gpu
{
namespace foundation
{
struct GpuFoundation;
}
}

namespace carb
{
namespace renderer
{
struct Context;
struct Renderer;
}
}

namespace omni
{
namespace usd
{

namespace hydra
{
class IHydraEngineFactory;
enum class EngineCreationFlags : uint32_t;
using IHydraEngineFactoryPtr = carb::ObjectPtr<IHydraEngineFactory>;
struct OpaqueSharedHydraEngineContext;
typedef struct OpaqueSharedHydraEngineContext* OpaqueSharedHydraEngineContextPtr;

/**
 * Configuration of an hydraengine
 * - UsdManager::getOrCreateHydraEngine will return the same engine if the configuration parameters match
 */
struct EngineCreationConfig
{
    EngineCreationFlags flags;
    uint32_t tickRateInHz = uint32_t(-1);
    uint32_t deviceMask = 0; ///< Determines what GPUs to be used for the hydra engine. If set to 0, then all GPUs in
                             ///< the DeviceGroup of the foundation is used.
                             /// Note: deviceMask deprecates the setting
                             /// "/renderer/{hydraengine_name}_context/activeDevice" which overrides the mask for all
                             /// engines. For backward compatibility, the setting still works if deviceMask is set to 0.
};

}

class UsdContext;
class UsdManager
{
public:
    /**
     * Creates a UsdContext.
     *
     * @param name Name of the UsdContext to be created. The default context is named with empty string "".
     * @return Created UsdContext, or existing one if a context with same name is already created.
     */
    OMNI_USD_API static UsdContext* createContext(const std::string& name = "");

    /**
     * Destroys a UsdContext.
     *
     * @param name of the UsdContext to be destroyed. The default context is named with empty string "".
     * @return true if destroyed successfully, false if failed.
     */
    OMNI_USD_API static bool destroyContext(const std::string& name = "");

    /**
     * Gets a UsdContext.
     *
     * @param name of the UsdContext to get. The default context is named with empty string "".
     */
    OMNI_USD_API static UsdContext* getContext(const std::string& name = "");


    /*
     * Use by IHydraEngines at load/unload to register their factory class
     *
     */
    OMNI_USD_API static void registerHydraEngineFactory(const char* name,
                                                        omni::usd::hydra::IHydraEngineFactoryPtr factory);

    OMNI_USD_API static void unregisterHydraEngineFactory(const char* name);

    /*
     * Creates a new hydra engine instance using the registered factory and attaches
     * it to the UsdContext. The hydra engine will have it's own unique syncScope
     * and run in a separate rendering thread if asyncRendering is enabled
     */
    OMNI_USD_API static hydra::OpaqueSharedHydraEngineContextPtr addHydraEngine(const char* name, UsdContext* context);

    /**
     * Adds all loaded hydra engines to the USDContext. Used at startup
     *
     * All loaded hydra engines will share a single syncScope and run in a single rendering
     * thread when asyncRendering is enabled.
     *
     * Return type, first is the name of the engine, second is an opaque pointer to engine data
     *
     * Until legacy carb settings /renderer/active and /renderer/enabled are no longer used to
     * set the default hydra engine or override what extensions are "enabled", we need this function
     * to support legacy STARTUP behavior and it should be executed once AFTER all the extensions have loaded
     */
    OMNI_USD_API static std::vector<std::pair<std::string, hydra::OpaqueSharedHydraEngineContextPtr>> attachAllHydraEngines(
        UsdContext* context);

    /*
     * Advances all the hydra engines sync scopes rtx::kMaxFramesInFlight to trigger
     * deferred releases of all objects. Typically used to allow fully unloading
     * the USD Mesh data before loading another USD Stage
     */
    OMNI_USD_API static void advanceAllSyncScopes();

    /**
     * releaseAllHydraEngines() is designed to support the legacy Kit 1.0 Editor
     *
     * Unregisters all HydraEngineFactories and destroys any created OpaqueSharedHydraEngineContextPtr.
     *
     * @param context Optional, if valid, context releases all hydra engines prior to destroying any
     * OpaqueSharedHydraEngineContextPtr
     */
    OMNI_USD_API static void releaseAllHydraEngines(UsdContext* context = nullptr);

    /*
     * setSettingsPlugin() + setFoundationPlugins() are required initialization
     * methods of USDManager
     */
    OMNI_USD_API static void setSettingsPlugin(carb::settings::ISettings* settings);

    OMNI_USD_API static void setFoundationPlugins(gpu::foundation::GpuFoundation* gpuFoundation, uint32_t syncScope);

    // Until Kit redoes their implmentation of GpuFoundations integration, let's provide a global
    // getter for those that need GpuFoundations to be unblocked
    OMNI_USD_API static void getFoundationPlugins(gpu::foundation::GpuFoundation** outGpuFoundation);

    /**
     * @brief Called when from omni.usd extension shutdown
     *
     */
    OMNI_USD_API static void shutdownUsd();

    /**
     * Gets corresponding UsdContext of specified stage id.
     */
    OMNI_USD_API static UsdContext* getContextFromStageId(long int stageId);

    /*
     * Obtains a hydraengine instance with a matching config to the one passed in (If none exist yet, will create a new
     * one). A new hydra engine will have it's own unique syncScope and run in a separate rendering thread if
     * asyncRendering is enabled
     */
    OMNI_USD_API static hydra::OpaqueSharedHydraEngineContextPtr getOrCreateHydraEngine(
        const char* name, UsdContext* context, const hydra::EngineCreationConfig& config);

    /**
     * Destroys a hydra engine and removes it from the rendering threads.
     * Viewports should be destroyed before calling this engine destruction API.
     * The engine slot (index) may be reserved for the future engine creation.
     */
    OMNI_USD_API static bool releaseHydraEngine(UsdContext* context,
                                                omni::usd::hydra::OpaqueSharedHydraEngineContextPtr hydraEngineContext);

private:
    /**
     * Constructor.
     */
    UsdManager();

    /**
     * No copy.
     */
    UsdManager(const UsdManager&) = delete;

    /**
     * No assign.
     */
    UsdManager& operator=(const UsdManager&) = delete;

    /**
     * Destructor.
     */
    ~UsdManager();

    static UsdManager* instance();

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
}
}

namespace gpu
{
namespace realm
{

template <>
struct TypeSerializer<omni::usd::hydra::EngineCreationConfig>
{
    static_assert(sizeof(omni::usd::hydra::EngineCreationConfig) == 12,
                  "This structure is serialized chaning its topology requires adjusting this code.");
    void write(Serializer& serializer, const omni::usd::hydra::EngineCreationConfig& engineCreationConfig)
    {
        serializer.write(engineCreationConfig.flags);
        serializer.write(engineCreationConfig.tickRateInHz);
        serializer.write(engineCreationConfig.deviceMask);
    }
    void read(Deserializer& deserializer, omni::usd::hydra::EngineCreationConfig& engineCreationConfig)
    {
        engineCreationConfig.flags = deserializer.read<omni::usd::hydra::EngineCreationFlags>();
        engineCreationConfig.tickRateInHz = deserializer.read<uint32_t>();
        engineCreationConfig.deviceMask = deserializer.read<uint32_t>();
    }
};

} // namespace realm
} // namespace gpu
