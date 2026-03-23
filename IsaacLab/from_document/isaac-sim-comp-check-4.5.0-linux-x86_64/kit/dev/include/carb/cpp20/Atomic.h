// Copyright (c) 2023, NVIDIA CORPORATION. All rights reserved.
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

#include "../cpp/Atomic.h"

CARB_FILE_DEPRECATED_MSG("Use carb/cpp include path and carb::cpp namespace instead")

namespace carb
{
namespace cpp20
{

using ::carb::cpp::atomic;
using ::carb::cpp::atomic_bool;
using ::carb::cpp::atomic_char;
using ::carb::cpp::atomic_char16_t;
using ::carb::cpp::atomic_char32_t;
using ::carb::cpp::atomic_int;
using ::carb::cpp::atomic_int16_t;
using ::carb::cpp::atomic_int32_t;
using ::carb::cpp::atomic_int64_t;
using ::carb::cpp::atomic_int8_t;
using ::carb::cpp::atomic_int_fast16_t;
using ::carb::cpp::atomic_int_fast32_t;
using ::carb::cpp::atomic_int_fast64_t;
using ::carb::cpp::atomic_int_fast8_t;
using ::carb::cpp::atomic_int_least16_t;
using ::carb::cpp::atomic_int_least32_t;
using ::carb::cpp::atomic_int_least64_t;
using ::carb::cpp::atomic_int_least8_t;
using ::carb::cpp::atomic_intmax_t;
using ::carb::cpp::atomic_intptr_t;
using ::carb::cpp::atomic_llong;
using ::carb::cpp::atomic_long;
using ::carb::cpp::atomic_notify_all;
using ::carb::cpp::atomic_notify_one;
using ::carb::cpp::atomic_ptrdiff_t;
using ::carb::cpp::atomic_ref;
using ::carb::cpp::atomic_schar;
using ::carb::cpp::atomic_short;
using ::carb::cpp::atomic_size_t;
using ::carb::cpp::atomic_uchar;
using ::carb::cpp::atomic_uint;
using ::carb::cpp::atomic_uint16_t;
using ::carb::cpp::atomic_uint32_t;
using ::carb::cpp::atomic_uint64_t;
using ::carb::cpp::atomic_uint8_t;
using ::carb::cpp::atomic_uint_fast16_t;
using ::carb::cpp::atomic_uint_fast32_t;
using ::carb::cpp::atomic_uint_fast64_t;
using ::carb::cpp::atomic_uint_fast8_t;
using ::carb::cpp::atomic_uint_least16_t;
using ::carb::cpp::atomic_uint_least32_t;
using ::carb::cpp::atomic_uint_least64_t;
using ::carb::cpp::atomic_uint_least8_t;
using ::carb::cpp::atomic_uintmax_t;
using ::carb::cpp::atomic_uintptr_t;
using ::carb::cpp::atomic_ullong;
using ::carb::cpp::atomic_ulong;
using ::carb::cpp::atomic_ushort;
using ::carb::cpp::atomic_wait;
using ::carb::cpp::atomic_wait_explicit;
using ::carb::cpp::atomic_wchar_t;

} // namespace cpp20
} // namespace carb

CARB_INCLUDE_PURIFY_TEST({
    static_assert(std::is_class<carb::cpp20::atomic<int>>::value, "1");
    static_assert(std::is_class<carb::cpp20::atomic_ref<int>>::value, "2");
    static_assert(std::is_function<decltype(carb::cpp20::atomic_wait<int>)>::value, "3");
    static_assert(std::is_function<decltype(carb::cpp20::atomic_wait_explicit<int>)>::value, "4");
    static_assert(std::is_function<decltype(carb::cpp20::atomic_notify_one<int>)>::value, "5");
    static_assert(std::is_function<decltype(carb::cpp20::atomic_notify_all<int>)>::value, "6");
});
