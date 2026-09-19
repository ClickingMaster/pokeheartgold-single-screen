#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_02230170(void *data) {
    switch (WORD(data, 0x2C)) {
    case 0:
        WORD(data, 0x18) = 0x7FFF;
        WORD(data, 0x24) = 5;
        WORD(data, 0x1C) = 0x10000;
        WORD(data, 0x28) = 0x10;
        break;
    case 1:
        WORD(data, 0x18) = 0x7FFF;
        WORD(data, 0x24) = -5;
        WORD(data, 0x1C) = -0x10000;
        WORD(data, 0x28) = 0x10;
        break;
    case 2:
        WORD(data, 0x18) = 0x7FFF;
        WORD(data, 0x24) = 10;
        WORD(data, 0x1C) = 0x14000;
        WORD(data, 0x28) = 0x10;
        break;
    }
}
