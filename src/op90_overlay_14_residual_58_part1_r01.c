#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "bg_cell_manager.h"
u16 *BgCellManager_GetCellTilemap(BgCellManager *, int);
void PCBox_SetPanelWindowFrame(BgCellManager *mgr, int id, Window *win);
// @fn PCBox_SetPanelWindowFrame
void PCBox_SetPanelWindowFrame(BgCellManager *mgr, int id, Window *win) {
    u16 size[2];
    u16 *map;
    u16 i;
    u16 j;
    u16 tile;
    u16 pal;
    u16 x;

    map = BgCellManager_GetCellTilemap(mgr, id);
    BgCellManager_GetCellSize(mgr, id, &size[1], &size[0]);
    tile = GetWindowBaseTile(win);
    pal = win->paletteNum << 12;
    map[0] = pal + 1000;
    map[size[1] - 1] = pal + 1002;
    map[size[1] * (size[0] - 1)] = pal + 1009;
    map[size[1] * size[0] - 1] = pal + 1011;
    for (i = 0; i < size[1] - 2; i++) {
        map[i + 1] = pal + 1001;
        map[i + size[1] * (size[0] - 1) + 1] = pal + 1010;
    }
    for (i = 0; i < size[0] - 2; i++) {
        map[size[1] * (i + 1)] = pal + 1003;
        map[size[1] * (i + 2) - 1] = pal + 1005;
    }
    for (j = 0; j < size[0] - 2; j++) {
        for (x = 0; x < size[1] - 2; x++) {
            map[x + (size[1] * (j + 1) + 1)] = pal + tile;
            tile++;
        }
    }
}
