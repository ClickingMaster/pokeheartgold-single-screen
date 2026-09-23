#include "global.h"

extern void ov07_02232F74(void *, int);
int ov07_022332C8(u8 *);

int ov07_022332C8(u8 *state) {
    int count = *(int *)(state + 0xc) + 1;
    *(int *)(state + 0xc) = count;
    if (count >= 14) {
        *(int *)(state + 8) += 1;
        *(int *)(state + 0xc) = 0;
        ov07_02232F74(state, 16);
    }
    return 1;
}
