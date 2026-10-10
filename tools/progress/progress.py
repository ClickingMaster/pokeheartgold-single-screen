#!/usr/bin/env python3
"""Build a source-language treemap from a matching ROM and its linker maps.

Only Python's standard library is required. This measures mapped code-section
bytes by source language, not a function-by-function matching percentage.

Functions listed in original_asm.json (next to this script) were hand-written in
assembly in the original program, are C functions whose original source used an
instruction that C cannot express (inline assembly), are produced by a build tool,
or are linker-style veneers. Inside objects that mix C and assembly, their
linker-map symbol bytes are reported as their own category, and the rest of the
object counts as C/C++ when no other assembly remains in it.
"""
import argparse
from collections import Counter, defaultdict
import hashlib
import html
import json
import math
from pathlib import Path
import re
import subprocess

CODE = {'.text', '.init', '.itcm', '.sinit', '.wram'}
ROW = re.compile(r'\s+([0-9A-Fa-f]{8}) ([0-9A-Fa-f]{8}) (\.\S+)\s+(.+?)\s+\(([^)]+)\)')
SOURCE_LANGUAGES = {'.c': 'c', '.cpp': 'cpp', '.s': 'asm'}
COLORS = {'c': '#35b85a', 'cpp': '#35a9b8', 'inline_asm': '#a5c75a', 'original_asm': '#6f8fd8',
          'generated': '#8d7bb0', 'veneer': '#9aa5b1', 'mixed': '#e4ad42', 'asm': '#455268', 'unknown': '#b875ae'}
LABELS = {'c': 'C-only source', 'cpp': 'C++-only source', 'inline_asm': 'C with original inline asm',
          'original_asm': 'Original hand-written assembly', 'generated': 'Build-tool generated',
          'veneer': 'Linker-style veneers', 'mixed': 'C/C++ with assembly', 'asm': 'Assembly',
          'unknown': 'Unresolved source'}
# Assembly written into the build directory by a build tool (dsprot's elfcoder), not source.
GENERATED_PREFIXES = ('lib/dsprot/build/',)
ORIGINAL_ASM = Path(__file__).with_name('original_asm.json')
# original_asm.json kinds and the status their symbol bytes receive.
ORIGINAL_KINDS = {'asm': 'original_asm', 'inline-asm': 'inline_asm', 'generated': 'generated', 'veneer': 'veneer'}
# Statuses that are not compiled code: excluded from the compiled-code denominator.
NOT_COMPILED = ('original_asm', 'generated', 'veneer')
# A veneer body: one jump through a literal, `ldr pc, =f` or `ldr ip, =f; bx ip`.
TRAMPOLINE = re.compile(r'\{\s*(?:ldr\s+pc\s*,\s*=\s*\w+|ldr\s+(ip|r12)\s*,\s*=\s*\w+\s*;?\s*bx\s+\1)\s*;?\s*\}\s*')


def code_family(section):
    """Return the code family for ordinary and named code sections."""
    for family in CODE:
        if section == family or section.startswith(family + '.'):
            return family
    return None


def union_size(ranges):
    end, total = -1, 0
    for start, stop in sorted(ranges):
        total += max(0, stop - max(start, end))
        end = max(end, stop)
    return total


def load_original_asm(path=ORIGINAL_ASM):
    """Return {(cpu, source): {map symbol: kind}} from the original-assembly list."""
    entries = json.loads(path.read_text())['functions']
    result = defaultdict(dict)
    for entry in entries:
        missing = {'function', 'cpu', 'source', 'reason'} - entry.keys()
        kind = entry.get('kind', 'asm')
        if missing or kind not in ORIGINAL_KINDS or entry['cpu'] not in ('ARM9', 'ARM7') \
                or not entry['reason'].strip():
            raise ValueError(f'{path.name}: invalid entry {entry}')
        functions = result[entry['cpu'], entry['source']]
        if entry['function'] in functions:
            raise ValueError(f'{path.name}: duplicate entry {entry["cpu"]} {entry["function"]}')
        functions[entry['function']] = kind
    return dict(result)


