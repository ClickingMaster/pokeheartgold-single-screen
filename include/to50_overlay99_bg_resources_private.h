#ifndef TO50_OV99_BG_RESOURCES_PRIVATE_H
#define TO50_OV99_BG_RESOURCES_PRIVATE_H
#include "global.h"
typedef struct Ov99BgWork { void *bgConfig; void *narc; u8 pad08[4]; u32 heapId; } Ov99BgWork;
void GfGfxLoader_GXLoadPalFromOpenNarc(void *, u32, u32, u32, u32, u32);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, u32, void *, u32, u32, u32, u32, u32);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, u32, void *, u32, u32, u32, u32, u32);
void ov99_021E7DA4(Ov99BgWork *);
#endif
