// Copyright (c) 2020-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief omni.kit.app interface definition file
#pragma once

#include "../../carb/Interface.h"
#include "../../carb/events/IEvents.h"
#include "../../carb/extras/Timer.h"
#include "../../carb/logging/ILogging.h"
#include "../../carb/RString.h"
#include "../String.h"

namespace omni
{

namespace ext
{
class ExtensionManager;
}

//! Namespace for kit
namespace kit
{

class IRunLoopRunner;

//! \defgroup appstruct IApp structs
//! @{

/**
 * Application descriptor.
 * @see IApp::startup()
 */
struct AppDesc
{
    const char* carbAppName; //!< Carb application name. If `nullptr` it is derived from the executable filename.
    const char* carbAppPath; //!< Carb application path. If `nullptr` it is derived from the executable folder.
    int argc; //!< Number of values in `argv`
    char** argv; //!< Array of arguments
};

/**
 * Build information.
 * @see IApp::getBuildInfo()
 */
struct BuildInfo
{
    omni::string kitVersion; //!< Full kit version, e.g. `103.5+release.7032.aac30830.tc.windows-x86_64.release'
    omni::string kitVersionShort; //!< Short kit version, `major.minor` e.g. `103.5`
    omni::string kitVersionHash; //!< Git hash of kit build, 8 letters, e.g. `aac30830`

    //! Full kernel version, e.g. `103.5+release.7032.aac30830.tc.windows-x86_64.release'
    //! Note that this may be different than `kitVersion`
    omni::string kernelVersion;
};

/**
 * Platform information.
 * @see IApp::getPlatformInfo()
 */
struct PlatformInfo
{
    const char* config; //!< Build configuration e.g. "debug", "release"

    //! Platform descriptor e.g. "windows-x86_64", "linux-x86_64", "linux-aarch64"
    const char* platform;
    const char* pythonVersion; //!< Python version e.g. "cp310" for Python 3.10
};

/**
 * App information.
 * @see IApp::getAppInfo(), AppDesc
 */
struct AppInfo
{
    omni::string filename; //!< App filename. Name of a kit file or just 'kit'
    omni::string name; //!< App name. It is app/name setting if defined, otherwise same as `filename`
    omni::string version; //!< App version. Version in kit file or kit version
    omni::string versionShort; //!< Short app version, currently major.minor, e.g. `2021.3`

    //! Environment the application is running in, from `/app/environment/name` setting
    //! e.g. "teamcity", "launcher", "etm", "default", etc.
    omni::string environment;
    bool isExternal; //!< Is external (public) configuration
};

/**
 * Run Loop.
 *
 * A Run Loop is a collection of event streams that are pumped in order for each Run Loop iteration.
 * @see IApp::getRunLoop(), carb::events::IEventStream, carb::events::IEvents
 */
class RunLoop
{
public:
    carb::events::IEventStream* preUpdate; //!< Pre update events pushed every loop and stream is pumped.
    carb::events::IEventStream* update; //!< Update events pushed every loop and stream is pumped.
    carb::events::IEventStream* postUpdate; //!< Post update events pushed every loop and stream is pumped.

