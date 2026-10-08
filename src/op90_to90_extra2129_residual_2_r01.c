#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 pad[0x18]; s32 sx, sy; s16 sn, cs; s32 tx, ty; u16 w, h; } TexSRT;
void texmtxCalc_flag___3dsmax(s32 *m, const TexSRT *t);
void texmtxCalc_flagS___3dsmax(s32 *m, const TexSRT *t);
void texmtxCalc_flagR___3dsmax(s32 *m, const TexSRT *t);
void FX_DivAsync(s32 a, s32 b);
s32 FX_GetDivResult(void);
// @fn texmtxCalc_flag___3dsmax
void texmtxCalc_flag___3dsmax(s32 *m, const TexSRT *t)
{
    s32 w = t->w << 12, h = t->h << 12;
    s32 a, b, c, d, x, y;
    FX_DivAsync(h, w);
    a = ((s64)t->sx * t->cs) >> 12;
    b = ((s64)t->sx * t->sn) >> 12;
    c = ((s64)t->sy * t->cs) >> 12;
    d = ((s64)t->sy * t->sn) >> 12;
    m[0] = a;
    m[5] = c;
    m[1] = (d * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    x = (-t->w << 11) - t->tx * t->w;
    y = t->ty * t->h + (-t->h << 11);
    m[12] = (s32)(((s64)a * x - (s64)b * y) >> 8) + (t->w << 15);
    m[13] = (s32)(((s64)d * x + (s64)c * y) >> 8) + (t->h << 15);
    m[4] = (-b * FX_GetDivResult()) >> 12;
}
// @fn texmtxCalc_flagS___3dsmax
void texmtxCalc_flagS___3dsmax(s32 *m, const TexSRT *t)
{
    s32 w = t->w << 12, h = t->h << 12;
    s32 x, y;
    FX_DivAsync(h, w);
    m[0] = t->cs;
    m[5] = t->cs;
    m[1] = (t->sn * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    x = (-t->w << 11) - t->tx * t->w;
    y = t->ty * t->h + (-t->h << 11);
    m[12] = (s32)(((s64)t->cs * x - (s64)t->sn * y) >> 8) + (t->w << 15);
    m[13] = (s32)(((s64)t->sn * x + (s64)t->cs * y) >> 8) + (t->h << 15);
    m[4] = (-t->sn * FX_GetDivResult()) >> 12;
}
// @fn texmtxCalc_flagR___3dsmax
void texmtxCalc_flagR___3dsmax(s32 *m, const TexSRT *t)
{
    s32 x, y;
    m[0] = t->sx;
    m[5] = t->sy;
    m[1] = 0;
    x = (-t->w << 11) - t->tx * t->w;
    y = t->ty * t->h + (-t->h << 11);
    m[12] = (s32)(((s64)t->sx * x) >> 8) + (t->w << 15);
    m[13] = (s32)(((s64)t->sy * y) >> 8) + (t->h << 15);
    m[4] = 0;
}
