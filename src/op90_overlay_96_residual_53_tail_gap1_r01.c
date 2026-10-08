#pragma force_active on
#include "global.h"

typedef struct Entry {
    u8 unk00[0x18];
    u32 animation;
    u8 unk1C[0xC];
    VecFx32 position;
    u8 unk34[0xC];
    u8 shape;
    u8 unk41[3];
    u8 active;
    u8 unk45[0x4B];
} Entry;

typedef struct Work {
    u8 unk00[0x20];
    Entry entries[4][3];
    u32 values[4];
    u8 unk6F0[0x34];
    u8 ring;
    u8 unk725;
    u8 field726;
    u8 field727;
    u8 unk728;
    u8 field729;
    u8 unk72A[8];
    u16 field732;
    u8 unk734[0x14];
    u8 field748;
    u8 unk749;
    u8 field74A;
    u8 field74B;
    u8 field74C;
} Work;

typedef struct Packet {
    u8 x[12];
    u8 y[12];
    u32 shape;
    u32 animation;
    u32 active;
} Packet;

void ov96_021F0F04(Work *work, Packet *packet);

// @fn ov96_021F0F04
void ov96_021F0F04(Work *work, Packet *packet) {
    u8 i;
    int x, y;
    u32 value;
    int index;
    Entry (*entries)[3];
    u32 shape, animation, active;

    shape = 0;
    animation = 0;
    active = 0;
    i = 0;
    entries = work->entries;
    for (; i < 12; i++) {
        Entry *entry = &entries[(int)i / 3][(int)i % 3];

        x = entry->position.x / FX32_ONE;
        y = entry->position.y / FX32_ONE;
        if (x < 0) {
            x = 0;
        } else if (x > 255) {
            x = 255;
        }
        if (y < 0) {
            y = 0;
        } else if (y > 255) {
            y = 255;
        }
        {
            u8 anim = entry->animation;
            u8 kind = entry->shape;

            packet->x[i] = x;
            packet->y[i] = y;
            shape += ((u32)kind - 1) << (2 * i);
            animation += (u32)anim << (2 * i);
        }
        active += entry->active << i;
    }

    packet->shape = shape;
    packet->shape += (u32)work->field726 << 24;
    packet->animation = 0;
    packet->animation += animation;
    packet->animation += (u32)work->field732 << 24;
    packet->animation += (u32)work->field74B << 28;

    index = work->ring;
    value = work->values[index];
    work->ring = (index + 1) % 4;

    packet->active = 0;
    packet->active += active;
    packet->active += (u32)work->field729 << 12;
    packet->active += (u32)work->field748 << 13;
    packet->active += (u32)work->field74A << 14;
    packet->active += (u32)work->field74C << 15;
    packet->active += (u8)value << 18;
    packet->active += (u32)index << 26;
    packet->active += (u32)work->field727 << 28;

    work->field748 = 0;
    work->field74B = 12;
    work->field74C = 0;
}
