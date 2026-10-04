#include "raf_luajit_freestanding_v1.h"

static int expect_kind(const unsigned char *data, raf_ljfs_size size,
                       int expected_kind, unsigned char expected_version,
                       unsigned char expected_signature_bytes)
{
  struct raf_ljfs_probe out;
  int kind = raf_ljfs_probe_header(data, size, &out);
  if (kind != expected_kind) return 1;
  if ((int)out.kind != expected_kind) return 2;
  if (out.version != expected_version) return 3;
  if (out.signature_bytes != expected_signature_bytes) return 4;
  if (out.reserved != 0u) return 5;
  return 0;
}

int main(void)
{
  static const unsigned char text[] = { 'p', 'r', 'i', 'n', 't' };
  static const unsigned char esc_only[] = { 0x1b };
  static const unsigned char wrong_sig[] = { 0x1b, 'L', 'X', 2u };
  static const unsigned char short_sig[] = { 0x1b, 'L', 'J' };
  static const unsigned char v2[] = { 0x1b, 'L', 'J', 2u };
  static const unsigned char v1[] = { 0x1b, 'L', 'J', 1u };
  static const unsigned char private_v[] = { 0x1b, 'L', 'J', 0x80u };
  unsigned char empty_anchor = 0u;
  int rc;

  rc = expect_kind((const unsigned char *)0, 0u,
                   RAF_LJFS_INVALID_POINTER, 0u, 0u);
  if (rc != 0) return 10 + rc;

  rc = expect_kind(&empty_anchor, 0u, RAF_LJFS_EMPTY, 0u, 0u);
  if (rc != 0) return 20 + rc;

  rc = expect_kind(text, sizeof(text), RAF_LJFS_NOT_LUAJIT_BYTECODE, 0u, 0u);
  if (rc != 0) return 30 + rc;

  rc = expect_kind(esc_only, sizeof(esc_only), RAF_LJFS_TRUNCATED_BYTECODE, 0u, 1u);
  if (rc != 0) return 40 + rc;

  rc = expect_kind(wrong_sig, sizeof(wrong_sig), RAF_LJFS_NOT_LUAJIT_BYTECODE, 0u, 1u);
  if (rc != 0) return 50 + rc;

  rc = expect_kind(short_sig, sizeof(short_sig), RAF_LJFS_TRUNCATED_BYTECODE, 0u, 3u);
  if (rc != 0) return 60 + rc;

  rc = expect_kind(v2, sizeof(v2), RAF_LJFS_BYTECODE_V2, 2u, 3u);
  if (rc != 0) return 70 + rc;

  rc = expect_kind(v1, sizeof(v1), RAF_LJFS_BYTECODE_VERSION_MISMATCH, 1u, 3u);
  if (rc != 0) return 80 + rc;

  rc = expect_kind(private_v, sizeof(private_v), RAF_LJFS_BYTECODE_PRIVATE_VERSION, 0x80u, 3u);
  if (rc != 0) return 90 + rc;

  return 0;
}
