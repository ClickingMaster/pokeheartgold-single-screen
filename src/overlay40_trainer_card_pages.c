#include "overlay40_trainer_card_pages.h"

int ov40_02241488(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        if (VsRecorder_IsFlipScreensDone(p) != NULL) {
            VsRecorder_InitBgBlend((int *)w, (int *)w + 1, 0);
            BYTE(w, 0x79C) = BYTE(p, 0x2604);
            PTR(w, 0x4BC) = (u8 *)p + 0x8B8;
            PTR(p, 0x8B4) = PTR(w, 0x4BC);
            VsRecorder_ImportBattleRecord(p);
            ov40_0224085C(p);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
            GfGfx_EngineBTogglePlanes(4, 0);
            GfGfx_EngineATogglePlanes(4, 0);
            GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
            GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
            VsRecorder_StartFlipScreens(p, 0);
            WORD(p, 8)
            ++;
        }
        break;
    case 1:
        if (VsRecorder_IsFlipScreensDone(p) != NULL) {
            if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 1)) {
                SetBgPriority(0, 1);
                SetBgPriority(1, 3);
                SetBgPriority(2, 0);
                SetBgPriority(3, 1);
                SetBgPriority(4, 1);
                SetBgPriority(5, 3);
                SetBgPriority(6, 0);
                SetBgPriority(7, 2);
                PTR(w, 0x608) = TouchHitboxController_Create(sVsRecorderSearchDetailHitboxes, 4, ov40_02241C10, p, 0x6D);
                PTR(w, 0x60C) = TouchHitboxController_Create(sVsRecorderSearchSaveSlotHitboxes, 4, ov40_02241C70, p, 0x6D);
                PTR(w, 0x610) = TouchHitboxController_Create(sVsRecorderSearchOverwriteHitboxes, 2, ov40_02241CD8, p, 0x6D);
                WORD(p, 8)
                ++;
            }
        }
        break;
    case 2:
        ov40_02242110(p);
        ov40_02241AB0(p);
        GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
        GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x1CC) == 0) {
            VsRecorder_CreateVideoInfo((u8 *)w + 0x10C, p);
            VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x10C, 0);
            VsRecorder_LoadScreen(p, ov40_02230410((u8 *)w + 0x10C), 3);
        } else {
            VsRecorder_CreateProfile((u8 *)w + 0x80, p);
            VsRecorder_SetProfileDrawFlag((u8 *)w + 0x80, 0);
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 0x50, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
        }
        VsRecorder_SetVBlankPaused(p, NULL);
        GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 6, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 3:
        WORD(w, 8) = 0;
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 0)) {
            if (WORD(w, 0x1CC) == 0) {
                VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x10C, 1);
            } else {
                VsRecorder_SetProfileDrawFlag((u8 *)w + 0x80, 1);
            }
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            GfGfx_EngineBTogglePlanes(8, 1);
            GfGfx_EngineATogglePlanes(8, 1);
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    case 4:
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 5:
        if (IsPaletteFadeFinished() == 1) {
            WORD(p, 8)
            ++;
        }
        break;
    default:
        WORD(p, 0x6D8)
        ++;
        VsRecorder_ClearResumeState(p);
        VsRecorder_SetSubState(p, 8);
        break;
    }
    return 0;
}

int ov40_022417A4(void *p) {
    VsRecorder_PlaceOrbitCursor(p, PTR(p, 0x6F0), 0x80, 0x60);
    OrbitCursor_SetVisible(PTR(p, 0x6F0), 1);
    OrbitCursor_SetRadius(PTR(p, 0x6F0), 0x18, 0x18);
    VsRecorder_SetSubState(p, 3);
    return 0;
}

