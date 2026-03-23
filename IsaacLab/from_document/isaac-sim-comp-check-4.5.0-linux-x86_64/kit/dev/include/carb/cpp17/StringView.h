// Copyright (c) 2023-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! \file
//! \brief Redirection for backwards compatibility
#pragma once

#include "../cpp/StringView.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp17
{

using ::carb::cpp::basic_string_view;
using ::carb::cpp::string_view;
using ::carb::cpp::wstring_view;
#if CARB_HAS_CPP20 && defined(__cpp_char8_t)
using ::carb::cpp::u8string_view;
#endif
using ::carb::cpp::u16string_view;
using ::carb::cpp::u32string_view;
using ::carb::cpp::operator""_sv;

} // namespace cpp17
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    static_assert(sizeof(carb::cpp17::basic_string_view<char>), "");
    static_assert(sizeof(carb::cpp17::string_view), "");
    static_assert(sizeof(carb::cpp17::wstring_view), "");
    static_assert(sizeof(carb::cpp17::u16string_view), "");
    static_assert(sizeof(carb::cpp17::u32string_view), "");
});
