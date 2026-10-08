#pragma force_active on
#include "global.h"
typedef struct ManagedSprite ManagedSprite;
void ManagedSprite_SetAnimNoRestart(ManagedSprite *s, int anim);
u32 ov96_021E5F24(void *);
void ov96_02219A5C(void *work, u8 sel);
typedef struct {
    void *course;
    u8 pad[0x84];
    ManagedSprite *sprites[4];
    u8 pad2[0xc2 - 0x98];
    u16 selected;
} Work;
void ov96_0221966C(Work *work, u16 type, u16 selected, const u8 *sorted);
// @fn ov96_0221966C
void ov96_0221966C(Work *work, u16 type, u16 selected, const u8 *sorted) {
    int anim;
    int i;
    GF_ASSERT(work != NULL);
    GF_ASSERT(type < 4);
    anim = 0;
    for (i = 0; i < 4; i++, sorted += 2) {
        ManagedSprite_SetAnimNoRestart(work->sprites[sorted[1]], anim + 6);
        if (i != 3 && sorted[0] != sorted[2]) {
            anim = i + 1;
        }
    }
    if (type == ov96_021E5F24(work->course) && selected != work->selected) {
        ov96_02219A5C(work, selected);
        work->selected = selected;
    }
}
