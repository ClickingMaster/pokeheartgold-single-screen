#include "to50_overlay65_residual13_private.h"

void ov65_0221F780(void *p) {
    Ov65StateR13 *state = p;
    if (state->field36B4 != NULL) {
        sub_0200F450(state->field36B4);
        state->field36B4 = NULL;
    }
}

s32 ov65_0221F79C(void *p) {
    s32 total = 0;
    s32 count = Party_GetCount(PTR(p, 0x2224));
    s32 i = 0;
    s32 selected;
    if (count > 0) {
        selected = U32(p, 0x94);
        do {
            if (i != selected) {
                total += U8(p, 0x6a1);
            }
            i++;
            p = (u8 *)p + 0x10;
        } while (i < count);
    }
    if (total == count - 1) {
        return 0;
    }
    return 1;
}

s32 ov65_0221F7E0(void *p) {
    Ov65StateR13 *state = p;
    s32 count;
    s32 i;
    count = Party_GetCount(state->partyA);
    for (i = 0; i < count; i++) {
        if (GetMonData(Party_GetMonByIndex(state->partyA, i), 3, NULL) != 0) {
            return 1;
        }
    }
    count = Party_GetCount(state->partyB);
    for (i = 0; i < count; i++) {
        if (GetMonData(Party_GetMonByIndex(state->partyB, i), 3, NULL) != 0) {
            return 2;
        }
    }
    return 0;
}
