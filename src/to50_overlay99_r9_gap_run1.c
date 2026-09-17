#include "to50_overlay99_r9_gap_private.h"
void ov99_021E80D0(void *work) {
    const ManagedSpriteTemplate *template = ov99_021EA180;
    int i = 0;
    u8 *cursor = work;
    do {
        PTR(cursor, 0x408) = ov98_0221E6E0(PTR(work, 0x404), template);
        ManagedSprite_SetAnimateFlag(PTR(cursor, 0x408), TRUE);
        template++;
        cursor += 4;
        i++;
    } while (i < 4);
}
