#include "ov40_r33_a04_tail_internal.h"

void ov40_0223B44C(void *p) {
    int i = 0;
    u8 *entry = PTR(p, 0x860);
    u8 *w = entry;
    u32 spriteOffset = 0x118;
    entry += 0x114;
    for (; i < 4; i++, entry += 0x1c, w += 0x1c) {
        ov40_0222D6D0(entry);
        Sprite_DeleteAndFreeResources(PTR(w, spriteOffset));
    }
    ov40_0222D7DC(p);
}
