"""Finite whole array88 ELF producer selection: no payload or relocation endpoint patches."""
import struct,hashlib,json
H='<16sHHIIIIIHHHHHH';S='<10I';Y='<IIIBBH';NAMES=('_Znam','_ZnamRKSt9nothrow_t')
def digest(b):return hashlib.sha256(b).hexdigest()
def _select(b):
 h=list(struct.unpack_from(H,b));assert h[0][:7]==b'\x7fELF\x01\x01\x01' and h[1:3]==[1,40] and h[11]==40
 ss=[list(struct.unpack_from(S,b,h[6]+40*i))for i in range(h[12])]
 get=lambda t,o:t[o:].split(b'\0',1)[0].decode()
 names=b[ss[h[13]][4]:ss[h[13]][4]+ss[h[13]][5]];ns=[get(names,s[0])for s in ss]
 st=next(i for i,s in enumerate(ss)if s[1]==2);strings=ss[st][6];t=b[ss[strings][4]:ss[strings][4]+ss[strings][5]]
 yy=[list(struct.unpack_from(Y,b,ss[st][4]+16*i))for i in range(ss[st][5]//16)];yn=[get(t,y[0])for y in yy]
 funcs=[(i,n,y)for i,(n,y)in enumerate(zip(yn,yy))if y[3]&15==2 and y[2]]
 assert [(n,y[2])for _,n,y in funcs]==[(n,44)for n in NAMES]
 codes={f[2][5]for f in funcs};assert len(codes)==2
 for code in codes:assert ss[code][1:4]==[1,0x20000006,0] and ss[code][5:]==[44,0,0,4,0]
 assert [(ns[i],s[5])for i,s in enumerate(ss)if s[2]&2 and s[5]]==[('.data',17),('.exceptix',12),('.exception',21),('.text',44),('.exceptix',12),('.exception',17),('.text',44)]
 rels=[i for i,s in enumerate(ss)if s[1]in(4,9)and s[7]in codes];assert len(rels)==2
 owned={0,st,strings,h[13],*codes,*rels};sm={i:j for j,i in enumerate(sorted(owned))}
 keep={0}|{i for i,y in enumerate(yy)if y[5]in codes}
 for i in rels:
  s=ss[i];assert s[1]==4 and s[9]==12 and s[6]==st
  for p in range(s[4],s[4]+s[5],12):
   off,info,add=struct.unpack_from('<IIi',b,p);j=info>>8
   assert yy[j][5]==0 or yy[j][5]in codes
   keep.add(j)
 assert {yn[i]for i in keep if yy[i][5]==0 and i}=={'__unexpected','_Znwm','_ZnwmRKSt9nothrow_t'}
 order=[0]+sorted(i for i in keep if i and yy[i][3]>>4==0)+sorted(i for i in keep if i and yy[i][3]>>4!=0);ym={i:j for j,i in enumerate(order)}
 payload={i:b[s[4]:s[4]+s[5]]for i,s in enumerate(ss)if i in owned};newy=[]
 for i in order:
  y=yy[i].copy()
  if y[5]:y[5]=sm[y[5]]
  newy.append(y)
 payload[st]=b''.join(struct.pack(Y,*y)for y in newy)
 for i in rels:
  s=ss[i];v=bytearray()
  for p in range(s[4],s[4]+s[5],12):
   off,info,add=struct.unpack_from('<IIi',b,p);v+=struct.pack('<IIi',off,(ym[info>>8]<<8)|(info&255),add)
  payload[i]=bytes(v)
 out=bytearray(b[:52]);newss=[]
 for i in sorted(owned):
  s=ss[i].copy()
  if i==0:newss.append([0]*10);continue
  out+=b'\0'*((-len(out))%max(1,s[8]));s[4]=len(out);out+=payload[i];s[5]=len(payload[i])
  if s[6]:s[6]=sm[s[6]]
  if s[1]in(4,9):s[7]=sm[s[7]]
  if i==st:s[7]=sum(y[3]>>4==0 for y in newy)
  newss.append(s)
 out+=b'\0'*((-len(out))%4);nh=h.copy();nh[6]=len(out);nh[12]=len(newss);nh[13]=sm[h[13]]
 out+=b''.join(struct.pack(S,*s)for s in newss);out[:52]=struct.pack(H,*nh)
 return bytes(out),{'original_sha256':digest(b),'selected_sha256':digest(out),'section_map':sm,'symbol_map':ym,'all_section_dispositions':[{'index':i,'name':n,'size':ss[i][5],'flags':ss[i][2],'disposition':'selected'if i in owned else'excluded compiler typename/EH/index/debug','sha256':digest(b[ss[i][4]:ss[i][4]+ss[i][5]])}for i,n in enumerate(ns)],'all_symbol_dispositions':[{'index':i,'name':n,'record':y,'disposition':'selected'if i in keep else'excluded'}for i,(n,y)in enumerate(zip(yn,yy))],'modifications':'ELF structural section/symbol indices and offsets only. Entire88-byte two-function payload, flags, alignment, import identities, relocation offsets/types/addends unchanged. All generated type-name/EH/index and debug sections excluded explicitly.'}

import shape
from pathlib import Path
def select(b,source):
 source=Path(source).resolve()
 assert source.name=='to91_msl_array_new.cpp', 'source basename drift'
 assert digest(source.read_bytes())=='1ec5af70c0bb3b5b0970d4b34317aae4cf441f2c1a30315401d8a56897b88458', 'source bytes drift'
 expected=json.loads((Path(__file__).parent/'array-shape.json').read_text())
 assert shape.inventory(b,source)==expected, 'complete producer shape drift'
 return _select(b)
