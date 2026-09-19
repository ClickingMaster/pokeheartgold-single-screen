#include "private_w10.h"

u8 ov13_022226C8(Overlay13CipherState *state) {
    u8 *table = state->table;
    u8 x = (state->x + 1) % state->size;
    u8 tx = table[x];
    u8 y = (tx + state->y) % state->size;
    u8 ty = table[y];
    state->x = x;
    state->y = y;
    table[y] = tx;
    table[x] = ty;
    return table[(tx + ty) % state->size];
}
