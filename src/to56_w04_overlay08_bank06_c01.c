#include "to56_w04_overlay08_bank06_private.h"

#pragma force_active on

int BattlePartyMenu_StartFadeOut(Ov08W04State *work) {
    PaletteData_BeginPaletteFade(work->paletteData, 0xA, 0xFFFF, -8, 0, 0x10, 0);
    return 0x1A;
}
int BattlePartyMenu_Exit(void *task, Ov08W04State *work) {
    if (PaletteData_GetSelectedBuffersBitmask(work->paletteData) != 0) {
        return 0;
    }
    BattlePartyMenu_FreeText(work);
    BattlePartyMenu_FreeSprites(work);
    BattlePartyMenu_RemoveWindows(work);
    BattlePartyMenu_FreeBgs(work->bgConfig);
    *(u8 *)((u8 *)*(void **)work + 0x32) = BattleDpadCursor_IsActive(work->smallState);
    BattleDpadCursor_Free(work->smallState);
    FontID_Release(4);
    *(u8 *)((u8 *)*(void **)work + 0x36) = 1;
    DestroySysTaskAndEnvironment(task);
    return 1;
}
void BattlePartyMenu_InitBgs(Ov08W04State *work) {
    struct GraphicsModes modes;
    struct BgTemplate bg7;
    struct BgTemplate bg6;
    struct BgTemplate bg5;
    struct BgTemplate bg4;
    modes = sBattlePartyGraphicsModes;
    SetScreenModeAndDisable(&modes, SCREEN_SUB);
    bg7 = sBattlePartyBgTemplateSub3;
    InitBgFromTemplate(work->bgConfig, 7, &bg7, 0);
    bg6 = sBattlePartyBgTemplateSub2;
    InitBgFromTemplate(work->bgConfig, 6, &bg6, 0);
    bg5 = sBattlePartyBgTemplateSub1;
    InitBgFromTemplate(work->bgConfig, 5, &bg5, 0);
    BgClearTilemapBufferAndCommit(work->bgConfig, 5);
    bg4 = sBattlePartyBgTemplateSub0;
    InitBgFromTemplate(work->bgConfig, 4, &bg4, 0);
    BgClearTilemapBufferAndCommit(work->bgConfig, 4);
    BG_ClearCharDataRange(5, 0x20, 0, (enum HeapID)(*(Ov08W04Root **)work)->heapId);
    BG_ClearCharDataRange(4, 0x20, 0, (enum HeapID)(*(Ov08W04Root **)work)->heapId);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 5);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 4);
}
void BattlePartyMenu_FreeBgs(void *bgConfig) {
    GfGfx_EngineBTogglePlanes(0x1F, 0);
    FreeBgTilemapBuffer(bgConfig, 4);
    FreeBgTilemapBuffer(bgConfig, 5);
    FreeBgTilemapBuffer(bgConfig, 6);
    FreeBgTilemapBuffer(bgConfig, 7);
}
