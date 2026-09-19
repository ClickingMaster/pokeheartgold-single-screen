#include "to53_overlay112_w01_private.h"


void ov112_021E7AC8(void *bg) {
    GfGfx_EngineATogglePlanes(0x1f, 0);
    GfGfx_EngineBTogglePlanes(0x1b, 0);
    FreeBgTilemapBuffer(bg, 6);
    FreeBgTilemapBuffer(bg, 5);
    FreeBgTilemapBuffer(bg, 4);
    FreeBgTilemapBuffer(bg, 3);
    FreeBgTilemapBuffer(bg, 2);
    FreeBgTilemapBuffer(bg, 1);
    Heap_FreeExplicit((enum HeapID)0x9a, bg);
}
