#include "overlay_120_residual_6_internal.h"

void ov120_0225FD2C(void *bgConfig, void *narc, int member, int bgId)
{
    ScrnData120 *screen;
    void *allocation = GfGfxLoader_GetScrnDataFromOpenNarc(narc, member, 0, &screen, 4);

    LoadRectToBgTilemapRect(bgConfig, bgId, screen->raw, 0, 0,
                           screen->width >> 3,
                           screen->height >> 3);
    BgTilemapRectChangePalette(bgConfig, bgId, 0, 0,
                               screen->width >> 3,
                               screen->height >> 3, 0);
    Heap_Free(allocation);
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}
