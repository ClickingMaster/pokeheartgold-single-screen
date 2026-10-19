#ifndef POKEHEARTGOLD_PARTICLE_SCENE_H
#define POKEHEARTGOLD_PARTICLE_SCENE_H

#include "gf_3d_vramman.h"
#include "heap.h"

typedef struct ParticleSceneParam {
    enum HeapID heapID;
    int resource;
} ParticleSceneParam;

typedef struct ParticleScene ParticleScene;

void ParticleScene_HidePlanes(void);
void ParticleScene_SetBlend(void);
GF3DVramMan *ParticleScene_CreateVramMan(enum HeapID);
void ParticleScene_ReserveVram(void);
void ParticleScene_DrawAndSwap(void);
ParticleScene *ParticleScene_New(ParticleSceneParam *);
void ParticleScene_CreateEmitter(ParticleScene *, int);
BOOL ParticleScene_IsActive(ParticleScene *);
void ParticleScene_Free(ParticleScene *);

#endif // POKEHEARTGOLD_PARTICLE_SCENE_H
