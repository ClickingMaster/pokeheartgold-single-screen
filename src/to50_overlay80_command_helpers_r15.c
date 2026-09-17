#include "to50_overlay80_command_helpers_r15_private.h"

BOOL ov80_0222CA94(void *c) {
    void *p;
    void *sprite;
    u16 *id;

    p = sub_0209680C(PTR(PTR(c, 0), 0));
    id = (u16 *)((u8 *)c + 0x78);
    sprite = ov80_02239700(p, *id);
    if (!ov80_02239734(p, *id) || !ManagedSprite_IsAnimated(sprite)) {
        return TRUE;
    }
    return FALSE;
}

void ov80_0222CAD0(void *c, int unused, int count) {
    void *sys;
    u16 *values;
    u16 var;
    int i;

    sys = PTR(c, 0);
    if (count > 7) {
        GF_AssertFail();
    }
    values = Heap_Alloc(U32(sys, 0x34), count * 2);
    var = FrontierScript_ReadVar(c);
    for (i = 0; i < 3; i++) {
        values[i] = FrontierScript_ReadVar(c);
    }
    ov80_02239BF0(PTR(sys, 0), var, 1, values, 3);
    Heap_Free(values);
}
