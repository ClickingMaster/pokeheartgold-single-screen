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


def section_names(obj: Path, readelf: str) -> dict[str, str]:
    result = {}
    for line in run(readelf, "-SW", str(obj)).splitlines():
        match = re.match(r"\s*\[\s*(\d+)\]\s+(\S+)", line)
        if match:
            result[match.group(1)] = match.group(2)
    return result


def set_symbol_section(obj: Path, name: str, shndx: int) -> None:
    """Point the (single) global symbol `name` at section index `shndx` in place.

    MWCC C++ objects can contain several sections with the same name (one
    `.text` per function).  objcopy --add-symbol resolves a section by name and
    always picks the first one, so for an owner in a later same-named section
    the freshly added alias is re-targeted here (ELF32 little-endian only)."""
    import struct
    data = bytearray(obj.read_bytes())
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        raise SystemExit(f"unsupported ELF class/endianness: {obj}")
    shoff, = struct.unpack_from("<I", data, 32)
    shentsize, shnum = struct.unpack_from("<HH", data, 46)
    headers = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    hits = []
    for header in headers:
        if header[1] != 2:  # SHT_SYMTAB
            continue
        strtab_off = headers[header[6]][4]
        for pos in range(header[4], header[4] + header[5], 16):
            name_off, = struct.unpack_from("<I", data, pos)
            end = data.index(b"\0", strtab_off + name_off)
            if data[strtab_off + name_off:end].decode() == name:
                hits.append(pos)
    if len(hits) != 1:
        raise SystemExit(f"expected exactly one symbol {name} in {obj}, found {len(hits)}")
    struct.pack_into("<H", data, hits[0] + 14, shndx)
    obj.write_bytes(bytes(data))


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

    loaded = json.loads(args.map.read_text())
    # A map may also be a JSON list of such configs (several owners in one object).
    for config in loaded if isinstance(loaded, list) else [loaded]:
        apply_config(args, config, objcopy, readelf)


def apply_config(args, config, objcopy, readelf) -> None:
    section = config["section"]
    indices = section_indices(args.object, readelf)
    if section not in indices:
        raise SystemExit(f"section {section} not found in {args.object}")
    section_index = indices[section]

    before = symbols(args.object, readelf)
    owner = config["owner"]
    # Several sections may share the configured name (MWCC C++ emits one
    # `.text` per function).  Then the owner's own section is the target; it
    # must still carry the configured name.
    names = section_names(args.object, readelf)
    duplicated = list(names.values()).count(section) > 1
    if duplicated and owner["name"] in before:
        owner_ndx = before[owner["name"]][4]
        if names.get(owner_ndx) != section:
            raise SystemExit(f"owner {owner['name']} is not in a {section} section")
        section_index = owner_ndx
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
        # Alias offsets are section-relative, like the owner's own offset.
        if offset < expected_owner[0] or offset >= expected_owner[0] + expected_owner[1]:
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
    if duplicated:
        after_section_index = after[owner["name"]][4]
        if section_names(args.object, readelf).get(after_section_index) != section:
            raise SystemExit(f"owner {owner['name']} left its {section} section after rewriting")
        for name in expected:
            if after.get(name, (None,) * 5)[4] != after_section_index:
                set_symbol_section(args.object, name, int(after_section_index))
        after = symbols(args.object, readelf)
    for name, value in expected.items():
        value = (*value[:-1], after_section_index)
        if after.get(name) != value:
            raise SystemExit(f"postcondition failed: {name} is {after.get(name)}, expected {value}")

    print(f"added and verified {len(expected)} aliases in {section}: {args.object}")


if __name__ == "__main__":
    main()