int ov40_022417DC(void *p) {
    void *w = PTR(p, 0x860);
    void *q;
    int a;
    switch (WORD(p, 8)) {
    case 0:
        VsRecorder_ShowTopMessage(p, 0x77);
        PlaySE(0x57D);
        WORD(p, 8)
        ++;
        break;
    case 1:
        if (!VsRecorder_IsNetIdle(p)) {
            return 0;
        }
        BattleRecord_Load(PTR(p, 0x830), 0x6D, &a, 0, 0);
        PTR(w, 0x1D0) = FrontierCard_New(0x6D);
        FrontierCard_InitFromSave(PTR(w, 0x1D0), PTR(p, 0x830));
        if (GlobalTerminalNet_RequestVideoUpload(VsRecorder_GetNet(p), PTR(w, 0x1D0)) == 1) {
            FrontierCard_Delete(PTR(w, 0x1D0));
            WORD(p, 8)
            ++;
        }
        break;
    case 2:
        if (!VsRecorder_IsNetIdle(p)) {
            return 0;
        }
        BattleRecord_Delete();
        VsRecorder_HideTopMessage(p);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(p), (u32 *)&q) == 1) {
            StopSE(0x57D, 0);
            VsRecorder_ShowMessageScreen(p, 6, WORD(q, 0xC), WORD(q, 4));
            OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
            OrbitCursor_SetRadius(PTR(p, 0x6F0), 0, 0);
            WORD(p, 8) = 0xFF;
        } else {
            VsRecorder_ShowTopMessage(p, 0x77);
            VsRecorder_PrintVideoNumber(w, p);
            StopSE(0x57D, 0);
            PlaySE(0x577);
            WORD(p, 8)
            ++;
        }
        break;
    case 3:
        if (System_GetTouchNew()) {
            VsRecorder_HideTopMessage(p);
            WORD(p, 8)
            ++;
        }
        break;
    default:
        OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
        OrbitCursor_SetRadius(PTR(p, 0x6F0), 0, 0);
        VsRecorder_AddGameStatsScore(p, 0x23);
        VsRecorder_SetSubState(p, 4);
        break;
    }
    return 0;
}

int ov40_02241948(void *p) {
    void *w = PTR(p, 0x860);
    if (WORD(p, 8) == 0) {
        VsRecorder_ResetFade((u8 *)w + 8);
        VsRecorder_MenuPop(p);
        VsRecorder_SetVBlankPaused(p, (void *)1);
        VsRecorder_RebuildMenuRowsParent(p);
        VsRecorder_SetVBlankPaused(p, NULL);
        VsRecorder_StartFlipScreens(p, 1);
        WORD(p, 8)
        ++;
    } else if (VsRecorder_IsFlipScreensDone(p) != NULL) {
        if (VsRecorder_StepFade((int *)((u8 *)w + 8), 0)) {
            VsRecorder_ClearPaletteBlend(p);
            VsRecorder_ResetFade((u8 *)w + 8);
            PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, 0x10, (u16)WORD(p, 0x58));
            VsRecorder_SetScreenIfDone(p, 1, 1, (u32 *)PTR(p, 0x10));
            VsRecorder_SetSubState(p, 5);
            Heap_Free(w);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
        } else {
            BLEND(p, w, 1, 2);
            BLEND(p, w, 3, 0xC);
        }
    }
    return 0;
}

void ov40_02241A34(void *p) {
    void *w = PTR(p, 0x860);
    ClearWindowTilemapAndCopyToVram((u8 *)w + 0x6D4);
    RemoveWindow((u8 *)w + 0x6D4);
}

