import sys
import time
import carb
import asyncio
import argparse
import omni.usd
from pxr import Sdf
from enum import Enum
from copy import deepcopy
from omni.kit import viewport_legacy
import omni.renderer_capture

windowCnt = 0
uniqueTickRates = [10, 24, 33, 48, 60, 120]
exitFrameCnt = 200
separateHydraEngine = False
postProcessGraph = False
windows = [None] * windowCnt
# +1 because we hide the default viewport and ignore it, but we still get the results
# for that window in the callback
frameNo = [0] * (windowCnt + 1)
cameras = ['/World/Cameras/CameraA', '/World/Cameras/CameraB', '/World/Cameras/CameraC','/World/Cameras/CameraD']
renderProducts = ['/Render/RenderDefault', '/Render/RenderView1', '/Render/RenderView2']
eventSubscription = None
captureFrame = False

async def capture_frame():
    global windowCnt
    global eventSubscription
    global windows
    outFile = carb.settings.get_settings().get("/app/captureFrame/fileName")
    capture_next_frame = omni.renderer_capture.acquire_renderer_capture_interface().capture_next_frame_swapchain
    capture_next_frame(outFile)
    await omni.kit.app.get_app().next_update_async()
    eventSubscription = None
    windows = [None] * windowCnt
    omni.kit.app.get_app().post_quit(0)

def on_rendering_event(e: carb.events.IEvent):
    global frameNo
    global captureFrame
    viewId = e.payload["viewport_handle"]
    frameNo[viewId] = e.payload["frame_number"]
    minFrameRate = sys.maxsize
    for i in range(1, len(frameNo)):
        minFrameRate = min(minFrameRate, frameNo[i])
    if minFrameRate >= exitFrameCnt and not captureFrame:
        captureFrame = True
        asyncio.ensure_future(capture_frame())


def load_callback(result, err):
    if result:
        run_test()
    else:
        carb.log_error("test_multiview.py: usd stage file load failed")
        omni.kit.app.get_app().post_quit(-1)

async def applyCarbSettings():
    try:
        carb.settings.get_settings().set_string("/rtx/rendermode", "RaytracedLighting")
        defaultWindow = viewport_legacy.get_viewport_interface().get_viewport_window()
        await omni.usd.get_context().next_frame_async(defaultWindow.get_id())
        asyncio.ensure_future(create_viewports())
    except Exception as e:
        carb.log_error("test_multiview.py: applyCarbSettings exception")
        carb.log_error(str(e))
        omni.kit.app.get_app().post_quit(-1)

async def create_viewports():
    try:
        global windows
        global uniqueTickRates
        global separateHydraEngine
        x_pos = 12
        y_pos = 75
        viewportFactory = viewport_legacy.get_viewport_interface()
        defaultWindow = viewportFactory.get_viewport_window()
        await omni.usd.get_context().next_frame_async(defaultWindow.get_id())
        defaultWindow.set_visible(False)
        for i in range(len(windows)):
            if i and i%3 == 0:
                x_pos = 12
                y_pos += 312
            handle = viewportFactory.create_instance()
            windows[i] = viewportFactory.get_viewport_window(handle)
            windows[i].set_window_size(300, 300)
            windows[i].set_window_pos(x_pos, y_pos)
            if separateHydraEngine:
                windows[i].set_hydra_engine_tick_rate(uniqueTickRates[i % len(uniqueTickRates)])
                # Await one app tick so a render is scheduled
                await omni.kit.app.get_app().next_update_async()
                await omni.usd.get_context().next_frame_async(windows[i].get_id())
            x_pos += 312
        asyncio.ensure_future(set_cameras())

    except Exception as e:
        carb.log_error("test_multiview.py: create_viewports exception")
        carb.log_error(str(e))
        omni.kit.app.get_app().post_quit(-1)

async def set_cameras():
    try:
        global cameras
        global windows
        global separateHydraEngine
        global postProcessGraph
        for i in range(len(windows)):
            if postProcessGraph:
                windows[i].set_render_product_path(renderProducts[i % len(renderProducts)])
            else:
                windows[i].set_active_camera(cameras[i % len(cameras)])
            if separateHydraEngine:
                await omni.usd.get_context().next_frame_async(windows[i].get_id())
    except Exception as e:
        carb.log_error("test_multiview.py: set_cameras exception")
        carb.log_error(str(e))
        omni.kit.app.get_app().post_quit(-1)

def run_test():
    asyncio.ensure_future(applyCarbSettings())

def main():
    global windows
    global frameNo
    global windowCnt
    global postProcessGraph
    global eventSubscription
    global separateHydraEngine
    parser = argparse.ArgumentParser()
    parser.add_argument("path", help="Path to USD stage.")
    parser.add_argument("--windowCnt", type=int, required=True, help="Number of viewports to create")
    parser.add_argument('--separateHydraEngine', dest='separateHydraEngine', action='store_true')
    parser.add_argument('--no-separateHydraEngine', dest='separateHydraEngine', action='store_false')
    parser.set_defaults(separateHydraEngine=False)
    parser.add_argument('--postProcessGraph', dest='postProcessGraph', action='store_true')
    parser.add_argument('--no-postProcessGraph', dest='postProcessGraph', action='store_false')
    parser.set_defaults(postProcessGraph=False)

    try:
        options = parser.parse_args()
        windowCnt = options.windowCnt
        windows = [None] * windowCnt
        frameNo = [0] * (windowCnt + 1)
        context = omni.usd.get_context()
        eventSubscription = (
            context
            .get_rendering_event_stream()
            .create_subscription_to_push_by_type(
                int(omni.usd.StageRenderingEventType.NEW_FRAME),
                on_rendering_event,
                name="Multiview Unit Test Subcription")
        )
        postProcessGraph = options.postProcessGraph
        separateHydraEngine = options.separateHydraEngine
        if separateHydraEngine:
            for i in range(windowCnt):
                omni.usd.add_hydra_engine("rtx", context)
        context.open_stage_with_callback(options.path, load_callback)
    except Exception as e:
        carb.log_error("test_multiview.py: main exception")
        carb.log_error(str(e))
        omni.kit.app.get_app().post_quit(-1)

main()
