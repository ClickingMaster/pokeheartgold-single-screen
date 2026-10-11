#include "overlay14_box_actions_internal.h"

u32 PCBox_ScrollToPrevBox(void *p, u32 next);
u32 PCBox_ScrollToNextBox(void *p, u32 next);
u32 PCBox_StartMoveModeBoxDrag(void *p, u32 idx);
u32 PCBox_StartWithdrawDrag(void *p, u32 idx);
u32 ov14_021F04D4(void *p, u32 idx);
u32 ov14_021F0530(void *p, u32 idx);
u32 PCBox_StartMoveModePartyDrag(void *p, u32 idx);
u32 PCBox_StartDepositDrag(void *p, u32 idx);
u32 ov14_021F0794(void *p, u32 idx);
u32 ov14_021F07F0(void *p, u32 idx);
u32 PCBox_StartBoxTrayDrag(void *p, u32 idx);
void PCBox_AllocMoveWork(void *p);
void PCBox_FreeMoveWork(void *p);
u32 PCBox_HandleItemCommand(void *p);
u32 ov14_021F09BC(void *p);
u32 ov14_021F0A80(void *p);
u32 ov14_021F0AAC(void *p);
u32 ov14_021F0AD8(void *p);
u32 ov14_021F0B34(void *p);
void ov14_021F0B70(void *p);
void ov14_021F0BB4(void *p);
void ov14_021F0BF4(void *p);
u32 ov14_021F0C0C(void *p);
u32 ov14_021F0C58(void *p);
u32 ov14_021F0C88(void *p);
u32 ov14_021F0CD8(void *p);
u32 PCBox_HideInfoPanelAndWait(void *p, u32 next);
u32 ov14_021F0D58(void *p, s32 delta);
u32 ov14_021F0EE8(void *p, u32 next);
u32 PCBox_JumpToTrayBox(void *p, u32 which);
void PCBox_ChangeTrayPage(void *p, s32 delta);
u32 PCBox_OpenBoxTray(void *p, u32 next);
u32 PCBox_CloseBoxTray(void *p, u32 next);
u32 ov14_021F10B4(void *p, u32 next);
u32 ov14_021F10DC(void *p, u32 next);
u32 ov14_021F1100(void *p, u32 next);
void ov14_021F1128(void *p);
u32 ov14_021F1170(void *p, s32 delta);
u32 ov14_021F11F8(void *p, s32 delta);
void PCBox_ChangeWallpaperPage(void *p, s32 delta);
void PCBox_SelectWallpaperInPage(void *p, u32 delta);
u32 PCBox_ConfirmWallpaperChoice(void *p);
u32 PCBox_TryWithdraw(void *p);
u32 ov14_021F1414(void *p);
u32 ov14_021F1448(void *p, s32 delta);
u32 ov14_021F1504(void *p, s32 delta);

u32 PCBox_ScrollToPrevBox(void *p, u32 next) {
    U8_AT(p, 0x1f) = (U8_AT(p, 0x1f) == 0) ? 0x11 : U8_AT(p, 0x1f) - 1;
    PCBox_LoadBoxIconImages(p, U8_AT(p, 0x1f));
    PCBox_LoadScrolledWallpaper(p, PCBox_GetWallpaperGfxIndex(p, U8_AT(p, 0x1f)), 0);
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 0, 2);
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        if (U32_AT(PTR_AT(p, 0), 8) == 2 || U32_AT(PTR_AT(p, 0), 8) == 3) {
            PCBox_SlideInPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            ov14_021E82A8(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
    }
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateScrollToPrevBox, next);
}

