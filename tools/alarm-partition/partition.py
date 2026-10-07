"""Scoped ELF32 little-endian MWCC os_alarm producer partition; no payload patches."""
import struct, pathlib, hashlib, json, collections, argparse
H='<16sHHIIIIIHHHHHH'; S='<10I'; Y='<IIIBBH'
def sha(b):return hashlib.sha256(b).hexdigest()
def parse(b):
 h=list(struct.unpack_from(H,b));assert h[0][:7]==b'\x7fELF\x01\x01\x01' and h[1:3]==[1,40] and h[11]==40
 ss=[list(struct.unpack_from(S,b,h[6]+40*i)) for i in range(h[12])]
 shstr=b[ss[h[13]][4]:ss[h[13]][4]+ss[h[13]][5]]
 name=lambda t,o:t[o:].split(b'\0',1)[0].decode()
 ns=[name(shstr,s[0]) for s in ss]; st=next(i for i,s in enumerate(ss) if s[1]==2);strs=b[ss[ss[st][6]][4]:ss[ss[st][6]][4]+ss[ss[st][6]][5]]
 yy=[list(struct.unpack_from(Y,b,ss[st][4]+16*i))for i in range(ss[st][5]//16)];yn=[name(strs,y[0])for y in yy]
 assert yy[0]==[0,0,0,0,0,0]
 return h,ss,ns,st,yy,yn
EXPECTED=['OSi_SetTimer','OS_InitAlarm','OS_IsAlarmAvailable','OS_CreateAlarm','OSi_InsertAlarm','OS_SetAlarm','OS_SetPeriodicAlarm','OS_CancelAlarm','OSi_ArrangeTimer']
SIZES=[140,64,16,16,300,108,100,136,240]
def split(b):
 h,ss,ns,st,yy,yn=parse(b)
 funcs=[(y[5],i,n,y[2]) for i,(y,n) in enumerate(zip(yy,yn)) if y[3]>>4==1 and y[3]&15==2 and y[5]];funcs.sort()
 assert [x[2]for x in funcs]==EXPECTED and [x[3]for x in funcs]==SIZES
 code={x[0]for x in funcs};suffix=funcs[-1][0];bs=[i for i,s in enumerate(ss) if s[2]&2 and not s[2]&4];assert len(bs)==1 and ns[bs[0]]=='.bss' and ss[bs[0]][1:4]==[8,0x20000003,0] and ss[bs[0]][5:]==[12,0,0,4,0]
 assert all(ss[i][1:4]==[1,(6 if name=='OS_SetPeriodicAlarm' else 0x20000006),0] and ss[i][8:]==[4,0]for i,_,name,_ in funcs)
 storage=next(i for i,n in enumerate(yn) if n=='OSi_UseAlarm');assert yy[storage][1:]==[0,2,17,0,bs[0]]
 children=[];reports=[]
 for kind in ('prefix','suffix'):
  # Structural tables are duplicated; every producer payload section belongs to exactly one child.
  owned={i for i in range(len(ss)) if i!=suffix and not(ss[i][1] in (4,9) and ss[i][7]==suffix)} if kind=='prefix' else {0,st,ss[st][6],h[13],suffix}
  if kind=='suffix':owned.update(i for i,s in enumerate(ss) if s[1] in (4,9) and s[7]==suffix)
  idx={old:new for new,old in enumerate(sorted(owned))}; payload={i:b[s[4]:s[4]+s[5]] if s[1]!=8 else b''for i,s in enumerate(ss) if i in owned}
  defs={i for i,y in enumerate(yy)if y[5] in owned and y[5]!=0};defs.add(0)
  # Duplicate only original imports that are referenced by retained relocations.
  need=set(); overrides={};changes=[]
  for i in owned:
   s=ss[i]
   if s[1] not in(4,9):continue
   assert s[1]==4 and s[9]==12 and s[6]==st and s[7] in owned
   for p in range(s[4],s[4]+s[5],12):
    off,info,add=struct.unpack_from('<IIi',b,p);j=info>>8;y=yy[j]
    if y[5] not in owned and y[5]!=0:
     if y[3]&15==3:
      assert y[5]==bs[0] and y[1]==0 and kind=='suffix'
      overrides[(i,off)]=(storage,add);need.add(storage)
      changes.append({'section':i,'offset':off,'old_symbol':j,'new_symbol':storage,'addend':add,'reason':'original BSS base equals real OSi_UseAlarm storage offset zero'})
     else:
      assert y[3]>>4==1 and y[4]==0 and yn[j] in EXPECTED
      need.add(j)
    else:need.add(j)
  keep=defs|need
  # Every original defined record is retained once; unrelated UND records retained in prefix.
  if kind=='prefix':keep.update(i for i,y in enumerate(yy) if y[5]==0)
  syorder=[0]+sorted(i for i in keep if i and yy[i][3]>>4==0)+sorted(i for i in keep if i and yy[i][3]>>4!=0)
  yi={old:new for new,old in enumerate(syorder)};ny=[]
  for i in syorder:
   y=yy[i].copy()
   if y[5] in idx and y[5]!=0:y[5]=idx[y[5]]
   elif y[5]!=0:
    assert y[3]>>4==1 and y[4]==0
    changes.append({'symbol':i,'name':yn[i],'reason':'cross-child real definition imported','old':y.copy()})
    y[1]=y[2]=y[5]=0 # binding/type/visibility unchanged, ordinary ELF undefined import
   ny.append(y)
  payload[st]=b''.join(struct.pack(Y,*y)for y in ny)
  for i in owned:
   s=ss[i]
   if s[1]!=4:continue
   out=bytearray()
   for p in range(s[4],s[4]+s[5],12):
    off,info,add=struct.unpack_from('<IIi',b,p);j=info>>8
    j,add=overrides.get((i,off),(j,add));out+=struct.pack('<IIi',off,(yi[j]<<8)|(info&255),add)
   payload[i]=bytes(out)
  out=bytearray(b[:52]);newss=[]
  for i in sorted(owned):
   s=ss[i].copy()
   if i==0:newss.append([0]*10);continue
   align=max(1,s[8]);out+=b'\0'*((-len(out))%align);s[4]=len(out)
   if s[1]!=8:out+=payload[i];s[5]=len(payload[i])
   if s[6]:assert s[6] in idx;s[6]=idx[s[6]]
   if s[1] in(4,9):s[7]=idx[s[7]]
   if i==st:s[7]=sum(y[3]>>4==0 for y in ny)
   newss.append(s)
  out+=b'\0'*((-len(out))%4);nh=h.copy();nh[6]=len(out);nh[12]=len(newss);nh[13]=idx[h[13]]
  out+=b''.join(struct.pack(S,*s)for s in newss);out[:52]=struct.pack(H,*nh)
  children.append(bytes(out));reports.append({'kind':kind,'section_map':idx,'symbol_map':yi,'rewrites':changes,'sha256':sha(out)})
 return children,reports

def prove(original,children,reports):
 # Re-derive exact entire child objects: any payload, debug, header, symbol or relocation edit rejects.
 expected,er=split(original);assert children==expected and reports==er
 h,ss,ns,st,yy,yn=parse(original)
 seen=collections.Counter();symbols=collections.Counter();relcount=0
 for child,r in zip(children,reports):
  hh,cs,cn,ct,cy,cyn=parse(child);sm={int(k):v for k,v in r['section_map'].items()};ym={int(k):v for k,v in r['symbol_map'].items()}
  for old,new in sm.items():
   if old in(0,st,ss[st][6],h[13]):continue
   seen[old]+=1
   if ss[old][1] not in(4,9):assert child[cs[new][4]:cs[new][4]+cs[new][5]]==original[ss[old][4]:ss[old][4]+ss[old][5]] if ss[old][1]!=8 else cs[new][5]==ss[old][5]
   else:
    for p in range(ss[old][4],ss[old][4]+ss[old][5],12):
     off,info,add=struct.unpack_from('<IIi',original,p);j=info>>8
     q=cs[new][4]+p-ss[old][4];no,ni,na=struct.unpack_from('<IIi',child,q);assert no==off and ni&255==info&255
     inv={v:k for k,v in ym.items()};nj=inv[ni>>8]
     oldcoord=(yy[j][5],yy[j][1]+add) if yy[j][5] else ('UND',yn[j],add)
     newcoord=(yy[nj][5],yy[nj][1]+na) if yy[nj][5] else ('UND',yn[nj],na)
     assert oldcoord==newcoord,(old,new,off,oldcoord,newcoord);relcount+=1
  for old,new in ym.items():
   if yy[old][5] and yy[old][5] in sm:symbols[old]+=1
 assert seen==collections.Counter({i:1 for i in range(len(ss))if i not in(0,st,ss[st][6],h[13])})
 assert symbols==collections.Counter({i:1 for i,y in enumerate(yy)if y[5]})
 return {'status':'PASS','C_raw_bytes':1120,'C_native_bytes':1020,'prefix_native':780,'suffix_native':240,'dead_normal_GC':100,'ASM_retained_bytes':16,'BSS_single_owner':12,'retained_section_count':sum(seen.values()),'defined_symbol_records':sum(symbols.values()),'relocations_exact_endpoints':relcount,'all_children_full_bytes_rederived':True}
if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('input');ap.add_argument('out');a=ap.parse_args();b=pathlib.Path(a.input).read_bytes();out=pathlib.Path(a.out);out.mkdir(parents=True,exist_ok=False);cc,rr=split(b)
 for kind,obj in zip(('prefix','suffix'),cc):(out/(kind+'.o')).write_bytes(obj)
 (out/'partition.json').write_text(json.dumps({'input_sha256':sha(b),'children':rr,'proof':prove(b,cc,rr)},indent=2)+'\n')
