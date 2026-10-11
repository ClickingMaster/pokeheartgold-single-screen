#include "sol_r9_ov18_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov18_021EEE58(void *work) {
    ov18_021EE35C(work, ov18_021F9FDC, 0x65);
    ov18_021EEED0(work, 0);
    ov18_021EF45C(work);
    ov18_021EF528(work);
    ov18_021EEE84(work);
}

void ov18_021EEE84(void *work) {
    BgCellManager_PutWindowTiles(PTR(work, 8), 6, (u8 *)work + 0x11c);
    BgCellManager_PutWindowTiles(PTR(work, 8), 6, (u8 *)work + 0x12c);
    BgCellManager_PutWindowTiles(PTR(work, 8), 6, (u8 *)work + 0x13c);
    BgCellManager_PutWindowTiles(PTR(work, 8), 7, (u8 *)work + 0x11c);
    BgCellManager_PutWindowTiles(PTR(work, 8), 7, (u8 *)work + 0x13c);
}
