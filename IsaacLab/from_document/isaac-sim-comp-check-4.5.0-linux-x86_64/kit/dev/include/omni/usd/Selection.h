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

#include "Api.h"
#include <omni/kit/KitTypes.h>

#include <memory.h>
#include <mutex>

namespace omni
{
namespace usd
{
class UsdContext;

typedef std::vector<std::pair<PXR_NS::SdfPath, uint8_t>> SdfPathGroupIdVector;

class Selection
{
public:
    Selection(UsdContext* usdContext);
    ~Selection();

    Selection(const Selection&) = delete;
    Selection& operator=(const Selection&) = delete;

    enum class SourceType
    {
        eUsd = 1 << 0, // Selection targeting USD stage
        eFabric = 1 << 1, // Selection targeting Fabric stage
        eAll = eUsd | eFabric // Selection targeting both stages
    };

    /* Control behaviors when setting selection with SourceType::eAll */
    enum class SourceBehavior
    {
        eFabricOnly = 1 << 0, // Add Fabric-only paths to Fabric selection
        eFabricAll = 1 << 1, // Add both USD and Fabric paths to Fabric Selection
    };

    /**
     * Deprecated. Use setSelectedPrimPaths() with SourceType instead.
     * Sets all selected prim paths for USD stage. This will replace existing selected paths with given ones.
     *
     * @param paths The vector of selected prim paths to be set.
     * @param expandInStage true to expand the path in Stage Window on selection.
     * @return true if the selected paths are changed.
     */
    OMNI_USD_API bool setSelectedPrimPaths(const std::vector<std::string>& paths, bool expandInStage);

    /**
     * Sets all selected prim paths for USD or Fabric stage. This will replace existing selected paths with given ones.
     *
     * @param paths The vector of selected prim paths to be set.
     * @param expandInStage true to expand the path in Stage Window on selection.
     * @param source one of SourceType, selection can be set to usd, fabric or both stages.
     * @return true if the selected paths are changed.
     */
    OMNI_USD_API bool setSelectedPrimPaths(const std::vector<std::string>& paths, bool expandInStage, SourceType source);

    /**
     * Deprecated. Use getSelectedPrimPaths() with SourceType instead.
     * Gets all selected prim paths from USD stage.
     * @return a vector containing all selected prim paths of USD stage.
     */
    OMNI_USD_API std::vector<std::string> getSelectedPrimPaths();

    /**
     * Gets all selected prim paths from USD, Fabric stage or both.
     * @param source one of SourceType, get selection from either usd or fabric stage, or from both as a unified
     * list
     * @return a vector containing all selected prim paths of USD, Fabric or both stages.
     */
    OMNI_USD_API std::vector<std::string> getSelectedPrimPaths(SourceType source);

    /**
     * Deprecated. Use clearSelectedPrimPaths() with SourceType instead.
     * Clears all selected prim paths.
     *
     * @return true if the selected paths are changed.
     */
    OMNI_USD_API bool clearSelectedPrimPaths();

    /**
     * Clears all selected prim paths.
     *
     * @param source one of SourceType, clear selection of usd, fabric, or both stages
     * @return true if the selected paths are changed.
     */
    OMNI_USD_API bool clearSelectedPrimPaths(SourceType source);

    /**
     * Deprecated. Use setPrimPathSelected() with SourceType instead.
     * Sets selection for a specific prim path. 
     *
     * @param path prim path to be set.
     * @param selected true to select and false to de-select.
     * @param forcePrim check if prim meets SelectionType.
     * @param clearSelected true to clear existing selection.
     * @param expandInStage true to expand the path in Stage Window on selection.
     * @return true if the selected paths are changed.
     */
    OMNI_USD_API bool setPrimPathSelected(
        const char* path, bool selected, bool forcePrim, bool clearSelected, bool expandInStage);

    /**
     * Sets selection for a specific prim path.
     *
     * @param path prim path to be set.
     * @param selected true to select and false to de-select.
     * @param forcePrim check if prim meets SelectionType.
     * @param clearSelected true to clear existing selection.
     * @param expandInStage true to expand the path in Stage Window on selection.
     * @param source one of SourceType, selection can be set to usd, fabric or both stages.
     * @return true if the selected paths are changed.
     */
    OMNI_USD_API bool setPrimPathSelected(
        const char* path, bool selected, bool forcePrim, bool clearSelected, bool expandInStage, SourceType source);

    /**
     * Deprecated. Use isPrimPathSelected() with SourceType instead.
     * Gets if a prim path is selected.
     * @param path path of prim to check
     * @return true if prim path is selected.
     */
    OMNI_USD_API bool isPrimPathSelected(const char* path) const;

    /**
     * Gets if a prim path is selected.
     * @param path path of prim to check
     * @param source one of SourceType, selection can come from usd, fabric or both stages.
     * @return true if prim path is selected.
     */
    OMNI_USD_API bool isPrimPathSelected(const char* path, SourceType source) const;

