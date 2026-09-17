#include "to50_trainer_card_signature_draw_private.h"

void ov52_021E8F2C(Window *window, const void *src, s32 srcX, s32 srcY, s32 srcWidth, s32 srcHeight, s32 destX, s32 destY, s32 destWidth, s32 destHeight) {
    if (destX < 0) {
        s32 clip = -destX;
        if (clip > destWidth) {
            clip = destWidth;
        }
        destX = 0;
        srcX += clip;
        srcWidth -= clip;
        destWidth -= clip;
    }
    if (destY < 0) {
        s32 clip = -destY;
        if (clip > destHeight) {
            clip = destHeight;
        }
        destY = 0;
        srcY += clip;
        srcHeight -= clip;
        destHeight -= clip;
    }
    BlitBitmapRectToWindow(window, src, srcX, srcY, srcWidth, srcHeight, destX, destY, destWidth, destHeight);
}

void ov52_021E8FAC(Window *window, const void *src, s32 x1, s32 y1, s32 *px, s32 *py, s32 arg6, s32 arg7) {
    s32 ex;
    s32 dx;
    s32 ey;
    s32 sy;
    s32 sx;
    s32 x;
    s32 y;
    s32 err;
    s32 dy;

    x = *px;
    y = *py;
    ex = x1;
    ey = y1;
    if (arg6 == 0 && arg7 == 0) {
        *px = x1;
        *py = y1;
        return;
    }
    dx = x1 - x;
    if (dx < 0) {
        dx = -dx;
    }
    dy = y1 - y;
    if (dy < 0) {
        dy = -dy;
    }
    if (dx > dy) {
        if (x > x1) {
            if (y > y1) {
                sy = 1;
            } else {
                sy = -1;
            }
            ex = x;
            x = x1;
            y = y1;
        } else {
            if (y < y1) {
                sy = 1;
            } else {
                sy = -1;
            }
        }
        ov52_021E8F2C(window, src, 0, 0, 4, 4, x, y, 4, 4);
        err = dx >> 1;
        for (x++; x <= ex; x++) {
            err -= dy;
            if (err < 0) {
                err += dx;
                y += sy;
            }
            ov52_021E8F2C(window, src, 0, 0, 4, 4, x, y, 4, 4);
        }
    } else {
        if (y > y1) {
            if (x > x1) {
                sx = 1;
            } else {
                sx = -1;
            }
            ey = y;
            y = y1;
            x = x1;
        } else {
            if (x < x1) {
                sx = 1;
            } else {
                sx = -1;
            }
        }
        ov52_021E8F2C(window, src, 0, 0, 4, 4, x, y, 4, 4);
        err = dy >> 1;
        for (y++; y <= ey; y++) {
            err -= dx;
            if (err < 0) {
                err += dy;
                x += sx;
            }
            ov52_021E8F2C(window, src, 0, 0, 4, 4, x, y, 4, 4);
        }
    }
    *px = x1;
    *py = y1;
}

void ov52_021E9124(const SigInput *entries, SigState *points) {
    int i;

    for (i = 0; i < 5; i++, entries++, points++) {
        points->count = entries->count;
        if (entries->count != 0) {
            points->x = entries->xs[entries->count - 1];
            points->y = entries->ys[entries->count - 1];
        }
    }
}

void ov52_021E9158(Window *window, SigInput *entries, SigState *points, u32 arg3) {
    s32 lastX;
    s32 lastY;
    int i;
    u32 drew;
    int j;
    SigInput *e;
    SigState *p;

    drew = 0;
    for (i = 0, e = entries, p = points; i < 5; i++, e++, p++) {
        if (e->count != 0) {
            if (p->count != 0) {
                lastX = p->x - 0x20;
                lastY = p->y - 0x48;
            }
            for (j = 0; j < e->count; j++) {
                drew = 1;
                ov52_021E8FAC(window, &ov52_021E95A0[e->style], e->xs[j] - 0x20, e->ys[j] - 0x48, &lastX, &lastY, j, p->count);
            }
        }
    }
    if (drew != 0 && arg3 != 0) {
        CopyWindowToVram(window);
    }
    ov52_021E9124(entries, points);
    for (j = 0; j < 5; j++) {
        entries->count = 0;
        entries++;
    }
}
