#include "to51_ov96_r86_gap3_private.h"

void ov96_02205AFC(u8 *a, u8 *dest);
BOOL ov96_02205C94(void *course);

int ov96_02205520(void *course, u8 *state) {
    u8 *copy;
    u32 *p;
    int i;

    copy = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourse_GetHeapAllocPtr4(course);
    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return 0;
    }
    if (ov96_021E5F24(course)) {
        return 1;
    }
    copy += 0x28;
    p = ov96_021E8A20(copy);
    for (i = 0; i < 4; i++) {
        u16 a = (p[i] >> 18) & 0x3ff;
        u16 b = (((u16 *)p)[i + 12] >> 11) & 31;
        u16 v = a + b;
        if (v > 999) {
            v = 999;
        }
        PokeathlonCourse_SetField5E0_AtIndex(course, (u8)i, v);
    }
    return 1;
}
