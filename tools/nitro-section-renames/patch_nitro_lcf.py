#!/usr/bin/env python3
"""Replace makelcf's whole-object Nitro selectors with verified split-section order."""
import json, sys
from pathlib import Path
if len(sys.argv) != 2:
    raise SystemExit('usage: patch_nitro_lcf.py MAIN.LCF')
path=Path(sys.argv[1]); text=path.read_text(); lines=text.splitlines()
plan=json.loads((Path(__file__).with_name('nitro_link_order.json')).read_text())
basenames={Path(x).name for x in plan['objects']}
for sec, rows in plan['sections'].items():
    needle={f'{name} (.{sec})' for name in basenames}
    indices=[i for i,line in enumerate(lines) if line.strip() in needle]
    if len(indices) != len(basenames):
        raise SystemExit(f'{path}: expected {len(basenames)} .{sec} selectors, found {len(indices)}')
    at=min(indices); indent=lines[at][:-len(lines[at].lstrip())]
    remove=set(indices)
    lines=[line for i,line in enumerate(lines) if i not in remove]
    ordered=[f"{indent}{Path(row['path']).name} ({row['selector']})" for row in rows]
    lines[at:at]=ordered
def apply_finite_interleaves(lines, groups):
    for group in groups:
        old = group['input']
        assert len(old) == len(set(old)) and old
        found = [i for i, line in enumerate(lines) if line.strip() in set(old)]
        assert len(found) == len(old), 'finite interleave missing/duplicate input'
        at = min(found)
        assert found == list(range(at, at + len(old)))
        assert [line.strip() for line in lines[at:at + len(old)]] == old
        assert lines[at - 1].strip() == group['previous']
        assert lines[at + len(old)].strip() == group['next']
        pos = sum(len(line) + 1 for line in lines[:at])
        text = '\n'.join(lines)
        assert 0 <= text.find(group['start_marker']) < pos < text.find(group['end_marker'])
        indent = lines[at][:-len(lines[at].lstrip())]
        lines[at:at + len(old)] = [indent + row for row in group['output']]
    return lines
lines = apply_finite_interleaves(lines, plan.get('finite_interleaves', []))
path.write_text('\n'.join(lines)+'\n')
