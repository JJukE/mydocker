import sys
import time
import carb
import asyncio
import argparse
import omni.usd
from pxr import Sdf
from enum import Enum   
from copy import deepcopy

loopCnt = 0
eventSubscription = None

def on_rendering_event(e: carb.events.IEvent):
    global loopCnt
    frameNo = e.payload["frame_number"]
    print(f"Exec python frame:{frameNo} completed loop:{loopCnt} ")

async def script_loop():
    global loopCnt
    print(f"Beginning script_loop {loopCnt}")
    while True:
        print(f"Exec python before waitBeginFrame: loop {loopCnt}")
        await omni.kit.app.get_app().pre_update_async()
        print(f"Exec python after waitBeginFrame: loop {loopCnt}")
        print(f"Exec python before waitEndFrame: loop {loopCnt}")
        await omni.kit.app.get_app().post_update_async()
        print(f"Exec python bafterefore waitEndFrame: loop {loopCnt}")
        loopCnt = loopCnt + 1

def main():
    global loopCnt
    global eventSubscription

    print(f"Exec python: create eventSubscription loop:{loopCnt} ")
    eventSubscription = (
        omni.usd.get_context()
        .get_rendering_event_stream()
        .create_subscription_to_push_by_type(
            int(omni.usd.StageRenderingEventType.NEW_FRAME),
            on_rendering_event,
            name="Python test exec order subcription")
    )
    print(f"Exec python: script execution begin, ensure_future pre loop:{loopCnt} ")
    asyncio.ensure_future(script_loop())
    print(f"Exec python: script execution begin, ensure_future post loop:{loopCnt} ")

main()
