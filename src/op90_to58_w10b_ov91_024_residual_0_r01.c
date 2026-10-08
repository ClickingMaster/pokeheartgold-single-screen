#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct Obj {
    Sprite *sprites[3];
    u32 timers[3];
    u8 pad18[0x3C - 0x18];
    int mode;
} Obj;
void ov91_0225D884(Obj *, int);
void ov91_0225D768(Obj *obj, int base, u32 anim, int kind);
// @fn ov91_0225D768
void ov91_0225D768(Obj *obj, int base, u32 anim, int kind) {
    int idx;
    BOOL found;
    u32 maxPrio;
    int i;
    u32 prio;
    int pal;
    found = FALSE;
    maxPrio = 0;
    idx = -1;
    for (i = 0; i < 3; i++) {
        if (Sprite_GetDrawFlag(obj->sprites[i]) == FALSE) {
            idx = i;
            found = TRUE;
        } else {
            prio = Sprite_GetDrawPriority(obj->sprites[i]);
            Sprite_SetDrawPriority(obj->sprites[i], prio + 1);
            if (found == FALSE && maxPrio <= prio + 1) {
                maxPrio = prio + 1;
                idx = i;
            }
        }
    }
    GF_ASSERT(idx >= 0);
    if (anim >= 3) {
        anim = 2;
    }
    if (kind != 2) {
        if (obj->mode == 1) {
            anim += 2;
        } else {
            anim += 8;
        }
        pal = base + 3;
    } else {
        if (obj->mode == 1) {
            anim += 5;
        } else {
            anim += 11;
        }
        pal = 7;
    }
    Sprite_SetAnimCtrlSeq(obj->sprites[idx], anim);
    obj->timers[idx] = 0;
    Sprite_SetDrawFlag(obj->sprites[idx], TRUE);
    ov91_0225D884(obj, idx);
    Sprite_SetDrawPriority(obj->sprites[idx], 0);
    Sprite_SetPaletteOverride(obj->sprites[idx], pal);
}