void ov40_02241A54(void *p, int msgId) {
    void *w = PTR(p, 0x860);
    void *win = (u8 *)w + 0x6D4;
    void *str;
    FillWindowPixelBuffer(win, 0);
    str = NewString_ReadMsgData(PTR(p, 0x48), msgId);
    AddTextPrinterParameterizedWithColor(win, 0, str, VsRecorder_GetCenteredTextX(win, str), 0, 0xFF, 0xF0D00, NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(win);
}

void ov40_02241AB0(void *p) {
    void *w = PTR(p, 0x860);
    void *win = (u8 *)w + 0x6D4;
    InitWindow(win);
    AddWindowParameterized(PTR(p, 0x24), win, 6, 8, 3, 0x10, 2, 0xE, 1);
    ov40_02241A54(p, WORD(w, 0x1CC) + 0x79);
}

void ov40_02241AFC(void *p) {
    void *w = PTR(p, 0x860);
    void *win = (u8 *)w + 0x6D4;
    void *str;
    InitWindow(win);
    AddWindowParameterized(PTR(p, 0x24), win, 6, 4, 6, 0xA, 2, 0xE, 1);
    FillWindowPixelBuffer(win, 0);
    str = NewString_ReadMsgData(PTR(p, 0x48), 0x82);
    AddTextPrinterParameterizedWithColor(win, 0, str, VsRecorder_GetCenteredTextX(win, str), 0, 0xFF, 0xF0D00, NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(win);
    win = (u8 *)w + 0x6E4;
    InitWindow(win);
    AddWindowParameterized(PTR(p, 0x24), win, 6, 0x12, 6, 0xA, 2, 0xE, 0x15);
    FillWindowPixelBuffer(win, 0);
    str = NewString_ReadMsgData(PTR(p, 0x48), 0x83);
    AddTextPrinterParameterizedWithColor(win, 0, str, VsRecorder_GetCenteredTextX(win, str), 0, 0xFF, 0xF0D00, NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(win);
}

void ov40_02241BDC(void *p) {
    void *w = PTR(p, 0x860);
    ClearWindowTilemapAndCopyToVram((u8 *)w + 0x6D4);
    RemoveWindow((u8 *)w + 0x6D4);
    ClearWindowTilemapAndCopyToVram((u8 *)w + 0x6E4);
    RemoveWindow((u8 *)w + 0x6E4);
}

void ov40_02241C10(u32 a, int b, void *p) {
    if (b == 0) {
        switch (a) {
        case 0:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 9);
            break;
        case 1:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0xA);
            break;
        case 2:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0xC);
            break;
        case 3:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0xB);
            break;
        }
    }
}

void ov40_02241C70(u32 a, int b, void *p) {
    void *w = PTR(p, 0x860);
    if (b == 0) {
        switch (a) {
        case 0:
        case 1:
        case 2:
            WORD(w, 0x1C8) = a + 1;
            if (WORD(p, 0x88C + WORD(w, 0x1C8) * 4) != 0) {
                VsRecorder_SetSubState(p, 0xF);
            } else {
                VsRecorder_SetSubState(p, 0x12);
            }
            VsRecorder_OnTouchConfirm(p);
            break;
        case 3:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0xE);
            break;
        }
    }
}

void ov40_02241CD8(u32 a, int b, void *p) {
    if (b == 0) {
        switch (a) {
        case 0:
            VsRecorder_OnTouchConfirm(p);
            ov40_02241BDC(p);
            VsRecorder_SetSubState(p, 0x12);
            break;
        case 1:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0x11);
            break;
        }
    }
}

void ov40_02241D10(u32 a, int b, void *p) {
    void *w = PTR(p, 0x860);
    if (b == 0) {
        switch (a) {
        case 0:
            VsRecorder_OnTouchConfirm(p);
            BYTE(w, 0x4C5) = a;
            VsRecorder_SetSubState(p, 5);
            break;
        case 1:
            VsRecorder_OnTouchConfirm(p);
            BYTE(w, 0x4C5) = a;
            VsRecorder_SetSubState(p, 5);
            break;
        case 2: {
            u32 v = FrontierCard_GetCountry(PTR(p, 0x88C));
            VsRecorder_OnTouchConfirm(p);
            if (v == 0) {
                PlaySE(0x57C);
                VsRecorder_SetMessageText(p, 0x80);
            } else {
                if (BYTE(w, 0x4C3) == 0xFF) {
                    BYTE(w, 0x4C3) = FrontierCard_GetCountry(PTR(p, 0x88C));
                    BYTE(w, 0x4C4) = FrontierCard_GetRegion(PTR(p, 0x88C));
                } else {
                    BYTE(w, 0x4C3) = 0xFF;
                    BYTE(w, 0x4C4) = 0xFF;
                }
                VsRecorder_PrintSearchLocation(p, BYTE(w, 0x4C3), BYTE(w, 0x4C4));
                VsRecorder_PrintLocationButtonLabel(p);
            }
            break;
        }
        case 3:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 4);
            break;
        case 4:
            VsRecorder_OnTouchConfirm(p);
            if (VsRecorder_HasSearchConditions(p) == 0) {
                PlaySE(0x57C);
                VsRecorder_SetMessageText(p, 0x74);
            } else {
                VsRecorder_SetSubState(p, 0xC);
            }
            break;
        }
    }
}

