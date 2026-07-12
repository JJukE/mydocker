#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
RUN_SCRIPT="${SCRIPT_DIR}/../docker_run.sh"
TEST_COUNT=0

fail() {
    printf 'FAIL: %s\n' "$1" >&2
    exit 1
}

assert_contains() {
    local haystack=$1
    local needle=$2
    [[ "$haystack" == *"$needle"* ]] || fail "expected output to contain: ${needle}"
}

assert_not_contains() {
    local haystack=$1
    local needle=$2
    [[ "$haystack" != *"$needle"* ]] || fail "expected output not to contain: ${needle}"
}

make_fixture() {
    FIXTURE=$(mktemp -d)
    export FIXTURE
    export HOME="${FIXTURE}/home"
    export XDG_RUNTIME_DIR="${FIXTURE}/runtime"
    export X11_SOCKET_DIR=/tmp/.X11-unix
    export MOCK_DOCKER_LOG="${FIXTURE}/docker.log"
    mkdir -p "$HOME" "$XDG_RUNTIME_DIR" "${FIXTURE}/bin"
    : >"$MOCK_DOCKER_LOG"
    printf '%s\n' '0100 HOME_COOKIE' >"${HOME}/.Xauthority"

    cat >"${FIXTURE}/bin/docker" <<'EOF'
#!/usr/bin/env bash
printf '%s\n' "$*" >>"$MOCK_DOCKER_LOG"
if [[ "${1:-}" == info ]]; then
    exit 0
fi
if [[ "${1:-}" == container && "${2:-}" == inspect ]]; then
    if [[ "${MOCK_CONTAINER_EXISTS:-0}" == 1 ]]; then
        printf '%s\n' "${MOCK_CONTAINER_STATUS:-created}"
        exit 0
    fi
    exit 1
fi
if [[ "${1:-}" == run ]]; then
    printf '%s\n' mock-container-id
    exit 0
fi
exit 0
EOF

    cat >"${FIXTURE}/bin/xauth" <<'EOF'
#!/usr/bin/env bash
authority_file=${XAUTHORITY:-${HOME}/.Xauthority}
if [[ "${1:-}" == -f ]]; then
    authority_file=${2:?}
    shift 2
fi
case "${1:-}" in
    nlist)
        [[ -f "$authority_file" ]] && cat "$authority_file"
        ;;
    nmerge)
        cat >"$authority_file"
        ;;
esac
EOF

    cat >"${FIXTURE}/bin/nvidia-smi" <<'EOF'
#!/usr/bin/env bash
exit 0
EOF

    chmod +x "${FIXTURE}/bin/docker" "${FIXTURE}/bin/xauth" "${FIXTURE}/bin/nvidia-smi"
    export PATH="${FIXTURE}/bin:${ORIGINAL_PATH}"
    export DISPLAY=:1
    export CONT_NAME=test_isaaclab
    export IMG_NAME=test-image
    export VOL_DATA=/host/data:/root/data
    export VOL_DATA2=/host/data2:/root/data2
    export VOL_RVI=/host/rvi:/root/rvi
    export VOL_MINE=/host/mine:/root/mine
    export VOL_RVI2=/host/rvi2:/root/rvi2
    export VOL_MINE2=/host/mine2:/root/mine2
    export VOL_MINE3=/host/mine3:/root/mine3
    export VOL_DEV=/host/dev:/root/dev
    unset VOL_ETC
    export PORT_TB=16006
    export PORT_SSH=10022
    export PORT_JUP=18888
    export PORT_JUP_=18889
    unset XAUTHORITY
    unset MOCK_CONTAINER_EXISTS MOCK_CONTAINER_STATUS
}

cleanup_fixture() {
    rm -rf "$FIXTURE"
}

run_test() {
    local name=$1
    shift
    make_fixture
    if "$@"; then
        TEST_COUNT=$((TEST_COUNT + 1))
        printf 'PASS: %s\n' "$name"
    else
        cleanup_fixture
        fail "$name"
    fi
    cleanup_fixture
}

test_rejects_missing_display() {
    unset DISPLAY
    local output status
    set +e
    output=$(bash "$RUN_SCRIPT" 2>&1)
    status=$?
    set -e
    [[ $status -ne 0 ]] || fail 'missing DISPLAY should fail'
    assert_contains "$output" 'DISPLAY is not set'
    assert_not_contains "$(cat "$MOCK_DOCKER_LOG")" 'run '
}

test_rejects_unsafe_container_name() {
    export CONT_NAME=../unsafe
    local output status
    set +e
    output=$(bash "$RUN_SCRIPT" 2>&1)
    status=$?
    set -e
    [[ $status -ne 0 ]] || fail 'unsafe container name should fail'
    assert_contains "$output" 'CONT_NAME'
    assert_not_contains "$(cat "$MOCK_DOCKER_LOG")" 'run '
}

