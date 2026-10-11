#include "overlay40_pc_box_grid_gap3_private.h"

#define BLEND(work, mask, coeff, color) \
    PaletteData_BlendPalettes(PTR(work, 0x28), mask, coeff, color, (u16)U32(work, 0x58))

int ov40_02230ED8(void *work) {
    switch (S32(work, 8)) {
    case 0:
        if (VsRecorder_HasResumeScreen(work) == TRUE) {
            VsRecorder_SetSubState(work, 1);
            break;
        }
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, 0x6D);
        PaletteData_BlendPalettes(PTR(work, 0x28), 2, 0xFFFE, 0x10, (u16)ov40_0222DAF0(work));
        PaletteData_BlendPalettes(PTR(work, 0x28), 0, 0xBFFF, 0x10, (u16)ov40_0222DAF0(work));
        PaletteData_BlendPalettes(PTR(work, 0x28), 3, 0x3FFE, 0x10, (u16)ov40_0222DAF0(work));
        PaletteData_BlendPalettes(PTR(work, 0x28), 1, 0xFFFF, 0x10, (u16)ov40_0222DAF0(work));
        sub_02087A30(PTR(work, 0x6F4));
        sub_02087A30(PTR(work, 0x6F0));
        S32(work, 8)
        ++;
        break;
    case 1:
        if (!IsPaletteFadeFinished()) {
            break;
        }
        OrbitCursor_SetPosition(PTR(work, 0x6F4), 0x80, 0x10);
        OrbitCursor_SetTarget(PTR(work, 0x6F4), 0x80, 0xD8);
        OrbitCursor_SetVisible(PTR(work, 0x6F4), 1);
        OrbitCursor_SetActive(PTR(work, 0x6F4), 1);
        PlaySE(0x576);
        S32(work, 8)
        ++;
        break;
    case 2:
        S32(work, 0xC)
        ++;
        if (S32(work, 0xC) < 0x19) {
            break;
        }
        S32(work, 0xC) = 0;
        OrbitCursor_SetPosition(PTR(work, 0x6F0), 0x80, -0x10);
        OrbitCursor_SetTarget(PTR(work, 0x6F0), 0x80, 0x60);
        OrbitCursor_SetVisible(PTR(work, 0x6F0), 1);
        OrbitCursor_SetActive(PTR(work, 0x6F0), 1);
        S32(work, 8)
        ++;
        break;
    case 3:
        S32(work, 0xC)
        ++;
        if (S32(work, 0xC) < 0x12) {
            break;
        }
        VsRecorder_ShowTouchHere(work);
        OrbitCursor_SetVisible(PTR(work, 0x6F4), 0);
        S32(work, 0xC) = 0;
        S32(work, 8)
        ++;
        break;
    case 4:
        if (S32(work, 0xC) < 0x10) {
            S32(work, 0xC) += 4;
            PaletteData_BlendPalettes(PTR(work, 0x28), 0, 0xF000, (u8)S32(work, 0xC), 0xFFFF);
        } else {
            S32(work, 8)
            ++;
        }
        break;
    case 5:
        if (S32(work, 0xC) > 0) {
            S32(work, 0xC) -= 4;
            PaletteData_BlendPalettes(PTR(work, 0x28), 0, 0xF000, (u8)S32(work, 0xC), 0xFFFF);
        } else {
            S32(work, 8)
            ++;
        }
        break;
    case 6:
        if (System_GetTouchHeld() == TRUE) {
            VsRecorder_HideTouchHere(work);
            OrbitCursor_SetVisible(PTR(work, 0x6F0), 0);
            sub_02087A54(PTR(work, 0x6F4));
            sub_02087A54(PTR(work, 0x6F0));
            S32(work, 8)
            ++;
        }
        break;
    default:
        VsRecorder_SetSubState(work, 1);
        break;
    }
    return 0;
}

