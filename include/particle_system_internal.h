#ifndef POKEHEARTGOLD_PARTICLE_SYSTEM_INTERNAL_H
#define POKEHEARTGOLD_PARTICLE_SYSTEM_INTERNAL_H

#include "particle_system.h"

// The public API (particle_system.h) passes a ParticleSystem * as SPLEmitter *.
typedef struct ParticleSystem {
    SPLManager *manager;
    void *resource;
    void *emitter;
    void *heapStart;
    void *heapCurrent; // bump pointer of the slot's SPL allocator
    void *heapEnd;
    texAllocFun texAlloc; // NULL: SPL_LoadTexByVRAMManager
    plttAllocFun plttAlloc;
    Camera *camera; // only when created with makeCamera
    u32 unk_24;
    u32 unk_28;
    u32 unk_2C;
    u16 perspective;
    u8 pad_32[2];
    VecFx32 pos;
    VecFx32 up;
    VecFx32 target;
    u32 texKeys[16];  // frame VRAM state (flag 1) or linked VRAM keys (flag 2)
    u32 plttKeys[16]; // same with flags 4 / 8
    u8 flags;
    u8 unk_D9;
    u8 slot;
    u8 perspectiveType;
} ParticleSystem;

void ParticleSystem_Draw(ParticleSystem *system);
void ParticleSystem_Calc(ParticleSystem *system);
void ParticleSystem_CreateEmitter(ParticleSystem *system, int resNo, void (*callback)(SPLEmitter *));
void ParticleSystem_DeleteAllEmitters(ParticleSystem *system);
void ParticleSystem_DeleteEmitter(ParticleSystem *system, SPLEmitter *emitter);
void ParticleSystem_GetCameraUp(ParticleSystem *system, VecFx32 *out);
void ParticleSystem_SetCameraUp(ParticleSystem *system, const VecFx32 *up);
int ParticleSystem_GetPerspectiveType(ParticleSystem *system);
void ParticleSystem_GetDefaultCameraUp(VecFx32 *out);
void ParticleSystem_FreeAll(void);

#endif // POKEHEARTGOLD_PARTICLE_SYSTEM_INTERNAL_H
