#!/usr/bin/env python3
"""Finite scratch-only exception SDK producer route; no tracked writes or installation."""
import argparse,collections,hashlib,json,os,pathlib,struct,subprocess
P=pathlib.Path(__file__).resolve().parent;R=next(p for p in P.parents if (p/'common.mk').is_file())
SOURCE=R/'lib/NitroSDK/src/os/os_exception.c'
ALIAS={17:'to91_OSi_ExceptionWork',18:'to91_OSi_ExContext'}
FUNC=['OS_InitException','OSi_ExceptionHandler','OSi_GetAndDisplayContext','OSi_SetExContext','OSi_DisplayExContext']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def elf(p):
 b=p.read_bytes();assert b[:7]==b'\x7fELF\x01\x01\x01';h=struct.unpack_from('<16sHHIIIIIHHHHHH',b);assert h[1:3]==(1,40) and h[11]==40
 se=[list(struct.unpack_from('<10I',b,h[6]+40*i)) for i in range(h[12])];st=se[h[13]];strings=b[st[4]:st[4]+st[5]]
 def txt(blob,o):return blob[o:blob.index(b'\0',o)].decode()
 sections=[{'hdr':s,'name':txt(strings,s[0]),'payload':b[s[4]:s[4]+s[5]] if s[1]!=8 else b''}for s in se]
 sy=[];rels={}
 for i,s in enumerate(se):
  if s[1]!=2:continue
  z=se[s[6]];names=b[z[4]:z[4]+z[5]]
  for off in range(s[4],s[4]+s[5],16):
   n,v,size,info,other,sh=struct.unpack_from('<IIIBBH',b,off);sy.append({'name':txt(names,n),'val':v,'size':size,'type':info&15,'bind':info>>4,'other':other,'sh':sh})
 for s in se:
  if s[1]==4:
   rels[s[7]]=[struct.unpack_from('<IIi',b,x)for x in range(s[4],s[4]+s[5],12)]
 return h,sections,sy,rels

def partition(src,dest,keep):
 h,se,sy,rels=elf(src);out=[{'hdr':[0]*10,'name':'','payload':b''}];mapping={}
 for i in keep:
  mapping[i]=len(out);s=se[i];out.append({'hdr':s['hdr'][:],'name':s['name'],'payload':s['payload']})
 # Preserve every native symbol in retained allocated sections. Explicit aliases are new globals only.
 selected=[(i,dict(s))for i,s in enumerate(sy)if i==0 or s['sh'] in keep]
 oldidx={};extra={}
 for i,s in selected:
  if s['sh'] in mapping:s['sh']=mapping[s['sh']]
 def imported(i):
  s=sy[i]
  if s['sh'] in keep:return ('old',i),dict(s),0
  if s['sh'] in ALIAS:
   return ('alias',s['sh']),{'name':ALIAS[s['sh']],'val':0,'size':0,'type':0,'bind':1,'other':0,'sh':0},s['val']
  assert s['sh']==0 or (s['bind']==1 and s['type']==2),(i,s)
  z=dict(s);z.update(sh=0,val=0,size=0,type=0)
  return ('old',i),z,0
 for owner in keep:
  for off,info,ad in rels.get(owner,[]):
   i=info>>8
   if not any(x==i for x,z in selected):
    key,z,add=imported(i);extra[key]=z
 if keep==[17,18]:
  for i in keep:extra[('defalias',i)]={'name':ALIAS[i],'val':0,'size':0,'type':0,'bind':1,'other':0,'sh':mapping[i]}
 entries=[(('old',i),s)for i,s in selected]+list(extra.items())
 entries.sort(key=lambda x:x[1]['bind']!=0);index={key:i for i,(key,s)in enumerate(entries)}
 names=bytearray(b'\0');nameoffs={ '':0 }
 def nameoff(s):
  if s not in nameoffs:nameoffs[s]=len(names);names.extend(s.encode()+b'\0')
  return nameoffs[s]
 symblob=b''.join(struct.pack('<IIIBBH',nameoff(s['name']),s['val'],s['size'],s['bind']*16+s['type'],s['other'],s['sh'])for key,s in entries)
 symidx=len(out);out.append({'name':'.symtab','hdr':[0,2,0,0,0,len(symblob),symidx+1,sum(s['bind']==0 for key,s in entries),4,16],'payload':symblob})
 out.append({'name':'.strtab','hdr':[0,3,0,0,0,len(names),0,0,1,0],'payload':bytes(names)})
 for owner in keep:
  if owner not in rels:continue
  rr=[]
  for off,info,ad in rels[owner]:
   key,s,add=imported(info>>8);rr.append(struct.pack('<IIi',off,(index[key]<<8)|(info&255),ad+add))
  payload=b''.join(rr);out.append({'name':'.rela'+se[owner]['name'],'hdr':[0,4,0,0,0,len(payload),symidx,mapping[owner],4,12],'payload':payload})
 shidx=len(out);out.append({'name':'.shstrtab','hdr':[0,3,0,0,0,0,0,0,1,0],'payload':b''})
 sn=bytearray(b'\0')
 for s in out:
  s['hdr'][0]=len(sn) if s['name'] else 0
  if s['name']:sn.extend(s['name'].encode()+b'\0')
 out[-1]['payload']=bytes(sn);out[-1]['hdr'][5]=len(sn)
 body=bytearray(b'\0'*52)
 for s in out[1:]:
  align=max(1,s['hdr'][8]);body.extend(b'\0'*((-len(body))%align));s['hdr'][4]=len(body)
  if s['hdr'][1]!=8:body.extend(s['payload'])
 body.extend(b'\0'*((-len(body))%4));shoff=len(body)
 for s in out:body.extend(struct.pack('<10I',*s['hdr']))
 hh=list(h);hh[6]=shoff;hh[12]=len(out);hh[13]=shidx;body[:52]=struct.pack('<16sHHIIIIIHHHHHH',*hh);dest.write_bytes(body)