def source_index(root, arm7=False):
    base = root / 'sub' if arm7 else root
    candidates = defaultdict(set)
    for folder in ('src', 'asm', 'lib'):
        for p in (base / folder).rglob('*'):
            if p.suffix in SOURCE_LANGUAGES:
                candidates[p.stem + '.o'].add(p.relative_to(root).as_posix())
    # Archive members appear in the map as "libNAME.a member.o" (mwld lower-cases
    # member names); libNAME.a is built from lib/NAME/{src,asm}.
    for folder in ('src', 'asm'):
        for p in (base / 'lib').glob(f'*/{folder}/*'):
            if p.suffix in SOURCE_LANGUAGES:
                member = f'lib{p.parents[1].name}.a {p.stem.lower()}.o'
                candidates[member].add(p.relative_to(root).as_posix())
    # Objects a build rule derives from another compiled object (section
    # partitions) keep that object's source, including any assembly in it.
    rules = root / 'common.mk'
    derived = dict(re.findall(r'^\$\(BUILD_DIR\)/(\S+\.o):\s*\$\(BUILD_DIR\)/(\S+\.o)\s',
                              rules.read_text(), re.M)) if rules.is_file() else {}
    # The linker spec resolves duplicate source basenames left in the tree.
    spec = base / ('ichneumon_sub.lsf' if arm7 else 'main.lsf')
    linked = defaultdict(set)
    for obj in re.findall(r'^\s*Object\s+(\S+\.o)', spec.read_text(), re.M):
        for ext in SOURCE_LANGUAGES:
            p = (base / derived.get(obj, obj)).with_suffix(ext)
            if p.is_file():
                linked[Path(obj).name].add(p.relative_to(root).as_posix())
        # dsprot links encrypted/encoded copies of objects compiled from its C sources
        # (the decrypter stubs from src/*_decrypter.c).
        m = re.fullmatch(r'lib/dsprot/(\w+?)_(?:encrypted|encoded)\.o', obj)
        if m:
            p = base / 'lib/dsprot/src' / (m[1] + '.c')
            if p.is_file():
                linked[Path(obj).name].add(p.relative_to(root).as_posix())
            # Without a stub source, the stubs are assembled from elfcoder-generated assembly.
            elif m[1].endswith('_decrypter'):
                p = base / 'lib/dsprot/build' / (m[1] + '.s')
                if p.is_file():
                    linked[Path(obj).name].add(p.relative_to(root).as_posix())
    candidates.update(linked)
    return candidates


def classify(root, paths):
    if len(paths) != 1:
        return 'unknown', None
    path = next(iter(paths))
    if path.endswith('.s'):
        return ('generated' if path.startswith(GENERATED_PREFIXES) else 'asm'), path
    # Conservatively mark the entire object mixed, including conditional ASM and
    # assembly hidden in macros from the file's own quoted (#include "...") headers.
    mixed = _has_asm(root / path, set())
    return ('mixed' if mixed else SOURCE_LANGUAGES[Path(path).suffix]), path


ASM_TOKEN = re.compile(r'\b(?:asm|__asm|__asm__|GLOBAL_ASM)\b')


def _strip_comments(path):
    return re.sub(r'/\*.*?\*/|//[^\n]*', '', path.read_text(errors='replace'), flags=re.S)


def _has_asm(path, seen):
    if path in seen or not path.is_file():
        return False
    seen.add(path)
    text = _strip_comments(path)
    return bool(ASM_TOKEN.search(text)) or _includes_asm(path, text, seen)


def _includes_asm(path, text, seen):
    for name in re.findall(r'^\s*#\s*include\s+"([^"]+)"', text, re.M):
        for base in (path.parent, path.parent.parent / 'include'):
            if _has_asm(base / name, seen):
                return True
    return False


