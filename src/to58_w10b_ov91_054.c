#include "global.h"
typedef struct Ov91Vec3 { s32 x, y, z; } Ov91Vec3;
typedef struct Ov91Angle { u16 x, y, z; } Ov91Angle;
extern void *Camera_New(int);
extern void Camera_Init_FromTargetDistanceAndAngle(void *, int, Ov91Angle *, int, int, int, void *);
extern void Camera_SetLookAtCamUp(Ov91Vec3 *, void *);
extern void Camera_SetStaticPtr(void *);
extern void Camera_SetPerspectiveClippingPlane(int, int, void *);
extern const u16 ov91_02261D5C[][4];
void ov91_0225FD2C(u8 *, int, int, int);
void ov91_0225FD2C(u8 *state, int a, int b, int heap) {
    Ov91Angle angle;
    Ov91Vec3 up;
    *(void **)(state + 0x190) = Camera_New(heap);
    *(u32 *)(state + 0x194) = 0;
    *(u32 *)(state + 0x198) = 0;
    *(u32 *)(state + 0x19C) = 0;
    angle.x = 0xF112;
    angle.y = ov91_02261D5C[a][b];
    angle.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(state + 0x194, 0x1D9000, &angle, 0xFA4, 0, 1, *(void **)(state + 0x190));
    up.x = 0;
    up.y = 0x1000;
    up.z = 0;
    Camera_SetLookAtCamUp(&up, *(void **)(state + 0x190));
    Camera_SetStaticPtr(*(void **)(state + 0x190));
    Camera_SetPerspectiveClippingPlane(0xC8000, 0x3E8000, *(void **)(state + 0x190));
}
