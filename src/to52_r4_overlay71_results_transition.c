#include "to52_r4_overlay71_results_transition_private.h"

BOOL ov71_02249D80(Ov71R30 *work, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            S32_(work, 0x10) = -0x180000;
            S32_(work, 0x14) = 0x50000;
            S32_(work, 0x18) = 0x5999;
            S32_(work, 0x1C) = -0x2222;
            U32_(work, 8) = 60;
            U32_(work, 0x20) = 0;
            SysTask_CreateOnVBlankQueue(ov71_02249E24, work, 0);
            (*state)++;
        }
        break;
    case 1:
        if (U32_(work, 0x20) != 0) {
            PlaySE(0x6A8);
            BeginNormalPaletteFade(0, 0, 0, 0x7FFF, 0x10, 1, 0x39);
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void ov71_02249E24(void *task, Ov71R30 *work) {
    if (U32_(work, 8) != 0) {
        U32_(work, 8)
        --;
        S32_(work, 0x10) += S32_(work, 0x18);
        S32_(work, 0x14) += S32_(work, 0x1C);
        ov71_0224A0B8(PTR_(work, 0xC), S32_(work, 0x10) >> 12, S32_(work, 0x14) >> 12);
    } else {
        ov71_0224A0B8(PTR_(work, 0xC), -0x30, -0x30);
        U32_(work, 0x20) = 1;
        SysTask_Destroy(task);
    }
}

void ov71_02249E6C(Ov71R30 *work) {
    GfGfx_SetBanks(ov71_0224BE54);
    *(volatile u16 *)0x04000304 |= 0x8000;
    SetBothScreensModesAndDisable(ov71_0224BDF0);
    InitBgFromTemplate(PTR_(work, 0xC), 1, ov71_0224BE00, 0);
    InitBgFromTemplate(PTR_(work, 0xC), 5, ov71_0224BE00, 0);
    InitBgFromTemplate(PTR_(work, 0xC), 2, ov71_0224BE1C, 0);
    InitBgFromTemplate(PTR_(work, 0xC), 3, ov71_0224BE38, 0);
    InitBgFromTemplate(PTR_(work, 0xC), 7, ov71_0224BE38, 0);
    GfGfxLoader_LoadCharData(0x59, 0x13, PTR_(work, 0xC), 3, 0, 0, 1, 0x39);
    GfGfxLoader_LoadCharData(0x59, 0x13, PTR_(work, 0xC), 7, 0, 0, 1, 0x39);
    GfGfxLoader_LoadScrnData(0x59, 0x12, PTR_(work, 0xC), 3, 0, 0, 1, 0x39);
    GfGfxLoader_LoadScrnData(0x59, 0x12, PTR_(work, 0xC), 7, 0, 0, 1, 0x39);
    GfGfxLoader_GXLoadPal(0x59, 0x14, 0, 0, 0x20, 0x39);
    GfGfxLoader_GXLoadPal(0x59, 0x14, 4, 0, 0x20, 0x39);
    BG_FillCharDataRange(PTR_(work, 0xC), 1, 0, 1, 0xC8);
    BG_FillCharDataRange(PTR_(work, 0xC), 5, 0, 1, 0xC8);
    BG_FillCharDataRange(PTR_(work, 0xC), 2, 0, 1, 0xC8);
    ov71_02247124(PTR_(work, 0), 0, 1, 0xE, 0);
    ov71_02247124(PTR_(work, 0), 0, 5, 0xE, 0);
    ov71_02247124(PTR_(work, 0), 1, 2, 0xF, 0);
    FillBgTilemapRect(PTR_(work, 0xC), 1, 0xC8, 0, 0, 0x20, 0x40, 0);
    FillBgTilemapRect(PTR_(work, 0xC), 5, 0xC8, 0, 0, 0x20, 0x40, 0);
    FillBgTilemapRect(PTR_(work, 0xC), 2, 0xC8, 0, 0, 0x20, 0x40, 0);
    ov71_02247230(PTR_(work, 0), 0, 1, 0xE, 0x14, 0);
    ov71_02247230(PTR_(work, 0), 0, 5, 0xE, 0x14, 0);
    ov71_02247230(PTR_(work, 0), 1, 2, 0xF, 2, 0);
    BgCommitTilemapBufferToVram(PTR_(work, 0xC), 1);
    BgCommitTilemapBufferToVram(PTR_(work, 0xC), 5);
    BgCommitTilemapBufferToVram(PTR_(work, 0xC), 2);
    ov71_0224A0B8(PTR_(work, 0xC), 0x50, -0x180);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
}
