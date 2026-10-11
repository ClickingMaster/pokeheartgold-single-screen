#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { int y; int x; } Pos;
typedef struct { u32 *w[32]; } State;
extern State *ov13_0224F4C0;
extern u32 *WifiSetupOamBlock_GetOam(u32 *, int);
extern void ov13_0222AAFC(int, int, int);
extern void WifiSetupTask_SetFunc(void *, void (*)(void *));
void ov13_0222AF5C(void *), ov13_0222AFD4(void *), ov13_0222B044(void *), ov13_0222B0B4(void *), ov13_0222B124(void *), ov13_0222B194(void *);
// @fn ov13_0222AF5C
void ov13_0222AF5C(void *w) {
    Pos pos;
    u32 word = *WifiSetupOamBlock_GetOam(ov13_0224F4C0->w[0x40 / 4], 0);
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_0222AAFC(4, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    WifiSetupTask_SetFunc(w, ov13_0222AFD4);
}
// @fn ov13_0222AFD4
void ov13_0222AFD4(void *w) {
    Pos pos;
    u32 word = *ov13_0224F4C0->w[0x34 / 4];
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_0222AAFC(3, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    WifiSetupTask_SetFunc(w, ov13_0222B044);
}
// @fn ov13_0222B044
void ov13_0222B044(void *w) {
    Pos pos;
    u32 word = *ov13_0224F4C0->w[0x28 / 4];
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_0222AAFC(2, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    WifiSetupTask_SetFunc(w, ov13_0222B0B4);
}
// @fn ov13_0222B0B4
void ov13_0222B0B4(void *w) {
    Pos pos;
    u32 word = *ov13_0224F4C0->w[0x1c / 4];
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_0222AAFC(1, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    WifiSetupTask_SetFunc(w, ov13_0222B124);
}
// @fn ov13_0222B124
void ov13_0222B124(void *w) {
    Pos pos;
    u32 word = *ov13_0224F4C0->w[0x10 / 4];
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    ov13_0222AAFC(0, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) {
        return;
    }
    WifiSetupTask_SetFunc(w, ov13_0222B194);
}
