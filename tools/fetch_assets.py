#!/usr/bin/env python3
"""Fills in the parts of files/ that this repo does not track.

    python3 tools/fetch_assets.py [--rom ROM.nds] [--pret-dir DIR] [--dry-run]

1. Source assets (scripts, message sources, event data, ...) are downloaded
   from pret/pokeheartgold at the commit this fork branched from, and only
   written where the file is missing, so tracked files are never touched.
   Script sources get the small command renames in tools/assets/script_renames.txt.
2. Raw game files (files/a/..., listed in filesystem.mk) come from your own
   ROM via tools/extract_rom.py; they cannot be downloaded.

Nothing here is committed: files/ is gitignored.
"""
import argparse
import io
import os
import re
import sys
import tarfile
import tempfile
import urllib.request

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PRET_COMMIT = 'a6ebf56ab7a454d91cc4715c73b62e93c305dc29'
PRET_URL = 'https://codeload.github.com/pret/pokeheartgold/tar.gz/' + PRET_COMMIT
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))


def load_renames():
    pairs = {}
    with open(os.path.join(ROOT, 'tools', 'assets', 'script_renames.txt')) as f:
        for line in f:
            if line.strip() and not line.startswith('#'):
                a, b = line.split()
                pairs[a] = b
    if not pairs:
        return None
    return re.compile(r'\b(' + '|'.join(map(re.escape, sorted(pairs, key=len, reverse=True))) + r')\b'), pairs


def fix_script(data, renames):
    text = data.decode('utf-8', errors='surrogateescape')
    text = re.sub(r'^#include "macros/', '    .include "macros/', text, flags=re.M)
    if 'ABILITY_' in text and 'constants/abilities.h' not in text:
        text = re.sub(r'^(#include "constants/scrcmd.h"\n)', r'\1#include "constants/abilities.h"\n', text, count=1, flags=re.M)
    text = text.replace('#include "unk_02009D48.h"', '#include "sprite_resource_header.h"')
    if renames:
        rx, pairs = renames
        text = rx.sub(lambda m: pairs[m.group(1)], text)
    return text.encode('utf-8', errors='surrogateescape')


def fetch_pret(pret_dir, dry):
    renames = load_renames()
    if pret_dir:
        def members():
            for base, _, names in os.walk(os.path.join(pret_dir, 'files')):
                for n in names:
                    p = os.path.join(base, n)
                    yield os.path.relpath(p, pret_dir).replace(os.sep, '/'), lambda p=p: open(p, 'rb').read()
        source = members()
    else:
        print('Downloading pret/pokeheartgold @ %s ...' % PRET_COMMIT[:9])
        blob = urllib.request.urlopen(PRET_URL).read()
        tar = tarfile.open(fileobj=io.BytesIO(blob))

        def members2():
            for m in tar:
                if not m.isfile():
                    continue
                rel = m.name.split('/', 1)[1] if '/' in m.name else m.name
                if rel.startswith('files/'):
                    yield rel, lambda m=m: tar.extractfile(m).read()
        source = members2()
    added = 0
    for rel, read in source:
        dest = os.path.join(ROOT, rel)
        if os.path.exists(dest):
            continue
        data = read()
        if rel.endswith(('.s', '.json', '.txt')):
            data = fix_script(data, renames)
        added += 1
        if not dry:
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            with open(dest, 'wb') as f:
                f.write(data)
            if rel.endswith('.sh'):
                os.chmod(dest, 0o755)
    print('%s %d source files from pret' % ('Would add' if dry else 'Added', added))


def fetch_rom(rom, dry):
    import extract_rom
    with tempfile.TemporaryDirectory() as tmp:
        extract_rom.extract(rom, tmp)
        added = 0
        for base, _, names in os.walk(tmp):
            for n in names:
                src = os.path.join(base, n)
                rel = 'files/' + os.path.relpath(src, tmp).replace(os.sep, '/')
                dest = os.path.join(ROOT, rel)
                if os.path.exists(dest):
                    continue
                added += 1
                if not dry:
                    os.makedirs(os.path.dirname(dest), exist_ok=True)
                    with open(src, 'rb') as a, open(dest, 'wb') as b:
                        b.write(a.read())
    print('%s %d raw files from the ROM' % ('Would add' if dry else 'Added', added))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--rom', help='your own SoulSilver (USA) .nds, for the raw files/a/... data')
    ap.add_argument('--pret-dir', help='use an existing pret/pokeheartgold checkout at the base commit instead of downloading')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()
    fetch_pret(args.pret_dir, args.dry_run)
    if args.rom:
        fetch_rom(args.rom, args.dry_run)
    else:
        print('No --rom given: raw game files (files/a/...) are still missing; the build needs them.')


if __name__ == '__main__':
    main()
