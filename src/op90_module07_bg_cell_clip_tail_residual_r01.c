#pragma force_active on
#include "bg_cell_internal.h"

extern void (*const _020F6298[])(BgConfig *,u8);
// @fn sub_0201980C
void sub_0201980C(UnkStruct_0201956C *w, int index) {
    BgCell *c = &w->cells[index];
    s8 cw, ch;
    s8 ymax;
    int width;
    s8 dx;
    u16 height;
    int sy;
    s8 ymin;
    s8 x;
    int endx;
    s8 xmax;
    s8 xmin;
    int endy;
    s8 y;

    c->active = 1;
    xmax = c->top;
    x = c->x;
    if (x >= xmax) {
        return;
    }
    ymax = c->bottom;
    y = c->y;
    if (y >= ymax) {
        return;
    }
    width = c->width;
    endx = x + width;
    xmin = c->left;
    if (endx < xmin) {
        return;
    }
    height = c->height;
    sy = y;
    endy = y + height;
    ymin = c->right;
    if (endy < ymin) {
        return;
    }
    dx = x;
    cw = width;
    if (x < xmin) {
        dx = xmin;
        cw -= xmin - x;
    }
    if (endx >= xmax) {
        cw -= endx - xmax;
    }
    ch = height;
    if (y < ymin) {
        y = ymin;
        ch -= ymin - sy;
    }
    if (endy >= ymax) {
        ch -= endy - ymax;
    }
    FillBgTilemapRect(w->bgConfig, c->bgId, 0, dx, y, cw, ch, 16);
    _020F6298[w->value](w->bgConfig, c->bgId);
}
