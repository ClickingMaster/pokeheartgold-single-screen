#include "to56_overlay08_bank12_private.h"
void BattlePartyMenu_ConsumeItem(void *battleSystem, u16 item, u8 pocket, enum HeapID heapId) {
    if (item != 0x41 && item != 0x43 && item != 0x42) {
        Bag_TakeItem(BattleSystem_GetBag(battleSystem), item, 1, heapId);
    }
    BagCursor_Battle_SetLastUsedItem(BattleSystem_GetBagCursor(battleSystem), item, pocket);
}
void BattlePartyMenu_AddWindows(Ov08Bank12Work *work) {
    u32 i;
    for (i = 0; i < 2; i++) {
        AddWindow(work->bg, &work->windows[i], &sBattlePartyWindowTemplates[i]);
    }
    BattlePartyMenu_AddPageWindows(work, work->windowType);
}
void BattlePartyMenu_AddPageWindows(Ov08Bank12Work *work, u8 type) {
    const WindowTemplate *templates;
    u8 i;
    switch (type) {
    case 0:
        templates = sBattlePartyPartyListWindows;
        work->dynamicCount = 6;
        break;
    case 1:
        templates = sBattlePartyMonMenuWindows;
        work->dynamicCount = 4;
        break;
    case 2:
        templates = sBattlePartySummaryWindows;
        work->dynamicCount = 0x23;
        break;
    case 3:
        templates = sBattlePartyCheckMovesWindows;
        work->dynamicCount = 0xB;
        break;
    case 4:
        templates = sBattlePartyMoveDetailWindows;
        work->dynamicCount = 0x11;
        break;
    case 5:
        templates = sBattlePartyRestorePPWindows;
        work->dynamicCount = 5;
        break;
    case 6:
    case 8:
        templates = sBattlePartyLearnMoveWindows;
        work->dynamicCount = 6;
        break;
    case 7:
        templates = sBattlePartyForgetMoveWindows;
        work->dynamicCount = 0xC;
        break;
    case 9:
        templates = sBattlePartyContestForgetMoveWindows;
        work->dynamicCount = 7;
        break;
    }
    work->dynamicWindows = AllocWindows(work->owner->heapId, work->dynamicCount);
    for (i = 0; i < work->dynamicCount; i++) {
        AddWindow(work->bg, &work->dynamicWindows[i], &templates[i]);
    }
}
