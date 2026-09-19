#include "to53_overlay92_camera_init_private.h"

void ov92_0225DD88(void *work) {
    VecFx32 target = { 0, 0, 0 };
    VecFx32 pos = ov92_02263B5C;
    Camera_Init_FromTargetAndPos(&target, &pos, 0xB60, 0, FALSE, PTR(work, 0x64));
    Camera_SetPerspectiveClippingPlane((fx32)0xFFC18000, 0x3E8000, PTR(work, 0x64));
    Camera_SetStaticPtr(PTR(work, 0x64));
}
