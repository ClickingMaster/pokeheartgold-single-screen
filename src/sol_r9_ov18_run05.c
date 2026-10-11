#include "sol_r9_ov18_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void PokedexApp_SetupSearchWindows(void *work) {
    PokedexApp_AddWindows(work, sSearchWindowTemplates, 0x65);
    PokedexApp_DrawSearchMessage(work, 0);
    PokedexApp_DrawSearchMenuPage(work);
    PokedexApp_DrawSearchCriteria(work);
    ov18_021EEE84(work);
}

void ov18_021EEE84(void *work) {
    BgCellManager_PutWindowTiles(PTR(work, 8), 6, (u8 *)work + 0x11c);
    BgCellManager_PutWindowTiles(PTR(work, 8), 6, (u8 *)work + 0x12c);
    BgCellManager_PutWindowTiles(PTR(work, 8), 6, (u8 *)work + 0x13c);
    BgCellManager_PutWindowTiles(PTR(work, 8), 7, (u8 *)work + 0x11c);
    BgCellManager_PutWindowTiles(PTR(work, 8), 7, (u8 *)work + 0x13c);
}
