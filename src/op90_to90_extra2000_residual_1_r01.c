#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "item.h"
typedef struct { u8 pad[0x28]; String *str; u8 pad2[4]; Window windows[64]; u8 pad3[0x44E - 0x430]; u8 toggle:4; u8 rest:4; } Inner;
typedef struct { u8 pad[0x34]; Inner *inner; } Work;
void ov14_021F4F24(Window *window, String *str, s32 x, s32 y, u32 font, u32 color, u32 align);
u32 ov14_021F5564(Work *p, u16 itemId);
// @fn ov14_021F5564
u32 ov14_021F5564(Work *p, u16 itemId) {
    u32 win;
    Window *w;
    Inner *inner = p->inner;
    if (inner->toggle == 0) win = 20; else win = 22;
    w = &inner->windows[win];
    inner->toggle ^= 1;
    FillWindowPixelBuffer(w, 13);
    FillWindowPixelBuffer(w + 1, 13);
    if (itemId != 0) {
        GetItemNameIntoString(p->inner->str, itemId, HEAP_ID_10);
        ov14_021F4F24(w, p->inner->str, 0, 0, 0, 0x10200, 0);
        CopyWindowPixelsToVram_TextMode(w);
        GetItemDescIntoString(p->inner->str, itemId, HEAP_ID_10);
        ov14_021F4F24(w + 1, p->inner->str, 0, 0, 0, 0x10200, 0);
        CopyWindowPixelsToVram_TextMode(w + 1);
    }
    return win;
}
