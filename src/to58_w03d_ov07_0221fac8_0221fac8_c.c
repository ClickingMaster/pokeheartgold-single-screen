#include "global.h"
int ov07_0221FAC8(u8 *, int);
int ov07_0221FAC8(u8 *state, int index) {
    u8 *data = *(u8 **)(state + 0xC0);
    return ((*(u32 *)(data + 0xFC + index * 4) & 0x200400C0) != 0);
}
