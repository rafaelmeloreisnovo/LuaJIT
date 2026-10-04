# RAFAELIA LuaJIT Freestanding Sidecar V1

State: `IMPLEMENTED_UNTESTED` until an exact-head gate terminates successfully.

## Purpose

Provide one independently authored, tiny freestanding boundary that can inspect the fixed LuaJIT bytecode prefix before a hosted LuaJIT runtime is entered.

This directory does **not** claim that inherited LuaJIT is freestanding. LuaJIT remains the upstream runtime authority. This sidecar is a local pre-gate with its own evidence boundary.

```text
SOURCE_UPSTREAM_LUAJIT != LOCAL_AUTHORIAL_SIDECAR
LOCAL_SOURCE != COMPILE != EXECUTION != EVIDENCE != CLAIM
IMPLEMENTED_UNTESTED != PASS
TOKEN_VAZIO != 0
```

## What the core does

`raf_ljfs_probe_header()` classifies a bounded byte span as:

- invalid pointer;
- empty;
- not LuaJIT bytecode;
- truncated LuaJIT bytecode prefix;
- LuaJIT bytecode version 2;
- private bytecode version (`>= 0x80`);
- other/mismatched bytecode version.

The format facts are derived from the upstream bytecode definition in `src/lj_bcdump.h` at the branch base. No upstream implementation body is copied into this sidecar.

## Freestanding boundary

Core files:

- `raf_luajit_freestanding_v1.h`
- `raf_luajit_freestanding_v1.c`

Declared constraints for the core:

- no libc headers;
- no heap/allocation;
- no syscall;
- no file/network/device I/O;
- no global mutable state;
- no LuaJIT internal headers;
- no runtime/JIT claim;
- fixed bounded reads only.

The hosted test executable is **test infrastructure**, not part of the freestanding core.

## Gate

Run:

```sh
sh etc/rafaelia_freestanding/check.sh
```

The gate:

1. compiles the core with `-ffreestanding -fno-builtin -fno-stack-protector`;
2. requires no undefined symbols in the core object;
3. replays deterministic hosted vectors against the same source;
4. when `clang` is present, compiles the same core for `armv7-none-eabi` and `aarch64-none-elf` without target libraries.

A successful gate proves only this sidecar boundary. It does not prove full LuaJIT freestanding execution, bare-metal JIT operation, device runtime, ABI parity with every target, or release suitability.

## Integration rule

Use the sidecar before entering a hosted adapter when an input must be classified without allocating or invoking LuaJIT. Do not insert it into upstream LuaJIT internals merely to increase the amount of "freestanding" code.

If a later integration requires LuaJIT execution, keep the split explicit:

```text
CORE_FREESTANDING -> ADAPTER_PLATFORM -> LUAJIT_UPSTREAM_RUNTIME
```

## Falsifier / rollback / replay

- **Falsifier:** core object gains an undefined symbol, deterministic vectors disagree, or a supported compile target cannot produce the object under the declared flags.
- **Rollback:** remove this self-contained directory and its dedicated CI lane; no upstream LuaJIT source needs reversal.
- **Replay:** run `check.sh` from the exact commit and bind compiler identity + commit SHA in the receipt.

## R3

- `F_ok`: bounded independently authored sidecar; upstream authorship boundary preserved.
- `F_gap`: exact-head CI and physical/device use are not yet evidence.
- `F_next`: close the dedicated gate; integrate only where a real caller benefits from the pre-gate.
