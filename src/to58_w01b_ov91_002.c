#include "to58_overlay91_small_private.h"
void ov91_0225CA74(u8 *base, int arg, int index) {
    int value = PlazaGameRoster_GetIndexOfNetId(base + 8, index);
    ov91_0225CD74(*(void **)(base + 0x30), arg, value);
}
