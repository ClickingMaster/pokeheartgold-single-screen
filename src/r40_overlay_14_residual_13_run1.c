#include "global.h"

void BgCellManager_DrawCellAt(void *arg0, int arg1, int arg2, int arg3);
void BgCellManager_StartCellMove(void *arg0, int arg1, int arg2, int arg3, int arg4);
void PCBox_DrawPartyButton(void *arg0, int arg1);
void PCBox_SlideInWallpaperList(void *arg0);
void PCBox_SlideOutWallpaperList(void *arg0);
void PCBox_ShowPartyButton(void *arg0);

void PCBox_SlideInWallpaperList(void *arg0) {
    BgCellManager_DrawCellAt(arg0, 13, 0, -7);
    BgCellManager_StartCellMove(arg0, 13, 0, 1, 7);
}

void PCBox_SlideOutWallpaperList(void *arg0) {
    BgCellManager_DrawCellAt(arg0, 13, 0, 0);
    BgCellManager_StartCellMove(arg0, 13, 0, -1, 7);
}

void PCBox_ShowPartyButton(void *arg0) {
    PCBox_DrawPartyButton(arg0, 0);
    BgCellManager_DrawCellAt(*(void **)((u8 *)*(void **)((u8 *)arg0 + 0x34) + 0x2F0), 8, 0, 21);
}
