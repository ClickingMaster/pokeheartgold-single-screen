#!/usr/bin/env python3
"""Add verified section-relative alias symbols to a compiled ELF object.

The input JSON names one owning symbol and a set of interior aliases.  This is
used where the original assembly exported multiple labels inside one BSS object,
something C cannot express portably and MWAS cannot express across objects.

It is also used for code: an owner may be a function ("type": "FUNC") whose
interior entry points are exported as function aliases.  This covers runs of
hand-written Thumb stubs at 2-byte aligned addresses, which MWCC cannot emit as
separate functions because every function gets its own 4-aligned section.  The
section's existing $t/$a mapping symbols keep defining the instruction set.
"""

import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path


def run(*args: str) -> str:
    return subprocess.check_output(args, text=True)


def section_indices(obj: Path, readelf: str) -> dict[str, str]:
    result = {}
    for line in run(readelf, "-SW", str(obj)).splitlines():
        match = re.match(r"\s*\[\s*(\d+)\]\s+(\S+)", line)
        if match:
            result[match.group(2)] = match.group(1)
    return result


def symbols(obj: Path, readelf: str) -> dict[str, tuple[int, int, str, str, str]]:
    result = {}
    pattern = re.compile(
        r"\s*\d+:\s+([0-9a-fA-F]+)\s+(\d+)\s+(\S+)\s+(\S+)\s+\S+\s+(\S+)\s+(\S+)$"
    )
    for line in run(readelf, "-sW", str(obj)).splitlines():
        match = pattern.match(line)
        if match:
            value, size, symbol_type, binding, ndx, name = match.groups()
            result[name] = (int(value, 16), int(size), symbol_type, binding, ndx)
    return result


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("object", type=Path)
    parser.add_argument("map", type=Path)
    args = parser.parse_args()

    objcopy = shutil.which("arm-none-eabi-objcopy")
    readelf = shutil.which("arm-none-eabi-readelf")
    if objcopy is None or readelf is None:
        raise SystemExit("arm-none-eabi-objcopy/readelf must be on PATH")
    if not args.object.is_file():
        raise SystemExit(f"object not found: {args.object}")

    config = json.loads(args.map.read_text())
    section = config["section"]
    indices = section_indices(args.object, readelf)
    if section not in indices:
        raise SystemExit(f"section {section} not found in {args.object}")
    section_index = indices[section]

    before = symbols(args.object, readelf)
    owner = config["owner"]
    owner_offset = owner.get("offset", 0)
    owner_offset = int(owner_offset, 0) if isinstance(owner_offset, str) else owner_offset
    expected_owner = (
        owner_offset,
        int(owner["size"], 0) if isinstance(owner["size"], str) else owner["size"],
        owner.get("type", "OBJECT"),
        "GLOBAL",
        section_index,
    )
    if before.get(owner["name"]) != expected_owner:
        raise SystemExit(
            f"owner mismatch: {owner['name']} is {before.get(owner['name'])}, "
            f"expected {expected_owner}"
        )

    if expected_owner[2] not in ("OBJECT", "FUNC"):
        raise SystemExit(f"unsupported owner type {expected_owner[2]}")
    alias_kind = "function" if expected_owner[2] == "FUNC" else "object"

    command = [objcopy]
    expected = {}
    for alias in config["aliases"]:
        name = alias["name"]
        if name in before:
            raise SystemExit(f"refusing to duplicate existing symbol: {name}")
        offset = int(alias["offset"], 0) if isinstance(alias["offset"], str) else alias["offset"]
        if offset < 0 or offset >= expected_owner[1]:
            raise SystemExit(f"alias {name} offset {offset:#x} is outside owner")
        command += ["--add-symbol", f"{name}={section}:{offset:#x},global,{alias_kind}"]
        expected[name] = (offset, 0, expected_owner[2], "GLOBAL", section_index)
    command += [str(args.object)]
    subprocess.run(command, check=True)

    # objcopy may discard empty compiler bookkeeping sections and renumber the
    # remaining section table.  Resolve the target index again from the output
    # object instead of assuming the input index survives the rewrite.
    after_indices = section_indices(args.object, readelf)
    after_section_index = after_indices.get(section)
    if after_section_index is None:
        raise SystemExit(f"section {section} missing after rewriting {args.object}")
    after = symbols(args.object, readelf)
    for name, value in expected.items():
        value = (*value[:-1], after_section_index)
        if after.get(name) != value:
            raise SystemExit(f"postcondition failed: {name} is {after.get(name)}, expected {value}")

    print(f"added and verified {len(expected)} aliases in {section}: {args.object}")


if __name__ == "__main__":
    main()
