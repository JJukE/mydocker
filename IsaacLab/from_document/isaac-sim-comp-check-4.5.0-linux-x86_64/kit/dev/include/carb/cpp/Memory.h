// Copyright (c) 2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! \file
//! \brief C++14-compatible implementation of select functionality from C++ `<memory>` library.
#pragma once

#include "../Defines.h"

#include <memory>
#include <type_traits>

namespace carb
{
namespace cpp
{

#if !CARB_HAS_CPP17 || defined DOXYGEN_BUILD

//! Get the address of \a arg, even if \c operator& is overloaded. This is generally only useful in memory management
//! functions -- \c to_address is almost always preferable.
//!
//! \note
//! This function is \c constexpr even in C++14 mode.
template <typename T>
constexpr T* addressof(T& arg) noexcept
{
    return __builtin_addressof(arg);
}

//! Taking the address of a \c const rvalue is never correct.
template <typename T>
T const* addressof(T const&&) = delete;

#else

using std::addressof;

#endif

//! Call the destructor of \a p.
template <typename T>
constexpr std::enable_if_t<!std::is_array<T>::value> destroy_at(T* const p) noexcept(std::is_nothrow_destructible<T>::value)
{
    p->~T();
}

//! Call the destructor of all \a array elements.
//!
//! \tparam T The element type of the array must have a \c noexcept destructor. There no mechanism to safely use this
//!         function if an element throws. This is a departure from the C++20, wherein an exception thrown from a
//!         destructor will result in either \c std::terminate or an "implementation defined manner." Instead, we force
//!         you to handle potential exceptions by disallowing it.
template <typename T, std::size_t N>
constexpr void destroy_at(T (*array)[N]) noexcept
{
    static_assert(noexcept(carb::cpp::destroy_at(array[0])),
                  "destroy_at for array requires elements to have noexcept destructor");

    for (T& elem : *array)
    {
        carb::cpp::destroy_at(carb::cpp::addressof(elem));
    }
}

#if !CARB_HAS_CPP20 || defined DOXYGEN_BUILD

//! Construct a \c T in \a place using the provided \a args.
//!
//! \note
//! This differs from the C++20 definition by not being \c constexpr, since placement new is not \c constexpr before
//! C++20. When C++20 is enabled, this function disappears in favor of \c std::construct_at.
template <typename T, typename... TArgs>
T* construct_at(T* place, TArgs&&... args) noexcept
    CARB_NO_DOC((noexcept(::new (static_cast<void*>(place)) T(std::forward<TArgs>(args)...))))
{
    return ::new (static_cast<void*>(place)) T(std::forward<TArgs>(args)...);
}

#else

using std::construct_at;

#endif

} // namespace cpp
} // namespace carb
