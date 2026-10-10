#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define I(p, o) (*(int *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))
#define V(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 *ov13_0224F5C8;
extern int ov13_0223D45C(void);
extern int ov13_0223D470(int);
extern int ov13_0223CF80(void);
extern int ov13_0223DF28(void);
extern void ov13_0223CB34(void *, u16);
extern void ov13_0223CFE8(void);
extern void ov13_0223D0E0(void);
extern void ov13_0223E7BC(void);
void ov13_0223EF78(void);

void ov13_0223EF78(void) {
    int state;
    switch (ov13_0223D45C()) {
    case 1:
        ov13_0223CB34(ov13_0224F5C8 + 0xab4, H(ov13_0224F5C8, 0x64a));
        break;
    case 2:
        if (ov13_0223D470(2)) {
            B(ov13_0224F5C8, 0xa90) = 5;
            return;
        }
        if (ov13_0223D470(3) || ov13_0223D470(4)) {
            B(ov13_0224F5C8, 0xa90) = 6;
            return;
        }
        if (B(ov13_0224F5C8, 0xa90) == 5 && !ov13_0223D470(2)) {
            B(ov13_0224F5C8, 0xa90) = 13;
        }
        break;
    case 3:
        if (ov13_0223CF80()) {
            ov13_0223CFE8();
            return;
        }
        if ((u8)(B(ov13_0224F5C8, 0xa90) + 250) <= 1 && !ov13_0223D470(3)) {
            B(ov13_0224F5C8, 0xa90) = 18;
        }
        break;
    case 5:
        B(ov13_0224F5C8, 0xa90) = 7;
        break;
    case 7:
        ov13_0223D0E0();
        B(ov13_0224F5C8, 0xa90) = 1;
        break;
    case 0:
        state = ov13_0223DF28();
        switch (state) {
        case 1:
            ov13_0223E7BC();
            break;
        case 0:
            B(ov13_0224F5C8, 0xa90) = 31;
            break;
        case 3:
            break;
        default:
            B(ov13_0224F5C8, 0xa90) = 31;
            break;
        }
        break;
    }
}
