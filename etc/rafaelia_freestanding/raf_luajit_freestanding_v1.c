#include "raf_luajit_freestanding_v1.h"

#define RAF_LJFS_HEAD1 0x1bu
#define RAF_LJFS_HEAD2 0x4cu
#define RAF_LJFS_HEAD3 0x4au
#define RAF_LJFS_VERSION 2u
#define RAF_LJFS_PRIVATE_VERSION_MIN 0x80u

static void raf_ljfs_store(struct raf_ljfs_probe *out, unsigned char kind,
                           unsigned char version,
                           unsigned char signature_bytes)
{
  if (out != (struct raf_ljfs_probe *)0) {
    out->kind = kind;
    out->version = version;
    out->signature_bytes = signature_bytes;
    out->reserved = 0u;
  }
}

int raf_ljfs_probe_header(const unsigned char *data, raf_ljfs_size size,
                          struct raf_ljfs_probe *out)
{
  if (data == (const unsigned char *)0) {
    raf_ljfs_store(out, RAF_LJFS_INVALID_POINTER, 0u, 0u);
    return RAF_LJFS_INVALID_POINTER;
  }

  if (size == 0u) {
    raf_ljfs_store(out, RAF_LJFS_EMPTY, 0u, 0u);
    return RAF_LJFS_EMPTY;
  }

  if (data[0] != RAF_LJFS_HEAD1) {
    raf_ljfs_store(out, RAF_LJFS_NOT_LUAJIT_BYTECODE, 0u, 0u);
    return RAF_LJFS_NOT_LUAJIT_BYTECODE;
  }

  if (size < 3u) {
    raf_ljfs_store(out, RAF_LJFS_TRUNCATED_BYTECODE, 0u, 1u);
    return RAF_LJFS_TRUNCATED_BYTECODE;
  }

  if (data[1] != RAF_LJFS_HEAD2 || data[2] != RAF_LJFS_HEAD3) {
    raf_ljfs_store(out, RAF_LJFS_NOT_LUAJIT_BYTECODE, 0u, 1u);
    return RAF_LJFS_NOT_LUAJIT_BYTECODE;
  }

  if (size < 4u) {
    raf_ljfs_store(out, RAF_LJFS_TRUNCATED_BYTECODE, 0u, 3u);
    return RAF_LJFS_TRUNCATED_BYTECODE;
  }

  if (data[3] == RAF_LJFS_VERSION) {
    raf_ljfs_store(out, RAF_LJFS_BYTECODE_V2, data[3], 3u);
    return RAF_LJFS_BYTECODE_V2;
  }

  if (data[3] >= RAF_LJFS_PRIVATE_VERSION_MIN) {
    raf_ljfs_store(out, RAF_LJFS_BYTECODE_PRIVATE_VERSION, data[3], 3u);
    return RAF_LJFS_BYTECODE_PRIVATE_VERSION;
  }

  raf_ljfs_store(out, RAF_LJFS_BYTECODE_VERSION_MISMATCH, data[3], 3u);
  return RAF_LJFS_BYTECODE_VERSION_MISMATCH;
}
