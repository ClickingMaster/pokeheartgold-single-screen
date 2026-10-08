#pragma force_active on
#include "global.h"

typedef struct Ov49Entry {
    u8 unk0[0x78];
} Ov49Entry;

typedef struct Ov49Source {
    u8 unk0[8];
    u32 unk8;
    Ov49Entry entries[16];
} Ov49Source;

typedef struct Ov49Item {
    u8 unk0[0xEC];
} Ov49Item;

typedef struct Ov49Unk {
    Ov49Item items[4];
    u32 unk3B0;
    u16 unk3B4;
    u16 unk3B6;
} Ov49Unk;

void ov49_02267EBC(Ov49Item *item, Ov49Entry *e0, Ov49Entry *e1, Ov49Entry *e2, Ov49Entry *e3, u32 a5, u32 a6);
void ov49_02267D98(Ov49Unk *data, u32 a1, u16 a2, u16 a3, u32 a4, Ov49Source *src);

// @fn ov49_02267D98
void ov49_02267D98(Ov49Unk *data, u32 a1, u16 a2, u16 a3, u32 a4, Ov49Source *src) {
    int i;
    data->unk3B0 = a1;
    data->unk3B4 = a2;
    data->unk3B6 = a3;
    for (i = 0; i < 4; i++) {
        ov49_02267EBC(&data->items[i], &src->entries[i * 4], &src->entries[i * 4 + 1], &src->entries[i * 4 + 2], &src->entries[i * 4 + 3], src->unk8, a4);
    }
}
