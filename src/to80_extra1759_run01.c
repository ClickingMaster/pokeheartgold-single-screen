#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {void *info; u16 (*decode)(const void **);} Fnt;
typedef struct {void *canvas; Fnt *font; int charSpace,lineSpace;} TC;
typedef struct {s8 x,y;} Dir;
typedef struct {int width,height;} Rect;
void NNS_G2dMapScrToChar256x16Pltt(u16 *,int,int,int,int,int);
int NNS_G2dCharCanvasDrawChar(void *,Fnt *,int,int,int,u16);
int NNSi_G2dFontGetStringWidth(Fnt *,int,const void *,void *);
int NNSi_G2dFontGetTextHeight(Fnt *,int,const void *);
Rect NNSi_G2dFontGetTextRect(Fnt *,int,int,const void *);
void NNSi_G2dTextCanvasDrawString(TC *,int,int,int,const void *,const void **,Dir);
void NNSi_G2dTextCanvasDrawTextAlign(TC *,int,int,int,int,int,const void *,Dir);
void NNS_G2dMapScrToCharText(u16 *,int,int,int,int,int,int,int);
u32 NNSi_G2dCalcRequiredOBJ(u32,u32);
void NNSi_G2dTextCanvasDrawText(TC *,int,int,int,int,const void *,Dir);
void NNSi_G2dTextCanvasDrawTextRect(TC *,int,int,int,int,int,int,const void *,Dir);
static inline Rect getrect(TC *tc,const void *text){Rect r=NNSi_G2dFontGetTextRect(tc->font,tc->charSpace,tc->lineSpace,text);return r;}
typedef struct {u32 signature;u16 endian,version;} FontHeader;
typedef struct {u8 pad[8];u8 *glyph;} FontInfo;
BOOL NNSi_G2dGetUnpackedFont(FontHeader *,FontInfo **);
void NNSi_G2dUnpackNFT(FontHeader *);
void *NNS_G2dFindBinaryBlock(FontHeader *,u32);
static inline int signature(FontHeader *p){if(p&&p->signature==0x4e465452)return TRUE;return FALSE;}
static inline int version(FontHeader *p,u16 v){if(p&&p->version>=v)return TRUE;return FALSE;}
static inline int valid(FontHeader *p,u16 v){if(p){int ok=(BOOL)(signature(p)!=0);if(ok)ok=(BOOL)(version(p,v)!=0);return ok;}return 0;}
void NNS_G2dMapScrToCharText(u16 *map,int width,int height,int x,int y,int stride,int tile,int palette){
 if(stride<=32){NNS_G2dMapScrToChar256x16Pltt(map+(stride*y+x),width,height,stride,tile,palette);return;}
 {int endX=x+width,endY=y+height;u16 pal=(u16)(palette<<12);for(;y<endY;y++){int row=y<32?y:y+32;int xx;u16 *base=map+row*32;for(xx=x;xx<endX;xx++){int col=xx<32?xx:xx+992;base[col]=tile++|pal;}}}
}
