from __future__ import annotations
import omni.client.impl._omniclient
import typing

__all__ = [
    "AccessFlags",
    "AclEntry",
    "AuthDeviceFlowParams",
    "ChannelEvent",
    "ConnectionStatus",
    "Content",
    "CopyBehavior",
    "FileStatus",
    "ItemFlags",
    "ListEntry",
    "ListEvent",
    "ListIncludeOption",
    "LiveUpdateType",
    "LogLevel",
    "Registration",
    "Request",
    "Result",
    "ServerInfo",
    "Url",
    "VERSION",
    "WriteFileExInfo",
    "add_bookmark",
    "add_default_search_path",
    "add_user_to_group_with_callback",
    "authentication_cancel",
    "break_url",
    "break_url_reference",
    "close_cached_file",
    "combine_urls",
    "combine_with_base_url",
    "copy_file_with_callback",
    "copy_folder_with_callback",
    "copy_with_callback",
    "create_checkpoint_with_callback",
    "create_folder_with_callback",
    "create_group_with_callback",
    "create_with_hash_with_callback",
    "delete_single_with_callback",
    "delete_with_callback",
    "enable_nucleus_cache_bypass",
    "get_acls_with_callback",
    "get_base_url",
    "get_branch_and_checkpoint_from_query",
    "get_default_search_paths",
    "get_group_users_with_callback",
    "get_groups_with_callback",
    "get_hub_http_uri_with_callback",
    "get_hub_version_with_callback",
    "get_local_file_with_callback",
    "get_server_info_with_callback",
    "get_user_groups_with_callback",
    "get_users_with_callback",
    "get_version",
    "initialize",
    "join_channel_with_callback",
    "list_bookmarks_with_callback",
    "list_checkpoints_with_callback",
    "list_subscribe_with_callback",
    "list_with_callback",
    "live_get_latest_server_time",
    "live_process",
    "live_process_up_to",
    "live_register_queued_callback",
    "live_set_queued_callback",
    "live_wait_for_pending_updates",
    "lock_with_callback",
    "make_file_url",
    "make_printable",
    "make_query_from_branch_and_checkpoint",
    "make_relative_url",
    "make_url",
    "move_file_with_callback",
    "move_folder_with_callback",
    "move_with_callback",
    "normalize_url",
    "obliterate_with_callback",
    "open_cached_file_with_callback",
    "pop_base_url",
    "push_base_url",
    "read_file_with_callback",
    "reconnect",
    "refresh_auth_token_with_callback",
    "register_authentication_callback",
    "register_authorize_callback",
    "register_connection_status_callback",
    "register_device_flow_auth_callback",
    "register_file_status_callback",
    "remove_bookmark",
    "remove_default_search_path",
    "remove_group_with_callback",
    "remove_user_from_group_with_callback",
    "rename_group_with_callback",
    "resolve_subscribe_with_callback",
    "resolve_with_callback",
    "send_message_with_callback",
    "set_acls_with_callback",
    "set_alias",
    "set_authentication_message_box_callback",
    "set_azure_sas_token",
    "set_hang_detection_time_ms",
    "set_http_header",
    "set_log_callback",
    "set_log_level",
    "set_product_info",
    "set_retries",
    "set_s3_configuration",
    "shutdown",
    "sign_out",
    "stat_subscribe_with_callback",
    "stat_with_callback",
    "trace_start",
    "trace_stop",
    "undelete_with_callback",
    "unlock_with_callback",
    "write_file_ex_with_callback",
    "write_file_with_callback"
]


class AccessFlags():
    """
    Members:

      READ : Can read

      WRITE : Can write

      ADMIN : Can change ACLs
    """
    def __and__(self, other: object) -> object: ...
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __invert__(self) -> object: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __or__(self, other: object) -> object: ...
    def __rand__(self, other: object) -> object: ...
    def __repr__(self) -> str: ...
    def __ror__(self, other: object) -> object: ...
    def __rxor__(self, other: object) -> object: ...
    def __setstate__(self, state: int) -> None: ...
    def __xor__(self, other: object) -> object: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    ADMIN: omni.client.impl._omniclient.AccessFlags # value = <AccessFlags.ADMIN: 4>
    READ: omni.client.impl._omniclient.AccessFlags # value = <AccessFlags.READ: 1>
    WRITE: omni.client.impl._omniclient.AccessFlags # value = <AccessFlags.WRITE: 2>
    __members__: dict # value = {'READ': <AccessFlags.READ: 1>, 'WRITE': <AccessFlags.WRITE: 2>, 'ADMIN': <AccessFlags.ADMIN: 4>}
    pass
class AclEntry():
    def __init__(self, arg0: str, arg1: int) -> None: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def access(self) -> int:
        """
        The access level for this user/group, test and set bitwise with `AccessFlags`

        :type: int
        """
    @access.setter
    def access(self, arg0: int) -> None:
        """
        The access level for this user/group, test and set bitwise with `AccessFlags`
        """
    @property
    def name(self) -> str:
        """
        The name the user/group

        :type: str
        """
    @name.setter
    def name(self, arg0: str) -> None:
        """
        The name the user/group
        """
    pass
class AuthDeviceFlowParams():
    @property
    def code(self) -> str:
        """
        :type: str
        """
    @property
    def expiration(self) -> int:
        """
        :type: int
        """
    @property
    def server(self) -> str:
        """
        :type: str
        """
    @property
    def url(self) -> str:
        """
        :type: str
        """
    pass
class ChannelEvent():
    """
    Members:

      ERROR : Error.

      MESSAGE : Someone sent a message.

      HELLO : You joined a channel that someone else was already in, and they said hello.

      JOIN : Someone joined.

      LEFT : Someone left.

      DELETED : Someone deleted the channel or changed ACLs so you no longer have access.
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    DELETED: omni.client.impl._omniclient.ChannelEvent # value = <ChannelEvent.DELETED: 5>
    ERROR: omni.client.impl._omniclient.ChannelEvent # value = <ChannelEvent.ERROR: 0>
    HELLO: omni.client.impl._omniclient.ChannelEvent # value = <ChannelEvent.HELLO: 2>
    JOIN: omni.client.impl._omniclient.ChannelEvent # value = <ChannelEvent.JOIN: 3>
    LEFT: omni.client.impl._omniclient.ChannelEvent # value = <ChannelEvent.LEFT: 4>
    MESSAGE: omni.client.impl._omniclient.ChannelEvent # value = <ChannelEvent.MESSAGE: 1>
    __members__: dict # value = {'ERROR': <ChannelEvent.ERROR: 0>, 'MESSAGE': <ChannelEvent.MESSAGE: 1>, 'HELLO': <ChannelEvent.HELLO: 2>, 'JOIN': <ChannelEvent.JOIN: 3>, 'LEFT': <ChannelEvent.LEFT: 4>, 'DELETED': <ChannelEvent.DELETED: 5>}
    pass
class ConnectionStatus():
    """
    Members:

      CONNECTING : Attempting to connect

      CONNECTED : Successfully connected

      CONNECT_ERROR : Connection error

      DISCONNECTED : Disconnected

      SIGNED_OUT : omniClientSignOut called

      NO_USERNAME : No username was provided (This status is no longer used)

      AUTH_ABORT : Application requested auth abort

      AUTH_CANCELLED : User or application request auth cancel

      AUTH_ERROR : Internal error while trying to authenticate

      AUTH_FAILED : Authentication failed

      SERVER_INCOMPATIBLE : The server is not compatible with this version of the client library

      INVALID_HOST : The host name is invalid
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    AUTH_ABORT: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.AUTH_ABORT: 6>
    AUTH_CANCELLED: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.AUTH_CANCELLED: 7>
    AUTH_ERROR: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.AUTH_ERROR: 8>
    AUTH_FAILED: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.AUTH_FAILED: 9>
    CONNECTED: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.CONNECTED: 1>
    CONNECTING: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.CONNECTING: 0>
    CONNECT_ERROR: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.CONNECT_ERROR: 2>
    DISCONNECTED: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.DISCONNECTED: 3>
    INVALID_HOST: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.INVALID_HOST: 11>
    NO_USERNAME: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.NO_USERNAME: 5>
    SERVER_INCOMPATIBLE: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.SERVER_INCOMPATIBLE: 10>
    SIGNED_OUT: omni.client.impl._omniclient.ConnectionStatus # value = <ConnectionStatus.SIGNED_OUT: 4>
    __members__: dict # value = {'CONNECTING': <ConnectionStatus.CONNECTING: 0>, 'CONNECTED': <ConnectionStatus.CONNECTED: 1>, 'CONNECT_ERROR': <ConnectionStatus.CONNECT_ERROR: 2>, 'DISCONNECTED': <ConnectionStatus.DISCONNECTED: 3>, 'SIGNED_OUT': <ConnectionStatus.SIGNED_OUT: 4>, 'NO_USERNAME': <ConnectionStatus.NO_USERNAME: 5>, 'AUTH_ABORT': <ConnectionStatus.AUTH_ABORT: 6>, 'AUTH_CANCELLED': <ConnectionStatus.AUTH_CANCELLED: 7>, 'AUTH_ERROR': <ConnectionStatus.AUTH_ERROR: 8>, 'AUTH_FAILED': <ConnectionStatus.AUTH_FAILED: 9>, 'SERVER_INCOMPATIBLE': <ConnectionStatus.SERVER_INCOMPATIBLE: 10>, 'INVALID_HOST': <ConnectionStatus.INVALID_HOST: 11>}
    pass
