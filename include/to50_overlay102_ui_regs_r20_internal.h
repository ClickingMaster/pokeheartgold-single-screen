#ifndef TO50_OV102_R20_GAP2_PRIVATE_H
#define TO50_OV102_R20_GAP2_PRIVATE_H
#include "global.h"
#include "filesystem.h"
typedef struct BgConfig BgConfig;
void *ov102_021EA268(void *);
void ov102_021EA80C(void *, NARC *);
void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *, s32, void *, u32, u32, u32, BOOL, u32);
u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *, s32, void *, u32, u32, u32, BOOL, u32);
u32 GfGfxLoader_LoadCharData(u32, s32, void *, u32, u32, u32, BOOL, u32);
void BgSetPosTextAndCommit(void *, u32, u32, s32);
void AddWindowParameterized(void *, void *, u8, u8, u8, u8, u8, u8, u16);
void *YesNoPrompt_Create(u32);
void GfGfxLoader_GXLoadPal(u32, s32, u32, u32, u32, u32);
u32 ov102_021E9084(void *);
void LoadUserFrameGfx2(void *, u32, u16, u32, u8, u32);
void DrawFrame2AroundWindow(void *, u32, u32);
void PutWindowTilemap(void *);
void ov102_021EA8C0(void *);
void ov102_021EAA3C(void *);
void ov102_021EAE40(void *, u32);
void ov102_021EA920(void *);
u32 ov102_021E8F6C(void *);
void ov102_021EAC20(void *);
void ov102_021EA41C(u8 *, NARC *);
void ov102_021EA644(u8 *);
void ov102_021EA678(void *, BOOL);
#endif
