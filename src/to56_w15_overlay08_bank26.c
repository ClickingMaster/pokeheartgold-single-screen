#include "global.h"

#include "bg_window.h"

#pragma require_prototypes off

typedef struct Ov08Bank26Work {
    u8 padding00[4];
    BgConfig *bgConfig;
    u8 padding08[0x1144];
    u8 state;
    u8 paletteIndex;
} Ov08Bank26Work;

void BattleBagMenu_RemovePageWindows(Ov08Bank26Work *work);
void BattleBagMenu_AddPageWindows(Ov08Bank26Work *work, int state);
void BattleBagMenu_PrintPageText(Ov08Bank26Work *work, int state);
void BattleBagMenu_DrawPageButtons(Ov08Bank26Work *work, int state);
void BattleBagMenu_SetPageCursor(Ov08Bank26Work *work, int state);
void BattleBagMenu_UpdateTutorialFinger(Ov08Bank26Work *work, int state);
void BattleBagMenu_ShowPageSprites(Ov08Bank26Work *work, int state);

void BattleBagMenu_SetPageBgOffset(Ov08Bank26Work *work, int state) {
    switch (state) {
    case 0:
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_Y, 0);
        break;
    case 1:
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_X, 0x100);
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_Y, 0);
        break;
    case 2:
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(work->bgConfig, 6, BG_POS_OP_SET_Y, 0x100);
        break;
    }
}

void BattleBagMenu_SetUseItemPagePalette(Ov08Bank26Work *work, int state) {
    if (state == 2) {
        BgTilemapRectChangePalette(work->bgConfig, 6, 2, 0x23, 0x1C, 4, work->paletteIndex + 8);
        BgTilemapRectChangePalette(work->bgConfig, 6, 2, 0x28, 0x1C, 8, work->paletteIndex + 8);
    }
}

void BattleBagMenu_ChangePage(Ov08Bank26Work *work, int state) {
    BattleBagMenu_SetUseItemPagePalette(work, state);
    BattleBagMenu_SetPageBgOffset(work, state);
    BgFillTilemapBufferAndSchedule(work->bgConfig, 4, 0);
    BgFillTilemapBufferAndSchedule(work->bgConfig, 5, 0);
    BattleBagMenu_RemovePageWindows(work);
    BattleBagMenu_AddPageWindows(work, state);
    BattleBagMenu_PrintPageText(work, state);
    BattleBagMenu_DrawPageButtons(work, state);
    BattleBagMenu_SetPageCursor(work, state);
    BattleBagMenu_UpdateTutorialFinger(work, state);
    work->state = state;
    BattleBagMenu_ShowPageSprites(work, work->state);
}
