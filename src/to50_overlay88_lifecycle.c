#include "overlay88_lifecycle_private.h"

BOOL ov88_02258800(OverlayManager *manager, int *state) {
    u8 *args;
    u8 *work;
#pragma unused(state)
    args = OverlayManager_GetArgs(manager);
    Heap_Create((enum HeapID)3, (enum HeapID)0x72, 5 << 16);
    work = OverlayManager_CreateAndGetData(manager, 0x270, (enum HeapID)0x72);
    memset(work, 0, 0x270);
    *(u32 *)(work + 0) = *(u32 *)(args + 0);
    *(u32 *)(work + 4) = *(u32 *)(args + 4);
    ov88_02258ABC(work + 8, 0x72);
    ov88_02258D64(work + 0x14C, work + 8, 0x72);
    ov88_02258EA8(work + 0x154, work + 8, work + 0x14C, *(u32 *)(work + 0), *(u32 *)(work + 4), *(u32 *)(args + 8), 0x72);
    ov88_022590C0(work + 0x160, work + 8, work + 0x14C, *(u32 *)(work + 0), *(u32 *)(args + 8), 0x72);
    ov88_022592EC(work + 0x164, work + 8, 0x72);
    ov88_0225967C(work + 0x200, work + 8, 0x72);
    Main_SetVBlankIntrCB(ov88_02258A64, work);
    HBlankInterruptDisable();
    return TRUE;
}

BOOL ov88_022588C4(OverlayManager *manager, u32 *state) {
    u8 *work = OverlayManager_GetData(manager);
    u8 *args = OverlayManager_GetArgs(manager);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade((enum FadeMode)0, (enum FadeType)1, (enum FadeType)1, 0, 6, 1, (enum HeapID)0x72);
        ov45_0222A520(*(void **)(args + 8), 1);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    case 2:
        if ((gSystem.newKeys & PAD_BUTTON_B) || ov45_0222A330(*(void **)(args + 8)) == 1 || ov45_0222D844() == 1) {
            if (ov45_0222A330(*(void **)(args + 8)) == 1) {
                ov45_0222A4A8(*(void **)(args + 8));
            }
            *state = 3;
        } else {
            if (ov88_02258EFC(work + 0x154, *(u32 *)(work + 0), *(u32 *)(work + 4)) == TRUE) {
                ov88_02259818(work + 0x200, 1);
            }
            if (ov88_022590D8(work + 0x160, *(void **)(work + 0)) == TRUE) {
                ov88_02259818(work + 0x200, 2);
            }
            if (ov88_02259404(work + 0x164, *(u32 *)(work + 0), *(void **)(args + 8), 0x72) == TRUE) {
                ov88_02259818(work + 0x200, 3);
            }
        }
        break;
    case 3:
        BeginNormalPaletteFade((enum FadeMode)0, (enum FadeType)0, (enum FadeType)0, 0, 6, 1, (enum HeapID)0x72);
        (*state)++;
        break;
    case 4:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    ov88_02258A70(work, *(u32 *)(args + 8));
    return FALSE;
}

BOOL ov88_022589FC(OverlayManager *manager, int *state) {
    u8 *work;
#pragma unused(state)
    work = OverlayManager_GetData(manager);
    OverlayManager_GetArgs(manager);
    ov88_022597DC(work + 0x200);
    ov88_02258EF8(work + 0x154);
    ov88_022590D4(work + 0x160);
    ov88_022593D0(work + 0x164);
    ov88_02258D84(work + 0x14C);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    ov88_02258AF4(work + 8);
    OverlayManager_FreeData(manager);
    Heap_Destroy((enum HeapID)0x72);
    return TRUE;
}
