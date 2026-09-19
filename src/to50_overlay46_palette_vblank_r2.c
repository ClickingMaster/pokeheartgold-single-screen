#include "to50_overlay46_palette_vblank_r2_private.h"
void ov46_02259210(void *task, Ov46PaletteWork *work) {
    if (work->enabled == NULL) {
        return;
    }
    work->tick ^= 1;
    if (work->tick & 1) {
        return;
    }
    if (work->mode <= 1) {
        GX_LoadBGPltt(work->palette + work->paletteIndex * 32, 0, 32);
    }
    if (work->mode == 0 || work->mode == 2) {
        GXS_LoadBGPltt(work->palette + work->paletteIndex * 32, 0, 32);
    }
    if (work->direction == 0) {
        work->paletteIndex++;
        if (work->paletteIndex >= 21) {
            work->paletteIndex = 19;
            work->direction ^= 1;
        }
    } else {
        work->paletteIndex--;
        if (work->paletteIndex < 0) {
            work->paletteIndex = 1;
            work->direction ^= 1;
        }
    }
}
