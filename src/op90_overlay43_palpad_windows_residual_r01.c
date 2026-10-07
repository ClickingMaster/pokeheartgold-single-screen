#pragma force_active on
#include "global.h"
#include "bg_window.h"

typedef struct Ov43PalPair {
    u8 pal[2];
} Ov43PalPair;

typedef struct Ov43PalTable {
    u8 unk0;
    Ov43PalPair pair;
} Ov43PalTable;

typedef struct Ov43Unk {
    BgConfig *bgConfig;
} Ov43Unk;

extern const Ov43PalTable _0222ED5C;

void ov43_0222D0F8(void *unused, Ov43Unk *data, int index);

// @fn ov43_0222D0F8
void ov43_0222D0F8(void *unused, Ov43Unk *data, int index) {
    Ov43PalPair pals = _0222ED5C.pair;
    BgTilemapRectChangePalette(data->bgConfig, GF_BG_LYR_MAIN_2, 17, 20, 14, 4, pals.pal[index]);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_MAIN_2);
}
