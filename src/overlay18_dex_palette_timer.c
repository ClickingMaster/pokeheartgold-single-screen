#include "../include/overlay18_dex_window_private.h"

void PokedexEntryPopup_FlashBanner(Ov18Work *work) {
    work->paletteTimer++;
    if (work->paletteTimer == 16) {
        BgTilemapRectChangePalette(work->bgConfig, 2, 0, 0, 32, 2, 7);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, 2);
    } else if (work->paletteTimer == 32) {
        BgTilemapRectChangePalette(work->bgConfig, 2, 0, 0, 32, 2, 0);
        ScheduleBgTilemapBufferTransfer(work->bgConfig, 2);
        work->paletteTimer = 0;
    }
}
