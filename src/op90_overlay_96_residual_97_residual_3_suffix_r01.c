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
        u32 low : 16;
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

void ov96_0220D0F8(Item *item);

// @fn ov96_0220D0F8
void ov96_0220D0F8(Item *item) {
    Slot *slot = &item->slot[item->state.kind];

    item->flags.flag = 0;
    item->timer.middle = 0;
    item->state.mid = 0;
    slot->val = 10;
}
