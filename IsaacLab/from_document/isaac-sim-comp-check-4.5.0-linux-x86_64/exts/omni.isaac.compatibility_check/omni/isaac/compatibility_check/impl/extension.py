# Copyright (c) 2018-2023, NVIDIA CORPORATION. All rights reserved.
#
# NVIDIA CORPORATION and its licensors retain all intellectual property
# and proprietary rights in and to this software, related documentation
# and any modifications thereto. Any use, reproduction, disclosure or
# distribution of this software and related documentation without an express
# license agreement from NVIDIA CORPORATION is strictly prohibited.
#

import asyncio

import carb
import omni.ext
import omni.kit.app

from . import compatibility_checker


class Extension(omni.ext.IExt):
    def on_startup(self, ext_id):
        # extension metadata
        import omni  # FIXME: UnboundLocalError: local variable 'omni' referenced before assignment

        extension_manager = omni.kit.app.get_app().get_extension_manager()
        ext_path = extension_manager.get_extension_path(ext_id)

        # get extension settings
        settings = carb.settings.get_settings()

        gpu_driver = settings.get("/exts/omni.isaac.compatibility_check/gpu_driver")
        gpu_vram = settings.get("/exts/omni.isaac.compatibility_check/gpu_vram")

        cpu_cores = settings.get("/exts/omni.isaac.compatibility_check/cpu_cores")
        ram = settings.get("/exts/omni.isaac.compatibility_check/ram")
        storage = settings.get("/exts/omni.isaac.compatibility_check/storage")

        operating_system = settings.get("/exts/omni.isaac.compatibility_check/operating_system")

        test_kit_app = settings.get("/exts/omni.isaac.compatibility_check/test_kit_app")
        test_kit_args = settings.get("/exts/omni.isaac.compatibility_check/test_kit_args")

        checker = compatibility_checker.Checker()

        omni.kit.app.get_app().print_and_log("")
        omni.kit.app.get_app().print_and_log("=============================================")
        omni.kit.app.get_app().print_and_log("")
        omni.kit.app.get_app().print_and_log("NVIDIA GPU(s)")

        checker.check_nvidia_smi({})
        checker.check_driver_version(gpu_driver)
        checker.check_rtx_gpu({})
        checker.check_vram(gpu_vram)

        omni.kit.app.get_app().print_and_log("")
        omni.kit.app.get_app().print_and_log("CPU, RAM and Storage")
        checker.check_cpu(cpu_cores)
        checker.check_cpu_cores(cpu_cores)
        checker.check_ram(ram)
        checker.check_storage(storage)

        omni.kit.app.get_app().print_and_log("")
        omni.kit.app.get_app().print_and_log("Others")
        checker.check_operating_system(operating_system)
        checker.check_display()

        omni.kit.app.get_app().print_and_log("")
        omni.kit.app.get_app().print_and_log("=============================================")
        omni.kit.app.get_app().print_and_log("")
        result = "PASSED" if checker.compatibility_check_status else "FAILED"
        omni.kit.app.get_app().print_and_log(f"System checking result: {result}")
        omni.kit.app.get_app().print_and_log("")
        omni.kit.app.get_app().print_and_log("=============================================")
        omni.kit.app.get_app().print_and_log("")

        # window
        self._window = None
        try:
            import omni.ui
        except ImportError as e:
            return
        from . import check_window

        self._window_title = "Compatibility check"
        self._window = check_window.CheckWindow(
            ext_path, self._window_title, checker, test={"kit": {"app": test_kit_app, "args": test_kit_args}}
        )
        self._build_task = asyncio.ensure_future(self._build_layout())

    async def _build_layout(self):
        import omni.ui as ui

        await omni.kit.app.get_app().next_update_async()

        window_handle = ui.Workspace.get_window(self._window_title)
        if window_handle is None:
            return

        # setup the docking Space
        main_dockspace = ui.Workspace.get_window("DockSpace")

        window_handle.dock_in(main_dockspace, ui.DockPosition.SAME)
        window_handle.dock_tab_bar_visible = False

        await omni.kit.app.get_app().next_update_async()

    def on_shutdown(self):
        if self._window:
            self._window.destroy()
            self._window = None
