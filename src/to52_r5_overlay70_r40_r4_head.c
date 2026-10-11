#include "to52_r5_overlay70_r40_r4_private.h"

int ov70_02243FE0(void *work, int index) {
    int base = ov70_02245E84[index];
    int count = ov70_02245F5C[index].a;
    if (count > 0) {
        int i = 0;
        if (i < count) {
            do {
                if (ov70_02243FD4(work, base + i) > 0) {
                    return TRUE;
                }
                i++;
            } while (i < ov70_02245F58[index].b);
        }
    } else if (ov70_02243FD4(work, base) > 0) {
        return TRUE;
    }
    return FALSE;
}

int ov70_02244038(void *work) {
    void *parent;
    ov70_02244670(work);
    *(vu16 *)0x04000304 &= 0xFFFF7FFF;
    BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, 0x3D);
    ov70_022441A4(PTR(work, 4));
    ov70_022442B4(work);
    ov70_0224458C(work);
    GfGfx_EngineATogglePlanes(1, TRUE);
    GfGfx_EngineATogglePlanes(2, TRUE);
    GfGfx_EngineATogglePlanes(4, FALSE);
    GfGfx_EngineATogglePlanes(8, FALSE);
    GfGfx_EngineBTogglePlanes(1, TRUE);
    GfGfx_EngineBTogglePlanes(2, TRUE);
    GfGfx_EngineBTogglePlanes(4, FALSE);
    GfGfx_EngineBTogglePlanes(8, FALSE);
    if (ov00_021EC5B4() == 0) {
        parent = PTR(work, 0);
        if (U32(parent, 0x3C) != 0) {
            sub_02039418(PTR(parent, 0x20));
            ov70_02244FA4(work, PTR(work, 0xBA8), 1, 1, 0xF0F);
            Gts_SetState(work, 0xC, 2);
            Gts_CreateWaitingIcon(work);
        } else {
            Sys_ClearSleepDisableFlag(4);
            U32(work, 0x2C) = 0;
        }
    } else {
        parent = PTR(work, 0);
        sub_02039418(PTR(parent, 0x20));
        ov70_02245124(work);
        U32(work, 0x2C) = 0x11;
    }
    return 2;
}
