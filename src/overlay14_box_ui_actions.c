#include "overlay14_box_ui_actions_internal.h"

void ov14_021E8294(void *p) {
    BgCellManager_StartCellMove(p, 9, 0, 1, 3);
}

void ov14_021E82A8(void *p) {
    BgCellManager_StartCellMove(p, 9, 0, -1, 3);
}

void ov14_021E82BC(void *p) {
    PCBox_DrawReturnButton(p, 0);
    BgCellManager_DrawCellAt(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa, 0x18, 0x15);
}

void ov14_021E82DC(void *p) {
    PCBox_DrawExitButton(p, 0);
    BgCellManager_DrawCellAt(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa, 0x18, 0x15);
}

void ov14_021E82FC(void *p) {
    BgCellManager_EraseCell(p, 0xa);
    BgCellManager_DrawCellAt(p, 0xa, 0x18, 0x18);
}

void ov14_021E8314(void *p) {
    BgCellManager_StartCellMove(p, 0xa, 0, 1, 3);
}

void ov14_021E8328(void *p) {
    BgCellManager_DrawCellAt(p, 0xa, 0x18, 0x18);
    BgCellManager_StartCellMove(p, 0xa, 0, -1, 3);
}

u32 ov14_021E834C(void *p) {
    s8 x;
    s8 y;
    BgCellManager_GetCellPosition(p, 0xb, &x, &y);
    return y == 0x15;
}

void ov14_021E8368(void *p) {
    BgCellManager_EraseCell(p, 8);
    BgCellManager_EraseCell(p, 9);
    BgCellManager_DrawCellAt(p, 8, 0, 0x18);
    BgCellManager_DrawCellAt(p, 9, 0xc, 0x18);
}

void ov14_021E8394(void *p) {
    u32 i;
    s32 v;
    for (i = 0, v = 5; i < 5; i++, v += 3) {
        BgCellManager_DrawCellAt(p, i + 3, 0x20, (s8)v);
    }
    BgCellManager_DrawCellAt(p, 0xb, 0x18, 0x18);
}

void ov14_021E83C4(void *p) {
    u32 i;
    s32 v;
    for (i = 0, v = 5; i < 5; i++, v += 3) {
        BgCellManager_DrawCellAt(p, i + 3, 0x15, (s8)v);
    }
    BgCellManager_DrawCellAt(p, 0xb, 0x18, 0x15);
}

void ov14_021E83F4(void *p) {
    u32 i;
    s32 v;
    for (i = 0, v = 5; i < 5; i++, v += 3) {
        BgCellManager_EraseCell(p, i + 3);
        BgCellManager_DrawCellAt(p, i + 3, 0x20, (s8)v);
    }
    BgCellManager_EraseCell(p, 0xb);
    BgCellManager_DrawCellAt(p, 0xb, 0x18, 0x18);
}

void PCBox_SlideInActionMenu(void *p) {
    u16 i;
    s8 x;
    s8 y;
    BgCellManager_GetCellPosition(p, 3, &x, &y);
    if (x == 0x15) {
        return;
    }
    for (i = 0; i < 5; i++) {
        BgCellManager_StartCellMove(p, i + 3, -1, 0, (u8)(x - 0x15));
    }
    BgCellManager_GetCellPosition(p, 0xb, &x, &y);
    BgCellManager_StartCellMove(p, 0xb, 0, -1, (u8)(y - 0x15));
}

void PCBox_SlideOutActionMenu(void *p) {
    u16 i;
    s8 x;
    s8 y;
    BgCellManager_GetCellPosition(p, 3, &x, &y);
    if (x == 0x20) {
        return;
    }
    for (i = 0; i < 5; i++) {
        BgCellManager_StartCellMove(p, i + 3, 1, 0, (u8)(0x20 - x));
    }
    BgCellManager_GetCellPosition(p, 0xb, &x, &y);
    BgCellManager_StartCellMove(p, 0xb, 0, 1, (u8)(0x18 - y));
}

u32 PCBox_UpdateActionMenuSlide(void *p) {
    u32 i;
    u32 ret = FALSE;
    for (i = 0; i < 5; i++) {
        if (BgCellManager_UpdateCellMove(p, i + 3) == TRUE) {
            ret = TRUE;
        }
    }
    if (BgCellManager_UpdateCellMove(p, 0xb) == TRUE) {
        ret = TRUE;
    }
    return ret;
}

