#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
void ClearChar(u32 *chr,int x,int y,int w,int h,u32 col,int bpp);
#pragma force_active on
// @fn ClearChar
void ClearChar(u32 *chr,int x,int y,int w,int h,u32 col,int bpp){
 if(w==8&&h==8){
  MIi_CpuClearFast(col,chr,bpp*8);
 }else if(bpp==4){
  const int s=x*4;
  const int e=32-(s+w*4);
  const u32 mask=((0xFFFFFFFFu>>s)<<(e+x*4))>>e;
  u32 *p=chr+y;
  const u32 v=col&mask;
  const u32 keep=~mask;
  u32 *end=p+h;
  for(;p<end;p++)*p=(*p&keep)|v;
 }else{
  u32 lo,hi;
  u32 s=x*8;
  u32 e=64-(s+w*8);
  lo=0xFFFFFFFFu>>s;
  if(e<32)lo<<=s;else{u32 t=e-32;lo=(lo<<(s+t))>>t;}
  hi=0xFFFFFFFFu<<e;
  if(s<32)hi>>=e;else{u32 t=s-32;hi=(hi>>(t+e))<<t;}
  {
   const u32 v0=col&lo;
   const u32 v1=col&hi;
   u32 *p=chr+y*2;
   u32 *end=p+h*2;
   const u32 k0=~lo;
   const u32 k1=~hi;
   for(;p<end;p+=2){p[0]=(p[0]&k0)|v0;p[1]=(p[1]&k1)|v1;}
  }
 }
}
