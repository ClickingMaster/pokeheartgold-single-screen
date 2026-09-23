#include "global.h"
int ov07_0221C3DC(void *);
int ov07_0221C394(u8 *);

int ov07_0221C394(u8 *state) {
    if (!ov07_0221C3DC(state)) return 0;
    (*(void (*)(void *))(*(void **)(state + 0xBC)))(state);
    return 1;
}
