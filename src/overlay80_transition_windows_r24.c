#include "overlay80_transition_windows_r24_private.h"

BOOL ov80_0222D390(void *work) {
    switch (U32_AT(work, 4)) {
    case 0:
        FrontierTransition_StartFlash(1, 0x10, -0x10, (u8 *)work + 0xc, 2);
        U32_AT(work, 4)
        ++;
        break;
    case 1:
        if (U32_AT(work, 0xc) != 0) {
            ResetVisibleHardwareWindows(0);
            ResetVisibleHardwareWindows(1);
            BeginNormalPaletteFade(0, 0x20, 0x20, 0, 0xc, 1, 0xb);
            U32_AT(work, 4)
            ++;
        }
        break;
    default:
        if (IsPaletteFadeFinished() == 1) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL ov80_0222D404(void *work) {
    BOOL done;
    switch (U32_AT(work, 4)) {
    case 0:
        FrontierTransition_StartFlash(1, 0x10, -0x10, (u8 *)work + 0xc, 2);
        U32_AT(work, 4)
        ++;
        break;
    case 1:
        if (U32_AT(work, 0xc) == 0) {
            break;
        }
        PTR_AT(work, 0x28) = AllocWindows(0xb, 1);
        AddWindowParameterized(PTR_AT(PTR_AT(work, 0), 0), PTR_AT(work, 0x28), 1, 0, 0, 0x20, 0x20, 0, 0);
        PaletteData_FillPaletteInBuffer(PTR_AT(PTR_AT(work, 0), 4), 0, 2, 0, 0, 0x10);
        FillWindowPixelBuffer(PTR_AT(work, 0x28), 0);
        ScheduleWindowCopyToVram(PTR_AT(work, 0x28));
        PTR_AT(work, 0x2c) = FrontierWipe_New(0xb);
        U32_AT(work, 4)
        ++;
    case 2:
        FrontierWipe_Start(PTR_AT(work, 0x2c), 1, 1, PTR_AT(work, 0x28), 0xf);
        U32_AT(work, 4)
        ++;
        break;
    case 3:
        done = FrontierWipe_UpdateHorizontal(PTR_AT(work, 0x2c));
        ScheduleWindowCopyToVram(PTR_AT(work, 0x28));
        if (done != 0) {
            U32_AT(work, 4)
            ++;
        }
        break;
    default:
        if (IsPaletteFadeFinished() == 1) {
            FrontierWipe_Free(PTR_AT(work, 0x2c));
            ClearWindowTilemapAndCopyToVram(PTR_AT(work, 0x28));
            RemoveWindow(PTR_AT(work, 0x28));
            WindowArray_Delete(PTR_AT(work, 0x28), 1);
            SetMasterBrightnessByColor(0, 0);
            SetMasterBrightnessByColor(1, 0);
            BG_ClearCharDataRange(1, 0x20, 0, 0xb);
            BgClearTilemapBufferAndCommit(PTR_AT(PTR_AT(work, 0), 0), 1);
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL ov80_0222D520(void *work) {
    BOOL done;
    switch (U32_AT(work, 4)) {
    case 0:
        FrontierTransition_StartFlash(1, 0x10, -0x10, (u8 *)work + 0xc, 2);
        U32_AT(work, 4)
        ++;
        break;
    case 1:
        if (U32_AT(work, 0xc) == 0) {
            break;
        }
        PTR_AT(work, 0x28) = AllocWindows(0xb, 1);
        AddWindowParameterized(PTR_AT(PTR_AT(work, 0), 0), PTR_AT(work, 0x28), 1, 0, 0, 0x20, 0x20, 0, 0);
        PaletteData_FillPaletteInBuffer(PTR_AT(PTR_AT(work, 0), 4), 0, 2, 0, 0, 0x10);
        FillWindowPixelBuffer(PTR_AT(work, 0x28), 0);
        ScheduleWindowCopyToVram(PTR_AT(work, 0x28));
        PTR_AT(work, 0x2c) = FrontierWipe_New(0xb);
        U32_AT(work, 4)
        ++;
    case 2:
        FrontierWipe_Start(PTR_AT(work, 0x2c), 1, 1, PTR_AT(work, 0x28), 0xf);
        U32_AT(work, 4)
        ++;
        break;
    case 3:
        done = FrontierWipe_UpdateVertical(PTR_AT(work, 0x2c));
        ScheduleWindowCopyToVram(PTR_AT(work, 0x28));
        if (done != 0) {
            U32_AT(work, 4)
            ++;
        }
        break;
    default:
        if (IsPaletteFadeFinished() == 1) {
            FrontierWipe_Free(PTR_AT(work, 0x2c));
            ClearWindowTilemapAndCopyToVram(PTR_AT(work, 0x28));
            RemoveWindow(PTR_AT(work, 0x28));
            WindowArray_Delete(PTR_AT(work, 0x28), 1);
            SetMasterBrightnessByColor(0, 0);
            SetMasterBrightnessByColor(1, 0);
            BG_ClearCharDataRange(1, 0x20, 0, 0xb);
            BgClearTilemapBufferAndCommit(PTR_AT(PTR_AT(work, 0), 0), 1);
            return FALSE;
        }
        break;
    }
    return TRUE;
}
