#ifndef OVERLAY_59_RESIDUAL_16_R40_PRIVATE_H
#define OVERLAY_59_RESIDUAL_16_R40_PRIVATE_H

#include <nnsys/gfd.h>

#include "global.h"

#include "heap.h"

void GF_AssertFail(void);
void ParticleSystem_ZeroAll(void);
void ParticleSystem_RegisterTexKey(u32 key);
void ParticleSystem_RegisterPlttKey(u32 key);
void ParticleSystem_SetPerspectiveType(void *emitter, int type);
void *ParticleSystem_LoadResourceFile(u32 narcId, u32 member, u32 heapId);
void ParticleSystem_SetResource(void *emitter, void *resource, u32 count, BOOL loadNow);

void ov59_0223C474(void);
u32 ov59_0223C4B4(u32 size, BOOL is4x4);
u32 ov59_0223C4D0(u32 size, BOOL is4color);
void *ov59_0223C53C(u32 heapId);
void ov59_0223C584(void *work);
void ov59_0223C4EC(void *work);
void *Heap_Alloc(enum HeapID heapId, u32 size);
void *ParticleSystem_New(u32 (*tex)(u32, BOOL), u32 (*pltt)(u32, BOOL), void *buffer, u32 size, BOOL makeCamera, enum HeapID heapId);
void *ParticleSystem_GetCamera(void *emitter);
void Camera_SetPerspectiveClippingPlane(s32 near, s32 far, void *camera);

#endif
