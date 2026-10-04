#ifndef RAF_LUAJIT_FREESTANDING_V1_H
#define RAF_LUAJIT_FREESTANDING_V1_H

/*
 * RAFAELIA LuaJIT freestanding sidecar V1.
 *
 * This header is an independently added local boundary. It does not make the
 * inherited LuaJIT runtime freestanding and it does not replace LuaJIT APIs.
 * No hosted C library headers are required by this interface.
 */

typedef unsigned long raf_ljfs_size;

enum raf_ljfs_kind {
  RAF_LJFS_INVALID_POINTER = 0,
  RAF_LJFS_EMPTY = 1,
  RAF_LJFS_NOT_LUAJIT_BYTECODE = 2,
  RAF_LJFS_TRUNCATED_BYTECODE = 3,
  RAF_LJFS_BYTECODE_V2 = 4,
  RAF_LJFS_BYTECODE_PRIVATE_VERSION = 5,
  RAF_LJFS_BYTECODE_VERSION_MISMATCH = 6
};

struct raf_ljfs_probe {
  unsigned char kind;
  unsigned char version;
  unsigned char signature_bytes;
  unsigned char reserved;
};

int raf_ljfs_probe_header(const unsigned char *data, raf_ljfs_size size,
                          struct raf_ljfs_probe *out);

#endif
