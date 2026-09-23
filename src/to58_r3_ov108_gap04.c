#include "global.h"
#include "camera.h"

#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern const u8 ov108_021EA9C4[];
extern const VecFx32 ov108_021EA9D8;

void ov108_021E9230(void *p, enum HeapID heap);
void ov108_021E9230(void *p, enum HeapID heap) {
    PTR(p, 0) = Camera_New(heap);
    U32(p, 0xdc) = 0;
    U32(p, 0xe0) = 0;
    U32(p, 0xe4) = 0;
    Camera_Init_FromTargetDistanceAndAngle((VecFx32 *)((u8 *)p + 0xdc), 0x15b000, (const CameraAngle *)ov108_021EA9C4, 0x5c1, 0, TRUE, PTR(p, 0));
    Camera_SetStaticPtr(PTR(p, 0));
    Camera_SetPerspectiveClippingPlane(0x64000, 0x4b0000, PTR(p, 0));
    Camera_OffsetLookAtPosAndTarget(&ov108_021EA9D8, PTR(p, 0));
}
