#include "to58_overlay91_small_private.h"
void ov91_0225D2EC(u8 *base, u8 *record) {
    u8 buffer[0x28];
    if (*(u32 *)(base + 0x14) != *(u16 *)record) {
        ov91_0225E294(record, buffer);
        ov91_0225E2E4(base, buffer, 2);
        if (*(u16 *)(record + 2) != 0) {
            ov91_0225D694(base + 0x1e38, *(u16 *)record, *(s8 *)(record + 0x12), *(void **)base);
        }
    }
}
