import carb
import time

import omni.client
import concurrent.futures
import contextlib

from omni.client import CopyBehavior

from .common import info_exec_time, change_envvar

LOCK_RETRY_COUNT = 5
LOCK_RETRY_DELAY = 1.0


def omni_client_lock(url: str) -> omni.client.Result:
    f = concurrent.futures.Future()

    def on_lock_cb(result):
        if not f.done():
            f.set_result(result)

    omni.client.lock_with_callback(url, on_lock_cb)

    return f.result()


def omni_client_unlock(url: str) -> omni.client.Result:
    f = concurrent.futures.Future()

    def on_unlock_cb(result):
        if not f.done():
            f.set_result(result)

    omni.client.unlock_with_callback(url, on_unlock_cb)

    return f.result()


@contextlib.contextmanager
def omni_client_lock_with_retry(url, enabled):
    if not enabled:
        yield omni.client.Result.OK
        return

    retry_count = LOCK_RETRY_COUNT
    result = None
    while True:
        result = omni_client_lock(url)
        if result == omni.client.Result.ERROR:
            retry_count -= 1
            if retry_count <= 0:
                break
            carb.log_info(
                f"lock: {url} result: {result}. Retrying in {LOCK_RETRY_DELAY} seconds. Retries left: {retry_count}"
            )
            time.sleep(LOCK_RETRY_DELAY)
        else:
            break

    carb.log_info(f"lock: {url} result: {result}")
    yield result
    if result == omni.client.Result.OK:
        result = omni_client_unlock(url)
        carb.log_info(f"unlock: {url} result: {result}")


@contextlib.contextmanager
def omni_client_set_retries(max_ms, base_ms, jitter_ms):
    prev = omni.client.set_retries(max_ms, base_ms, jitter_ms)
    yield
    omni.client.set_retries(*prev)


@contextlib.contextmanager
def omni_client_reduce_retries():
    with omni_client_set_retries(1000, 100, 100), change_envvar("OMNICLIENT_HTTP_RETRIES", "3"), change_envvar(
        "OMNICLIENT_HTTP_TIMEOUT", "10"
    ):
        yield


@info_exec_time()
def omni_client_stat(url: str) -> omni.client.Result:
    return omni.client.stat(url)


def omni_client_exists(url: str, reason="") -> bool:
    carb.log_info(f"({reason}) check exists, omni.client.stat: {url}")
    result, _ = omni.client.stat(url)
    carb.log_info(f"stat result: {result}")
    return result == omni.client.Result.OK


def omni_client_stat(url: str):
    carb.log_info(f"omni.client.stat: {url}")
    result, entry = omni.client.stat(url)
    carb.log_info(f"stat result: {result}")
    return result, entry


async def omni_client_exists_async(url: str, reason="") -> bool:
    carb.log_info(f"({reason}) check exists, omni.client.stat: {url}")
    result, _ = await omni.client.stat_async(url)
    carb.log_info(f"stat async result: {result}")
    return result == omni.client.Result.OK


async def omni_client_stat_async(url: str):
    carb.log_info(f"omni.client.stat async: {url}")
    result, entry = await omni.client.stat_async(url)
    carb.log_info(f"stat async result: {result}")
    return result, entry


def omni_client_try_delete(url: str):
    result = omni.client.delete(url)
    carb.log_info(f"delete: {url}. result: {result}")


@info_exec_time()
def omni_client_copy(
    src: str, dst: str, reason: str, behavior: CopyBehavior = CopyBehavior.ERROR_IF_EXISTS
) -> omni.client.Result:
    carb.log_info(f"({reason}) copying: {src} -> {dst}")
    result = omni.client.copy(src, dst, behavior)
    carb.log_info(f"result: {result}")
    return result


async def omni_client_copy_async(
    src: str, dst: str, reason: str, behavior: CopyBehavior = CopyBehavior.ERROR_IF_EXISTS
) -> omni.client.Result:
    carb.log_info(f"({reason}) copying: {src} -> {dst}")
    result = await omni.client.copy_async(src, dst, behavior)
    carb.log_info(f"result: {result}")
    return result


def omni_client_read_file(url: str):
    carb.log_info(f"omni.client.read_file: {url}")
    result, version, content = omni.client.read_file(url)
    carb.log_info(f"read_file result: {result}")
    return result, content


def omni_client_write_file(url: str, content: bytes):
    carb.log_info(f"omni.client.write_file: {url}")
    result = omni.client.write_file(url, content)
    carb.log_info(f"write_file result: {result}")
    return result


@info_exec_time()
def upload_file(path_local, path_remote, overwrite: bool = True) -> bool:
    """Upload file to from local path to remote using a client library.

    Returns True if file was uploaded or already exists, False otherwise.
    """
    path_local = path_local.replace("\\", "/")
    carb.log_info(f"uploading '{path_local}' to '{path_remote}' (overwrite: {overwrite})...")
    behavior = CopyBehavior.OVERWRITE if overwrite else CopyBehavior.ERROR_IF_EXISTS
    result = omni_client_copy(path_local, path_remote, "uploading archive", behavior=behavior)
    if result != omni.client.Result.OK:
        if result == omni.client.Result.ERROR_ALREADY_EXISTS and not overwrite:
            return True
        carb.log_error(f"Failed uploading file to: '{path_remote}' with: {result}")
        return False
    return True
