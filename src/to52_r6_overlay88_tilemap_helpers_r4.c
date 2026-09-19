#include "to52_r6_overlay88_tilemap_helpers_r4_private.h"

void ov88_02258D90(Ov88ScreenOwner *owner, void **bg, u32 srcX, u32 destX, u8 row) {
    Ov88Screen *screen = owner->screen;
    CopyToBgTilemapRect(*bg, 3, (u8)(destX + 5), (u8)(row * 2 + 4), 1, 2, screen->data, (u8)(srcX + 6), 0, (u8)(screen->width >> 3), (u8)(screen->height >> 3));
    ScheduleBgTilemapBufferTransfer(*bg, 3);
}

void ov88_02258DE8(Ov88ScreenOwner *owner, void **bg, u32 col, u32 row, u32 tile, u32 style, u32 palette3, u32 page, u32 palette4) {
    Ov88Screen *screen;
    u32 srcX = tile % 3;
    u32 srcY = tile / 3;
    u32 y;
    u32 x;
    if (page != 0) {
        srcY += 2;
    }
    if (style == 1) {
        srcY++;
    }
    screen = owner->screen;
    y = row * 2 + 4;
    x = col * 2 + 0x15;
    CopyToBgTilemapRect(*bg, 3, (u8)x, (u8)y, 2, 2, screen->data, (u8)(srcX * 2), (u8)(srcY * 2), (u8)(screen->width >> 3), (u8)(screen->height >> 3));
    if (palette4 != 0) {
        BgTilemapRectChangePalette(*bg, 3, (u8)x, (u8)y, 2, 2, 4);
    } else if (palette3 != 0) {
        BgTilemapRectChangePalette(*bg, 3, (u8)x, (u8)y, 2, 2, 3);
    }
    ScheduleBgTilemapBufferTransfer(*bg, 3);
}

void ov88_02258EA8(u8 *state, void *a1, void *a2, void *a3, void *a4, void *clock) {
    u32 value = ov45_0222AD4C(clock);
    state[0] = 0;
    state[1] = value / 60 + 1;
    state[2] = 0;
    *(u32 *)(state + 8) = value / 40;
    state[4] = 1;
    *(u16 *)(state + 6) = 0;
    state[5] = 0;
    ov88_02258EFC(state, a3, a4);
    ov88_02258F60(state, a2, a1);
}
