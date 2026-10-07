#pragma force_active on
#include "global.h"

typedef struct Work {
    u8 unk0[0x130];
    void *objs[27];
    int state;
    u16 idx;
} Work;

VecFx32 *ov96_021EB594(void *obj);
void ov96_021EB588(void *obj, VecFx32 *pos);
void ov96_022082BC(Work *work, u8 idx, int arg);
void ov96_022088AC(Work *work, u8 arg);
BOOL ov96_02208448(Work *work, int arg);

// @fn ov96_02208448
BOOL ov96_02208448(Work *work, int arg) {
    int *state = &work->state;

    switch (*state) {
    case 0: {
        VecFx32 pos = *ov96_021EB594(work->objs[work->idx]);
        pos.y += 0x20000;
        ov96_021EB588(work->objs[work->idx], &pos);
        if (pos.y >= 0x39C000) {
            pos.y = 0x294000;
            ov96_021EB588(work->objs[work->idx], &pos);
            (*state)++;
        }
        break;
    }
    case 1:
        ov96_022082BC(work, work->idx, arg);
        work->idx = (work->idx + 1) % 2;
        (*state)++;
        break;
    case 2: {
        VecFx32 pos = *ov96_021EB594(work->objs[work->idx]);
        pos.y += 0x20000;
        ov96_021EB588(work->objs[work->idx], &pos);
        if (pos.y >= 0x344000) {
            pos.y = 0x344000;
            ov96_021EB588(work->objs[work->idx], &pos);
            work->idx = (work->idx + 1) % 2;
            (*state)++;
        }
        break;
    }
    case 3: {
        VecFx32 pos = *ov96_021EB594(work->objs[work->idx]);
        pos.y += 0x20000;
        ov96_021EB588(work->objs[work->idx], &pos);
        if (pos.y >= 0x2EC000) {
            pos.y = 0x2EC000;
            ov96_021EB588(work->objs[work->idx], &pos);
            work->idx = (work->idx + 1) % 2;
            ov96_022088AC(work, (arg + 2) % 3);
            *state = 0;
            return TRUE;
        }
        break;
    }
    }
    return FALSE;
}
