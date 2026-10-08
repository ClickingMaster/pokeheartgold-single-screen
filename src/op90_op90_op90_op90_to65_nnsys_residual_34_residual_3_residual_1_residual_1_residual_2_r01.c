#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { u32 a:8; u32 b:8; u32 rest:16; } MapParam;
typedef struct { u8 *buf; int w; int h; u8 bpp; u8 pad[3]; MapParam param; } Canvas;
typedef struct { u8 cellWidth; u8 cellHeight; u16 cellSize; s8 baseline; u8 maxWidth; u8 bpp; u8 flags; } GlyphInfo;
typedef struct { u32 a; u32 b; GlyphInfo *glyph; } FontRes;
typedef struct { FontRes *res; } Font;
typedef struct { s8 left; u8 glyphWidth; s8 charWidth; } CharWidths;
typedef struct { const CharWidths *widths; const u8 *image; } Glyph;
typedef struct { u8 *dst; const u8 *src; int x; int y; int w; int h; int stride; int bits; int bpp; int palette; } Letter;
void LetterChar(Letter *l);
int GetCharIndex1D(int cx, int cy, int w, int h, int a, int b);

#pragma force_active on

#pragma force_active on






void DrawGlyph1D(Canvas *c, Font *f, int x, int y, int color, Glyph *g);
// @fn DrawGlyph1D
void DrawGlyph1D(Canvas *c, Font *f, int x, int y, int color, Glyph *g){
 int px;
 int cx1;
 int ny;
 int xEnd;
 const u32 cw=c->w;
 int cy1;
 int cx0;
 int cx;
 u8 *buf;
 u32 cy0;
 int pb;
 int pa;
 int yEnd;
 int nx;
 const int charSize=c->bpp*8*8/8;
 const u32 ch=c->h;
 const int w=g->widths->glyphWidth;
 const int h=f->res->glyph->cellHeight;
 Letter letter;
 if(w==0)return;
 if(x+w<0)return;
 if(y+h<0)return;
 cx0=x<=0?0:(u32)x>>3;
 cy0=y<=0?0:(u32)y>>3;
 cx1=(u32)(x+w+7)>>3; if(cw<=(u32)cx1)cx1=cw;
 cy1=(u32)(y+h+7)>>3; if(ch<=(u32)cy1)cy1=ch;
 nx=cx1-cx0;
 ny=cy1-cy0;
 if(nx<0)return;
 if(ny<0)return;
 buf=c->buf;
 if(x>=0)x&=7;
 if(y>=0)y&=7;
 xEnd=x-nx*8;
 yEnd=y-ny*8;
 letter.src=g->image;
 letter.w=w;
 letter.palette=color-1;
 letter.h=h;
 letter.bits=f->res->glyph->bpp;
 letter.bpp=c->bpp;
 letter.stride=letter.bits*f->res->glyph->cellWidth;
 {
  const MapParam param=c->param;
  const int areaW=c->w;
  const int areaH=c->h;
  pa=param.a;
  pb=param.b;
  for(;y>yEnd;y-=8){
   letter.y=y;
   for(cx=cx0,px=x;px>xEnd;px-=8){
    {
     int idx=GetCharIndex1D(cx,cy0,areaW,areaH,pa,pb);
     letter.dst=buf+idx*charSize;
    }
    letter.x=px;
    LetterChar(&letter);
    cx++;
   }
   cy0++;
  }
 }
}
