__all__ = [
    "APP_SCRIPTING_EVENT_COMMAND",
    "APP_SCRIPTING_EVENT_STDOUT",
    "APP_SCRIPTING_EVENT_STDERR",
    "RUN_LOOP_DEFAULT",
    "RUN_LOOP_SIMULATION",
    "RUN_LOOP_RENDERING",
    "RUN_LOOP_UI",
    "POST_QUIT_EVENT_TYPE",
    "PRE_SHUTDOWN_EVENT_TYPE",
    "EVENT_APP_STARTED",
    "EVENT_APP_READY",
    "EVENT_ORDER_DEFAULT",
    "UPDATE_ORDER_PYTHON_ASYNC_FUTURE_BEGIN_UPDATE",
    "UPDATE_ORDER_PYTHON_ASYNC_FUTURE_END_UPDATE",
    "UPDATE_ORDER_PYTHON_EXEC_BEGIN_UPDATE",
    "UPDATE_ORDER_PYTHON_EXEC_END_UPDATE",
    "UPDATE_ORDER_USD",
    "UPDATE_ORDER_UNSPECIFIED",
    "UPDATE_ORDER_FABRIC_FLUSH",
    "UPDATE_ORDER_HYDRA_RENDER",
    "UPDATE_ORDER_UI_RENDER",
    "POST_UPDATE_ORDER_PYTHON_ASYNC_FUTURE",
    "POST_UPDATE_ORDER_PYTHON_EXEC",
    "IApp",
    "acquire_app_interface",
    "IAppScripting",
    "crash",
    "get_app_interface",
    "get_app",
    "telemetry_helpers",
    "send_telemetry_event",
    "log_deprecation",
    "deprecated",
    "SettingChangeSubscription",
]

import collections
import logging
import os
import sys
import traceback
from functools import lru_cache
from typing import Callable

import carb
import carb.eventdispatcher
import carb.profiler
import carb.settings
import carb.tokens
import omni.ext

from .._app import *
from .app_iface import *
from .telemetry_helpers import *

########################################################################################################################
# Various python utils on top of Kit Core
# TODO: maybe move to omni.kit.utils ?
########################################################################################################################


@lru_cache()
def _is_developer_warnings_enabled():
    return bool(carb.settings.get_settings().get("/app/enableDeveloperWarnings"))


def log_deprecation(message: str):
    """Log deprecation message."""
    if _is_developer_warnings_enabled():
        carb.log_warn(message)
    else:
        carb.log_info(message)


def deprecated(message=""):
    """Decorator which can be used to mark functions or classes as deprecated. It will result in warn log when the function is used or a class is instantiated."""

    def deprecated_decorator(obj):
        if isinstance(obj, type):  # Check if it's a class
            # Create a wrapper class to preserve the class type
            class WrappedClass(obj):
                def __init__(self, *args, **kwargs):
                    log_deprecation(
                        "`{}` is deprecated. {}. Callstack: {}".format(
                            obj.__name__, message, "".join(traceback.format_stack(limit=4)[:-1])
                        )
                    )
                    super().__init__(*args, **kwargs)

            return WrappedClass
        else:  # a function

            def new_func(*args, **kwargs):
                log_deprecation(
                    "`{}` is deprecated. {}. Callstack: {}".format(
                        obj.__name__, message, traceback.format_stack(limit=3)
                    )
                )
                return obj(*args, **kwargs)

            new_func.__name__ = obj.__name__
            new_func.__doc__ = obj.__doc__
            new_func.__dict__.update(obj.__dict__)
            return new_func

    return deprecated_decorator


class SettingChangeSubscription:
    """
    Setting change subscription wrapper to make it scoped (auto unsubscribe on del)
    """

    def __init__(self, path: str, on_change: Callable):
        setting_py = carb.settings.get_settings().get(path)
        if not isinstance(setting_py, str) and isinstance(setting_py, collections.abc.Sequence):

            def on_change_wrapper(tree_item, changed_item, event_type):
                on_change(tree_item, event_type)

            self._setting_sub = carb.settings.get_settings().subscribe_to_tree_change_events(path, on_change_wrapper)
        else:
            self._setting_sub = carb.settings.get_settings().subscribe_to_node_change_events(path, on_change)

    def __del__(self):
        carb.settings.get_settings().unsubscribe_to_change_events(self._setting_sub)


class StreamInterceptor:
    def __init__(self, mq, is_stdout, log_std):
        self._mq = mq
        self._is_stdout = is_stdout
        self._name = "stdout" if is_stdout else "stderr"
        self._out = sys.__stdout__ if is_stdout else sys.__stderr__
        self._event_name = f"omni.kit.app:script_{self._name}"
        self._log_std = log_std
        self._buf = []

    def write(self, msg):
        if msg.endswith("\n"):
            self._buf.append(msg[:-1])
            text = "".join(self._buf)
            if self._log_std:
                func = carb.log_info if self._is_stdout else carb.log_error
                func(f"[py {self._name}]: {text}")
            payload = {"text": text}
            carb.eventdispatcher.get_eventdispatcher().dispatch_event(f"{self._event_name}:immediate", payload=payload)
            self._mq.push(self._event_name, payload=payload)
            self._buf = []
        else:
            self._buf.append(msg)

        if self._out and (self._is_stdout or not self._log_std):
            # Only forward to stderr if we don't log. Logging will forward to stderr anyway and we don't want double output.
            self._out.write(msg)

    def flush(self):
        if self._out:
            self._out.flush()

    def isatty(self):
        if self._out:
            return self._out.isatty()
        return False

    def __getattr__(self, attr):
        if attr in self.__dict__:
            return getattr(self, attr)
        return getattr(self._out, attr)