test_preserves_existing_container() {
    export MOCK_CONTAINER_EXISTS=1
    export MOCK_CONTAINER_STATUS=created
    local output status
    set +e
    output=$(bash "$RUN_SCRIPT" 2>&1)
    status=$?
    set -e
    [[ $status -ne 0 ]] || fail 'existing container should stop creation'
    assert_contains "$output" "Container 'test_isaaclab' already exists (status: created)"
    assert_contains "$output" 'docker rm "test_isaaclab"'
    assert_not_contains "$(cat "$MOCK_DOCKER_LOG")" 'run '
}

test_guides_running_container_reuse() {
    export MOCK_CONTAINER_EXISTS=1
    export MOCK_CONTAINER_STATUS=running
    local output status
    set +e
    output=$(bash "$RUN_SCRIPT" 2>&1)
    status=$?
    set -e
    [[ $status -ne 0 ]] || fail 'running container should stop creation'
    assert_contains "$output" 'docker exec -it "test_isaaclab" bash'
    assert_contains "$output" 'docker stop "test_isaaclab"'
    assert_not_contains "$(cat "$MOCK_DOCKER_LOG")" 'run '
}

test_uses_neutral_xauthority_mount() {
    local output
    output=$(bash "$RUN_SCRIPT" 2>&1)
    assert_contains "$output" 'mock-container-id'
    local docker_log
    docker_log=$(cat "$MOCK_DOCKER_LOG")
    assert_contains "$docker_log" 'run --gpus all'
    assert_contains "$docker_log" 'XAUTHORITY=/tmp/.docker.xauth'
    assert_contains "$docker_log" ":/tmp/.docker.xauth:ro"
    assert_not_contains "$docker_log" '/root/.Xauthority'
    [[ -s "${XDG_RUNTIME_DIR}/isaaclab-docker/test_isaaclab.xauth" ]] || fail 'generated authority file is empty'
}

test_prefers_explicit_xauthority() {
    export XAUTHORITY="${FIXTURE}/explicit.Xauthority"
    printf '%s\n' '0100 EXPLICIT_COOKIE' >"$XAUTHORITY"
    mkdir -p "${XDG_RUNTIME_DIR}/gdm"
    printf '%s\n' '0100 GDM_COOKIE' >"${XDG_RUNTIME_DIR}/gdm/Xauthority"

    bash "$RUN_SCRIPT" >/dev/null

    local generated="${XDG_RUNTIME_DIR}/isaaclab-docker/test_isaaclab.xauth"
    assert_contains "$(cat "$generated")" 'EXPLICIT_COOKIE'
    assert_not_contains "$(cat "$generated")" 'GDM_COOKIE'
}

test_falls_back_to_gdm_xauthority() {
    : >"${HOME}/.Xauthority"
    mkdir -p "${XDG_RUNTIME_DIR}/gdm"
    printf '%s\n' '0100 GDM_COOKIE' >"${XDG_RUNTIME_DIR}/gdm/Xauthority"

    bash "$RUN_SCRIPT" >/dev/null

    local generated="${XDG_RUNTIME_DIR}/isaaclab-docker/test_isaaclab.xauth"
    assert_contains "$(cat "$generated")" 'GDM_COOKIE'
}

test_reports_checked_authority_files() {
    : >"${HOME}/.Xauthority"
    local output status
    set +e
    output=$(bash "$RUN_SCRIPT" 2>&1)
    status=$?
    set -e
    [[ $status -ne 0 ]] || fail 'missing display cookie should fail'
    assert_contains "$output" "${XDG_RUNTIME_DIR}/gdm/Xauthority"
    assert_contains "$output" "${HOME}/.Xauthority"
    assert_not_contains "$(cat "$MOCK_DOCKER_LOG")" 'run '
}

test_uses_container_specific_authority_files() {
    bash "$RUN_SCRIPT" >/dev/null
    export CONT_NAME=second_isaaclab
    bash "$RUN_SCRIPT" >/dev/null

    [[ -s "${XDG_RUNTIME_DIR}/isaaclab-docker/test_isaaclab.xauth" ]] || fail 'first container authority file is missing'
    [[ -s "${XDG_RUNTIME_DIR}/isaaclab-docker/second_isaaclab.xauth" ]] || fail 'second container authority file is missing'
}

ORIGINAL_PATH=$PATH
run_test 'rejects missing DISPLAY' test_rejects_missing_display
run_test 'rejects an unsafe container name' test_rejects_unsafe_container_name
run_test 'preserves an existing container' test_preserves_existing_container
run_test 'guides reuse of a running container' test_guides_running_container_reuse
run_test 'uses a neutral Xauthority mount' test_uses_neutral_xauthority_mount
run_test 'prefers explicit Xauthority' test_prefers_explicit_xauthority
run_test 'falls back to GDM Xauthority' test_falls_back_to_gdm_xauthority
run_test 'reports checked authority files' test_reports_checked_authority_files
run_test 'uses container-specific authority files' test_uses_container_specific_authority_files
printf '%d tests passed\n' "$TEST_COUNT"
