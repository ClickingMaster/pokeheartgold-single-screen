#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"

typedef struct Ov43Gfx {
    BgConfig *bgConfig;
    u8 unk4[0x208];
    NNSG2dScreenData *screen;
} Ov43Gfx;

typedef struct Ov43Unk {
    u8 unk0[0x10];
    int mode;
} Ov43Unk;

extern const u8 ov43_0222EDB8[][8];
extern const u8 ov43_0222EDF8[][8];
extern const u8 ov43_0222F0C4[];

void ov43_0222DB94(Ov43Unk *data, Ov43Gfx *gfx, u8 index);

// @fn ov43_0222DB94
void ov43_0222DB94(Ov43Unk *data, Ov43Gfx *gfx, u8 index) {
    if (data->mode == 1) {
        CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 20, 32, 4, gfx->screen->rawData, 0, 12, gfx->screen->screenWidth >> 3, gfx->screen->screenHeight >> 3);
    } else {
        CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 20, 32, 4, gfx->screen->rawData, 0, 20, gfx->screen->screenWidth >> 3, gfx->screen->screenHeight >> 3);
    }
    if (index == 0xFF) {
        ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
        return;
    }
    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, ov43_0222EDB8[data->mode][index], 20, ov43_0222EDF8[data->mode][index], 4, gfx->screen->rawData, ov43_0222EDB8[data->mode][index], ov43_0222F0C4[data->mode], gfx->screen->screenWidth >> 3, gfx->screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}
