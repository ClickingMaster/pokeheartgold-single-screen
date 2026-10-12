#ifndef POKEHEARTGOLD_FRONTIER_PARTICLES_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_PARTICLES_INTERNAL_H

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

void *FrontierParticles_New(u32 heapId);
void FrontierParticles_Free(void *manager);
void *FrontierParticles_CreateSystem(void *manager, u32 index, u32 member, void *camera);
void FrontierParticles_DestroySystem(void *manager, u32 index);
BOOL FrontierParticles_Update(void);
void *FrontierParticles_GetSystem(void *manager, u32 index);
BOOL FrontierParticles_AreAllDone(void *manager);
void FrontierParticles_FreeSystem(void *object);
u32 FrontierParticles_AllocTexVram(u32 size, int flag);
u32 FrontierParticles_AllocPlttVram(u32 size, int flag);

#endif
