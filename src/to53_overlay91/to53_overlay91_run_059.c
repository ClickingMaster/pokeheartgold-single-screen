#include "private_w20.h"

void ov91_02260364(u8 *p, u16 value) {
    *(u16 *)(p + 0x22e) = value;
}

void ov91_02260370(u8 *p, u16 value) {
    *(u16 *)(p + 0x22c) = value;
}