void ov40_02241E14(u32 a, int b, void *p) {
    if (b == 0) {
        if (VsRecorder_SearchCategoryHasSeen(p, a) == 0) {
            PlaySE(0x57C);
        } else {
            VsRecorder_BuildSearchSpeciesList(p, a);
        }
    }
}

void ov40_02241E40(u32 a, int b, void *p) {
    void *w = PTR(p, 0x860);
    if (b == 0) {
        switch (a) {
        case 0:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0x13);
            break;
        case 1:
            VsRecorder_OnTouchConfirm(p);
            if (BattleRecord_Exists()) {
                VsRecorder_SetSubState(p, 0x14);
            } else {
                WORD(w, 0xC) = 0x14;
                VsRecorder_SetSubState(p, 0x1D);
            }
            break;
        case 2:
            VsRecorder_OnTouchConfirm(p);
            if (BattleRecord_Exists()) {
                VsRecorder_SetSubState(p, 0x16);
            } else {
                WORD(w, 0xC) = 0x16;
                VsRecorder_SetSubState(p, 0x1D);
            }
            break;
        case 3:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0x15);
            break;
        }
    }
}

void ov40_02241ED4(u32 a, int b, void *p) {
    void *w = PTR(p, 0x860);
    if (b == 0) {
        switch (a) {
        case 0:
        case 1:
        case 2:
            WORD(w, 0x1C8) = a + 1;
            if (WORD(p, 0x88C + WORD(w, 0x1C8) * 4) != 0) {
                VsRecorder_SetSubState(p, 0x19);
            } else {
                VsRecorder_SetSubState(p, 0x1C);
            }
            VsRecorder_OnTouchConfirm(p);
            break;
        case 3:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0x18);
            break;
        }
    }
}

void ov40_02241F3C(u32 a, int b, void *p) {
    if (b == 0) {
        switch (a) {
        case 0:
            VsRecorder_OnTouchConfirm(p);
            ov40_02241BDC(p);
            VsRecorder_SetSubState(p, 0x1C);
            break;
        case 1:
            VsRecorder_OnTouchConfirm(p);
            VsRecorder_SetSubState(p, 0x1B);
            break;
        }
    }
}

void ov40_02241F74(void *p, int a) {
    void *w = PTR(p, 0x860);
    if (a == 0) {
        ManagedSprite_SetDrawFlag(PTR(w, 0x14), 1);
        TextOBJ_SetSpritesDrawFlag(PTR(w, 0x18), 1);
        ManagedSprite_SetPositionXY(PTR(w, 0x30), 0x80, 0xA8);
    } else {
        ManagedSprite_SetDrawFlag(PTR(w, 0x14), 0);
        TextOBJ_SetSpritesDrawFlag(PTR(w, 0x18), 0);
        ManagedSprite_SetPositionXY(PTR(w, 0x30), 0x50, 0xA8);
    }
    TextOBJ_SetPosition(PTR(w, 0x18), 0x24, -8);
    TextOBJ_SetPosition(PTR(w, 0x34), 0x24, -8);
}

void ov40_02241FD0(void *p) {
    void *w = PTR(p, 0x860);
    ov40_0222D6EC(p, 1);
    PTR(w, 0x14) = VsRecorder_NewButtonSprite(p, 1);
    PTR(w, 0x30) = VsRecorder_NewButtonSprite(p, 1);
    VsRecorder_CreateTextSprite((u8 *)w + 0x10, (u8 *)p + 0x14, 1);
    VsRecorder_CreateTextSprite((u8 *)w + 0x2C, (u8 *)p + 0x14, 1);
    VsRecorder_SetTextSpriteMsg((u8 *)w + 0x10, (u8 *)p + 0x14, 3);
    VsRecorder_SetTextSpriteMsg((u8 *)w + 0x2C, (u8 *)p + 0x14, 0x6F);
    ManagedSprite_SetPositionXY(PTR(w, 0x14), 0x20, 0xA8);
    ManagedSprite_SetPositionXY(PTR(w, 0x30), 0x80, 0xA8);
    TextOBJ_SetPosition(PTR(w, 0x18), 0x24, -8);
    TextOBJ_SetPosition(PTR(w, 0x34), 0x24, -8);
    TextOBJ_SetSpritesDrawFlag(PTR(w, 0x18), 1);
    TextOBJ_SetSpritesDrawFlag(PTR(w, 0x34), 1);
    ManagedSprite_SetAnim(PTR(w, 0x14), 0);
    ManagedSprite_SetAnim(PTR(w, 0x30), 1);
    ov40_02241F74(p, 0);
}

