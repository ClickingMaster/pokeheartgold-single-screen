#include "global.h"
void sub_020061EC(int);
int IsSEPlaying(int);
int ov07_0221C5C8(u8 *);

int ov07_0221C5C8(u8 *state) {
    u8 old = state[4];
    state[4] = old + 1;
    if (old < state[3]) {
        return 1;
    }
    state[4] = 0;
    *(int *)(state + 0x14) += *(int *)(state + 0x10);
    int result = 1;
    if (*(int *)(state + 0x10) == 0) {
        result = 0;
    } else if (*(int *)(state + 8) < *(int *)(state + 0xC)) {
        if (*(int *)(state + 0x14) >= *(int *)(state + 0xC)) result = 0;
    } else {
        if (*(int *)(state + 0x14) <= *(int *)(state + 0xC)) result = 0;
    }
    sub_020061EC(*(int *)(state + 0x14));
    if (!IsSEPlaying(*(u16 *)(state + 0x1A))) result = 0;
    return result;
}
