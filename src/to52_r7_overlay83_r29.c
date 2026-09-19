#include "to52_r7_overlay83_r29_internal.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
void ov83_02244CDC(void *work) {
    PlaySE(0x5DC);
    if (ov80_02237D8C(U8(work, 9)) == 1) {
        ov83_022450A8(work, 0x16, U8(work, 0xD));
    }
    ov83_02244D0C(work, U8(work, 0xD), 0);
}
void ov83_02244D0C(void *work, u32 index, int flag) {
    void *sprite;
    int sequence;
    int sequence2;
    int x, y;
    if (flag == 0) {
        sprite = PTR(work, 0x508);
        sequence = 1;
        sequence2 = 0;
    } else {
        sprite = PTR(work, 0x50C);
        sequence = 2;
        sequence2 = 0x11;
    }
    if (index >= U8(work, 0x15)) {
        ov83_022475D4(sprite, sequence2);
        ov83_02247568(sprite, 0xE0, 0xA0);
        if (flag == 0) {
            ov83_02246938(work, 0);
        }
    } else {
        ov83_022475D4(sprite, sequence);
        ov83_02244DA0(work, (u32 *)&x, (u32 *)&y, index);
        ov83_02247568(sprite, (u16)x, (u16)y);
        if (flag == 0) {
            if (U8(work, 0xC) >= U8(work, 0x15)) {
                ov83_02246938(work, 1);
            }
            ov83_0224691C(work, 0);
        }
    }
}

void ov83_02244DA0(void *work, u32 *x, u32 *y, u32 index) {
    if (ov80_02237D8C(U8(work, 9)) == 1) {
        if (index == 0) {
            *x = 0x28;
        } else if (index == 1) {
            *x = 0x68;
        } else if (index == 2) {
            *x = 0xA8;
        } else {
            *x = 0xE8;
        }
    } else {
        if (index == 0) {
            *x = 0x48;
        } else if (index == 1) {
            *x = 0x88;
        } else {
            *x = 0xC8;
        }
    }
    *y = 0x58;
}