IDENT = re.compile(r'[A-Za-z_]\w*')
NOT_NAMES = {'asm', '__asm', '__asm__', '__attribute__', '__declspec', 'if', 'while', 'for', 'switch', 'return', 'sizeof'}


def _top_level(text):
    """Split comment-free source into top-level functions, declarations and macros.

    Returns (functions, declarations, macros): functions map a name to a list of
    (start, end, header) spans; declarations are (start, end) spans of statements
    ending in ';'; macros map a name to (start, end) spans of #define lines.
    """
    functions, declarations, macros = defaultdict(list), [], defaultdict(list)
    i, depth, statement, n = 0, 0, 0, len(text)
    while i < n:
        ch = text[i]
        if ch in '"\'':
            j = i + 1
            while j < n and text[j] != ch:
                j += 2 if text[j] == '\\' else 1
            i = j + 1
            continue
        if ch == '#' and depth == 0 and not text[text.rfind('\n', 0, i) + 1:i].strip():
            end = i
            while True:
                end = text.find('\n', end)
                if end < 0:
                    end = n
                    break
                if text[end - 1] != '\\':
                    break
                end += 1
            m = re.match(r'#\s*define\s+([A-Za-z_]\w*)', text[i:end])
            if m:
                macros[m[1]].append((i, end))
            i = statement = end
            continue
        if ch == '{':
            if depth == 0:
                opened = i
            depth += 1
        elif ch == '}' and depth:
            depth -= 1
            if depth == 0:
                header = text[statement:opened]
                if '(' in header and '=' not in header:
                    names = [m[1] for m in re.finditer(r'([A-Za-z_]\w*)\s*\(', header) if m[1] not in NOT_NAMES]
                    if names:
                        functions[names[0]].append((statement, i + 1, header))
                statement = i + 1
        elif ch == ';' and depth == 0:
            declarations.append((statement, i + 1))
            statement = i + 1
        i += 1
    return functions, declarations, macros


def source_name(symbol):
    """The unqualified source identifier of a (possibly demangled C++) map symbol."""
    return symbol.split('(', 1)[0].split('::')[-1].strip()


def split_original_asm(path, entries, cpu=None):
    """Apply original-assembly entries to one source file.

    Returns ({map symbol: kind} for entries the source confirms, whether any
    other assembly remains). An 'asm' entry applies only to a function defined
    with the asm keyword; an 'inline-asm' entry only to a C function that contains
    or inlines assembly; a 'veneer' entry only to an asm function whose body is a
    single literal jump; a 'generated' entry only to a name the source (or one of its
    local headers) defines, directly or through macros. Inline helpers and macros left
    unused after removing the listed functions are dropped; any other assembly keeps
    the remainder mixed.
    """
    text = _select_cpu(_strip_comments(path), cpu)
    local_headers = []
    for name in re.findall(r'^\s*#\s*include\s+"([^"]+)"', text, re.M):
        for base in (path.parent, path.parent.parent / 'include'):
            if (base / name).is_file():
                local_headers.append(_strip_comments(base / name))
                break
    functions, declarations, macros = _top_level(text)
    local = {name: spans for name, spans in functions.items()
             if all(re.search(r'\binline\b', header) for _, _, header in spans)}
    local.update({name: [(s, e, '') for s, e in spans] for name, spans in macros.items()})

    def uses_asm(name, seen):
        if name in seen:
            return False
        seen.add(name)
        for start, end, _ in functions.get(name, []) + local.get(name, []):
            body = text[start:end]
            if ASM_TOKEN.search(body) or any(word in local and uses_asm(word, seen)
                                             for word in set(IDENT.findall(body))):
                return True
        return False

    applied, removed = {}, []
    for symbol, kind in entries.items():
        name = source_name(symbol)
        # Every definition (one per conditional-compilation branch) must have the listed form.
        spans = functions.get(name, [])
        asm_headers = [bool(ASM_TOKEN.search(header)) for _, _, header in spans]
        if spans and kind in ('asm', 'generated') and all(asm_headers) or \
                spans and kind == 'veneer' and all(asm_headers) and \
                all(TRAMPOLINE.fullmatch(text[s:e][text[s:e].index('{'):]) for s, e, _ in spans):
            removed += [(s, e) for s, e, _ in spans]
            removed += [(s, e) for s, e in declarations
                        if ASM_TOKEN.search(text[s:e]) and re.search(rf'\b{re.escape(name)}\s*\(', text[s:e])]
        elif spans and kind == 'inline-asm' and not any(asm_headers) and uses_asm(name, set()):
            removed += [(s, e) for s, e, _ in spans]
        elif not spans and kind == 'generated' and any(
                re.search(rf'\b{re.escape(name)}\b', t) for t in [text] + local_headers):
            pass  # defined through a macro: the macro use stays, so the rest stays mixed
        else:
            continue
        applied[symbol] = kind
    if not applied:
        return applied, True

    def blank(spans):
        nonlocal remaining
        for s, e in spans:
            remaining = remaining[:s] + re.sub(r'[^\n]', ' ', remaining[s:e]) + remaining[e:]

    remaining = text
    blank(removed)
    changed = True
    while changed:  # drop inline helpers and macros nothing else uses any more
        changed = False
        for name, spans in local.items():
            if not any(remaining[s:e].strip() for s, e, _ in spans):
                continue
            outside = remaining
            for s, e, _ in spans:
                outside = outside[:s] + ' ' * (e - s) + outside[e:]
            if not re.search(rf'\b{re.escape(name)}\b', outside):
                blank([(s, e) for s, e, _ in spans])
                changed = True
    other = bool(ASM_TOKEN.search(remaining)) or _includes_asm(path, text, {path})
    return applied, other


