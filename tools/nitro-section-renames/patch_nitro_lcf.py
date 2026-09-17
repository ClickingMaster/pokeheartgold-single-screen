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
path.write_text('\n'.join(lines)+'\n')