class Content():
    """
    Object to hold file content.

    Use python :obj:`memoryview` to access it. For example:

    .. code:: python

            _, content = await omni.client.read_file_async("omniverse://ov-sandbox/some/file.txt")
            print(memoryview(content).tobytes())

        
    """
    def __getitem__(self, arg0: int) -> str: ...
    def __len__(self) -> int: ...
    def __setitem__(self, arg0: int, arg1: str) -> None: ...
    pass
class CopyBehavior():
    """
    Members:

      ERROR_IF_EXISTS : Fails to copy with an error if the destination already exists

      OVERWRITE : Overwrites any files on copy
    """
    def __and__(self, other: object) -> object: ...
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __invert__(self) -> object: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __or__(self, other: object) -> object: ...
    def __rand__(self, other: object) -> object: ...
    def __repr__(self) -> str: ...
    def __ror__(self, other: object) -> object: ...
    def __rxor__(self, other: object) -> object: ...
    def __setstate__(self, state: int) -> None: ...
    def __xor__(self, other: object) -> object: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    ERROR_IF_EXISTS: omni.client.impl._omniclient.CopyBehavior # value = <CopyBehavior.ERROR_IF_EXISTS: 0>
    OVERWRITE: omni.client.impl._omniclient.CopyBehavior # value = <CopyBehavior.OVERWRITE: 1>
    __members__: dict # value = {'ERROR_IF_EXISTS': <CopyBehavior.ERROR_IF_EXISTS: 0>, 'OVERWRITE': <CopyBehavior.OVERWRITE: 1>}
    pass
