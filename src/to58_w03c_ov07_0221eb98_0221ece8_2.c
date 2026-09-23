#include "global.h"
int ov07_0221ECE8(u8 *);
int ov07_0221ECE8(u8 *state) {
    *(u16 *)(state + 0x44) = (*(u16 *)(state + 0x44) & ~1) | 1;
    return 0;
}