void ov40_02242084(void *p) {
    void *w = PTR(p, 0x860);
    VsRecorder_FreeTextSprite((u8 *)w + 0x10);
    VsRecorder_FreeTextSprite((u8 *)w + 0x2C);
    Sprite_DeleteAndFreeResources(PTR(w, 0x14));
    Sprite_DeleteAndFreeResources(PTR(w, 0x30));
    VsRecorder_FreeButtonGfx(p);
}

void ov40_022420B4(void *p, int a) {
    void *w = PTR(p, 0x860);
    if (a == 0) {
        ManagedSprite_SetDrawFlag(PTR(w, 0x30), 1);
        TextOBJ_SetSpritesDrawFlag(PTR(w, 0x34), 1);
        ManagedSprite_SetPositionXY(PTR(w, 0x14), 0x20, 0xE8);
    } else {
        ManagedSprite_SetDrawFlag(PTR(w, 0x30), 0);
        TextOBJ_SetSpritesDrawFlag(PTR(w, 0x34), 0);
        ManagedSprite_SetPositionXY(PTR(w, 0x14), 0x50, 0xE8);
    }
    TextOBJ_SetPosition(PTR(w, 0x18), 0x24, -8);
    TextOBJ_SetPosition(PTR(w, 0x34), 0x24, -8);
}

void ov40_02242110(void *p) {
    void *w = PTR(p, 0x860);
    ov40_0222D6EC(p, 2);
    PTR(w, 0x14) = VsRecorder_NewButtonSprite(p, 2);
    PTR(w, 0x30) = VsRecorder_NewButtonSprite(p, 2);
    VsRecorder_CreateTextSprite((u8 *)w + 0x10, (u8 *)p + 0x14, 2);
    VsRecorder_CreateTextSprite((u8 *)w + 0x2C, (u8 *)p + 0x14, 2);
    if (WORD(p, 0x86C) == 0xD2) {
        VsRecorder_SetTextSpriteMsg((u8 *)w + 0x10, (u8 *)p + 0x14, 3);
        VsRecorder_SetTextSpriteMsg((u8 *)w + 0x2C, (u8 *)p + 0x14, 0x6F);
        ManagedSprite_SetAnim(PTR(w, 0x14), 0);
        ManagedSprite_SetAnim(PTR(w, 0x30), 1);
    } else {
        VsRecorder_SetTextSpriteMsg((u8 *)w + 0x10, (u8 *)p + 0x14, 3);
        VsRecorder_SetTextSpriteMsg((u8 *)w + 0x2C, (u8 *)p + 0x14, 0x5E);
        ManagedSprite_SetAnim(PTR(w, 0x14), 0);
        ManagedSprite_SetAnim(PTR(w, 0x30), 3);
    }
    ManagedSprite_SetPositionXY(PTR(w, 0x14), 0x20, 0xE8);
    ManagedSprite_SetPositionXY(PTR(w, 0x30), 0x80, 0xE8);
    TextOBJ_SetPosition(PTR(w, 0x18), 0x24, -8);
    TextOBJ_SetPosition(PTR(w, 0x34), 0x24, -8);
    TextOBJ_SetSpritesDrawFlag(PTR(w, 0x18), 1);
    TextOBJ_SetSpritesDrawFlag(PTR(w, 0x34), 1);
    ov40_022420B4(p, 0);
}

void ov40_022421FC(void *p) {
    void *w = PTR(p, 0x860);
    VsRecorder_FreeTextSprite((u8 *)w + 0x10);
    VsRecorder_FreeTextSprite((u8 *)w + 0x2C);
    Sprite_DeleteAndFreeResources(PTR(w, 0x14));
    Sprite_DeleteAndFreeResources(PTR(w, 0x30));
    VsRecorder_FreeButtonGfx(p);
}

