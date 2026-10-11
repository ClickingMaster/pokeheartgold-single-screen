#include "to41_overlay_40_residual_29_unmatched_2_private.h"

#define BLEND(p, w) PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 0x1AC), (u16)WORD(p, 0x58))

int ov40_022366CC(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        if (VsRecorder_StepFade((u8 *)w + 0x1AC, 1)) {
            WORD(p, 8)
            ++;
        }
        PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, (u8)WORD(w, 0x1AC), (u16)WORD(p, 0x58));
        PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 0x1AC), (u16)WORD(p, 0x58));
        PaletteData_BlendPalettes(PTR(p, 0x28), 1, 2, (u8)WORD(w, 0x1AC), (u16)WORD(p, 0x58));
        break;
    case 1:
        VsRecorder_SetVBlankPaused(p, 1);
        VsRecorder_FreeMenuRows(p);
        VsRecorder_SetVBlankPaused(p, 0);
        VsRecorder_StartFlipScreens(p, 0);
        OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
        WORD(p, 8)
        ++;
        break;
    case 2:
        if (VsRecorder_IsFlipScreensDone(p)) {
            VsRecorder_SetVBlankPaused(p, 1);
            ov40_022364D0(p);
            VsRecorder_ResetBgPriorities(VsRecorder_SetVBlankPaused(p, 0));
            WORD(p, 8)
            ++;
        }
        break;
    default:
        VsRecorder_SetSubState(p, 2);
        break;
    }
    return 0;
}