u32 PCBox_ScrollToNextBox(void *p, u32 next) {
    U8_AT(p, 0x1f) = (U8_AT(p, 0x1f) == 0x11) ? 0 : U8_AT(p, 0x1f) + 1;
    PCBox_LoadBoxIconImages(p, U8_AT(p, 0x1f));
    PCBox_LoadScrolledWallpaper(p, PCBox_GetWallpaperGfxIndex(p, U8_AT(p, 0x1f)), 1);
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 1, 4);
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        if (U32_AT(PTR_AT(p, 0), 8) == 2 || U32_AT(PTR_AT(p, 0), 8) == 3) {
            PCBox_SlideInPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            ov14_021E82A8(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
    }
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateScrollToNextBox, next);
}

u32 PCBox_StartMoveModeBoxDrag(void *p, u32 idx) {
    U8_AT(p, 0x21) = idx;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
    PCBox_CreateSelectedIconOutline(p);
    PCBox_AllocMoveWork(p);
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
        PCBox_SlideOutPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        ov14_021E8294(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    U8_AT(p, 0x22) = 0;
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateMoveModeBoxDrag, 0xd);
}

u32 PCBox_StartWithdrawDrag(void *p, u32 idx) {
    U8_AT(p, 0x21) = idx;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
    PCBox_CreateSelectedIconOutline(p);
    ov14_021F3044(p);
    PCBox_SetIconsTranslucent(p, 1, 1);
    PCBox_SetIconsTranslucent(p, 2, 1);
    PCBox_SetSlotIconTranslucent(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
    PCBox_PrintMonActionMessage(p, U8_AT(p, 0x21), 1, 0x27);
    if (Party_GetCount(PTR_AT(p, 8)) != 6) {
        PCBox_PrintPanelHint(p, 0x28, 1);
    } else {
        PCBox_PrintPanelHint(p, 0x28, 3);
    }
    PCBox_AllocMoveWork(p);
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
        ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    U8_AT(p, 0x22) = 2;
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateWithdrawDrag, 0x57);
}

u32 ov14_021F04D4(void *p, u32 idx) {
    U8_AT(p, 0x21) = idx;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 1);
    PCBox_CreateSelectedIconOutline(p);
    PCBox_PrintMonActionMessage(p, U8_AT(p, 0x21), 1, 0x27);
    PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8824(PTR_AT(p, 0x34));
    return PCBox_WaitForUpdateFunc(p, ov14_021EA068, 0x58);
}

u32 ov14_021F0530(void *p, u32 idx) {
    U8_AT(p, 0x21) = idx;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 1);
    PCBox_CreateSelectedIconOutline(p);
    PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_SlideOutPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8294(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8824(PTR_AT(p, 0x34));
    return PCBox_WaitForUpdateFunc(p, ov14_021EA0B8, 0x4a);
}

u32 PCBox_StartMoveModePartyDrag(void *p, u32 idx) {
    void *mon;
    U8_AT(p, 0x21) = idx;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
    PCBox_CreateSelectedIconOutline(p);
    mon = Party_GetMonByIndex(PTR_AT(p, 8), idx - 0x1e);
    if (PCBox_PartyHasOtherUsableMon(p, idx - 0x1e) == 0 && PCStorage_CountMonsInBox(PTR_AT(p, 4), U8_AT(p, 0x1f)) == 0) {
        PCBox_PrintPanelHint(p, 0x28, 8);
    } else if (ItemIdIsMail(GetMonData(mon, 6, NULL)) == 1) {
        PCBox_PrintPanelHint(p, 0x28, 6);
    } else if (GetMonData(mon, 0xa2, NULL) != 0) {
        PCBox_PrintPanelHint(p, 0x28, 7);
    } else {
        PCBox_PrintPanelHint(p, 0x28, 0);
    }
    PCBox_AllocMoveWork(p);
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    U8_AT(p, 0x22) = 1;
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateMoveModePartyDrag, 0x27);
}

