#ifndef R40_OV103_R4_PRIVATE_H
#define R40_OV103_R4_PRIVATE_H
#include "global.h"
typedef struct R40Ov103Work4 {
    u8 pad[0xC];
    u8 *ctx;
} R40Ov103Work4;
extern void *BgCellManager_New(void *, u32, u32, u32);
extern void BgCellManager_InitCell(void *, u32, u32, u32, u32);
extern void BgCellManager_LoadCellTilemap(void *, u32, u32, u32, u32);
extern void *BgCellManager_GetCellTilemap(void *, u32);
extern void BgCellManager_DrawCellAt(void *, u32, s32, s32);
void ov103_021ECD68(R40Ov103Work4 *);
#endif
