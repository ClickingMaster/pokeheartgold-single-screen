#include "field_follow_shadow_slop_r1.h"

void ov01_021FD980(void *a0, FollowShadowWork *work) {
    VecFx32 pos;

    if (work->unkC == 1) {
        return;
    }
    {
        u32 index = (u32)sub_02068D90(a0);
        FollowShadow *shadow = work->unk18;
        NNSG3dRenderObj *renderObj = &shadow->unk78[index];
        sub_02068DB8(a0, &pos);
        pos.x -= 0x800;
        pos.y -= 0x4000;
        pos.z += 0x1000;
        RenderObj_DrawAt((int)renderObj, &pos);
    }
}
