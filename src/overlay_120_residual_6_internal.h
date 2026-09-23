#ifndef OVERLAY_120_RESIDUAL_6_PRIVATE_H
#define OVERLAY_120_RESIDUAL_6_PRIVATE_H

#include "global.h"

typedef struct ScrnData120 {
    u16 width;
    u16 height;
    u8 padding04[8];
    u8 raw[1];
} ScrnData120;

void *GfGfxLoader_GetScrnDataFromOpenNarc(void *narc, int member, int compressed, ScrnData120 **data, int heapId);
void LoadRectToBgTilemapRect(void *bgConfig, u8 bgId, const void *buffer, u8 x, u8 y, u8 width, u8 height);
void BgTilemapRectChangePalette(void *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette);
void Heap_Free(void *ptr);
void ScheduleBgTilemapBufferTransfer(void *bgConfig, u8 bgId);

void ov120_0225FD2C(void *bgConfig, void *narc, int member, int bgId);

#endif
