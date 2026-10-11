#ifndef TO50_OV15_R21_PRIVATE_H
#define TO50_OV15_R21_PRIVATE_H
#include <nitro/fx/fx.h>
#include <nnsys/g3d.h>

#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern const VecFx32 sBagModelScale;
extern const MtxFx33 sBagModelRotation;
extern void BagApp_UpdateCameraTween(void *, s32);
extern void Camera_Init_FromTargetDistanceAndAngle(void *, void *, void *, u16, u8, BOOL, fx32);
extern void Thunk_G3X_Reset(void);
extern void Camera_PushLookAtToNNSGlb(void);
extern void BagApp_AdvanceAnimLoop(void *);
extern void GF3dRender_DrawModel(void *, const VecFx32 *, const MtxFx33 *, const VecFx32 *);
extern void RequestSwap3DBuffers(u32, u32);
void BagApp_Draw3D(void *);
#endif
