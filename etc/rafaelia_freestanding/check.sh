#!/bin/sh
set -eu

CC_BIN="${CC:-cc}"
NM_BIN="${NM:-nm}"
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
WORK="${TMPDIR:-/tmp}/raf-luajit-freestanding-$$"
mkdir -p "$WORK"
trap 'rm -rf "$WORK"' EXIT HUP INT TERM

COMMON='-std=c99 -Wall -Wextra -Werror -pedantic -fno-builtin -fno-stack-protector'

"$CC_BIN" $COMMON -ffreestanding -c \
  "$ROOT/raf_luajit_freestanding_v1.c" \
  -o "$WORK/raf_luajit_freestanding_v1.o"

if "$NM_BIN" -u "$WORK/raf_luajit_freestanding_v1.o" | grep -q .; then
  echo 'freestanding core has undefined symbols' >&2
  "$NM_BIN" -u "$WORK/raf_luajit_freestanding_v1.o" >&2
  exit 1
fi

"$CC_BIN" $COMMON \
  "$ROOT/raf_luajit_freestanding_v1.c" \
  "$ROOT/test_raf_luajit_freestanding_v1.c" \
  -o "$WORK/raf_luajit_freestanding_v1_test"
"$WORK/raf_luajit_freestanding_v1_test"

if command -v clang >/dev/null 2>&1; then
  clang $COMMON -ffreestanding --target=armv7-none-eabi -c \
    "$ROOT/raf_luajit_freestanding_v1.c" \
    -o "$WORK/raf_luajit_freestanding_v1.armv7.o"
  clang $COMMON -ffreestanding --target=aarch64-none-elf -c \
    "$ROOT/raf_luajit_freestanding_v1.c" \
    -o "$WORK/raf_luajit_freestanding_v1.aarch64.o"
fi

printf '%s\n' 'RAFAELIA_LUAJIT_FREESTANDING_SIDECAR_V1=PASS'
printf '%s\n' 'CORE_UNDEFINED_SYMBOLS=NONE'
printf '%s\n' 'HOSTED_VECTOR_REPLAY=PASS'
