#include "private_w09.h"

void ov13_022217A0(int count, u8 *dst, const void *src) {
    int i;
    for (i = 0; i < count; i++) {
        ov13_02222968(dst, src, 6);
        *(u16 *)(dst + 6) = ov13_02222AB0();
        *(u16 *)(dst + 6) = ov13_02222A44(*(u16 *)(dst + 6));
        dst += 8;
    }
}
int ov13_022217D0(int value) {
    if (value == -1) {
        ov13_022459B4[1] = value;
        return 0;
    }
    if (ov13_022459B4[1] != value) {
        ov13_022459B4[1] = value;
        return ov13_02223100(value);
    }
    return 0;
}
