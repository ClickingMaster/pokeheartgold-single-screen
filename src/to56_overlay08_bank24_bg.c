#include "to56_overlay08_bank24_private.h"
#pragma force_active on

void BattleBagMenu_InitBgs(u8 *work) {
    GraphicsModes modes = sBattleBagGraphicsModes;
    SetScreenModeAndDisable(&modes, SCREEN_SUB);
    {
        BgTemplate bg6 = sBattleBagBgTemplateSub2;
        InitBgFromTemplate(PTR(work, 4), 6, &bg6, 0);
    }
    {
        BgTemplate bg5 = sBattleBagBgTemplateSub1;
        InitBgFromTemplate(PTR(work, 4), 5, &bg5, 0);
    }
    BgClearTilemapBufferAndCommit(PTR(work, 4), 5);
    {
        BgTemplate bg4 = sBattleBagBgTemplateSub0;
        InitBgFromTemplate(PTR(work, 4), 4, &bg4, 0);
    }
    BgClearTilemapBufferAndCommit(PTR(work, 4), 4);
    BG_ClearCharDataRange(5, 0x20, 0, (enum HeapID)U32(PTR(work, 0), 0xC));
    BG_ClearCharDataRange(4, 0x20, 0, (enum HeapID)U32(PTR(work, 0), 0xC));
    ScheduleBgTilemapBufferTransfer(PTR(work, 4), 5);
    ScheduleBgTilemapBufferTransfer(PTR(work, 4), 4);
}

void BattleBagMenu_FreeBgs(void *bgConfig) {
    GfGfx_EngineBTogglePlanes(0x17, 0);
    FreeBgTilemapBuffer(bgConfig, 4);
    FreeBgTilemapBuffer(bgConfig, 5);
    FreeBgTilemapBuffer(bgConfig, 6);
}
