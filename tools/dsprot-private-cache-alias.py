#!/usr/bin/env python3
"""Preserve genuine allocated sections, private symbols, and zero-size cache aliases."""
import argparse,collections,hashlib,json,os,pathlib,struct,subprocess
ALIAS={5:'to91_dsprot_clear_cache'}
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
 if 5 in keep:
  for i in [5]:extra[('defalias',i)]={'name':ALIAS[i],'val':0,'size':0,'type':0,'bind':1,'other':0,'sh':mapping[i]}
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

if __name__=='__main__':
 import sys
 assert len(sys.argv)==3,'usage private_alias.py C_PREFIX_INPUT OUTPUT'
 src=pathlib.Path(sys.argv[1]);dst=pathlib.Path(sys.argv[2]);h,se,sy,rr=elf(src)
 keep=[i for i,z in enumerate(se)if z['hdr'][2]&2]
 assert keep==[5,7,8] and [se[i]['hdr'][5]for i in keep]==[32,60,272],'plain prefix section inventory changed'
 cache=[x for x in sy if x['name']=='clearDataAndInstructionCache']
 assert len(cache)==1 and (cache[0]['sh'],cache[0]['val'],cache[0]['size'],cache[0]['bind'],cache[0]['type'])==(5,0,32,0,2),'private cache owner changed'
 assert not any(x['name']in ALIAS.values()for x in sy),'alias already defined'
 partition(src,dst,keep)
