#include "to51_overlay96_r53_gap1_private.h"
u32 ov96_021F0B58(void *course, u8 *state) {
    int i;
    u8 *work;
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return 0;
    }
    if (ov96_021E5F24(course)) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        int total = 0;
        int j = total;
        u8 *participant = work;
        for (; j < 3; j++) {
            total += U16_AT(participant, 0x62);
            PokeathlonCourse_SetField5E0_AtIndex(course, (u8)i, total);
            participant += 0x90;
        }
        work += 0x1B0;
    }
    return 1;
}
