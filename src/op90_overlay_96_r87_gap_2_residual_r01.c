#pragma force_active on
#include "global.h"
#include <nitro/fx/fx_vec.h>

typedef struct Pt {
    int x;
    int y;
} Pt;

typedef struct Edge {
    Pt p0;
    Pt p1;
} Edge;

typedef struct Rect {
    Pt pts[4];
    Edge edges[4];
} Rect;

typedef struct Codes {
    int v[4];
} Codes;

extern const Codes ov96_0221CB00;
extern const Codes ov96_0221CAE0;

BOOL sub_02020F4C(Pt *a0, Pt *a1, Pt *b0, Pt *b1, Pt *hit);
fx32 ov96_02207390(VecFx32 *a, VecFx32 *b, VecFx32 *pt, fx32 radius);
int ov96_0220703C(VecFx32 *a, VecFx32 *b, int radius, Rect *rect, VecFx32 *out);

// @fn ov96_0220703C
int ov96_0220703C(VecFx32 *a, VecFx32 *b, int radius, Rect *rect, VecFx32 *out)
{
    VecFx32 *end = b;
    VecFx32 *result = out;
    int i;
    Pt hit;
    Pt ai;
    Pt bi;
    fx32 t;
    BOOL flag;
    int sideX, sideY;
    int lo;
    BOOL useX, useY;
    int hi;
    Pt *p;
    int x0, x1, y0, y1;
    s32 ret;
    ai.x = a->x / FX32_ONE;
    int dx0, dx1, dy0, dy1;
    ai.y = a->y / FX32_ONE;
    bi.x = end->x / FX32_ONE;
    bi.y = end->y / FX32_ONE;
    for (i = 0; i < 4; ++i)
    {
        Codes c = ov96_0221CB00;
        if (sub_02020F4C(&rect->edges[i].p0, &rect->edges[i].p1, &ai, &bi, &hit))
        {
            result->x = hit.x << 12;
            result->y = hit.y << 12;
            result->z = 0;
            return c.v[i];
        }
        flag = FALSE;
        if (i == 0 && i == 2)
        {
            if (ai.y == rect->edges[i].p0.y)
            {
                if (rect->edges[i].p0.x < rect->edges[i].p1.x)
                {
                    lo = rect->edges[i].p0.x;
                    hi = rect->edges[i].p1.x;
                }
                else
                {
                    lo = rect->edges[i].p1.x;
                    hi = rect->edges[i].p0.x;
                }
                if (lo <= ai.x && ai.x <= hi)
                {
                    flag = TRUE;
                }
            }
        }
        else
        {
            if (ai.x == rect->edges[i].p0.x)
            {
                if (rect->edges[i].p0.y < rect->edges[i].p1.y)
                {
                    lo = rect->edges[i].p0.y;
                    hi = rect->edges[i].p1.y;
                }
                else
                {
                    lo = rect->edges[i].p1.y;
                    hi = rect->edges[i].p0.y;
                }
                if (lo <= ai.y && ai.y <= hi)
                {
                    flag = TRUE;
                }
            }
        }
        if (flag)
        {
            *result = *a;
            return c.v[i];
        }
    }
    i = 0, p = rect->pts;
    while (i < 4)
    {
        Codes c = ov96_0221CAE0;
        VecFx32 pt;
        VecFx32 d;
        pt.x = p->x << 12;
        pt.y = p->y << 12;
        pt.z = 0;
        VEC_Subtract(end, &pt, &d);
        if (VEC_Mag(&d) <= radius << 12)
        {
            VecFx32 dv;
            VecFx32 res;
            t = ov96_02207390(a, end, &pt, radius * FX32_ONE);
            VEC_Subtract(end, a, &dv);
            VEC_MultAdd(t, &dv, a, &res);
            result->x = res.x;
            result->y = res.y;
            result->z = 0;
            return c.v[i];
        }
        i++, p++;
    }
    if (rect->pts[0].x << 12 <= end->x && end->x <= rect->pts[2].x << 12 && rect->pts[0].y << 12 <= end->y && end->y <= rect->pts[2].y << 12)
    {
        *result = *end;
        x0 = rect->pts[0].x;
        useX = useY = FALSE;
        dx0 = (x0 << 12) - end->x;
        x1 = rect->pts[2].x;
        dx1 = (x1 << 12) - end->x;
        if (dx0 < 0)
        {
            dx0 *= -1;
        }
        if (dx1 < 0)
        {
            dx1 *= -1;
        }
        if (dx0 < dx1)
        {
            sideX = 12;
        }
        else
        {
            dx0 = dx1;
            sideX = 10;
        }
        y0 = rect->pts[0].y;
        dy0 = (y0 << 12) - end->y;
        y1 = rect->pts[2].y;
        dy1 = (y1 << 12) - end->y;
        if (dy0 < 0)
        {
            dy0 *= -1;
        }
        if (dy1 < 0)
        {
            dy1 *= -1;
        }
        if (dy0 < dy1)
        {
            sideY = 9;
        }
        else
        {
            dy0 = dy1;
            sideY = 11;
        }
        if (dx0 <= dy0)
        {
            useX = TRUE;
            ret = sideX;
        }
        else
        {
            useY = TRUE;
            ret = sideY;
        }
        if (useX)
        {
            if (sideX == 12)
            {
                result->x = (x0 - radius) << 12;
            }
            else
            {
                result->x = (x1 + radius) << 12;
            }
            return ret;
        }
        if (useY)
        {
            if (sideY == 9)
            {
                result->y = (y0 - radius) << 12;
            }
            else
            {
                result->y = (y1 + radius) << 12;
            }
            return ret;
        }
        return ret;
    }
    return 0;
}
