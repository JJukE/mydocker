from ._renderer import *


# Cached interface instance pointer
def get_renderer_interface() -> IRenderer:
    """Returns cached :class:`omni.kit.renderer.IRenderer` interface"""

    if not hasattr(get_renderer_interface, "renderer"):
        get_renderer_interface.renderer = acquire_renderer_interface()
    return get_renderer_interface.renderer
