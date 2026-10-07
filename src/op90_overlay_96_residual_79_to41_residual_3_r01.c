#pragma force_active on
#include "global.h"
typedef struct {
    u8 raw[0x64];
} Shape;
void ov96_021FF0BC(void *obj, fx32 x, u32 **listA, u32 **listB, u8 *countA, u8 *countB);
void ov96_021FF1E0(void *ctx, u32 *item, Shape *shape);
BOOL ov96_021FF574(VecFx32 *pos, Shape *shape);
int ov96_021FF2A0(VecFx32 *pos, int a4, void *ctx, Shape *shape, VecFx32 *out);
u16 ov96_021FF5A8(void *obj, int a1, void *ctx, VecFx32 *pos, int a4, VecFx32 *out);
// @fn ov96_021FF5A8
u16 ov96_021FF5A8(void *obj, int a1, void *ctx, VecFx32 *pos, int a4, VecFx32 *out) {
    u32 *listA[30];
    u32 *listB[30];
    Shape shapeB;
    Shape shapeA;
    VecFx32 v;
    u8 counts[2];
    int i;
    u8 found;
    u8 result;
    ov96_021FF0BC(obj, pos->x, listA, listB, &counts[1], &counts[0]);
    found = FALSE;
    for (i = 0; i < counts[0]; i++) {
        ov96_021FF1E0(ctx, listB[i], &shapeB);
        if (ov96_021FF574(pos, &shapeB)) {
            found = TRUE;
            break;
        }
    }
    result = 0;
    for (i = 0; i < counts[1]; i++) {
        int r;
        ov96_021FF1E0(ctx, listA[i], &shapeA);
        r = ov96_021FF2A0(pos, a4, ctx, &shapeA, &v);
        if (r != 0) {
            if (r >= 10) {
                out->x = v.x;
                out->y = v.y;
                out->z = 0;
            } else if (r != a1) {
                out->x = v.x;
                out->y = v.y;
                out->z = 0;
            }
            result = r;
            break;
        }
    }
    return (found << 8) + result;
}
