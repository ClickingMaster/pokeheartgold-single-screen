#include "to52_r5_overlay96_r97_r3_r2_private.h"

void *ov96_0220D1A0(void *sys, void *mgr, u16 x, u16 y, u16 arg5, u16 arg6) {
    SpriteTemplateR5W19 t = ov96_0221CEB8;
    void *sprite;

    if (sys == NULL) {
        GF_AssertFail();
    }
    if (mgr == NULL) {
        GF_AssertFail();
    }
    t.x = x;
    t.y = y;
    t.arg5 = arg5;
    t.arg6 = arg6;
    sprite = SpriteSystem_NewSpriteWithYOffset(sys, mgr, &t, 0x100000);
    ManagedSprite_SetAnimateFlag(sprite, 1);
    return sprite;
}

void ov96_0220D200(PackedR5W19 *dst, u32 index, void *context) {
    u32 table[15];
    int i;
    RecordR5W19 *rec;

    ReadWholeNarcMemberByIdPair(table, 0xAA, 5);
    for (i = 0; i < 3; i++) {
        rec = ov96_021E60D8(context, index, i);
        if (rec->a >= 5) {
            GF_AssertFail();
        }
        if (rec->b >= 5) {
            GF_AssertFail();
        }
        if (rec->c >= 5) {
            GF_AssertFail();
        }
        dst->bits.low = table[rec->a];
        dst->bits.target = table[5 + rec->b];
        dst->bits.top = table[10 + rec->c];
        dst->bits.current = dst->bits.target;
        dst++;
    }
}
