#pragma force_active on
#include "global.h"
#include "bg_window.h"

typedef struct Root {
    u8 pad[0x27];
    u8 col[5];
    u8 row[5];
} Root;

typedef struct Work {
    Root *root;
    u8 pad[0x28];
    Window *windows;
    u8 pad2[0x114D - 0x30];
    u8 sel;
} Work;

void ov08_022239CC(Work *, u32);
void ov08_02223678(Work *, u32, u32, u32, u32, u32, u32);
void ov08_02223A3C(Work *, u32);
void ov08_022234A4(Work *, u32, u32, u32, u32, u32);
void ov08_02223AA0(Work *work);

// @fn ov08_02223AA0
void ov08_02223AA0(Work *work) {
    u32 i;
    u8 col, row;
    for (i = 0; i < 4; i++) {
        FillWindowPixelBuffer(&work->windows[i], 0);
    }
    col = work->root->col[work->sel];
    row = work->root->row[work->sel];
    ov08_022239CC(work, col + row * 6);
    ov08_02223678(work, col + row * 6, 0, 1, 0, 0, 0x10200);
    ov08_02223A3C(work, col + row * 6);
    ov08_022234A4(work, 3, 31, 4, 5, 0x30201);
}
