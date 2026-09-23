#ifndef TO50_OV99_GFX_SETUP_LANDING_PRIVATE_H
#define TO50_OV99_GFX_SETUP_LANDING_PRIVATE_H
#include "global.h"
typedef struct Ov99GfxWork { u8 pad00[4]; void *bgConfig; void *narc; u32 heapId; u8 pad10[8]; void *sprites[40]; } Ov99GfxWork;
void GfGfxLoader_GXLoadPalFromOpenNarc(void *, u32, u32, u32, u32, u32);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, u32, void *, u32, u32, u32, u32, u32);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, u32, void *, u32, u32, u32, u32, u32);
void ov99_021E5D58(Ov99GfxWork *);
typedef struct Ov99Pos { u32 sprite; s32 x; s32 y; } Ov99Pos;
extern const u32 ov99_021E95D8[];
extern const u32 ov99_021E95DC[];
extern const Ov99Pos ov99_021E9690[];
void GfGfx_EngineATogglePlanes(u32, u32);
void GfGfx_EngineBTogglePlanes(u32, u32);
void BgSetPosTextAndCommit(void *, u32, u32, s32);
void ManagedSprite_OffsetPositionXY(void *, s16, s16);
#endif