CPU_MACROS = {'ARM9': 'SDK_ARM9', 'ARM7': 'SDK_ARM7'}


def _select_cpu(text, cpu):
    """Blank the branches of top-level #ifdef/#ifndef SDK_ARM9 / SDK_ARM7 blocks that this
    CPU does not compile, keeping offsets. Other conditionals are left untouched."""
    if cpu not in CPU_MACROS:
        return text
    lines = text.split('\n')
    stack = []  # per open #if: (is_cpu_block, active_now)
    out = []
    for line in lines:
        m = re.match(r'\s*#\s*(ifdef|ifndef|if|else|elif|endif)\b\s*(\w*)', line)
        if m:
            kw, arg = m.group(1), m.group(2)
            if kw in ('ifdef', 'ifndef'):
                cpu_block = arg in CPU_MACROS.values()
                active = (arg == CPU_MACROS[cpu]) == (kw == 'ifdef') if cpu_block else True
                stack.append([cpu_block, active])
            elif kw == 'if':
                stack.append([False, True])
            elif kw == 'else' and stack and stack[-1][0]:
                stack[-1][1] = not stack[-1][1]
            elif kw == 'elif' and stack and stack[-1][0]:
                stack[-1] = [False, True]  # unknown form: stop selecting
            elif kw == 'endif' and stack:
                stack.pop()
            out.append(line)
            continue
        live = all(active for cpu_block, active in stack if cpu_block)
        out.append(line if live else re.sub(r'\S', ' ', line))
    return '\n'.join(out)


def split_mixed(root, row, symbols, entries):
    """Split a mixed object row by original-assembly functions and the remainder."""
    if not entries:
        return [row]
    # An entry counts only when the object really has bytes for that symbol.
    present = {name for name, _, _, _ in symbols}
    applied, other_asm = split_original_asm(root / row['source'],
                                            {name: kind for name, kind in entries.items() if name in present},
                                            row.get('cpu'))
    spans = defaultdict(lambda: defaultdict(list))
    names = defaultdict(set)
    for name, family, start, stop in symbols:
        if name in applied:
            spans[applied[name]][family].append((start, stop))
            names[applied[name]].add(name)
    rows, rest = [], row['bytes']
    for kind in ORIGINAL_KINDS:
        size = sum(union_size(ranges) for ranges in spans[kind].values())
        if size:
            rest -= size
            rows.append(dict(row, bytes=size, status=ORIGINAL_KINDS[kind], functions=sorted(names[kind])))
    if rest:
        language = SOURCE_LANGUAGES[Path(row['source']).suffix]
        rows.insert(0, dict(row, bytes=rest, status='mixed' if other_asm else language))
    return rows


