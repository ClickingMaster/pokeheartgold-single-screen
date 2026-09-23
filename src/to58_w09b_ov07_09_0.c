#include "global.h"

extern const s16 ov07_02237254[];
extern void GF_AssertFail(void);
extern void ManagedSprite_SetAffineZRotation(void *, int);
extern void ManagedSprite_OffsetPositionXY(void *, s16, s16);
extern void ManagedSprite_OffsetAffineZRotation(void *, int);
extern void sub_0200602C(int, int);
extern int _s32_div_f(int, int);
extern void ov07_02232F74(void *, int);
int ov07_022332F4(u8 *);

int ov07_022332F4(u8 *state) {
    switch (*(int *)(state + 8)) {
    case 0:
        if (*(int *)(state + 0x18) >= 3) GF_AssertFail();
        *(int *)(state + 8) += 1;
    case 1: {
        int row = *(int *)(state + 0x18);
        const s16 *rowptr = (const s16 *)((const u8 *)ov07_02237254 + row * 24);
        int index = *(int *)(state + 0xc);
        s16 amount = rowptr[index];
        *(int *)(state + 0xc) += 1;
        if (*(int *)(state + 0xc) > 11 || amount == 0xff) {
            *(int *)(state + 8) += 1;
            *(int *)(state + 0x18) += 1;
            ManagedSprite_SetAffineZRotation(*(void **)(state + 0x30), 0);
            ov07_02232F74(state, 18);
            return 1;
        }
        if (*(int *)(state + 0xc) == 5) sub_0200602C(0x5fd, 0x75);
        ManagedSprite_OffsetPositionXY(*(void **)(state + 0x30), amount, 0);
        int delta = _s32_div_f(0x1fffe * amount, 360);
        ManagedSprite_OffsetAffineZRotation(*(void **)(state + 0x30), delta);
        break;
    }
    }
    return 1;
}