u32 PCBox_StartDepositDrag(void *p, u32 idx) {
    void *mon;
    u32 i;
    U8_AT(p, 0x21) = idx;
    for (i = 0x1e; i < 0x24; i++) {
        if (i == U8_AT(p, 0x21)) {
            PCBox_SetIconLayer(PTR_AT(p, 0x34), i, 0);
        } else {
            PCBox_SetIconLayer(PTR_AT(p, 0x34), i, 1);
        }
    }
    PCBox_CreateSelectedIconOutline(p);
    PCBox_SetIconsTranslucent(p, 2, 1);
    PCBox_SetSlotIconTranslucent(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
    PCBox_PrintMonActionMessage(p, U8_AT(p, 0x21), 1, 0x27);
    mon = Party_GetMonByIndex(PTR_AT(p, 8), idx - 0x1e);
    if (PCBox_PartyHasOtherUsableMon(p, idx - 0x1e) == 0) {
        PCBox_PrintPanelHint(p, 0x28, 8);
    } else if (ItemIdIsMail(GetMonData(mon, 6, NULL)) == 1) {
        PCBox_PrintPanelHint(p, 0x28, 6);
    } else if (GetMonData(mon, 0xa2, NULL) != 0) {
        PCBox_PrintPanelHint(p, 0x28, 7);
    } else if (PCStorage_CountEmptySpotsInBox(PTR_AT(p, 4), U8_AT(p, 0x1f)) == 0) {
        PCBox_PrintPanelHint(p, 0x28, 2);
    } else {
        PCBox_PrintPanelHint(p, 0x28, 0);
    }
    PCBox_AllocMoveWork(p);
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
        ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    U8_AT(p, 0x22) = 1;
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateDepositDrag, 0x6e);
}

u32 ov14_021F0794(void *p, u32 idx) {
    U8_AT(p, 0x21) = idx;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 1);
    PCBox_CreateSelectedIconOutline(p);
    PCBox_PrintMonActionMessage(p, U8_AT(p, 0x21), 1, 0x27);
    PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8824(PTR_AT(p, 0x34));
    return PCBox_WaitForUpdateFunc(p, ov14_021EA068, 0x6f);
}

u32 ov14_021F07F0(void *p, u32 idx) {
    U8_AT(p, 0x21) = idx;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 1);
    PCBox_CreateSelectedIconOutline(p);
    PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8824(PTR_AT(p, 0x34));
    return PCBox_WaitForUpdateFunc(p, ov14_021EA0B8, 0x4c);
}

u32 PCBox_StartBoxTrayDrag(void *p, u32 idx) {
    U8_AT(p, 0x21) = idx;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
    PCBox_CreateSelectedIconOutline(p);
    if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutChangeBoxButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    } else if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    ov14_021E8824(PTR_AT(p, 0x34));
    PCBox_DrawSummaryButton(p, 0);
    PCBox_AllocMoveWork(p);
    U8_AT(p, 0x22) = 2;
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateBoxTrayDrag, 0x2a);
}

void PCBox_AllocMoveWork(void *p) {
    void *buf;
    u32 i;
    void *cur;
    buf = Heap_Alloc(0xa, 0xf0);
    for (i = 0, cur = buf; i < 7; i++, cur = (u8 *)cur + 0x20) {
        PTR_AT(cur, 0) = AllocMonZeroed(0xa);
    }
    U32_AT(buf, 0xe0) = 0;
    PTR_AT(PTR_AT(p, 0x34), 0xc) = buf;
}

void PCBox_FreeMoveWork(void *p) {
    void *buf;
    u32 i;
    void *cur;
    buf = PTR_AT(PTR_AT(p, 0x34), 0xc);
    cur = buf;
    for (i = 0; i < 7; i++, cur = (u8 *)cur + 0x20) {
        Heap_Free(PTR_AT(cur, 0));
    }
    Heap_Free(buf);
}

u32 PCBox_HandleItemCommand(void *p) {
    u32 item;
    if (PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 0x4c, 0) != 0) {
        PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
        PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        return PCBox_WaitForUpdateFunc(p, ov14_021E9450, 0x4f);
    }
    item = PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 6, 0);
    if (item == 0) {
        U32_AT(p, 0x2c) = 0x26;
        return PCBox_StartSubApp(p, 1);
    }
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    if (ItemIdIsMail(item) == 1) {
        PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        return PCBox_WaitForUpdateFunc(p, ov14_021E9450, 0x50);
    }
    ov14_021E83F4(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_PrintTakeItemPrompt(p, item, 0x25);
    return PCBox_ShowYesNo(p, 0);
}