def source_coverage(totals):
    """Separate clean C/C++ coverage, using the existing classified denominator."""
    denominator = sum(totals.values())
    c_bytes, cpp_bytes = totals.get('c', 0), totals.get('cpp', 0)
    original, inline = totals.get('original_asm', 0), totals.get('inline_asm', 0)
    def portion(size):
        return dict(bytes=size, percent=100 * size / denominator if denominator else 0)
    def share(size, of):
        return dict(bytes=size, of=of, percent=100 * size / of if of else 0)
    source_bytes = denominator - totals.get('generated', 0)
    with_asm = c_bytes + cpp_bytes + totals.get('mixed', 0) + original + inline + totals.get('veneer', 0)
    return dict(denominator_bytes=denominator, c=portion(c_bytes),
                cpp=portion(cpp_bytes), combined=portion(c_bytes + cpp_bytes),
                original_asm=portion(original), inline_asm=portion(inline),
                generated=portion(totals.get('generated', 0)), veneer=portion(totals.get('veneer', 0)),
                with_asm_functions=portion(with_asm),
                source_files_in_c=share(with_asm, source_bytes),
                compiled_code_in_c=share(c_bytes + cpp_bytes + inline,
                                         denominator - sum(totals.get(s, 0) for s in NOT_COMPILED)))


def parse_map(path, root, cpu, original=None):
    if original is None:
        original = load_original_asm()
    sources = source_index(root, cpu == 'ARM7')
    ranges = defaultdict(list)
    symbols = defaultdict(list)
    module = None
    for line in path.read_text().splitlines():
        if line.startswith('# .'):
            module = line[3:]
        match = ROW.fullmatch(line)
        if not match or module is None:
            continue
        address, size, section, symbol, obj = match.groups()
        size = int(size, 16)
        family = code_family(section)
        if size and family is not None:
            start = int(address, 16)
            ranges[module, obj.strip(), family].append((start, start + size))
            symbols[module, obj.strip()].append((symbol.strip(), family, start, start + size))
    objects = Counter()
    for (module, obj, section), intervals in ranges.items():
        objects[module, obj] += union_size(intervals)
    result = []
    for (module, obj), size in sorted(objects.items()):
        status, source = classify(root, sources.get(obj, set()))
        row = dict(cpu=cpu, module=module, object=obj, bytes=size, status=status, source=source)
        if status == 'mixed':
            result += split_mixed(root, row, symbols[module, obj], original.get((cpu, source), {}))
        else:
            result.append(row)
    if not result:
        raise ValueError(f'No code symbols found in {path}')
    return result


def layout(items, x, y, width, height):
    """Deterministic balanced binary treemap; rectangle area tracks bytes."""
    if not items:
        return
    if len(items) == 1:
        yield items[0], x, y, width, height
        return
    total = sum(item['bytes'] for item in items)
    acc, split = 0, 1
    best = float('inf')
    for i, item in enumerate(items[:-1], 1):
        acc += item['bytes']
        if abs(total / 2 - acc) < best:
            best, split = abs(total / 2 - acc), i
    fraction = sum(item['bytes'] for item in items[:split]) / total
    if width >= height:
        cut = width * fraction
        yield from layout(items[:split], x, y, cut, height)
        yield from layout(items[split:], x + cut, y, width - cut, height)
    else:
        cut = height * fraction
        yield from layout(items[:split], x, y, width, cut)
        yield from layout(items[split:], x, y + cut, width, height - cut)


