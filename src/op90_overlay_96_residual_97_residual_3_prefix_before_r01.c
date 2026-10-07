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

extern u16 ov96_0221CDE8[];
extern s16 ov96_0221CDEE[];
extern u16 ov96_0221CDF4[];
extern s16 ov96_0221CDFE[];

u32 MTRandom(void);
BOOL ov96_0220CD00(Item *item);

// @fn ov96_0220CD00
BOOL ov96_0220CD00(Item *item) {
    int sum = 0;
    u8 roll = MTRandom() % 100;
    int i;

    GF_ASSERT(item != NULL);
    for (i = 0; i < 5; i++) {
        if (item->slot[item->state.kind].val < ov96_0221CDF4[i]) {
            sum += ov96_0221CDFE[i];
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (item->state.mid >= ov96_0221CDE8[i]) {
            sum += ov96_0221CDEE[i];
            break;
        }
    }
    if (roll < sum) {
        return TRUE;
    }
    return FALSE;
}
