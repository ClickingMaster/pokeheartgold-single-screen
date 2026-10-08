#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct Ov86List {
    u16 *ids;
    u32 count;
} Ov86List;
typedef struct State {
    u8 pad0[0xC];
    BgConfig *bgConfig;
    Window windows[32];
    u8 pad210[0x250 - 0x210];
    u8 rectX, rectY, rectW, rectH;
    u8 pad254[4];
    u16 listIdx;
    u8 pad25A[2];
    s8 page;
    u8 pad25D;
    u16 toggle;
    Ov86List lists[4];
} State;
void ov86_021E668C(State *, int);
void ov86_021E6064(State *, u32, u32, u32, u32, u32, u32, u32);
void ov86_021E78B8(State *, int);
void ov86_021E7B68(State *);
// @fn ov86_021E7B68
void ov86_021E7B68(State *s) {
    int base;
    u16 start;
    u8 i;
    u8 pal;
    if (s->toggle == 0) {
        base = 5;
    } else {
        base = 13;
    }
    start = s->page * 8;
    for (i = 0; i < 8; i++) {
        FillWindowPixelBuffer(&s->windows[base + i], 0);
        if (s->lists[s->listIdx].count > start + i) {
            ov86_021E668C(s, s->lists[s->listIdx].ids[start + i]);
            ov86_021E6064(s, base + i, 0x13, GetWindowWidth(&s->windows[base + i]) * 8 / 2, 0, 4, 0xF0100, 2);
            pal = 0;
        } else {
            pal = 1;
        }
        ScheduleWindowCopyToVram(&s->windows[base + i]);
        ov86_021E78B8(s, i);
        BgTilemapRectChangePalette(s->bgConfig, 2, s->rectX, s->rectY, s->rectW, s->rectH, pal);
    }
    ScheduleBgTilemapBufferTransfer(s->bgConfig, 2);
    s->toggle ^= 1;
}
