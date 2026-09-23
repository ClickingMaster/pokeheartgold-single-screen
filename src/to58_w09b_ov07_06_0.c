#include "global.h"

extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void ov07_02222268(void *, s16, s16, s16, s16, int);
extern int ov07_022222F0(void *, void *);
extern void ov07_02232F74(void *, int);
int ov07_022331B0(u8 *);

int ov07_022331B0(u8 *state) {
    switch (*(int *)(state + 8)) {
    case 0:
        *(int *)(state + 0xc) += 1;
        if (*(int *)(state + 0xc) >= 1) {
            s16 y, x;
            ManagedSprite_GetPositionXY(*(void **)(state + 0x30), &y, &x);
            ov07_02222268(state + 0x48, y, y, x, (s16)(x + 32), 10);
            *(int *)(state + 8) += 1;
        }
        break;
    case 1:
        if (ov07_022222F0(state + 0x48, *(void **)(state + 0x30)) == 0) {
            *(int *)(state + 8) += 1;
            ov07_02232F74(state, 13);
        }
        break;
    }
    return 1;
}
