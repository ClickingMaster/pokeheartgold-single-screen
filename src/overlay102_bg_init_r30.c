#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"

typedef struct Ov102Work30 {
    BgConfig *bgConfig;
    u8 pad_004[4];
    void *menu;
} Ov102Work30;

BgConfig *ov102_021EA268(BgConfig *bgConfig);
u32 ov102_021EB77C(Ov102Work30 *work, BgConfig *bgConfig, u32 tile);
u32 ov102_021EB880(Ov102Work30 *work, BgConfig *bgConfig, u32 tile);
void ov102_021EB94C(Ov102Work30 *work, BgConfig *bgConfig, u32 tile);
void ov102_021EB950(Ov102Work30 *work);
void ov102_021EB6E0(Ov102Work30 *work, NARC *narc);

void ov102_021EB6E0(Ov102Work30 *work, NARC *narc) {
    BgConfig *bgConfig = ov102_021EA268(work->bgConfig);
    u32 size;
    u32 tile;
    int zero = 0;

    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_35);
    size = GfGfxLoader_LoadCharDataFromOpenNarc(narc, 3, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_35);
    tile = ov102_021EB77C(work, bgConfig, size >> 5);
    tile = ov102_021EB880(work, bgConfig, tile);
    ov102_021EB94C(work, bgConfig, tile);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, (enum BgPosAdjustOp)zero, zero - 4);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, (enum BgPosAdjustOp)3, 0x10);
    ov102_021EB950(work);
    G2x_SetBlendBrightness_((vu16 *)0x04000050, 2, -10);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_1);
}
