#include "global.h"

#include "r40_overlay_103_residual_4_private.h"
#define PTR(c, o) (*(void **)((c) + (o)))

void ov103_021ECD68(R40Ov103Work4 *work) {
    u32 i;
    void *first;
    PTR(work->ctx, 4) = BgCellManager_New(PTR(work->ctx, 0), 2, 11, 0x9D);
    for (i = 0; i < 10; i++) {
        BgCellManager_InitCell(PTR(work->ctx, 4), i, 2, 14, 4);
    }
    BgCellManager_InitCell(PTR(work->ctx, 4), 10, 1, 14, 4);
    BgCellManager_LoadCellTilemap(PTR(work->ctx, 4), 0, 0xFC, 3, 1);
    first = BgCellManager_GetCellTilemap(PTR(work->ctx, 4), 0);
    for (i = 1; i <= 10; i++) {
        MI_CpuCopy8(first, BgCellManager_GetCellTilemap(PTR(work->ctx, 4), i), 0x70);
    }
    for (i = 0; i < 10; i++) {
        BgCellManager_DrawCellAt(PTR(work->ctx, 4), i, (s8)((i & 1) * 16 + 1), (s8)((i >> 1) * 4));
    }
}
