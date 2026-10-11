#include "to56_w21_private.h"

void ov08_0221D81C(u8 *work) {
    u16 i;
    for (i = 0; i < 6; i++) {
        ov08_0221D77C(work, 0x125, i);
    }
}

void BattlePartyMenu_ChangePage(u8 *work, int state) {
    extern void BattlePartyMenu_LoadPageTilemaps(u8 *, int);
    extern void BattlePartyMenu_DrawExpBar(u8 *, int);
    BattlePartyMenu_LoadPageTilemaps(work, state);
    BgFillTilemapBufferAndSchedule(*(BgConfig **)(work + 0x1e4), 4, 0);
    BgFillTilemapBufferAndSchedule(*(BgConfig **)(work + 0x1e4), 5, 0);
    BattlePartyMenu_ShowPageSprites(work, state);
    BattlePartyMenu_RemovePageWindows(work);
    BattlePartyMenu_AddPageWindows(work, state);
    BattlePartyMenu_PrintPageText(work, state);
    BattlePartyMenu_DrawExpBar(work, state);
    BattlePartyMenu_SetPageCursor(work, state);
    BattlePartyMenu_DrawPageButtons(work, state);
    BattlePartyMenu_LoadPagePalette(work, state);
    work[0x207a] = state;
}

void BattlePartyMenu_LoadPageTilemaps(u8 *work, int state) {
    u32 i;
    const u32 *ids = &sBattlePartyPageScreenFiles[state * 2];
    for (i = 0; i < 2; i++) {
        NNSG2dScreenData *screen;
        void *file = AllocAndReadWholeNarcMemberByIdPair((NarcId)0x47, ids[i], (enum HeapID) * (u32 *)(*(u8 **)work + 0xc));
        NNS_G2dGetUnpackedScreenData(file, &screen);
        LoadRectToBgTilemapRect(*(BgConfig **)(work + 0x1e4), i + 6, (u8 *)screen + 0xc, 0, 0, 0x20, 0x18);
        ScheduleBgTilemapBufferTransfer(*(BgConfig **)(work + 0x1e4), i + 6);
        Heap_Free(file);
    }
}
