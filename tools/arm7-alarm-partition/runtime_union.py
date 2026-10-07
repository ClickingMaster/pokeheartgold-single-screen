"""Finite original alarm runtime/storage union with explicit natural producer transitions."""
from pathlib import Path
import sys,collections,struct
sys.dont_write_bytecode=True
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P));import partition
LIVE=partition.EXPECTED;HANDLER='OSi_AlarmHandler'
def inspect(b):
 h,ss,ns,st,yy,yn=partition.parse(b)
 f={n:(i,y)for i,(n,y)in enumerate(zip(yn,yy))if y[3]>>4==1 and y[3]&15==2 and y[5] and y[5]<len(ss)}
 return h,ss,ns,yy,yn,f

def reconcile(original,prefix,suffix,handler):
 ob=Path(original).read_bytes();oh,os,on,oy,oyn,of=inspect(ob)
 assert set(of)==set(LIVE+[HANDLER]);assert oh[7]==0x2100000
 bss=next(i for i,s in enumerate(os)if s[1]==8 and on[i]=='.bss');assert os[bss][5]==12
 def symcoord(y,n,add):
  if y[5]==0:
   if n in of:return ('CODE',n,add)
   if n in('OSi_UseAlarm','OSi_AlarmQueue'):return ('DATA',0 if n=='OSi_UseAlarm' else 4,add)
   return ('UND',n,add)
  if y[5]==bss:return ('DATA',y[1],add)
  found=[fn for fn,(_,z)in of.items()if z[5]==y[5]]
  assert len(found)==1;return ('CODE',found[0],y[1]+add)
 def relocs(b,ss,yy,yn,sec,coordinate):
  out=[]
  for rs in ss:
   if rs[1]!=4 or rs[7]!=sec:continue
   for off in range(rs[4],rs[4]+rs[5],12):
    at,info,add=struct.unpack_from('<IIi',b,off);j=info>>8;out.append((at,info&255,coordinate(yy[j],yn[j],add)))
  return sorted(out)
 owned=set();data=0;reports=[]
 for role,path in [('prefix',prefix),('suffix',suffix),('handler',handler)]:
  cb=Path(path).read_bytes();ch,cs,cn,cy,cyn,cf=inspect(cb)
  assert ch[7]==(0 if role=='handler'else 0x2100000)
  assert set(cf)==({HANDLER}if role=='handler'else({LIVE[-1]}if role=='suffix'else set(LIVE[:-1])))
  localowners={y[5]:n for n,(_,y)in cf.items()}
  allocated=[i for i,s in enumerate(cs)if s[2]&2]
  assert set(allocated)==set(localowners)|({next(i for i,s in enumerate(cs)if s[1]==8)}if role=='prefix'else set())
  def cc(y,n,add):
   if y[5]==0:
    if n in of:return ('CODE',n,add)
    if n in('OSi_UseAlarm','OSi_AlarmQueue'):return ('DATA',0 if n=='OSi_UseAlarm'else 4,add)
    return ('UND',n,add)
   if y[5] in localowners:return ('CODE',localowners[y[5]],y[1]+add)
   assert role=='prefix'and cn[y[5]]=='.bss';return ('DATA',y[1],add)
  for n,(j,y)in cf.items():
   assert n not in owned;owned.add(n);_,old=of[n];a=os[old[5]];z=cs[y[5]]
   assert all(a[k]==z[k]for k in(1,3,5,8,9)),(n,'natural section nonflag metadata')
   assert z[2]==a[2],(n,'finite natural C KEEP flags')
   assert y[1:5]==old[1:5],(n,'raw function definition')
   oldbody=ob[a[4]:a[4]+a[5]];newbody=cb[z[4]:z[4]+z[5]]
   if role=='handler':
    assert oldbody[4:8]==bytes.fromhex('000000eb') and newbody[4:8]==bytes.fromhex('feffffeb'),'one finite genuine MWCC/MWAS BL placeholder pair'
    assert oldbody[:4]+oldbody[8:]==newbody[:4]+newbody[8:]
    assert relocs(cb,cs,cy,cyn,y[5],cc)==[(4,1,('CODE','OSi_ArrangeTimer',-8))],'exact BL relocation type/target/addend'
   else:assert newbody==oldbody,(n,'exact original body')
   assert relocs(cb,cs,cy,cyn,y[5],cc)==relocs(ob,os,oy,oyn,old[5],symcoord),(n,'cross-object actual endpoint')
   oldlocal=collections.Counter((name,*v[1:5])for name,v in zip(oyn,oy)if v[5]==old[5]);newlocal=collections.Counter((name,*v[1:5])for name,v in zip(cyn,cy)if v[5]==y[5])
   if role=='handler':
    extra=('.text',0,16,3,0);assert newlocal[extra]==1;newlocal.subtract({extra:1});newlocal=+newlocal
   assert oldlocal==newlocal,(n,'complete local/global runtime multiset')
  if role=='prefix':
   i=next(i for i,s in enumerate(cs)if s[1]==8);s=cs[i];assert s[1:4]==[8,3,0] and s[5:]==[12,0,0,4,0]
   assert collections.Counter((n,*y[1:5])for n,y in zip(cyn,cy)if y[5]==i)==collections.Counter((n,*y[1:5])for n,y in zip(oyn,oy)if y[5]==bss)
   data+=12
  reports.append({'role':role,'functions':sorted(cf),'natural_header_flags':ch[7]})
 assert owned==set(of)and data==12
 return {'status':'PASS_FINITE_ORIGINAL_RUNTIME_UNION','functions':10,'raw_text':1164,'mapped_C':1148,'original_BSS_once':12,'dead_periodic':0,'retained_handler_ASM':16,'natural_transitions':{'live_C_flags':'6 unchanged','prefix_BSS_flags':'3 unchanged','handler_ELF_flags':'0x02100000 -> 0','handler_extra_local_SECTION':['.text',0,16,3,0],'handler_exact_PC24_placeholder_pair':['offset4','eb000000','ebfffffe','BL_AL','OSi_ArrangeTimer',-8]},'parts':reports}
