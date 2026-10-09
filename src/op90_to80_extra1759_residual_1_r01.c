#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u32 charWidthShift : 8;
    u32 charHeightShift : 8;
    u32 rest : 16;
} CharLayout;

extern u8 _02109414[4][4][2];
extern u8 _02109408[];
void InitCharCanvas(void *cc, void *base, int w, int h, int bpp, const void *table, CharLayout layout);
void NNS_G2dCharCanvasInitForOBJ1D(void *cc, void *base, int w, int h, int bpp);

static inline u32 CountLeadingZeros(u32 x) {
    asm { clz x, x }
    return x;
}

static inline int ShiftOf(int n) {
    return (n >= 8) ? 3 : (int)(31 - CountLeadingZeros((u32)n));
}

// @fn NNS_G2dCharCanvasInitForOBJ1D
void NNS_G2dCharCanvasInitForOBJ1D(void *cc, void *base, int w, int h, int bpp) {
    CharLayout layout;
    int sw = ShiftOf(w);
    int sh = ShiftOf(h);
    const u8 *p = _02109414[sh][sw];
    layout.charWidthShift = p[0];
    layout.charHeightShift = p[1];
    InitCharCanvas(cc, base, w, h, bpp, _02109408, layout);
}
