// Copyright (c) 2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! \file
//! \brief Implementation detail
#pragma once

#include "../TypeTraits.h"

#include <string>

#if !CARB_HAS_CPP17
#    include <functional>
#endif

namespace carb
{
namespace cpp
{

/**
 * This struct provides implementations of a subset of the functions found in std::char_traits. It is used to provide
 * constexpr implementations of the functions because std::char_traits did not become constexpr until C++20. Currently
 * only the methods used by omni::string and omni::string_view are provided.
 */
template <typename CharT>
struct char_traits : public std::char_traits<CharT>
{
    /** Base type for a character value. */
    using char_type = typename std::char_traits<CharT>::char_type;
    /** Base type for an integer value. */
    using int_type = typename std::char_traits<CharT>::int_type;
    /** Base type for an offset value. */
    using off_type = typename std::char_traits<CharT>::off_type;
    /** Base type for a position in a string. */
    using pos_type = typename std::char_traits<CharT>::pos_type;
    /** Base type for a state value. */
    using state_type = typename std::char_traits<CharT>::state_type;
#if CARB_HAS_CPP20
    /** Base type for a character comparison result. */
    using comparison_category = typename std::char_traits<CharT>::comparison_category;
#endif

// For pre-C++20, define our own constexpr implementations
#if !(CARB_HAS_CPP20 && defined __cpp_lib_constexpr_string && __cpp_lib_constexpr_string >= 201907L) ||                \
    defined DOXYGEN_BUILD

    // Allow access to assign(char_type&, const char_type&)
    using std::char_traits<CharT>::assign;

    /**
     * Assigns \p count copies of \p c to \p dest.
     *
     * @returns \p dest.
     */
    static constexpr char_type* assign(char_type* dest, std::size_t count, const char_type c) noexcept
    {
        for (char_type* temp = dest; count > 0; --count, ++temp)
            *temp = c;
        return dest;
    }

    /**
     * Copies \p count characters from \p source to \p dest.
     *
     * This function performs correctly even if \p dest and \p source overlap.
     * @returns \p dest
     */
    static constexpr char_type* move(char_type* dest, const char_type* source, std::size_t count) noexcept
    {
        // Fallback basic constexpr implementation
        if (dest == source)
        {
            // do nothing
        }
        else if (std::less<const char_type*>{}(source, dest))
        {
            // If source is less than dest, copy from the back to the front to avoid overwriting characters in source if
            // source and dest overlap.
            for (std::size_t i = count; i > 0; --i)
            {
                dest[i - 1] = source[i - 1];
            }
        }
        else
        {
            // If source is greater than dest, copy from the the front to the back
            for (std::size_t i = 0; i < count; ++i)
            {
                dest[i] = source[i];
            }
        }
        return dest;
    }

    /**
     * Copies \p count characters from \p source to \p dest.
     *
     * Behavior of this function is undefined if \p dest and \p source overlap.
     * @returns \p dest
     */
    static constexpr char_type* copy(char_type* const dest, const char_type* const source, std::size_t const count) noexcept
    {
        // Fallback basic constexpr implementation
        for (std::size_t i = 0; i < count; ++i)
        {
            dest[i] = source[i];
        }
        return dest;
    }

#endif

#if !CARB_HAS_CPP17 || defined DOXYGEN_BUILD // For pre-C++17, define our own versions that are constexpr

    /**
     * Assigns \p c to \p dest.
     */
    static constexpr void assign(char_type& dest, const char_type& c) noexcept
    {
        dest = c;
    }

    /**
     * Lexicographically compares the first \p count characters of \p s1 and \p s2.
     *
     * @return Negative value if \p s1 is less than \p s2.
     *         ​0​ if \p s1 is equal to \p s2.
     *         Positive value if \p s1 is greater than \p s2.
     */
    [[nodiscard]] static constexpr int compare(const char_type* s1, const char_type* s2, std::size_t count) noexcept
    {
        for (; count > 0; --count, ++s1, ++s2)
            if (*s1 != *s2)
                return *s1 < *s2 ? -1 : +1;

        return 0;
    }

    /**
     * Searches the first \p count characters of \p s for the character \p ch.
     *
     * @return A pointer to the first character equal to \p ch, or \c nullptr if no such character exists.
     */
    static constexpr const char_type* find(const char_type* s, std::size_t count, char_type const ch) noexcept
    {
        for (; count > 0; --count, ++s)
            if (*s == ch)
                return s;

        return nullptr;
    }

#endif

    // vvv Carbonite extensions for security (we always override length())

    /**
     * Computes the length of a bounded array of char_type
     *
     * \note This function is a Carbonite extension. `std::char_traits::length` behaves differently in that the array
     *   must contain a `char_type(0)` character.
     * @tparam N The array length of \p s.
     * @param s A bounded array of \c char_type.
     * @returns The length of \p s which may equal but not exceed \c N.
     */
    template <size_t N>
    [[nodiscard]] static constexpr std::size_t length(const char_type (&s)[N]) noexcept
    {
        return length_s(s, N);
    }

    /**
     * Computes the length of \p s.
     *
     * \note This function is specialized to be more secure when used with literal strings and character arrays, in
     *   which case the maximum length returned will be the number of characters of the literal string or array.
     * @return The length of \p s.
     */
#ifdef DOXYGEN_BUILD
    [[nodiscard]] static constexpr std::size_t length(const char_type* s) noexcept;
#else
    template <class T,
              std::enable_if_t<!is_bounded_array_v<remove_cvref_t<T>> && std::is_convertible<T, const char_type*>::value,
                               bool> = false>
    [[nodiscard]] static constexpr std::size_t length(T&& p) noexcept
#endif
    {
#if CARB_HAS_CPP17
        return std::char_traits<char_type>::length(p);
#else
        const char_type* s = p;
        std::size_t result = 0;
        while (*s != char_type())
        {
            ++result;
            ++s;
        }

        return result;
#endif
    }

    /**
     * Computes the length of \p s bounded by \p count
     *
     * @return The length of \p s or \p count whichever is smaller.
     *
     * \note This is a Carbonite extension added for additional security.
     */
    [[nodiscard]] static constexpr std::size_t length_s(const char_type* const s, std::size_t const max_count) noexcept
    {
        for (std::size_t i = 0; i != max_count; ++i)
        {
            if (!s[i])
            {
                return i;
            }
        }
        return max_count;
    }

    /**
     * Computes the length of \p s bounded by \p N
     *
     * @return The length of \p s or \p N whichever is smaller.
     *
     * \note This is a Carbonite extension added for additional security.
     */
    template <std::size_t N>
    [[nodiscard]] static constexpr std::size_t length_s(const char_type (&s)[N]) noexcept
    {
        return length_s(s, N);
    }
};

} // namespace cpp
} // namespace carb
