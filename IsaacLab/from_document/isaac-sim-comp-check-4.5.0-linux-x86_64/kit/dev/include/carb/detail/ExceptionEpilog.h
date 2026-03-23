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
// This file is counterpart to ExceptionProlog.h

#include "../Defines.h"

#undef CARBLOCAL_TRY_BEGIN
#undef CARBLOCAL_CATCH_ALL
#undef CARBLOCAL_CATCH_END
#undef CARBLOCAL_RETHROW
#undef CARBLOCAL_THROW
#undef CARBLOCAL_THROW0
