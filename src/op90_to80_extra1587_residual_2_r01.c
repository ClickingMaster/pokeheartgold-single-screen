#pragma force_active on
#include "global.h"

typedef struct Point {
    s32 x;
    s32 y;
} Point;

typedef struct Table {
    int v[4];
} Table;

typedef struct Bounds {
    int state;
    Point vertices[4];
    Point edges[4][2];
} Bounds;

extern const Table ov96_0221C5FC;
extern const Table ov96_0221C60C;

BOOL sub_02020F4C(Point *p0, Point *p1, Point *a, Point *b, Point *hit);
fx32 ov96_021FF67C(const VecFx32 *a, const VecFx32 *b, const VecFx32 *c, fx32 r);
int ov96_021FF2A0(VecFx32 *start, VecFx32 *end, int radius, Bounds *w, VecFx32 *out);

// @fn ov96_021FF2A0
int ov96_021FF2A0(VecFx32 *start, VecFx32 *end, int radius, Bounds *w, VecFx32 *out) {
    Table cur1;
    Table cur2;
    VecFx32 vertex;
    VecFx32 diff;
    VecFx32 delta;
    VecFx32 result;
    Point hit;
    Point a;
    Point b;
    int i;
    BOOL inside;
    int lo, hi;

    if (w->state == 3) {
        return 0;
    }
    a.x = start->x / FX32_ONE;
    a.y = start->y / FX32_ONE;
    b.x = end->x / FX32_ONE;
    b.y = end->y / FX32_ONE;
    for (i = 0; i < 4; i++) {
        cur1 = ov96_0221C5FC;
        if (sub_02020F4C(&w->edges[i][0], &w->edges[i][1], &a, &b, &hit)) {
            out->x = hit.x << FX32_SHIFT;
            out->y = hit.y << FX32_SHIFT;
            out->z = 0;
            return cur1.v[i];
        }
        inside = FALSE;
        if (i == 0 && i == 2) {
            if (a.y == w->edges[i][0].y) {
                if (w->edges[i][0].x < w->edges[i][1].x) {
                    lo = w->edges[i][0].x;
                    hi = w->edges[i][1].x;
                } else {
                    lo = w->edges[i][1].x;
                    hi = w->edges[i][0].x;
                }
                if (lo <= a.x && a.x <= hi) {
                    inside = TRUE;
                }
            }
        } else {
            if (a.x == w->edges[i][0].x) {
                if (w->edges[i][0].y < w->edges[i][1].y) {
                    lo = w->edges[i][0].y;
                    hi = w->edges[i][1].y;
                } else {
                    lo = w->edges[i][1].y;
                    hi = w->edges[i][0].y;
                }
                if (lo <= a.y && a.y <= hi) {
                    inside = TRUE;
                }
            }
        }
        if (inside) {
            *out = *start;
            return cur1.v[i];
        }
    }
    for (i = 0; i < 4; i++) {
        cur2 = ov96_0221C60C;
        vertex.x = w->vertices[i].x << FX32_SHIFT;
        vertex.y = w->vertices[i].y << FX32_SHIFT;
        vertex.z = 0;
        VEC_Subtract(end, &vertex, &diff);
        if (VEC_Mag(&diff) <= radius * FX32_ONE) {
            fx32 t = ov96_021FF67C(start, end, &vertex, radius << FX32_SHIFT);
            VEC_Subtract(end, start, &delta);
            VEC_MultAdd(t, &delta, start, &result);
            out->x = result.x;
            out->y = result.y;
            out->z = 0;
            return cur2.v[i];
        }
    }
    if (w->vertices[0].x << FX32_SHIFT <= end->x && end->x <= w->vertices[2].x << FX32_SHIFT
        && w->vertices[0].y << FX32_SHIFT <= end->y && end->y <= w->vertices[2].y << FX32_SHIFT) {
        int dx1;
        BOOL xflag;
        int xlo;
        int dx0;
        BOOL yflag;
        int yhi;
        int xdir;
        int ylo;
        int dy0;
        int dy1;
        int ydir;
        int ret;
        *out = *end;
        yflag = FALSE;
        xflag = FALSE;
        xlo = w->vertices[0].x;
        dx0 = (xlo << FX32_SHIFT) - end->x;
        dx1 = (w->vertices[2].x << FX32_SHIFT) - end->x;
        if (dx0 < 0) {
            dx0 *= -1;
        }
        if (dx1 < 0) {
            dx1 *= -1;
        }
        if (dx0 < dx1) {
            xdir = 13;
        } else {
            dx0 = dx1;
            xdir = 11;
        }
        ylo = w->vertices[0].y;
        dy0 = (ylo << FX32_SHIFT) - end->y;
        yhi = w->vertices[2].y;
        dy1 = (yhi << FX32_SHIFT) - end->y;
        if (dy0 < 0) {
            dy0 *= -1;
        }
        if (dy1 < 0) {
            dy1 *= -1;
        }
        if (dy0 < dy1) {
            ydir = 10;
        } else {
            dy0 = dy1;
            ydir = 12;
        }
        if (dx0 <= dy0) {
            xflag = TRUE;
            ret = xdir;
        } else {
            yflag = TRUE;
            ret = ydir;
        }
        if (xflag) {
            if (xdir == 13) {
                out->x = (xlo - radius) << FX32_SHIFT;
            } else {
                out->x = (w->vertices[2].x + radius) << FX32_SHIFT;
            }
        } else if (yflag) {
            if (ydir == 10) {
                out->y = (ylo - radius) << FX32_SHIFT;
            } else {
                out->y = (yhi + radius) << FX32_SHIFT;
            }
        }
        return ret;
    }
    return 0;
}
