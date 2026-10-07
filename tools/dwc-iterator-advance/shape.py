"""Finite complete MWCC D1 producer inventory; precise source-directory DWARF normalization only."""
import struct,hashlib
H='<16sHHIIIIIHHHHHH';S='<10I';Y='<IIIBBH'
def digest(b):return hashlib.sha256(b).hexdigest()
def parse(b):
 h=list(struct.unpack_from(H,b));assert h[0][:7]==b'\x7fELF\x01\x01\x01' and h[1:3]==[1,40] and h[11]==40
 ss=[list(struct.unpack_from(S,b,h[6]+40*i))for i in range(h[12])]
 get=lambda t,o:t[o:].split(b'\0',1)[0].decode()
 names=b[ss[h[13]][4]:ss[h[13]][4]+ss[h[13]][5]];ns=[get(names,s[0])for s in ss]
 st=next(i for i,s in enumerate(ss)if s[1]==2);t=b[ss[ss[st][6]][4]:ss[ss[st][6]][4]+ss[ss[st][6]][5]]
 yy=[list(struct.unpack_from(Y,b,ss[st][4]+16*i))for i in range(ss[st][5]//16)];yn=[get(t,y[0])for y in yy]
 return h,ss,ns,st,yy,yn

def uleb(b,offset):
 value=0;shift=0
 for _ in range(5):
  byte=b[offset];offset+=1;value|=(byte&127)<<shift
  if not byte&128:return value,offset
  shift+=7
 raise AssertionError("oversized DWARF ULEB")

def comp_directory(info,abbrev,source_directory,basename):
 # Parse the actual single DWARF-v2 compile-unit DIE, not a global string replacement.
 length,version,abbrevoffset,address_size=struct.unpack_from('<IHIB',info)
 assert version==2 and abbrevoffset==0 and address_size==4 and length==len(info)-4
 code,offset=uleb(info,11);cursor=0;found=None
 while cursor<len(abbrev):
  number,cursor=uleb(abbrev,cursor)
  if number==0:break
  tag,cursor=uleb(abbrev,cursor);children=abbrev[cursor];cursor+=1;attributes=[]
  while True:
   attr,cursor=uleb(abbrev,cursor);form,cursor=uleb(abbrev,cursor)
   if attr==form==0:break
   assert attr and form;attributes.append((attr,form))
  if number==code:assert found is None;found=(tag,children,attributes)
 expected=[(0x13,0x0f),(0x11,0x10),(0x12,0x10),(0x10,0x06),(0x43,0x06),(0x03,0x08),(0x25,0x08),(0x1b,0x08)]
 assert found==(0x11,1,expected), "unexpected compile-unit attribute/form contract"
 bounds=None
 for attr,form in expected:
  start=offset
  if form==0x0f:
   value,offset=uleb(info,offset);assert attr==0x13 and value==4
  elif form in(0x10,0x06):offset+=4
  else:
   offset=info.index(b'\0',offset)+1;value=info[start:offset]
   if attr==0x03:assert value==basename.encode()+b'\0'
   elif attr==0x25:assert value==b'Metrowerks C/C++ for ARM\0'
   elif attr==0x1b:assert value==source_directory;bounds=(start,offset)
 assert bounds is not None and offset==bounds[1]
 return bounds

def sibling_fields(info,abbrev):
 # Parse complete DIEs to identify only real DW_AT_sibling DW_FORM_ref_addr fields.
 table={};cursor=0
 while cursor<len(abbrev):
  number,cursor=uleb(abbrev,cursor)
  if number==0:break
  tag,cursor=uleb(abbrev,cursor);children=abbrev[cursor];cursor+=1;attrs=[]
  while True:
   attr,cursor=uleb(abbrev,cursor);form,cursor=uleb(abbrev,cursor)
   if attr==form==0:break
   attrs.append((attr,form))
  assert number not in table;table[number]=attrs
 cursor=11;siblings=[];die_positions=set()
 while cursor<len(info):
  die_positions.add(cursor);number,cursor=uleb(info,cursor)
  if number==0:continue
  assert number in table
  for attr,form in table[number]:
   start=cursor
   if form in(0x06,0x10):cursor+=4
   elif form==0x05:cursor+=2
   elif form in(0x0b,0x0c):cursor+=1
   elif form==0x08:cursor=info.index(b'\0',cursor)+1
   elif form==0x0f:_,cursor=uleb(info,cursor)
   elif form==0x03:
    length=struct.unpack_from('<H',info,cursor)[0];cursor+=2+length
   else:raise AssertionError(('unobserved DWARF form',form))
   assert cursor<=len(info)
   if attr==0x01:assert form==0x10;siblings.append(start)
 assert cursor==len(info)
 return siblings,die_positions

def inventory(b,source,nitro_directory=None):
 h,ss,ns,st,yy,yn=parse(b)
 # The observed normal MWCC container uses16-byte payload boundaries, including empty sections.
 previous=52
 for i,section in enumerate(ss):
  if i==0:assert section==[0]*10;continue
  offset=(previous+15)&~15
  assert section[4]==offset and not any(b[previous:offset]), ('physical section layout drift',i)
  assert section[1]!=8 and offset+section[5]<=h[6]
  previous=offset+section[5]
 assert h[6]==(previous+15)&~15 and not any(b[previous:h[6]])
 assert h[6]+40*h[12]==len(b), 'unowned trailing/container bytes'
 payload={i:b[s[4]:s[4]+s[5]]for i,s in enumerate(ss)}
 line=next(i for i,n in enumerate(ns)if n=='.debug_line'and ss[i][5]);raw=payload[line]
 directory=('Z:'+str(source.resolve().parent).replace('/','\\')+'\\').encode()+b'\0';assert raw.count(directory)==1
 at=raw.index(directory);end=at+len(directory);replacement=b'@ITERATOR_SOURCE_DIRECTORY@\0';delta=len(directory)-len(replacement)
 nitro_directory=nitro_directory or(source.resolve().parents[3]/'lib/include/nitro')
 nitro=('Z:'+str(nitro_directory.resolve()).replace('/','\\')+'\\').encode()+b'\0'
 assert raw.count(nitro)==1 and raw.index(nitro)==end
 nitro_end=end+len(nitro);nitro_replacement=b'@ITERATOR_NITRO_DIRECTORY@\0'
 nitro_delta=len(nitro)-len(nitro_replacement)
 assert raw[10:at]==bytes.fromhex('0201ff040c0001010101000000010000')
 assert raw[nitro_end:10+struct.unpack_from('<I',raw,6)[0]]==b'(\0\0tree-iterator4.h\0\x01\0\0code32.h\0\x02\0\0types.h\0\x02\0\0command-line defines)\0\x03\0\0to91_ov45_iterator_advance108.cpp\0\x01\0\0\0'
 line_delta=delta+nitro_delta
 # Source-directory bytes reside entirely in the DWARF v2 header, never in the line program.
 total,version,header=struct.unpack_from('<IHI',raw);assert version==2 and total==len(raw)-4 and 10<=at<end<=10+header
 canonical=bytearray(raw[:at]+replacement+nitro_replacement+raw[nitro_end:]);struct.pack_into('<I',canonical,0,total-line_delta);struct.pack_into('<I',canonical,6,header-line_delta);payload[line]=bytes(canonical)
 info=next(i for i,n in enumerate(ns)if n=='.debug_info');abbrev=next(i for i,n in enumerate(ns)if n=='.debug_abbrev')
 assert not ss[line][2]&2 and not ss[info][2]&2 and not ss[abbrev][2]&2
 info_raw=payload[info];info_at,info_end=comp_directory(info_raw,payload[abbrev],directory,source.name)
 info_delta=info_end-info_at-len(replacement);assert info_delta==delta
 info_canonical=bytearray(info_raw[:info_at]+replacement+info_raw[info_end:]);cu_length=struct.unpack_from('<I',info_raw)[0]
 struct.pack_into('<I',info_canonical,0,cu_length-info_delta)
 siblings,die_positions=sibling_fields(info_raw,payload[abbrev]);sibling_adjustments={}
 info_rel=next(i for i,q in enumerate(ss)if q[1]==4 and q[7]==info)
 relocation_rows={struct.unpack_from('<IIi',payload[info_rel],pos)[0]:struct.unpack_from('<IIi',payload[info_rel],pos)for pos in range(0,len(payload[info_rel]),12)}
 for field in siblings:
  offset,relocinfo,addend=relocation_rows[field];target=yy[relocinfo>>8]
  assert relocinfo&255==2 and target[1:]==[0,0,3,0,info], 'sibling must target local debug_info section'
  value=struct.unpack_from('<I',info_raw,field)[0]
  assert value==addend and value in die_positions and field>=info_end and value>=info_end
  struct.pack_into('<I',info_canonical,field-info_delta,value-info_delta)
  sibling_adjustments[field]=info_delta
 payload[info]=bytes(info_canonical)
 pub=next(i for i,n in enumerate(ns)if n=='.debug_pubnames');assert not ss[pub][2]&2
 pub_raw=payload[pub];assert len(pub_raw)==18 and struct.unpack('<IHIII',pub_raw)==(14,2,0,cu_length,0)
 pub_canonical=bytearray(pub_raw);struct.pack_into('<I',pub_canonical,10,cu_length-info_delta);payload[pub]=bytes(pub_canonical)
 normalizations={line:(nitro_end,line_delta),info:(info_end,info_delta)}

 sections=[]
 for i,s in enumerate(ss):
  row=s.copy();row[4]=0 # Physical container offsets carry no runtime ownership.
  if i in normalizations:row[5]-=normalizations[i][1]
  if s[1]in(4,9)and s[7]in normalizations:
   limit,shrink=normalizations[s[7]]
   v=bytearray(payload[i]);entry=12 if s[1]==4 else 8
   for p in range(0,len(v),entry):
    off=struct.unpack_from('<I',v,p)[0]
    assert not (s[7]==info and info_at<=off<info_end), 'relocation inside comp_dir'
    if off>=limit:struct.pack_into('<I',v,p,off-shrink)
    if s[7]==info and off in sibling_adjustments:
     assert entry==12
     addend=struct.unpack_from('<i',v,p+8)[0];struct.pack_into('<i',v,p+8,addend-sibling_adjustments[off])
   payload[i]=bytes(v)
  # Structural symtab is compared through every decoded record below, with precise DWARF offset correction.
  sections.append({'index':i,'name':ns[i],'record':row,'payload_sha256':None if i==st else digest(payload[i])})
 symbols=[]
 for i,(y,n)in enumerate(zip(yy,yn)):
  row=y.copy()
  if y[5]in normalizations:
   limit,shrink=normalizations[y[5]]
   assert not (y[5]==info and info_at<=y[1]<info_end), 'symbol inside comp_dir'
   if y[1]>=limit:row[1]-=shrink
  symbols.append({'index':i,'name':n,'record':row})
 header=h.copy();header[0]=header[0].hex();header[6]=0 # Rebuilt object offsets are structural, not source payload.
 return {'header':header,'sections':sections,'symbols':symbols,'normalization':'Only the declared source and physical Nitro header directories in the exact nonallocated DWARF-v2 line-header/file-table contract, and actual source compile-unit DW_AT_comp_dir: exact directory strings, header/CU/pubnames lengths and affected local symbol/relocation offsets plus parser-identified sibling references to later DIEs in the same CU. All other bytes and all runtime payloads/names/bindings remain fixed.','source_basename':source.name}
