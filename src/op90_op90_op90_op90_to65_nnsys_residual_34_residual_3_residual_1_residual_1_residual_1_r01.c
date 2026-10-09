#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { u32 *dst; const u8 *src; int x; int y; int w; int h; int stride; int bits; int bpp; u32 palette; } Letter;
typedef struct { const u8 *ptr; s8 avail; u8 byte; } BitReader;
u32 NNSi_G2dBitReaderRead(BitReader *r, int n);

void LetterChar(Letter *l);
// @fn LetterChar
void LetterChar(Letter *l){
 int x;
 int y;
 int ny;
 u32 left;
 int bitpos;
 u32 pal8;
 int top;
 u32 right;
 int bottom;
 const u8 *src;
 u32 v0;
 int bpp;
 u32 c;
 int stride;
 u32 v1;
 u32 *p;
 int bits;
 u32 i;
 u32 pal4;
 u32 v;
 u32 *end;
 x=l->x;
 y=l->y;
 left=x>=0?x:0;
 top=y>=0?y:0;
 right=x+l->w<8?x+l->w:8;
 bottom=y+l->h<8?y+l->h:8;
 y=y>0?0:y;
 ny=-y;
 x=x>0?0:x;
 bits=l->bits;
 stride=l->stride;
 bitpos=ny*stride;
 bitpos+=bits*(-x);
 bpp=l->bpp;
 left*=bpp;
 right*=bpp;
 src=l->src;
 if(bpp==4){
  pal4=l->palette;
  p=l->dst+top;
  end=l->dst+bottom;
  for(;p<end;p++){
   BitReader r;
   v=*p;
   r.ptr=src+((u32)bitpos>>3);
   r.avail=0;
   r.byte=0;
   NNSi_G2dBitReaderRead(&r,bitpos%8);
   for(i=left;i<right;i+=4){
    c=NNSi_G2dBitReaderRead(&r,bits);
    if(c!=0) v=(v&~(0xF<<i))|((pal4+c)<<i);
   }
   *p=v;
   bitpos+=stride;
  }
 }else{
  pal8=l->palette;
  end=l->dst+bottom*2;
  p=l->dst+top*2;
  for(;p<end;p+=2){
   BitReader r;
   v0=p[0];
   v1=p[1];
   r.ptr=src+((u32)bitpos>>3);
   r.avail=0;
   r.byte=0;
   NNSi_G2dBitReaderRead(&r,bitpos%8);
   for(i=left;i<right;i+=8){
    c=NNSi_G2dBitReaderRead(&r,bits);
    if(c!=0){
     if(i<32) v0=(v0&~(0xFF<<i))|((pal8+c)<<i);
     else v1=(v1&~(0xFF<<(i-32)))|((pal8+c)<<(i-32));
    }
   }
   p[0]=v0;p[1]=v1;
   bitpos+=stride;
  }
 }
}
