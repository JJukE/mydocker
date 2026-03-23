import argparse
import asyncio
import time
import carb
import carb.events
import omni.usd
from omni.kit.viewport.utility import create_viewport_window

# StageLoader taken from load_usd.py
class StageLoader:
    def __init__(self) -> None:
        self._current_url = ""
        self._stage_event_sub = (
            omni.usd.get_context()
            .get_stage_event_stream()
            .create_subscription_to_pop(self.on_stage_event, name="Stage Load Timer Subscription")
        )
        self._loading = False
        self._future_usd_loaded = asyncio.Future()
        self._start_ts = 0
        self._duration = 0

    def duration(self):
        return self._duration

    async def wait(self):
        await self._future_usd_loaded

    def result(self):
        return self._future_usd_loaded.result()

    def load(self, url: str) -> None:
        if self._loading:
            carb.log_error("*** Cannot load a new stage while one is loading")
            return
        # enable the flag, track the current url being loaded
        self._loading = True
        self._current_url = url
        omni.usd.get_context().open_stage(self._current_url)
        return

    def on_stage_event(self, e: carb.events.IEvent):
        event = omni.usd.StageEventType(e.type)
        payload: dict = e.payload
        carb.log_info(f"***Stage Event: {event} {payload}")

        if event == omni.usd.StageEventType.OPENING:
            url = payload.get("val", None)
            if self._loading and url:
                self._start_ts = time.monotonic()

        elif event == omni.usd.StageEventType.ASSETS_LOADED:
            if self._loading:
                self._loading = False
                self._duration = time.monotonic() - self._start_ts
                carb.log_warn(f"{self._current_url} is loaded in {self._duration}s.")
                self._future_usd_loaded.set_result(True)

        elif event == omni.usd.StageEventType.OPEN_FAILED and self._loading:
            self._loading = False
            carb.log_error(f"{self._current_url} failed to load.")
            self._future_usd_loaded.set_result(False)

usd_loader = StageLoader()

def createViewport(w, h, x, y):
    create_viewport_window(width = w, height = h, position_x = x, position_y = y)

async def run(options):
    # create viewports
    createViewport(options.width, options.height, options.posx, options.posy)

    # load the scene
    usd_loader.load(options.filename)
    carb.log_warn("Start loading")
    await usd_loader.wait()
    carb.log_warn("End loading")
    if not usd_loader.result():
        omni.kit.app.get_app().post_quit()
        return

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("width", help="Viewport width.", type=int)
    parser.add_argument("height", help="Viewport height.", type=int)
    parser.add_argument("posx", help="Viewport position - x.", type=int)
    parser.add_argument("posy", help="Viewport position - y.", type=int)
    parser.add_argument("--filename", help="Path to USD stage.")
    asyncio.ensure_future (run(parser.parse_args()))
