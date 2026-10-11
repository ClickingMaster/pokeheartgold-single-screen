#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct{u8 p[0x30];u32*a;u8 p2[0x2c];u32*b;u8 p3[0x2c];u32*c;u8 p4[0x2c];u32*d;u8 p5[0x59];u8 mode;} State;
extern State*ov13_0224F4BC;
extern void ov13_022290F0(int,int,int);
extern void WifiSetupTask_SetFunc(void*,void(*)(void*));
void ov13_02228444(void*),ov13_022284D8(void*),ov13_0222856C(void*),ov13_02228600(void*);
typedef struct { int y; int x; } Pos;
// @fn ov13_02228444
void ov13_02228444(void *w) {
    Pos pos;
    Pos *p = &pos;
    u32 word = *ov13_0224F4BC->a;
    p->x = (word & 0x1ff0000) >> 16;
    p->y = (word & 0xff) - 12;
    if (p->y > 0x51) {
        ov13_022290F0(ov13_0224F4BC->mode, 0, p->y);
        return;
    }
    ov13_022290F0(ov13_0224F4BC->mode, 0, 0x51);
    ov13_022290F0(ov13_0224F4BC->mode, 1, 0xc0);
    WifiSetupTask_SetFunc(w, ov13_022284D8);
}
// @fn ov13_022284D8
void ov13_022284D8(void *w) {
    Pos pos;
    Pos *p = &pos;
    u32 word = *ov13_0224F4BC->b;
    p->x = (word & 0x1ff0000) >> 16;
    p->y = (word & 0xff) - 12;
    if (p->y > 0x67) {
        ov13_022290F0(ov13_0224F4BC->mode, 1, p->y);
        return;
    }
    ov13_022290F0(ov13_0224F4BC->mode, 1, 0x67);
    ov13_022290F0(ov13_0224F4BC->mode, 2, 0xc0);
    WifiSetupTask_SetFunc(w, ov13_0222856C);
}
// @fn ov13_0222856C
void ov13_0222856C(void *w) {
    Pos pos;
    Pos *p = &pos;
    u32 word = *ov13_0224F4BC->c;
    p->x = (word & 0x1ff0000) >> 16;
    p->y = (word & 0xff) - 12;
    if (p->y > 0x7d) {
        ov13_022290F0(ov13_0224F4BC->mode, 2, p->y);
        return;
    }
    ov13_022290F0(ov13_0224F4BC->mode, 2, 0x7d);
    ov13_022290F0(ov13_0224F4BC->mode, 3, 0xc0);
    WifiSetupTask_SetFunc(w, ov13_02228600);
}