_logLevelToCarbLogLevel = {
    logging.CRITICAL: carb.logging.LEVEL_FATAL,
    logging.ERROR: carb.logging.LEVEL_ERROR,
    logging.WARNING: carb.logging.LEVEL_WARN,
    logging.INFO: carb.logging.LEVEL_INFO,
    logging.DEBUG: carb.logging.LEVEL_VERBOSE,
    logging.NOTSET: carb.logging.LEVEL_VERBOSE,
}


class _CarbLogHandler(logging.StreamHandler):
    """
    A handler class to redirect all python logs into carb logger.
    """

    def emit(self, record):
        try:
            msg = self.format(record)
            level = _logLevelToCarbLogLevel.get(record.levelno, carb.logging.LEVEL_VERBOSE)
            carb.log(record.name, level, record.filename, record.funcName, record.lineno, msg)
        except Exception:
            self.handleError(record)


def _set_default_and_get_setting(path, default=None):
    settings = carb.settings.get_settings()
    settings.set_default(path, default)
    return settings.get(path)


_app_ready_sub = None


def _gc_profile_callback(phase, info):
    if phase == "start":
        carb.profiler.begin(1, "Py::gc.collect")
    else:
        carb.profiler.end(1)


def _toggle_gc_profiling(enable: bool):
    import gc

    is_enabled = _gc_profile_callback in gc.callbacks
    if enable != is_enabled:
        if enable:
            gc.callbacks.append(_gc_profile_callback)
        else:
            gc.callbacks.remove(_gc_profile_callback)


def _setup_gc():
    # Instrument GC collect calls
    if _set_default_and_get_setting("/app/python/enableGCProfiling", True):
        _toggle_gc_profiling(True)

    # Disable GC during startup until "app ready". That seems to make startup faster and delay GC hickups. Reduces total
    # time spent in gc.collect by 20-30% (in Create startup measurements). Also that makes extension startup time more
    # predictable.
    if _set_default_and_get_setting("/app/python/disableGCDuringStartup", True):
        import gc

        carb.log_info("python GC: gc.disable()")
        gc.disable()

        global _app_ready_sub

        def on_app_ready(e):
            global _app_ready_sub
            _app_ready_sub = None
            carb.log_info("python GC: gc.enable()")
            gc.enable()

        _app_ready_sub = carb.eventdispatcher.get_eventdispatcher().observe_event(
            observer_name="omni.kit.app GC reenable", event_name="app.ready", on_event=on_app_ready
        )


def _startup_kit_scripting():
    """Internal method to be called by IApp.

    Setups stdout and stderr interceptors to push them into IApp's scripting event stream.
    """

    # setup python's GC
    _setup_gc()

    tokens = carb.tokens.get_tokens_interface()
    try:
        python_path = tokens.resolve("${kit}/python")
        if os.path.exists(python_path):
            os.add_dll_directory(python_path)
    except AttributeError:
        pass

    # Setup log handler to redirect all python native logging into Carbonite. Set log level to DEBUG to
    # get all messages through and allow Carbonite to sort it out.
    logging_handler = _CarbLogHandler()
    # logging_handler.setFormatter(logging.Formatter("[%(pathname)s:%(lineno)d] %(message)s"))
    logging.getLogger().addHandler(logging_handler)
    logging.getLogger().setLevel(logging.DEBUG)

    settings = carb.settings.get_settings()
    if _set_default_and_get_setting("/app/python/interceptSysStdOutput", True):
        log_std = settings.get("/app/python/logSysStdOutput")
        mq = carb.eventdispatcher.get_messagequeue_factory().get_message_queue("omni.kit.app_scripting.mq")
        sys.stdout = StreamInterceptor(mq, True, log_std)
        sys.stderr = StreamInterceptor(mq, False, log_std)

    # Extra sys.path?
    extra_paths = settings.get("/app/python/extraPaths")
    for p in extra_paths:
        p = os.path.normpath(tokens.resolve(p))
        sys.path.append(p)
        carb.log_info(f"added sys.path: {p}")

    # Patch sys.exit such that Kit is guaranteed to be notified and can set its internal
    # returnCode accordingly. This can be necessary when running Kit from python directly,
    # where we do not otherwise have control over the shutdown process.
    if settings.get("/app/fastShutdown") and _set_default_and_get_setting("/app/python/interceptSysExit", True):
        sys._exit_orig = sys.exit

        def kit_exit(status=None):
            import omni.kit.app

            app = omni.kit.app.get_app()
            if app:
                app.post_quit(status)
            sys._exit_orig(status)

        sys.exit = kit_exit


def _shutdown_kit_scripting():
    global _app_ready_sub
    _app_ready_sub = None

    _toggle_gc_profiling(False)

    if isinstance(sys.stdout, StreamInterceptor):
        sys.stdout = sys.__stdout__

    if isinstance(sys.stderr, StreamInterceptor):
        sys.stderr = sys.__stderr__

    if hasattr(sys, "_exit_orig"):
        sys.exit = sys._exit_orig
        del sys._exit_orig
