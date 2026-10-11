#include "to55_overlay91_merge_private.h"

void ov91_022601AC(u8 *p) {
    ModelAnimation_AdvanceLooped(p + 0x198, 0x1000);
    ov91_02260378(p);
}

void ov91_022601C8(u8 *p, u8 *b) {
    int i;
    for (i = 0; i < 3; i++, p += 0x78) {
        ModelInstance_SetPosition(p, *(u32 *)(b + 0xd8), *(u32 *)(b + 0xdc), *(u32 *)(b + 0xe0));
    }
}
