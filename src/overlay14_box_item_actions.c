#include "overlay14_box_item_actions_private.h"

#define FREE_RETURN(name, value)                 \
    u32 name(void *p) {                          \
        Heap_Free(PTR_AT(PTR_AT(p, 0x34), 0xc)); \
        return value;                            \
    }

#define TWO_CALLS(name, second) \
    void name(void *p) {        \
        PCBox_ClearMonInfoAndSelection(p);       \
        second(p);              \
    }

u32 ov14_021F1F44(void *p) {
    u32 v;
    if (PCBox_IsSummaryShortcutPressed(p) == 1) {
        PlaySE(0x5dd);
        U8_AT(p, 0x26) = 1;
        U8_AT(p, 0x27) = 1;
        U8_AT(p, 0x28) = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c));
        return ov14_021F2330(p, 0xf, 0x97);
    }
    v = ov14_021F7340(p);
    if (GridInputHandler_IsButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c)) == 0) {
        v = (u32)-2;
    }
    switch (v) {
    case (u32)-3:
        PlaySE(0x5dc);
        return PCBox_WaitForPendingUpdateFunc(p, 0x74);
    case (u32)-2:
    case 0x24:
        return ov14_021F15C8(p, 0xff);
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2a:
    default:
        return ov14_021F15C8(p, v);
    case (u32)-4:
    case (u32)-1:
        return 0x73;
    }
}

FREE_RETURN(ov14_021F2010, 0x73)

u32 PCBox_TakeHeldItemToBag(void *p) {
    int item;
    void *boxmon;
    item = PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 6, 0);
    if (Bag_AddItem(PTR_AT(p, 0xc), item, 1, 0xa) == 1) {
        boxmon = PCBox_GetSlotBoxMon(p, U8_AT(p, 0x1f), U8_AT(p, 0x21));
        PCBox_PrintTookItem(p, item, 0x25);
        item = 0;
        PCBox_SetSlotMonData(p, U8_AT(p, 0x21), 6, &item);
        if (PCBox_UpdateArceusGiratinaForm(boxmon) == 1) {
            PCBox_RefreshSlotIcon(p, U8_AT(p, 0x1f), U8_AT(p, 0x21), *((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21) + 0x4094));
        }
        PCBox_RedrawSlotMonInfo(p, U8_AT(p, 0x21));
    } else {
        PlaySE(0x5f3);
        PCBox_PrintBagFull(p, 0x25);
    }
    U32_AT(p, 0x30) = 0xe;
    return 6;
}

u32 PCBox_ConfirmRelease(void *p) {
    PCBox_HideSelectedIconOutline(p);
    PCBox_CloseMessageWindow(PTR_AT(p, 0x34), 0x25);
    PCBox_StartReleaseMoveCheck(p);
    PCBox_StartReleaseScale(PTR_AT(PTR_AT(p, 0x34), 0x88dc));
    ov14_021E884C(PTR_AT(p, 0x34));
    return 0x1a;
}

u32 PCBox_PutAwayHeldItem(void *p) {
    u16 v;
    if (Bag_AddItem(Save_Bag_Get(PTR_AT(PTR_AT(p, 0), 0)), U16_AT(PTR_AT(p, 0x34), 0x88c8), 1, 0xa) == 0) {
        PlaySE(0x5f3);
        PCBox_PrintItemActionMessage(p, 6, 0x25);
        U32_AT(p, 0x30) = 0x7a;
        return 6;
    }
    v = 0;
    PCBox_SetSlotMonData(p, U8_AT(p, 0x21), 6, &v);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(p, U8_AT(p, 0x1f), U8_AT(p, 0x21))) == 1) {
        PCBox_RefreshSlotIcon(p, U8_AT(p, 0x1f), U8_AT(p, 0x21), *((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21) + 0x4094));
    }
    PCBox_ShowSlotMonInfo(p, U8_AT(p, 0x21));
    PCBox_HideSelectedIconOutline(p);
    PCBox_CloseMessageWindow(PTR_AT(p, 0x34), 0x25);
    ov14_021F391C(PTR_AT(p, 0x34), 1);
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 0xb, 2);
    ov14_021E88F8(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return 0x79;
}

u32 PCBox_ConfirmExit(void *p) {
    PlaySE(0x60d);
    U32_AT(p, 0x30) = 0xb3;
    return PCBox_StartFadeOut(p, 1);
}

u32 PCBox_CancelExit(void *p) {
    PCBox_CloseMessageWindow(PTR_AT(p, 0x34), 0x25);
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
    switch (U32_AT(PTR_AT(p, 0), 8)) {
    case 0:
        PCBox_PrintMonActionMessage(p, 0, 0, 0x27);
        PCBox_SetIconsTranslucent(p, 2, 0);
        return 0x5b;
    case 1:
        PCBox_PrintMonActionMessage(p, 0, 0, 0x27);
        PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 1);
        PCBox_SetIconsTranslucent(p, 1, 0);
        return 0x51;
    case 2:
        PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 1);
        PCBox_SetIconsTranslucent(p, 1, 0);
        return 0xc;
    case 3:
        PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 1);
        PCBox_SetIconsTranslucent(p, 0x81, 0);
        return 0x75;
    }
    return 0xc;
}