u32 PCBox_IsActionMenuOpen(void *p) {
    s8 x;
    s8 y;
    BgCellManager_GetCellPosition(p, 3, &x, &y);
    return x != 0x20;
}

void ov14_021E8560(void *p) {
    BgCellManager_DrawCellAt(p, 0xc, 0x20, 0xe);
    BgCellManager_StartCellMove(p, 0xc, -1, 0, 0xb);
}

void ov14_021E8588(void *p) {
    BgCellManager_DrawCellAt(p, 0xc, 0x15, 0xe);
    BgCellManager_StartCellMove(p, 0xc, 1, 0, 0xb);
}

void ov14_021E85AC(void *p) {
    BgCellManager_DrawCellAt(p, 0xe, 0, 0x18);
}

void PCBox_SlideInChangeBoxButton(void *p) {
    BgCellManager_StartCellMove(p, 0xe, 0, -1, 3);
}

void PCBox_SlideOutChangeBoxButton(void *p) {
    BgCellManager_StartCellMove(p, 0xe, 0, 1, 3);
}

u32 PCBox_IsChangeBoxButtonShown(void *p) {
    s8 x;
    s8 y;
    BgCellManager_GetCellPosition(p, 0xe, &x, &y);
    return y == 0x15;
}

void PCBox_HideSummaryButton(void *p) {
    BgCellManager_DrawCellAt(p, 0xf, 0, 0x18);
}

void PCBox_ShowSummaryButton(void *p) {
    BgCellManager_DrawCellAt(p, 0xf, 0, 0x15);
}

void PCBox_SlideInSummaryButton(void *p) {
    BgCellManager_StartCellMove(p, 0xf, 0, -1, 3);
}

void PCBox_SlideOutSummaryButton(void *p) {
    BgCellManager_StartCellMove(p, 0xf, 0, 1, 3);
}

u32 PCBox_IsSummaryButtonShown(void *p) {
    s8 x;
    s8 y;
    BgCellManager_GetCellPosition(p, 0xf, &x, &y);
    return y == 0x15;
}