BOOL ov40_0224222C(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x1CC) == 0) {
            VsRecorder_FreeVideoInfo((u8 *)w + 0x10C, p);
        } else {
            VsRecorder_FreeProfile((u8 *)w + 0x80, p);
        }
        VsRecorder_SetVBlankPaused(p, NULL);
        GfGfx_EngineATogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        break;
    case 1:
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 1, 1)) {
            WORD(p, 8)
            ++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x1CC) == 0) {
            VsRecorder_CreateProfile((u8 *)w + 0x80, p);
            VsRecorder_SetProfileDrawFlag((u8 *)w + 0x80, 0);
            VsRecorder_LoadScreen(p, 0x50, 3);
        } else {
            VsRecorder_CreateVideoInfo((u8 *)w + 0x10C, p);
            VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x10C, 0);
            VsRecorder_LoadScreen(p, ov40_02230410((u8 *)w + 0x10C), 3);
        }
        VsRecorder_SetVBlankPaused(p, NULL);
        WORD(p, 8)
        ++;
        break;
    case 3:
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 1)) {
            if (WORD(w, 0x1CC) == 0) {
                VsRecorder_SetProfileDrawFlag((u8 *)w + 0x80, 1);
            } else {
                VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x10C, 1);
            }
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            WORD(p, 8)
            ++;
        }
        break;
    default:
        WORD(w, 0x1CC) = 1 ^ WORD(w, 0x1CC);
        ov40_02241A54(p, WORD(w, 0x1CC) + 0x79);
        return TRUE;
    }
    return FALSE;
}

