// Copyright (c) 2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

#pragma once

#include "../carb/Defines.h"

#if (CARB_HAS_CPP17 || defined __INTELLISENSE__) && !defined DOXYGEN_BUILD

#    define OMNI_MOF 1

#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_CV const
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_REF &
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_REF &&
#    define OMNI_MOF_MOVE std::move
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_CV const
#    define OMNI_MOF_REF &
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_CV const
#    define OMNI_MOF_REF &&
#    define OMNI_MOF_MOVE std::move
#    include "detail/MoveOnlyFunctionImpl.h"

// Until MSVC is updated it must have separate specializations for noexcept(true)
// and noexcept(false).
// Once it's updated noexcept can be inferred from a template argument:
//      template <typename Ret, typename... Args, bool Noex>
//      class move_only_function<Ret(Args...) OMNI_MOF_CV OMNI_MOF_REF noexcept(Noex)>
#    define OMNI_MOF_NOEX noexcept
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_CV const
#    define OMNI_MOF_NOEX noexcept
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_REF &
#    define OMNI_MOF_NOEX noexcept
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_REF &&
#    define OMNI_MOF_MOVE std::move
#    define OMNI_MOF_NOEX noexcept
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_CV const
#    define OMNI_MOF_REF &
#    define OMNI_MOF_NOEX noexcept
#    include "detail/MoveOnlyFunctionImpl.h"
#    define OMNI_MOF_CV const
#    define OMNI_MOF_REF &&
#    define OMNI_MOF_MOVE std::move
#    define OMNI_MOF_NOEX noexcept
#    include "detail/MoveOnlyFunctionImpl.h"

#    undef OMNI_MOF

#elif !defined(CARB_INCLUDE_PURIFY)

#    error MoveOnlyFunction requires at least C++17

#endif
