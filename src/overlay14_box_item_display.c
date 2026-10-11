#include "overlay14_box_item_display_private.h"

void PCBox_DrawMonInfo(void *p, void *mon) {
    u32 base = ov14_021F5404(p, mon);
    u32 i;

    for (i = 0; i < 4; i++) {
        BgCellManager_PutWindowTiles(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0x10, (u8 *)PTR_AT(p, 0x34) + 0x30 + (base + i) * 0x10);
    }
    ov14_021E88A4(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_ShowMonPic(p, mon, 2);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), (U16_AT(PTR_AT(p, 0x34), 0x88d0) ^ 1) + 2, 1);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), U16_AT(PTR_AT(p, 0x34), 0x88d0) + 2, 0);
    PCBox_DrawTypeIcons(PTR_AT(p, 0x34), mon);
    PCBox_PrintMonInfo(p, mon);
    PCBox_DrawMarkings(p, U16_AT(mon, 0x10));
}

void PCBox_DrawMonItemInfo(void *p, void *mon) {
    u32 base;

    if (U16_AT(mon, 6) != 0) {
        base = PCBox_PrintItemNameAndDesc(p, U16_AT(mon, 6));
        BgCellManager_PutWindowTiles(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0x10, (u8 *)PTR_AT(p, 0x34) + 0x30 + base * 0x10);
        BgCellManager_PutWindowTiles(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0x10, (u8 *)PTR_AT(p, 0x34) + 0x30 + (base + 1) * 0x10);
        PCBox_LoadInfoItemIcon(PTR_AT(p, 0x34), U16_AT(mon, 6));
    }
    ov14_021E8944(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_ShowMonPic(p, mon, 2);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), (U16_AT(PTR_AT(p, 0x34), 0x88d0) ^ 1) + 2, 1);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), U16_AT(PTR_AT(p, 0x34), 0x88d0) + 2, 0);
    PCBox_DrawTypeIcons(PTR_AT(p, 0x34), mon);
    PCBox_PrintMonInfo(p, mon);
    PCBox_DrawMarkings(p, U16_AT(mon, 0x10));
}

BOOL PCBox_ShowSlotMonInfo(void *p, u32 index) {
    void *view;
    u32 lock;
    void *boxMon = PCBox_GetSlotBoxMon(p, U8_AT(p, 0x1f), index);

    if (boxMon != NULL) {
        lock = AcquireBoxMonLock(boxMon);
        view = PCBox_CreateMonInfo(boxMon);
        if (view != NULL) {
            if (U32_AT(PTR_AT(p, 0), 8) == 3) {
                PCBox_DrawMonItemInfo(p, view);
            } else {
                PCBox_DrawMonInfo(p, view);
            }
            PCBox_FreeMonInfo(view);
        } else {
            PCBox_ClearMonInfo(p);
            ReleaseBoxMonLock(boxMon, lock);
            return FALSE;
        }
        ReleaseBoxMonLock(boxMon, lock);
    } else {
        PCBox_ClearMonInfo(p);
        return FALSE;
    }
    return TRUE;
}

void PCBox_RedrawSlotMonInfo(void *p, u32 index) {
    void *boxMon = PCBox_GetSlotBoxMon(p, U8_AT(p, 0x1f), index);
    u32 lock = AcquireBoxMonLock(boxMon);
    void *view = PCBox_CreateMonInfo(boxMon);

    PCBox_ShowMonPic(p, view, 2);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), (U16_AT(PTR_AT(p, 0x34), 0x88d0) ^ 1) + 2, 1);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), U16_AT(PTR_AT(p, 0x34), 0x88d0) + 2, 0);
    PCBox_DrawTypeIcons(PTR_AT(p, 0x34), view);
    PCBox_PrintMonInfo(p, view);
    PCBox_FreeMonInfo(view);
    ReleaseBoxMonLock(boxMon, lock);
}

void PCBox_ClearMonInfo(void *p) {
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 2, 0);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 3, 0);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 0xd, 0);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 0xe, 0);
    PCBox_ClearMonInfoWindows(PTR_AT(p, 0x34));
    PCBox_DrawMarkings(p, 0);
    if (U32_AT(PTR_AT(p, 0), 8) == 3) {
        ov14_021E8944(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    } else {
        ov14_021E88A4(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
}
