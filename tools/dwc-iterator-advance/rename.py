from pathlib import Path
import struct,hashlib
import shape
CPP='_ZN8Iterator7AdvanceEv';NATIVE='ov45_02236E58'
def rename(raw,source):
 h,se,names,st,sy,yn=shape.parse(raw)
 assert [(names[i],s[5]) for i,s in enumerate(se) if s[2]&2 and s[5]]==[('.text',108)]
 definitions=[(i,n,y)for i,(n,y)in enumerate(zip(yn,sy))if y[5]and se[y[5]][2]&2 and(y[3]>>4)==1]
 assert len(definitions)==1
 index,name,symbol=definitions[0];assert name==CPP and symbol[1:5]==[0,108,18,0]
 text=se[symbol[5]];payload=raw[text[4]:text[4]+text[5]]
 assert hashlib.sha256(payload).hexdigest()==NATIVE_PAYLOAD_SHA256
 string_index=se[st][6];string_section=se[string_index];old=raw[string_section[4]:string_section[4]+string_section[5]]
 new=old+NATIVE.encode()+b'\0';out=bytearray(raw)
 while len(out)%4:out.append(0)
 new_offset=len(out);out.extend(new)
 struct.pack_into('<II',out,h[6]+string_index*40+16,new_offset,len(new))
 struct.pack_into('<I',out,se[st][4]+index*16,len(old))
 ah,ase,an,ast,asy,ayn=shape.parse(out)
 # Every section payload/attribute remains literal, except the symbol table's one st_name and relocated string table.
 assert h==ah and se[st][4]==ase[ast][4]
 for i,(before,after)in enumerate(zip(se,ase)):
  if i==string_index:assert before[:4]==after[:4] and before[6:]==after[6:];continue
  assert before==after
  a=raw[before[4]:before[4]+before[5]];b=out[after[4]:after[4]+after[5]]
  if i==st:
   expected=bytearray(a);struct.pack_into('<I',expected,index*16,len(old));assert bytes(expected)==bytes(b)
  else:assert a==b
 assert [(n,y[1:])for n,y in zip(ayn,asy)]==[(NATIVE if n==CPP else n,y[1:])for n,y in zip(yn,sy)]
 assert out[string_section[4]:string_section[4]+string_section[5]]==old
 return bytes(out),{'from':CPP,'to':NATIVE,'bytes':108,'raw_payload_sha256':hashlib.sha256(payload).hexdigest(),'projection':'ARM nonvirtual void member this r0 = canonical C Iterator* argument r0; no hidden arguments, returns ignored; metadata-only global symbol binding','no_code_data_relocation_change':True}
NATIVE_PAYLOAD_SHA256='0b4364fd6c6c68c3069d669b878bbcc15bc98d52b2b71947547a90b69c3fb710'
