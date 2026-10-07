#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct Work {
    u8 unk00[0x20];
    BgConfig *bgConfig;
} Work;
u32 AGB_GetBoxMonData(void *boxMon, int field, void *dest);
void ov74_02232700(Work *work, void *boxMon);

// @fn ov74_02232700
void ov74_02232700(Work *work, void *boxMon) {
    u16 *tilemap = GetBgTilemapBuffer(work->bgConfig, 2);
    u8 markings;
    int i;
    if (boxMon != NULL) {
        markings = AGB_GetBoxMonData(boxMon, 8, NULL);
    } else {
        markings = 0;
    }
    for (i = 0; i < 4; i++) {
        if (markings & 1) {
            tilemap[0x6B + i] = 0x80 + i;
        } else {
            tilemap[0x6B + i] = 0x60 + i;
        }
        markings >>= 1;
    }
    BgCommitTilemapBufferToVram(work->bgConfig, 2);
}
