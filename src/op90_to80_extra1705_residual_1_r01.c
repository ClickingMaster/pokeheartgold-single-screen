#pragma force_active on
#include "global.h"
#include "bg_window.h"

typedef struct {Window *window;u8 frames[8][128];u8 frozen[128];u16 tile;u8 tick;u8 frame:7,flag:1;u8 state:2,statePadding:6;} S;
void sub_0200F1D4(S*,int);
// @fn sub_0200F1D4
void sub_0200F1D4(S *s, int mode) {
    u8 bg = GetWindowBgId(s->window);
    u8 x = GetWindowX(s->window);
    u8 y = GetWindowY(s->window);
    u8 width = GetWindowWidth(s->window);
    if (mode == 2) {
        BG_LoadCharTilesData(s->window->bgConfig, bg, s->frozen, 128, s->tile + 18);
        FillBgTilemapRect(s->window->bgConfig, bg, s->tile + 10, x + width + 1, y + 2, 1, 1, 16);
        FillBgTilemapRect(s->window->bgConfig, bg, s->tile + 11, x + width + 2, y + 2, 1, 1, 16);
        FillBgTilemapRect(s->window->bgConfig, bg, s->tile + 10, x + width + 1, y + 3, 1, 1, 16);
        FillBgTilemapRect(s->window->bgConfig, bg, s->tile + 11, x + width + 2, y + 3, 1, 1, 16);
        BgCommitTilemapBufferToVram(s->window->bgConfig, bg);
        return;
    }
    BG_LoadCharTilesData(s->window->bgConfig, bg, s->frames[s->frame], 128, s->tile + 18);
    if (mode != 0) {
        FillBgTilemapRect(s->window->bgConfig, bg, s->tile + 18, x + width + 1, y + 2, 1, 1, 16);
        FillBgTilemapRect(s->window->bgConfig, bg, s->tile + 19, x + width + 2, y + 2, 1, 1, 16);
        FillBgTilemapRect(s->window->bgConfig, bg, s->tile + 20, x + width + 1, y + 3, 1, 1, 16);
        FillBgTilemapRect(s->window->bgConfig, bg, s->tile + 21, x + width + 2, y + 3, 1, 1, 16);
        BgCommitTilemapBufferToVram(s->window->bgConfig, bg);
    }
}
