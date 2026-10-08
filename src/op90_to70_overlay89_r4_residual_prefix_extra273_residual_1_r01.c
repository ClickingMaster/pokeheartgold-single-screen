#pragma force_active on
#include "global.h"
#include "bg_window.h"

typedef struct Ov89Pos {
    u8 x;
    u8 y;
} Ov89Pos;

typedef struct Ov89Work {
    u8 pad0[8];
    BgConfig *bgConfig;
    u8 padC[0x34 - 0xC];
    Window windows[8];
    Window window;
} Ov89Work;

extern const Ov89Pos ov89_0225C9CC[];
void ov89_02259CD0(Ov89Work *work);

#pragma opt_common_subs off
#pragma opt_strength_reduction off
#pragma opt_rotateloops off
// @fn ov89_02259CD0
void ov89_02259CD0(Ov89Work *work) {
    Window *windows;
    int i;
    const Ov89Pos *pos;

    AddWindowParameterized(work->bgConfig, &work->window, GF_BG_LYR_MAIN_1, 2, 1, 0x1B, 4, 0xD, 0x5B);
    FillWindowPixelBuffer(&work->window, 15);
    i = 0;
    pos = ov89_0225C9CC;
    windows = work->windows;
    for (; i < 8; i++) {
        AddWindowParameterized(work->bgConfig, &windows[i], GF_BG_LYR_SUB_1, pos[i].x, pos[i].y, 8, 2, 0xD, 0x200 + i * 16);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}
