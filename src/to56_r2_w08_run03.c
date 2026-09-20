#include "global.h"

#include "to56_r2_w08_private.h"

void ov74_0222A240(void *manager, int mode) {
    void *work = OverlayManager_GetData(manager);
    int value;
    void *win;
    if (mode == 0) {
        value = 4;
    } else {
        value = -1;
    }
    win = (u8 *)work + 0x48;
    if (U32(win, 0) == 0) {
        AddWindowParameterized(PTR(work, 0), win, 0, 0x17, 10, 6, 4, 0, 0x1ac);
    }
    DrawFrameAndWindow1(win, 1, 0x1f, 3);
    ov74_0222A89C(manager, ov74_0223C260, 2, win, value);
}
