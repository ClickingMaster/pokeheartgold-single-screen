#include "global.h"
int GF_IsAnySEPlaying(void);
void ov07_0221F39C(u8 *);
void ov07_0221F3F4(u8 *);

void ov07_0221F39C(u8 *state) {
    if (*(u16 *)(state + 0x90)) {
        state[0x8D] = 1;
        state[0x17D] = 0;
        return;
    }
    if (GF_IsAnySEPlaying()) {
        state[0x17D]++;
        if (state[0x17D] > 0x5A) {
            state[0x17D] = 0;
            return;
        }
        state[0x8D] = 1;
        return;
    }
    state[0x8D] = 0;
    state[0x17D] = 0;
    *(u8 **)(state + 0x18) += 4;
}
