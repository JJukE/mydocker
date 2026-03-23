// Copyright (c) 2018-2023, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief carb.logging Logger definition
#pragma once

#include "../Defines.h"

#include <cstdint>

// example-begin Logger
namespace carb
{
//! Namespace for logging interfaces and utilities
namespace logging
{

/**
 * Defines an extension interface for logging backends to register with the ILogging system.
 *
 * @see ILogging::addLogger
 * @see ILogging::removeLogger
 */
struct Logger
{
    /**
     * Handler for a formatted log message.
     *
     * This function is called by @ref ILogging if the Logger has
     * been registered via @ref ILogging::addLogger(), log level passes the threshold (for module or
     * globally if not set for module), and logging is enabled (for module or globally if not set
     * for module).
     *
     * @note As of 161.0 the Framework protects against recursively calling a Logger in the event that a Logger performs
     * an action that would recursively log. In these situations, the offending Logger will not receive the recursive
     * log message, but other Loggers will receive it.
     *
     * @note In some cases such as when certain mutexes are locked, log messages are deferred by the Framework in order
     * to prevent Logger objects from calling back into the Framework on the thread that has the mutex locked.
     *
     * @param logger The logger interface - can be nullptr if not used by handleMessage
     * @param source The source of the message in UTF8 character encoding - commonly plugin name
     * @param level The severity level of the message
     * @param filename The file name where the message originated from.
     * @param functionName The name of the function where the message originated from.
     * @param lineNumber The line number where the message originated from
     * @param message The formatted message in UTF8 character encoding
     *
     * @thread_safety this function will potentially be called simultaneously from multiple threads. The thread that
     * calls this function is not necessarily the thread that originated the log message, such as in the case of
     * asynchronous logging.
     */
    void(CARB_ABI* handleMessage)(Logger* logger,
                                  const char* source,
                                  int32_t level,
                                  const char* filename,
                                  const char* functionName,
                                  int lineNumber,
                                  const char* message);

    // NOTE: This interface, because it is inherited from, is CLOSED and may not have any additional functions added
    // without an ILogging major version increase.
};

} // namespace logging
} // namespace carb
// example-end
