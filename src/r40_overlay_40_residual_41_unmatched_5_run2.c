#include "r40_overlay_40_residual_41_unmatched_5_private.h"

int ov40_0224483C(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        WORD(p, 8)
        ++;
    case 1:
        WORD(p, 8)
        ++;
    case 2:
        VsRecorder_RebuildMenuRowsParent(p);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 2);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 6);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 3);
        BgClearTilemapBufferAndCommit(PTR(p, 0x24), 7);
        VsRecorder_StartFlipScreens(p, 1);
        WORD(p, 8)
        ++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(p) != NULL) {
            if (VsRecorder_StepFade((u8 *)w + 8, 0)) {
                VsRecorder_ClearPaletteBlend(p);
                VsRecorder_ResetFade((u8 *)w + 8);
                PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, 0x10, (u16)WORD(p, 0x58));
                VsRecorder_SetScreenIfDone(p, 1, TRUE, PTR(p, 0x10));
                VsRecorder_SetSubState(p, 5);
                Heap_Free(w);
            } else {
                PaletteData_BlendPalettes(PTR(p, 0x28), 1, 2, (u8)WORD(w, 8), (u16)WORD(p, 0x58));
                PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 8), (u16)WORD(p, 0x58));
            }
        }
        break;
    }
    return 0;
}