class FileStatus():
    """
    Members:

      READING : Reading

      WRITING : Writing

      COPYING : Copying

      MOVING : Moving

      DELETING : Deleting

      OBLITERATING : Obliterating

      SENDING_UPDATE : Sending Live Update

      RECEIVED_UPDATE : Received Live Update

      LISTING : Listing

      STATING : Stating
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    COPYING: omni.client.impl._omniclient.FileStatus # value = <FileStatus.COPYING: 2>
    DELETING: omni.client.impl._omniclient.FileStatus # value = <FileStatus.DELETING: 4>
    LISTING: omni.client.impl._omniclient.FileStatus # value = <FileStatus.LISTING: 8>
    MOVING: omni.client.impl._omniclient.FileStatus # value = <FileStatus.MOVING: 3>
    OBLITERATING: omni.client.impl._omniclient.FileStatus # value = <FileStatus.OBLITERATING: 5>
    READING: omni.client.impl._omniclient.FileStatus # value = <FileStatus.READING: 0>
    RECEIVED_UPDATE: omni.client.impl._omniclient.FileStatus # value = <FileStatus.RECEIVED_UPDATE: 7>
    SENDING_UPDATE: omni.client.impl._omniclient.FileStatus # value = <FileStatus.SENDING_UPDATE: 6>
    STATING: omni.client.impl._omniclient.FileStatus # value = <FileStatus.STATING: 9>
    WRITING: omni.client.impl._omniclient.FileStatus # value = <FileStatus.WRITING: 1>
    __members__: dict # value = {'READING': <FileStatus.READING: 0>, 'WRITING': <FileStatus.WRITING: 1>, 'COPYING': <FileStatus.COPYING: 2>, 'MOVING': <FileStatus.MOVING: 3>, 'DELETING': <FileStatus.DELETING: 4>, 'OBLITERATING': <FileStatus.OBLITERATING: 5>, 'SENDING_UPDATE': <FileStatus.SENDING_UPDATE: 6>, 'RECEIVED_UPDATE': <FileStatus.RECEIVED_UPDATE: 7>, 'LISTING': <FileStatus.LISTING: 8>, 'STATING': <FileStatus.STATING: 9>}
    pass
class ItemFlags():
    """
    Members:

      READABLE_FILE : You can call read_file on this. note: ACLs may still prevent you from reading it

      WRITEABLE_FILE : You can call write_file on this. note: ACLs may still prevent you from writing it

      CAN_HAVE_CHILDREN : This thing can contain other things (a folder-like thing)

      DOES_NOT_HAVE_CHILDREN : This thing does not have any children. The lack of this flag does not mean it does have children!
                   Sometimes we are not sure if it has children until you attempt to list the children.
                   This is only intended to be used for UI elements to hide the 'expand' button if we
                   are sure it does not have any children.

      IS_MOUNT : This thing is the root of a mount point

      IS_INSIDE_MOUNT : This thing is located inside a mounted folder

      CAN_LIVE_UPDATE : This thing supports live updates

      IS_OMNI_OBJECT : This thing is in omni-object format. You must use a special API to read/write it

      IS_CHANNEL : This thing is a channel. You can call join_channel on it

      IS_CHECKPOINTED : This item is checkpointed (meaning you can revert to it)

      IS_DELETED : This item is deleted
    """
    def __and__(self, other: object) -> object: ...
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __invert__(self) -> object: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __or__(self, other: object) -> object: ...
    def __rand__(self, other: object) -> object: ...
    def __repr__(self) -> str: ...
    def __ror__(self, other: object) -> object: ...
    def __rxor__(self, other: object) -> object: ...
    def __setstate__(self, state: int) -> None: ...
    def __xor__(self, other: object) -> object: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    CAN_HAVE_CHILDREN: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.CAN_HAVE_CHILDREN: 4>
    CAN_LIVE_UPDATE: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.CAN_LIVE_UPDATE: 64>
    DOES_NOT_HAVE_CHILDREN: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.DOES_NOT_HAVE_CHILDREN: 8>
    IS_CHANNEL: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.IS_CHANNEL: 256>
    IS_CHECKPOINTED: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.IS_CHECKPOINTED: 512>
    IS_DELETED: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.IS_DELETED: 1024>
    IS_INSIDE_MOUNT: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.IS_INSIDE_MOUNT: 32>
    IS_MOUNT: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.IS_MOUNT: 16>
    IS_OMNI_OBJECT: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.IS_OMNI_OBJECT: 128>
    READABLE_FILE: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.READABLE_FILE: 1>
    WRITEABLE_FILE: omni.client.impl._omniclient.ItemFlags # value = <ItemFlags.WRITEABLE_FILE: 2>
    __members__: dict # value = {'READABLE_FILE': <ItemFlags.READABLE_FILE: 1>, 'WRITEABLE_FILE': <ItemFlags.WRITEABLE_FILE: 2>, 'CAN_HAVE_CHILDREN': <ItemFlags.CAN_HAVE_CHILDREN: 4>, 'DOES_NOT_HAVE_CHILDREN': <ItemFlags.DOES_NOT_HAVE_CHILDREN: 8>, 'IS_MOUNT': <ItemFlags.IS_MOUNT: 16>, 'IS_INSIDE_MOUNT': <ItemFlags.IS_INSIDE_MOUNT: 32>, 'CAN_LIVE_UPDATE': <ItemFlags.CAN_LIVE_UPDATE: 64>, 'IS_OMNI_OBJECT': <ItemFlags.IS_OMNI_OBJECT: 128>, 'IS_CHANNEL': <ItemFlags.IS_CHANNEL: 256>, 'IS_CHECKPOINTED': <ItemFlags.IS_CHECKPOINTED: 512>, 'IS_DELETED': <ItemFlags.IS_DELETED: 1024>}
    pass
class ListEntry():
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def access(self) -> int:
        """
        YOUR access level, some combination of AccessFlags

        :type: int
        """
    @property
    def comment(self) -> str:
        """
        Provider-specific comment

        Not all providers support this, so it may be empty
                    

        :type: str
        """
    @property
    def created_by(self) -> str:
        """
        User Name of the client that created it

        Not all providers support this, so it may be empty
                    

        :type: str
        """
    @property
    def created_time(self) -> datetime.datetime:
        """
        When the file was created

        :type: datetime.datetime
        """
    @property
    def deleted_by(self) -> str:
        """
        User Name of the client that deleted it

        Not all providers support this, so it may be empty
                    

        :type: str
        """
    @property
    def deleted_time(self) -> datetime.datetime:
        """
        When the file was deleted

        :type: datetime.datetime
        """
    @property
    def flags(self) -> int:
        """
        Some combination of ItemFlags

        :type: int
        """
    @property
    def hash(self) -> str:
        """
        Provider specific file hash

        Not all providers support this, so it may be empty
                    

        :type: str
        """
    @property
    def locked_by(self) -> str:
        """
        User Name of the client that locked it. Empty if it's not locked.
                    

        :type: str
        """
    @property
    def modified_by(self) -> str:
        """
        User Name of the last client to modify it

        Not all providers support this, so it may be empty
                    

        :type: str
        """
    @property
    def modified_time(self) -> datetime.datetime:
        """
        The last time the file was modified

        :type: datetime.datetime
        """
    @property
    def relative_path(self) -> str:
        """
        The path for this item relative to the url provided to "List"

        :type: str
        """
    @property
    def size(self) -> int:
        """
        For files, the size in bytes

        :type: int
        """
    @property
    def version(self) -> str:
        """
        Provider-specific version

        May not be an always incrementing number (could be a hash, for example)

        Not all providers support this, so it may be empty
                    

        :type: str
        """
    pass
class ListEvent():
    """
    Members:

      UNKNOWN

      CREATED

      UPDATED

      DELETED

      METADATA

      LOCKED

      UNLOCKED

      OBLITERATED
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    CREATED: omni.client.impl._omniclient.ListEvent # value = <ListEvent.CREATED: 1>
    DELETED: omni.client.impl._omniclient.ListEvent # value = <ListEvent.DELETED: 3>
    LOCKED: omni.client.impl._omniclient.ListEvent # value = <ListEvent.LOCKED: 5>
    METADATA: omni.client.impl._omniclient.ListEvent # value = <ListEvent.METADATA: 4>
    OBLITERATED: omni.client.impl._omniclient.ListEvent # value = <ListEvent.OBLITERATED: 7>
    UNKNOWN: omni.client.impl._omniclient.ListEvent # value = <ListEvent.UNKNOWN: 0>
    UNLOCKED: omni.client.impl._omniclient.ListEvent # value = <ListEvent.UNLOCKED: 6>
    UPDATED: omni.client.impl._omniclient.ListEvent # value = <ListEvent.UPDATED: 2>
    __members__: dict # value = {'UNKNOWN': <ListEvent.UNKNOWN: 0>, 'CREATED': <ListEvent.CREATED: 1>, 'UPDATED': <ListEvent.UPDATED: 2>, 'DELETED': <ListEvent.DELETED: 3>, 'METADATA': <ListEvent.METADATA: 4>, 'LOCKED': <ListEvent.LOCKED: 5>, 'UNLOCKED': <ListEvent.UNLOCKED: 6>, 'OBLITERATED': <ListEvent.OBLITERATED: 7>}
    pass
class ListIncludeOption():
    """
    Members:

      NO_DELETED_FILES : Ignore deleted files

      INCLUDE_DELETED_FILES : Include deleted files

      ONLY_DELETED_FILES : Only deleted files
    """
    def __and__(self, other: object) -> object: ...
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __invert__(self) -> object: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __or__(self, other: object) -> object: ...
    def __rand__(self, other: object) -> object: ...
    def __repr__(self) -> str: ...
    def __ror__(self, other: object) -> object: ...
    def __rxor__(self, other: object) -> object: ...
    def __setstate__(self, state: int) -> None: ...
    def __xor__(self, other: object) -> object: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    INCLUDE_DELETED_FILES: omni.client.impl._omniclient.ListIncludeOption # value = <ListIncludeOption.INCLUDE_DELETED_FILES: 1>
    NO_DELETED_FILES: omni.client.impl._omniclient.ListIncludeOption # value = <ListIncludeOption.NO_DELETED_FILES: 0>
    ONLY_DELETED_FILES: omni.client.impl._omniclient.ListIncludeOption # value = <ListIncludeOption.ONLY_DELETED_FILES: 2>
    __members__: dict # value = {'NO_DELETED_FILES': <ListIncludeOption.NO_DELETED_FILES: 0>, 'INCLUDE_DELETED_FILES': <ListIncludeOption.INCLUDE_DELETED_FILES: 1>, 'ONLY_DELETED_FILES': <ListIncludeOption.ONLY_DELETED_FILES: 2>}
    pass
class LiveUpdateType():
    """
    Members:

      REMOTE : A remote client sent an update.

      LOCAL : The server acknowledged a local update.

      MORE : Due to Jitter reduction, a queued update may not be processed by `omniClientLiveProcess`. When this happens, the callback is called with this update type indicating that it's now time to process the update.
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    LOCAL: omni.client.impl._omniclient.LiveUpdateType # value = <LiveUpdateType.LOCAL: 1>
    MORE: omni.client.impl._omniclient.LiveUpdateType # value = <LiveUpdateType.MORE: 2>
    REMOTE: omni.client.impl._omniclient.LiveUpdateType # value = <LiveUpdateType.REMOTE: 0>
    __members__: dict # value = {'REMOTE': <LiveUpdateType.REMOTE: 0>, 'LOCAL': <LiveUpdateType.LOCAL: 1>, 'MORE': <LiveUpdateType.MORE: 2>}
    pass
class LogLevel():
    """
    Members:

      DEBUG : Extra chatty

      VERBOSE : Chatty

      INFO : Not a problem

      WARNING : Potential problem

      ERROR : Definite problem
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    DEBUG: omni.client.impl._omniclient.LogLevel # value = <LogLevel.DEBUG: 0>
    ERROR: omni.client.impl._omniclient.LogLevel # value = <LogLevel.ERROR: 4>
    INFO: omni.client.impl._omniclient.LogLevel # value = <LogLevel.INFO: 2>
    VERBOSE: omni.client.impl._omniclient.LogLevel # value = <LogLevel.VERBOSE: 1>
    WARNING: omni.client.impl._omniclient.LogLevel # value = <LogLevel.WARNING: 3>
    __members__: dict # value = {'DEBUG': <LogLevel.DEBUG: 0>, 'VERBOSE': <LogLevel.VERBOSE: 1>, 'INFO': <LogLevel.INFO: 2>, 'WARNING': <LogLevel.WARNING: 3>, 'ERROR': <LogLevel.ERROR: 4>}
    pass
