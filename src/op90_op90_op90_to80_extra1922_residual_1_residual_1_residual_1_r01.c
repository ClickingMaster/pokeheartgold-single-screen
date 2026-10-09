#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u32 attr01;
    u16 attr2;
    u16 attr3;
} Oam;

extern u8 _02109414[4][4][2];
u32 OBJSizeToShape(const u8 *p);
int NNS_G2dArrangeOBJ1D(Oam *oam, int w, int h, int x, int y, int color, int charName, int vramMode);

static inline u32 CountLeadingZeros(u32 x) {
    asm { clz x, x }
    return x;
}

static inline int ShiftOf(int n) {
    return (n >= 8) ? 3 : (int)(31 - CountLeadingZeros((u32)n));
}

// @fn NNS_G2dArrangeOBJ1D
int NNS_G2dArrangeOBJ1D(Oam *oam, int w, int h, int x, int y, int color, int charName, int vramMode) {
    int sw = ShiftOf(w);
    int sh = ShiftOf(h);
    const u8 *p = _02109414[sh][sw];
    int objShW = p[0];
    int objShH = p[1];
    u32 areaW = w & (-1 << objShW);
    u32 areaH = h & (-1 << objShH);
    int unit = (color == 0) ? 1 : 2;
    int count = 0;
    u32 shape = OBJSizeToShape(p);
    int step;
    int nx = w >> objShW;
    int ny = h >> objShH;
    step = (unit << objShW) << objShH;
    int i, j;

    for (j = 0; j < ny; j++) {
        int oy = y + ((j << objShH) << 3);
        for (i = 0; i < nx; i++) {
            int ox = x + ((i << objShW) << 3);
            oam->attr01 = (oam->attr01 & 0xFE00FF00) | (oy & 0xff) | (((u32)ox << 23) >> 7);
            oam->attr01 = (oam->attr01 & 0x3FFF3FFF) | shape;
            oam->attr2 = (u16)((oam->attr2 & ~0x3ff) | charName);
            charName += step >> vramMode;
            oam->attr01 = (oam->attr01 & ~0x2000) | (color << 13);
            oam++;
        }
    }
    count += nx * ny;

    if (areaW < w) {
        int dw = w - areaW;
        int ox = x + areaW * 8;
        int n = NNS_G2dArrangeOBJ1D(oam, dw, areaH, ox, y, color, charName, vramMode);
        oam += n;
        charName += (unit * dw * areaH) >> vramMode;
        count += n;
    }
    if (areaH < h) {
        int dh = h - areaH;
        int n = NNS_G2dArrangeOBJ1D(oam, areaW, dh, x, y + areaH * 8, color, charName, vramMode);
        oam += n;
        charName += (unit * areaW * dh) >> vramMode;
        count += n;
    }
    if (areaW < w && areaH < h) {
        count += NNS_G2dArrangeOBJ1D(oam, w - areaW, h - areaH, x + areaW * 8, y + areaH * 8, color, charName, vramMode);
    }
    return count;
}
