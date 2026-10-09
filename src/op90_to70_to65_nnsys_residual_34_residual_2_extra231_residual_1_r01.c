#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    int shW;
    int shH;
} CharShift;

extern u8 _02109414[4][4][2];
u32 GetCharIndex1D(u32 x, u32 y, int w, int h, CharShift s);

static inline u32 CountLeadingZeros(u32 x) {
    asm { clz x, x }
    return x;
}

#define ShiftOf(n) (((n) >= 8) ? 3 : (int)(31 - CountLeadingZeros((u32)(n))))

static inline const u8 *GetShiftEntry(int w, int h) {
    int sw = ShiftOf(w);
    int sh = ShiftOf(h);
    return _02109414[sh][sw];
}

// @fn GetCharIndex1D
u32 GetCharIndex1D(u32 x, u32 y, int w, int h, CharShift s) {
    int idx = 0;
    int ones;
    int shH;
    u32 ww;
    int shW;
    shW = s.shW;
    shH = s.shH;
    for (;;) {
        ones = -1;
        ww = w & (ones << shW);
        int hh = h & (-1 << shH);
        if (hh <= y) {
            idx += w * hh;
            if (ww <= x) {
                h -= hh;
                idx += ww * h;
                x -= ww;
                y -= hh;
                w -= ww;
            } else {
                y -= hh;
                w = ww;
                h -= hh;
            }
        } else {
            h = ~(-1 << shH);
            if (ww <= x) {
                h = hh;
                idx += ww * hh;
                x -= ww;
                w -= ww;
            } else {
                int lowW = ~(-1 << shW);
                idx += ww * ((-1 << shH) & y);
                idx += ((-1 << shW) & x) << shH;
                idx += (y & h) << shW;
                return idx + (x & lowW);
            }
        }
        {
            const u8 *p = GetShiftEntry(w, h);
            shW = p[0];
            shH = p[1];
        }
        s.shW = shW;
        s.shH = shH;
    }
}
