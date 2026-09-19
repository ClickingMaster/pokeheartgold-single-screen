#include "to52_r7_overlay87_r02_internal.h"

void ov87_021E6668(W07State *state) {
    int i;
    u8 *cursor = (u8 *)state;
    const W07Pair16 *table = ov87_021E82E4;
    void *manager;
    i = 0;
    manager = (u8 *)state + 0x16C;
    do {
        W07SpriteWrap *sprite = ov87_021E7F6C(manager, 3, 2, table->a, table->b, 0, 2, 0xB);
        *(W07SpriteWrap **)(cursor + 0x344) = sprite;
        ov87_021E7FD4(*(W07SpriteWrap **)(cursor + 0x344), 0);
        i++;
        table++;
        cursor += 4;
    } while (i < 3);
}

void ov87_021E66B8(W07State *state) {
    int i;
    u8 *cursor = (u8 *)state;
    const W07Pair16 *table = ov87_021E8194;
    void *manager;
    i = 0;
    manager = (u8 *)state + 0x16C;
    do {
        W07SpriteWrap *sprite = ov87_021E7F6C(manager, 1, 0, table->a, table->b, 0, 0, 0);
        *(W07SpriteWrap **)(cursor + 0x350) = sprite;
        ov87_021E7FD4(*(W07SpriteWrap **)(cursor + 0x350), 0);
        i++;
        table++;
        cursor += 4;
    } while (i < 3);
}

void ov87_021E6704(W07State *state) {
    W07SpriteWrap *sprite;
    sprite = ov87_021E7F6C((u8 *)state + 0x16C, 3, 1, 0x44, 0xA0, 0, 1, 0);
    *(W07SpriteWrap **)((u8 *)state + 0x33C) = sprite;
    ov87_021E7FD4(*(W07SpriteWrap **)((u8 *)state + 0x33C), 0);
    sprite = ov87_021E7F6C((u8 *)state + 0x16C, 3, 3, 0x44, 0x44, 0, 1, 0);
    *(W07SpriteWrap **)((u8 *)state + 0x340) = sprite;
    ov87_021E7FD4(*(W07SpriteWrap **)((u8 *)state + 0x340), 0);
}
