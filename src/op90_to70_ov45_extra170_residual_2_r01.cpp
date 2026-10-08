#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" void *memset(void *,int,unsigned);
extern "C" void *memcpy(void *,const void *,unsigned);
class Tag {};
class TrueType {};
struct Vec {char *ptr;u32 size,cap;};
struct CapPair {
 u32 cap;u32 *alloc;
 CapPair(u32 *a):cap(0),alloc(a){}
 u32 &First(){return cap;}
};
struct Buf {
 char *ptr;u32 size;CapPair pair;u32 start;
 Buf(u32 *a):ptr(0),size(0),pair(a),start(0){}
 u32 &EndCap(){return pair.First();}
 void Allocate(u32 n,Tag t);
 void AllocA(u32 n);
};
extern "C" u32 ov45_02237890(Vec *,u32);
extern "C" void ov45_022389CC(Buf *,u32,Tag);
extern "C" void ov45_022389B0(Buf *);
inline void Buf::Allocate(u32 n,Tag t){ov45_022389CC(this,n,t);}
inline void Buf::AllocA(u32 n){Allocate(n,Tag());}
extern "C" void ov45_02238820(Vec *,u32);
template<class T> inline void Swap(T &a,T &b){T t=a;a=b;b=t;}
inline u32 Recommend(Vec *v,u32 n,TrueType){return ov45_02237890(v,n);}
inline void FillN(char *p,u32 n,char c){TrueType f=TrueType();(void)f;memset(p,c,n);}
inline char *CopyT(const char *first,const char *last,char *out){u32 n=(u32)(last-first);memcpy(out,first,n);return out+n;}
inline void MoveBack(Buf &b,Vec *v){char *first=v->ptr;char *last=first+v->size;TrueType f=TrueType();(void)f;
 u32 len=(u32)(last-first);
 b.start-=len;
 CopyT(first,last,b.ptr+b.start);
 memset(first,0,len);
 b.size+=len;
}
inline u32 Wrap(TrueType,u32 x){return x;}
inline void Clear(Vec *v){TrueType f=TrueType();(void)f;v->size=0;}
// @fn ov45_02238820
extern "C" void ov45_02238820(Vec *v,u32 n){
 Buf b(&v->cap);
 u32 len=ov45_02237890(v,v->size+n-v->cap);
 b.AllocA(len);
 b.start=v->size;
 FillN(b.ptr+b.start+b.size,n,0);
 b.size+=n;
 MoveBack(b,v);
 Clear(v);
 Swap(v->cap,b.EndCap());
 Swap(v->ptr,b.ptr);
 Swap(v->size,b.size);
 ov45_022389B0(&b);
}
