#ifndef POKEHEARTGOLD_GEOMETRY_MATH_H
#define POKEHEARTGOLD_GEOMETRY_MATH_H

#include "global.h"

// A point on an integer 2D grid (field/course coordinates in whole units).
typedef struct Point2D {
    s32 x;
    s32 y;
} Point2D;

// NitroSDK fixed-point routines the SDK headers in lib/include do not declare yet.
u16 FX_Atan2Idx(fx32 y, fx32 x);
fx32 FX_Modf(fx32 value, fx32 *integerPart);

int Math_Min(int a, int b);
int Math_Max(int a, int b);
fx32 GetDistanceFromPointToLine(VecFx32 *point, VecFx32 *near, VecFx32 *far);
u16 CalcAngleBetweenVecs(VecFx32 *a, VecFx32 *b);
void Math_CalcRotationMatrixXYZ(MtxFx33 *rotation, VecFx32 *angles);
void Math_CalcRotationMatrixXYZDeg(MtxFx33 *rotation, u16 x, u16 y, u16 z);
void Math_CalcPerspectiveViewSize(s32 angle, fx32 distance, fx32 aspect, fx32 *width, fx32 *height);
BOOL Point2D_IsLeftOfLine(const Point2D *lineStart, const Point2D *lineEnd, const Point2D *point);
BOOL Point2D_SegmentsIntersect(const Point2D *a, const Point2D *b, const Point2D *c, const Point2D *d);
BOOL Point2D_CalcSegmentIntersection(Point2D *a, Point2D *b, Point2D *c, Point2D *d, Point2D *out);

#endif // POKEHEARTGOLD_GEOMETRY_MATH_H
