#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void FX_DivAsync(s32, s32);
s32 FX_GetDivResult(void);

typedef struct {
    u32 pad[6];
    s32 sx, sy;
    s16 sine, cosine;
    s32 tx, ty;
    u16 width, height;
} Tex;

static inline s32 UnitOffset(s32 n) {
    return n + 4096;
}

void texmtxCalc_flagS_(s32 *, const Tex *);

void texmtxCalc_flagS_(s32 *dest, const Tex *p) {
    s32 width = p->width << 12, height = p->height << 12;
    FX_DivAsync(height, width);
    dest[0] = p->cosine;
    dest[5] = p->cosine;
    dest[1] = (-p->sine * FX_GetDivResult()) >> 12;
    FX_DivAsync(width, height);
    dest[12] = (p->width * (UnitOffset(-(p->sine + p->cosine))) << 3) - (p->tx * p->width << 4);
    dest[13] = (p->height * (p->sine - p->cosine + 4096) << 3) + (p->ty * p->height << 4);
    dest[4] = (p->sine * FX_GetDivResult()) >> 12;
}
