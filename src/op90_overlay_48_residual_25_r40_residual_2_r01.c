#pragma force_active on
#include "global.h"

typedef struct Ov48Entry {
    u8 unk0[0x10];
} Ov48Entry;

typedef struct Ov48Unk {
    Ov48Entry entries[3];
    u8 unk30[4];
    u8 flags[4];
    u8 types[4];
} Ov48Unk;

extern const Ov48Entry ov48_0225B330[][3];

u8 ov48_0225B050(u8 value);
void ov48_0225AE3C(Ov48Entry *entry, const Ov48Entry *src);
void ov48_0225AE5C(Ov48Entry *entry, int a1);
void ov48_0225AE58(Ov48Entry *entry, int a1);
void ov48_0225ACD8(Ov48Unk *data, u32 index, u8 value, u8 next);

// @fn ov48_0225ACD8
void ov48_0225ACD8(Ov48Unk *data, u32 index, u8 value, u8 next) {
    data->flags[index] = 1;
    data->types[index] = ov48_0225B050(value);
    ov48_0225AE3C(&data->entries[index], &ov48_0225B330[data->types[index]][index]);
    if (next < 24) {
        u8 type = ov48_0225B050(next);
        if (type == data->types[index] && index < 2) {
            ov48_0225AE5C(&data->entries[index + 1], 1);
            ov48_0225AE58(&data->entries[index], 1);
        }
    }
}