    //! Stream for extensions to push events to, pumped every loop after postUpdate.
    carb::events::IEventStream* messageBus;
};

//! @}

/**
 * How to handle passed arguments when restarting an app.
 * @see IApp::restart()
 */
enum class RestartArgsPolicy
{
    eAppend, //!< Append to existing args
    eReplace, //!< Replace existing args
};

// Few predefined run loop names. Serve as hints, doesn't have to use only those.

//! \defgroup apprunloop Run Loop Names
//! @{

//! Predefined Run Loop name
//!
//! Predefined names serve as hints for Run Loops but other names may also exist
constexpr char kRunLoopDefault[] = "main";
//! @copydoc kRunLoopDefault
constexpr char kRunLoopSimulation[] = "simulation";
//! @copydoc kRunLoopDefault
constexpr char kRunLoopRendering[] = "rendering";
//! @copydoc kRunLoopDefault
constexpr char kRunLoopUi[] = "ui";

//! @}

class IAppScripting;

//! \defgroup appevents App Events
//! @{

//! A shutdown event that is dispatched during the next update after postQuit is called.
//!
//! Once \ref IApp::postQuit() is called, the next \ref IApp::update() call will check for shutdown requests and start
//! the shutdown sequence. The first step of this sequence is that this event (`kGlobalEventPostQuit`) is dispatched.
//! During this event, any calls to \ref IApp::tryCancelShutdown() will abort this process (unless the posted quit
//! request is noncancellable). If no attempt to cancel the shutdown has been made, \ref kGlobalEventPreShutdown will be
//! dispatched.
//! @see IApp::postQuit(), IApp::tryCancelShutdown(), IApp::postUncancellableQuit()
//! \par Event Arguments
//! * "uncancellable" (bool) a `true` value indicates that the shutdown is not cancellable
const auto kGlobalEventPostQuit = carb::RString("omni.kit.app:post_quit");

//! A shutdown event that is dispatched to indicate the start of shutdown.
//! @see kGlobalEventPostQuit, IApp::postQuit(), IApp::tryCancelShutdown(),
//!   IApp::postUncancellableQuit()
//! \par Event Arguments
//! * None
const auto kGlobalEventPreShutdown = carb::RString("omni.kit.app:pre_shutdown");

//! An event that is dispatched at app startup time.
//!
//! This event is dispatched globally via \ref carb::eventdispatcher::IEventDispatcher immediately before
//! \ref IApp::startup() returns. This event is dispatched _before_ \ref kGlobalEventAppReady.
//! \par Event Arguments
//! * None
const auto kGlobalEventAppStarted = carb::RString("omni.kit.app:started");

//! An event that is dispatched when the application becomes ready.
//!
//! After application startup (see \ref kGlobalEventAppStarted), every call to \ref IApp::update() after the first call
//! will check for readiness. Readiness can be delayed with \ref IApp::delayAppReady() which must be called during each
//! main run loop iteration to prevent readiness. Once a run loop completes with no calls to \ref IApp::delayAppReady()
//! the application is considered "ready" and this event is dispatched via \ref carb::eventdispatcher::IEventDispatcher.
//! \par Event Arguments
//! * None
const auto kGlobalEventAppReady = carb::RString("omni.kit.app:ready");

/**
 * Legacy version of \ref kGlobalEventPostQuit.
 *
 * For backwards compatibility, this event is dispatched to the local legacy event stream obtainable with
 * \ref IApp::getShutdownEventStream().
 * \par Event Arguments
 * * "uncancellable" (bool) a `true` value indicates that the shutdown is not cancellable
 */
constexpr carb::events::EventType kPostQuitEventType = 0;

/**
 * Legacy version of \ref kGlobalEventPreShutdown.
 *
 * For backwards compatibility, this event is dispatched to the local legacy event stream obtainable with
 * \ref IApp::getShutdownEventStream().
 * \par Event Arguments
 * * None
 */
constexpr carb::events::EventType kPreShutdownEventType = 1;

/**
 * Legacy version of \ref kGlobalEventAppStarted.
 *
 * For backwards compatibility, this event is dispatched to the local legacy event stream obtainable with
 * \ref IApp::getStartupEventStream().
 * \par Event Arguments
 * * None
 */
const carb::events::EventType kEventAppStarted = CARB_EVENTS_TYPE_FROM_STR("APP_STARTED");

/**
 * Legacy version of \ref kGlobalEventAppReady.
 *
 * For backwards compatibility, this event is dispatched to the local legacy event stream obtainable with
 * \ref IApp::getStartupEventStream().
 * \par Event Arguments
 * * None
 */
const carb::events::EventType kEventAppReady = CARB_EVENTS_TYPE_FROM_STR("APP_READY");


//! @}

/**
 * Main Kit Application plugin.
 *
 * It runs all Kit extensions and contains necessary pieces to make them work together: settings, events, python, update
 * loop. Generally an application will bootstrap *omni.kit.app* by doing the following steps:
 * 1. Initialize the Carbonite \ref carb::Framework
 * 2. Load the *omni.kit.app* plugin with \ref carb::Framework::loadPlugin()
 * 3. Call \ref IApp::run() which will not return until the application shuts down
 * 4. Shut down the Carbonite \ref carb::Framework
 *
 * The *kit* executable is provided by Carbonite for this purpose. It is a thin bootstrap executable around starting
 * *omni.kit.app*.
 *
 * This interface can be acquired from Carbonite with \ref carb::Framework::acquireInterface().
 */
class IApp
{
public:
    CARB_PLUGIN_INTERFACE("omni::kit::IApp", 1, 3);