void ov14_021E8664(void *p) {
    ov14_021E82DC(p);
    BgCellManager_EraseCell(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 8);
    BgCellManager_EraseCell(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
    BgCellManager_DrawCellAt(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 8, 0, 0x18);
    BgCellManager_DrawCellAt(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9, 0xc, 0x18);
    BgCellManager_DrawCellAt(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0, 0, 0);
    PCBox_DrawTrayBoxNameAndCount(p);
    PCBox_SetupBoxTraySprites(p);
    PCBox_UpdateCurrentBoxMarker(p);
    PCBox_UpdateTrayCursor(p);
    PCBox_MoveBoxTraySprites(p, 0x30);
}

void PCBox_SlideInBoxTray(void *p) {
    BgCellManager_DrawCellAt(p, 0, 0, -6);
    BgCellManager_StartCellMove(p, 0, 0, 1, 6);
}

void PCBox_SlideOutBoxTray(void *p) {
    BgCellManager_StartCellMove(p, 0, 0, -1, 6);
}

void ov14_021E8718(void *p) {
    BgCellManager_DrawCellAt(p, 0xc, 0x15, 8);
    BgCellManager_DrawCellAt(p, 6, 0x15, 0xe);
    BgCellManager_DrawCellAt(p, 7, 0x15, 0x11);
}

void ov14_021E8740(void *p) {
    ov14_021E8664(p);
    PCBox_DrawBoxMenu(p);
    ov14_021E8718(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
}

void PCBox_SlideInBoxMenu(void *p) {
    BgCellManager_DrawCellAt(p, 0xc, 0x20, 8);
    BgCellManager_DrawCellAt(p, 6, 0x20, 0xe);
    BgCellManager_DrawCellAt(p, 7, 0x20, 0x11);
    BgCellManager_StartCellMove(p, 0xc, -1, 0, 0xb);
    BgCellManager_StartCellMove(p, 6, -1, 0, 0xb);
    BgCellManager_StartCellMove(p, 7, -1, 0, 0xb);
}

void PCBox_SlideOutBoxMenu(void *p) {
    BgCellManager_StartCellMove(p, 0xc, 1, 0, 0xb);
    BgCellManager_StartCellMove(p, 6, 1, 0, 0xb);
    BgCellManager_StartCellMove(p, 7, 1, 0, 0xb);
}

void ov14_021E87F4(void *p) {
    BgCellManager_EraseCell(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 8);
    BgCellManager_EraseCell(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
    PCBox_HidePartyButton(p);
    ov14_021E827C(p);
}

void ov14_021E8824(void *p) {
    if (((u32)U8_AT(p, 0x44e) << 25) >> 29) {
        BgCellManager_StartCellMove(PTR_AT(p, 0x2f0), 0x10, -1, 0, 0xc);
    }
}

void ov14_021E884C(void *p) {
    if (((u32)U8_AT(p, 0x44e) << 25) >> 29) {
        BgCellManager_StartCellMove(PTR_AT(p, 0x2f0), 0x10, 1, 0, 0xc);
    }
}

void ov14_021E8874(void *p) {
    if (((u32)U8_AT(p, 0x44e) << 25) >> 29) {
        BgCellManager_EraseCell(PTR_AT(p, 0x2f0), 0x10);
        BgCellManager_DrawCellAt(PTR_AT(p, 0x2f0), 0x10, 0x14, 0xc);
    }
}

void ov14_021E88A4(void *p) {
    BgCellManager_EraseCell(p, 0x10);
    BgCellManager_DrawCellAt(p, 0x10, 0x20, 0xc);
}

void ov14_021E88BC(void *p) {
    s8 x;
    s8 y;
    BgCellManager_GetCellPosition(p, 0x10, &x, &y);
    if (y != 0xf) {
        BgCellManager_StartCellMove(p, 0x10, 0, -1, (u8)(9 - (0x18 - y)));
    }
}

void ov14_021E88F8(void *p) {
    s8 x;
    s8 y;
    BgCellManager_GetCellPosition(p, 0x10, &x, &y);
    if (y != 0x18) {
        BgCellManager_StartCellMove(p, 0x10, 0, 1, (u8)(0x18 - y));
    }
}

void ov14_021E892C(void *p) {
    BgCellManager_EraseCell(p, 0x10);
    BgCellManager_DrawCellAt(p, 0x10, 0, 0xf);
}

void ov14_021E8944(void *p) {
    BgCellManager_EraseCell(p, 0x10);
    BgCellManager_DrawCellAt(p, 0x10, 0, 0x18);
}

void PCBox_DrawMarkings(void *p, u32 mask) {
    u16 i;
    u16 v;
    for (i = 0; i < 6; i++) {
        if ((1 << i) & mask) {
            v = i + 0x3a;
        } else {
            v = i + 0x1a;
        }
        FillBgTilemapRect(PTR_AT(PTR_AT(p, 0x34), 0x14), 5, v, (u8)(i + 0xf), 0x12, 1, 1, 0);
    }
    ScheduleBgTilemapBufferTransfer(PTR_AT(PTR_AT(p, 0x34), 0x14), 5);
}

u32 PCBox_StartSummaryApp(void *p) {
    void *w;
    PTR_AT(p, 0x18) = Heap_Alloc(9, 0x3c);
    w = PTR_AT(p, 0x18);
    if (U8_AT(p, 0x21) < 0x1e) {
        PTR_AT(w, 0) = PCBox_GetSlotBoxMon(p, U8_AT(p, 0x1f), 0);
        U8_AT(w, 0x11) = 2;
        U8_AT(w, 0x13) = 0x1e;
        U8_AT(w, 0x14) = U8_AT(p, 0x21);
    } else {
        PTR_AT(w, 0) = PTR_AT(p, 8);
        U8_AT(w, 0x11) = 1;
        U8_AT(w, 0x13) = Party_GetCount(PTR_AT(p, 8));
        U8_AT(w, 0x14) = U8_AT(p, 0x21) - 0x1e;
    }
    PTR_AT(w, 4) = PTR_AT(p, 0x10);
    sub_0208AD34(w, Save_PlayerData_GetProfile(PTR_AT(PTR_AT(p, 0), 0)));
    U8_AT(w, 0x12) = 0;
    sub_02089D40(w, ov14_021F7D0C);
    U8_AT(w, 0x16) = 0;
    U8_AT(w, 0x17) = 0;
    U16_AT(w, 0x18) = 0;
    U32_AT(w, 0x1c) = SaveArray_IsNatDexEnabled(PTR_AT(PTR_AT(p, 0), 0));
    PTR_AT(w, 0x20) = Save_SpecialRibbons_Get(PTR_AT(PTR_AT(p, 0), 0));
    U32_AT(w, 0x24) = 0;
    U32_AT(w, 0x28) = 0;
    PTR_AT(w, 0x2c) = sub_02088288(PTR_AT(PTR_AT(p, 0), 0));
    PTR_AT(w, 0x34) = sub_0208828C(PTR_AT(PTR_AT(p, 0), 0));
    PTR_AT(w, 0x30) = PTR_AT(PTR_AT(p, 0), 4);
    PTR_AT(p, 0x14) = OverlayManager_New(gOverlayTemplate_PokemonSummary, w, 9);
    return 0;
}

u32 PCBox_EndSummaryApp(void *p) {
    void *w = PTR_AT(p, 0x18);
    if (U8_AT(p, 0x27) == 0) {
        if (U8_AT(p, 0x21) < 0x1e) {
            U8_AT(p, 0x21) = U8_AT(w, 0x14);
        } else {
            U8_AT(p, 0x21) = U8_AT(w, 0x14) + 0x1e;
        }
    }
    if (U8_AT(p, 0x21) < 0x1e && U32_AT(w, 0x38) == 1) {
        PCStorage_SetBoxModified(PTR_AT(p, 4), U8_AT(p, 0x1f));
    }
    Heap_Free(PTR_AT(p, 0x18));
    return 0;
}

u32 PCBox_StartBagApp(void *p) {
    PTR_AT(p, 0x18) = Bag_CreateView(Save_Bag_Get(PTR_AT(PTR_AT(p, 0), 0)), ov14_021F7D14, 9);
    sub_0207789C(PTR_AT(p, 0x18), PTR_AT(PTR_AT(p, 0), 0), 1, 0, PTR_AT(PTR_AT(p, 0), 4));
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_15), 2);
    PTR_AT(p, 0x14) = OverlayManager_New(gOverlayTemplate_Bag, PTR_AT(p, 0x18), 9);
    return 0;
}

u32 PCBox_EndBagApp(void *p) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_15));
    U16_AT(p, 0x1c) = BagView_GetItemId(PTR_AT(p, 0x18));
    Heap_Free(PTR_AT(p, 0x18));
    return 0;
}

