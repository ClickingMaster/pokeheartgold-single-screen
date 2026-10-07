#pragma force_active on
#include "global.h"

typedef struct Slot {
    u32 lo : 18;
    u32 val : 8;
    u32 hi : 6;
} Slot;

typedef struct Item {
    u32 unk0;
    Slot slot[4];
    struct {
        u32 low : 8;
        u32 count : 8;
        u32 mid : 8;
        u32 state : 6;
        u32 kind : 2;
    } state;
    struct {
        u32 counter : 11;
        u32 middle : 11;
        u32 duration : 8;
        u32 kind : 2;
    } timer;
    struct {
        u32 count : 9;
        u32 flag : 1;
        u32 rest : 22;
    } flags;
} Item;

typedef struct Packet {
    u32 low : 8;
    u32 val : 8;
    u32 mid : 8;
    u32 kind : 2;
    u32 flag : 1;
    u32 extra : 1;
    u32 single : 1;
    u32 rest : 3;
} Packet;

u8 *PokeathlonCourse_GetDataCopyArea(void *course);
u8 *ov96_021E8A20(void *ptr);
void ov96_0220CF50(Item *item, u32 value, void *course);

// @fn ov96_0220CF50
void ov96_0220CF50(Item *item, u32 value, void *course) {
    Packet *packet = (Packet *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x50 + item->timer.kind * 0x28);

    Slot *slot = &item->slot[item->state.kind];

    packet->low = item->state.low;
    packet->val = slot->val;
    packet->mid = item->state.mid;
    packet->flag = item->flags.flag;
    packet->extra = value & 2;
    packet->kind = item->state.kind;
    packet->single = item->state.state == 1;
}
