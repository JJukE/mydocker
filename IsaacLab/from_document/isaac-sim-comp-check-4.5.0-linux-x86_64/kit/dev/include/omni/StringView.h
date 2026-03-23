// Copyright (c) 2023-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief Implementation of \ref omni::span
#pragma once

#include "../carb/cpp/StringView.h"

namespace omni
{

//! @copydoc carb::cpp::string_view
using string_view = ::carb::cpp::string_view;

//! @copydoc carb::cpp::wstring_view
using wstring_view = ::carb::cpp::wstring_view;

#if CARB_HAS_CPP20 && defined(__cpp_char8_t)
//! basic_string_view<char8_t>
//! @see basic_string_view
using u8string_view = ::carb::cpp::u8string_view;
#endif

//! @copydoc carb::cpp::u16string_view
using u16string_view = ::carb::cpp::u16string_view;

//! @copydoc carb::cpp::u32string_view
using u32string_view = ::carb::cpp::u32string_view;

} // namespace omni
