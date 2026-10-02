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

void InitCharCanvas(u8 *p,void *mem,u32 width,u32 height,u32 bpp,void *table,u32 layout) { W(p,4)=width;W(p,8)=height;B(p,12)=bpp;W(p,0)=(u32)mem;W(p,20)=(u32)table;W(p,16)=layout; }
