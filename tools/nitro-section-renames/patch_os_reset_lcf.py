#!/usr/bin/env python3
"""Route the reviewed single reset C producer around genuine retained ASM."""
from pathlib import Path
import sys

def rewrite(text):
 rows=text.splitlines(keepends=True)
 def one(value):
  matches=[i for i,x in enumerate(rows) if x.strip()==value]
  if len(matches)!=1:raise ValueError('unique exact reset selector required: '+value)
  return matches[0]
 main=one('to90_reset_pure.o (.text)');asm=one('to90_reset_reset_system.o (.text)')
 system=one('os_system.o (.text)')
 split_system=[i for i,x in enumerate(rows) if x.strip()=='to91_os_system_pure.o (.text)']
 if split_system:
  preceding_system=(len(split_system)==1 and split_system[0]==system+1 and main==system+2)
 else:
  preceding_system=(main==system+1)
 if asm!=main+1 or not preceding_system or rows[asm+1].strip()!='os_ownerInfo.o (.text)':raise ValueError('original reset main neighbors/order')
 entry=one('to90_reset_pure.o (.itcm)');boot=one('to90_reset_boot.o (.itcm)');clear=one('to90_reset_clear32.o (.itcm)')
 if (boot,clear)!=(entry+1,entry+2) or rows[entry-1].strip()!='os_irqHandler.o (.itcm)' or rows[clear+1].strip()!='mi_dma.o (.itcm)':raise ValueError('original reset ITCM neighbors/order')
 if any('.to90_reset_' in x for x in rows):raise ValueError('already reset-routed')
 rows[main]=rows[main].replace('(.text)','(.text.to90_reset_main_C)')
 rows[entry]=rows[entry].replace('(.itcm)','(.itcm.to90_reset_entry_C)')
 rows.insert(clear+1,rows[entry].replace('.itcm.to90_reset_entry_C','.itcm.to90_reset_reload_C'))
 return ''.join(rows)
if __name__=='__main__':
 if len(sys.argv)!=2:raise SystemExit('usage: patch_os_reset_lcf.py MAIN.LCF')
 p=Path(sys.argv[1]);p.write_text(rewrite(p.read_text()))
