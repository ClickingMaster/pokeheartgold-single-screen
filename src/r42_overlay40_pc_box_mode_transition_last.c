#include "r42_overlay40_pc_box_mode_transition_last_private.h"

#define BLEND(p, w, mask) PaletteData_BlendPalettes(PTR(p, 0x28), mask, 12, (u8)U32(w, 8), (u16)U32(p, 0x58))

int ov40_02234D94(void *p) {
    void *w = PTR(p, 0x860);
    switch (S32(p, 8)) {
    case 0:
        SetBgPriority(0, 1);
        SetBgPriority(1, 3);
        SetBgPriority(2, 2);
        SetBgPriority(3, 1);
        SetBgPriority(4, 1);
        SetBgPriority(5, 3);
        SetBgPriority(6, 2);
        SetBgPriority(7, 1);
        Heap_Free(PTR(w, 0x238));
        FrontierCard_Delete(PTR(w, 0x250));
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
        S32(p, 8)
        ++;
        break;
    case 1:
        VsRecorder_StepFade((u8 *)w + 8, 1);
        if (VsRecorder_StepBlend(w, (u8 *)w + 4, 1, 0)) {
            VsRecorder_SetVBlankPaused(p, 1);
            ov40_02235B10(p);
            VsRecorder_SetVBlankPaused(p, 0);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
            BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
            S32(p, 8)
            ++;
        }
        BLEND(p, w, 3);
        break;
    case 2:
        VsRecorder_ResetFade((u8 *)w + 8);
        VsRecorder_SetVBlankPaused(p, 1);
        VsRecorder_RebuildMenuRowsParent(p);
        VsRecorder_SetVBlankPaused(p, 0);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
        VsRecorder_StartFlipScreens(p, 1);
        S32(p, 8)
        ++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(p)) {
            if (VsRecorder_StepFade((u8 *)w + 8, 0)) {
                VsRecorder_SetVBlankPaused(p, 1);
                if (PTR(w, 0x228) != NULL) {
                    AccessoryPortrait_Destroy();
                    VsRecorder_ResetBg2(p);
                    SetBgPriority(2, 0);
                    ov40_02235994();
                }
                VsRecorder_SetVBlankPaused(p, 0);
                VsRecorder_ClearPaletteBlend(p);
                VsRecorder_ResetFade((u8 *)w + 8);
                PaletteData_BlendPalettes(PTR(p, 0x28), 2, 12, 0x10, (u16)U32(p, 0x58));
                VsRecorder_SetScreenIfDone(p, 1, 1, PTR(p, 0x10));
                VsRecorder_SetSubState(p, 5);
                Heap_Free(w);
                UnloadOverlayByID(FS_OVERLAY_ID(OVY_41));
                Main_SetVBlankIntrCB(VsRecorder_VBlankCB, p);
            } else {
                PaletteData_BlendPalettes(PTR(p, 0x28), 1, 2, (u8)U32(w, 8), (u16)U32(p, 0x58));
                BLEND(p, w, 3);
            }
        }
        break;
    }
    return 0;
}

int ov40_02234F98(void *p) {
    void *w = PTR(p, 0x860);
    void *q;
    if (!VsRecorder_IsNetIdle(p)) {
        return 0;
    }
    switch (S32(p, 8)) {
    case 0:
        VsRecorder_PlaceOrbitCursor(p, PTR(p, 0x6f0), 0x80, 0x60);
        OrbitCursor_SetVisible(PTR(p, 0x6f0), 1);
        OrbitCursor_SetRadius(PTR(p, 0x6f0), 0x18, 0x18);
        VsRecorder_ShowTopMessage(p, 0x11f);
        PlaySE(0x57d);
        S32(p, 8)
        ++;
        break;
    case 1:
        if (GlobalTerminalNet_RequestDressUpDownload(VsRecorder_GetNet(p), PTR(w, 0x2e0)) == 1) {
            S32(p, 8)
            ++;
        }
        break;
    case 2:
        VsRecorder_HideTopMessage(p);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(p), &q) == 1) {
            StopSE(0x57d, 0);
            VsRecorder_ShowMessageScreen(p, 1, PTR(q, 0xc), PTR(q, 4));
            S32(w, 0x2e4) = 0;
            S32(p, 0xc) = 0;
            OrbitCursor_SetVisible(PTR(p, 0x6f0), 0);
            VsRecorder_SetSubState(p, 3);
        } else {
            StopSE(0x57d, 0);
            PlaySE(0x577);
            S32(p, 8)
            ++;
        }
        break;
    case 3:
        GfGfx_EngineATogglePlanes(1, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        GfGfx_EngineBTogglePlanes(4, 0);
        OrbitCursor_SetVisible(PTR(p, 0x6f0), 0);
        OrbitCursor_SetRadius(PTR(p, 0x6f0), 0, 0);
        S32(p, 8)
        ++;
        break;
    case 4:
        if (S32(w, 0x2e4) != 0) {
            S32(w, 0x234) = 0;
            S32(w, 0x230) = 0;
            VsRecorder_SetSubState(p, 7);
        } else {
            VsRecorder_ShowTopMessage(p, 0x125);
            S32(p, 8)
            ++;
            S32(p, 0xc) = 0;
        }
        break;
    default:
        S32(p, 0xc)
        ++;
        if (S32(p, 0xc) >= 60 || System_GetTouchNew() == 1) {
            S32(p, 0xc) = 0;
            VsRecorder_HideTopMessage(p);
            VsRecorder_SetSubState(p, 3);
        }
        break;
    }
    return 0;
}
