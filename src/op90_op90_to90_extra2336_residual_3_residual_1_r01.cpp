#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" void *memcpy(void *,const void *,unsigned);
extern "C" void *memmove(void *,const void *,unsigned);
extern "C" int printf(const char *,...);
extern "C" void abort(void);
extern "C" void *ov45_02242394(u32);
extern "C" void ov00_021EC2EC(int,void *,int);
extern "C" const char ov45_02255144[],ov45_0225517C[];
extern "C" const signed char _02254F58[];
class Tag {};
struct Str;
extern "C" Str *ov45_02237BC8(Str *);
extern "C" Str *ov45_022384BC(Str *,const char *,const char *,int);
struct Str {
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];} raw;
 Str(){u32 i;u32 *p=raw.words;for(i=0;i<3;i++)p[i]=0;}
 void Destroy(){ov45_02237BC8(this);}
 const char *CStr() const {const Str *t=this;return !t->raw.l.flag?t->raw.s.data:t->raw.l.data;}
};
inline const u32 &Min(const u32 &a,const u32 &b){return b<a?b:a;}
extern "C" Str *ov45_02238104(Str *s,u32 pos,u32 n1,const char *first,const char *last,Tag);

#pragma opt_common_subs off
// @fn ov45_02238104
extern "C" Str *ov45_02238104(Str *s,u32 pos,u32 n1,const char *first,const char *last,Tag){
 bool isLong=s->raw.l.flag;
 u32 cap;char *data;u32 size;
 if(!isLong){cap=11;data=s->raw.s.data;size=s->raw.s.size;}
 else{cap=s->raw.l.capacity;size=s->raw.l.size;data=s->raw.l.data;}
 if(pos>size){printf(ov45_02255144);abort();}
 u32 rem=size-pos;
 u32 n=Min(n1,rem);
 u32 n2=last-first;
 u32 len;
 if(n2>0x7ffffffe ||(len=size-n)>0x7ffffffe - n2){printf(ov45_0225517C);abort();}
 u32 newSize=n2+len;
 u32 tail=size-(pos+n);
 if(newSize>=cap){
  u32 newCap=(cap+15)&~15;
  while(newCap<newSize+1)newCap=(newCap*2+15)&~15;
  char *p=(char *)ov45_02242394(newCap);
  if(pos)memcpy(p,data,pos);
  char *q=p+pos;
  memmove(q,first,last-first);
  if(tail)memcpy(q+n2,data+pos+n,tail);
  p[newSize]=_02254F58[3];
  if(isLong){if(data)ov00_021EC2EC(0,data,0);}
  else s->raw.l.flag=1;
  s->raw.l.data=p;
  s->raw.l.size=newSize;
  s->raw.l.capacity=newCap;
 }else{
  Str tmp;
  if(tail&&data+pos+n2<last&&last<=data+size){
   ov45_022384BC(&tmp,first,last,0);
   first=tmp.CStr();
  }
  if(tail)memmove(data+pos+n2,data+pos+n,tail);
  memmove(data+pos,first,n2);
  data[newSize]=_02254F58[1];
  if(isLong)s->raw.l.size=newSize;
  else s->raw.s.size=newSize;
  tmp.Destroy();
 }
 return s;
}
#pragma opt_common_subs reset
