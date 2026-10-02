#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Bitmap{void *pixels;u16 width,height;}Bitmap;
void BlitBitmapRect4Bit(Bitmap *,Bitmap *,u16,u16,u16,u16,u16,u16,int);
void sub_0200EA24(void *,u16,u16,u16,u16,void *,u16,u16,u16,u16,u16,u16);
void sub_0200EA24(void *src,u16 x,u16 y,u16 width,u16 height,void *dst,u16 dw,u16 dh,u16 dx,u16 dy,u16 cw,u16 ch){Bitmap a,b;a.pixels=src;a.width=width;a.height=height;b.pixels=dst;b.width=dw;b.height=dh;BlitBitmapRect4Bit(&a,&b,x,y,dx,dy,cw,ch,0);}
