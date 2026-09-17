#include "to50_overlay15_bag_bg_palette_rows_private.h"
void ov15_021FE528(void *work) {
    u16 i;
    for (i = 0; i < 12; i++) {
        FillBgTilemapRect(*(void **)work, 3, i + 0xCD, i, 13, 1, 1, 4);
        FillBgTilemapRect(*(void **)work, 3, i + 0xF1, i, 14, 1, 1, 4);
    }
}
