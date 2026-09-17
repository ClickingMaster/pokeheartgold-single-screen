#include "global.h"

#include "bg_window.h"

void sub_0200E5D4(Window *, BOOL);

void sub_0200E5D4(Window *window, BOOL dontCopy) {
    u8 bgId = GetWindowBgId(window);
    u8 x = GetWindowX(window);
    u8 y = GetWindowY(window);
    u8 width = GetWindowWidth(window);
    u8 height = GetWindowHeight(window);

    FillBgTilemapRect(window->bgConfig, bgId, 0, x - 1, y - 1, width + 2, height + 2, 0);
    if (dontCopy == FALSE) {
        ClearWindowTilemapAndCopyToVram(window);
    }
}
