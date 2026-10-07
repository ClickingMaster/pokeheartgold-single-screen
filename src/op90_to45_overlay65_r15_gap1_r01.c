#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "render_window.h"
#include "options.h"
typedef struct WindowPos {
    u16 x;
    u16 y;
} WindowPos;
typedef struct WindowDesc {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u16 baseTile;
} WindowDesc;
extern WindowPos ov65_0222010C[];
extern WindowDesc ov65_02220144[];
void ov65_0221F8D0(BgConfig *bgConfig, Window *windows, Options *options);

// @fn ov65_0221F8D0
void ov65_0221F8D0(BgConfig *bgConfig, Window *windows, Options *options) {
    int i;
    AddWindowParameterized(bgConfig, &windows[0], 1, 2, 1, 10, 2, 8, 1);
    FillWindowPixelBuffer(&windows[0], 0);
    AddWindowParameterized(bgConfig, &windows[1], 1, 20, 1, 10, 2, 8, 21);
    FillWindowPixelBuffer(&windows[1], 0);
    AddWindowParameterized(bgConfig, &windows[6], 1, 26, 21, 5, 2, 8, 41);
    FillWindowPixelBuffer(&windows[6], 0);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 985, 10, Options_GetFrame(options), HEAP_ID_26);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 1015, 11, 0, HEAP_ID_26);
    AddWindowParameterized(bgConfig, &windows[21], 0, 2, 21, 20, 2, 13, 1);
    FillWindowPixelBuffer(&windows[21], 0);
    AddWindowParameterized(bgConfig, &windows[22], 0, 2, 21, 27, 2, 13, 40);
    FillWindowPixelBuffer(&windows[22], 0);
    AddWindowParameterized(bgConfig, &windows[23], 0, 2, 19, 27, 4, 13, 54);
    FillWindowPixelBuffer(&windows[23], 0);
    AddWindowParameterized(bgConfig, &windows[25], 0, 20, 19, 11, 4, 13, 162);
    FillWindowPixelBuffer(&windows[25], 0);
    for (i = 0; i < 14; i++) {
        AddWindowParameterized(bgConfig, &windows[i + 7], 1, ov65_0222010C[i].x, ov65_0222010C[i].y, 8, 2, 8, 51 + i * 16);
        FillWindowPixelBuffer(&windows[i + 7], 0);
    }
    for (i = 0; i < 8; i++) {
        AddWindowParameterized(bgConfig, &windows[i + 26], 4, ov65_02220144[i].x, ov65_02220144[i].y, ov65_02220144[i].width, ov65_02220144[i].height, 8, ov65_02220144[i].baseTile);
        FillWindowPixelBuffer(&windows[i + 26], 0);
    }
}
