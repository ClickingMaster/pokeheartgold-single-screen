#include "global.h"
int ov07_0221C3DC(void *);
void NARC_Delete(void *);
void Heap_Free(void *);
int ov07_0221BFE0(u8 *);

int ov07_0221BFE0(u8 *state) {
    if (!ov07_0221C3DC(state)) return 0;
    int offset = 0x1B0;
    int i = 0;
    u8 *cursor = state;
    for (; i < 6; i++) {
        NARC_Delete(*(void **)(cursor + offset));
        cursor += 4;
    }
    Heap_Free(*(void **)(state + 0xC0));
    Heap_Free(state);
    return 1;
}
