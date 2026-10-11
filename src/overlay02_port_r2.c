#include "overlay02_port_private.h"

WIP_LOCAL Sprite *ov02_02248CAC(void *mgr) {
    VecFx32 pos = { 0, 0, 0 };
    VecFx32 affineMatrix = { 0, 0, 0 };
    VecFx32 affineScale;
    Sprite *sprite;

    affineScale = ov02_02253360;
    sprite = ov02_02248C10(mgr, &pos, 0, 0, 0, 2, 0, 0x84);
    Sprite_SetAffineOverwriteMode(sprite, 2);
    Sprite_SetAffineMatrix(sprite, &affineMatrix);
    Sprite_SetAffineScale(sprite, &affineScale);
    Sprite_SetAffineZRotation(sprite, GF_DegreeToSinCosIdx(0));
    return sprite;
}

WIP_LOCAL Sprite *ov02_02248D18(void *mgr, int a1) {
    VecFx32 pos = { 0, 0, 0 };
    Sprite *sprite;
    int plttId;

    plttId = 0;
    if (a1 == 1) {
        plttId = 1;
    }
    sprite = ov02_02248C10(mgr, &pos, 2, plttId, 2, 1, 0, 0x83);
    Sprite_SetDrawFlag(sprite, FALSE);
    Sprite_SetAnimCtrlSeq(sprite, 6);
    return sprite;
}

WIP_LOCAL void *ov02_02248D58(void *arg0, void *arg1, void *arg2, void *arg3) {
    VecFx32 pos = { 0, 0, 0 };
    struct {
        void *unk0;
        void *unk4;
        void *unk8;
        void *unkC;
    } a4;
    a4.unk0 = arg1;
    a4.unk4 = arg0;
    a4.unkC = arg2;
    a4.unk8 = arg3;
    return FieldEffectPool_AddBasicEffect(arg1, &ov02_02253454, &pos, 0, &a4, 0x82);
}
