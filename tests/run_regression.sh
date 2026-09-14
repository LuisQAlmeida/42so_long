#!/usr/bin/env bash

set -u
set -o pipefail

ROOT_DIR="$(
    cd "$(dirname "${BASH_SOURCE[0]}")/.." &&
    pwd
)"

BIN="$ROOT_DIR/so_long"
TIMEOUT_SECONDS="${TEST_TIMEOUT:-3}"
COMPILER="${CC:-cc}"

PASSED=0
FAILED=0
TOTAL=0

TMP_DIR="$(mktemp -d)"
BUILD_LOG="$TMP_DIR/build.log"

cleanup()
{
    cd "$ROOT_DIR" || return
    make fclean >/dev/null 2>&1 || true
    rm -rf "$TMP_DIR"
}

trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

pass()
{
    printf '[PASS] %s\n' "$1"
    PASSED=$((PASSED + 1))
}

fail()
{
    printf '[FAIL] %s\n' "$1"
    FAILED=$((FAILED + 1))
}

run_case()
{
    local name="$1"
    local expected_status="$2"
    local expected_message="$3"
    local stdout_file
    local stderr_file
    local expected_stderr
    local status

    shift 3

    TOTAL=$((TOTAL + 1))

    stdout_file="$TMP_DIR/stdout.$TOTAL"
    stderr_file="$TMP_DIR/stderr.$TOTAL"
    expected_stderr="$TMP_DIR/expected-stderr.$TOTAL"

    : > "$stdout_file"
    : > "$stderr_file"

    printf 'Error\n%s\n' "$expected_message" > "$expected_stderr"

    DISPLAY= timeout "${TIMEOUT_SECONDS}s" \
        "$BIN" "$@" \
        >"$stdout_file" \
        2>"$stderr_file"

    status=$?

    if [ "$status" -eq 124 ]; then
        fail "$name (timeout)"
        return
    fi

    if [ "$status" -ne "$expected_status" ]; then
        fail "$name (exit $status, expected $expected_status)"
        printf '       stderr:\n'
        sed 's/^/       | /' "$stderr_file"
        return
    fi

    if [ -s "$stdout_file" ]; then
        fail "$name (unexpected stdout)"
        sed 's/^/       | /' "$stdout_file"
        return
    fi

    if ! cmp -s "$stderr_file" "$expected_stderr"; then
        fail "$name (unexpected stderr)"
        printf '       expected:\n'
        sed 's/^/       | /' "$expected_stderr"
        printf '       actual:\n'
        sed 's/^/       | /' "$stderr_file"
        return
    fi

    pass "$name"
}

require_file()
{
    if [ ! -f "$1" ]; then
        printf 'ERROR: required fixture missing: %s\n' "$1" >&2
        exit 1
    fi
}

cd "$ROOT_DIR" || exit 1

printf '\nso_long regression suite\n'
printf '========================\n\n'

if ! command -v timeout >/dev/null 2>&1; then
    echo 'ERROR: timeout command not available' >&2
    exit 1
fi

if ! command -v "$COMPILER" >/dev/null 2>&1; then
    printf 'ERROR: compiler not available: %s\n' "$COMPILER" >&2
    exit 1
fi

printf 'Compiler: %s\n' "$COMPILER"
printf 'Timeout:  %ss per case\n\n' "$TIMEOUT_SECONDS"

printf 'Fixture corpus gate\n'
printf '%s\n' '-------------------'

invalid_count="$(
    find maps -maxdepth 1 -type f -name 'map_invalid_*.ber' |
    wc -l
)"

valid_count="$(
    find maps -maxdepth 1 -type f -name 'map_valid_*.ber' |
    wc -l
)"

if [ "$invalid_count" -ne 22 ]; then
    printf 'ERROR: expected 22 repository invalid fixtures, found %s\n' \
        "$invalid_count" >&2
    exit 1
fi

if [ "$valid_count" -ne 6 ]; then
    printf 'ERROR: expected 6 repository valid fixtures, found %s\n' \
        "$valid_count" >&2
    exit 1
fi

echo 'PASS: 22 invalid + 6 valid repository fixtures found'

format_maps=(
    maps/map_invalid_border_bot.ber
    maps/map_invalid_border_bot_left.ber
    maps/map_invalid_border_bot_right.ber
    maps/map_invalid_border_left.ber
    maps/map_invalid_border_right.ber
    maps/map_invalid_border_top.ber
    maps/map_invalid_border_top_left.ber
    maps/map_invalid_border_top_right.ber
    maps/map_invalid_empty_line.ber
    maps/map_invalid_letters.ber
    maps/map_invalid_multiple_exits.ber
    maps/map_invalid_multiple_players.ber
    maps/map_invalid_no_exit.ber
    maps/map_invalid_no_player.ber
    maps/map_invalid_shape.ber
    maps/map_invalid_tabs_right.ber
    maps/map_invalid_trailing_garbage.ber
    maps/map_invalid_empty.ber
    maps/map_invalid_too_small.ber
    maps/map_invalid_no_collectible.ber
)

