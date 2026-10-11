#!/usr/bin/env python3
"""Compile one scratch C file with production MWCC flags (Linux wine).

usage: compile.py SRC.c OUT.o [--game hg|ss] [--ver 2.0/sp2p2] [--no-ipa]
Run from anywhere; include paths resolve against the checkout root.
Library ARM sources must start with:
    #include "global.h"          (or nitro/types.h etc.)
    #include <nitro/code32.h>    (AFTER global.h; global.h forces Thumb)
"""
import argparse, contextlib, os, subprocess, sys, tempfile, fcntl, time
from pathlib import Path

ROOT = next(p for p in Path(__file__).resolve().parents if (p/'common.mk').is_file())


def compile_c(src, out, game="hg", ver="2.0/sp2p2", ipa=None, extra=()):
    src, out = Path(src).resolve(), Path(out).resolve()
    if ipa is None: ipa = not ver.startswith("1.2/")
    gdef = []
    args = ["wine", str(ROOT / f"tools/mwccarm/{ver}/mwccarm.exe"), *gdef,
            "-DSDK_ARM9", "-DSDK_CODE_ARM", "-DSDK_FINALROM",
            "-O4,p", "-sym", "on", "-enum", "int", "-lang", "c99", "-Cpp_exceptions", "off", "-gccext,on",
            "-proc", "arm946e", "-msgstyle", "gcc", "-gccinc", "-i", "./src", "-i", "./include", "-i", "./include/library",
            "-i", "./files", "-I./lib/include", "-i", "./lib/NitroSDK/include", "-i", "./lib/NitroSDK/include-mw",
            "-i", "./lib/NitroSDK/include-pokediamond", *(["-ipa", "file"] if ipa else []), "-interworking",
            "-inline", "on,noauto", "-char", "signed", "-W", "all", "-W", "pedantic", "-W", "noimpl_signedunsigned",
            "-W", "noimplicitconv", "-W", "nounusedarg", "-W", "nomissingreturn", "-W", "error", *extra,
            "-c", "-o", os.path.relpath(out, ROOT), os.path.relpath(src, ROOT)]
    env = dict(os.environ, WINEPREFIX=os.environ.get("WINEPREFIX", str(Path.home() / ".wine")), WINEDEBUG="-all")
    # Wine helper processes may inherit a pipe after the compiler exits.
    # A regular temporary log keeps completion tied to the compiler launcher.
    with tempfile.TemporaryFile(mode="w+", encoding="utf-8", errors="replace") as log:
        r = subprocess.run(args, cwd=ROOT, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=300)
        if r.returncode:
            log.seek(0); sys.stderr.write(log.read())
    return r.returncode



_compile_c_unlimited = compile_c


@contextlib.contextmanager
def compiler_slot():
    """Optionally bound concurrent Wine compiler starts across processes.

    Off unless COMPILER_SLOTS_DIR names a directory of flock slot files (12 slots, or 4 while a
    `full-build-active` file exists next to that directory). Under make, -j already bounds the jobs.
    """
    slots = os.environ.get("COMPILER_SLOTS_DIR")
    if not slots:
        yield
        return
    slots = Path(slots)
    slots.mkdir(parents=True, exist_ok=True)
    deadline = time.monotonic() + 900
    held = None
    while held is None:
        limit = 4 if (slots.parent / "full-build-active").exists() else 12
        for index in range(limit):
            handle = (slots / str(index)).open("a")
            try:
                fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
            except BlockingIOError:
                handle.close()
                continue
            held = handle
            break
        if held is None:
            if time.monotonic() >= deadline:
                raise TimeoutError("scratch compiler slot wait exceeded 900 seconds")
            time.sleep(0.2)
    try:
        yield
    finally:
        held.close()


def compile_c(src, out, game="hg", ver="2.0/sp2p2", ipa=None, extra=()):
    # Throttling changes no compilation flags or checks.
    with compiler_slot():
        return _compile_c_unlimited(src, out, game, ver, ipa, extra)

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("src"); p.add_argument("out")
    p.add_argument("--game", default="hg", choices=["hg", "ss"])
    p.add_argument("--ver", default="2.0/sp2p2")
    p.add_argument("--no-ipa", action="store_true")
    a = p.parse_args()
    raise SystemExit(compile_c(a.src, a.out, a.game, a.ver, not a.no_ipa))
