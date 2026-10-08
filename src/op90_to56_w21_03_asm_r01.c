#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct { u8 pad[0x1e4]; BgConfig *bg; } Work;
void ov08_0221D77C(Work *w, u16 tile, int pos);
// @fn ov08_0221D77C
void ov08_0221D77C(Work *w, u16 tile, int pos) {
    FillBgTilemapRect(w->bg, 7, tile, pos * 2 + 2, 14, 1, 1, 16);
    FillBgTilemapRect(w->bg, 7, tile + 1, pos * 2 + 3, 14, 1, 1, 16);
    FillBgTilemapRect(w->bg, 7, tile + 32, pos * 2 + 2, 15, 1, 1, 16);
    FillBgTilemapRect(w->bg, 7, tile + 33, pos * 2 + 3, 15, 1, 1, 16);
}