u32 ov14_021F09BC(void *p) {
    switch (U32_AT(PTR_AT(p, 0), 8)) {
    case 0:
        PCBox_CreateGridInput(p, 0, 9);
        break;
    case 1:
        PCBox_CreateGridInput(p, 2, 0x24);
        break;
    case 2:
    default:
        if (U8_AT(p, 0x21) < 0x1e) {
            PCBox_CreateGridInput(p, 3, 0x27);
        } else {
            PCBox_CreateGridInput(p, 5, 0xb);
        }
        break;
    }
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    PCBox_CloseMessageWindow(PTR_AT(p, 0x34), 0x26);
    if (U8_AT(p, 0x24) != 0) {
        PCBox_DrawTrayBoxNameAndCount(p);
    }
    if (U8_AT(p, 0x21) < 0x1e) {
        PCBox_SetIconsTranslucent(p, 1, 0);
    }
    PCBox_SetIconsTranslucent(p, 2, 0);
    PCBox_SlideOutMarkingMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    if (U32_AT(PTR_AT(p, 0), 8) == 2 && U8_AT(p, 0x21) < 0x1e) {
        return PCBox_WaitForUpdateFunc(p, PCBox_UpdateMarkingMenuSlide, 0x17);
    }
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateMarkingMenuSlide, 0xe);
}

u32 ov14_021F0A80(void *p) {
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E9434, 0x13);
}

u32 ov14_021F0AAC(void *p) {
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E9450, 0x19);
}

u32 ov14_021F0AD8(void *p) {
    U8_AT(p, 0x23) = 1;
    PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 0);
    PCBox_SetIconsTranslucent(p, 1, 1);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    PCBox_SlideOutPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8294(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E94BC, 0x22);
}

u32 ov14_021F0B34(void *p) {
    U8_AT(p, 0x23) = 0;
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
        return ov14_021EC710(p);
    }
    PCBox_ClearMonInfoAndSelection(p);
    return ov14_021F1100(p, 0x25);
}

