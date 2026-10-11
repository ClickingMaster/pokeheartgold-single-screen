#include "to57_overlay91_058_private.h"

void ov91_02260254(u8 *p, u16 value) {
    u32 idx = GF_DegreeToSinCosIdx(0xF222);
    u32 a = (90 * (value - 0xE38)) / idx;
    s32 b = _s32_div_f((s32)((45 << 14) * a), 90);
    b = (45 << 14) - b;
    ModelAnimation_SetFrame(p + 0x1AC, b);
}

void ov91_02260298(u8 *raw, u32 n) {
    T57Work *p = (T57Work *)raw;
    u16 old;
    if (n >= 6) {
        GF_AssertFail();
    }
    old = p->current;
    if (old != n) {
        ModelInstance_RemoveAnimation(&p->anim[ov91_0226274C[old]], &p->slots[old]);
        ModelInstance_AddAnimation(&p->anim[ov91_0226274C[n]], &p->slots[n]);
        ModelAnimation_SetFrame(&p->slots[n], 0);
        ModelInstance_SetVisible(&p->anim[ov91_0226274C[p->current]], 0);
        ModelInstance_SetVisible(&p->anim[ov91_0226274C[n]], 1);
        p->current = n;
    }
}

u32 ov91_0226031C(u8 *p, u32 a) {
    return ModelAnimation_Advance(p + 0x198 + *(u16 *)(p + 0x210) * 0x14, a);
}

void ov91_02260334(u8 *p, int n) {
    u16 old = *(u16 *)(p + 0x212);
    if (old != n) {
        if (n == 1) {
            ModelInstance_AddAnimation(p, p + 0x198);
        } else {
            ModelInstance_RemoveAnimation(p, p + 0x198);
        }
        *(u16 *)(p + 0x212) = n;
    }
}
