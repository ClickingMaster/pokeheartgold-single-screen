#include <nitro/types.h>
#include <nitro/code32.h>
#include <size_t.h>
inline void *operator new(std::size_t,void *p){return p;}
struct Str;
extern "C" Str *ov45_02237A24(Str *,const Str *);
extern "C" void ov45_02237BC8(Str *);
extern "C" void ov45_02237D48(Str *,const Str *);
extern "C" int ov45_02237EB0(const Str *,const Str *,int);
extern "C" void ov45_02236680(Str *,const Str *,int,int);
struct Str {
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];} raw;
 Str(){}
 Str(const Str &o){ov45_02237A24(this,&o);}
 u32 size() const {return !raw.l.flag ? raw.s.size : raw.l.size;}
 void Destroy(){ov45_02237BC8(this);}
};
class Alloc {};
struct Vec;
extern "C" void ov45_02237634(Vec *,const Str &,Alloc);
struct Vec {
 Str *data;u32 count,cap;
 Alloc get_alloc() const {return Alloc();}
 Vec():data(0),count(0),cap(0){}
 void push_back(const Str &x){
  Alloc a=get_alloc();(void)a;
  if(count<cap){
   new(data+count) Str(x);
   count++;
  }else{
   ov45_02237634(this,x,Alloc());
  }
 }
};
extern "C" Vec ov45_0223649C(const Str &s,const Str &delim,int max);
#pragma force_active on
// @fn ov45_0223649C
extern "C" Vec ov45_0223649C(const Str &s,const Str &delim,int max){
 Vec out;
 int n=1;
 Str rest(s);
 int pos;
 while(n!=max&&(pos=ov45_02237EB0(&rest,&delim,0))!=-1){
  if(pos!=0){
   Str piece;
   ov45_02236680(&piece,&rest,0,pos);
   out.push_back(piece);
   piece.Destroy();
   n++;
  }
  Str tmp;
  ov45_02236680(&tmp,&rest,pos+delim.size(),-1);
  ov45_02237D48(&rest,&tmp);
  tmp.Destroy();
 }
 if(rest.size()!=0)out.push_back(rest);
 rest.Destroy();
 return out;
}
