#include "overlay74_thumb_w06_private.h"

void ov74_02229BC0(void (*func)(), u16 value) {
    PTR(ov74_0223D0A4, 0x20) = func;
    U16(ov74_0223D0A4, 0x24) = value;
}