path_maps=(
    maps/map_invalid_path_unreachable_exit.ber
    maps/map_invalid_path_unreachable_collectible.ber
)

valid_maps=(
    maps/map_valid_10x7_4C.ber
    maps/map_valid_4x4_1C.ber
    maps/map_valid_5x5_2C.ber
    maps/map_valid_p-e-c.ber
    maps/map_valid_subject_1.ber
    maps/map_valid_subject_2.ber
)

for fixture in \
    "${format_maps[@]}" \
    "${path_maps[@]}" \
    "${valid_maps[@]}"
do
    require_file "$fixture"
done

printf '\nBuild\n'
printf '%s\n' '-----'

make fclean >/dev/null 2>&1 || true

if ! make CC="$COMPILER" >"$BUILD_LOG" 2>&1; then
    echo 'FAIL: project build failed'
    cat "$BUILD_LOG"
    exit 1
fi

if [ ! -x "$BIN" ]; then
    echo 'FAIL: build completed without producing so_long'
    cat "$BUILD_LOG"
    exit 1
fi

echo 'PASS: project built successfully'

printf '\nArgument / file handling\n'
printf '%s\n' '------------------------'

run_case \
    'no arguments' \
    1 \
    '[INVALID ARGUMENTS]'

run_case \
    'too many arguments' \
    1 \
    '[INVALID ARGUMENTS]' \
    maps/map_valid_4x4_1C.ber \
    maps/map_valid_5x5_2C.ber

run_case \
    'invalid extension' \
    2 \
    '[INVALID FILE EXTENSION]' \
    README.md

run_case \
    'missing .ber file' \
    3 \
    '[FAILED TO OPEN FILE]' \
    maps/does_not_exist.ber

printf '\nFormat validation\n'
printf '%s\n' '-----------------'

for map in "${format_maps[@]}"; do
    run_case \
        "$(basename "$map")" \
        4 \
        '[INVALID MAP FORMAT]' \
        "$map"
done

printf '\nPath validation\n'
printf '%s\n' '---------------'

for map in "${path_maps[@]}"; do
    run_case \
        "$(basename "$map")" \
        5 \
        '[INVALID MAP PATH]' \
        "$map"
done

printf '\nValid maps -> graphical boundary\n'
printf '%s\n' '--------------------------------'

for map in "${valid_maps[@]}"; do
    run_case \
        "$(basename "$map")" \
        7 \
        '[FAILED TO INITIALIZE GRAPHICS]' \
        "$map"
done

printf '\nCleanup verification\n'
printf '%s\n' '--------------------'

make fclean >/dev/null 2>&1 || {
    echo 'FAIL: make fclean failed'
    exit 1
}

if [ -e so_long ]; then
    echo 'FAIL: executable remains after cleanup'
    exit 1
fi

if find src -type f -name '*.o' -print -quit | grep -q .; then
    echo 'FAIL: project object files remain after cleanup'
    exit 1
fi

if [ -e external/libft/libft/libft.a ]; then
    echo 'FAIL: Libft archive remains after cleanup'
    exit 1
fi

if [ -e external/minilibx-linux/libmlx_Linux.a ]; then
    echo 'FAIL: MiniLibX archive remains after cleanup'
    exit 1
fi

if [ -n "$(git -C external/libft status --porcelain)" ]; then
    echo 'FAIL: Libft submodule dirty after regression suite'
    git -C external/libft status --short
    exit 1
fi

if [ -n "$(git -C external/minilibx-linux status --porcelain)" ]; then
    echo 'FAIL: MiniLibX submodule dirty after regression suite'
    git -C external/minilibx-linux status --short
    exit 1
fi

echo 'PASS: build artefacts cleaned'
echo 'PASS: dependency submodules remain clean'

printf '\nSummary\n'
printf '%s\n' '-------'
printf '%d passed\n' "$PASSED"
printf '%d failed\n' "$FAILED"
printf '%d total\n' "$TOTAL"

if [ "$FAILED" -ne 0 ]; then
    exit 1
fi

if [ "$TOTAL" -ne 32 ]; then
    printf 'ERROR: expected 32 regression cases, executed %d\n' "$TOTAL" >&2
    exit 1
fi

echo
echo 'Regression suite passed.'
