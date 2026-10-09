#pragma force_active on
#include "global.h"

extern const VecFx32 ov96_0221C0E8;
extern int ov96_0221E5A0;
static int ov96_0221DC10 = 20;
static int sColorG = 10;

void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 num);
void NNS_G3dGlbFlushP(void);
void ov96_021F61C8(int *work);

static inline void GeVtx16(u32 xy, u32 z) {
    u32 args[2];

    args[0] = xy;
    args[1] = z;
    NNS_G3dGeBufferOP_N(35, args, 2);
}

static inline void GeColor(u32 rgb) {
    NNS_G3dGeBufferOP_N(32, &rgb, 1);
}

// @fn ov96_021F61C8
void ov96_021F61C8(int *work) {
    VecFx32 scale = ov96_0221C0E8;
    u32 polygonAttr;
    u32 color;
    u32 primitive;
    int i;

    NNS_G3dGeBufferOP_N(27, (const u32 *)&scale, 3);
    polygonAttr = 0x310081;
    NNS_G3dGeBufferOP_N(41, &polygonAttr, 1);
    color = 191;
    NNS_G3dGeBufferOP_N(32, &color, 1);
    NNS_G3dGlbFlushP();
    primitive = 3;
    NNS_G3dGeBufferOP_N(64, &primitive, 1);
    ov96_0221E5A0 = 0;
    sColorG = 10;
    ov96_0221DC10 = 20;
    GeVtx16(0xE000, 0);
    GeVtx16(0xE800E000, 0);
    for (i = 1; i <= 128; i++) {
        float x = ((float)i - 64.0) * 4.0 / 128.0;

        GeVtx16((u16)(int)(x * 4096.0f) | ((u32)(u16)(s16)work[i + 103] << 16), 0);
        GeVtx16((u16)(int)(x * 4096.0f) | 0xE8000000, 0);
        GeColor((u16)(ov96_0221E5A0 | (sColorG << 5) | (ov96_0221DC10 << 10)));
        ov96_0221E5A0 = (ov96_0221E5A0 + 1) % 31;
        sColorG = (sColorG + 1) % 31;
        ov96_0221DC10 = (ov96_0221DC10 + 1) % 31;
    }
    GeVtx16(0xE000, 0);
    GeVtx16(0xE800E000, 0);
    NNS_G3dGeBufferOP_N(65, NULL, 0);
}
