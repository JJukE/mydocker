// Copyright (c) 2018-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief carb.logging interface definition
#pragma once

#include "../Interface.h"
#include "../IObject.h"

//! Latest version for carb::logging::ILogging
#define carb_logging_ILogging_latest CARB_HEXVERSION(1, 2)
#ifndef carb_logging_ILogging
//! The default current version for carb::logging::ILogging
#    define carb_logging_ILogging CARB_HEXVERSION(1, 1)
#endif

#include "StandardLogger2.h"

namespace carb
{
namespace logging
{


struct StandardLogger;
class StandardLogger2;
struct Logger;

/**
 * Defines a callback type for setting log level for every source.
 */
typedef void(CARB_ABI* SetLogLevelFn)(int32_t logLevel);

/**
 * Defines a log setting behavior.
 */
enum class LogSettingBehavior
{
    eInherit,
    eOverride
};

/**
 * Function pointer typedef for a logging function
 *
 * @note Typically this is not used directly. It exists solely for \ref g_carbLogFn and represents the type of
 *   \ref carb::logging::ILogging::log().
 * @param source The log source (typically client or plugin)
 * @param level The log level, such as \ref kLevelInfo
 * @param fileName The name of the source file that generated the log message (typically from `__FILE__`)
 * @param functionName The name of the source function that generated the log message (such as via
 *   \ref CARB_PRETTY_FUNCTION)
 * @param lineNumber The line number of \p filename that generated the log message (typically from `__LINE__`)
 * @param fmt A printf format string
 * @param ... varargs for \p fmt
 */
typedef void (*LogFn)(const char* source,
                      int32_t level,
                      const char* fileName,
                      const char* functionName,
                      int lineNumber,
                      const char* fmt,
                      ...);

// 'destroyStandardLoggerOld' is deprecated: Use StandardLogger2::release() instead
CARB_IGNOREWARNING_CLANG_WITH_PUSH("-Wdeprecated-declarations")

/**
 * Defines the log system that is associated with the Framework.
 *
 * This interface defines the log system, which is a singleton object. It can be used at any moment, including
 * before the startup of the Framework and after the Framework was shutdown. It allows a user to setup the logging
 * behavior in advance and allows the Framework to log during its initialization.
 *
 * Logger - is an interface for logging backend. ILogging can contain multiple Loggers and every message will be passed
 * to every logger. There is one implementation of a Logger provided - StandardLogger. It can log into file, console and
 * debug window. ILogging starts up with one instance of \ref StandardLogger2, which can be retrieved by calling
 * getDefaultLogger(). It is added by default, but can be removed with
 * `getLogging()->removeLogger(getLogging()->getDefaultLogger()->getLogger())`.
 *
 * ILogging supports multiple sources of log messages. Source is just a name to differentiate the origins of a message.
 * Every plugin, application and Framework itself are different sources. However user can add more custom sources if
 * needed.
 *
 * Use the logging macros from Log.h for all logging in applications and plugins.
 *
 * There are 2 log settings: log level (to control log severity threshold) and log enabled (to toggle whole logging).
 * Both of them can be set globally and per source.
 */
struct ILogging
{
    CARB_PLUGIN_INTERFACE_EX("carb::logging::ILogging", carb_logging_ILogging_latest, carb_logging_ILogging)

    /**
     * Logs a formatted message to the specified log source and log level.
     *
     * This API is used primarily by the CARB_LOG_XXXX macros.
     *
     * @param source The log source to log the message to.
     * @param level The level to log the message at.
     * @param fileName The file name to log to.
     * @param functionName The name of the function where the message originated from.
     * @param lineNumber The line number
     * @param fmt The print formatted message.
     * @param ... The variable arguments for the formatted message variables.
     */
    void(CARB_ABI* log)(const char* source,
                        int32_t level,
                        const char* fileName,
                        const char* functionName,
                        int lineNumber,
                        const char* fmt,
                        ...) CARB_PRINTF_FUNCTION(6, 7);

    /**
     * Sets global log level threshold. Messages below this threshold will be dropped.
     *
     * @param level The log level to set.
     */
    void(CARB_ABI* setLevelThreshold)(int32_t level);

    /**
     * Gets global log level threshold. Messages below this threshold will be dropped.
     *
     * @return Global log level.
     */
    int32_t(CARB_ABI* getLevelThreshold)();

