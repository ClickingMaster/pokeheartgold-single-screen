#include "slop_r1_geometry_intersection_m12.h"

static void sub_02020EF4(fx32 *out, int x1, int y1, int x2, int y2) {
    struct {
        volatile fx32 slope;
        fx32 intercept;
    } line;
    int dy = (s16)(y2 - y1);
    int dx = (s16)(x2 - x1);

    if (dx != 0) {
        line.slope = FX_Div(dy << FX32_SHIFT, dx << FX32_SHIFT);
    } else {
        line.slope = 0xFF << FX32_SHIFT;
    }
    line.intercept = (y1 << FX32_SHIFT) - (((s64)line.slope * (x1 << FX32_SHIFT) + 0x800LL) >> FX32_SHIFT);
    out[0] = line.slope;
    out[1] = line.intercept;
}

BOOL sub_02020F4C(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d, VecFx32 *out) {
    fx32 intPartX;
    fx32 intPartY;
    fx32 line1[2];
    fx32 line2[2];
    fx32 ix;
    fx32 iy;
    fx32 maxX1, minX1, maxY1, minY1, maxX2, minX2, maxY2, minY2;

    if (out != NULL) {
        out->x = 0xFFFF;
        out->y = 0xFFFF;
    }
    if (sub_02020EB0(a, b, c, d) == 0) {
        return FALSE;
    }
    sub_02020EF4(line1, a->x, a->y, b->x, b->y);
    sub_02020EF4(line2, c->x, c->y, d->x, d->y);
    if (line1[0] == line2[0]) {
        return FALSE;
    }
    ix = FX_Div(line2[1] - line1[1], line1[0] - line2[0]);
    iy = line2[1] + FX_Mul(line2[0], ix);
    if (FX_Modf(ix, &intPartX) >= 0x800) {
        intPartX += FX32_ONE;
    }
    out->x = intPartX >> FX32_SHIFT;
    if (FX_Modf(iy, &intPartY) >= 0x800) {
        intPartY += FX32_ONE;
    }
    out->y = intPartY >> FX32_SHIFT;
    if (a->x >= b->x) {
        maxX1 = a->x;
        minX1 = b->x;
    } else {
        maxX1 = b->x;
        minX1 = a->x;
    }
    if (a->y >= b->y) {
        maxY1 = a->y;
        minY1 = b->y;
    } else {
        maxY1 = b->y;
        minY1 = a->y;
    }
    if (c->x >= d->x) {
        maxX2 = c->x;
        minX2 = d->x;
    } else {
        maxX2 = d->x;
        minX2 = c->x;
    }
    if (c->y >= d->y) {
        maxY2 = c->y;
        minY2 = d->y;
    } else {
        maxY2 = d->y;
        minY2 = c->y;
    }
    if (maxX1 >= out->x && minX1 <= out->x && maxY1 >= out->y && minY1 <= out->y && maxX2 >= out->x && minX2 <= out->x && maxY2 >= out->y && minY2 <= out->y) {
        return TRUE;
    }
    return FALSE;
}
