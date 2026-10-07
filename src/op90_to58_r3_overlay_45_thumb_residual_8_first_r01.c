#pragma force_active on
#include "global.h"
#include "assert.h"

typedef struct Ov45Unk {
    u8 unk0[0x4C];
    u8 slots[12];
} Ov45Unk;

u8 ov45_0222A92C(Ov45Unk *data, u32 index);

#pragma opt_common_subs off
// @fn ov45_0222A92C
u8 ov45_0222A92C(Ov45Unk *data, u32 index) {
    int count;
    int i;

    GF_ASSERT(index < 12);
    count = 0;
    for (i = 0; i < 12; i++) {
        if (data->slots[i] < 24) {
            count++;
        }
        if (count - 1 == index) {
            return data->slots[i];
        }
    }
    return 24;
}
#pragma opt_common_subs reset
