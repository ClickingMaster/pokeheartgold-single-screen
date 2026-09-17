#!/usr/bin/env python3
import json,struct,sys
from pathlib import Path
src,out,mapfile=map(Path,sys.argv[1:4]);M=json.loads(mapfile.read_text());mapping={**M['text'],**M['sections']}; undef=set(M.get('undefine',[]))
b=bytearray(src.read_bytes());assert b[:6]==b'\x7fELF\x01\x01';shoff=struct.unpack_from('<I',b,32)[0];es=struct.unpack_from('<H',b,46)[0];n=struct.unpack_from('<H',b,48)[0];ssidx=struct.unpack_from('<H',b,50)[0];assert es==40
S=[list(struct.unpack_from('<10I',b,shoff+i*es)) for i in range(n)];syi=[i for i,s in enumerate(S) if s[1]==2];assert len(syi)==1;sy=S[syi[0]];st=S[sy[6]];strs=bytes(b[st[4]:st[4]+st[5]])
def cs(x,o):return x[o:x.index(b'\0',o)].decode()
found={}
for p in range(sy[4],sy[4]+sy[5],sy[9]):
 no,val,size=struct.unpack_from('<III',b,p);nm=cs(strs,no) if no else '';idx=struct.unpack_from('<H',b,p+14)[0]
 if nm in undef:
  struct.pack_into('<II',b,p+4,0,0); struct.pack_into('<H',b,p+14,0)
 if nm in mapping:
  if val!=0:raise SystemExit(f'{nm} has nonzero section offset {val:#x}')
  if nm in found:raise SystemExit(f'duplicate {nm}')
  found[nm]=idx
missing=set(mapping)-set(found)
if missing:raise SystemExit(f'missing {sorted(missing)}')
# All anchors for a section must request the same name; selected functions may share a requested output name but not an input section.
bysec={}
for nm,idx in found.items():
 want=mapping[nm]
 if idx in bysec and bysec[idx]!=want:raise SystemExit(f'section {idx}: conflicting names {bysec[idx]} / {want}')
 bysec[idx]=want
ss=S[ssidx];old=bytes(b[ss[4]:ss[4]+ss[5]]);new=bytearray(old);offs={}
for idx,want in bysec.items():offs[idx]=len(new);new+=want.encode()+b'\0'
new_off=len(b);b+=new;struct.pack_into('<II',b,shoff+ssidx*es+16,new_off,len(new))
for idx,no in offs.items():struct.pack_into('<I',b,shoff+idx*es,no)
out.write_bytes(b);print(f'{src.name}: renamed {len(bysec)} sections')
