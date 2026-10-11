#include "overlay18_pokedex_info_windows_internal.h"

void PokedexApp_ClearSearchEntryWindows(void *p) {
    u32 i;
    u8 *window = (u8 *)p + 0x51C;
    for (i = 0x51; i <= 0x5D; i++) {
        ClearWindowTilemapAndScheduleTransfer((Window *)window);
        window += 0x10;
    }
}

void ov18_021F0168(void *p) {
    u32 i;
    for (i = 0; i < 6; i++) {
        BgCellManager_InitCell(PTR(p, 8), i + 0x11, 2, 0x12, 2);
    }
}

void ov18_021F018C(void *p) {
    u16 i;
    u16 *dst;
    u16 base;
    u16 j;
    for (i = 0; i < 6; i++) {
        dst = BgCellManager_GetCellTilemap(PTR(p, 8), i + 0x11);
        base = *(const u16 *)(sAreaWindowTemplates + i * 8 + 0x46);
        for (j = 0; j < 0x24; j++) {
            dst[j] = (base + j) | 0x1000;
        }
    }
}

void ov18_021F01D4(void *p) {
    u16 i;
    u16 *dst;
    u16 base;
    u16 j;
    for (i = 0; i < 6; i++) {
        dst = BgCellManager_GetCellTilemap(PTR(p, 8), i + 0x11);
        base = *(const u16 *)(sFormsWindowTemplates + i * 8 + 0x56);
        for (j = 0; j < 0x24; j++) {
            dst[j] = (base + j) | 0x1000;
        }
    }
}
