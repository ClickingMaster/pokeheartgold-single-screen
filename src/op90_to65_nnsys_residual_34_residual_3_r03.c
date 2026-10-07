#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 *buf; int w; int h; u8 bpp; u8 pad[3]; int stride; } Canvas;
void ClearChar(void *chr,int x,int y,int w,int h,u32 col,int bpp);
void ClearAreaLine(Canvas *,u32,int,int,int,int);
#pragma force_active on
static inline int CharSize(int bpp){return bpp*8*8/8;}
// @fn ClearAreaLine
void ClearAreaLine(Canvas *c,u32 col,int x,int y,int w,int h){
 int cy;
 int cx;
 int hh;
 const int bpp=c->bpp;
 const int x1=x+w;
 const int y1=y+h;
 if(bpp==4){col|=col<<4;col|=col<<8;col|=col<<16;}else{col|=col<<8;col|=col<<16;}
 {
  const int cs=CharSize(bpp);
  cy=y&~7;
  const int cx0=x&~7;
  const int cx1=(x1+7)&~7;
  const int cy1=(y1+7)&~7;
  u8 *line=c->buf+cs*((cy/8)*c->stride+cx0/8);
  const int lineStep=c->stride*cs;
  for(;cy<cy1;cy+=8){
   const int top=(cy<y)?y-cy:0;
   int bottom=y1-cy;
   u8 *p=line;
   if(bottom>8)bottom=8;
   hh=bottom-top;
   for(cx=cx0;cx<cx1;cx+=8){
    const int left=(cx<x)?x-cx:0;
    int right=x1-cx;
    if(right>8)right=8;
    ClearChar(p,left,top,right-left,hh,col,bpp);
    p+=cs;
   }
   line+=lineStep;
  }
 }
}