    /**
     * Sets global log enabled setting.
     *
     * @param enabled Global log enabled setting.
     */
    void(CARB_ABI* setLogEnabled)(bool enabled);

    /**
     * If global log is enabled.
     *
     * @return Global log enabled.
     */
    bool(CARB_ABI* isLogEnabled)();

    /**
     * Sets log level threshold for the specified source. Messages below this threshold will be dropped.
     * Per source log settings can either inherit global or override it, it is configured with
     * LogSettingBehavior::eInherit and LogSettingBehavior::eOverride accordingly.
     *
     * @param source The source to set log level setting for. Must not be nullptr.
     * @param behavior The log setting behavior for the source.
     * @param level The log level to set. This param is ignored if behavior is set to LogSettingBehavior::eInherit.
     */
    void(CARB_ABI* setLevelThresholdForSource)(const char* source, LogSettingBehavior behavior, int32_t level);

    /**
     * Sets log enabled setting for the specified source.
     * Per source log settings can either inherit global or override it, it is configured with
     * LogSettingBehavior::eInherit and LogSettingBehavior::eOverride accordingly.
     *
     * @param source The source to set log enabled setting for. Must not be nullptr.
     * @param behavior The log setting behavior for the source.
     * @param enabled The log enabled setting. This param is ignored if behavior is set to LogSettingBehavior::eInherit.
     */
    void(CARB_ABI* setLogEnabledForSource)(const char* source, LogSettingBehavior behavior, bool enabled);

    /**
     * Reset all log settings set both globally and per source.
     * Log system resets to the defaults: log is enabled and log level is 'warn'.
     */
    void(CARB_ABI* reset)();

    /**
     * Adds a logger to the ILogging.
     * @see StandardLogger2::getLogger()
     * @param logger The logger to be added.
     */
    void(CARB_ABI* addLogger)(Logger* logger);

    /**
     * Removes the logger from the ILogging.
     * @see StandardLogger2::getLogger()
     * @param logger The logger to be removed.
     */
    void(CARB_ABI* removeLogger)(Logger* logger);

#if CARB_VERSION_ATLEAST(carb_logging_ILogging, 1, 1)
    /**
     * Accesses the default logger.
     *
     * This logger can be disabled by passing it to \ref removeLogger(). This logger is owned by the logging system and
     * cannot be destroyed by passing it to \ref destroyStandardLoggerOld().
     *
     * @rst
     * .. deprecated:: 156.0
     *     Use getDefaultLogger() instead
     * @endrst
     *
     * @returns the default logger.
     */
    CARB_DEPRECATED("Use getDefaultLogger() instead") StandardLogger*(CARB_ABI* getDefaultLoggerOld)();

    /**
     * Creates a new \ref StandardLogger instance.
     *
     * Use this method to create additional \ref StandardLogger instances. This can be useful when you want to log
     * to multiple output destinations but want different configuration for each. Use \ref addLogger() to make it
     * active. When finished with the \ref StandardLogger pass it to \ref destroyStandardLoggerOld().
     *
     * @rst
     * .. deprecated:: 156.0
     *     Use createStandardLogger() instead
     * @endrst
     *
     * @returns A new \ref StandardLogger.
     */
    CARB_DEPRECATED("Use createStandardLogger() instead") StandardLogger*(CARB_ABI* createStandardLoggerOld)();

    /**
     * Use this method to destroy a \ref StandardLogger that was created via \ref createStandardLoggerOld().
     *
     * @rst
     * .. deprecated:: 156.0
     *     The ``StandardLogger2`` that replaces ``StandardLogger`` can be destroyed by calling ``release()``.
     * @endrst
     *
     * @param logger The logger to be destroyed.
     */
    CARB_DEPRECATED("Use StandardLogger2::release() instead")
    void(CARB_ABI* destroyStandardLoggerOld)(StandardLogger* logger);
#else
    /**
     * Accesses the default logger.
     *
     * This logger can be disabled by passing it to \ref removeLogger(). This logger is owned by the logging system and
     * cannot be destroyed by passing it to \ref destroyStandardLogger().
     *
     * @returns the default logger.
     */
    StandardLogger*(CARB_ABI* getDefaultLogger)();

