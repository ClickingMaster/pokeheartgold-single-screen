#include "to56_r2_w21_overlay120_grid_step_private.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

#define DRAW_CELL(work, value, bottom) \
    ov120_0225F040(PTR((u8 *)(work) + (value) * 4, 4), ((value) % 8) * 32 + 16, ((value) / 8) * 32 + 16, U8((work), 0xc7), PTR((work), 0), NULL, (bottom) ? NULL : (void *)32, (void *)32, (void *)32, U8((work), 0xc9))

int ov120_0225F318(void *work) {
    int i;
    int value;
    int result;
    int other;

    if (work == NULL) {
        GF_AssertFail();
    }
    if (U8(work, 0xca) == 0) {
        return 1;
    }
    if (U8(work, 0xc4) < 24) {
        S8(work, 0xc8)
        --;
        if (S8(work, 0xc8) <= 0) {
            S8(work, 0xc8) = U8(work, 0xc6);
            if (((Ov120GridWork *)work)->flip != 0) {
                value = ov120_0226032C[U8(work, 0xc4)];
            } else {
                value = ov120_022602FC[U8(work, 0xc4)];
            }
            DRAW_CELL(work, value, 0);
            if (((Ov120GridWork *)work)->flip != 0) {
                value = ov120_0226032C[U8(work, 0xc4)];
            } else {
                value = ov120_022602FC[U8(work, 0xc4)];
            }
            value = 47 - value;
            DRAW_CELL(work, value, 0);
            U8(work, 0xc4)
            ++;
        }
    }
    for (i = U8(work, 0xc5); i < U8(work, 0xc4); i++) {
        if (((Ov120GridWork *)work)->flip != 0) {
            value = ov120_0226032C[i];
            other = 47 - value;
        } else {
            value = ov120_022602FC[i];
            other = 47 - value;
        }
        result = ov120_0225F08C(PTR((u8 *)work + value * 4, 4));
        result |= ov120_0225F08C(PTR((u8 *)work + other * 4, 4));
        if (result == 1) {
            U8(work, 0xc5)
            ++;
        }
    }
    if (U8(work, 0xc5) >= 24 && result == 1) {
        U8(work, 0xca) = 0;
        return 1;
    }
    return 0;
}

int ov120_0225F4DC(void *work) {
    int i;
    int value;
    int result;
    int other;

    if (work == NULL) {
        GF_AssertFail();
    }
    if (U8(work, 0xca) == 0) {
        return 1;
    }
    if (U8(work, 0xc4) < 24) {
        S8(work, 0xc8)
        --;
        if (S8(work, 0xc8) <= 0) {
            S8(work, 0xc8) = U8(work, 0xc6);
            if (((Ov120GridWork *)work)->flip != 0) {
                value = ov120_02260314[U8(work, 0xc4)];
            } else {
                value = 47 - ov120_02260314[U8(work, 0xc4)];
            }
            DRAW_CELL(work, value, 1);
            if (((Ov120GridWork *)work)->flip != 0) {
                value = ov120_022602E4[U8(work, 0xc4)];
            } else {
                value = 47 - ov120_022602E4[U8(work, 0xc4)];
            }
            DRAW_CELL(work, value, 1);
            U8(work, 0xc4)
            ++;
        }
    }
    for (i = U8(work, 0xc5); i < U8(work, 0xc4); i++) {
        if (((Ov120GridWork *)work)->flip != 0) {
            value = ov120_02260314[i];
            other = ov120_022602E4[i];
        } else {
            value = 47 - ov120_02260314[i];
            other = 47 - ov120_022602E4[i];
        }
        result = ov120_0225F08C(PTR((u8 *)work + value * 4, 4));
        result |= ov120_0225F08C(PTR((u8 *)work + other * 4, 4));
        if (result == 1) {
            U8(work, 0xc5)
            ++;
        }
    }
    if (U8(work, 0xc5) >= 24 && result == 1) {
        U8(work, 0xca) = 0;
        return 1;
    }
    return 0;
}
