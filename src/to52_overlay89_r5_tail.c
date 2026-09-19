#include "to52_overlay89_r5_private.h"

void ov89_0225A148(u8 *context, void *network) {
    ov89_0225C724(
        *(void **)(context + 0x30),
        *(void **)(context + 0x2C),
        context + 0x34,
        **(void ***)context,
        network);
}
