#include "to58_overlay91_small_private.h"
void ov91_0225CA9C(u8 *base, int arg, int index) {
    int value = PlazaGameRoster_GetIndexOfNetId(base + 8, index);
    void *object = *(void **)(base + 0x30);
    if (object != NULL) {
        ov91_0225CD84(object, arg, value);
    }
}
void ov91_0225CABC(u8 *base, u32 *values) {
    ov91_0225D368(*(void **)(base + 0x34));
    *(u32 *)(base + 0x40) = 1;
    for (int i = 0; i < 4; i++) {
        *(u32 *)(base + 0x18) = *values;
        values++;
        base += 4;
    }
}
