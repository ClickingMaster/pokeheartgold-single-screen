#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define S(p,o) (*(s8 *)((u8 *)(p)+(o)))
extern void NNSi_G2dGetUnpackedFont(void *,void *);
extern u16 NNSi_G2dSplitCharUTF16(void **);
extern u32 _02109434[4][4];
extern void MIi_CpuClearFast(u32,u32 *,u32);
extern u8 *_021DA498;
extern void *NNSi_G2dGetCellTransferState(u32);
extern void DrawCellToSurface3D_(void *,void *),DrawCellToSurface2D_(void *,void *);
#pragma warn_unusedvar off
u16 GetGlyphIndex(u8 *p,u16 c) ;
void NNS_G2dFontInitUTF16(u32 *p,void *data) ;
u16 NNS_G2dFontFindGlyphIndex(u32 *font,u16 c) ;
void *NNS_G2dFontGetCharWidthsFromIndex(u32 *font,u16 c) ;
int NNSi_G2dFontGetStringWidth(u32 *font,int gap,void *str,void **next) ;
int NNSi_G2dFontGetTextHeight(u32 *font,int gap,void *str) ;
void NNSi_G2dFontGetTextRect(int *out,u32 *font,int xgap,int ygap,void *text) ;
u32 OBJSizeToShape(u8 *p) ;
void ClearContinuous(u8 *p,u32 c) ;
void ClearLine(u8 *p,u32 c) ;
void InitCharCanvas(u8 *p,void *mem,u32 width,u32 height,u32 bpp,void *table,u32 layout) ;
void NNS_G2dRndCoreDrawCellVramTransfer(void *cell,u32 id) ;

u16 GetGlyphIndex(u8 *p,u16 c) { u16 result=0xffff;switch(H(p,4)){case 0:{int diff=c-H(p,0);result=H(p,12)+diff;}break;case 1:{u32 diff=c-H(p,0);p+=diff*2;result=H(p,12);break;}case 2:{struct Pair {u16 c,i;} *lo=(void *)(p+14),*hi=lo+(H(p,12)-1);while(lo<=hi){struct Pair *mid=lo+(hi-lo)/2;if(mid->c<c)lo=mid+1;else if(c<mid->c)hi=mid-1;else{result=mid->i;break;}}break;}}return result; }

void NNS_G2dFontInitUTF16(u32 *p,void *data) { NNSi_G2dGetUnpackedFont(data,p);p[1]=(u32)NNSi_G2dSplitCharUTF16; }

u16 NNS_G2dFontFindGlyphIndex(u32 *font,u16 c) { u8 *p=(void *)W((void *)font[0],16);while(p){if(H(p,0)<=c && c<=H(p,2))return GetGlyphIndex(p,c);p=(void *)W(p,8);}return 0xffff; }
