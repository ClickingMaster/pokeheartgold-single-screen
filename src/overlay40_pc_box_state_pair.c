#include "overlay40_pc_box_state_pair_private.h"
#define BLEND2(p, w) PaletteData_BlendPalettes(PTR(p, 0x28), 3, 0xC, (u8)WORD(w, 0x1AC), (u16)WORD(p, 0x58))
int ov40_02236D60(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        ov40_0222D980((u8 *)w + 0x1A4, (u8 *)w + 0x1A8, 8, 0x12, 8, 0x12, 1);
        ov40_02230964(p, 1);
        ov40_02237564(p);
        ov40_02237410(p);
        ov40_022371A0(p);
        ov40_02230964(p, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        WORD(p, 8)
        ++;
    case 1:
        ov40_0222DA84((u8 *)w + 0x1AC, 1);
        if (ov40_0222DA00((u8 *)w + 0x1A4, (u8 *)w + 0x1A8, 1, 0)) {
            WORD(p, 8)
            ++;
        }
        BLEND2(p, w);
        break;
    case 2:
        VsRecorder_SetSubState(p, 0xA);
        break;
    }
    return 0;
}
int ov40_02236E18(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        ov40_02236FE0(p);
        ov40_0222D980((u8 *)w + 0x1A4, (u8 *)w + 0x1A8, 8, 0x12, 8, 0x12, 1);
        GfGfx_EngineATogglePlanes(4, 0);
        WORD(p, 8)
        ++;
    case 1:
        ov40_0222DA84((u8 *)w + 0x1AC, 1);
        if (ov40_0222DA00((u8 *)w + 0x1A4, (u8 *)w + 0x1A8, 1, 0)) {
            WORD(p, 8)
            ++;
        }
        BLEND2(p, w);
        break;
    case 2:
        VsRecorder_SetSubState(p, 0xD);
        break;
    }
    return 0;
}
