#include "overlay87_resource_ui_r13_internal.h"

void ov87_021E7A44(u8 *work, int x, int y) {
    int dx;
    int dy;
    for (dx = -3; dx < 3; dx++) {
        for (dy = -3; dy < 3; dy++) {
            if (x + dy > 0 && x + dy < 256 && y + dx > 0 && y + dx < 192) {
                ov87_021E7AB0(work, x + dy, y + dx);
            }
        }
    }
    BG_LoadCharTilesData(PTR(work, 0x58), 0, PTR(work, 0x38C), PTR(PTR(work, 0x388), 0x10), 0);
    BgCommitTilemapBufferToVram(PTR(work, 0x58), 0);
}
