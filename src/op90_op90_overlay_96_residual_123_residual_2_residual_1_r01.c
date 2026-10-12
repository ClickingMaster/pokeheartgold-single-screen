#pragma force_active on
#include "global.h"
#include "math_util.h"
#include <string.h>
typedef struct { s16 x; s16 y; } Pt16;
typedef struct {
    void *owner;
    s8 delay;
    s8 count;
    s8 period;
    s8 other;
    u32 id:2;
    u32 reserved:2;
    u32 busy:1;
    u32 copy:1;
    u32 rest:26;
} Work;
extern VecFx32 *PokeathlonRingDrop_GetMonPosition(void *, u8);
extern u8 PokeathlonRingDrop_GetMonDirection(void *, u8);
extern u8 PokeathlonRingDrop_GetMonStamina(void *, u8);
extern int PokeathlonRingDrop_GetMonState(void *, u8);
extern int PokeathlonRingDrop_IsMonSwitching(void *, u8);
extern void PokeathlonRingDrop_SetMonTarget(void *, u8, Pt16 *);
extern void PokeathlonRingDrop_RequestJump(void *, u8);
extern void PokeathlonRingDrop_SetMonDestination(void *, u8, Pt16 *);
extern BOOL ov96_0221A034(VecFx32 *, VecFx32 *, fx32);
extern BOOL ov96_0221A05C(VecFx32 *, VecFx32 *, u8);
extern const u8 ov96_0221D97C[];
extern const u8 ov96_0221D978[];
void ov96_0221A08C(Work *w);
// @fn ov96_0221A08C
void ov96_0221A08C(Work *w) {
    int dir;
    VecFx32 *pos;
    u8 kinds[4];
    u8 best[4];
    Pt16 target;
    Pt16 dest;
    VecFx32 *myPos;
    u8 nearCount;
    u8 closeCount;
    u8 chance;
    u8 threshold;
    int i;
    int r;
    int r2;
    s8 first;
    s8 second;
    s16 min;
    s16 dist;
    int state;

    nearCount = 0;
    closeCount = 0;
    memset(kinds, 0, sizeof(kinds));
    myPos = PokeathlonRingDrop_GetMonPosition(w->owner, w->id);
    dir = PokeathlonRingDrop_GetMonDirection(w->owner, w->id);
    for (i = 0; i < 4; i++) {
        if (i == w->id) {
            continue;
        }
        pos = PokeathlonRingDrop_GetMonPosition(w->owner, i);
        if (ov96_0221A034(myPos, pos, 2 * FX32_ONE * 16)) {
            if (ov96_0221A05C(myPos, pos, dir)) {
                kinds[i] = 3;
            } else {
                kinds[i] = 1;
            }
            closeCount++;
        } else if (ov96_0221A034(myPos, pos, 3 * FX32_ONE * 16)) {
            if (ov96_0221A05C(myPos, pos, dir)) {
                kinds[i] = 2;
            }
            nearCount++;
        }
    }
    threshold = chance = 0;
    memset(best, 0, sizeof(best));
    for (i = 0; i < 4; i++) {
        if (i != w->id && best[1] < kinds[i]) {
            best[0] = i;
            best[1] = kinds[i];
        }
    }
    r = MTRandom() % 100;
    threshold += ov96_0221D97C[best[1]];
    if (r < threshold) {
        pos = PokeathlonRingDrop_GetMonPosition(w->owner, best[0]);
        target.x = pos->x >> 12;
        target.y = pos->y >> 12;
        PokeathlonRingDrop_SetMonTarget(w->owner, w->id, &target);
        w->delay = 16;
        return;
    }
    r = MTRandom() % 100;
    if (nearCount != 0) {
        chance += 5;
    }
    if (closeCount != 0) {
        chance += ov96_0221D978[closeCount - 1];
    }
    if (r < chance) {
        PokeathlonRingDrop_RequestJump(w->owner, w->id);
        w->delay = 20;
        return;
    }
    r = MTRandom() % 100;
    if (r < 60) {
        r2 = MTRandom() % 100;
        if (w->copy) {
            return;
        }
        first = -1;
        second = -1;
        min = 999;
        for (i = 0; i < 4; i++) {
            dist = PokeathlonRingDrop_GetMonStamina(w->owner, i);
            state = PokeathlonRingDrop_GetMonState(w->owner, i);
            if (i != w->id && state != 3 && state != 4 && !PokeathlonRingDrop_IsMonSwitching(w->owner, w->other) && min > dist) {
                second = first;
                first = i;
                min = dist;
            }
        }
        if (r2 < 60) {
            w->other = first;
        } else if (r2 < 100) {
            w->other = second;
        } else {
            GF_ASSERT(FALSE);
        }
        if (w->other != -1) {
            w->copy = 1;
            w->delay = 0;
        }
    } else if (r < 80) {
        w->delay = 10;
    } else if (r < 100) {
        dest.x = MTRandom() % 145 + 56;
        dest.y = MTRandom() % 80 + 72;
        PokeathlonRingDrop_SetMonDestination(w->owner, w->id, &dest);
        w->delay = 10;
    }
}
