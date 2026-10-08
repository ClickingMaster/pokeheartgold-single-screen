#pragma force_active on
#include "global.h"
#include "bg_window.h"

typedef struct Ov40Sub {
    u8 unk0[0x1B4];
    Window windows[1];
} Ov40Sub;

typedef struct Ov40Unk {
    u8 unk0[0x860];
    Ov40Sub *sub;
} Ov40Unk;

void ov40_02237D6C(Ov40Unk *data);

// @fn ov40_02237D6C
void ov40_02237D6C(Ov40Unk *data) {
    u32 i = 0;
    Window *window = data->sub->windows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        i++;
        window++;
    } while (i < 1);
}
