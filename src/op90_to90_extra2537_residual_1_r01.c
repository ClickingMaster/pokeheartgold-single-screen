#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 pad[0x18]; s32 sx, sy; s16 sn, cs; s32 tx, ty; u16 w, h; } TexSRT;
void texmtxCalc_flag___xsi(s32 *m, const TexSRT *t);
void FX_DivAsync(s32 a, s32 b);
s32 FX_GetDivResult(void);
// @fn texmtxCalc_flag___xsi
void texmtxCalc_flag___xsi(s32 *m, const TexSRT *t)
{
    s32 w = t->w << 12, h = t->h << 12;
    s32 sx, sy, sn, cs, b, d, a;
    s64 p, q;
    s32 x, y;
    FX_DivAsync(h, w);
    cs = t->cs;
    sx = t->sx;
    sn = t->sn;
    sy = t->sy;
    a = ((s64)sx * cs) >> 12;
    b = ((s64)sx * sn) >> 12;
    d = ((s64)sy * cs) >> 12;
    m[0] = a;
    m[5] = d;
    m[1] = ((s32)(((s64)sy * sn) >> 12) * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    p = ((s64)t->tx * t->sn - (s64)t->ty * t->cs) >> 12;
    q = ((s64)t->tx * t->cs + (s64)t->ty * t->sn) >> 12;
    y = d + (s32)((p * t->sy) >> 12);
    x = b - (s32)((q * t->sx) >> 12);
    m[12] = (t->w * x) << 4;
    m[13] = (-t->h * (y - 0x1000)) << 4;
    m[4] = (-b * FX_GetDivResult()) >> 12;
}
