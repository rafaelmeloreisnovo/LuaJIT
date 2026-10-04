# Third-Party Provenance — LuaJIT fork

Status: THIRD_PARTY_UPSTREAM_WITH_LOCAL_DELTAS / claim_allowed=false for whole-repository authorship.

## Upstream authority
- Repository: `LuaJIT/LuaJIT`
- Destination: `rafaelmeloreisnovo/LuaJIT`
- Relationship: GitHub fork; this repository inherits LuaJIT source, history, copyright and license obligations from upstream.

## Authorship boundary
Repository ownership, renaming, rebasing, compilation or local modification do not transfer copyright in inherited LuaJIT code.

Any RAFAELIA/RMR authorship claim must be limited to independently added files or deltas supported by path/commit/blob provenance. Unverified paths remain `TOKEN_VAZIO` and `claim_allowed=false`.

## License control
The repository-level GitHub license detector reports `NOASSERTION`; therefore the authoritative license/copyright surfaces in the upstream tree must be preserved and audited before redistribution claims. This file does not replace or alter upstream license terms.

## Required legal record
For local modifications maintain: `source -> upstream ref -> destination path -> delta -> author/copyright -> license -> obligations -> evidence -> release gate`.

## Local authorial delta registry

### RAFAELIA LuaJIT Freestanding Sidecar V1

- source/base: `v2.1@31b5ee9a41e380834d1920a8c1a16cade53ca84f`;
- upstream format reference: `src/lj_bcdump.h` at that base, used only for bytecode signature/version facts;
- destination: `etc/rafaelia_freestanding/` + dedicated CI workflow;
- delta: independently added bounded bytecode-prefix pre-gate; no inherited LuaJIT implementation body replaced or re-authored;
- authorship boundary: local delta only; whole-repository authorship remains `claim_allowed=false`;
- local license selection: `TOKEN_VAZIO_EXPLICIT_LICENSE_SELECTION`;
- obligations: preserve upstream copyright/license surfaces and do not describe the inherited runtime as freestanding;
- evidence: `etc/rafaelia_freestanding/check.sh` + exact-head workflow result;
- release gate: `BLOCKED_UNTIL_EXACT_HEAD_PASS_AND_LICENSE_SELECTION`.
