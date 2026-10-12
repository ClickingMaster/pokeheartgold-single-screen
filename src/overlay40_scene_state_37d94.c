#include "overlay40_scene_state_37d94_private.h"

int ov40_02237D94(void *p) {
    Ov40Inner *inner = PTR(p, 0x860);
    void *selected;

    if (!VsRecorder_IsNetIdle(p)) {
        return 0;
    }

    switch (WORD(p, 8)) {
    case 0:
        OrbitCursor_SetVisible(PTR(p, 0x6F4), 0);
        ov40_02237008(p);
        ov40_02236FE0(p);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
        VsRecorder_ApplyBlend((u8 *)inner + 0x1A4, (u8 *)inner + 0x1A8, 8, 0x12, 8, 0x12, 1);
        GfGfx_EngineATogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        /* fallthrough */
    case 1:
        VsRecorder_StepFade((u8 *)inner + 0x1AC, 1);
        if (VsRecorder_StepBlend((u8 *)inner + 0x1A4, (u8 *)inner + 0x1A8, 1, 0)) {
            GfGfxLoader_LoadCharDataFromOpenNarc(PTR(p, 0x14), 0x3E, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 3, PTR(p, 0x24), 3, 0, 0, 0, 0x6D);
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 0x4E, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
            WORD(p, 8)
            ++;
        }
        PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(inner, 0x1AC), (u16)WORD(p, 0x58));
        break;
    case 2:
        VsRecorder_PlaceOrbitCursor(p, PTR(p, 0x6F0), 0x80, 0x60);
        OrbitCursor_SetVisible(PTR(p, 0x6F0), 1);
        OrbitCursor_SetRadius(PTR(p, 0x6F0), 0x18, 0x18);
        VsRecorder_ShowTopMessage(p, 0x121);
        PlaySE(0x57D);
        WORD(p, 8)
        ++;
        break;
    case 3:
        if (GlobalTerminalNet_RequestBoxDownload(VsRecorder_GetNet(p), PTR(inner, 0x380)) == 1) {
            WORD(p, 8)
            ++;
        }
        break;
    case 4:
        VsRecorder_HideTopMessage(p);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(p), &selected) == 1) {
            StopSE(0x57D, 0);
            VsRecorder_ShowMessageScreen(p, 3, WORD(selected, 0xC), WORD(selected, 4));
            WORD(inner, 0x388) = 0;
            WORD(inner, 0x2F64) = 0;
            VsRecorder_SetVBlankPaused(p, 1);
            ov40_02236EB4(p);
            OrbitCursor_SetVisible(PTR(p, 0x6F4), 0);
            OrbitCursor_SetActive(PTR(p, 0x6F0), 0);
            OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
            OrbitCursor_SetRadius(PTR(p, 0x6F0), 0, 0);
            GfGfx_EngineATogglePlanes(4, 0);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
            VsRecorder_SetVBlankPaused(p, 0);
            WORD(p, 8) = 7;
        } else {
            StopSE(0x57D, 0);
            PlaySE(0x577);
            WORD(p, 8)
            ++;
        }
        break;
    case 5:
        OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
        OrbitCursor_SetRadius(PTR(p, 0x6F0), 0, 0);
        if (WORD(inner, 0x388) == 0) {
            WORD(p, 8)
            ++;
            WORD(p, 0xC) = 0;
            VsRecorder_ShowTopMessage(p, 0x126);
        } else {
            WORD(p, 8) = 0xFF;
        }
        break;
    case 6:
        WORD(p, 0xC)
        ++;
        if ((s32)WORD(p, 0xC) >= 60 || System_GetTouchNew() == 1) {
            WORD(p, 0xC) = 0;
            VsRecorder_HideTopMessage(p);
            WORD(inner, 0x2F64) = 0;
            VsRecorder_SetVBlankPaused(p, 1);
            ov40_02236EB4(p);
            OrbitCursor_SetVisible(PTR(p, 0x6F4), 0);
            OrbitCursor_SetActive(PTR(p, 0x6F0), 0);
            OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
            OrbitCursor_SetRadius(PTR(p, 0x6F0), 0, 0);
            GfGfx_EngineATogglePlanes(4, 0);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
            VsRecorder_SetVBlankPaused(p, 0);
            WORD(p, 8)
            ++;
        }
        break;
    case 7:
        if (!VsRecorder_StepBlend((u8 *)inner + 0x1A4, (u8 *)inner + 0x1A8, 1, 0)) {
            break;
        }
        GfGfx_EngineATogglePlanes(4, 0);
        GfGfx_EngineBTogglePlanes(4, 0);
        GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(p, 0x14), 0x4F, PTR(p, 0x24), 7, 0, 0, 0, 0x6D);
        WORD(p, 8)
        ++;
        break;
    case 8:
        VsRecorder_StepFade((u8 *)inner + 0x1AC, 0);
        if (VsRecorder_StepBlend((u8 *)inner + 0x1A4, (u8 *)inner + 0x1A8, 0, 2)) {
            ov40_0222C710(p, 2);
            ov40_02237030(p, 0x113);
            ov40_02236F38(p);
            GfGfx_EngineATogglePlanes(4, 1);
            GfGfx_EngineBTogglePlanes(4, 1);
            VsRecorder_SetSubState(p, 3);
        }
        PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(inner, 0x1AC), (u16)WORD(p, 0x58));
        break;
    default:
        VsRecorder_StepFade((u8 *)inner + 0x1AC, 0);
        if (VsRecorder_StepBlend((u8 *)inner + 0x1A4, (u8 *)inner + 0x1A8, 0, 0)) {
            ov40_02237C9C(p);
            ov40_02238290(p);
            VsRecorder_SetVBlankPaused(p, 1);
            inner->copied = ov40_02245310;
            WORD(inner, 0x2EB0) = WORD(inner, 0x388);
            VsRecorder_CreateTouchScrollSpritesWithAnim((u8 *)p + 0x47C, p, 2);
            VsRecorder_ClearListWindow((u8 *)p + 0x49C);
            VsRecorder_InitPairList((u8 *)p + 0x49C, p, &inner->copied, (u8 *)inner + 0x2E0C);
            VsRecorder_TouchScrollSetEnabled((u8 *)p + 0x47C, (u8 *)p + 0x49C);
            VsRecorder_CreateListArrows((u8 *)p + 0x49C, p, 1);
            VsRecorder_SetListArrowsY((u8 *)p + 0x49C, 0x38, 0xB0);
            VsRecorder_SetVBlankPaused(p, 0);
            ov40_02237AC0(p);
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            VsRecorder_PlaceOrbitCursor(p, PTR(p, 0x6F0), 0x10, (s16)(WORD(p, 0x4D8) * 24 + 0x44));
            OrbitCursor_SetVisible(PTR(p, 0x6F0), 1);
            OrbitCursor_SetRadius(PTR(p, 0x6F0), 0xC, 0xC);
            VsRecorder_SetSubState(p, 5);
        }
        PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(inner, 0x1AC), (u16)WORD(p, 0x58));
        PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, (u8)WORD(inner, 0x1AC), (u16)WORD(p, 0x58));
        break;
    }
    return 0;
}