class Registration():
    def __enter__(self) -> Registration: ...
    def __exit__(self, arg0: object, arg1: object, arg2: object) -> None: ...
    pass
class Request():
    """
    Generic request object. Can be used to stop request before completing.
    """
    def __enter__(self) -> Request: ...
    def __exit__(self, arg0: object, arg1: object, arg2: object) -> None: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    def is_finished(self) -> bool: ...
    def stop(self) -> None: ...
    def wait(self) -> None: ...
    @property
    def id(self) -> int:
        """
        :type: int
        """
    pass
class Result():
    """
    Members:

      OK

      OK_LATEST

      OK_NOT_YET_FOUND

      ERROR

      ERROR_CONNECTION

      ERROR_NOT_SUPPORTED

      ERROR_ACCESS_DENIED

      ERROR_NOT_FOUND

      ERROR_BAD_VERSION

      ERROR_ALREADY_EXISTS

      ERROR_SOURCE_IS_DEST

      ERROR_ACCESS_LOST

      ERROR_LOCKED

      ERROR_BAD_REQUEST

      ERROR_FOLDER_NOT_EMPTY

      ERROR_WRONG_TYPE

      ERROR_UNSUPPORTED_VERSION
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    ERROR: omni.client.impl._omniclient.Result # value = <Result.ERROR: 3>
    ERROR_ACCESS_DENIED: omni.client.impl._omniclient.Result # value = <Result.ERROR_ACCESS_DENIED: 6>
    ERROR_ACCESS_LOST: omni.client.impl._omniclient.Result # value = <Result.ERROR_ACCESS_LOST: 11>
    ERROR_ALREADY_EXISTS: omni.client.impl._omniclient.Result # value = <Result.ERROR_ALREADY_EXISTS: 9>
    ERROR_BAD_REQUEST: omni.client.impl._omniclient.Result # value = <Result.ERROR_BAD_REQUEST: 13>
    ERROR_BAD_VERSION: omni.client.impl._omniclient.Result # value = <Result.ERROR_BAD_VERSION: 8>
    ERROR_CONNECTION: omni.client.impl._omniclient.Result # value = <Result.ERROR_CONNECTION: 4>
    ERROR_FOLDER_NOT_EMPTY: omni.client.impl._omniclient.Result # value = <Result.ERROR_FOLDER_NOT_EMPTY: 14>
    ERROR_LOCKED: omni.client.impl._omniclient.Result # value = <Result.ERROR_LOCKED: 12>
    ERROR_NOT_FOUND: omni.client.impl._omniclient.Result # value = <Result.ERROR_NOT_FOUND: 7>
    ERROR_NOT_SUPPORTED: omni.client.impl._omniclient.Result # value = <Result.ERROR_NOT_SUPPORTED: 5>
    ERROR_SOURCE_IS_DEST: omni.client.impl._omniclient.Result # value = <Result.ERROR_SOURCE_IS_DEST: 10>
    ERROR_UNSUPPORTED_VERSION: omni.client.impl._omniclient.Result # value = <Result.ERROR_UNSUPPORTED_VERSION: 16>
    ERROR_WRONG_TYPE: omni.client.impl._omniclient.Result # value = <Result.ERROR_WRONG_TYPE: 15>
    OK: omni.client.impl._omniclient.Result # value = <Result.OK: 0>
    OK_LATEST: omni.client.impl._omniclient.Result # value = <Result.OK_LATEST: 1>
    OK_NOT_YET_FOUND: omni.client.impl._omniclient.Result # value = <Result.OK_NOT_YET_FOUND: 2>
    __members__: dict # value = {'OK': <Result.OK: 0>, 'OK_LATEST': <Result.OK_LATEST: 1>, 'OK_NOT_YET_FOUND': <Result.OK_NOT_YET_FOUND: 2>, 'ERROR': <Result.ERROR: 3>, 'ERROR_CONNECTION': <Result.ERROR_CONNECTION: 4>, 'ERROR_NOT_SUPPORTED': <Result.ERROR_NOT_SUPPORTED: 5>, 'ERROR_ACCESS_DENIED': <Result.ERROR_ACCESS_DENIED: 6>, 'ERROR_NOT_FOUND': <Result.ERROR_NOT_FOUND: 7>, 'ERROR_BAD_VERSION': <Result.ERROR_BAD_VERSION: 8>, 'ERROR_ALREADY_EXISTS': <Result.ERROR_ALREADY_EXISTS: 9>, 'ERROR_SOURCE_IS_DEST': <Result.ERROR_SOURCE_IS_DEST: 10>, 'ERROR_ACCESS_LOST': <Result.ERROR_ACCESS_LOST: 11>, 'ERROR_LOCKED': <Result.ERROR_LOCKED: 12>, 'ERROR_BAD_REQUEST': <Result.ERROR_BAD_REQUEST: 13>, 'ERROR_FOLDER_NOT_EMPTY': <Result.ERROR_FOLDER_NOT_EMPTY: 14>, 'ERROR_WRONG_TYPE': <Result.ERROR_WRONG_TYPE: 15>, 'ERROR_UNSUPPORTED_VERSION': <Result.ERROR_UNSUPPORTED_VERSION: 16>}
    pass
class ServerInfo():
    @property
    def auth_token(self) -> str:
        """
        :type: str
        """
    @property
    def cache_enabled(self) -> bool:
        """
        :type: bool
        """
    @property
    def checkpoints_enabled(self) -> bool:
        """
        :type: bool
        """
    @property
    def connection_id(self) -> str:
        """
        :type: str
        """
    @property
    def omniojects_enabled(self) -> bool:
        """
        :type: bool
        """
    @property
    def undelete_enabled(self) -> bool:
        """
        :type: bool
        """
    @property
    def username(self) -> str:
        """
        :type: str
        """
    @property
    def version(self) -> str:
        """
        :type: str
        """
    pass
class Url():
    @property
    def fragment(self) -> str:
        """
        :type: str
        """
    @property
    def host(self) -> str:
        """
        :type: str
        """
    @property
    def is_raw(self) -> bool:
        """
        :type: bool
        """
    @property
    def path(self) -> str:
        """
        :type: str
        """
    @property
    def port(self) -> str:
        """
        :type: str
        """
    @property
    def query(self) -> str:
        """
        :type: str
        """
    @property
    def scheme(self) -> str:
        """
        :type: str
        """
    @property
    def user(self) -> str:
        """
        :type: str
        """
    pass
class WriteFileExInfo():
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def hash(self) -> str:
        """
        Provider-supplied file hash

        :type: str
        """
    @property
    def version(self) -> str:
        """
        Provider-supplied file version

        :type: str
        """
    pass
def add_bookmark(name: str, url: str) -> None:
    """
    Add a bookmark.
    """
def add_default_search_path(search_path: str) -> None:
    """
    Add a default search path to the list of search paths used by resolve

    New default search paths are added to the top of the stack (meaning they
    will match before older default search paths), but all default search paths
    are underneath the search_paths explicitly provided to resolve.

    If this search_path is already in the list, it is moved to the top
    """
def add_user_to_group_with_callback(url: str, user: str, group: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Add a user to a group
    """
