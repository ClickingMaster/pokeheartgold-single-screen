#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct {
    void *args;
    BgConfig *bg;
} Work;
void ov99_021E86D4(Work *w, int index, u8 x, int y, int page, int sub);
// @fn ov99_021E86D4
void ov99_021E86D4(Work *w, int index, u8 x, int y, int page, int sub) {
    int row;
    u8 bg = sub ? 6 : 2;
    int col;
    for (row = 0; row < 6; row++) {
        for (col = 0; col < 6; col++) {
            u16 tile = page * 360 + (col + 1 + index * 6 + row * 60);
            LoadRectToBgTilemapRect(w->bg, bg, &tile, x + col, y, 1, 1);
        }
        y++;
    }
    BgCommitTilemapBufferToVram(w->bg, bg);
}
