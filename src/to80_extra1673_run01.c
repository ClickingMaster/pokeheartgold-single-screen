#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define I(p, o) (*(int *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))
#define V(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct {
    u8 pad[24];
    int sx, sy;
    s16 sn, cs;
    u8 pad2[8];
    u16 w, h;
} Tex;

void texmtxCalc_flagT_(int *, Tex *), texmtxCalc_flagTS_(int *, Tex *), texmtxCalc_flagTR_(int *, Tex *);
extern void FX_DivAsync(int, int);
extern int FX_GetDivResult(void);

void texmtxCalc_flagT_(int *m, Tex *t) {
    int w = t->w << 12, h = t->h << 12;
    int a, b, c, d;
    FX_DivAsync(h, w);
    a = (s64)t->sx * t->sn >> 12;
    b = (s64)t->sx * t->cs >> 12;
    c = (s64)t->sy * t->sn >> 12;
    d = (s64)t->sy * t->cs >> 12;
    m[0] = b;
    m[5] = d;
    m[1] = (-c * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m[12] = (t->w * (t->sx - (a + b))) << 3;
    m[13] = (t->h * (c - d - t->sy + 8192L)) << 3;
    m[4] = (a * FX_GetDivResult()) >> 12;
}

void texmtxCalc_flagTS_(int *m, Tex *t) {
    int w = t->w << 12, h = t->h << 12;
    FX_DivAsync(h, w);
    m[0] = t->cs;
    m[5] = t->cs;
    m[1] = (-t->sn * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m[12] = (t->w * ((-(t->sn + t->cs)) + 4096L)) << 3;
    m[13] = (t->h * (t->sn - t->cs + 4096L)) << 3;
    m[4] = (t->sn * FX_GetDivResult()) >> 12;
}

void texmtxCalc_flagTR_(int *m, Tex *t) {
    m[0] = t->sx;
    m[5] = t->sy;
    m[1] = 0;
    m[12] = 0;
    m[13] = (t->h * ((-(t->sy << 1)) + 8192L)) << 3;
    m[4] = 0;
}
