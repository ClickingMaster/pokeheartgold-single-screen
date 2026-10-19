#ifndef POKEHEARTGOLD_FRONTIER_PARTICLES_H
#define POKEHEARTGOLD_FRONTIER_PARTICLES_H

#include "heap.h"
#include "particle_system.h"

#define FRONTIER_PARTICLE_SYSTEM_MAX 8

// The particle systems of the frontier map, created by the script commands.
typedef struct FrontierParticles {
    SPLEmitter *systems[FRONTIER_PARTICLE_SYSTEM_MAX];
    u16 heapId;
} FrontierParticles;

FrontierParticles *FrontierParticles_New(enum HeapID heapId);
void FrontierParticles_Free(FrontierParticles *particles);
SPLEmitter *FrontierParticles_CreateSystem(FrontierParticles *particles, u32 index, u32 member, int perspectiveType);
void FrontierParticles_DestroySystem(FrontierParticles *particles, u32 index);
BOOL FrontierParticles_Update(void);
SPLEmitter *FrontierParticles_GetSystem(FrontierParticles *particles, u32 index);
BOOL FrontierParticles_AreAllDone(FrontierParticles *particles);

#endif