u32 PCBox_StartNamingScreenApp(void *p) {
    PTR_AT(p, 0x18) = NamingScreen_CreateArgs(9, 2, 0, 8, PTR_AT(p, 0x10), PTR_AT(PTR_AT(p, 0), 4));
    PCStorage_GetBoxName(PTR_AT(p, 4), U8_AT(p, 0x25), PTR_AT(PTR_AT(p, 0x18), 0x18));
    PTR_AT(p, 0x14) = OverlayManager_New(gOverlayTemplate_NamingScreen, PTR_AT(p, 0x18), 9);
    return 0;
}

u32 PCBox_EndNamingScreenApp(void *p) {
    void *w = PTR_AT(p, 0x18);
    PCStorage_SetBoxName(PTR_AT(p, 4), U8_AT(p, 0x25), PTR_AT(w, 0x18));
    U16_AT(p, 0x1c) = U32_AT(w, 0x14);
    NamingScreen_DeleteArgs(PTR_AT(p, 0x18));
    return 0;
}

u32 PCBox_UpdateMoveModeBoxDrag(void *p) {
    u32 x;
    u32 y;
    u32 c;
    u32 d;
    void *q = PTR_AT(p, 0x34);
    u32 a = PCBox_UpdateActionMenuSlide(PTR_AT(q, 0x2f0));
    u32 b = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 8);
    c = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
    d = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa);
    switch (U16_AT(q, 0x10)) {
    case 0:
        if (System_GetTouchHeldCoords(&x, &y) == 0) {
            a = 0xff;
            if (U8_AT(p, 0x24) != 0) {
                a = PCBox_GetTouchedTrayBox(U32_AT(PTR_AT(p, 0x34), 0x40b8), U32_AT(PTR_AT(p, 0x34), 0x40bc));
            }
            if (a == 0xff) {
                a = PCBox_GetTouchedBoxSlot(U32_AT(PTR_AT(p, 0x34), 0x40b8), U32_AT(PTR_AT(p, 0x34), 0x40bc));
            }
            PCBox_SetupMonMove(p, U8_AT(p, 0x21), a);
            PCBox_SetSelectedIconOutlineVisible(p, 0);
            if (!(a & 0x80)) {
                U8_AT(p, 0x21) = PCBox_GetMoveEntryTarget(p, U8_AT(p, 0x21));
            }
            PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            ov14_021E8824(PTR_AT(p, 0x34));
            U16_AT(q, 0x10) = 1;
        } else {
            ManagedSprite_SetPositionXY(PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc), x, (s16)y - 8);
            PCBox_UpdateSelectedIconOutline(p);
            U32_AT(PTR_AT(p, 0x34), 0x40b8) = x;
            U32_AT(PTR_AT(p, 0x34), 0x40bc) = y;
        }
        break;
    case 1:
        if (PCBox_AnimateMonMove(p) == 0 && a == 0 && b == 0 && c == 0 && d == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(q, 0xc));
            PCBox_UpdateSelectedIconOutline(p);
            PCBox_SetSelectedIconOutlineVisible(p, U32_AT(PTR_AT(p, 0x34), 0x40c4));
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    return 1;
}

