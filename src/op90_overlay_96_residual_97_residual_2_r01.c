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
} Item;

extern u32 ov96_0221CE14[];
extern int ov96_0221CE24[];
extern u32 ov96_0221CE34[];
extern int ov96_0221CE48[];

u32 MTRandom(void);
BOOL ov96_0220CC38(Item *item);

// @fn ov96_0220CC38
BOOL ov96_0220CC38(Item *item) {
    int sum = 0;
    u8 roll = MTRandom() % 100;
    int i;

    GF_ASSERT(item != NULL);
    for (i = 0; i < 5; i++) {
        if (item->slot[item->state.kind].val >= ov96_0221CE34[i]) {
            sum += ov96_0221CE48[i];
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (item->state.mid >= ov96_0221CE14[i]) {
            sum += ov96_0221CE24[i];
            break;
        }
    }
    if (roll < sum) {
        return TRUE;
    }
    return FALSE;
}
