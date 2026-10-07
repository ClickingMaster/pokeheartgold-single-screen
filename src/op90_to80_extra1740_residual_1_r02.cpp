#include <nitro/types.h>
#include <nitro/code32.h>
struct Elem {u32 a,b;};
struct It {
 Elem *p;
 It(Elem *q):p(q){}
};
inline int Distance(It first,It last){return last.p-first.p;}
struct DIter {
 Elem **map;Elem **begin;u32 unk8;Elem **end;Elem *cur;
 DIter(const DIter &o):map(o.map),begin(o.begin),unk8(o.unk8),end(o.end),cur(o.cur){}
};
class Tag {};
struct Deque {u32 unk0;u32 size;};
template<class T> inline const T &Min(const T &a,const T &b){return a<b?a:b;}
extern "C" DIter ov45_0223B250(Deque *,int);
extern "C" DIter ov45_02239090(It,It,DIter);
extern "C" void ov45_02238E60(Deque *,It *,const It &,Tag);
extern "C" void ov45_0223B764(Deque *,u32,Tag);
extern "C" void ov45_02238D54(Deque *,It,const It &,Tag);
#pragma force_active on
// @fn ov45_02238D54
extern "C" void ov45_02238D54(Deque *d,It first,const It &last,Tag){
 u32 n=Distance(first,last);
 u32 m=Min(d->size,n);
 It mid=first;
 mid.p+=m;
 ov45_02239090(first,mid,ov45_0223B250(d,0));
 n-=m;
 if(n!=0){
  Tag t;
  ov45_02238E60(d,&mid,last,t);
 }else{
  u32 size=d->size;
  if(m<size)ov45_0223B764(d,size-m,Tag());
 }
}