    /**
     * Creates a new \ref StandardLogger instance.
     *
     * Use this method to create additional \ref StandardLogger instances. This can be useful when you want to log
     * to multiple output destinations but want different configuration for each. Use \ref addLogger() to make it
     * active. When finished with the \ref StandardLogger pass it to \ref destroyStandardLogger().
     *
     * @returns A new \ref StandardLogger.
     */
    StandardLogger*(CARB_ABI* createStandardLogger)();

    /**
     * Use this method to destroy a \ref StandardLogger that was created via \ref createStandardLogger().
     *
     * @param logger The logger to be destroyed.
     */
    void(CARB_ABI* destroyStandardLogger)(StandardLogger* logger);
#endif

    /**
     * Register new logging source.
     * This function is mainly automatically used by plugins, application and the Framework itself to create
     * their own logging sources.
     *
     * Custom logging source can also be created if needed. It is the user's responsibility to call
     * ILogging::unregisterSource in that case.
     *
     * It is the source responsibility to track its log level. The reason is that it allows to filter out
     * logging before calling into the logging system, thus making the performance cost for the disabled logging
     * negligible. It is done by providing a callback `setLevelThreshold` to be called by ILogging when any setting
     * which influences this source is changed.
     *
     * @param source The source name. Must not be nullptr.
     * @param setLevelThreshold The callback to be called to update log level. ILogging::unregisterSource must be called
     * when the callback is no longer valid.
     */
    void(CARB_ABI* registerSource)(const char* source, SetLogLevelFn setLevelThreshold);

    /**
     * Unregister logging source.
     *
     * @param source The source name. Must not be nullptr.
     */
    void(CARB_ABI* unregisterSource)(const char* source);

    /**
     * Instructs the logging system to deliver all log messages to the Logger backends asynchronously. Async logging is
     * OFF by default.
     *
     * This causes log() calls to be buffered so that log() may return as quickly as possible. A background thread then
     * issues these buffered log messages to the registered Logger backend objects.
     *
     * @param logAsync True to use async logging; false to disable async logging.
     * @return true if was previously using async logging; false if was previously using synchronous logging.
     */
    bool(CARB_ABI* setLogAsync)(bool logAsync);

    /**
     * Returns whether the ILogging system is using async logging.
     * @return true if currently using async logging; false if currently using synchronous logging
     */
    bool(CARB_ABI* getLogAsync)();

    /**
     * When ILogging is in async mode, wait until all log messages have flushed out to the various loggers.
     */
    void(CARB_ABI* flushLogs)();

#if CARB_VERSION_ATLEAST(carb_logging_ILogging, 1, 1)
    /**
     * Retrieves the extended StandardLogger2 interface from an old \ref StandardLogger instance.
     * @param logger The logger to retrieve the instance from. If `nullptr` then `nullptr` will be returned.
     * @returns The \ref StandardLogger2 interface for \p logger, or `nullptr`.
     */
    StandardLogger2*(CARB_ABI* getStandardLogger2)(StandardLogger* logger);

    /**
     * Accesses the default logger.
     *
     * This logger can be disabled by passing the underlying logger (from \ref StandardLogger2::getLogger()) to
     * \ref removeLogger(). This logger is owned by the logging system and calling \ref StandardLogger2::release() has
     * no effect.
     *
     * @returns the default \ref StandardLogger2.
     */
    StandardLogger2*(CARB_ABI* getDefaultLogger)();

    //! @private
    StandardLogger2*(CARB_ABI* createStandardLoggerInternal)();

    /**
     * Creates a new \ref StandardLogger2 instance.
     *
     * Use this method to create additional \ref StandardLogger2 instances. This can be useful when you want to log
     * to multiple output destinations but want different configuration for each. Pass the value from
     * \ref StandardLogger2::getLogger() to \ref addLogger() to make it active. When finished with the
     * \ref StandardLogger2 call \ref StandardLogger2::release().
     *
     * @returns A new \ref StandardLogger2.
     */
    ObjectPtr<StandardLogger2> createStandardLogger()
    {
        using Ptr = ObjectPtr<StandardLogger2>;
        return Ptr(createStandardLoggerInternal(), Ptr::InitPolicy::eSteal);
    }
#endif
};
CARB_IGNOREWARNING_CLANG_POP

} // namespace logging
} // namespace carb