def authentication_cancel(auth_handle: int) -> None:
    """
    Cancel an authentication attempt.

    This should be called in response to a user clicking the "Cancel" button in an authentication message box.

    Args:
       auth_handle: The parameter passed to a callback registered with set_authentication_message_box_callback.
    """
def break_url(url: str) -> Url:
    """
    Break a URL into components. This behaves similarly to :py:func:`omni.client.break_url_reference` except it assumes the URL is either a full URL (which starts with a scheme:) or is a raw local file path (such as "C:\\path" on Windows or "/path" on Linux). This affects how it handles special characters such as : % ? and #. If `is_raw` on the returned object is True, the URL was determined to be a raw local file path, and only the 'path' value is set.
    """
def break_url_reference(url: str) -> Url:
    """
    Break a URL into components. This should be used instead of :py:func:`omni.client.break_url` if you have a URL such as '/path?query#fragment' which could be interpreted as a local file path.
    """
def close_cached_file(handle: Request) -> None:
    """
    Close a file handle that was opened with `open_cached_file`. This allows it to be garbage collected.
    """
def combine_urls(base_url: str, other_url: str) -> str:
    """
    Returns a URL which is a combination of the `base_url` and `other_url`
    """
def combine_with_base_url(other_url: str) -> str:
    """
    This calls :py:func:`omni.client.combine_urls` with the URL on top of the base url stack.
    """
def copy_file_with_callback(src_url: str, dst_url: str, callback: typing.Callable[[Result], None], behavior: CopyBehavior = CopyBehavior.ERROR_IF_EXISTS, message: str = None) -> Request:
    """
    Copy a file from ``src_url`` to ``dst_url``.

                This operation will fail with Result.ERROR_WRONG_TYPE if the item at 'src_url' is not a file.
                It will fail with Result.ERROR_NOT_FOUND if the item at 'src_url' does not exist.
                It will fail with Result.ERROR_ALREADY_EXISTS if the item at 'dst_url' already exists and 'behavior' is CopyBehavior.ERROR_IF_EXISTS.
                It will also fail with Result.ERROR_ALREADY_EXISTS if the item at 'dst_url' exists but is not a file and 'behavior' is CopyBehavior.OVERWRITE.

                Destination folders will be created as needed.

                Args:
                    src_url: Source url.
                    dst_url: Destination url.
                    callback: Callback to be called with the result.
                    behavior: Behavior if the destination exists (CopyBehavior.ERROR_IF_EXISTS or CopyBehavior.OVERWRITE).
                    message: Message to apply to atomic checkpoint of destination url.

                Returns:
                    Request object.
            
    """
def copy_folder_with_callback(src_url: str, dst_url: str, callback: typing.Callable[[Result], None], behavior: CopyBehavior = CopyBehavior.ERROR_IF_EXISTS, message: str = None) -> Request:
    """
    Recursively copy a folder from ``src_url`` to ``dst_url``.

                This operation will fail with Result.ERROR_WRONG_TYPE if the item at 'src_url' is not a folder.
                It will fail with Result.ERROR_NOT_FOUND if the item at 'src_url' does not exist.
                It will fail with Result.ERROR_ALREADY_EXISTS if the item at 'dst_url' already exists and 'behavior' is CopyBehavior.ERROR_IF_EXISTS.
                It will also fail with Result.ERROR_ALREADY_EXISTS if the item at 'dst_url' exists but is not a folder and 'behavior' is CopyBehavior.OVERWRITE.

                Setting 'behavior' to CopyBehavior.OVERWRITE will overwrite each individual file inside the folder, but will not remove files that exist in 'dst_url' but not in 'src_url'.
                Trailing slashes are ignored.

                Destination folders will be created as needed.

                Args:
                    src_url: Source url.
                    dst_url: Destination url.
                    callback: Callback to be called with the result.
                    behavior: Behavior if the destination exists (CopyBehavior.ERROR_IF_EXISTS or CopyBehavior.OVERWRITE).
                    message: Message to apply to atomic checkpoint of destination url.

                Returns:
                    Request object.
            
    """
def copy_with_callback(src_url: str, dst_url: str, callback: typing.Callable[[Result], None], behavior: CopyBehavior = CopyBehavior.ERROR_IF_EXISTS, message: str = None) -> Request:
    """
    Copy a thing from ``src_url`` to ``dst_url``.

                This is equivalent to first checking the type of the item at 'src_url' and then copying it as either a file or a folder. The copy is done server-side if both 'src_url' and 'dst_url' are on the same server. Otherwise, it is downloaded from 'src_url' and then uploaded to 'dst_url'.

                Destination folders will be created as needed.

                Args:
                    src_url: Source url.
                    dst_url: Destination url.
                    callback: Callback to be called with the result.
                    behavior: Behavior if the destination exists (CopyBehavior.ERROR_IF_EXISTS or CopyBehavior.OVERWRITE).
                    message: Message to apply to atomic checkpoint of destination url.

                Returns:
                    Request object.
            
    """
def create_checkpoint_with_callback(*args, **kwargs) -> typing.Any:
    """
    Create a checkpoint.

    Args:
        url: URL of file to create a checkpoint for.
        comment: Comment to associate with the checkpoint
        force: Set to true to force creating a checkpoint even if there are no changes
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
def create_folder_with_callback(url: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Create a folder.

    Args:
        url: URL of folder to create.
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
def create_group_with_callback(url: str, group: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Create a group
    """
def create_with_hash_with_callback(url: str, hash: str, overwrite: bool, callback: typing.Callable[[Result, str, str], None], message: str = None) -> Request:
    """
    Create a new file with the content the hash describes, will fail if the server doesn't know the hash

    Args:
        url: URL of file to create.
        hash: Hash of content.
        overwrite: Allow to overwrite an existing file or fail if file exists
        callback: Callback to be called with the result.
        message: Message to apply to atomic checkpoint of destination url.

    Returns:
        Request object.
    """
