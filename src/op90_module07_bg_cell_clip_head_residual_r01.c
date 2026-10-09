#pragma force_active on
#include "bg_cell_internal.h"

extern void (*const _020F6298[])(BgConfig *,u8);
// @fn sub_020196E8
void sub_020196E8(UnkStruct_0201956C *w, int index, int x, int y) {
    BgCell *c = &w->cells[index];
    u8 cw;
    u8 dx, sx;
    s16 width, right, height, bottom;
    s32 endx, endy, left;
    u8 ch, sy, dy;
    int top;

    c->x = x;
    c->y = y;
    right = c->top;
    if (x >= right) {
        return;
    }
    bottom = c->bottom;
    if (y >= bottom) {
        return;
    }
    width = c->width;
    endx = x + width;
    left = c->left;
    if (endx < left) {
        return;
    }
    height = c->height;
    endy = y + height;
    top = c->right;
    if (endy < top) {
        return;
    }
    dx = x;
    cw = width;
    sx = 0;
    if (x < left) {
        dx = left;
        cw -= left - x;
        sx = left - x;
    }
    if (endx >= right) {
        cw -= endx - right;
    }
    dy = y;
    ch = height;
    sy = 0;
    if (y < top) {
        dy = top;
        ch -= top - y;
        sy = top - y;
    }
    if (endy >= bottom) {
        ch -= endy - bottom;
    }
    CopyToBgTilemapRect(w->bgConfig, c->bgId, dx, dy, cw, ch, c->data, sx, sy, (u8)width, (u8)height);
    _020F6298[w->value](w->bgConfig, c->bgId);
    c->active = 0;
}
