#pragma force_active on
#include "global.h"
typedef struct { void *sprite; int slot; int box; } CopySlot;
typedef struct { void *mon; void *item; void *extra; } SlotSprites;
typedef struct {
    u8 pad0[0x1a8];
    SlotSprites slots[30];
    u8 pad1[0x3c8 - 0x1a8 - 30 * 12];
    CopySlot copies[6];
    int count;
    u8 pad2[0xe884 - 0x414];
    int box;
} Work;
BOOL Sprite_GetDrawFlag(void *);
const VecFx32 *Sprite_GetMatrixPtr(void *);
void Sprite_SetMatrix(void *, const VecFx32 *);
void Sprite_SetDrawFlag(void *, BOOL);
int ov74_02232398(Work *, int);
int ov74_022323D0(Work *, int);
int ov74_02232424(Work *, int);
int ov74_02232474(Work *, int);
int ov74_022324A0(Work *w, int slot);
// @fn ov74_022324A0
int ov74_022324A0(Work *w, int slot) {
    int found;
    int i;
    VecFx32 pos;
    if (w->slots[slot].mon != NULL && !Sprite_GetDrawFlag(w->slots[slot].mon)) {
        return 3;
    }
    found = -1;
    for (i = 0; found == -1 && i < 6; i++) {
        if (w->box == w->copies[i].box && slot == w->copies[i].slot) {
            found = i;
        }
    }
    if (ov74_02232398(w, slot) == 1) {
        return 4;
    }
    if (ov74_022323D0(w, slot) == 1) {
        return 5;
    }
    if (ov74_02232424(w, slot) == 1) {
        return 6;
    }
    if (ov74_02232474(w, slot) == 1) {
        return 7;
    }
    if (found == -1) {
        if (w->count == 6) {
            return 0;
        }
        for (i = 0; i < 6; i++) {
            if (w->copies[i].slot == -1) {
                const VecFx32 *mtx = Sprite_GetMatrixPtr(w->slots[slot].mon);
                Sprite_SetMatrix(w->copies[i].sprite, mtx);
                pos = *Sprite_GetMatrixPtr(w->copies[i].sprite);
                pos.x -= FX32_CONST(8);
                pos.y -= FX32_CONST(4);
                Sprite_SetMatrix(w->copies[i].sprite, &pos);
                Sprite_SetDrawFlag(w->copies[i].sprite, TRUE);
                w->copies[i].slot = slot;
                w->copies[i].box = w->box;
                w->count++;
                return 1;
            }
        }
    } else {
        Sprite_SetDrawFlag(w->copies[found].sprite, FALSE);
        w->copies[found].slot = -1;
        w->count--;
        return 2;
    }
    return 0;
}