void ov14_021F0B70(void *p) {
    ov14_021E81A8(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 1);
    ov14_021F63F0(PTR_AT(p, 0x34));
    ov14_021F63A8(PTR_AT(p, 0x34));
    ov14_021E8368(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E82DC(p);
    ov14_021E7E78(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021F30B0(p);
}

void ov14_021F0BB4(void *p) {
    ov14_021E81A8(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 1);
    ov14_021F63F0(PTR_AT(p, 0x34));
    ov14_021F63B8(PTR_AT(p, 0x34));
    ov14_021E8368(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E7E88(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021F311C(p);
}

void ov14_021F0BF4(void *p) {
    ov14_021F30B0(p);
    ov14_021E7E78(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
}

u32 ov14_021F0C0C(void *p) {
    U8_AT(p, 0x21) = 0xff;
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    ov14_021E8020(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    return PCBox_WaitForUpdateFunc(p, ov14_021E952C, 0x2d);
}

u32 ov14_021F0C58(void *p) {
    U8_AT(p, 0x21) = 0xff;
    PCBox_SetIconsTranslucent(p, 1, 1);
    ov14_021E805C(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E954C, 0x2e);
}

u32 ov14_021F0C88(void *p) {
    U8_AT(p, 0x24) = 1;
    U8_AT(p, 0x29) = 0;
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    PCBox_SlideOutPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8294(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E94BC, 0x2f);
}

u32 ov14_021F0CD8(void *p) {
    U8_AT(p, 0x24) = 0;
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        return ov14_021F0EE8(p, 0x31);
    }
    if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        return PCBox_HideInfoPanelAndWait(p, 0x31);
    }
    return PCBox_CloseBoxTray(p, 0x32);
}

u32 PCBox_HideInfoPanelAndWait(void *p, u32 next) {
    PCBox_SlideOutSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E9970, next);
}

u32 ov14_021F0D58(void *p, s32 delta) {
    u32 st;
    s32 cur;
    s32 nv;
    u8 x;
    u8 y;
    st = (U32_AT(PTR_AT(p, 0), 8) == 3) ? 0x82 : 0x29;
    cur = U8_AT(p, 0x25);
    nv = delta + cur / 6 * 6;
    if (nv != cur) {
        U8_AT(p, 0x25) = nv;
        PCBox_UpdateTrayCursor(p);
        PCBox_DrawTrayBoxNameAndCount(p);
    }
    if (U8_AT(p, 0x25) == U8_AT(p, 0x1f)) {
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
            GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
            PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 0xe);
            return ov14_021F0EE8(p, st);
        }
        if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
            GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
            PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 0xe);
            PCBox_SlideOutSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            return PCBox_WaitForUpdateFunc(p, ov14_021E9970, st);
        }
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 0xe);
        return st;
    }
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), 0x2d);
    if (GridInputHandler_IsButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c)) == 1) {
        DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(PTR_AT(PTR_AT(p, 0x34), 0x2c), 0x2d), &x, &y);
        ManagedSprite_SetPositionXY(PTR_AT(PTR_AT(p, 0x34), 0x320), x, y);
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
        PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 1);
    }
    if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        return st;
    }
    PCBox_DrawChangeBoxButton(p);
    if (PCBox_IsSummaryButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
        PCBox_SlideOutSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    return PCBox_WaitForUpdateFunc(p, ov14_021E9920, st);
}

u32 ov14_021F0EE8(void *p, u32 next) {
    PCBox_SlideOutChangeBoxButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E9970, next);
}

u32 PCBox_JumpToTrayBox(void *p, u32 which) {
    void *cb;
    u32 prev;
    prev = U8_AT(p, 0x1f);
    U8_AT(p, 0x1f) = U8_AT(p, 0x25);
    if (prev > U8_AT(p, 0x25)) {
        PCBox_LoadBoxIconImages(p, U8_AT(p, 0x1f));
        PCBox_LoadScrolledWallpaper(p, PCBox_GetWallpaperGfxIndex(p, U8_AT(p, 0x1f)), 0);
        cb = PCBox_UpdateScrollToPrevBox;
    } else {
        PCBox_LoadBoxIconImages(p, U8_AT(p, 0x1f));
        PCBox_LoadScrolledWallpaper(p, PCBox_GetWallpaperGfxIndex(p, U8_AT(p, 0x1f)), 1);
        cb = PCBox_UpdateScrollToNextBox;
    }
    switch (which) {
    case 0:
        PCBox_UpdateCurrentBoxMarker(p);
        if (U8_AT(p, 0x23) == 0) {
            U32_AT(p, 0x30) = 0xc;
        } else {
            U32_AT(p, 0x30) = 0x24;
        }
        break;
    case 1:
        PCBox_UpdateCurrentBoxMarker(p);
        U32_AT(p, 0x30) = 0x3d;
        break;
    case 2:
        U32_AT(p, 0x30) = 0x47;
        break;
    case 3:
        PCBox_UpdateCurrentBoxMarker(p);
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            PCBox_SlideOutChangeBoxButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
        U32_AT(p, 0x30) = 0x29;
        break;
    case 4:
        PCBox_UpdateCurrentBoxMarker(p);
        if (PCBox_IsChangeBoxButtonShown(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            PCBox_SlideOutChangeBoxButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
        U32_AT(p, 0x30) = 0x82;
        break;
    }
    return PCBox_WaitForUpdateFunc(p, cb, U32_AT(p, 0x30));
}

void PCBox_ChangeTrayPage(void *p, s32 delta) {
    s32 v;
    v = U8_AT(p, 0x25) + delta * 6;
    if (v < 0) {
        v += 0x12;
    } else if (v >= 0x12) {
        v -= 0x12;
    }
    U8_AT(p, 0x25) = v;
    PCBox_LoadTrayPageThumbnails(p);
    PCBox_UpdateTrayCursor(p);
    PCBox_UpdateCurrentBoxMarker(p);
    PCBox_DrawTrayBoxNameAndCount(p);
    if (delta > 0) {
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 5, 4);
    } else {
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 4, 2);
    }
}