def inventory(p):
 h,se,sy,rr=elf(p)
 rows=[]
 for i,s in enumerate(se):
  z=s['hdr']
  if not z[2]&2:continue
  symbols=[{k:v for k,v in x.items()if k!='sh'}for x in sy if x['sh']==i]
  reloc=[]
  for off,inf,ad in rr.get(i,[]):
   t=sy[inf>>8];reloc.append({'offset':off,'type':inf&255,'target':t,'addend':ad})
  rows.append({'ordinal':i,'name':s['name'],'attrs':{k:z[j]for k,j in [('type',1),('flags',2),('addr',3),('size',5),('align',8),('entsize',9)]},'payload_sha256':hashlib.sha256(s['payload']).hexdigest(),'symbols':symbols,'relocations':reloc})
 return rows

def audit(original,anchor,residual,candidate):
 oh,os,oy,orr=elf(original);files=[anchor,candidate,residual];parsed=[elf(p)for p in files]
 expected=[17,18,19,21,23,25,27];actual=[]
 # Each output allocated section maps to exactly one numeric native owner, including BSS aliases.
 for p,(h,se,sy,rr)in zip(files,parsed):
  assert h[7]==oh[7],('ELF flags',p)
  for i,s in enumerate(se):
   z=s['hdr']
   if not z[2]&2:continue
   funcs=[x['name']for x in sy if x['sh']==i and x['bind']==1 and x['type']==2]
   if funcs:
    assert len(funcs)==1;orig=next(x for x in oy if x['name']==funcs[0]);oi=orig['sh']
   else:
    aliases=[x for x in sy if x['sh']==i and x['name'] in ALIAS.values()];assert len(aliases)==1
    oi=next(k for k,v in ALIAS.items()if v==aliases[0]['name'])
   actual.append(oi);native=os[oi];assert s['name']==native['name']
   assert all(z[k]==native['hdr'][k]for k in (1,2,3,5,8,9)),('allocated attrs',p,oi)
   assert s['payload']==native['payload'],('bytes',p,oi)
   def symbols(sy,i):return collections.Counter(tuple(x[k]for k in ('name','val','size','type','bind','other'))for x in sy if x['sh']==i and x['type']!=3 and x['name'] not in ALIAS.values())
   assert symbols(sy,i)==symbols(oy,oi),('whole symbol inventory',p,oi)
   def endpoint(t,ad,native):
    if t['name'] in ALIAS.values():return ('DATA',next(k for k,v in ALIAS.items()if v==t['name']),ad)
    if native and t['sh'] in ALIAS:return ('DATA',t['sh'],t['val']+ad)
    if t['name'] in FUNC:return ('FUNCTION',t['name'],ad)
    assert t['sh']==0,(t,ad);return ('EXTERNAL',t['name'],t['type'],t['bind'],t['other'],t['val'],ad)
   old=[(off,inf&255,endpoint(oy[inf>>8],ad,True))for off,inf,ad in orr.get(oi,[])]
   new=[(off,inf&255,endpoint(sy[inf>>8],ad,False))for off,inf,ad in rr.get(i,[])]
   assert old==new,('relocations',p,oi,old,new)
 assert actual==expected,('ordinal conservation/order',actual)
 assert sum(os[i]['hdr'][5]for i in expected if os[i]['hdr'][2]&4)==516
 assert sum(os[i]['hdr'][5]for i in expected if not os[i]['hdr'][2]&4)==172
 # Aliases are zero-size global NOTYPE addresses, not duplicate sized storage.
 ah,ase,asy,ar=parsed[0]
 for native,name in ALIAS.items():
  matches=[x for x in asy if x['name']==name];assert len(matches)==1
  x=matches[0];assert (x['val'],x['size'],x['type'],x['bind'],x['other'])==(0,0,0,1,0)
  assert ase[x['sh']]['hdr'][1:4]==[8,3,0] and ase[x['sh']]['hdr'][5]==({17:44,18:128}[native]),'zero-size alias must resolve to genuine original BSS parent'
 ch,cs,cy,cr=parsed[1];assert not any(s['hdr'][2]&2 and not s['hdr'][2]&4 and s['hdr'][5]for s in cs)
 return {'complete_allocated_ordinal_order':actual,'C_bytes':112,'ASM_code_bytes':404,'ASM_BSS_bytes':172,'new_KEEP':False,'exact_bytes_attrs_symbols_relocations':True,'units':{p.name:inventory(p)for p in files}}


def main():
 import sys
 assert len(sys.argv)==4 and sys.argv[3]in('anchor','residual'),'usage partition_os_exception.py ORIGINAL OUTPUT anchor|residual'
 src=pathlib.Path(sys.argv[1]);dst=pathlib.Path(sys.argv[2]);h,se,sy,rr=elf(src)
 assert [i for i,s in enumerate(se)if s['hdr'][2]&2]==[17,18,19,21,23,25,27],'native exception owner ordinals changed'
 assert [se[i]['hdr'][5]for i in [17,18,19,21,23,25,27]]==[44, 128, 112, 116, 20, 144, 124],'native exception span changed'
 partition(src,dst,[17,18]if sys.argv[3]=='anchor'else[21,23,25,27])
if __name__=='__main__':main()