u32 PCBox_UpdateMoveModePartyDrag(void *p) {
    s16 sx;
    s16 sy;
    u32 x;
    u32 y;
    void *q = PTR_AT(p, 0x34);
    u32 a = PCBox_UpdateActionMenuSlide(PTR_AT(q, 0x2f0));
    u32 b;
    if (U16_AT(q, 0x10) == 0) {
        ManagedSprite_GetPositionXY(PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc), &sx, &sy);
    }
    b = PCBox_UpdatePartyPanelSlide(p);
    switch (U16_AT(q, 0x10)) {
    case 0:
        if (U8_AT(PTR_AT(p, 0x34), 0x44a) == 1 && b == 0) {
            U8_AT(PTR_AT(p, 0x34), 0x44a) = 2;
            ov14_021F69F0(p, 0x28);
            PCBox_SetIconsTranslucent(p, 1, 0);
        }
        if (System_GetTouchHeldCoords(&x, &y) == 0) {
            u32 r;
            ManagedSprite_SetPositionXY(PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc), sx, sy);
            if (U8_AT(PTR_AT(p, 0x34), 0x44a) == 2) {
                r = PCBox_GetTouchedBoxSlot(U32_AT(PTR_AT(p, 0x34), 0x40b8), U32_AT(PTR_AT(p, 0x34), 0x40bc));
            } else {
                r = PCBox_GetTouchedPartySlot(U32_AT(PTR_AT(p, 0x34), 0x40b8), U32_AT(PTR_AT(p, 0x34), 0x40bc), ov14_021F7BF0);
            }
            PCBox_SetupMonMove(p, U8_AT(p, 0x21), r);
            PCBox_SetSelectedIconOutlineVisible(p, 0);
            U8_AT(p, 0x21) = PCBox_GetMoveEntryTarget(p, U8_AT(p, 0x21));
            if (U8_AT(p, 0x21) < 0x1e) {
                U32_AT(PTR_AT(p, 0x34), 0x40c4) = 0;
            } else {
                U32_AT(PTR_AT(p, 0x34), 0x40c4) = 1;
            }
            if (U8_AT(p, 0x21) < 0x1e) {
                if (PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 0xac, 0) == 0) {
                    U16_AT(q, 0x10) = 3;
                    break;
                }
                PCBox_SlideInPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
                PCBox_ShowSlotMonInfo(p, 0xff);
                U16_AT(q, 0x10) = 1;
                break;
            }
            ov14_021E7FEC(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            ov14_021E8824(PTR_AT(p, 0x34));
            U16_AT(q, 0x10) = 1;
            break;
        }
        if (a == 0 && U8_AT(PTR_AT(p, 0x34), 0x44a) == 0 && (x < 0x10 || y < 0x30 || x >= 0x68)) {
            U8_AT(PTR_AT(p, 0x34), 0x44a) = 1;
            ov14_021E7FB8(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
        ManagedSprite_SetPositionXY(PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc), x, (s16)y - 8);
        PCBox_UpdateSelectedIconOutline(p);
        U32_AT(PTR_AT(p, 0x34), 0x40b8) = x;
        U32_AT(PTR_AT(p, 0x34), 0x40bc) = y;
        break;
    case 1:
    case 2:
        if (ov14_021E6814(p) == 0 && a == 0 && b == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(q, 0xc));
            if (U8_AT(p, 0x21) < 0x1e) {
                U8_AT(p, 0x21) = 0xff;
            }
            PCBox_UpdateSelectedIconOutline(p);
            PCBox_SetSelectedIconOutlineVisible(p, U32_AT(PTR_AT(p, 0x34), 0x40c4));
            U16_AT(q, 0x10) = 5;
        }
        break;
    case 3:
        if (ov14_021E66F4(p) == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(q, 0xc));
            PCBox_SlideInPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            PCBox_ShowSlotMonInfo(p, 0xff);
            U8_AT(p, 0x21) = 0xff;
            U16_AT(q, 0x10)
            ++;
        }
        break;
    case 4:
        if (b == 0) {
            U16_AT(q, 0x10) = 5;
        }
        break;
    case 5:
        PCBox_SetIconsTranslucent(p, 1, 1);
        U8_AT(PTR_AT(p, 0x34), 0x44a) = 0;
        U16_AT(q, 0x10) = 0;
        return 0;
    }
    return 1;
}

