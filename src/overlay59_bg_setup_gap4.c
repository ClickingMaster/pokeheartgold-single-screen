#include "overlay59_bg_setup_gap4_private.h"
void ov59_0223B434(Ov59R10Gap4Work *work) {
    GraphicsModes modes;
    BgTemplate bg7;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;
    ov59_0223B414();
    work->bgConfig = BgConfig_Alloc(work->heap);
    modes = ov59_0223C978;
    SetBothScreensModesAndDisable(&modes);
    *(vu16 *)0x04000304 &= 0xFFFF7FFF;
    bg7 = ov59_0223C9B0;
    InitBgFromTemplate(work->bgConfig, 7, &bg7, 0);
    BgClearTilemapBufferAndCommit(work->bgConfig, 7);
    bg1 = ov59_0223C9CC;
    InitBgFromTemplate(work->bgConfig, 1, &bg1, 0);
    BgClearTilemapBufferAndCommit(work->bgConfig, 1);
    bg2 = ov59_0223C9E8;
    InitBgFromTemplate(work->bgConfig, 2, &bg2, 0);
    BgClearTilemapBufferAndCommit(work->bgConfig, 2);
    bg3 = ov59_0223CA04;
    InitBgFromTemplate(work->bgConfig, 3, &bg3, 0);
    BgClearTilemapBufferAndCommit(work->bgConfig, 3);
    BG_ClearCharDataRange(7, 0x20, 0, work->heap);
    BG_ClearCharDataRange(1, 0x20, 0, work->heap);
    BG_ClearCharDataRange(2, 0x20, 0, work->heap);
    BG_ClearCharDataRange(3, 0x20, 0, work->heap);
}
