#include "global.h"

#include "bg_window.h"

void ClearFrameAndWindow2(Window *, BOOL);

void ClearFrameAndWindow2(Window *window, BOOL dontCopy) {
    u8 bgId = GetWindowBgId(window);
    u8 x = GetWindowX(window);
    u8 y = GetWindowY(window);
    u8 width = GetWindowWidth(window);
    u8 height = GetWindowHeight(window);

    FillBgTilemapRect(window->bgConfig, bgId, 0, x - 2, y - 1, width + 5, height + 2, 0);
    if (dontCopy == FALSE) {
        ClearWindowTilemapAndCopyToVram(window);
    }
}