BOOL ov40_02242378(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x1CC) == 0) {
            VsRecorder_FreeVideoInfo((u8 *)w + 0x10C, p);
        } else {
            VsRecorder_FreeProfile((u8 *)w + 0x80, p);
        }
        VsRecorder_SetVBlankPaused(p, NULL);
        ov40_02241A34(p);
        GfGfx_EngineATogglePlanes(4, 0);
        GfGfx_EngineBTogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        break;
    case 1:
        VsRecorder_StepFade((int *)w + 2, 1);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 1, 0)) {
            VsRecorder_SetVBlankPaused(p, (void *)1);
            ov40_022421FC(p);
            ov40_02241054(p);
            VsRecorder_SetVBlankPaused(p, NULL);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    case 2:
        VsRecorder_StepFade((int *)w + 2, 0);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 1)) {
            ov40_02240E28(p, 0x64, 0);
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL ov40_02242490(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        VsRecorder_StepFade((int *)w + 2, 1);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 1, 2)) {
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 0x54, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
            ov40_02240F24(p, 0x66, WORD(w, 0x1C8));
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    case 1:
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 2)) {
            ov40_02241AFC(p);
            GfGfx_EngineBTogglePlanes(4, 1);
            WORD(p, 8)
            ++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL ov40_0224253C(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        GfGfx_EngineBTogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        break;
    case 1:
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 1, 2)) {
            ov40_02241BDC(p);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
            WORD(p, 8)
            ++;
        }
        break;
    case 2:
        VsRecorder_StepFade((int *)w + 2, 0);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 2)) {
            ov40_02240F24(p, 0x64, 0);
            GfGfx_EngineBTogglePlanes(4, 1);
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

int ov40_022425E8(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        GfGfx_EngineBTogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        break;
    case 1:
        VsRecorder_StepFade((int *)w + 2, 1);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 1, 2)) {
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
            ov40_02240F24(p, 0x116, 0);
            VsRecorder_ResetRecordSave(p);
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    case 2:
        if (VsRecorder_StepRecordSave(p, WORD(w, 0x1C8), BYTE(w, 0x79C))) {
            BYTE(w, 0x79C) = 0;
            WORD(p, 8)
            ++;
        }
        break;
    case 3:
        VsRecorder_PlaceOrbitCursor(p, PTR(p, 0x6F0), 0x80, 0x60);
        OrbitCursor_SetVisible(PTR(p, 0x6F0), 1);
        OrbitCursor_SetRadius(PTR(p, 0x6F0), 0x18, 0x18);
        PlaySE(0x57D);
        WORD(p, 8)
        ++;
        break;
    case 4: {
        u64 v;
        if (!VsRecorder_IsNetIdle(p)) {
            return 0;
        }
        ov40_02240F24(p, 0x118, 0);
        v = BattleRecordHeader_GetField(BattleRecord_GetHeader(), 4, 0);
        if (ov39_0222774C(VsRecorder_GetNet(p), (u32)v, (u32)(v >> 32)) == 1) {
            WORD(p, 8)
            ++;
        }
        break;
    }
    case 5: {
        u32 tmp;
        if (!VsRecorder_IsNetIdle(p)) {
            return 0;
        }
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(p), &tmp) == 1) {
            StopSE(0x57D, 0);
        } else {
            StopSE(0x57D, 0);
            PlaySE(0x577);
        }
        WORD(p, 8)
        ++;
        ov40_02240F24(p, 0x119, 0);
        break;
    }
    case 6:
        if (System_GetTouchNew()) {
            VsRecorder_RemoveTopMessage(p);
            ov40_02241114(p);
            OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
            OrbitCursor_SetRadius(PTR(p, 0x6F0), 0, 0);
            VsRecorder_FreeSavedVideos(p);
            VsRecorder_LoadSavedVideos(p);
            VsRecorder_ClearListWindow((u8 *)p + 0x49C);
            WORD(p, 8)
            ++;
        }
        break;
    case 7:
        TouchHitboxController_Destroy(PTR(w, 0x608));
        TouchHitboxController_Destroy(PTR(w, 0x60C));
        TouchHitboxController_Destroy(PTR(w, 0x610));
        VsRecorder_ResetFade((u8 *)w + 8);
        VsRecorder_SetVBlankPaused(p, (void *)1);
        ov40_02240910(p);
        VsRecorder_RebuildMenuRowsParent(p);
        VsRecorder_SetVBlankPaused(p, NULL);
        WirelessStatusIcon_ShowOnBothScreens(1, 0x6D);
        VsRecorder_StartFlipScreens(p, 1);
        WORD(p, 8)
        ++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(p) != NULL) {
            if (VsRecorder_StepFade((int *)w + 2, 0)) {
                VsRecorder_ClearPaletteBlend(p);
                VsRecorder_ResetFade((u8 *)w + 8);
                PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, 0x10, (u16)WORD(p, 0x58));
                VsRecorder_SetScreenIfDone(p, 1, TRUE, (u32 *)PTR(p, 0x10));
                VsRecorder_SetSubState(p, 5);
                Heap_Free(w);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
                BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
                if (BattleRecord_Exists() == 1) {
                    BattleRecord_Delete();
                }
            } else {
                BLEND(p, w, 1, 2);
                BLEND(p, w, 3, 0xC);
            }
        }
        break;
    }
    return 0;
}