u32 PCBox_OpenBoxTray(void *p, u32 next) {
    PCBox_SetupBoxTraySprites(p);
    PCBox_UpdateCurrentBoxMarker(p);
    PCBox_UpdateTrayCursor(p);
    PCBox_DrawTrayBoxNameAndCount(p);
    PCBox_SlideInBoxTray(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateBoxTraySlideIn, next);
}

u32 PCBox_CloseBoxTray(void *p, u32 next) {
    PCBox_SlideOutBoxTray(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, PCBox_UpdateBoxTraySlideOut, next);
}

u32 ov14_021F10B4(void *p, u32 next) {
    PCBox_DrawExitButton(p, 0);
    ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E95B4, next);
}

u32 ov14_021F10DC(void *p, u32 next) {
    ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E95B4, next);
}

u32 ov14_021F1100(void *p, u32 next) {
    U8_AT(p, 0x21) = 0xff;
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E9434, next);
}

void ov14_021F1128(void *p) {
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    U32_AT(PTR_AT(p, 0x34), 0x43c) = U8_AT(p, 0x1f) % 6;
    U32_AT(p, 0x2c) = U32_AT(PTR_AT(p, 0x34), 0x43c);
    if (PCBox_IsActionMenuOpen(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
        ov14_021ED5B0(p);
    } else {
        ov14_021F1100(p, 0x35);
    }
}

u32 ov14_021F1170(void *p, s32 delta) {
    s32 v;
    u8 x;
    u8 y;
    U8_AT(p, 0x25) = delta + U8_AT(p, 0x25) / 6 * 6;
    PCBox_UpdateTrayCursor(p);
    PCBox_DrawTrayBoxNameAndCount(p);
    v = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c));
    U32_AT(PTR_AT(p, 0x34), 0x43c) = v;
    GridInputHandler_SetNextLastUnk0FInputs(PTR_AT(PTR_AT(p, 0x34), 0x2c), 8, v, v);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(PTR_AT(PTR_AT(p, 0x34), 0x2c), 8), &x, &y);
    ManagedSprite_SetPositionXY(PTR_AT(PTR_AT(p, 0x34), 0x320), x, y);
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 1);
    return 0x3d;
}

u32 ov14_021F11F8(void *p, s32 delta) {
    PCBox_ChangeTrayPage(p, delta);
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6));
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
    return 0x3d;
}

void PCBox_ChangeWallpaperPage(void *p, s32 delta) {
    s32 v;
    v = U8_AT(PTR_AT(p, 0x34), 0x44d) + delta * 4;
    if (v < 0) {
        v += 0x18;
    } else if (v >= 0x18) {
        v -= 0x18;
    }
    U8_AT(PTR_AT(p, 0x34), 0x44d) = v;
    if (delta > 0) {
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 5, 4);
    } else {
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 4, 2);
    }
    PCBox_LoadWallpaperThumbnails(p);
    PCBox_UpdateCurrentWallpaperMarker(p);
    PCBox_DrawWallpaperName(p);
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(PTR_AT(p, 0x34), 0x44d) % 4));
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
}

