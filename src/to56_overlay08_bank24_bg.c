#include "to56_overlay08_bank24_private.h"
#pragma force_active on

void ov08_02223000(u8 *work) {
    GraphicsModes modes = ov08_02225AE8;
    SetScreenModeAndDisable(&modes, SCREEN_SUB);
    {
        BgTemplate bg6 = ov08_02225B30;
        InitBgFromTemplate(PTR(work, 4), 6, &bg6, 0);
    }
    {
        BgTemplate bg5 = ov08_02225B14;
        InitBgFromTemplate(PTR(work, 4), 5, &bg5, 0);
    }
    BgClearTilemapBufferAndCommit(PTR(work, 4), 5);
    {
        BgTemplate bg4 = ov08_02225AF8;
        InitBgFromTemplate(PTR(work, 4), 4, &bg4, 0);
    }
    BgClearTilemapBufferAndCommit(PTR(work, 4), 4);
    BG_ClearCharDataRange(5, 0x20, 0, (enum HeapID)U32(PTR(work, 0), 0xC));
    BG_ClearCharDataRange(4, 0x20, 0, (enum HeapID)U32(PTR(work, 0), 0xC));
    ScheduleBgTilemapBufferTransfer(PTR(work, 4), 5);
    ScheduleBgTilemapBufferTransfer(PTR(work, 4), 4);
}

void ov08_022230CC(void *bgConfig) {
    GfGfx_EngineBTogglePlanes(0x17, 0);
    FreeBgTilemapBuffer(bgConfig, 4);
    FreeBgTilemapBuffer(bgConfig, 5);
    FreeBgTilemapBuffer(bgConfig, 6);
}
