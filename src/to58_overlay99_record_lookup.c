#include "global.h"

void GF_AssertFail(void);
void *ov99_021E9430(void *work, u32 index, int group);

void *ov99_021E9430(void *work, u32 index, int group) {
    u8 *base;

    if (index >= 10) {
        GF_AssertFail();
    }
    if (group != 0) {
        base = ((u8 ***)work)[0][1];
        return base + index * 0xA4;
    }
    return (u8 *)work + 4 + index * 0x2C;
}
