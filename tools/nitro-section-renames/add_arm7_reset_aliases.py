"""Add only the original two LOCAL sized reset flag aliases to the residual BSS."""
import pathlib,struct,sys

def add(source,output):
 b=pathlib.Path(source).read_bytes();assert b[:7]==b'\x7fELF\x01\x01\x01';off=struct.unpack_from('<I',b,32)[0];sz,n,st=struct.unpack_from('<HHH',b,46);assert sz==40
 S=[list(struct.unpack_from('<10I',b,off+i*sz))for i in range(n)]
 def text(section,pos):return b[section[4]+pos:b.index(b'\0',section[4]+pos)].decode()
 names=[text(S[st],s[0])for s in S];tables=[i for i,s in enumerate(S)if s[1]==2];assert len(tables)==1;t=tables[0];sym=S[t];strings=sym[6];assert sym[9]==16
 Y=[struct.unpack_from('<IIIBBH',b,p)for p in range(sym[4],sym[4]+sym[5],16)];assert Y[0]==(0,0,0,0,0,0)
 named=[(text(S[strings],x[0]),x)for x in Y];z=[x for name,x in named if name=='OSi_ResetFlags'];assert len(z)==1
 owner=z[0];sec=owner[-1];assert owner[1:5]==(0,4,17,0)and names[sec]=='.bss'and S[sec][1:4]==[8,3,0]and S[sec][5]==4 and S[sec][8]==2
 assert not any(name in ('OSi_IsInitReset','OSi_IsResetOccurred')for name,x in named)
 boundary=sym[7];assert all(x[3]>>4==0 for x in Y[:boundary])and all(x[3]>>4!=0 for x in Y[boundary:])
 payload={i:b[s[4]:s[4]+s[5]]for i,s in enumerate(S)if s[1]!=8}
 strs=bytearray(payload[strings]);aliases=[]
 for name,val in [('OSi_IsInitReset',0),('OSi_IsResetOccurred',2)]:
  index=len(strs);strs.extend(name.encode()+b'\0');aliases.append((index,val,2,1,0,sec))
 Y=[(x[0],x[1],0,16,x[4],x[5])if text(S[strings],x[0])=='OSi_ResetFlags'else x for x in Y]
 payload[strings]=bytes(strs);payload[t]=b''.join(struct.pack('<IIIBBH',*x)for x in Y[:boundary]+aliases+Y[boundary:]);S[t][7]+=2
 for i,s in enumerate(S):
  if s[1]not in(4,9):continue
  assert s[6]==t and s[9]in(8,12)
  p=bytearray(payload[i])
  for at in range(0,len(p),s[9]):
   info=struct.unpack_from('<I',p,at+4)[0];idx=info>>8;assert idx<len(Y);idx+=2 if idx>=boundary else 0;struct.pack_into('<I',p,at+4,idx<<8|info&255)
  payload[i]=bytes(p)
 out=bytearray(b[:52]);assert struct.unpack_from('<I',b,28)[0]==0
 for i,s in enumerate(S):
  if i==0:continue
  alignment=max(s[8],1);out.extend(bytes((-len(out))%alignment));s[4]=len(out)
  if s[1]!=8:s[5]=len(payload[i]);out.extend(payload[i])
 out.extend(bytes((-len(out))%4));struct.pack_into('<I',out,32,len(out));out.extend(b''.join(struct.pack('<10I',*s)for s in S))
 output=pathlib.Path(output);assert not output.exists();output.write_bytes(out)
if __name__=='__main__':add(*sys.argv[1:])
