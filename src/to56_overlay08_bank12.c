#include "to56_overlay08_bank12_private.h"
void ov08_0221DBCC(void *battleSystem, u16 item, u8 pocket, enum HeapID heapId) {
    if (item != 0x41 && item != 0x43 && item != 0x42) {
        Bag_TakeItem(BattleSystem_GetBag(battleSystem), item, 1, heapId);
    }
    BagCursor_Battle_SetLastUsedItem(BattleSystem_GetBagCursor(battleSystem), item, pocket);
}
void ov08_0221DC00(Ov08Bank12Work *work) {
    u32 i;
    for (i = 0; i < 2; i++) {
        AddWindow(work->bg, &work->windows[i], &ov08_02224FD0[i]);
    }
    ov08_0221DC3C(work, work->windowType);
}
void ov08_0221DC3C(Ov08Bank12Work *work, u8 type) {
    const WindowTemplate *templates;
    u8 i;
    switch (type) {
    case 0:
        templates = ov08_02225084;
        work->dynamicCount = 6;
        break;
    case 1:
        templates = ov08_0222500C;
        work->dynamicCount = 4;
        break;
    case 2:
        templates = ov08_0222522C;
        work->dynamicCount = 0x23;
        break;
    case 3:
        templates = ov08_022250EC;
        work->dynamicCount = 0xB;
        break;
    case 4:
        templates = ov08_022251A4;
        work->dynamicCount = 0x11;
        break;
    case 5:
        templates = ov08_0222502C;
        work->dynamicCount = 5;
        break;
    case 6:
    case 8:
        templates = ov08_02225054;
        work->dynamicCount = 6;
        break;
    case 7:
        templates = ov08_02225144;
        work->dynamicCount = 0xC;
        break;
    case 9:
        templates = ov08_022250B4;
        work->dynamicCount = 7;
        break;
    }
    work->dynamicWindows = AllocWindows(work->owner->heapId, work->dynamicCount);
    for (i = 0; i < work->dynamicCount; i++) {
        AddWindow(work->bg, &work->dynamicWindows[i], &templates[i]);
    }
}
