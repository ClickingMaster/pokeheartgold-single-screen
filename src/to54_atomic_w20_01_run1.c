#include "to54_atomic_w20_to54_overlay112_walker_helpers_private.h"

int ov112_021EE754(Ov112State *p) {
    if (!TextPrinterCheckActive((u8)U32_AT(p, 0x1E524))) {
        U32_AT(p, 4) = 11;
        return 7;
    }
    return 6;
}

int ov112_021EE778(Ov112State *p) {
    FillBgTilemapRect(PTR_AT(p, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(PTR_AT(p, 0x18), 1);
    return 8;
}
