#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void FX_DivAsync(int, int);
extern int FX_GetDivResult(void);
#pragma force_active on

typedef struct {
    u8 pad[0x20];
    s16 sine, cosine;
    s32 tx, ty;
    u16 width, height;
} Texture;

void texmtxCalc_flagS___xsi(s32 *, const Texture *);

void texmtxCalc_flagS___xsi(s32 *m, const Texture *p) {
    int width = p->width << 12, height = p->height << 12;
    FX_DivAsync(height, width);
    m[0] = p->cosine;
    m[5] = p->cosine;
    m[1] = (p->sine * FX_GetDivResult()) >> 12;
    FX_DivAsync(width, height);
    {
        int y = p->ty, sine = p->sine, cosine = p->cosine, x = p->tx;
        int a = (int)(((s64)x * cosine + (s64)y * sine) >> 12);
        int b = (int)(((s64)x * sine - (s64)y * cosine) >> 12);
        m[12] = (p->width * (sine - a)) << 4;
        m[13] = (-p->height * (p->cosine + b - 0x1000)) << 4;
    }
    m[4] = (-p->sine * FX_GetDivResult()) >> 12;
}
