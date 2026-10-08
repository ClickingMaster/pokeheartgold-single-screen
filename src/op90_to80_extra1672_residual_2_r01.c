#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 pad[0x18]; s32 sx, sy; s32 rot; s32 tx, ty; u16 w, h; } TexSRT;
void texmtxCalc_flagR_(s32 *m, const TexSRT *t);
// @fn texmtxCalc_flagR_
void texmtxCalc_flagR_(s32 *m, const TexSRT *t)
{
    m[0] = t->sx;
    m[5] = t->sy;
    m[1] = 0;
    m[12] = -(s32)(((s64)t->sx * t->tx) >> 8) * t->w;
    m[13] = (s32)(((s64)t->sy * t->ty) >> 8) * t->h + ((-2 * t->sy + 0x2000) * t->h << 3);
    m[4] = 0;
}
