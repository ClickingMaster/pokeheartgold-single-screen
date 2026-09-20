#include "to55_r2_t08_private.h"

void ov13_02222638(Ov13CipherStateT08 *state, const u8 *key, u32 keySize, u32 tableSize) {
    u8 *table = state->table;
    u32 i;
    u32 keyIndex;
    u32 value;

    state->unk04 = 0;
    state->unk00 = 0;
    state->tableSize = tableSize;
    for (i = 0; i < tableSize; i++) {
        table[i] = i;
    }
    value = 0;
    keyIndex = 0;
    for (i = 0; i < tableSize; i++) {
        u8 saved = table[i];
        u8 replacement;
        value = (value + key[keyIndex] + saved) % state->tableSize;
        replacement = table[value];
        table[value] = saved;
        table[i] = replacement;
        keyIndex++;
        if (keyIndex >= keySize) {
            keyIndex = 0;
        }
    }
}

void ov13_0222269C(Ov13CipherStateT08 *state, u8 *dst, const u8 *src, u32 size) {
    u32 i;

    for (i = 0; i < size; i++) {
        u8 value = ov13_022226C8(state);
        dst[i] = src[i] ^ value;
    }
}
