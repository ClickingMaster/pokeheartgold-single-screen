#!/usr/bin/env python3
'''Rename ELF32 section names by the GLOBAL function symbol they contain.
Fail closed: rejects non-ELF32-LE, missing/duplicate functions, shared sections, and nonzero symbol offsets.
The payload, relocation records, symbol table and all section indices remain byte-for-byte unchanged;
only sh_name fields and the relocated .shstrtab are changed.
'''
import json,struct,sys
from pathlib import Path
src,out,mapfile=map(Path,sys.argv[1:4]); mapping=json.loads(mapfile.read_text())
b=bytearray(src.read_bytes())
if b[:6]!=b'\x7fELF\x01\x01': raise SystemExit('not ELF32 little endian')
shoff=struct.unpack_from('<I',b,32)[0]; entsz=struct.unpack_from('<H',b,46)[0]; n=struct.unpack_from('<H',b,48)[0]; shstridx=struct.unpack_from('<H',b,50)[0]
if entsz!=40: raise SystemExit(f'unexpected shentsize {entsz}')
def sh(i): return list(struct.unpack_from('<10I',b,shoff+i*entsz))
sects=[sh(i) for i in range(n)]
symtabs=[(i,s) for i,s in enumerate(sects) if s[1]==2]
if len(symtabs)!=1: raise SystemExit(f'expected one symtab, got {len(symtabs)}')
si,sy=symtabs[0]; st=sects[sy[6]]; strtab=bytes(b[st[4]:st[4]+st[5]])
def cstr(buf,o): return buf[o:buf.index(b'\0',o)].decode()
found={}; secowners={}
for off in range(sy[4],sy[4]+sy[5],sy[9] or 16):
 name,val,size=struct.unpack_from('<III',b,off); info=b[off+12]; shndx=struct.unpack_from('<H',b,off+14)[0]
 nm=cstr(strtab,name) if name else ''
 if nm in mapping:
  if val!=0: raise SystemExit(f'{nm}: nonzero section-relative value {val:#x}')
  if nm in found: raise SystemExit(f'{nm}: duplicate symbol')
  found[nm]=shndx; secowners.setdefault(shndx,[]).append(nm)
missing=set(mapping)-set(found)
if missing: raise SystemExit(f'missing: {sorted(missing)}')
for idx,names in secowners.items():
 if len(names)!=1: raise SystemExit(f'section {idx} contains multiple selected functions {names}')
# append a complete new shstrtab and repoint section header; old bytes remain inert
ss=sects[shstridx]; old=bytes(b[ss[4]:ss[4]+ss[5]]); new=bytearray(old); offsets={}
for nm,idx in found.items():
 secname=mapping[nm]
 if not secname.startswith('.text.card_'): raise SystemExit(f'unsafe section name {secname}')
 offsets[idx]=len(new); new += secname.encode()+b'\0'
new_off=len(b); b += new
struct.pack_into('<I',b,shoff+shstridx*entsz+16,new_off)
struct.pack_into('<I',b,shoff+shstridx*entsz+20,len(new))
for idx,noff in offsets.items(): struct.pack_into('<I',b,shoff+idx*entsz,noff)
out.write_bytes(b)
print(f'renamed {len(found)} function sections; payload and relocations untouched')
