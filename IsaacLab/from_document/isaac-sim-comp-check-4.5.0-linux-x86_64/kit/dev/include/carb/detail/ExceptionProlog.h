// Copyright (c) 2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief Implementation detail

// Note! Intentionally without include guards
// This file is counterpart to ExceptionEpilog.h

#include "../Defines.h"

//! \cond DEV
// clang-format off
#if CARB_EXCEPTIONS_ENABLED
#    define CARBLOCAL_TRY_BEGIN try
#    define CARBLOCAL_CATCH_ALL catch (...)
#    define CARBLOCAL_RETHROW throw
#    define CARBLOCAL_THROW(x, y) throw x(y)
#    define CARBLOCAL_THROW0(x) throw x()
#else
#    define CARBLOCAL_TRY_BEGIN if (1)
#    define CARBLOCAL_CATCH_ALL else if (0)
#    define CARBLOCAL_RETHROW static_cast<void>(0)
#    define CARBLOCAL_THROW(x, y) CARB_FATAL_UNLESS(false, "Exceptions disabled but should have thrown " #x ": %s", (y))
#    define CARBLOCAL_THROW0(x) CARB_FATAL_UNLESS(false, "Exceptions disabled but should have thrown " #x)
#endif
// clang-format on
//! \endcond
