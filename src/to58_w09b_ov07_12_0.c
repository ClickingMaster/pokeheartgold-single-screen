#include "global.h"

extern void ManagedSprite_SetOamMode(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ov07_02232F74(void *, int);
int ov07_0223350C(u8 *);

int ov07_0223350C(u8 *state) {
    switch (*(int *)(state + 8)) {
    case 0:
        ManagedSprite_SetOamMode(*(void **)(state + 0x30), 1);
        *(int *)(state + 8) += 1;
    case 1:
        if (state[0x20] != 0) {
            state[0x20]--;
            state[0x21]++;
        } else {
            state[0x20] = 0;
            state[0x21] = 15;
            ManagedSprite_SetDrawFlag(*(void **)(state + 0x30), 0);
            *(int *)(state + 8) += 1;
        }
        *(volatile u16 *)0x04000052 = state[0x20] | (state[0x21] << 8);
        break;
    default:
        ov07_02232F74(state, 26);
        break;
    }
    return 1;
}
