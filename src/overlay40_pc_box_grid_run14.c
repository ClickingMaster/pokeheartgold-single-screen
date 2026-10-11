#include "overlay40_pc_box_grid_private.h"

int ov40_02234470(void *p) {
    void *w = PTR(p, 0x860);
    if (TouchscreenHitbox_TouchNewIsIn(ov40_022451C8)) {
        VsRecorder_OnTouchConfirm(p);
        VsRecorder_SetSubState(p, 5);
    }
    if (TouchscreenHitbox_TouchNewIsIn(ov40_022451CC)) {
        VsRecorder_OnTouchConfirm(p);
        VsRecorder_SetSubState(p, 4);
    }
    if (PTR(w, 0x228) != NULL) {
        Thunk_G3X_Reset();
        AccessoryPortrait_Draw(PTR(w, 0x228));
        RequestSwap3DBuffers(0, 0);
    }
    return 0;
}
