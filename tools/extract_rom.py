#!/usr/bin/env python3
"""Extracts the NitroFS file tree of a Nintendo DS ROM into a folder (default: files/).

    python3 tools/extract_rom.py ROM.nds [OUT_DIR]
"""
import os
import struct
import sys


def extract(rom_path, out_dir):
    data = open(rom_path, 'rb').read()
    fnt_off, fnt_size, fat_off, fat_size = struct.unpack_from('<4I', data, 0x40)

    def u32(o):
        return struct.unpack_from('<I', data, o)[0]

    def u16(o):
        return struct.unpack_from('<H', data, o)[0]

    count = 0

    def walk(dir_id, path):
        nonlocal count
        sub = u32(fnt_off + (dir_id & 0xFFF) * 8)
        file_id = u16(fnt_off + (dir_id & 0xFFF) * 8 + 4)
        pos = fnt_off + sub
        os.makedirs(path, exist_ok=True)
        while True:
            t = data[pos]
            pos += 1
            if t == 0:
                return
            n = t & 0x7F
            name = data[pos:pos + n].decode('utf-8', 'replace')
            pos += n
            if t & 0x80:
                child = u16(pos)
                pos += 2
                walk(child, os.path.join(path, name))
            else:
                start, end = struct.unpack_from('<2I', data, fat_off + file_id * 8)
                with open(os.path.join(path, name), 'wb') as f:
                    f.write(data[start:end])
                file_id += 1
                count += 1

    walk(0xF000, out_dir)
    return count


if __name__ == '__main__':
    if len(sys.argv) < 2:
        raise SystemExit(__doc__)
    out = sys.argv[2] if len(sys.argv) > 2 else os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'files')
    print('Extracted %d files to %s' % (extract(sys.argv[1], out), out))