    //////////// main API ////////////

    /**
     * Starts up the application.
     * @note Typically this function is not called directly. Instead use \ref run().
     * @param desc the \ref AppDesc that describes the application
     */
    virtual void startup(const AppDesc& desc) = 0;

    /**
     * Performs one application update loop.
     * @note Typically this function is not called directly. Instead use \ref run().
     */
    virtual void update() = 0;

    /**
     * Checks whether the application is still running.
     * @retval true The application is still running
     * @retval false The application has requested to quit and has shut down
     */
    virtual bool isRunning() = 0;

    /**
     * Shuts down the application.
     *
     * This function should be called once \ref isRunning() returns false.
     * @returns The application exit code (0 indicates success)
     */
    virtual int shutdown() = 0;

    /**
     * A helper function that starts, continually updates, and shuts down the application.
     *
     * This function essentially performs:
     * ```cpp
     *    startup(desc);
     *    while (isRunning())
     *        update();
     *    return shutdown()
     * ```
     * @see startup(), isRunning(), update(), shutdown()
     * @param desc the \ref AppDesc that describes the application
     * @returns The application exit code (0 indicates success)
     */
    int run(const AppDesc& desc);

    //////////// extensions API ////////////

    /**
     * Requests application exit.
     *
     * @note This does not immediately exit the application. The next call to \ref update() will evaluate the shutdown
     * process.
     * @note This shutdown request is cancellable by calling \ref tryCancelShutdown().
     * @see kGlobalEventPostQuit, postUncancellableQuit()
     * @param returnCode The requested return code that will be returned by \ref shutdown() once the application exits.
     */
    virtual void postQuit(int returnCode = 0) = 0;

    /**
     * Access the log event stream.
     *
     * Accesses a \ref carb::events::IEventStream that receives every "error" (or higher) message.
     * \par Event Arguments
     * * Event ID is always 0
     * * "source" - The log source, typically plugin name (string)
     * * "level" - The \ref loglevel (integer)
     * * "filename" - The source file name, such as by `__FILE__` (string)
     * * "lineNumber" - The line in the source file, such as by `__LINE__` (integer)
     * * "functionName" - The name of the function doing the logging, such as by \ref CARB_PRETTY_FUNCTION (string)
     * * "message" - The log message
     * @returns a \ref carb::events::IEventStream that receives every error (or higher) log message
     * @see carb::events::IEventStream, carb::events::IEvents
     */
    virtual carb::events::IEventStream* getLogEventStream() = 0;

    /**
     * Replays recorded log messages for the specified target.
     *
     * This function will call \ref carb::logging::Logger::handleMessage() for each log message that has been retained.
     * @param target The \ref carb::logging::Logger instance to replay messages for
     */
    virtual void replayLogMessages(carb::logging::Logger* target) = 0;

    /**
     * Toggles log message recording.
     *
     * By default, \ref startup() enables log message recording. Recorded log messages can be replayed on a
     * \ref carb::logging::Logger with \ref replayLogMessages().
     *
     * @param logMessageRecordingEnabled if `true`, future log messages are recorded; if `false` the log messages are
     *   not recorded
     */
    virtual void toggleLogMessageRecording(bool logMessageRecordingEnabled) = 0;

    /**
     * Access and/or creates a Run Loop by name.
     *
     * @param name The Run Loop to create or find; `nullptr` is interpreted as \ref kRunLoopDefault
     * @returns the specified \ref RunLoop instance
     */
    virtual RunLoop* getRunLoop(const char* name) = 0;

