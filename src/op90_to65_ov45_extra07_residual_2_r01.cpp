#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" void *memset(void *,int,unsigned);
class Alloc {};
class TrueType {};
struct Vec {char *ptr;u32 size,cap;};
extern "C" void ov45_02237800(Vec *,u32,Alloc);
extern "C" void ov45_022377AC(Vec *,u32);
inline void FillN(char *p,u32 n,char c,TrueType){memset(p,c,n);}
// @fn ov45_022377AC
extern "C" void ov45_022377AC(Vec *v,u32 n){
 if(n){
  ov45_02237800(v,n,Alloc());
  FillN(v->ptr+v->size,n,char(),TrueType());
  v->size+=n;
 }
}
