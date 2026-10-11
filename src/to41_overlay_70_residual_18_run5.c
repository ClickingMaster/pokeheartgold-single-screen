#include "to41_overlay_70_residual_18_private.h"

void ov70_0223B8B4(void *bg) {
    Gts_FreeSubScreenBgs(bg);
    FreeBgTilemapBuffer(bg, 2);
    FreeBgTilemapBuffer(bg, 1);
    FreeBgTilemapBuffer(bg, 0);
    FreeBgTilemapBuffer(bg, 3);
}
