#include "overlay59_window_setup_gap5_private.h"
void ov59_0223B740(Ov59R10Gap5Work *work) {
    int i;
    int tile;
    int y;
    for (i = 0; i <= 4; i++) {
        AddWindow(work->bgConfig, &work->windows[i], &ov59_0223CA40[i]);
        FillWindowPixelBuffer(&work->windows[i], 0);
    }
    tile = 0xC1;
    i = 0;
    y = 4;
    for (; i < 5; i++) {
        AddWindowParameterized(work->bgConfig, &work->windows[i + 5], 2, 1, (u8)y, 0x10, 2, 0xC, (u16)tile);
        FillWindowPixelBuffer(&work->windows[i + 5], 0);
        tile += 0x20;
        y += 4;
    }
    work->prompt = YesNoPrompt_Create(work->heap);
}
