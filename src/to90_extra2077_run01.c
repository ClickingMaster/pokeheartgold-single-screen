#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 value[10];} Banks;
typedef struct {u32 value[4];} Modes;
extern const Banks _0223B340;
extern const Modes _0223B330;
typedef struct {u32 heap;void *bg;u8 pad[12];void *options;} View;
void GfGfx_SetBanks(const Banks*);
void SetBothScreensModesAndDisable(const Modes*);
void ov74_02235308(void*,int,int,int,int);
void ResetAllTextPrinters(void);
void LoadFontPal0(int,int,u32);
void LoadUserFrameGfx1(void*,int,int,int,int,u32);
void LoadUserFrameGfx2(void*,int,int,int,int,u32);
u32 Options_GetFrame(void*);
void GfGfxLoader_GXLoadPal(int,int,int,int,int,u32);
void GfGfxLoader_LoadCharData(int,int,void*,int,int,int,int,u32);
void GfGfxLoader_LoadScrnData(int,int,void*,int,int,int,int,u32);
void BgTilemapRectChangePalette(void*,int,int,int,int,int,int);
void BgCommitTilemapBufferToVram(void*,int);
void ov74_02228E98(View*);
void ov74_02228D64(View*);
void ov74_02228D64(View *v){
 Banks banks=_0223B340;Modes modes=_0223B330;
 GfGfx_SetBanks(&banks);SetBothScreensModesAndDisable(&modes);
 ov74_02235308(v->bg,0,1,0xf000,0);ov74_02235308(v->bg,1,1,0xf800,0x4000);ov74_02235308(v->bg,4,1,0x7800,0);ov74_02235308(v->bg,5,1,0x7000,0x4000);
 ResetAllTextPrinters();LoadFontPal0(0,0,v->heap);LoadUserFrameGfx1(v->bg,0,1,1,0,v->heap);LoadUserFrameGfx2(v->bg,0,10,2,(u8)Options_GetFrame(v->options),v->heap);
 GfGfxLoader_GXLoadPal(113,0,0,256,32,v->heap);GfGfxLoader_LoadCharData(113,1,v->bg,1,0,0x1400,1,v->heap);GfGfxLoader_LoadScrnData(113,2,v->bg,1,0,0x600,1,v->heap);
 BgTilemapRectChangePalette(v->bg,1,0,0,32,24,8);BgCommitTilemapBufferToVram(v->bg,1);ov74_02228E98(v);
}
