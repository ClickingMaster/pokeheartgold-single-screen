#!/usr/bin/env python3
"""Insert the genuine C getter between the two os_thread C text partitions."""
import re, sys
from pathlib import Path

def rewrite(text):
    rows = text.splitlines(keepends=True)
    old = [i for i, line in enumerate(rows) if line.strip() == 'os_thread.o (.text)']
    getter = [i for i, line in enumerate(rows) if line.strip() == 'to90_os_thread_getter16.o (.text)']
    if len(old) != 1 or len(getter) != 1:
        raise ValueError('exactly one original and one new getter text selector required')
    at = old[0]
    if getter[0] != at + 1:
        raise ValueError('getter must immediately follow os_thread in the generated plan')
    if at == 0 or getter[0] + 1 >= len(rows):
        raise ValueError('missing original neighbor selectors')
    if rows[at - 1].strip() != 'os_printf.o (.text)' or rows[getter[0] + 1].strip() != 'os_context.o (.text)':
        raise ValueError('os_thread original text placement neighbors changed')
    if any('.text.os_thread_' in line for line in rows):
        raise ValueError('already routed or unexpected os_thread text partition')
    start = text.find('SDK_STATIC_TEXT_START')
    end = text.find('SDK_STATIC_TEXT_END')
    position = sum(map(len, rows[:at]))
    if not 0 <= start < position < end:
        raise ValueError('os_thread selector must stay inside the original static text block')
    indent = rows[at][:-len(rows[at].lstrip())]
    newline = '\r\n' if rows[at].endswith('\r\n') else '\n'
    replacement = [indent + 'os_thread.o (.text.os_thread_before_getter)' + newline,
                   indent + 'to90_os_thread_getter16.o (.text)' + newline,
                   indent + 'os_thread.o (.text.os_thread_after_getter)' + newline]
    return ''.join(rows[:at] + replacement + rows[getter[0] + 1:])

if __name__ == '__main__':
    if len(sys.argv) != 2:
        raise SystemExit('usage: patch_os_thread_lcf.py MAIN.LCF')
    path = Path(sys.argv[1])
    original = path.read_text()
    path.write_text(rewrite(original))
