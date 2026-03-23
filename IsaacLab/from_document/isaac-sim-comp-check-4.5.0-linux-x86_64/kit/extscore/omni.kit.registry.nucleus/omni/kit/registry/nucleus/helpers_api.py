import os
from typing import List

import carb.dictionary
import omni.client

from .registry_manager import get_registry_manager_instance


def get_registry_url_by_name(registry_name: str) -> str:
    """Get registry url by registry name."""
    manager = get_registry_manager_instance()
    provider = manager.get_provider(registry_name)
    if provider:
        urls = provider.get_urls()
        if urls:
            return urls.index


def get_registry_url_by_ext_dict(ext_dict: carb.dictionary.Item) -> str:
    """Get registry url using extension data dictionary."""
    return get_registry_url_by_name(ext_dict.get("registryProviderName", None))


def get_package_resource_url_by_key(key: str, ext_dict: carb.dictionary.Item) -> str:
    """Get package resource url using extension data dictionary by key in package dict (like icon_remote)."""
    registry_url = get_registry_url_by_ext_dict(ext_dict)
    if not registry_url:
        return None

    preview_image_remote = ext_dict.get("package", {}).get(key, None)
    if preview_image_remote:
        return f"{registry_url}/{preview_image_remote}"


def get_preview_image_url(ext_dict: carb.dictionary.Item) -> str:
    """Get preview image url using extension data dictionary."""
    return get_package_resource_url_by_key("preview_image_remote", ext_dict)


def get_icon_url(ext_dict: carb.dictionary.Item) -> str:
    """Get icon url using extension data dictionary."""
    return get_package_resource_url_by_key("icon_remote", ext_dict)


def get_extension_metadata_possible_urls(ext_dict: carb.dictionary.Item) -> List[str]:
    """Get possible urls for extension metadata using extension data dictionary.

    To support older format, more than one url can be returned if extension is hosted on different host than registry.

    The first one is the most likely to be correct.
    """

    registry_url = get_registry_url_by_ext_dict(ext_dict)
    if not registry_url:
        return []

    package_dict = archive_path = ext_dict.get("package", {})
    # Same as archive path, but extension is `.json`:
    archive_path = package_dict.get("archivePath", None)
    if archive_path:
        # build url relative to the registry url
        archive_path = archive_path.replace("\\", "/")
        archive_path = omni.client.combine_urls(registry_url + "/index.zip", archive_path)
        archive_path = omni.client.normalize_url(archive_path)
        archive_path = os.path.splitext(archive_path)[0] + ".json"

        paths = []
        # if archive is on the same host as registry (usually on nucleus), try archive path first. Metadata would be
        # next to it
        if omni.client.break_url(archive_path).host == omni.client.break_url(registry_url).host:
            paths.append(archive_path)

        # If archive path is on different host (like packman). Do the best guess using registry url.
        # Old format is one big folder, new format is a subfolder for each extension
        data_filename = os.path.basename(archive_path)
        for path in (
            "{}/archives/{}/{}".format(registry_url, package_dict.get("name", ""), data_filename),
            "{}/archives/{}".format(registry_url, data_filename),
        ):
            if path not in paths:
                paths.append(path)
        return paths

    return []


def get_extension_metadata_url(ext_dict: carb.dictionary.Item) -> str:
    """Get extension metadata url using extension data dictionary.

    Only the latest supported format is returned. To get all possible urls use `get_extension_metadata_possible_urls`.
    """
    urls = get_extension_metadata_possible_urls(ext_dict)
    if urls:
        return urls[0]