BOOL ov40_022428D4(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
        GfGfx_EngineBTogglePlanes(4, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        break;
    case 1:
        VsRecorder_StepFade((int *)w + 2, 1);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 1, 0)) {
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    case 2:
        if (WORD(p, 0x86C) == 0xD2) {
            VsRecorder_SetTextSpriteMsg((u8 *)w + 0x10, (u8 *)p + 0x14, 3);
            VsRecorder_SetTextSpriteMsg((u8 *)w + 0x2C, (u8 *)p + 0x14, 0x5E);
            ManagedSprite_SetAnim(PTR(w, 0x14), 0);
            ManagedSprite_SetAnim(PTR(w, 0x30), 3);
        }
        ov40_02241A34(p);
        ov40_02241AB0(p);
        GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
        GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x1CC) == 0) {
            VsRecorder_FreeVideoInfo((u8 *)w + 0x10C, p);
            VsRecorder_CreateVideoInfo((u8 *)w + 0x10C, p);
            VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x10C, 0);
            VsRecorder_LoadScreen(p, ov40_02230410((u8 *)w + 0x10C), 3);
        } else {
            VsRecorder_FreeProfile((u8 *)w + 0x80, p);
            VsRecorder_CreateProfile((u8 *)w + 0x80, p);
            VsRecorder_SetProfileDrawFlag((u8 *)w + 0x80, 0);
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 0x50, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
        }
        VsRecorder_SetVBlankPaused(p, NULL);
        GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 6, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 3:
        VsRecorder_StepFade((int *)w + 2, 0);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 0)) {
            if (WORD(w, 0x1CC) == 0) {
                VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x10C, 1);
            } else {
                VsRecorder_SetProfileDrawFlag((u8 *)w + 0x80, 1);
            }
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL ov40_02242AEC(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        VsRecorder_RemoveTopMessage(p);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
        GfGfx_EngineBTogglePlanes(4, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        break;
    case 1:
        VsRecorder_StepFade((int *)w + 2, 1);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 1, 0)) {
            ov40_02241114(p);
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    case 2:
        ov40_02242110(p);
        if (WORD(p, 0x86C) == 0xD2) {
            VsRecorder_SetTextSpriteMsg((u8 *)w + 0x10, (u8 *)p + 0x14, 3);
            VsRecorder_SetTextSpriteMsg((u8 *)w + 0x2C, (u8 *)p + 0x14, 0x5E);
            ManagedSprite_SetAnim(PTR(w, 0x14), 0);
            ManagedSprite_SetAnim(PTR(w, 0x30), 3);
        }
        ov40_02241AB0(p);
        GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
        GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x1CC) == 0) {
            VsRecorder_CreateVideoInfo((u8 *)w + 0x10C, p);
            VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x10C, 0);
            VsRecorder_LoadScreen(p, ov40_02230410((u8 *)w + 0x10C), 3);
        } else {
            VsRecorder_CreateProfile((u8 *)w + 0x80, p);
            VsRecorder_SetProfileDrawFlag((u8 *)w + 0x80, 0);
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 0x50, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
        }
        VsRecorder_SetVBlankPaused(p, NULL);
        GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 6, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 3:
        VsRecorder_StepFade((int *)w + 2, 0);
        if (VsRecorder_StepBlend((int *)w, (int *)w + 1, 0, 0)) {
            if (WORD(w, 0x1CC) == 0) {
                VsRecorder_SetVideoPartyIconsDrawFlag((u8 *)w + 0x10C, 1);
            } else {
                VsRecorder_SetProfileDrawFlag((u8 *)w + 0x80, 1);
            }
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            WORD(p, 8)
            ++;
        }
        BLEND(p, w, 3, 0xC);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL ov40_02242CFC(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            WORD(p, 8)
            ++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(p, (void *)1);
        if (WORD(w, 0x1CC) == 0) {
            VsRecorder_FreeVideoInfo((u8 *)w + 0x10C, p);
        } else {
            VsRecorder_FreeProfile((u8 *)w + 0x80, p);
        }
        VsRecorder_SetVBlankPaused(p, NULL);
        TouchHitboxController_Destroy(PTR(w, 0x608));
        TouchHitboxController_Destroy(PTR(w, 0x60C));
        TouchHitboxController_Destroy(PTR(w, 0x610));
        VsRecorder_SetVBlankPaused(p, (void *)1);
        ov40_02240910(p);
        ov40_02241A34(p);
        ov40_022421FC(p);
        VsRecorder_RebuildMenuRows(p);
        VsRecorder_SetVBlankPaused(p, NULL);
        VsRecorder_SetResumeScreen(p, WORD(PTR(p, 0x10), 0));
        sub_02087A84(PTR(p, 0x868), 1, 0);
        WORD(p, 8)
        ++;
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

void ov40_02242DF0(void *p, u32 *flag) {
    void *w = PTR(p, 0x860);
    if (*flag != 1) {
        u64 v = GlobalTerminalNet_GetUploadedVideoNumber(VsRecorder_GetNet(p));
        WORD(w, 0x4B0) = (u32)v;
        WORD(w, 0x4B4) = (u32)(v >> 32);
    }
}

void ov40_02242E14(void *p, u32 *flag) {
    if (*flag != 1) {
        GlobalTerminalNet_GetDownloadedVideo(VsRecorder_GetNet(p), (u8 *)p + 0x8B4);
        VsRecorder_ImportBattleRecord(p);
        BYTE(p, 0x2604) = BYTE(PTR(p, 0x8B4), 0xA7);
    }
}
