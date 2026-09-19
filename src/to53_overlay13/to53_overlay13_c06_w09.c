#include "private_w09.h"

int ov13_02221F64(const u8 *a, void *dst) {
    const u8 *item = a + 6;
    int size = ov13_02222A84(*(u16 *)(item + 2));
    if (size <= 0) {
        return -1;
    }
    if (item[0] != 0x70) {
        return -1;
    }
    ov13_02222968(dst, item + 6, size);
    return 0;
}
