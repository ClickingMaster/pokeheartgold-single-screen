#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 *ov00_021F955C(void);
extern int ov00_021EE568(void);
extern int ov00_021F631C(void *);
extern int ov00_021F7158(u32);
extern int ov00_021F6100(void *);
extern void ov00_021F7294(u8, int);
extern void ov00_021F7128(void);
extern void ov00_021F6EC4(int);
extern void ov00_021F64C0(int);
int ov00_021F7384(void);

int ov00_021F7384(void) {
    u64 elapsed;
    if (W(ov00_021F955C(), 0x1a0) == 9 || W(ov00_021F955C(), 0x1a0) == 16 || W(ov00_021F955C(), 0x1a0) == 17) {
        u64 now = OS_GetTick();
        elapsed = ((now - *(u64 *)(ov00_021F955C() + 0x1e0)) << 6) / 33514;
    } else {
        return 1;
    }
    switch (W(ov00_021F955C(), 0x1a0)) {
    case 9:
        if (elapsed > 6000) {
            if (ov00_021EE568() == 5 && B(ov00_021F955C(), 0x1ad) >= 5) {
                if (!ov00_021F631C(P(ov00_021F955C(), 0xf4))) {
                    return 0;
                }
            } else {
                B(ov00_021F955C(), 0x1ad)
                ++;
                ov00_021F7294(B(ov00_021F955C(), 0x2d0), 3);
            }
        }
        break;
    case 16:
        if (elapsed > 6000) {
            B(ov00_021F955C(), 0x1ab)
            ++;
            if (B(ov00_021F955C(), 0x1ab) > 5) {
                if (B(ov00_021F955C(), 21) == 0 || B(ov00_021F955C(), 21) == 1) {
                    ov00_021F7128();
                    ov00_021F6EC4(1);
                } else {
                    if (!ov00_021F7158(W(ov00_021F955C(), 0x1d4))) {
                        return 0;
                    }
                    if (B(ov00_021F955C(), 13)) {
                        u8 *w;
                        B(ov00_021F955C(), 0x1ab) = 0;
                        w = ov00_021F955C();
                        *(u64 *)(w + 0x1e0) = OS_GetTick();
                    } else if (!ov00_021F6100(P(ov00_021F955C(), 0x20c))) {
                        return 0;
                    }
                }
            } else {
                int i;
                for (i = 1; i <= B(ov00_021F955C(), 13); i++) {
                    u8 *w = ov00_021F955C();
                    if (!(W(w, 0x1d4) & (1 << B(ov00_021F955C() + i, 0x2d0)))) {
                        ov00_021F7294(B(ov00_021F955C() + i, 0x2d0), 2);
                    }
                }
            }
        }
        break;
    case 17:
        if (elapsed > H(ov00_021F955C(), 0x1b0)) {
            ov00_021F64C0(4);
        }
        break;
    }
    return 1;
}
