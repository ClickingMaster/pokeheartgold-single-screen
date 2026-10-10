#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

static inline u32 valA(void) {
    return 0x40000200;
}

static inline u32 valB(void) {
    return 0xc0080000;
}

static inline const u32 *params(NNSG3dGlb *p) {
    return ((const u32 *)p) + 1;
}

static inline void sendGlobal(void) {
    NNS_G3dGeBufferOP_N(*(volatile u32 *)&NNS_G3dGlb.cmd0, params(&NNS_G3dGlb), 0x3e);
}

static inline void clearFlag(u32 mask) {
    NNS_G3dGlb.flag &= ~mask;
}

void NNS_G3dGlbInit(void);
void NNS_G3dGlbFlushP(void);

void NNS_G3dGlbInit(void) {
    u32 va, vb;
#define G(off) ((u32 *)&NNS_G3dGlb)[(off) / 4]
    NNS_G3dGlb.cmd0 = 0x17101610;
    NNS_G3dGlb.mtxmode_proj = 0;
    NNS_G3dGlb.mtxmode_posvec = 2;
    NNS_G3dGlb.cmd1 = 0x32323232;
    NNS_G3dGlb.cmd2 = 0x60293130;
    NNS_G3dGlb.cmd3 = 0x33333333;
    NNS_G3dGlb.cmd4 = 0x002a1b19;
    MTX_Identity43_(&NNS_G3dGlb.cameraMtx);
    MTX_Identity44_(&NNS_G3dGlb.projMtx);
    va = valA();
    vb = valB();
    NNS_G3dGlb.lightVec[0] = 0x2d8b62d8;
    NNS_G3dGlb.lightVec[1] = va;
    NNS_G3dGlb.lightVec[2] = va - 0xc0000001;
    NNS_G3dGlb.lightVec[3] = vb;
    NNS_G3dGlb.prmMatColor0 = 0x4210c210;
    NNS_G3dGlb.prmMatColor1 = 0x4210c210;
    NNS_G3dGlb.prmPolygonAttr = 0x001f008f;
    NNS_G3dGlb.prmViewPort = vb - 0x90000;
    NNS_G3dGlb.lightColor[0] = 0x7fff;
    NNS_G3dGlb.lightColor[1] = 0x4000001f;
    NNS_G3dGlb.lightColor[2] = 0x800003e0;
    NNS_G3dGlb.lightColor[3] = 0x7e00 - va;
    NNS_G3dGlb.prmBaseTrans.x = 0;
    NNS_G3dGlb.prmBaseTrans.y = 0;
    NNS_G3dGlb.prmBaseTrans.z = 0;
    MTX_Identity33_(&NNS_G3dGlb.prmBaseRot);
    NNS_G3dGlb.prmBaseScale.x = 0x1000;
    NNS_G3dGlb.prmBaseScale.y = 0x1000;
    NNS_G3dGlb.prmBaseScale.z = 0x1000;
    NNS_G3dGlb.prmTexImageParam = 0;
    NNS_G3dGlb.flag = 0;
    NNS_G3dGlb.camPos.z = 0;
    NNS_G3dGlb.camPos.y = 0;
    NNS_G3dGlb.camPos.x = 0;
    NNS_G3dGlb.camUp.z = 0;
    NNS_G3dGlb.camUp.x = 0;
    NNS_G3dGlb.camUp.y = 0x1000;
    NNS_G3dGlb.camTarget.y = 0;
    NNS_G3dGlb.camTarget.x = 0;
    NNS_G3dGlb.camTarget.z = -0x1000;
}