    /**
     * Tests whether the specified run loop exists.
     * @param name The Run Loop to find; `nullptr` is interpreted as \ref kRunLoopDefault
     * @retval true The specified \p name has a valid \ref RunLoop
     * @retval false The specified \p name does not have a valid \ref RunLoop
     */
    virtual bool isRunLoopAlive(const char* name) = 0;

    /**
     * Requests the run loop to terminate
     *
     * @note This calls \ref IRunLoopRunner::onRemoveRunLoop() but the \ref RunLoop itself is not actually removed.
     * @param name The Run Loop to find; `nullptr` is interpreted as \ref kRunLoopDefault
     * @param block Passed to \ref IRunLoopRunner::onRemoveRunLoop()
     */
    virtual void terminateRunLoop(const char* name, bool block) = 0;

    /**
     * Helper function to access a Run Loop event stream.
     * @see RunLoop
     * @param runLoopName The name of the Run Loop; `nullptr` is interpreted as \ref kRunLoopDefault
     * @returns A \ref carb::events::IEventStream instance
     */
    carb::events::IEventStream* getPreUpdateEventStream(const char* runLoopName = kRunLoopDefault)
    {
        return getRunLoop(runLoopName)->preUpdate;
    }

    //! @copydoc getPreUpdateEventStream()
    carb::events::IEventStream* getUpdateEventStream(const char* runLoopName = kRunLoopDefault)
    {
        return getRunLoop(runLoopName)->update;
    }

    //! @copydoc getPreUpdateEventStream()
    carb::events::IEventStream* getPostUpdateEventStream(const char* runLoopName = kRunLoopDefault)
    {
        return getRunLoop(runLoopName)->postUpdate;
    }

    //! @copydoc getPreUpdateEventStream()
    carb::events::IEventStream* getMessageBusEventStream(const char* runLoopName = kRunLoopDefault)
    {
        return getRunLoop(runLoopName)->messageBus;
    }

    /**
     * Set particular Run Loop runner implementation. This function must be called only once during app startup. If
     * no `IRunLoopRunner` was set application will quit. If `IRunLoopRunner` is set it becomes responsible for spinning
     * run loops. Application will call in functions on `IRunLoopRunner` interface to communicate the intent.
     */
    /**
     * Sets the current Run Loop Runner instance
     *
     * @note The \ref IRunLoopRunner instance is retained by `*this` until destruction when the plugin is unloaded. To
     *   un-set the \ref IRunLoopRunner instance, call this function with `nullptr`.
     *
     * @see RunLoop, IRunLoopRunner
     * @param runner The \ref IRunLoopRunner instance that will receive notifications about Run Loop events; `nullptr`
     *   to un-set the \ref IRunLoopRunner instance.
     */
    virtual void setRunLoopRunner(IRunLoopRunner* runner) = 0;

    /**
     * Accesses the Extension Manager.
     * @returns a pointer to the \ref omni::ext::ExtensionManager instance
     */
    virtual omni::ext::ExtensionManager* getExtensionManager() = 0;

    /**
     * Accesses the Application Scripting Interface.
     * @returns a pointer to the \ref IAppScripting instance
     */
    virtual IAppScripting* getPythonScripting() = 0;

    /**
     * Access the build version string.
     *
     * This is effectively `getBuildInfo().kitVersion.c_str()`.
     * @see getBuildInfo(), BuildInfo
     * @returns The build version string
     */
    virtual const char* getBuildVersion() = 0;

    /**
     * Reports whether the application is running 'debug' configuration.
     *
     * @note This is based on whether the plugin exporting the \ref IApp interface (typically *omni.kit.app.plugin*)
     *   that is currently running was built as a debug configuration.
     *
     * A debug configuration is one where \ref CARB_DEBUG was non-zero at compile time.
     *
     * @retval true \ref CARB_DEBUG was non-zero at compile-time
     * @retval false \ref CARB_DEBUG was zero at compile-time (release build)
     */
    virtual bool isDebugBuild() = 0;

    /**
     * Retrieves information about the currently running platform.
     * @returns a @ref PlatformInfo struct describing the currently running platform.
     */
    virtual PlatformInfo getPlatformInfo() = 0;

