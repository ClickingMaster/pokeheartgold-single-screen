#include "field_follow_shadow_slop_r1.h"

void ov01_021FD838(void *a0, FollowShadowWork *work) {
    VecFx32 pos;
    VecFx32 scale;
    MtxFx33 mtx;
    VecFx32 offset;
    LocalMapObject *object;
    FollowShadow *shadow;
    NNSG3dRenderObj *renderObj;

    if (work->unkC != 0) {
        return;
    }
    mtx = ov01_02208EF0;
    shadow = work->unk18;
    object = work->unk1C;
    renderObj = &shadow->unk78[0];
    ov01_021FD488(shadow, &scale);
    if (ov01_021F8F88(object) != 0) {
        sub_02068DB8(a0, &pos);
        pos.x -= 0x800;
        pos.y -= 0x4000;
        pos.z += 0x1000;
        ov01_021FD9CC(MapObject_GetFacingDirection(object), &pos);
    } else {
        MapObject_CopyPositionVector(object, &pos);
        ov01_021F8FA0(object, &offset);
        pos.x += offset.x;
        pos.z += offset.z;
        pos.x -= 0x800;
        pos.y -= 0x4000;
        pos.z += 0x1000;
    }
    sub_020699AC(renderObj, &pos, &scale, &mtx);
}
