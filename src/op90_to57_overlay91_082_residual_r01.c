#pragma force_active on
#include "global.h"
typedef struct BgConfig BgConfig;
void SetBgPriority(u8 bgId, u8 priority);
void BgCommitTilemapBufferToVram(BgConfig *bgConfig, u8 bgId);
void BgTilemapRectChangePalette(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette);
typedef struct Work {
    BgConfig *bgConfig;
} Work;
void ov91_022614FC(Work *work, int base, int pal);
// @fn ov91_022614FC
void ov91_022614FC(Work *work, int base, int pal) {
    int i;
    int layer;
    for (i = 0; i < 3; i++) {
        layer = base - i;
        if (layer < 0) {
            layer += 3;
        }
        SetBgPriority(layer + 4, 2 - i);
        if (i == 0) {
            BgTilemapRectChangePalette(work->bgConfig, layer + 4, 0, 0, 32, 32, pal * 2 + 2);
        } else {
            BgTilemapRectChangePalette(work->bgConfig, layer + 4, 0, 0, 32, 32, pal * 2 + 1);
        }
        BgCommitTilemapBufferToVram(work->bgConfig, layer + 4);
    }
}