def headline_percent(share):
    """Two decimals, truncated so that only complete coverage reads 100%."""
    if share['bytes'] == share['of']:
        return '100%'
    return f'{math.floor(share["percent"] * 100) / 100:.2f}%'


def svg(report):
    total = sum(report['totals'].values())
    coverage = source_coverage(report['totals'])
    pct = coverage['combined']['percent']
    game_title = 'HeartGold' if report['game'] == 'heartgold.us' else 'SoulSilver'
    esc = html.escape
    out = ['<svg xmlns="http://www.w3.org/2000/svg" width="1200" height="780" viewBox="0 0 1200 780" role="img">',
           f'<title>{game_title} decompilation source-language treemap</title>',
           '<desc>Rectangle area represents mapped code bytes. Green is C-only source, teal is C++-only source, lime is C with original inline asm, blue is original hand-written assembly, lilac is build-tool generated, grey is linker-style veneers, amber is mixed source and assembly, slate is assembly, purple is unresolved. Hover for file details.</desc>',
           '<rect width="1200" height="780" rx="16" fill="#101722"/>',
           '<g font-family="system-ui, sans-serif" fill="#edf2fa">',
           f'<text x="28" y="42" font-size="25" font-weight="700">{game_title} · decompilation</text>',
           f'<text x="28" y="71" font-size="14" fill="#b9c7da">Matching US ROM verified · {esc(report["commit"][:9])} · ARM9 + ARM7 · {total:,} mapped code bytes</text>']
    if any(report['totals'].get(s) for s in ('mixed', 'generated', 'original_asm', 'inline_asm', 'veneer')):
        files = headline_percent(coverage['source_files_in_c'])
        compiled = headline_percent(coverage['compiled_code_in_c'])
        out += [f'<text x="1172" y="44" text-anchor="end" font-size="30" font-weight="700">{files} of source in C/C++</text>',
                f'<text x="1172" y="71" text-anchor="end" font-size="14" fill="#b9c7da">{compiled} compiled code in C/C++ · '
                f'{pct:.2f}% C + C++ source</text>']
    else:
        out += [f'<text x="1172" y="42" text-anchor="end" font-size="22">{pct:.2f}% C + C++ source</text>']
    statuses = [s for s in COLORS if report['totals'].get(s, 0)]
    shares = [report['totals'][s] / total * 100 for s in statuses]
    legend = [f'{LABELS[s]} {share:.1f}%' if share >= 0.05 else f'{LABELS[s]} <0.1%' for s, share in zip(statuses, shares)]
    # Flow the legend by estimated label width; wrap to a second row when it does not fit.
    font, x, row = 14, 28, 0
    for status, text in zip(statuses, legend):
        width = 19 + len(text) * font * 0.55
        if x > 28 and x + width > 1172:
            x, row = 28, row + 1
        y = 91 + row * 22
        out += [f'<rect x="{x:.0f}" y="{y}" width="12" height="12" rx="2" fill="{COLORS[status]}"/>',
                f'<text x="{x+19:.0f}" y="{y+11}" font-size="{font}">{esc(text)}</text>']
        x += width + 12
    top = 125 + row * 22
    groups = defaultdict(list)
    for item in report['objects']:
        groups[item['cpu'], item['module']].append(item)
    modules = [dict(name=f'{cpu} / {module}', bytes=sum(o['bytes'] for o in items), children=items)
               for (cpu, module), items in sorted(groups.items())]
    modules.sort(key=lambda m: (-m['bytes'], m['name']))
    for module, x, y, w, h in layout(modules, 28, top, 1144, 717 - top):
        for item, ox, oy, ow, oh in layout(sorted(module['children'], key=lambda o: (-o['bytes'], o['object'], o['status'])), x, y, w, h):
            title = f"{module['name']} / {item['object']}\n{item['bytes']:,} bytes · {LABELS[item['status']]}\n{item['source'] or 'Source mapping unresolved'}"
            if item.get('functions'):
                shown = item['functions'][:6]
                title += '\n' + ', '.join(shown) + (f' and {len(item["functions"]) - len(shown)} more' if len(item['functions']) > len(shown) else '')
            out += [f'<rect x="{ox:.3f}" y="{oy:.3f}" width="{ow:.3f}" height="{oh:.3f}" fill="{COLORS[item["status"]]}" stroke="#101722" stroke-width="0.6"><title>{esc(title)}</title></rect>']
        out += [f'<rect x="{x:.3f}" y="{y:.3f}" width="{w:.3f}" height="{h:.3f}" fill="none" stroke="#101722" stroke-width="2"/>']
        if w > 140 and h > 40:
            label = module['name']
            out += [f'<rect x="{x+3:.3f}" y="{y+3:.3f}" width="{min(w-6, len(label)*7+14):.3f}" height="22" rx="3" fill="#101722" opacity="0.88"/>',
                    f'<text x="{x+9:.3f}" y="{y+19:.3f}" font-size="12">{esc(label)}</text>']
    out += ['<text x="28" y="744" font-size="14" fill="#b9c7da">Area = code-section bytes · Hover for files · Mixed objects are excluded from C + C++ source · '
            'Hand-written, generated and veneer code is excluded from compiled code</text>',
            '<text x="28" y="765" font-size="12" fill="#b9c7da">Source-language coverage, not a per-function matching score. Assets and unmapped padding are excluded; code literal pools are included.</text>',
            '</g></svg>']
    return '\n'.join(out) + '\n'


