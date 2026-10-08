#pragma force_active on
#include "global.h"
typedef struct CellBounds {u16 count;u16 flags;u32 offset;s16 maxX,maxY,minX,minY;} CellBounds;
typedef struct Transform2 {fx32 m00,m01,m10,m11,x,y;} Transform2;
typedef struct Rect {fx32 x,y,width,height;} Rect;
BOOL sub_02025C98(const CellBounds *cell,const Transform2 *m,const Rect *rect);
// @fn sub_02025C98
BOOL sub_02025C98(const CellBounds *cell,const Transform2 *m,const Rect *rect){
    fx32 dx, dy, y0, y1, x0, x1, tmp, top, bottom, left, right;
    u8 r;
    const s16 *b = &cell->maxX;
    r = (u8)(cell->flags & 0x3f) * 4;
    dx = m->x - rect->x;
    dy = m->y - rect->y;
    if (((cell->flags >> 11) & 1) == 1) {
        y0 = b[3] * FX32_ONE;
        y1 = b[1] * FX32_ONE;
        x0 = b[2] * FX32_ONE;
        x1 = b[0] * FX32_ONE;
    } else {
        y0 = -r * FX32_ONE;
        y1 = r * FX32_ONE;
        x0 = y0;
        x1 = y1;
    }
    top = dy + (FX_Mul(y0, m->m01) + FX_Mul(y0, m->m11));
    bottom = dy + (FX_Mul(y1, m->m01) + FX_Mul(y1, m->m11));
    left = dx + (FX_Mul(x0, m->m00) + FX_Mul(x0, m->m10));
    right = dx + (FX_Mul(x1, m->m00) + FX_Mul(x1, m->m10));
    if (bottom < top) { tmp = bottom; bottom = top; top = tmp; }
    if (right < left) { tmp = right; right = left; left = tmp; }
    if (bottom > 0 && top < rect->height && right > 0 && left < rect->width) {
        return TRUE;
    }
    return FALSE;
}
