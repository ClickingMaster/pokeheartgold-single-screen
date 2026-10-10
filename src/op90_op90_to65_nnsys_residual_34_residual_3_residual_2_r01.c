#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 a : 8;
    u32 b : 8;
} ObjParam;

typedef struct {
    u8 *buf;
    int w;
    int h;
    u8 bpp;
    u8 pad[3];
    ObjParam param;
} Canvas;

void ClearChar(void *chr, int x, int y, int w, int h, u32 col, int bpp);
int GetCharIndex1D(int cx, int cy, int w, int h, int a, int b);
void ClearArea1D(Canvas *, u32, int, int, int, int);
#pragma force_active on

static inline int CharSize(int bpp) {
    return bpp * 8 * 8 / 8;
}

void ClearArea1D(Canvas *c, u32 col, int x, int y, int w, int h) {
    int cy;
    int cx;
    int x1;
    int hh;
    int cxi;
    int y1;
    int cy1;
    u8 *buf;
    int cxi0;
    int cx0;
    ObjParam p;
    int cx1;
    int pb;
    int pa;
    int ch;
    int cw;
    int cs;
    int idx;
    int cyi;
    int top;
    int bottom;
    int left;
    u8 bpp;
    int right;

    bpp = c->bpp;
    x1 = x + w;
    y1 = y + h;
    if (bpp == 4) {
        col |= col << 4;
        col |= col << 8;
        col |= col << 16;
    } else {
        col |= col << 8;
        col |= col << 16;
    }
    cx0 = x & ~7;
    cy = y & ~7;
    cy1 = (y1 + 7) & ~7;
    cs = CharSize(c->bpp);
    cxi0 = cx0 / 8;
    cyi = cy / 8;
    p = c->param;
    cx1 = (x1 + 7) & ~7;
    cw = c->w;
    ch = c->h;
    buf = c->buf;
    pa = p.a;
    pb = p.b;
    for (; cy < cy1; cy += 8) {
        top = (cy < y) ? y - cy : 0;
        bottom = y1 - cy;
        if (bottom > 8) {
            bottom = 8;
        }
        hh = bottom - top;
        for (cx = cx0, cxi = cxi0; cx < cx1; cx += 8) {
            idx = GetCharIndex1D(cxi, cyi, cw, ch, pa, pb);
            left = (cx < x) ? x - cx : 0;
            right = x1 - cx;
            if (right > 8) {
                right = 8;
            }
            ClearChar(buf + cs * idx, left, top, right - left, hh, col, bpp);
            cxi++;
        }
        cyi++;
    }
}