def generate(root, game):
    rom = root / 'build' / game / f'poke{game}.nds'
    expected = (root / game / 'rom.sha1').read_text().split()[0]
    actual = hashlib.sha1(rom.read_bytes()).hexdigest()
    if actual != expected:
        raise ValueError(f'ROM mismatch: expected {expected}, got {actual}')
    maps = {'ARM9': root / 'build' / game / 'main.elf.xMAP',
            'ARM7': root / 'sub/build/ichneumon_sub.elf.xMAP'}
    original = load_original_asm()
    objects = []
    for cpu, path in maps.items():
        objects += parse_map(path, root, cpu, original)
    totals = Counter()
    for item in objects:
        totals[item['status']] += item['bytes']
    return dict(schema_version=3, game=game, commit=subprocess.check_output(
        ['git', 'rev-parse', 'HEAD'], cwd=root, text=True).strip(), rom_sha1=actual,
        tracked_worktree_dirty=bool(subprocess.check_output(
            ['git', 'status', '--porcelain', '--untracked-files=no'], cwd=root, text=True)),
        map_sha256={cpu: hashlib.sha256(path.read_bytes()).hexdigest() for cpu, path in maps.items()},
        metric='Mapped code-section bytes classified by source language; mixed objects excluded from clean C, C++, '
               'and combined source coverage. Functions listed in tools/progress/original_asm.json are split out of '
               'mixed objects by linker-map symbol: original hand-written assembly, build-tool generated code and '
               'linker-style veneers are excluded from the compiled-code denominator, and C functions with '
               'original inline asm count as compiled C.',
        totals=dict(totals), coverage=source_coverage(totals), objects=objects)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument('--game', choices=['heartgold.us', 'soulsilver.us'], default='heartgold.us')
    parser.add_argument('--output', type=Path, default=Path('build/progress'))
    args = parser.parse_args()
    try:
        report = generate(args.root.resolve(), args.game)
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        parser.exit(1, f'progress: {error}\nRun the matching build before generating progress.\n')
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / 'progress.json').write_text(json.dumps(report, indent=2) + '\n')
    (args.output / 'treemap.svg').write_text(svg(report))
    print(json.dumps({k: report[k] for k in ('game', 'rom_sha1', 'totals', 'coverage')}, indent=2))
    print(f'Treemap: {args.output / "treemap.svg"}')


if __name__ == '__main__':
    main()