u32 PCBox_UpdateBoxTrayDrag(void *p) {
    u32 x;
    u32 y;
    void *q = PTR_AT(p, 0x34);
    u32 a = BgCellManager_UpdateCellMove(PTR_AT(q, 0x2f0), 0xe);
    u32 b = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xf);
    switch (U16_AT(q, 0x10)) {
    case 0:
        if (System_GetTouchHeldCoords(&x, &y) == 0) {
            a = 0xff;
            if (U8_AT(p, 0x24) != 0) {
                a = PCBox_GetTouchedTrayBox(U32_AT(PTR_AT(p, 0x34), 0x40b8), U32_AT(PTR_AT(p, 0x34), 0x40bc));
            }
            if (a == 0xff) {
                a = PCBox_GetTouchedBoxSlot(U32_AT(PTR_AT(p, 0x34), 0x40b8), U32_AT(PTR_AT(p, 0x34), 0x40bc));
            }
            if (a == 0xff) {
                a = PCBox_GetTouchedPartySlot(U32_AT(PTR_AT(p, 0x34), 0x40b8), U32_AT(PTR_AT(p, 0x34), 0x40bc), ov14_021F7C08);
            }
            PCBox_SetupMonMove(p, U8_AT(p, 0x21), a);
            PCBox_SetSelectedIconOutlineVisible(p, 0);
            if (!(a & 0x80)) {
                U8_AT(p, 0x21) = PCBox_GetMoveEntryTarget(p, U8_AT(p, 0x21));
            }
            ov14_021E884C(PTR_AT(p, 0x34));
            U16_AT(q, 0x10)
            ++;
        } else {
            ManagedSprite_SetPositionXY(PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc), x, (s16)y - 8);
            PCBox_UpdateSelectedIconOutline(p);
            U32_AT(PTR_AT(p, 0x34), 0x40b8) = x;
            U32_AT(PTR_AT(p, 0x34), 0x40bc) = y;
        }
        break;
    case 1:
        if (a == 0 && b == 0) {
            void *m = PTR_AT(q, 0xc);
            if (U32_AT(m, 0xe8) != 0xff && (U32_AT(m, 0xe8) & 0x80)) {
                if (U32_AT(m, 0xe4) >= 0x1e && U32_AT(m, 0xe4) - 0x1e < (u32)(Party_GetCount(PTR_AT(p, 8)) - 1)) {
                    PCBox_SlideInSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
                }
            } else {
                PCBox_SlideInSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            }
            U16_AT(q, 0x10)
            ++;
        }
        /* fallthrough */
    case 2:
        if (PCBox_AnimateMonMove(p) == 0 && b == 0 && U16_AT(q, 0x10) == 2) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(q, 0xc));
            PCBox_UpdateSelectedIconOutline(p);
            PCBox_SetSelectedIconOutlineVisible(p, 0);
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    return 1;
}

