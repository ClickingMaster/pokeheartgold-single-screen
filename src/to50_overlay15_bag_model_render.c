#include "to50_overlay15_bag_model_render_private.h"
void BagApp_Draw3D(void *work) {
    MtxFx33 rotation = sBagModelRotation;
    VecFx32 scale = sBagModelScale;
    u8 *base;

    BagApp_UpdateCameraTween((u8 *)work + 0x808, U8(work, 0x615));
    Camera_Init_FromTargetDistanceAndAngle((u8 *)work + 0x904, PTR(work, 0x910), (u8 *)work + 0x914, U16(work, 0x91E), U8(work, 0x91C), TRUE, U32(work, 0x818));
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    base = (u8 *)work + 0x81C;
    BagApp_AdvanceAnimLoop(PTR(base, 0xA0 + U32(base, 0xE4) * 4));
    BagApp_AdvanceAnimLoop(PTR(base, 0xC0 + U32(base, 0xE4) * 4));
    BagApp_AdvanceAnimLoop(PTR(base, 0xE0));
    GF3dRender_DrawModel(base, (VecFx32 *)((u8 *)work + 0x934), &rotation, &scale);
    RequestSwap3DBuffers(0, 0);
}
