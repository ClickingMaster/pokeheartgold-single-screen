#ifndef POKEHEARTGOLD_PARTICLE_SYSTEM_H
#define POKEHEARTGOLD_PARTICLE_SYSTEM_H

#include "camera.h"
#include "filesystem_files_def.h"
#include "spl.h"

typedef u32 (*texAllocFun)(u32 szByte, BOOL is4x4comp);
typedef u32 (*plttAllocFun)(u32 szByte, BOOL is4pltt);

// The SPLEmitter * handle below is a particle system (ParticleSystem in particle_system_internal.h).
void ParticleSystem_ZeroAll(void);
SPLEmitter *ParticleSystem_New(texAllocFun texAlloc, plttAllocFun plttAlloc, void *particleHeap, u32 workSize, BOOL makeCamera, enum HeapID heapID);
void ParticleSystem_Free(SPLEmitter *emitter);
Camera *ParticleSystem_GetCamera(SPLEmitter *emitter);
void *ParticleSystem_LoadResourceFile(NarcId narcId, int fileId, enum HeapID heapID);
void ParticleSystem_SetResource(SPLEmitter *emitter, void *data, u32 flag, BOOL loadNow);
void ParticleSystem_RegisterTexKey(NNSGfdTexKey texKey);
void ParticleSystem_RegisterPlttKey(NNSGfdPlttKey plttKey);
void ParticleSystem_CreateEmitterWithParam(SPLEmitter *emitter, int res_no, void (*fp_callback)(struct SPLEmitter *), void *arg);
BOOL ParticleSystem_GetActiveEmitterCount(SPLEmitter *emitter);
void *ParticleSystem_GetEmitterParam(void);
int ParticleSystem_DrawAll(void);
int ParticleSystem_CalcAll(void);
void *ParticleSystem_GetHeap(SPLEmitter *emitter);
int ParticleSystem_CountActive(void);
void ParticleEmitter_GetAxis(SPLEmitter *emitter, VecFx16 *axis);
void ParticleSystem_SetPerspectiveType(SPLEmitter *emitter, int type);

#endif // POKEHEARTGOLD_PARTICLE_SYSTEM_H
