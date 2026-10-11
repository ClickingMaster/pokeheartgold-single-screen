#ifndef OV01_021FEA0C_PRIVATE_H
#define OV01_021FEA0C_PRIVATE_H
#include <string.h>

#include "global.h"
typedef struct Vec3 {
    s32 x, y, z;
} Vec3;
typedef struct Copy20 {
    u32 v[5];
} Copy20;
typedef struct SpawnSmall {
    u32 kind;
    void *ctx;
    void *fieldSystem;
    void *member;
    void *mapObj;
} SpawnSmall;
typedef struct Rot9 {
    s32 v[9];
} Rot9;
typedef struct EffectCtx {
    u8 raw[0x28];
} EffectCtx;
typedef struct EffectWork {
    s32 state;
    s32 dir;
    s32 counter;
    u8 pad[0x10];
    EffectCtx *ctx;
} EffectWork;
extern void *FieldEffectManager_HeapAllocAndFill(u32, u32, u32, u32);
extern void FieldEffectManager_HeapFree(void *);
extern void GF_AssertFail(void);
extern void ov01_021F19F4(void *, void *, u32, u32, u32);
extern void RenderObj_InitFromModel(void *, void *);
extern void RenderModel_Free(void *);
extern void *FieldEffect_GetState(void *);
extern Copy20 *FieldEffect_GetUserData(void *);
extern void *FieldEffect_GetParam(void *);
extern void FieldEffect_SetPosition(void *, Vec3 *);
extern void sub_0205F9A0(void *, Vec3 *);
extern void MapObject_CopyPositionVector(void *, Vec3 *);
extern void *FieldEffectManager_GetFromMapObject(void *);
extern void *MapObject_GetFieldSystem(void *);
extern void *FieldEffectManager_GetRendererData(void *, u32);
extern void sub_0206121C(void *, Vec3 *);
extern s32 MapObject_GetPriorityPlusValue(void *, s32);
extern void FieldEffectManager_CreateEffect(void *, const void *, Vec3 *, u32, SpawnSmall *, s32);
extern const u8 ov01_022090FC[];
extern const Vec3 ov01_022090F0;
extern void Math_CalcRotationMatrixXYZDeg(Rot9 *, u16, u16, u16);
extern void RenderObj_Draw(void *, Vec3 *, Vec3 *, Rot9 *);
extern void FieldEffect_GetPosition(void *, Vec3 *);
void *ov01_021FEA0C(u32);
void ov01_021FEA20(void *);
void ov01_021FEA30(void *);
void ov01_021FEA38(void *);
void ov01_021FEA48(void *);
void ov01_021FEA7C(void *);
void ov01_021FEA90(void *);
void ov01_021FEAA0(void *);
void ov01_021FEB30(void *, void *);
BOOL ov01_021FEB3C(void *, void *);
void ov01_021FEB78(void *, void *);
void ov01_021FEB8C(void *, void *);
void ov01_021FEAB0(void *, s32, s32, u32, u32);
void ov01_021FEBC0(void *task, EffectWork *data);
#endif
