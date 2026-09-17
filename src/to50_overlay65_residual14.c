#include "to50_overlay65_residual14_private.h"

void ov65_0221F864(void *p) {
    Ov65StateR14 *state = p;
    if (state->field36B8 != NULL) {
        state->field36BC++;
        if (state->field36BC > 0x708) {
            sub_02039AD8(4);
        }
    }
}

void ov65_0221F890(void) {
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & 0xFFFF1FFF) | ((u32)0x04000000 >> 13);
    G2_SetBlendBrightness(0x1E, 7);
}