    /**
     * Returns the time elapsed since startup.
     *
     * This function returns the fractional time that has elapsed since \ref startup() was called, in the requested
     *   scale units.
     * @param timeScale Desired time scale for the returned time interval (seconds, milliseconds, etc.)
     * @return time passed since \ref startup() was most recently called
     */
    virtual double getTimeSinceStart(carb::extras::Timer::Scale timeScale = carb::extras::Timer::Scale::eMilliseconds) = 0;

    /**
     * Returns the time elapsed since update.
     *
     * This function returns the fractional time that has elapsed since \ref update() was called (or if \ref update()
     *   has not yet been called, since \ref startup() was called), in milliseconds.
     * @return time passed since \ref update() or \ref startup() (whichever is lower) in milliseconds
     */
    virtual double getLastUpdateTime() = 0;

    /**
     * Returns the current update number.
     *
     * This value is initialized to zero at \ref startup(). Every time the \ref kRunLoopDefault \ref RunLoop::postUpdate
     *   \ref carb::events::IEventStream is pumped, this value is incremented by one.
     * @returns the current update number (number of times that the \ref kRunLoopDefault \ref RunLoop has updated)
     */
    virtual uint32_t getUpdateNumber() = 0;

    /**
     * Formats and prints a log message.
     *
     * The given log \p message is prepended with the time since start (as via \ref getTimeSinceStart()) and first sent
     * to `CARB_LOG_INFO()`, and then outputted to `stdout` if `/app/enableStdoutOutput` is true (defaults to true).
     * @param message The message to log and print to `stdout`
     */
    virtual void printAndLog(const char* message) = 0;

    /**
     * Accesses the shutdown event stream.
     * @warning This function is deprecated in favor of Events 2.0 using *carb.eventdispatcher.plugin* with named
     *   events.
     * @see carb::events::IEvents, kPostQuitEventType, kPreShutdownEventType
     * @returns The \ref carb::events::IEventStream that contains shutdown events
     */
    /*CARB_DEPRECATED("Use carb::eventdispatcher")*/ virtual carb::events::IEventStream* getShutdownEventStream() = 0;

    /**
     * Attempts to cancel a shutdown in progress.
     * @param reason The reason to interrupt shutdown; `nullptr` is interpreted as an empty string
     * @retval true The shutdown process was interrupted successfully
     * @retval false A noncancellable shutdown is in progress and cannot be interrupted
     */
    virtual bool tryCancelShutdown(const char* reason) = 0;

    /**
     * Requests application exit that cannot be cancelled.
     *
     * @note This does not immediately exit the application. The next call to \ref update() will evaluate the shutdown
     * process.
     * @note This shutdown request is **not** cancellable by calling \ref tryCancelShutdown().
     * @see kPostQuitEventType, kGlobalEventPostQuit, postQuit()
     * @param returnCode The requested return code that will be returned by \ref shutdown() once the application exits.
     */
    virtual void postUncancellableQuit(int returnCode) = 0;

    /**
     * Accesses the startup event stream.
     * @warning This function is deprecated in favor of Events 2.0 using *carb.eventdispatcher.plugin* with named
     *   events.
     * @see carb::events::IEvents, kEventAppStarted, kEventAppReady
     * @returns The \ref carb::events::IEventStream that contains startup events
     */
    /*CARB_DEPRECATED("Use carb::eventdispatcher")*/ virtual carb::events::IEventStream* getStartupEventStream() = 0;

    /**
     * Checks whether the app is in a ready state.
     * @retval true The app is in a ready state; \ref kGlobalEventAppReady has been previously dispatched
     * @retval false The app is not yet ready
     */
    virtual bool isAppReady() = 0;

    /**
     * Instructs the app to delay setting the ready state.
     *
     * @note This function is only useful prior to achieving the ready state. Calling it after \ref isAppReady() reports
     *   `true` has no effect.
     *
     * Every \ref update() call resets the delay request; therefore to continue delaying the ready state this function
     * must be called during every update cycle.
     * @param requesterName (required) The requester who is delaying the ready state, used for logging
     */
    virtual void delayAppReady(const char* requesterName) = 0;