    /**
     * Something has changed (layers may have been added, removed, or muted), where we can no longer
     * guarantee the selection paths are valid, so mark them as dirty.
     *
     */
    OMNI_USD_API void setDirty();

    /** Deprecated. Use add() with SourceType instead. */
    OMNI_USD_API bool add(PXR_NS::SdfPath path, bool forcePrimMode, bool clearSelected, bool expandInStage);
    OMNI_USD_API bool add(PXR_NS::SdfPath path, bool forcePrimMode, bool clearSelected, bool expandInStage,
        SourceType source, SourceBehavior behavior=SourceBehavior::eFabricAll);

    /** Deprecated. Use remove() with SourceType instead. */
    OMNI_USD_API bool remove(PXR_NS::SdfPath path, bool forcePrimMode);
    OMNI_USD_API bool remove(PXR_NS::SdfPath path, bool forcePrimMode, SourceType source, SourceBehavior behavior=SourceBehavior::eFabricAll);
    OMNI_USD_API bool removePathAndDescendents(PXR_NS::SdfPath path);
    OMNI_USD_API PXR_NS::SdfPathVector& getRenderablePaths();
    OMNI_USD_API PXR_NS::SdfPathVector& getAddedRenderablePaths();
    OMNI_USD_API PXR_NS::SdfPathVector& getRemovedRenderablePaths();
    OMNI_USD_API const PXR_NS::SdfPathVector& getModelPaths();
    OMNI_USD_API const PXR_NS::SdfPathVector& getCameraPaths();
    OMNI_USD_API const std::vector<PXR_NS::UsdLuxDistantLight>& getDistantLights();
    OMNI_USD_API const std::vector<PXR_NS::UsdLuxRectLight>& getRectLights();
    OMNI_USD_API const std::vector<PXR_NS::UsdLuxSphereLight>& getSphereLights();
    OMNI_USD_API const std::vector<PXR_NS::UsdLuxCylinderLight>& getCylinderLights();
    OMNI_USD_API const std::vector<PXR_NS::UsdLuxDiskLight>& getDiskLights();
    OMNI_USD_API const std::vector<PXR_NS::UsdPrim>& getSounds();
    OMNI_USD_API const std::vector<PXR_NS::UsdPrim>& getListeners();
    OMNI_USD_API const std::vector<PXR_NS::UsdShadeMaterial>& getMaterials();
    OMNI_USD_API const std::vector<PXR_NS::UsdShadeShader>& getShaders();
    OMNI_USD_API PXR_NS::SdfPath getQueriedPath() const;
    OMNI_USD_API void setQueriedPath(PXR_NS::SdfPath path, bool addOutline = false);
    OMNI_USD_API void getPrimBySelectionMode(PXR_NS::UsdPrim& prim) const;

    OMNI_USD_API std::recursive_mutex& getMutex();

    OMNI_USD_API void update();

    /** Deprecated. Use setSelectedPrimPathsV2() with SourceType instead. */ 
    OMNI_USD_API bool setSelectedPrimPathsV2(const PXR_NS::SdfPathVector& paths);
    /**
     * Sets all selected prim paths for USD or Fabric stage. This will replace existing selected paths with given ones.
     *
     * @param paths The vector of Sdf paths of selected prims to be set.
     * @param source one of SourceType, selection can be set to usd, fabric or both stages.
     * @param behavior only works when source is eAll, when it's eFabricOnly, Fabric selection will only contain Fabric-only prim paths.
     * @return true if the selected paths are changed.
     */
    OMNI_USD_API bool setSelectedPrimPathsV2(const PXR_NS::SdfPathVector& paths, SourceType source, SourceBehavior behavior=SourceBehavior::eFabricAll);


    /** Deprecated. Use getSelectedPrimPathsV2() with SourceType instead. */
    OMNI_USD_API PXR_NS::SdfPathVector getSelectedPrimPathsV2() const;
    /**
     * Gets all selected prim paths from USD, Fabric stage or both.
     * @param source one of SourceType, get selection from either usd or fabric stage, or from both as a unified
     * list
     * @return a SdfPathVector containing all selected prim paths of USD, Fabric or both stages.
     */
    OMNI_USD_API PXR_NS::SdfPathVector getSelectedPrimPathsV2(SourceType source) const;
    OMNI_USD_API bool empty() const;
    OMNI_USD_API void selectAllPrims(const std::vector<std::string>& typeNames);
    OMNI_USD_API void selectInvertedPrims();

    /*
     * Custom selection with groupId.
     * ibychkov: I added new methods for that to be sure there is no regression for regular selection.
     *           But it might be considered just add groupId support for regular selection.
     */
    OMNI_USD_API SdfPathGroupIdVector& getCustomRenderablePaths();
    OMNI_USD_API SdfPathGroupIdVector& getAddedCustomRenderablePaths();
    OMNI_USD_API PXR_NS::SdfPathVector& getRemovedCustomRenderablePaths();
    OMNI_USD_API void setCustomSelection(const PXR_NS::SdfPathVector& paths, uint8_t groupId);
    OMNI_USD_API void clearCustomSelection();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
}
}
