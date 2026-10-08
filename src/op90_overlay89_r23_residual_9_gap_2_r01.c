#pragma force_active on
#include "global.h"
typedef struct Entry {
    u8 a;
    u8 b;
    u8 pad[2];
} Entry;
typedef struct State {
    u8 pad0[0xB0];
    u8 buf[4];
    u8 padB4[0x53C - 0xB4];
    Entry entries[128];
    u8 x73C;
    u8 x73D;
    u8 x73E;
    u8 x73F;
} State;
extern const s16 ov89_0225CCD0[];
void MI_CpuFill8(void *, u8, u32);
BOOL ov89_0225AF10(State *, u8, u8);
#pragma opt_common_subs off
// @fn ov89_0225AF10
BOOL ov89_0225AF10(State *s, u8 idx, u8 val) {
    if (idx == 0xFF) {
        return FALSE;
    }
    s->entries[idx].a++;
    s->entries[idx].b++;
    if (s->x73C == 0 && s->x73D == 0 && s->entries[idx].a >= ov89_0225CCD0[s->x73F]) {
        s->x73D = val;
        MI_CpuFill8(s->buf, 0, 4);
        return TRUE;
    }
    return FALSE;
}
