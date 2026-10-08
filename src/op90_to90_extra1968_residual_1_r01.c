#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { int y; int x; } Pos;
typedef struct { u32 *w[0x11d / 4]; u8 pad; u8 mode; } State;
extern State *ov13_0224F4BC;
extern u32 *ov13_0223FDE0(u32 *, int);
extern void ov13_022290F0(int, int, int, int);
extern void ov13_022425C8(void *, void (*)(void *));
void ov13_022299CC(void *), ov13_02229A50(void *), ov13_02229AC4(void *), ov13_02229B38(void *), ov13_02229BAC(void *), ov13_02229C20(void *);
// @fn ov13_022299CC
void ov13_022299CC(void *w) {
    Pos pos;
    u32 word = *ov13_0223FDE0(ov13_0224F4BC->w[0xfc / 4], 0);
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_022290F0(ov13_0224F4BC->mode, 4, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    ov13_022425C8(w, ov13_02229A50);
}
// @fn ov13_02229A50
void ov13_02229A50(void *w) {
    Pos pos;
    u32 word = *ov13_0224F4BC->w[0xc0 / 4];
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_022290F0(ov13_0224F4BC->mode, 3, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    ov13_022425C8(w, ov13_02229AC4);
}
// @fn ov13_02229AC4
void ov13_02229AC4(void *w) {
    Pos pos;
    u32 word = *ov13_0224F4BC->w[0x90 / 4];
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_022290F0(ov13_0224F4BC->mode, 2, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    ov13_022425C8(w, ov13_02229B38);
}
// @fn ov13_02229B38
void ov13_02229B38(void *w) {
    Pos pos;
    u32 word = *ov13_0224F4BC->w[0x60 / 4];
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_022290F0(ov13_0224F4BC->mode, 1, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    ov13_022425C8(w, ov13_02229BAC);
}
// @fn ov13_02229BAC
void ov13_02229BAC(void *w) {
    Pos pos;
    u32 word = *ov13_0224F4BC->w[0x30 / 4];
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_022290F0(ov13_0224F4BC->mode, 0, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    ov13_022425C8(w, ov13_02229C20);
}
