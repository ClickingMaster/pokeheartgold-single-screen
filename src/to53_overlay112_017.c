#include "to53_overlay112_w01_private.h"


void *ov112_021E7668(void) {
    return ov112_021E59A8();
}

u8 ov112_021E7670(void) {
    return 255 * *(u16 *)(ov112_021FFB24 + 0x36) / *(u16 *)(ov112_021FFB24 + 0x34);
}

u8 ov112_021E768C(void) {
    return 255 * *(u16 *)(ov112_021FFB24 + 0x3a) / *(u16 *)(ov112_021FFB24 + 0x38);
}

u32 ov112_021E76A8(void) {
    u32 old = *(u32 *)(ov112_021FFAA4 + 0x14);
    if (ov112_021E59A8() != NULL) {
        *(u32 *)(ov112_021FFAA4 + 0x14) = 0;
    } else {
        *(u32 *)(ov112_021FFAA4 + 0x14) = 1;
    }
    return old;
}