u32 ov14_021E9194(void *p) {
    void *q = PTR_AT(p, 0x34);
    BgCellManager_UpdateCellMove(PTR_AT(q, 0x2f0), 0xa);
    switch (U16_AT(q, 0x10)) {
    case 0:
        ov14_021E70E0(p, U8_AT(p, 0x21));
        U16_AT(q, 0x10)
        ++;
        /* fallthrough */
    case 1:
        if (PCBox_AnimateMonMove(p) == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(q, 0xc));
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    return 1;
}

u32 PCBox_AnimateMoveToPartyEnd(void *p) {
    void *q = PTR_AT(p, 0x34);
    switch (U16_AT(q, 0x10)) {
    case 0: {
        u32 n = Party_GetCount(PTR_AT(p, 8)) + 0x1e;
        PCBox_SetupMonMove(p, U8_AT(p, 0x21), n);
        PCBox_SetSelectedIconOutlineVisible(p, 0);
        U16_AT(q, 0x10)
        ++;
        break;
    }
    case 1:
        if (PCBox_AnimateMonMove(p) == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(q, 0xc));
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    return 1;
}

u32 PCBox_AnimateMoveToFirstEmptySlot(void *p) {
    int slotno;
    int boxno;
    void *q = PTR_AT(p, 0x34);
    switch (U16_AT(q, 0x10)) {
    case 0:
        boxno = U8_AT(p, 0x1f);
        slotno = 0;
        PCStorage_FindFirstEmptySlot(PTR_AT(p, 4), &boxno, &slotno);
        PCBox_SetupMonMove(p, U8_AT(p, 0x21), slotno);
        PCBox_SetSelectedIconOutlineVisible(p, 0);
        U8_AT(p, 0x21) = 0xff;
        U16_AT(q, 0x10)
        ++;
        break;
    case 1:
        if (PCBox_AnimateMonMove(p) == 0) {
            void *m = PTR_AT(q, 0xc);
            PCBox_SetSlotIconTranslucent(PTR_AT(p, 0x34), U32_AT(m, 0xe4), 1);
            PCBox_ApplyMoveEntryIcons(p, m);
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    return 1;
}

u32 PCBox_UpdateScrollToPrevBox(void *p) {
    u32 a;
    u32 b;
    if (U16_AT(PTR_AT(p, 0x34), 0x12) == 0x17) {
        if (GridInputHandler_IsButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c)) == 1) {
            GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        }
        U16_AT(PTR_AT(p, 0x34), 0x12) = 0;
        U16_AT(PTR_AT(p, 0x34), 0x10) = 0;
        return 0;
    }
    ScheduleSetBgPosText(PTR_AT(PTR_AT(p, 0x34), 0x14), 3, 2, 8);
    PCBox_ScrollBoxIcons(p, 8);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xe);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 8);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
    a = BgCellManager_IsCellMoving(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xb);
    PCBox_UpdateActionMenuSlide(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    b = BgCellManager_IsCellMoving(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xb);
    if (a == 1 && b == 0) {
        ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    U16_AT(PTR_AT(p, 0x34), 0x12)
    ++;
    return 1;
}

u32 PCBox_UpdateScrollToNextBox(void *p) {
    u32 a;
    u32 b;
    if (U16_AT(PTR_AT(p, 0x34), 0x12) == 0x17) {
        if (GridInputHandler_IsButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c)) == 1) {
            GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        }
        U16_AT(PTR_AT(p, 0x34), 0x12) = 0;
        U16_AT(PTR_AT(p, 0x34), 0x10) = 0;
        return 0;
    }
    ScheduleSetBgPosText(PTR_AT(PTR_AT(p, 0x34), 0x14), 3, 1, 8);
    PCBox_ScrollBoxIcons(p, -8);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xe);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 8);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
    a = BgCellManager_IsCellMoving(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xb);
    PCBox_UpdateActionMenuSlide(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    b = BgCellManager_IsCellMoving(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xb);
    if (a == 1 && b == 0) {
        ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    U16_AT(PTR_AT(p, 0x34), 0x12)
    ++;
    return 1;
}

u32 ov14_021E9434(void *p) {
    if (PCBox_UpdateActionMenuSlide(PTR_AT(PTR_AT(p, 0x34), 0x2f0))) {
        return 1;
    }
    return 0;
}

u32 ov14_021E9450(void *p) {
    u32 a = PCBox_UpdateActionMenuSlide(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    u32 b = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 8);
    u32 c = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
    u32 d = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa);
    if (a == 0 && b == 0 && c == 0 && d == 0) {
        return 0;
    }
    return 1;
}
