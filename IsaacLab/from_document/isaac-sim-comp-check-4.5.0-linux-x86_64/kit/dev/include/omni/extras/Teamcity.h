// Copyright (c) 2023-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//
/** @file
 *  @brief Teamcity helpers
 */
#pragma once

#include <string>

namespace omni
{
namespace extras
{

/** Escape a string so that it can be included safely in a Teamcity message.
 *  @param[in] str The string to escape.
 *  @returns The escaped form of @p str.
 */
inline std::string escapeForTeamcityMessage(const std::string& str)
{
    std::string out;
    out.reserve(str.size());
    for (char c : str)
    {
        switch (c)
        {
            case '|':
                out += "||";
                break;
            case '\'':
                out += "|'";
                break;
            case '\n':
                out += "|n";
                break;
            case '\r':
                out += "|r";
                break;
            case '[':
                out += "|[";
                break;
            case ']':
                out += "|]";
                break;
            default:
                out += c;
                break;
        }
    }

    return out;
}


} // namespace extras
} // namespace omni
