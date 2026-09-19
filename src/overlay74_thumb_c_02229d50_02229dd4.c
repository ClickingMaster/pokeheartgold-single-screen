#include "overlay74_thumb_w06_private.h"

void ov74_02229D50(u8 value) {
    U8(ov74_0223D0A4, 0x27) = value;
    ov74_02229BC0(ov74_02229C60, 0);
}
int ov74_02229D6C(void *dst) {
    int i;
    for (i = 0; i < 16; i++) {
        void *src = sub_020358D0(i);
        if (src != NULL && U16(src, 0x4C) != 0) {
            memcpy((u8 *)dst + 0x8C, src, 0x50);
            return i;
        }
    }
    return -1;
}
void ov74_02229DA0(u8 value) {
    if (U8(ov74_0223D0A4, 0x27) == value) {
        U8(ov74_0223D0A4, 0x3C0) = 1;
    }
}
u8 ov74_02229DBC(void) {
    return U8(ov74_0223D0A4, 0x3C0);
}
int ov74_02229DCC(void) {
    return 0x358;
}
void *ov74_02229DD4(void) {
    return ov74_0223D0A4 + 0x68;
}
