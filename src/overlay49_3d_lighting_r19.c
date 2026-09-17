#include "overlay49_3d_lighting_r19_private.h"

void ov49_0225A854(void) {
    VecFx16 vec;
    GfGfx_EngineATogglePlanes(1, 1);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(ov49_02269754);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0x6B5A, 0, 0x7FFF, 0x3F, FALSE);
    *(vu32 *)0x04000580 = 0xBFFF0000;
    NNS_G3dGlbLightVector(GX_LIGHTID_0, -2043, -3548, 110);

    vec.x = 0;
    vec.y = 0;
    vec.z = FX16_ONE;
    VEC_Fx16Normalize(&vec, &vec);
    NNS_G3dGlbLightVector(GX_LIGHTID_1, vec.x, vec.y, vec.z);
    vec.x = 0;
    vec.y = 0;
    vec.z = FX16_ONE;
    VEC_Fx16Normalize(&vec, &vec);
    NNS_G3dGlbLightVector(GX_LIGHTID_2, vec.x, vec.y, vec.z);
    NNS_G3dGlbLightColor(GX_LIGHTID_2, 0x7FFF);
    vec.x = 0;
    vec.y = 0;
    vec.z = FX16_ONE;
    VEC_Fx16Normalize(&vec, &vec);
    NNS_G3dGlbLightVector(GX_LIGHTID_3, vec.x, vec.y, vec.z);
    NNS_G3dGlbLightColor(GX_LIGHTID_3, 0x7FFF);
}
