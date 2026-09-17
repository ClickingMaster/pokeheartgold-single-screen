#include "to50_overlay93_palette_tilemap_r7_private.h"

void ov93_02260BB0(void *sys) {
    if (U8_AT(PTR_AT(sys, 0), 0x30) == 3) {
        BgTilemapRectChangePalette(PTR_AT(sys, 0x2C), 4, 0, 13, 12, 4, 3);
        BgTilemapRectChangePalette(PTR_AT(sys, 0x2C), 4, 20, 13, 12, 4, 0);
    }
}
