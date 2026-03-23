"""pybind11 omni.isaac.compatibility_check bindings"""
from __future__ import annotations
import omni.isaac.compatibility_check._compatibility_check
import typing

__all__ = [
    "ICompatibilityCheckInterface",
    "RtxGpuInfo",
    "acquire_compatibility_check_interface",
    "release_compatibility_check_interface"
]


class ICompatibilityCheckInterface():
    def get_rtx_gpu_info(self, arg0: bool) -> typing.Tuple[bool, typing.List[RtxGpuInfo]]: ...
    pass
class RtxGpuInfo():
    """
    RTX GPU Info
    """
    def __init__(self) -> None: ...
    @property
    def device_id(self) -> str:
        """
        unique identifier for the physical device, (:obj:`str`)

        :type: str
        """
    @property
    def device_uuid(self) -> str:
        """
        unique device identifier, (:obj:`str`)

        :type: str
        """
    @property
    def pci_bus_id(self) -> str:
        """
        PCI bus identifier, (:obj:`str`)

        :type: str
        """
    @property
    def raytracing_shader_feature(self) -> bool:
        """
        whether non-vendor specific ray tracing is supported, (:obj:`bool`)

        :type: bool
        """
    @property
    def raytracing_supported(self) -> bool:
        """
        whether ray tracing is supported, (:obj:`bool`)

        :type: bool
        """
    @property
    def sub_sys_id(self) -> str:
        """
        PCI ID of the sub system, or zero if unavailable, (:obj:`str`)

        :type: str
        """
    @property
    def vendor_id(self) -> str:
        """
        unique vendor identifier. NVIDIA: 0x10DE, (:obj:`str`)

        :type: str
        """
    pass
def acquire_compatibility_check_interface(plugin_name: str = None, library_path: str = None) -> ICompatibilityCheckInterface:
    pass
def release_compatibility_check_interface(arg0: ICompatibilityCheckInterface) -> None:
    pass
