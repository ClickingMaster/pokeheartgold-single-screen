#!/usr/bin/env python3
"""Rename only the reviewed reset C producer's three physical partitions.

The raw compiler producer is retained separately. The plan pins every allocated
section, symbol and relocation by section-name ordinal, excluding debug only.
This tool changes only sh_name and the .shstrtab offset/size, never instructions,
relocations, flags, symbols, addresses, section indices or data ownership.
"""
import collections, hashlib, json, struct, sys
from pathlib import Path

def need(value, message):
    if not value:
        raise ValueError(message)

def inspect(blob):
    need(blob[:6] == b'\x7fELF\x01\x01', 'ELF32 little endian required')
    need(struct.unpack_from('<HH', blob, 16) == (1, 40), 'ARM relocatable required')
    shoff = struct.unpack_from('<I', blob, 32)[0]
    entsz, count, strings_index = struct.unpack_from('<HHH', blob, 46)
    need(entsz == 40 and 0 < strings_index < count, 'invalid section table')
    need(shoff + entsz * count <= len(blob), 'truncated section table')
    sections = [list(struct.unpack_from('<10I', blob, shoff + i * entsz)) for i in range(count)]
    shstrings = sections[strings_index]
    need(shstrings[1] == 3 and shstrings[2] == 0, 'section-name strings required')
    names = blob[shstrings[4]:shstrings[4] + shstrings[5]]
    def cstr(data, offset):
        need(0 <= offset < len(data), 'bad string offset')
        end = data.find(b'\0', offset)
        need(end >= 0, 'unterminated string')
        return data[offset:end].decode()
    counts = collections.Counter()
    identities = []
    for section in sections:
        name = cstr(names, section[0])
        identities.append((name, counts[name]))
        counts[name] += 1
    tab_indices = [i for i, s in enumerate(sections) if s[1] == 2]
    tabs = [sections[i] for i in tab_indices]
    need(identities[strings_index] == ('.shstrtab', 0) and counts['.shstrtab'] == 1, 'actual section-name table required')
    need(len(tabs) == 1 and tabs[0][9] == 16, 'one normal symbol table required')
    tab = tabs[0]
    st = sections[tab[6]]
    strings = blob[st[4]:st[4] + st[5]]
    symbols = []
    for offset in range(tab[4], tab[4] + tab[5], 16):
        name, value, size, info, other, index = struct.unpack_from('<IIIBBH', blob, offset)
        symbols.append((cstr(strings, name) if name else '', value, size, info, other, index))
    def owned(index):
        section = sections[index]
        return identities[index][0] == '.ARM.attributes' or bool(section[2] & 2) or (section[1] in (1, 8) and section[5] and not identities[index][0].startswith(('.debug', '.comment')))
    relocs = collections.defaultdict(list)
    for section in sections:
        if section[1] not in (4, 9):
            continue
        need(section[1] == 4 and section[9] == 12 and section[6] == tab_indices[0], 'reviewed MWCC RELA with actual symbol table required')
        target_section = section[7]
        if not owned(target_section):
            continue
        for offset in range(section[4], section[4] + section[5], 12):
            position, info = struct.unpack_from('<II', blob, offset)
            addend = struct.unpack_from('<i', blob, offset + 8)[0]
            target = symbols[info >> 8]
            name, value, size, binding, other, index = target
            relocs[target_section].append((position, info & 255, addend, name, value, size, binding, other, identities[index] if index < len(sections) else index))
    runtime = []
    for i, section in enumerate(sections):
        if not owned(i):
            continue
        payload = b'' if section[1] == 8 else blob[section[4]:section[4] + section[5]]
        runtime.append({'identity': identities[i], 'metadata': [section[k] for k in (1, 2, 3, 5, 8, 9)], 'payload_sha256': hashlib.sha256(payload).hexdigest(), 'relocations': relocs[i]})
    inventory = []
    for name, value, size, info, other, index in symbols:
        if name and (index == 0 or (index < len(sections) and owned(index))):
            inventory.append((name, value, size, info, other, identities[index] if index < len(sections) else index))
    closure = {'elf_flags': struct.unpack_from('<I', blob, 36)[0], 'runtime': runtime, 'symbols': sorted(inventory, key=str)}
    return sections, identities, symbols, closure, shoff, strings_index

def normalized(value):
    return json.loads(json.dumps(value))

def rename(source, output, plan_path):
    raw = source.read_bytes()
    plan = json.loads(plan_path.read_text())
    sections, identities, symbols, closure, shoff, strings_index = inspect(raw)
    need(normalized(closure) == plan['producer_closure'], 'unreviewed producer runtime inventory')
    text = [i for i, identity in enumerate(identities) if identity[0] in ('.text','.itcm') and sections[i][2] & 4]
    need(len(text) == 7, 'exact seven reset C sections')
    changes = {}
    expected = {'OS_InitReset':('.text',0,84,'.text.to90_reset_main_C'),'OS_IsResetOccurred':('.text',1,16,'.text.to90_reset_main_C'),'OSi_CommonCallback':('.text',2,52,'.text.to90_reset_main_C'),'OSi_SendToPxi':('.text',3,44,'.text.to90_reset_main_C'),'OSi_DoResetSystem':('.itcm',0,52,'.itcm.to90_reset_entry_C'),'OSi_ReloadRomData':('.itcm',1,168,'.itcm.to90_reset_reload_C'),'OSi_ReadCardRom32':('.itcm',2,216,'.itcm.to90_reset_reload_C')}
    need({r['name'] for r in plan['functions']} == set(expected), 'finite reset definitions')
    for row in plan['functions']:
        owners = [s for s in symbols if s[0] == row['name'] and s[5] in text]
        need(len(owners) == 1, 'single reset function owner')
        name,value,size,info,other,index = owners[0]
        sec,ordinal,span,selector = expected[name]
        need(value == 0 and size == span and info == 18 and other == 0, 'function raw ABI/span')
        need(identities[index] == (sec,ordinal) and row['identity'] == [sec,ordinal] and row['size'] == span and row['selector'] == selector, 'fixed ordinal selector')
        need(index not in changes, 'shared function section')
        changes[index] = selector
    need(set(changes) == set(text), 'exhaustive original reset C sections')
    string_section = sections[strings_index]
    old_strings = raw[string_section[4]:string_section[4] + string_section[5]]
    new_strings = bytearray(old_strings)
    result = bytearray(raw)
    allowed = set()
    for index, name in changes.items():
        field = shoff + index * 40
        allowed.update(range(field, field + 4))
        struct.pack_into('<I', result, field, len(new_strings))
        new_strings += name.encode() + b'\0'
    field = shoff + strings_index * 40 + 16
    allowed.update(range(field, field + 8))
    struct.pack_into('<II', result, field, len(raw), len(new_strings))
    result += new_strings
    need(all(a == b or i in allowed for i, (a, b) in enumerate(zip(raw, result))), 'unexpected non-name byte mutation')
    need(result[len(raw):] == new_strings, 'unexpected appended bytes')
    # Retain the exact raw compiler producer, including debug, before in-place use.
    if source.resolve() == output.resolve():
        source.with_suffix('.compiler.o').write_bytes(raw)
    output.write_bytes(result)
    print('reset: renamed 4 main / 1 ITCM entry / 2 ITCM reload sections; raw producer retained')

if __name__ == '__main__':
    if len(sys.argv) != 4:
        raise SystemExit('usage: rename_os_reset_sections.py RAW.o OUT.o PLAN.json')
    rename(*(Path(arg) for arg in sys.argv[1:]))
