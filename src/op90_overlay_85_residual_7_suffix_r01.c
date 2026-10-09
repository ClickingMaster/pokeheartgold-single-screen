#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad0[0x1c];
    fx32 angle;
} Ov85Obj;
typedef struct {
    int state;
    u8 pad4[0x28 - 4];
    Ov85Obj *obj;
    int idx;
    int count;
    u8 pad34[0xd4 - 0x34];
    u8 unkD4[4];
} Ov85Work;
extern const u16 ov85_021EA7C4[][5];
extern void ov85_021E7644(void *, fx32);
extern void ov85_021E78A4(Ov85Work *, fx32);
extern void ov85_021E8530(fx32 *, fx32);
BOOL ov85_021E6224(Ov85Work *w);
// @fn ov85_021E6224
BOOL ov85_021E6224(Ov85Work *w) {
    fx32 cur;
    fx32 target;
    fx32 step;
    fx32 d;
    int idx = w->idx;
    int count = w->count;
    step = FX32_CONST(4);
    cur = w->obj->angle;
    target = ov85_021EA7C4[count][idx] << 12;
    d = target - cur;
    if (d < 0) {
        d = target + FX32_CONST(360) - cur;
    }
    if (d <= FX32_CONST(16)) {
        step >>= 1;
    }
    if (d < step) {
        step = d;
    }
    ov85_021E7644(w->unkD4, step);
    ov85_021E78A4(w, step);
    ov85_021E8530(&cur, step);
    if (cur / FX32_ONE == target / FX32_ONE) {
        w->state = 32;
    }
    return FALSE;
}