    /**
     * Access information about how the plugin was built.
     * @returns A reference to \ref BuildInfo describing version and hash information
     */
    virtual const BuildInfo& getBuildInfo() = 0;

    /**
     * Access information about the running application.
     * @returns A reference to \ref AppInfo describing how the application was started and configured
     */
    virtual const AppInfo& getAppInfo() = 0;

    /**
     * Restarts the application.
     *
     * Quits the current process and starts a new process. The command line arguments can be inherited, appended or
     * replaced.
     *
     * To quit a regular \ref postQuit() is used, unless \p uncancellable is set to `true` in which case
     * \ref postUncancellableQuit() is called.
     *
     * @param args Array of command line arguments for a new process; may be `nullptr`
     * @param argCount Number of command line arguments in \p args
     * @param argsPolicy A \ref RestartArgsPolicy value that controls replacing existing args with \p args, or appending
     *   \p args to the existing args
     * @param uncancellable If `true` \ref postUncancellableQuit() is used to quit the current process; `false` will
     *   instead of postQuit().
     */
    virtual void restart(const char* const* args = nullptr,
                         size_t argCount = 0,
                         RestartArgsPolicy argsPolicy = RestartArgsPolicy::eAppend,
                         bool uncancellable = false) = 0;
};


//! \addtogroup appevents
//! @{

//! An event that is dispatched when a scripting command is issued (update).
//!
//! @note This event is dispatched during \ref IApp::update; to receive the event immediately when it happens, observe
//!   \ref kGlobalEventScriptingCommandImmediate.
//! @see IAppScripting::executeString(), IAppScripting::executeFile()
//! \par Event Arguments
//! * "text" - A human-readable text block (string)
const auto kGlobalEventScriptingCommand = carb::RString("omni.kit.app:script_command");

//! An event that is dispatched when a scripting command is issued (immediate).
//!
//! @note This event is dispatched immediately as it happens; to receive the event during the next \ref IApp::update,
//!   observe \ref kGlobalEventScriptingCommand.
//! @see IAppScripting::executeString(), IAppScripting::executeFile()
//! \par Event Arguments
//! * "text" - A human-readable text block (string)
const auto kGlobalEventScriptingCommandImmediate = carb::RString("omni.kit.app:script_command:immediate");

//! An event that is dispatched when python prints to stdout (update).
//!
//! @note This event is dispatched during \ref IApp::update; to receive the event immediately when it happens, observe
//!   \ref kGlobalEventScriptingStdOutImmediate.
//! @see IAppScripting::executeString(), IAppScripting::executeFile()
//! \par Event Arguments
//! * "text" - A human-readable text block (string)
const auto kGlobalEventScriptingStdOut = carb::RString("omni.kit.app:script_stdout");

//! An event that is dispatched when python prints to stdout (immediate).
//!
//! @note This event is dispatched immediately as it happens; to receive the event during the next \ref IApp::update,
//!   observe \ref kGlobalEventScriptingStdOut.
//! @see IAppScripting::executeString(), IAppScripting::executeFile()
//! \par Event Arguments
//! * "text" - A human-readable text block (string)
const auto kGlobalEventScriptingStdOutImmediate = carb::RString("omni.kit.app:script_stdout:immediate");

//! An event that is dispatched when python prints to stderr (update).
//!
//! @note This event is dispatched during \ref IApp::update; to receive the event immediately when it happens, observe
//!   \ref kGlobalEventScriptingStdErrImmediate.
//! @see IAppScripting::executeString(), IAppScripting::executeFile()
//! \par Event Arguments
//! * "text" - A human-readable text block (string)
const auto kGlobalEventScriptingStdErr = carb::RString("omni.kit.app:script_stderr");

//! An event that is dispatched when python prints to stderr (immediate).
//!
//! @note This event is dispatched immediately as it happens; to receive the event during the next \ref IApp::update,
//!   observe \ref kGlobalEventScriptingStdErr.
//! @see IAppScripting::executeString(), IAppScripting::executeFile()
//! \par Event Arguments
//! * "text" - A human-readable text block (string)
const auto kGlobalEventScriptingStdErrImmediate = carb::RString("omni.kit.app:script_stderr:immediate");

/**
 * Legacy version of \ref kGlobalEventScriptingCommand.
 *
 * For backwards compatibility, this event is dispatched to the local legacy event stream obtainable with
 * \ref IAppScripting::getEventStream().
 * \par Arguments
 * * "text" (string) - A human-readable text block.
 */
const carb::events::EventType kScriptingEventCommand = 0;

//! \copydoc kScriptingEventCommand
//! \brief Legacy version of \ref kGlobalEventScriptingStdOut.
const carb::events::EventType kScriptingEventStdOut = 1;

//! \copydoc kScriptingEventCommand
//! \brief Legacy version of \ref kGlobalEventScriptingStdErr.
const carb::events::EventType kScriptingEventStdErr = 2;

//! @}

/**
 * Scripting Engine interface.
 */
class IAppScripting
{
public:
    /**
     * Run script from a string.
     *
     * A \ref kGlobalEventScriptingCommand event is always dispatched before executing the script.
     * @param str Content of the script to execute
     * @param commandName (optional) A command name that will logged for multi-line scripts
     * @param executeAsFile If `true`, \p str is first written to a temporary file which is then executed. The file is
     *   not removed which allows inspecting the script at a later point.
     * @retval true if execution was successful
     * @retval false if an error occurs (\ref kGlobalEventScriptingStdErr was dispatched with the error message)
     */
    virtual bool executeString(const char* str, const char* commandName = nullptr, bool executeAsFile = false) = 0;

