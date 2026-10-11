#include "overlay40_pc_box_grid_private.h"

int ov40_02231868(void *p) {
    if (ov40_0223142C(p)) {
        if (WORD(p, 0x44) == 1) {
            OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
            VsRecorder_SetSubState(p, 0);
        } else {
            VsRecorder_SetSubState(p, 1);
        }
    }
    return 0;
}

int ov40_0223189C(void *p) {
    if (WORD(p, 0x44) == 1) {
        OrbitCursor_SetVisible(PTR(p, 0x6F0), 0);
        VsRecorder_SetSubState(p, 0);
    } else {
        VsRecorder_SetSubState(p, 1);
    }
    return 0;
}
