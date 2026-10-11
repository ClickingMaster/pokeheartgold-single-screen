#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct { u8 p[8]; BgConfig *bg; Window windows[1]; } Work;
u16 *BgCellManager_GetCellTilemap(BgConfig *, int);
void ov18_021F006C(Work *w, u32 index);
// @fn ov18_021F006C
void ov18_021F006C(Work *w, u32 index) {
u16 *map; Window *wins; u16 base; u8 x, y, width, height; u32 i, j; map = BgCellManager_GetCellTilemap(w->bg, 15); wins = w->windows; base = GetWindowBaseTile(&wins[index]); x = GetWindowX(&wins[index]); y = GetWindowY(&wins[index]); width = GetWindowWidth(&wins[index]); height = GetWindowHeight(&wins[index]); for (j = 0; j < height; j++) { for (i = 0; i < width; i++) { map[(y + j) * 32 + x + i] = (map[(y + j) * 32 + x + i] & 0xF000) + base + j * width + i; } } CopyWindowPixelsToVram_TextMode(&wins[index]);
}
