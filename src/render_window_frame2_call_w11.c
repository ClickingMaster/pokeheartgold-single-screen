#include "global.h"

#include "bg_window.h"

void sub_0200E6B4(void *, u32, u32, u32, u32, u32, u32, u32);
void sub_0200E948(Window *, u32, u32);

void sub_0200E948(Window *window, u32 baseTile, u32 palette) {
    u8 bgId = GetWindowBgId(window);
    u8 x = GetWindowX(window);
    u8 y = GetWindowY(window);
    u8 width = GetWindowWidth(window);
    u8 height = GetWindowHeight(window);

    sub_0200E6B4(window->bgConfig, bgId, x, y, width, height, (u8)palette, (u16)baseTile);
}
