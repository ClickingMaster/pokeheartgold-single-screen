#include "global.h"

typedef union Ov96R118FlagsW14 {
    u32 raw;
    struct {
        u32 field00 : 12;
        u32 field12 : 2;
        u32 field14 : 2;
        u32 field16 : 4;
        u32 field20 : 2;
        u32 field22 : 2;
        u32 field24 : 2;
        u32 bit26 : 1;
        u32 bit27 : 1;
        u32 bit28 : 1;
        u32 bit29 : 1;
        u32 bit30 : 1;
        u32 bit31 : 1;
    } bits;
} Ov96R118FlagsW14;

typedef struct Ov96R118ObjectW14 {
    void *unk00;
    void *unk04;
    void *unk08;
    void *unk0C;
    void *unk10;
    s32 state;
    u8 pad18[8];
    s32 vec20[3];
    s32 vec2C[3];
    s32 vec38[3];
    s32 vec44[3];
    s32 unk50;
    s32 unk54;
    s8 unk58;
    s8 unk59;
    s8 unk5A;
    s8 unk5B;
    s8 unk5C;
    s8 unk5D;
    s8 unk5E;
    u8 unk5F;
    Ov96R118FlagsW14 flags;
    u32 timer;
    u8 pad68[0x40];
} Ov96R118ObjectW14;

typedef struct Ov96R118PacketW14 {
    u8 x;
    u8 y;
    u8 speed : 6;
    u8 field14 : 2;
    u8 field00;
    u8 state : 4;
    u8 field12 : 2;
    u8 field22 : 2;
    u8 unk5F : 3;
    u8 field16 : 2;
    u8 field24 : 2;
    u8 pad : 1;
} Ov96R118PacketW14;

typedef union Ov96R118PacketFlagsW14 {
    u32 raw;
    struct {
        u32 low : 8;
        u32 states : 24;
    } bits;
} Ov96R118PacketFlagsW14;

extern void *PokeathlonCourse_GetDataCopyArea(void *course);
extern void *ov96_021E8A20(void *p);
void ov96_02218330(void *base, void *course);

void ov96_02218330(void *base, void *course) {
    Ov96R118PacketW14 *copy;
    int any;
    Ov96R118ObjectW14 *record;
    Ov96R118PacketW14 *packet;
    int i;
    Ov96R118PacketFlagsW14 *packetFlags;

    any = 0;
    copy = ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course) + 0x28);

    for (i = 0, record = (Ov96R118ObjectW14 *)((u8 *)base + 0x1C), packet = copy; i < 4; i++, record++, packet++) {
        s32 x = (record->vec2C[0] << 4) >> 16;
        s32 y = (record->vec2C[1] << 4) >> 16;

        if (record->timer != 0) {
            record->timer--;
        } else if (record->timer == 0) {
            record->flags.bits.field12 = 0;
            record->flags.bits.field14 = 0;
        }

        packet->field12 = record->flags.bits.field12;
        packet->field14 = record->flags.bits.field14;
        packet->field22 = record->flags.bits.field22;
        packet->field16 = record->flags.bits.field16 - 1;

        if (x > 255) {
            x = 255;
        } else if (x < 0) {
            x = 0;
        }
        packet->x = x;

        if (y > 255) {
            y = 255;
        } else if (y < 0) {
            y = 0;
        }
        packet->y = y;

        packet->speed = record->unk50 >> 12;
        packet->state = record->state;
        packet->field00 = record->flags.bits.field00;
        packet->field24 = record->flags.bits.field24;
        packet->unk5F = record->unk5F;

        if (record->flags.bits.bit31) {
            any = 1;
            record->flags.bits.bit31 = 0;
        }
    }

    packetFlags = (Ov96R118PacketFlagsW14 *)((u8 *)copy + 0x1C);
    packetFlags->raw = (packetFlags->raw & ~0x80) | (((u32)any << 31) >> 24);
    packetFlags->raw = (u8)packetFlags->raw;

    for (i = 0; i < 12; i++) {
        int row = i / 3;
        int column = i % 3;
        s32 value = *(s32 *)((u8 *)base + row * 0xA8 + column * 0x10 + 0x8C) >> 12;
        int state;

        if (value == 0) {
            state = 2;
        } else if (value < 30) {
            state = 1;
        } else {
            state = 0;
        }
        packetFlags->bits.states += state << (i * 2);
    }
}
