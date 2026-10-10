#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nnsys/g3d/glbstate.h>
#pragma force_active on
NNSG3dGlb NNS_G3dGlb;

void NNS_G3dGlbFlushP(void) {
    NNS_G3dGeBufferOP_N(NNS_G3dGlb.cmd0, (u32 *)&NNS_G3dGlb + 1, 62);
    NNS_G3dGlb.flag &= ~NNS_G3D_GLB_FLAG_FLUSH_WVP;
    NNS_G3dGlb.flag &= ~NNS_G3D_GLB_FLAG_FLUSH_VP;
}
