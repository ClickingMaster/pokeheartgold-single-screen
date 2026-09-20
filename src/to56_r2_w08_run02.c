#include "global.h"

#include "to56_r2_w08_private.h"

int ov74_0222A0E4(GiftSlot *gift) {
    int species;
    switch (gift->kind) {
    case 13:
        return 1;
    case 3:
        if (U32(gift, 8) == 1) {
            return 1;
        }
        break;
    case 1:
    case 2:
        species = GetMonData(gift->mon, 0x98, NULL);
        if ((species >= 3 && species <= 14) || (species >= 0x41 && species <= 0x47)) {
            return 1;
        }
        break;
    }
    return 0;
}

void ov74_0222A130(void *work) {
    if (U32(work, 0x29ec) == 1) {
        U32(work, 0x29e8) = 0;
    }
    if ((u32)(ov74_022360B0() - 2) <= 1) {
        if (ov74_0222A0E4((GiftSlot *)((u8 *)work + 0xdc)) != 1) {
            PlaySE(1562);
        }
        U32(work, 0x29e8) = 0;
    }
}
