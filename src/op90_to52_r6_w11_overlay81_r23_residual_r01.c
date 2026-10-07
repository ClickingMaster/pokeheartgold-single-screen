#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"

void ov81_02243140(BgConfig *bgConfig, NARC *narc, int which);

// @fn ov81_02243140
void ov81_02243140(BgConfig *bgConfig, NARC *narc, int which) {
    NNSG2dScreenData *scrnData;
    void *data;
    u8 top;
    u16 *tiles;
    u8 rows;
    u8 y;
    u8 x;

    data = GfGfxLoader_GetScrnDataFromOpenNarc(narc, which + 0x87, TRUE, &scrnData, HEAP_ID_100);
    if (which == 0) {
        top = 11;
        rows = 7;
    } else {
        top = 14;
        rows = 4;
    }
    tiles = (u16 *)scrnData->rawData;
    for (y = 0; y < rows; y++) {
        for (x = 0; x < 32; x++) {
            FillBgTilemapRect(bgConfig, GF_BG_LYR_SUB_1, tiles[y * 32 + x] + 0x3D9, x, top + y, 1, 1, 9);
        }
    }
    Heap_Free(data);
}
