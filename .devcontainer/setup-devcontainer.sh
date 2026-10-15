#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
if [[ ! -d tools/mwccarm || ! -d tools/bin ]]; then
    echo 'Install your locally supplied compiler and SDK tools as described in INSTALL.md.' >&2
    exit 1
fi
tools/jsonproc/jsonproc -h &>/dev/null || { make clean-tools && make -j"$(nproc)" tools; }