def delete_single_with_callback(url: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Delete a file or an empty folder.

    result is ERROR_FOLDER_NOT_EMPTY if the folder is not empty.

    Args:
        url: URL of item to delete.
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
def delete_with_callback(url: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Delete something (file, folder, mount, live object, channel etc..).

    Args:
        url: URL of item to delete.
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
def enable_nucleus_cache_bypass(bypass_on: bool) -> bool:
    """
    For unit tests only. Enable/disable Nucleus Cache bypass.
    """
def get_acls_with_callback(url: str, callback: typing.Callable[[Result, typing.List[AclEntry]], None]) -> Request:
    """
    Get the ACL's on a folder/file.

    Args:
        url: URL of item to get the ACLs for.
        callback: Callback to be called with the result and current ACLs.

    Returns:
        Request object.
    """
def get_base_url() -> str:
    """
    Returns the top of the base URL stack

    Returns:
        str: The top of the base URL stack, or None if the stack is empty.
    """
def get_branch_and_checkpoint_from_query(query: str) -> tuple:
    """
    Extract the branch and checkpoint from a query parameter
    """
def get_default_search_paths() -> typing.List[str]:
    """
    Retrieve the current list of default search paths

    Returns:
        List[str]: The list of default search paths.
    """
def get_group_users_with_callback(url: str, group: str, callback: typing.Callable[[Result, typing.List[str]], None]) -> Request:
    """
    Get a list of users in a group
    """
def get_groups_with_callback(url: str, callback: typing.Callable[[Result, typing.List[str]], None]) -> Request:
    """
    Get a list of all groups
    """
def get_hub_http_uri_with_callback(callback: typing.Callable[[Result, str], None]) -> Request:
    pass
def get_hub_version_with_callback(callback: typing.Callable[[Result, str], None]) -> Request:
    pass
def get_local_file_with_callback(*args, **kwargs) -> typing.Any:
    """
    DEPRECATED: Use `open_cached_file_with_callback` instead
    """
def get_server_info_with_callback(url: str, callback: typing.Callable[[Result, ServerInfo], None]) -> Request:
    pass
def get_user_groups_with_callback(url: str, user: str, callback: typing.Callable[[Result, typing.List[str]], None]) -> Request:
    """
    Retrieve the groups a user is in
    """
def get_users_with_callback(url: str, callback: typing.Callable[[Result, typing.List[str]], None]) -> Request:
    """
    Get a list of all users
    """
def get_version() -> str:
    """
    Get the version of the client library being used.

    Returns:
        Returns a human readable version string.
    """
def initialize(version: int = 563186176622592) -> bool:
    """
    Initialize the client library.

    Returns:
        False if the library failed to initialize.
    """
def join_channel_with_callback(url: str, callback: typing.Callable[[Result, ChannelEvent, str, Content], None]) -> Request:
    """
    Join a channel.

    Args:
        url: URL of of channel to join.
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
def list_bookmarks_with_callback(callback: typing.Callable[[typing.Dict[str, str]], None]) -> Request:
    """
    Retrieve a list of bookmarks. The callback is called any time the list changes (by this application or any other).
    """
def list_checkpoints_with_callback(url: str, callback: typing.Callable[[Result, typing.List[ListEntry]], None]) -> Request:
    """
    List checkpoints.

    Args:
        url: URL of file to list the checkpoints of.
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
def list_subscribe_with_callback(url: str, list_callback: typing.Callable[[Result, typing.List[ListEntry]], None], subscribe_callback: typing.Callable[[Result, ListEvent, ListEntry], None], include_deleted_option: ListIncludeOption = ListIncludeOption.NO_DELETED_FILES) -> Request:
    """
    Subscribe to change notifications for a url.

    '`list_callback` is called once with the initial list,
     then '`subscribe_callback`' may be called multiple times after that as items change.

    Args:
        url: URL of folder to subscribe to.
        list_callback: Callback to be called once with the list.
        subscribe_callback: Callback to be called when changes happen.
        include_deleted_option: Option to include deleted files (default NO_DELETED_FILES)

    Returns:
        Subscription object.
    """
def list_with_callback(url: str, callback: typing.Callable[[Result, typing.List[ListEntry]], None], include_deleted_option: ListIncludeOption = ListIncludeOption.NO_DELETED_FILES) -> Request:
    """
    List content of a folder.

    Args:
        url: URL of a folder to list the contents of.
        callback: Callback to be called with the results.
        include_deleted_option: Option to include deleted files (default NO_DELETED_FILES)

    Returns:
        Request object.
    """
def live_get_latest_server_time(url: str) -> int:
    """
    Returns the server timestamp of the most recently received message (0 if no messages have been received).
    """
def live_process() -> None:
    """
    Call this to process live updates received from the server.
    """
def live_process_up_to(url: str, server_time: int) -> None:
    """
    Same as :py:func:`omni.client.live_process` but you can specify a server time to stop processing updates.

    Args:
        url: URL of server to process updates for. Only the "prefix" such as "omniverse://content.ov.nvidia.com" is needed, the path is ignored.
        server_time: The server time to stop processing updates.
    """
def live_register_queued_callback(callback: typing.Callable[[LiveUpdateType, Result, str, int, int, int], None]) -> Registration:
    """
    Register a function which is called any time we receive a live update.

    This is intended to notify you that you should call live_process.
    DO NOT call :py:func:`omni.client.live_process` from within the callback function!

    Args:
        callback: Callback to be called with information about the live update that was queued.

    Returns:
        Subscription Object. Callback will be unregistered once subcription is released.
    """
def live_set_queued_callback(callback: typing.Callable[[], None]) -> None:
    """
    DEPRECATED: Use live_register_queued_callback
    """
def live_wait_for_pending_updates() -> None:
    """
    Call this to wait for all pending live updates to complete.
    """
def lock_with_callback(url: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Lock a URL so only this client can modify it.

    Args:
        url: URL of file to lock.
    """
def make_file_url(path: str) -> str:
    """
    Returns a "file:" URL for the given `path`
    """
def make_printable(url: str) -> str:
    """
    Returns a url which is safe for printing (no invalid UTF-8 sequences or ASCII control characters)
    """
def make_query_from_branch_and_checkpoint(branch: str, checkpoint: int) -> str:
    """
    Return a query parameter that can be used to reference this branch and checkpoint
    """
def make_relative_url(base_url: str, other_url: str) -> str:
    """
    Returns a url which will result in `other_url`, when combined with `base_url`
    """
def make_url(scheme: str = None, user: str = None, host: str = None, port: str = None, path: str = None, query: str = None, fragment: str = None) -> str:
    """
    Compose a URL from the provided parts
    """
def move_file_with_callback(src_url: str, dst_url: str, callback: typing.Callable[[Result, bool], None], behavior: CopyBehavior = CopyBehavior.ERROR_IF_EXISTS, message: str = None) -> Request:
    """
    Move a file from ``src_url`` to ``dst_url``.

                This operation fails with Result.ERROR_WRONG_TYPE if 'src_url' is not a file, with Result.ERROR_NOT_FOUND if 'src_url' doesn't exist, and with Result.ERROR_ALREADY_EXISTS if 'dst_url' already exists and 'behavior' is CopyBehavior.ERROR_IF_EXISTS, or if 'dst_url' exists but is not a file and 'behavior' is CopyBehavior.OVERWRITE.

                The move is done server-side if both 'src_url' and 'dst_url' are on the same server. Otherwise, it is first copied, then deleted from 'src_url'. Note: It is possible for the copy to succeed and the delete to fail.

                Destination (parent) folders will be created as needed.

                Args:
                    src_url: Source url.
                    dst_url: Destination url.
                    callback: Callback to be called with the result.
                    behavior: Behavior if the destination exists (CopyBehavior.ERROR_IF_EXISTS or CopyBehavior.OVERWRITE).
                    message: Message to apply to atomic checkpoint of destination url.

                Returns:
                    Request object.
            
    """
def move_folder_with_callback(src_url: str, dst_url: str, callback: typing.Callable[[Result, bool], None], behavior: CopyBehavior = CopyBehavior.ERROR_IF_EXISTS, message: str = None) -> Request:
    """
    Recursively move a folder from ``src_url`` to ``dst_url``.

                This operation fails with Result.ERROR_WRONG_TYPE if 'src_url' is not a folder, with Result.ERROR_NOT_FOUND if 'src_url' doesn't exist, and with Result.ERROR_ALREADY_EXISTS if 'dst_url' already exists and 'behavior' is CopyBehavior.ERROR_IF_EXISTS, or if 'dst_url' exists but is not a folder and 'behavior' is CopyBehavior.OVERWRITE.

                The move is done server-side if both 'src_url' and 'dst_url' are on the same server. Otherwise, it is first copied, then deleted from 'src_url'. Note: It is possible for the copy to succeed and the delete to fail.

                Destination folders will be created as needed. Trailing slashes are ignored.

                Args:
                    src_url: Source url.
                    dst_url: Destination url.
                    callback: Callback to be called with the result.
                    behavior: Behavior if the destination exists (CopyBehavior.ERROR_IF_EXISTS or CopyBehavior.OVERWRITE).
                    message: Message to apply to atomic checkpoint of destination url.

                Returns:
                    Request object.
    """
def move_with_callback(src_url: str, dst_url: str, callback: typing.Callable[[Result, bool], None], behavior: CopyBehavior = CopyBehavior.ERROR_IF_EXISTS, message: str = None) -> Request:
    """
    Move a thing from ``src_url`` to ``dst_url``.

                This is equivalent to first checking the type of the item at 'src_url' and then moving it as either a file or a folder. The move is done server-side if both 'src_url' and 'dst_url' are on the same server. Otherwise, it is first copied from 'src_url' to 'dst_url', then deleted from 'src_url'.

                Note: It is possible for the copy to succeed and the delete to fail, resulting in an error code but with 'copied' being true. If the result is 'Ok' but 'copied' is false, the move was done entirely on the server, and no local copy was made.

                Destination folders will be created as needed.

                Args:
                    src_url: Source url.
                    dst_url: Destination url.
                    callback: Callback to be called with the result.
                    behavior: Behavior if the destination exists (CopyBehavior.ERROR_IF_EXISTS or CopyBehavior.OVERWRITE).
                    message: Message to apply to atomic checkpoint of destination url.

                Returns:
                    Request object.
            
    """
def normalize_url(url: str) -> str:
    """
    Normalize a URL by parsing it then recomposing it
    """
def obliterate_with_callback(url: str, obliterate_checkpoints: bool, callback: typing.Callable[[Result], None]) -> Request:
    """
    Obliterate a path

    Doesn't support recursive removal, doesn't support wildcards
    Supports branches / checkpoints
    Only empty folders can be obliterated

    Args:
        url: URL of item to delete.
        obliterate_checkpoints: whether to obliterate all checkpoints
        callback: Callback to be called with the result of the operation

    Returns:
        Request object.
    """
def open_cached_file_with_callback(*args, **kwargs) -> typing.Any:
    """
    Open a file from the cache.

    Args:
        url: URL of file to get the local path for.
        download: Pass false to get an error if the file has not already been downloaded.
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
@typing.overload
def pop_base_url(url: str) -> bool:
    """
    Pop a URL off the base stack


            Pop a base URL from the context stack

            If the provided base_url is not on the top of the stack,
            a warning will be printed to the log and this will return false
            
    """
@typing.overload
def pop_base_url(base_url: str) -> bool:
    pass
@typing.overload
def push_base_url(url: str) -> None:
    """
    Push a URL onto the base stack, which is used with :py:func:`omni.client.combine_with_base_url`.


            Push a base URL for relative URLs to resolve against

            For example if you push "omniverse://sandbox.ov.nvidia.com/path/" then calling omniList("subpath") will
            return the results of "omniverse://sandbox.ov.nvidia.com/path/subpath". The rules for combining a relative
            URL with a base URLs is defined by https://tools.ietf.org/html/rfc3986 and is the same
            as a web browser would resolve links inside an HTML document.
            ** Note that the context is per-thread! **
            This is so you can load resources on different threads with different base URLs
            
    """
@typing.overload
def push_base_url(base_url: str) -> None:
    pass
def read_file_with_callback(url: str, callback: typing.Callable[[Result, str, Content], None]) -> Request:
    """
    Read a file.

    Args:
        url: URL of file to read.
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
def reconnect(url: str) -> None:
    """
    Reconnect to a URL after a failure.

    The client library doesn't automatically reconnect after failures.

    This triggers a background reconnect attempt, you can call a function such as :py:func:`omni.client.stat`
    or use :py:func:`omni.client.register_connection_status_callback` to determine if the reconnect attempt was successful.

    Args:
        url: Attempt to connect to this url
    """
def refresh_auth_token_with_callback(url: str, callback: typing.Callable[[Result, str], None]) -> Request:
    pass
def register_authentication_callback(callback: typing.Callable[[str], object]) -> Registration:
    """
    Register an authentication callback.

    The callback receives the URL prefix (such as "scheme://server:port") as a parameter and should return either:
        * None or False: Continue to the next authentication callback (or default authentication method)
        * AUTH_ABORT: Abort the connection
        * String: The auth_token provided by get_server_info
        * (String, String): A username and password or you can return ("$omni-api-token", api_token) to use an API token.

    Args:
        callback: Callback to be called to provide authentication information.

    Returns:
        Subscription Object. Callback will be unregistered once subscription is released.
    """
def register_authorize_callback(callback: typing.Callable[[str], object]) -> Registration:
    """
    DEPRECATED - Use register_authentication_callback
    """
def register_connection_status_callback(callback: typing.Callable[[str, ConnectionStatus], None]) -> Registration:
    """
    Register connection status callback.

    Args:
        callback: Callback to be called with the status.

    Returns:
        Subscription Object. Callback will be unregistered once subcription is released.
    """
def register_device_flow_auth_callback(callback: typing.Callable[[int, AuthDeviceFlowParams], None]) -> Registration:
    """
    Register a "Device Flow" authentication callback.

    If _any_ device flow auth callbacks are registered, we will prefer the device flow auth instead of launching a web browser.

    Args:
        callback: Function to be called when performing device flow auth.
                  It receives an auth handle (which can be used with `authentication_cancel`) and an object containing information
                  that you should display to the user to complete authentication. If the information object is `None` that means the
                  authentication attempt is finished (either sucessfully or not, register a connection status callback to determine which).

    Returns:
        Subscription Object. The callback will be unregistered once subcription is released.
    """
def register_file_status_callback(callback: typing.Callable[[str, FileStatus, int], None]) -> Registration:
    """
    Register file status callback.

    Args:
        callback: Callback to be called with the status.

    Returns:
        Subscription Object. Callback will be unregistered once subcription is released.
    """
def remove_bookmark(name: str) -> None:
    """
    Remove a bookmark.
    """
def remove_default_search_path(search_path: str) -> None:
    """
    Remove a default search path from the list of search paths used by resolve
    """
def remove_group_with_callback(url: str, group: str, callback: typing.Callable[[Result, int], None]) -> Request:
    """
    Remove a group
    """
def remove_user_from_group_with_callback(url: str, user: str, group: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Remove a user from a group
    """
def rename_group_with_callback(url: str, group: str, new_group: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Rename a group
    """
def resolve_subscribe_with_callback(url: str, search_urls: typing.List[str], resolve_callback: typing.Callable[[Result, ListEntry, str], None], subscribe_callback: typing.Callable[[Result, ListEvent, ListEntry, str], None]) -> Request:
    """
    Resolve & subscribe to change notifications for a url.

    Performs 'resolve' but also establishes a subscription so you will be notified if the resolution changes.

    If 'url' is a file, you will only receive information about that file.
    If 'url' is a folder, you will only receive information about that folder.
    '`resolve_callback` is called once with the initial resolve info,then '`subscribe_callback`' may be called multiple times after that the item changes.

    Args:
        url: URL of item to resolve. Can be relative or absolute.
        search_urls: List of URLs to search.
        resolve_callback: Callback to be called once with the initial resolve.
        subscribe_callback: Callback to be called when changes happen.

    Returns:
        Subscrption object.
    """
def resolve_with_callback(url: str, search_urls: typing.List[str], callback: typing.Callable[[Result, ListEntry, str], None]) -> Request:
    """
    Resolve a file or folder, looking in the search paths for it.

    Args:
        url: URL of item to resolve.
        search_urls: List of URLs to search.
        callback: Callback to be called with the results.

    Returns:
        Request object.
    """
def send_message_with_callback(join_request_id: int, content: buffer, callback: typing.Callable[[Result], None]) -> Request:
    """
    Send a message to a channel.

    Args:
        join_request_id: Request.id that you get from :py:func:`omni.client.join_channel_with_callback`.
        content: Message content.
        callback: Callback to be called with the result.

    Returns:
        Request object.
    """
def set_acls_with_callback(url: str, acls: list, callback: typing.Callable[[Result], None]) -> Request:
    """
    Set the ACL's on a folder/file.

    Args:
        url: URL of item to set the ACLs on.
        acls: the complete new set of ACLs to put on this file/folder.
        callback: Callback to be called with the result and current ACLs.

    Returns:
        Request object.
    """
def set_alias(alias: str, value: str) -> None:
    """
    Redirect a URL to a different location

              For example set_alias("home:", "C:\\Users\\myname\\")

              Call it with value=None to remove an alias
              
    """
def set_authentication_message_box_callback(callback: typing.Callable[[bool, str, int], None]) -> None:
    """
    Set an authentication message box callback.

    The callback is called when authentication requires opening a web browser to complete sign-in.
        The intention is the application should show a dialog box letting the user know that
        a browser window has opened and to complete signing in using the web browser.

    The callback receives:
        1. A bool indicating if the dialog should be shown (true) or hidden (false)
        2. A string indicating the host name that the authentication is for
        3. An integer handle that can be passed to "authentication_cancel"

    You should provide a "Cancel" button and call cancel_authentication if the user presses it.

    You should call this function with "None" as the callback prior to shutting down in order to
        free memory that was allocated to store the callback.

    Note that signing in to multiple servers simultaneously is allowed, so you may receive multiple
    callbacks with "show" set to true before receiving them with "show" set to false. You can use
    the third parameter to track show/hide pairs.

    Args:
        callback: Callback to be called to show an authentication dialog box.
    """
def set_azure_sas_token(host: str, container: str, sasToken: str, writeConfig: bool) -> Result:
    """
    Set SAS Token for an Azure blob container.

    Returns:
        OmniClient Result code.
    """
def set_hang_detection_time_ms(timeout: int) -> None:
    """
    Configure the amount of time to wait before the blocking versions of each function prints a warning (and a stack trace).

    Note: this value must be set per thread!

    Args:
        timeout: The amount of time to wait (in milliseconds) before printing a warning.
    """
def set_http_header(key: str, value: str) -> None:
    """
    Set a header to pass along with any http request.

                Pass None as the value to clear a previously set header.

                Note: This does not apply to redirects from Nucleus
                
    """
def set_log_callback(callback: typing.Callable[[str, str, LogLevel, str], None]) -> None:
    """
    Set a function which is called any time the library wants to print something to the log.

                .. code:: python

                    def log_callback(thread, component, level, message):
                        print(f"{thread} {component} {level} {message}")

                    omni.client.set_log_callback(log_callback)

                
    """
def set_log_level(log_level: LogLevel) -> None:
    """
    Only print messages to the log which are at this level or higher.
    """
def set_product_info(name: str, version: str = None, extra: str = None) -> None:
    """
    Sets product information that's sent to Nucleus when connecting.

    If you're already connected to a Nucleus server, this will not send the new values to the server,
    only on reconnect. For this reason you should set the product information prior to connecting.

    This is also used in Hub to help identify which applications are running.

    Args:
        name: The human-readable name of this product.
        version: The version of this product (not the client library version).
        extra: Any additional information you think would be valuable in the logs.
    """
def set_retries(max_ms: int, base_ms: int, jitter_ms: int) -> tuple:
    """
    Set parameters to control retry behavior

    The formula for delay between retries is:
    delay_ms = (base_ms * count * count) + rand(0, jitter_ms * count)
    where "count" is the current retry count.

    Retries are aborted after 'max_ms' time has elapsed. Set it to 0 to disable retries.

    Default behavior is:
    max_ms = 120,000
    base_ms = 100
    jitter_ms = 100

    Returns previous values in a tuple of (max_ms, base_ms, jitter_ms).
    """
def set_s3_configuration(url: str, bucket: str = None, region: str = None, accessKeyId: str = None, secretAccessKey: str = None, sessionToken: str = None, cloudfrontUrl: str = None, cloudfrontForList: bool = False, writeConfig: bool = False) -> Result:
    """
    Set S3 configuration for a host.

    Note that for backwards compatibilty reasons, the host parameter is named 'url' and can accept either a plain host name or a url.

    Returns:
        OmniClient Result code.
    """
def shutdown() -> None:
    """
    Shut down the client library.

    It is not safe to call any client library functions after calling shutdown.
    """
def sign_out(url: str) -> None:
    """
    Sign out specific url connection.

    Args:
        url: Immediately disconnect from the server specified by this URL.
             Any outstanding requests will call their callbacks with OmniClientResult.ErrorConnection.
             Additionally, clear the saved authentication token so future requests to this server will
             trigger re-authentication.
    """
def stat_subscribe_with_callback(url: str, stat_callback: typing.Callable[[Result, ListEntry], None], subscribe_callback: typing.Callable[[Result, ListEvent, ListEntry], None], include_deleted_option: ListIncludeOption = ListIncludeOption.NO_DELETED_FILES) -> Request:
    """
    Subscribe to change notifications for a url.

    If 'url' is a file, you will only receive information about that file.
    If 'url' is a folder, you will only receive information about that folder.
    '`stat_callback` is called once with the initial stat info,then '`subscribe_callback`' may be called multiple times after that the item changes.

    Args:
        url: URL of item to stat.
        stat_callback: Callback to be called once with the initial stat.
        subscribe_callback: Callback to be called when changes happen.
        include_deleted_option: Option to include deleted files (default NO_DELETED_FILES)

    Returns:
        Subscription object.
    """
def stat_with_callback(url: str, callback: typing.Callable[[Result, ListEntry], None], include_deleted_option: ListIncludeOption = ListIncludeOption.NO_DELETED_FILES) -> Request:
    """
    Retrieve information about a file or folder.

    Args:
        url: URL of item to stat.
        callback: Callback to be called with the results.
        include_deleted_option: Option to include deleted files (default NO_DELETED_FILES)

    Returns:
        Request object.
    """
def trace_start() -> bool:
    """
    Start tracing using carb::tracer

    If the client library is already using tracer, this does nothing.

    Tracing is automatically started when the library is loaded, but this function may be used if the tracer library
    was downloaded and installed while the application was running.

    Returns True if tracing was enabled (or was already enabled).
    """
def trace_stop() -> None:
    """
    Stop tracing using carb::tracer

    If the client library is not using tracer, this does nothing.

    Tracing is automatically stopped when the library is unloaded, but this can be used to stop it early.
    """
def undelete_with_callback(url: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Undelete soft-deleted paths

    Supports recursive parents undelete

    Args:
        url: URL of item to undelete
        callback: Callback to be called with the result of the operation

    Returns:
        Request object.
    """
def unlock_with_callback(url: str, callback: typing.Callable[[Result], None]) -> Request:
    """
    Unlock a URL so other clients can modify it.

    Args:
        url: URL of file to unlock.
    """
def write_file_ex_with_callback(url: str, content: buffer, callback: typing.Callable[[Result, WriteFileExInfo], None], message: str = None, skip_checkpoint: bool = False) -> Request:
    """
    Create a new file, overwriting if it already exists.

    Args:
        url: URL of file to create.
        content: File content.
        callback: Callback to be called with the result.
        message: Message to apply to atomic checkpoint of destination url.
        skip_checkpoint: If true, a checkpoint will not be created.
                         This is a destructive operation that will not keep the history of the changes.

    Returns:
        Request object.
    """
def write_file_with_callback(url: str, content: buffer, callback: typing.Callable[[Result], None], message: str = None) -> Request:
    """
    Create a new file, overwriting if it already exists.

    Args:
        url: URL of file to create.
        content: File content.
        callback: Callback to be called with the result.
        message: Message to apply to atomic checkpoint of destination url.

    Returns:
        Request object.
    """
VERSION = 563186176622592
