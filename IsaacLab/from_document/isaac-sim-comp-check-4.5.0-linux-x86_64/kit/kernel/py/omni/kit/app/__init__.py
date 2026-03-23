"""This module contains bindings to C++ omni::kit::IApp interface, core C++ part of Omniverse Kit.

All the function are in omni.kit.IApp class, to get it use get_app_interface method, which caches
acquire interface call:

    >>> import omni.kit.app
    >>> a = omni.kit.app.get_app()
"""

from ._impl import *
from ._impl import _shutdown_kit_scripting, _startup_kit_scripting
