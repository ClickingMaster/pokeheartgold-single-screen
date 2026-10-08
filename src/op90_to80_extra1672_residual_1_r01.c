#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 pad[0x18]; s32 sx, sy; s16 sn, cs; s32 tx, ty; u16 w, h; } TexSRT;
void texmtxCalc_flag_(s32 *m, const TexSRT *t);
void FX_DivAsync(s32 a, s32 b);
s32 FX_GetDivResult(void);
// @fn texmtxCalc_flag_
void texmtxCalc_flag_(s32 *m, const TexSRT *t)
{
    s32 w = t->w << 12, h = t->h << 12;
    s32 a, b, c, d;
    FX_DivAsync(h, w);
    b = ((s64)t->sx * t->sn) >> 12;
    a = ((s64)t->sx * t->cs) >> 12;
    c = ((s64)t->sy * t->sn) >> 12;
    d = ((s64)t->sy * t->cs) >> 12;
    m[0] = a;
    m[5] = d;
    m[1] = (-c * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m[12] = ((t->sx - (b + a)) * t->w << 3) - (s32)(((s64)t->sx * t->tx) >> 8) * t->w;
    m[13] = ((c - d - t->sy + 0x2000) * t->h << 3) + (s32)(((s64)t->sy * t->ty) >> 8) * t->h;
    m[4] = (b * FX_GetDivResult()) >> 12;
}
