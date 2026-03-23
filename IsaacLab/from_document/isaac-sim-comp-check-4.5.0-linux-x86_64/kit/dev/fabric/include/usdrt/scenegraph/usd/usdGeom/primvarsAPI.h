// Copyright (c) 2022-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//
// TODO USD COPYRIGHT AS WELL?
#pragma once

/// @file usdGeom/primvarsAPI.h

// GENERATED FILE DO NOT EDIT

#include "usdrt/scenegraph/usd/usd/apiSchemaBase.h"

// clang-format off
/* Extra Includes
// USDRT NOTE: Use custom includes below instead to allow for
// implementation differences in the custom code. This section comes
// from the pxr schema.usda files which we may not have in usdrt.
//#include "usdrt/scenegraph/usd/usdGeom/primvar.h" */
// clang-format on

// ===================================================================== //
// Feel free to add custom includes between BEGIN and END CUSTOM INCLUDES
// below this line. It will be preserved by the code generator.
// We are using this instead of the `extraIncludes` in the pxr schema.usda
// files to allow for implementation differences in the custom code.
// ===================================================================== //
// --(BEGIN CUSTOM INCLUDES)--
// --(END CUSTOM INCLUDES)--

#include <omni/core/IObject.h>
#include <usdrt/scenegraph/base/tf/token.h>
#include <usdrt/scenegraph/base/vt/array.h>
#include <usdrt/scenegraph/interface/IRtAttribute.h>
#include <usdrt/scenegraph/interface/IRtPrim.h>
#include <usdrt/scenegraph/interface/IRtPrimRange.h>
#include <usdrt/scenegraph/interface/IRtRelationship.h>
#include <usdrt/scenegraph/interface/IRtStage.h>
#include <usdrt/scenegraph/usd/sdf/path.h>
#include <usdrt/scenegraph/usd/sdf/types.h>
#include <usdrt/scenegraph/usd/sdf/valueTypeName.h>
#include <usdrt/scenegraph/usd/usd/attribute.h>
#include <usdrt/scenegraph/usd/usd/common.h>
#include <usdrt/scenegraph/usd/usd/prim.h>
#include <usdrt/scenegraph/usd/usd/timeCode.h>

namespace usdrt
{

// -------------------------------------------------------------------------- //
// PRIMVARSAPI                                                                 //
// -------------------------------------------------------------------------- //

/// @class UsdGeomPrimvarsAPI
///
/// UsdGeomPrimvarsAPI encodes geometric "primitive variables",
/// as UsdGeomPrimvar, which interpolate across a primitive's topology,
/// can override shader inputs, and inherit down namespace.
///
/// \section usdGeom_PrimvarFetchingAPI Which Method to Use to Retrieve Primvars
///
/// While creating primvars is unambiguous (CreatePrimvar()), there are quite
/// a few methods available for retrieving primvars, making it potentially
/// confusing knowing which one to use.  Here are some guidelines:
///
/// \li If you are populating a GUI with the primvars already available for
/// authoring values on a prim, use GetPrimvars().
/// \li If you want all of the "useful" (e.g. to a renderer) primvars
/// available at a prim, including those inherited from ancestor prims, use
/// FindPrimvarsWithInheritance().  Note that doing so individually for many
/// prims will be inefficient.
/// \li To find a particular primvar defined directly on a prim, which may
/// or may not provide a value, use GetPrimvar().
/// \li To find a particular primvar defined on a prim or inherited from
/// ancestors, which may or may not provide a value, use
/// FindPrimvarWithInheritance().
/// \li To *efficiently* query for primvars using the overloads of
/// FindPrimvarWithInheritance() and FindPrimvarsWithInheritance(), one
/// must first cache the results of FindIncrementallyInheritablePrimvars() for
/// each non-leaf prim on the stage.

class UsdGeomPrimvarsAPI : public UsdAPISchemaBase
{

public:
    /// Compile time constant representing what kind of schema this class is.
    ///
    /// @sa UsdSchemaType
    static const UsdSchemaType schemaType = UsdSchemaType::NonAppliedAPI;


    /// Construct a UsdGeomPrimvarsAPI on UsdPrim @p prim.
    /// Equivalent to UsdGeomPrimvarsAPI::Get(prim.GetStage(), prim.GetPath())
    /// for a @em valid @p prim , but will not immediately throw an error for
    /// an invalid @p prim
    explicit UsdGeomPrimvarsAPI(const UsdPrim& prim = UsdPrim()) : UsdAPISchemaBase(prim)
    {
    }

    /// Construct a UsdGeomPrimvarsAPI on the prim held by @p schemaObj .
    /// Should be preferred over UsdGeomPrimvarsAPI(schemaObj.GetPrim()),
    /// as it preserves SchemaBase state.
    explicit UsdGeomPrimvarsAPI(const UsdSchemaBase& schemaObj) : UsdAPISchemaBase(schemaObj)
    {
    }

    /// Destructor.
    virtual ~UsdGeomPrimvarsAPI()
    {
    }

    /// @private
    static const TfToken _GetStaticTfType();

private:
    /* virtual */
    const TfToken _GetTfTypeToken() const override;

    // ===================================================================== //
    // Feel free to add custom code for the class definition between BEGIN
    // and END CUSTOM DECLARATION CODE below this line. It will be preserved by
    // the code generator.
    // ===================================================================== //
    // --(BEGIN CUSTOM DECLARATION CODE)--
    // --(END CUSTOM DECLARATION CODE)--
};

inline const TfToken UsdGeomPrimvarsAPI::_GetStaticTfType()
{
    const static TfToken token("UsdGeomPrimvarsAPI");
    return token;
}

/* virtual */
inline const TfToken UsdGeomPrimvarsAPI::_GetTfTypeToken() const
{
    const static TfToken token("UsdGeomPrimvarsAPI");
    return token;
}


} // namespace usdrt
// ===================================================================== //
// Feel free to add custom code below this line. It will be preserved by
// the code generator.
//
// Just remember to wrap code in the appropriate delimiters:
// 'namespace usdrt {', '}'.
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--
