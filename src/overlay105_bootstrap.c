#include "overlay105_bootstrap_internal.h"

BOOL ov105_021E5900(OverlayManager *man) {
    u8 i;
    Ov105Args *args;
    Ov105App *app;

    Heap_Create((enum HeapID)3, (enum HeapID)0x97, 0x31000);
    app = OverlayManager_CreateAndGetData(man, sizeof(Ov105App), (enum HeapID)0x97);
    memset(app, 0, sizeof(Ov105App));
    args = OverlayManager_GetArgs(man);
    app->unk9C = args->unk0;
    app->unk9D = args->unk1;
    app->unk9E = 0;
    app->camera = Camera_New((enum HeapID)0x97);
    ov105_021E5B68();
    ov105_021E5CA4(app);
    ov105_021E5BCC(app);
    for (i = 0; i < 4; i++) {
        NNS_G3dGlbLightVector((GXLightId)i, args->config->lightVecs[i].x, args->config->lightVecs[i].y, args->config->lightVecs[i].z);
        NNS_G3dGlbLightColor((GXLightId)i, args->config->lightColors[i]);
    }
    NNS_G3dGlbMaterialColorDiffAmb(args->config->diffuse, args->config->ambient, args->config->unk28);
    NNS_G3dGlbMaterialColorSpecEmi(args->config->specular, args->config->emission, args->config->unk2C);
    NNS_G3dGlbPolygonAttr(args->config->polyAttr0, (GXPolygonMode)args->config->polyAttr1, (GXCull)args->config->polyAttr2, args->config->polyAttr3, args->config->polyAttr4, args->config->polyAttr5);
    BeginNormalPaletteFade((enum FadeMode)3, (enum FadeType)1, (enum FadeType)1, 0, 6, 1, (enum HeapID)0x97);
    return TRUE;
}
