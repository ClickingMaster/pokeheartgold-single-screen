#include "frontier/frontier_particles.h"

#include "global.h"

#include "assert.h"
#include "camera.h"
#include "heap.h"
#include "swap_3d_buffers.h"

static void FrontierParticles_FreeSystem(SPLEmitter *system);
static u32 FrontierParticles_AllocTexVram(u32 size, BOOL is4x4comp);
static u32 FrontierParticles_AllocPlttVram(u32 size, BOOL is4pltt);

FrontierParticles *FrontierParticles_New(enum HeapID heapId) {
    FrontierParticles *particles = Heap_Alloc(heapId, sizeof(FrontierParticles));
    MI_CpuFill8(particles, 0, sizeof(FrontierParticles));
    particles->heapId = heapId;
    ParticleSystem_ZeroAll();
    return particles;
}

void FrontierParticles_Free(FrontierParticles *particles) {
    s32 i;
    for (i = 0; i < FRONTIER_PARTICLE_SYSTEM_MAX; i++) {
        if (particles->systems[i] != NULL) {
            FrontierParticles_FreeSystem(particles->systems[i]);
        }
    }
    Heap_Free(particles);
}

SPLEmitter *FrontierParticles_CreateSystem(FrontierParticles *particles, u32 index, u32 member, int perspectiveType) {
    void *buffer;
    SPLEmitter *system;
    void *resource;
    Camera *camera;
    GF_ASSERT(particles->systems[index] == NULL);
    buffer = Heap_Alloc((enum HeapID)particles->heapId, 0x4800);
    system = ParticleSystem_New(FrontierParticles_AllocTexVram, FrontierParticles_AllocPlttVram, buffer, 0x4800, TRUE, (enum HeapID)particles->heapId);
    camera = ParticleSystem_GetCamera(system);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, FX32_CONST(900), camera);
    ParticleSystem_SetPerspectiveType(system, perspectiveType);
    resource = ParticleSystem_LoadResourceFile(NARC_a_1_8_8, member, (enum HeapID)particles->heapId);
    ParticleSystem_SetResource(system, resource, 10, TRUE);
    particles->systems[index] = system;
    return system;
}

void FrontierParticles_DestroySystem(FrontierParticles *particles, u32 index) {
    GF_ASSERT(particles->systems[index] != NULL);
    FrontierParticles_FreeSystem(particles->systems[index]);
    particles->systems[index] = NULL;
}

BOOL FrontierParticles_Update(void) {
    Thunk_G3X_Reset();
    if (ParticleSystem_CountActive() == 0) {
        return FALSE;
    }
    if (ParticleSystem_DrawAll() > 0) {
        Thunk_G3X_Reset();
    }
    ParticleSystem_CalcAll();
    return TRUE;
}

SPLEmitter *FrontierParticles_GetSystem(FrontierParticles *particles, u32 index) {
    GF_ASSERT(particles->systems[index] != NULL);
    return particles->systems[index];
}

BOOL FrontierParticles_AreAllDone(FrontierParticles *particles) {
    s32 i;
    for (i = 0; i < FRONTIER_PARTICLE_SYSTEM_MAX; i++) {
        if (particles->systems[i] != NULL && ParticleSystem_GetActiveEmitterCount(particles->systems[i]) > 0) {
            return FALSE;
        }
    }
    return TRUE;
}

static void FrontierParticles_FreeSystem(SPLEmitter *system) {
    void *buffer = ParticleSystem_GetHeap(system);
    ParticleSystem_Free(system);
    Heap_Free(buffer);
}

static u32 FrontierParticles_AllocTexVram(u32 size, BOOL is4x4comp) {
    u32 key = NNS_GfdDefaultFuncAllocTexVram(size, is4x4comp, 0);
    GF_ASSERT(key != 0);
    ParticleSystem_RegisterTexKey(key);
    return (u16)key << 3;
}

static u32 FrontierParticles_AllocPlttVram(u32 size, BOOL is4pltt) {
    u32 key = NNS_GfdDefaultFuncAllocPlttVram(size, is4pltt, 1);
    GF_ASSERT(key != 0);
    ParticleSystem_RegisterPlttKey(key);
    return (u16)key << 3;
}
