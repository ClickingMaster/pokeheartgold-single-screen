#ifndef PRIVATE_OVERLAY_80_02239960_H
#define PRIVATE_OVERLAY_80_02239960_H

#include "global.h"

void *Heap_Alloc(u32 heapId, u32 size);
void Heap_Free(void *ptr);
void ParticleSystem_ZeroAll(void);
void *ParticleSystem_New(u32 (*tex)(u32, int), u32 (*pltt)(u32, int), void *buffer, u32 size, u32 flag, u32 heapId);
void ParticleSystem_Free(void *object);
void *ParticleSystem_GetHeap(void *object);
void *ParticleSystem_GetCamera(void *object);
void ParticleSystem_SetPerspectiveType(void *object, void *camera);
u32 ParticleSystem_CountActive(void);
s32 ParticleSystem_DrawAll(void);
void ParticleSystem_CalcAll(void);
s32 ParticleSystem_GetActiveEmitterCount(void *object);
void ParticleSystem_RegisterTexKey(u32 key);
void ParticleSystem_RegisterPlttKey(u32 key);
void *ParticleSystem_LoadResourceFile(u32 narcId, u32 member, u32 heapId);
void ParticleSystem_SetResource(void *object, void *resource, u32 count, u32 flag);
void Camera_SetPerspectiveClippingPlane(u32 near, u32 far, void *camera);
void Thunk_G3X_Reset(void);
void GF_AssertFail(void);

void *ov80_02239960(u32 heapId);
void ov80_02239980(void *manager);
void *ov80_022399A4(void *manager, u32 index, u32 member, void *camera);
void ov80_02239A1C(void *manager, u32 index);
BOOL ov80_02239A38(void);
void *ov80_02239A60(void *manager, u32 index);
BOOL ov80_02239A74(void *manager);
void ov80_02239A98(void *object);
u32 ov80_02239AB0(u32 size, int flag);
u32 ov80_02239AD4(u32 size, int flag);

#endif
