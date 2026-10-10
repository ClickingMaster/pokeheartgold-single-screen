#include "geometry_math.h"

#include "math_util.h"

int Math_Min(int a, int b) {
    return a < b ? a : b;
}

int Math_Max(int a, int b) {
    return a > b ? a : b;
}

fx32 GetDistanceFromPointToLine(VecFx32 *point, VecFx32 *near, VecFx32 *far) {
    VecFx32 line, delta, distance, projected, zero = { 0, 0, 0 };
    fx32 scale;
    VEC_Subtract(far, near, &line);
    VEC_Subtract(point, near, &delta);
    scale = FX_Div(VEC_DotProduct(&line, &delta),
        FX_Mul(line.x, line.x) + FX_Mul(line.y, line.y) + FX_Mul(line.z, line.z));
    VEC_MultAdd(scale, &line, &zero, &projected);
    VEC_Subtract(&projected, &delta, &distance);
    return VEC_Mag(&distance);
}

u16 CalcAngleBetweenVecs(VecFx32 *a, VecFx32 *b) {
    VecFx32 na, nb;
    fx32 dot;
    fx32 cross;
    VEC_Normalize(a, &na);
    VEC_Normalize(b, &nb);
    dot = FX_Mul(na.z, nb.z) + FX_Mul(na.x, nb.x);
    cross = FX_Mul(na.z, nb.x) - FX_Mul(na.x, nb.z);
    if (dot == 0) {
        if (cross > 0) {
            return 0x4000;
        }
        return 3 << 14;
    }
    return FX_Atan2Idx(cross, dot);
}

void Math_CalcRotationMatrixXYZ(MtxFx33 *rotation, VecFx32 *angles) {
    MtxFx33 temp;
    MTX_RotX33_(rotation, FX_SinIdx((u16)angles->x), FX_CosIdx((u16)angles->x));
    MTX_RotY33_(&temp, FX_SinIdx((u16)angles->y), FX_CosIdx((u16)angles->y));
    MTX_Concat33(rotation, &temp, rotation);
    MTX_RotZ33_(&temp, FX_SinIdx((u16)angles->z), FX_CosIdx((u16)angles->z));
    MTX_Concat33(rotation, &temp, rotation);
}

void Math_CalcRotationMatrixXYZDeg(MtxFx33 *rotation, u16 x, u16 y, u16 z) {
    MtxFx33 temp;
    fx32 sin, cos;
    cos = GF_CosDegNoWrap(x);
    sin = GF_SinDegNoWrap(x);
    MTX_RotX33_(rotation, sin, cos);
    cos = GF_CosDegNoWrap(y);
    sin = GF_SinDegNoWrap(y);
    MTX_RotY33_(&temp, sin, cos);
    MTX_Concat33(rotation, &temp, rotation);
    cos = GF_CosDegNoWrap(z);
    sin = GF_SinDegNoWrap(z);
    MTX_RotZ33_(&temp, sin, cos);
    MTX_Concat33(rotation, &temp, rotation);
}

// Size of the view at `distance` from a perspective camera: height = 2 * distance * tan(angle),
// width = height * aspect. `angle` is a sin/cos table index (half of the vertical field of view).
void Math_CalcPerspectiveViewSize(s32 angle, fx32 distance, fx32 aspect, fx32 *width, fx32 *height) {
    fx32 tangent = FX_Div(FX_SinIdx(angle), FX_CosIdx(angle));
    fx32 value = FX_Mul(distance, tangent);

    value = FX_Mul(value, 2 * FX32_ONE);
    *height = value;
    *width = FX_Mul(*height, aspect);
}

// Sign of the cross product (lineEnd - lineStart) x (point - lineStart): TRUE when `point` lies on
// the left of the directed line (y axis up) or on it.
BOOL Point2D_IsLeftOfLine(const Point2D *lineStart, const Point2D *lineEnd, const Point2D *point) {
    s32 endTerm = lineEnd->x * (point->y - lineStart->y);
    s32 startTerm = lineStart->x * (lineEnd->y - point->y);
    s32 pointTerm = point->x * (lineStart->y - lineEnd->y);

    return pointTerm + startTerm + endTerm >= 0;
}

// TRUE when segment a-b and segment c-d properly cross (each one's endpoints straddle the other).
BOOL Point2D_SegmentsIntersect(const Point2D *a, const Point2D *b, const Point2D *c, const Point2D *d) {
    if ((Point2D_IsLeftOfLine(a, b, c) ^ Point2D_IsLeftOfLine(a, b, d)) == 1) {
        if ((Point2D_IsLeftOfLine(c, d, a) ^ Point2D_IsLeftOfLine(c, d, b)) == 1) {
            return TRUE;
        }
    }
    return FALSE;
}

// Line through (x1, y1) and (x2, y2) as y = slope * x + intercept (fx32); vertical lines get slope 255.
static void Point2D_CalcLineThroughPoints(fx32 *out, int x1, int y1, int x2, int y2) {
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

// Intersection point of segments a-b and c-d, rounded to whole units. Returns FALSE when they do not
// cross or are parallel; `out` is preset to (0xFFFF, 0xFFFF).
BOOL Point2D_CalcSegmentIntersection(Point2D *a, Point2D *b, Point2D *c, Point2D *d, Point2D *out) {
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
    if (Point2D_SegmentsIntersect(a, b, c, d) == 0) {
        return FALSE;
    }
    Point2D_CalcLineThroughPoints(line1, a->x, a->y, b->x, b->y);
    Point2D_CalcLineThroughPoints(line2, c->x, c->y, d->x, d->y);
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