void PCBox_SelectWallpaperInPage(void *p, u32 delta) {
    u8 x;
    u8 y;
    U8_AT(PTR_AT(p, 0x34), 0x44d) = delta + U8_AT(PTR_AT(p, 0x34), 0x44d) / 4 * 4;
    PCBox_UpdateWallpaperCursor(p);
    PCBox_DrawWallpaperName(p);
    U32_AT(PTR_AT(p, 0x34), 0x43c) = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c));
    ((void (*)(void *, s32, u8, u8))GridInputHandler_SetNextLastUnk0FInputs)(
        PTR_AT(PTR_AT(p, 0x34), 0x2c),
        6,
        (u8)U32_AT(PTR_AT(p, 0x34), 0x43c),
        (u8)U32_AT(PTR_AT(p, 0x34), 0x43c));
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(PTR_AT(PTR_AT(p, 0x34), 0x2c), 6), &x, &y);
    ManagedSprite_SetPositionXY(PTR_AT(PTR_AT(p, 0x34), 0x320), x, y);
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 1);
}

u32 PCBox_ConfirmWallpaperChoice(void *p) {
    if (U8_AT(PTR_AT(p, 0x34), 0x44d) >= 0x10
        && PCStorage_IsBonusWallpaperUnlocked(PTR_AT(p, 4), U8_AT(PTR_AT(p, 0x34), 0x44d) - 0x10) == 0) {
        PlaySE(0x5f3);
        return PCBox_FlashPanel(p, 0xc, 0x42);
    }
    if (U8_AT(PTR_AT(p, 0x34), 0x44d) == PCBox_GetWallpaperGfxIndex(p, U8_AT(p, 0x25))) {
        PlaySE(0x5f3);
        return PCBox_FlashPanel(p, 0xc, 0x42);
    }
    if (U8_AT(p, 0x25) != U8_AT(p, 0x1f)) {
        PlaySE(0x5dc);
        return PCBox_FlashPanel(p, 0xc, 0xa5);
    }
    PlaySE(0x5dd);
    return PCBox_FlashPanel(p, 0xc, 0x47);
}

u32 PCBox_TryWithdraw(void *p) {
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    if (Party_GetCount(PTR_AT(p, 8)) != 6) {
        PCBox_CloseMessageWindow(PTR_AT(p, 0x34), 0x27);
        PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        ov14_021E884C(PTR_AT(p, 0x34));
        return PCBox_WaitForUpdateFunc(p, ov14_021E9434, 0x53);
    }
    ov14_021E83F4(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_PrintMonActionMessage(p, 0, 2, 0x25);
    U32_AT(p, 0x30) = 0xe;
    return 6;
}

u32 ov14_021F1414(void *p) {
    PCBox_CloseMessageWindow(PTR_AT(p, 0x34), 0x27);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 0);
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E9450, 0x5c);
}

u32 ov14_021F1448(void *p, s32 delta) {
    s32 v;
    u8 x;
    u8 y;
    U8_AT(p, 0x25) = delta + U8_AT(p, 0x25) / 6 * 6;
    if (System_GetTouchNew() == 0) {
        if (PCStorage_CountMonsAndEggsInBox(PTR_AT(p, 4), U8_AT(p, 0x25)) == 0x1e) {
            PlaySE(0x5f3);
        } else {
            PlaySE(0x5dd);
        }
        return 0x66;
    }
    PlaySE(0x5dd);
    PCBox_UpdateTrayCursor(p);
    PCBox_DrawTrayBoxNameAndCount(p);
    v = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c));
    U32_AT(PTR_AT(p, 0x34), 0x43c) = v;
    GridInputHandler_SetNextLastUnk0FInputs(PTR_AT(PTR_AT(p, 0x34), 0x2c), 8, v, v);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(PTR_AT(PTR_AT(p, 0x34), 0x2c), 8), &x, &y);
    ManagedSprite_SetPositionXY(PTR_AT(PTR_AT(p, 0x34), 0x320), x, y);
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
    PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 9, 1);
    return 0x61;
}

u32 ov14_021F1504(void *p, s32 delta) {
    PCBox_ChangeTrayPage(p, delta);
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6));
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
    return 0x61;
}