    /**
     * Run script from a file.
     *
     * A \ref kGlobalEventScriptingCommand event is always dispatched before executing the script.
     * @param path The path to the script file. May be a file name that exists in folders that have been added to the
     *   search path with \ref addSearchScriptFolder(). A ".py" suffix is optional.
     * @param args An optional array of string arguments to pass to the script file
     * @param argCount The number of arguments in the \p args array
     * @retval true if execution was successful
     * @retval false if the file was not found (an error is logged), or the script could not be loaded (an error is
     *   logged), or execution failed (\ref kGlobalEventScriptingStdErr was dispatched with the error message)
     */
    virtual bool executeFile(const char* path, const char* const* args, size_t argCount) = 0;

    /**
     * Adds a folder path that will be searched for scripts.
     *
     * Calls to \ref executeFile() will search the paths added in the order that they were added.
     * @see removeSearchScriptFolder()
     * @note If the given \p path does not exist it will be created.
     * @param path A relative or absolute path. If the path does not exist it will be created.
     * @retval true The given \p path was added to the list of search paths
     * @retval false The given \p path already exists in the list of search paths
     */
    virtual bool addSearchScriptFolder(const char* path) = 0;

    /**
     * Removes a folder from the list of folders that will be searched for scripts.
     *
     * @see addSearchScriptFolder()
     * @param path A relative or absolute path which was previously passed to \ref addSearchScriptFolder()
     * @retval true The given \p path was found and removed from the list of script search folders
     * @retval false The given \p path was not found in the list of script search folders
     */
    virtual bool removeSearchScriptFolder(const char* path) = 0;

    /**
     * Access the scripting event stream.
     * @warning This function is deprecated in favor of Events 2.0 using *carb.eventdispatcher.plugin* with named
     *   events (i.e. \ref kGlobalEventScriptingCommand, \ref kGlobalEventScriptingStdOut, \ref
     * kGlobalEventScriptingStdErr).
     * @see kScriptingEventCommand, kScriptingEventStdOut, kScriptingEventStdErr
     * @returns a \ref carb::events::IEventStream that receives scripting events
     */
    /*CARB_DEPRECATED("Use carb::eventdispatcher")*/ virtual carb::events::IEventStream* getEventStream() = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Inline Functions                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline int IApp::run(const AppDesc& desc)
{
    this->startup(desc);
    while (this->isRunning())
    {
        this->update();
    }
    return this->shutdown();
}


} // namespace kit
} // namespace omni
