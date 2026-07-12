#!/usr/bin/env bash

set -Eeo pipefail

die() {
    printf 'Error: %s\n' "$*" >&2
    exit 1
}

require_command() {
    command -v "$1" >/dev/null 2>&1 || die "Required command '$1' was not found."
}

require_variable() {
    local name=$1
    [[ -n "${!name:-}" ]] || die "Required environment variable '$name' is not set."
}

prepare_xauthority() {
    local authority_dir authority_file authority_records candidate snapshot temporary_file
    local container_key=${CONT_NAME#/}
    local -a authority_candidates=()
    local -a checked_candidates=()

    authority_dir="${XDG_RUNTIME_DIR:-${HOME}/.cache}/isaaclab-docker"
    authority_file="${authority_dir}/${container_key}.xauth"

    if [[ -n "${XAUTHORITY:-}" ]]; then
        authority_candidates+=("$XAUTHORITY")
    fi
    if [[ -n "${XDG_RUNTIME_DIR:-}" ]]; then
        authority_candidates+=("${XDG_RUNTIME_DIR}/gdm/Xauthority")
    fi
    authority_candidates+=("${HOME}/.Xauthority")

    umask 077
    mkdir -p "$authority_dir"

    authority_records=
    for candidate in "${authority_candidates[@]}"; do
        checked_candidates+=("$candidate")
        [[ -r "$candidate" ]] || continue

        snapshot=$(mktemp "${authority_dir}/source.XXXXXX") ||
            die 'Unable to create a temporary X11 authority snapshot.'
        if ! cp -- "$candidate" "$snapshot"; then
            rm -f -- "$snapshot"
            continue
        fi

        authority_records=$(xauth -f "$snapshot" nlist "$DISPLAY" 2>/dev/null || true)
        rm -f -- "$snapshot"
        [[ -n "$authority_records" ]] && break
    done

    if [[ -z "$authority_records" ]]; then
        printf "Error: No X11 authorization cookie was found for DISPLAY='%s'. Checked:\n" "$DISPLAY" >&2
        printf '  %s\n' "${checked_candidates[@]}" >&2
        exit 1
    fi

    temporary_file=$(mktemp "${authority_file}.tmp.XXXXXX") ||
        die 'Unable to create the container X11 authority file.'

    # FamilyWild lets the cookie remain valid when the container hostname differs.
    printf '%s\n' "$authority_records" |
        sed -e 's/^..../ffff/' |
        xauth -f "$temporary_file" nmerge - || {
            rm -f -- "$temporary_file"
            die 'Unable to populate the container X11 authority file.'
        }

    chmod 600 "$temporary_file"
    mv -f -- "$temporary_file" "$authority_file"
    printf '%s\n' "$authority_file"
}

for command_name in docker nvidia-smi sed xauth; do
    require_command "$command_name"
done

for variable_name in \
    CONT_NAME IMG_NAME \
    VOL_DATA VOL_DATA2 VOL_RVI VOL_MINE VOL_RVI2 VOL_MINE2 VOL_MINE3 VOL_DEV \
    PORT_TB PORT_SSH PORT_JUP PORT_JUP_; do
    require_variable "$variable_name"
done

[[ "$CONT_NAME" =~ ^[a-zA-Z0-9][a-zA-Z0-9_.-]*$ ]] ||
    die "CONT_NAME='$CONT_NAME' is not a valid Docker container name."

[[ -n "${DISPLAY:-}" ]] || die 'DISPLAY is not set. Run this script from the local graphical desktop.'

display_number=${DISPLAY#*:}
display_number=${display_number%%.*}
[[ "$display_number" =~ ^[0-9]+$ ]] ||
    die "DISPLAY='$DISPLAY' is not a supported local X11 display."

x11_socket_dir=${X11_SOCKET_DIR:-/tmp/.X11-unix}
[[ -S "${x11_socket_dir}/X${display_number}" ]] ||
    die "X11 socket '${x11_socket_dir}/X${display_number}' does not exist."

docker info >/dev/null 2>&1 || die 'The Docker daemon is unavailable to the current user.'
nvidia-smi >/dev/null 2>&1 || die 'NVIDIA GPU access is unavailable on the host.'

if container_status=$(docker container inspect --format '{{.State.Status}}' "$CONT_NAME" 2>/dev/null); then
    printf "Container '%s' already exists (status: %s).\n" "$CONT_NAME" "$container_status" >&2
    if [[ "$container_status" == running ]]; then
        printf 'Use the existing container without changing it:\n' >&2
        printf '  docker exec -it "%s" bash\n' "$CONT_NAME" >&2
        printf 'Or stop it before explicitly replacing it:\n' >&2
        printf '  docker stop "%s"\n' "$CONT_NAME" >&2
        printf '  docker rm "%s"\n' "$CONT_NAME" >&2
    else
        printf 'Start it if its configuration is still valid:\n' >&2
        printf '  docker start "%s"\n' "$CONT_NAME" >&2
        printf 'Or remove/rename it before creating a replacement:\n' >&2
        printf '  docker rm "%s"\n' "$CONT_NAME" >&2
    fi
    printf 'To preserve it under another name:\n' >&2
    printf '  docker rename "%s" "%s-backup"\n' "$CONT_NAME" "$CONT_NAME" >&2
    exit 1
fi

host_xauthority=$(prepare_xauthority)

docker_args=(
    run --gpus all -id
    --security-opt seccomp=unconfined
    --name "$CONT_NAME"
    -e NVIDIA_VISIBLE_DEVICES=all
    -e NVIDIA_DRIVER_CAPABILITIES=all
    -e ACCEPT_EULA=Y
    -e PRIVACY_CONSENT=Y
    -e "DISPLAY=$DISPLAY"
    -e XAUTHORITY=/tmp/.docker.xauth
    -e QT_X11_NO_MITSHM=1
    -v /usr/share/vulkan/icd.d:/usr/share/vulkan/icd.d:ro
    -v /etc/vulkan/icd.d:/etc/vulkan/icd.d:ro
    -v /usr/share/nvidia:/usr/share/nvidia:ro
    -v "${x11_socket_dir}:/tmp/.X11-unix:rw"
    -v "${host_xauthority}:/tmp/.docker.xauth:ro"
    -v "${HOME}/docker/isaac-sim/cache/kit:/isaac-sim/kit/cache:rw"
    -v "${HOME}/docker/isaac-sim/cache/ov:/root/.cache/ov:rw"
    -v "${HOME}/docker/isaac-sim/cache/pip:/root/.cache/pip:rw"
    -v "${HOME}/docker/isaac-sim/cache/glcache:/root/.cache/nvidia/GLCache:rw"
    -v "${HOME}/docker/isaac-sim/cache/computecache:/root/.nv/ComputeCache:rw"
    -v "${HOME}/docker/isaac-sim/logs:/root/.nvidia-omniverse/logs:rw"
    -v "${HOME}/docker/isaac-sim/data:/root/.local/share/ov/data:rw"
    -v "${HOME}/docker/isaac-sim/documents:/root/Documents:rw"
    -v "$VOL_DATA"
    -v "$VOL_DATA2"
    -v "$VOL_RVI"
    -v "$VOL_MINE"
    -v "$VOL_RVI2"
    -v "$VOL_MINE2"
    -v "$VOL_MINE3"
    -v "$VOL_DEV"
)

if [[ -n "${VOL_ETC:-}" ]]; then
    docker_args+=(-v "$VOL_ETC")
fi

docker_args+=(
    --ipc host
    --restart unless-stopped
    -p "${PORT_TB}:6006"
    -p "${PORT_SSH}:22"
    -p "${PORT_JUP}:8888"
    -p "${PORT_JUP_}:8889"
    "$IMG_NAME"
)

if ! container_id=$(docker "${docker_args[@]}"); then
    die "Container creation failed. Check the Docker error above; if '$CONT_NAME' was created, remove or rename it before retrying."
fi

printf '%s\n' "$container_id"