int ov40_02231100(void *work) {
    switch (S32(work, 8)) {
    case 0:
        VsRecorder_SetVBlankPaused(work, 1);
        VsRecorder_LoadDecorGfx(work);
        VsRecorder_CreateDecorSprites(work);
        VsRecorder_LoadMenuRowGfx(work);
        VsRecorder_CreateMenuRowSprites(work);
        VsRecorder_CreateMenuRowText(work);
        VsRecorder_RefreshChoiceRows(work);
        VsRecorder_RefreshPathRows(work);
        VsRecorder_SetVBlankPaused(work, 0);
        if (VsRecorder_HasResumeScreen(work) == TRUE) {
            if (S32(work, 0) == 0) {
                GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(work, 0x14), 0x45, PTR(work, 0x24), 5, 0, 0, 0, 0x6D);
            } else {
                GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(work, 0x14), 0x37, PTR(work, 0x24), 5, 0, 0, 0, 0x6D);
            }
            OrbitCursor_SetPosition(PTR(work, 0x6F0), 0x80, 0xE0);
            OrbitCursor_SetTarget(PTR(work, 0x6F0), 0x80, 0xE0);
            OrbitCursor_SetVisible(PTR(work, 0x6F0), 0);
            OrbitCursor_SetActive(PTR(work, 0x6F0), 1);
            VsRecorder_SetScreenIfDone(work, PTR(work, 0x83C), 1, PTR(work, 0x10));
            break;
        }
        PlaySE(0x573);
        S32(work, 0xC) = 0x10;
        S32(work, 8)
        ++;
        break;
    case 1:
        if (VsRecorder_HasResumeScreen(work) == TRUE) {
            if (IsPaletteFadeFinished() == TRUE) {
                S32(work, 8)
                ++;
            }
            break;
        }
        if (S32(work, 0xC) != 0) {
            S32(work, 0xC) -= 2;
            PaletteData_BlendPalettes(PTR(work, 0x28), 2, 0xFFFE, (u8)S32(work, 0xC), (u16)ov40_0222DAF0(work));
            PaletteData_BlendPalettes(PTR(work, 0x28), 0, 0xFFFF, (u8)S32(work, 0xC), (u16)ov40_0222DAF0(work));
            PaletteData_BlendPalettes(PTR(work, 0x28), 3, 0x3FFE, (u8)S32(work, 0xC), (u16)ov40_0222DAF0(work));
            PaletteData_BlendPalettes(PTR(work, 0x28), 1, 0xFFFF, (u8)S32(work, 0xC), (u16)ov40_0222DAF0(work));
        } else {
            BLEND(work, 1, 2, 0x10);
            BLEND(work, 2, 0xC, 0x10);
            S32(work, 8)
            ++;
        }
        break;
    case 2:
        if (S32(work, 0) == 0) {
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(work, 0x14), 0x45, PTR(work, 0x24), 5, 0, 0, 0, 0x6D);
        } else {
            GfGfxLoader_LoadScrnDataFromOpenNarc(PTR(work, 0x14), 0x37, PTR(work, 0x24), 5, 0, 0, 0, 0x6D);
        }
        S32(work, 8)
        ++;
        break;
    default:
        if (S32(work, 0x724) >= 3) {
            VsRecorder_SetSubState(work, 2);
        }
        break;
    }
    return 0;
}

int ov40_0223131C(void *work) {
    switch (S32(work, 8)) {
    case 0:
        OrbitCursor_SetPosition(PTR(work, 0x6F0), 0x80, 0x60);
        OrbitCursor_SetTarget(PTR(work, 0x6F0), 0x80, 0x60);
        OrbitCursor_SetVisible(PTR(work, 0x6F0), 1);
        OrbitCursor_SetActive(PTR(work, 0x6F0), 1);
        S32(work, 0xC)
        ++;
        BLEND(work, 1, 2, (u8)(0x10 - S32(work, 0xC)));
        if (S32(work, 0xC) == 2) {
            S32(work, 0xC) = 0xF;
            S32(work, 8)
            ++;
        }
        break;
    case 1:
        BLEND(work, 1, 2, (u8)S32(work, 0xC));
        S32(work, 8)
        ++;
        break;
    case 2:
        S32(work, 0xC)
        --;
        BLEND(work, 1, 2, (u8)S32(work, 0xC));
        if (S32(work, 0xC) == 0) {
            S32(work, 8)
            ++;
        }
        break;
    default:
        VsRecorder_SetSubState(work, 3);
        break;
    }
    return 0;
}
