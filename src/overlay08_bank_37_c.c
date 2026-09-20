#include "overlay08_exact_bank_private.h"

#pragma force_active on

Ov08SmallState *ov08_02224B64(u32 heapId) {
    Ov08SmallState *state = Heap_Alloc(heapId, sizeof(Ov08SmallState));
    u8 *cursor = (u8 *)state;
    u32 size = sizeof(Ov08SmallState);
    do {
        *cursor++ = 0;
    } while (--size != 0);
    return state;
}

void ov08_02224B7C(Ov08SmallState *state) {
    Heap_Free(state);
}

void *ov08_02224B84(const Ov08SmallState *state) {
    return state->owner;
}

u8 ov08_02224B88(const Ov08SmallState *state) {
    return state->index;
}

u8 ov08_02224B8C(const Ov08SmallState *state) {
    return state->state;
}

void ov08_02224B90(Ov08SmallState *state, u8 value) {
    state->state = value;
}

void ov08_02224B94(Ov08SmallState *state, void *owner) {
    state->owner = owner;
}