u32 PCBox_FlashPanel(void *p, u32 a1, u32 a2) {
    s8 a;
    s8 b;
    u16 c;
    u16 d;
    BgCellManager_GetCellPosition(PTR_AT(PTR_AT(p, 0x34), 0x2f0), a1, &a, &b);
    BgCellManager_GetCellSize(PTR_AT(PTR_AT(p, 0x34), 0x2f0), a1, &c, &d);
    BF17(p, 0x88d4).lo = 1;
    BF17(p, 0x88d4).hi = BgCellManager_GetCellBgId(PTR_AT(PTR_AT(p, 0x34), 0x2f0), a1);
    BF44(p, 0x88d5).lo = 0xd;
    BF44(p, 0x88d5).hi = 0xc;
    WQ8(p, 0x88d6) = 0;
    WQ8(p, 0x88d7) = 0;
    WQ8(p, 0x88d8) = a;
    WQ8(p, 0x88d9) = b;
    WQ8(p, 0x88da) = c;
    WQ8(p, 0x88db) = d;
    U32_AT(p, 0x30) = a2;
    return 8;
}

u32 ov14_021F2330(void *p, u32 a1, u32 a2) {
    s8 a;
    s8 b;
    u16 c;
    u16 d;
    BgCellManager_GetCellPosition(PTR_AT(PTR_AT(p, 0x34), 0x2f0), a1, &a, &b);
    BgCellManager_GetCellSize(PTR_AT(PTR_AT(p, 0x34), 0x2f0), a1, &c, &d);
    BF17(p, 0x88d4).lo = 1;
    BF17(p, 0x88d4).hi = BgCellManager_GetCellBgId(PTR_AT(PTR_AT(p, 0x34), 0x2f0), a1);
    BF44(p, 0x88d5).lo = 3;
    BF44(p, 0x88d5).hi = 2;
    WQ8(p, 0x88d6) = 0;
    WQ8(p, 0x88d7) = 0;
    WQ8(p, 0x88d8) = a;
    WQ8(p, 0x88d9) = b;
    WQ8(p, 0x88da) = c;
    WQ8(p, 0x88db) = d;
    U32_AT(p, 0x30) = a2;
    return 8;
}

u32 ov14_021F23F0(void *p, u32 a1, u32 a2) {
    BF17(p, 0x88d4).lo = 1;
    BF17(p, 0x88d4).hi = BgCellManager_GetCellBgId(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 2);
    BF44(p, 0x88d5).lo = 3;
    BF44(p, 0x88d5).hi = 2;
    WQ8(p, 0x88d6) = 0;
    WQ8(p, 0x88d7) = 0;
    if (a1 == 0) {
        WQ8(p, 0x88d9) = 0x10;
    } else {
        WQ8(p, 0x88d9) = 0x14;
    }
    WQ8(p, 0x88d8) = 0x16;
    WQ8(p, 0x88da) = 9;
    WQ8(p, 0x88db) = 4;
    U32_AT(p, 0x30) = a2;
    return 8;
}

u32 ov14_021F2490(void *p, u32 a1, u32 a2) {
    BF17(p, 0x88d4).lo = 1;
    BF17(p, 0x88d4).hi = BgCellManager_GetCellBgId(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 1);
    BF44(p, 0x88d5).lo = 3;
    BF44(p, 0x88d5).hi = 2;
    WQ8(p, 0x88d6) = 0;
    WQ8(p, 0x88d7) = 0;
    if (a1 == 0) {
        WQ8(p, 0x88d8) = 3;
    } else {
        WQ8(p, 0x88d8) = 0x16;
    }
    WQ8(p, 0x88d9) = 0x14;
    WQ8(p, 0x88da) = 9;
    WQ8(p, 0x88db) = 4;
    U32_AT(p, 0x30) = a2;
    return 8;
}

void ov14_021F2534(void *p) {
    switch (U32_AT(PTR_AT(p, 0), 8)) {
    case 0:
        PCBox_SetIconsTranslucent(p, 2, 1);
        break;
    case 1:
    case 2:
    case 3:
        PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 0);
        PCBox_SetIconsTranslucent(p, 1, 1);
        break;
    }
}

u32 ov14_021F2574(void *p) {
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    PCBox_PrintExitPrompt(p, 0x25, 1);
    ov14_021F2534(p);
    return PCBox_ShowYesNo(p, 3);
}

u32 ov14_021F259C(void *p) {
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    PCBox_PrintExitPrompt(p, 0x25, 0);
    ov14_021F2534(p);
    return PCBox_ShowYesNo(p, 4);
}

TWO_CALLS(ov14_021F25C4, ov14_021F0AD8)

TWO_CALLS(ov14_021F25D4, ov14_021F0C88)

u32 ov14_021F25E4(void *p) {
    PCBox_SetSelectedIconOutlineVisible(p, 0);
    U8_AT(p, 0x24) = 1;
    U8_AT(p, 0x29) = 0;
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    return ov14_021F1100(p, 0x2f);
}
