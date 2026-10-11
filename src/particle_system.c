#include "particle_system.h"

#include "global.h"

#include "camera.h"
#include "filesystem.h"
#include "heap.h"
#include "particle_system_internal.h"
#include "sys_task.h"
#include "sys_task_api.h"

typedef void *(*ParticleAllocFunc)(u32 size);

// Field objects returned by ParticleEmitter_FindField
typedef struct ParticleMagnetField {
    VecFx32 pos;
    fx16 force;
} ParticleMagnetField;

typedef struct ParticleSpinField {
    u16 angle;
    u16 axis;
} ParticleSpinField;

typedef struct ParticleConvergenceField {
    VecFx32 pos;
    fx16 force;
} ParticleConvergenceField;

enum ParticleFieldType {
    PARTICLE_FIELD_GRAVITY,
    PARTICLE_FIELD_RANDOM,
    PARTICLE_FIELD_MAGNET,
    PARTICLE_FIELD_SPIN,
    PARTICLE_FIELD_SCFIELD,
    PARTICLE_FIELD_CONVERGENCE,
};

// asm/particle_system_data.s
extern const VecFx32 gParticleCameraUp;
extern const VecFx32 gParticleCameraTarget;
extern const VecFx32 gParticleCameraPos;
extern const ParticleAllocFunc gParticleSystemAllocFuncs[16];
// [0]: system whose resource is being loaded, [1]: param of the running ParticleSystem_CreateEmitterWithParam.
// gParticleSystems directly follows it, so the slot allocators reach system n as gParticleSystemCallbackState[2 + n].
extern void *gParticleSystemCallbackState[2 + 16];
extern ParticleSystem *gParticleSystems[16];

// SPL library functions not declared in include/library/spl.h
extern void *SPL_Init(ParticleAllocFunc alloc, int, int, int, int, int);
extern void SPL_Load(void *, void *);
extern void SPL_LoadTexByVRAMManager(void *);
extern int SPL_LoadTexByCallbackFunction(void *, texAllocFun);
extern void SPL_LoadTexPlttByVRAMManager(void *);
extern int SPL_LoadTexPlttByCallbackFunction(void *, plttAllocFun);
extern void SPL_Draw(void *, void *);
extern void SPL_Calc(void *);
extern SPLEmitter *SPL_Create(SPLManager *, int, void (*)(SPLEmitter *));
extern void SPL_DeleteAll(void *);
extern void SPL_Delete(void *, SPLEmitter *);
extern void spl_calc_gravity(const void *, SPLParticle *, VecFx32 *, SPLEmitter *);
extern void spl_calc_random(const void *, SPLParticle *, VecFx32 *, SPLEmitter *);
extern void spl_calc_magnet(const void *, SPLParticle *, VecFx32 *, SPLEmitter *);
extern void spl_calc_spin(const void *, SPLParticle *, VecFx32 *, SPLEmitter *);
extern void spl_calc_scfield(const void *, SPLParticle *, VecFx32 *, SPLEmitter *);
extern void spl_calc_convergence(const void *, SPLParticle *, VecFx32 *, SPLEmitter *);

extern void NNS_GfdGetFrmTexVramState(void *);
extern void NNS_GfdSetFrmTexVramState(void *);
extern int NNS_GfdFreeLnkTexVram(u32);
extern void NNS_GfdGetFrmPlttVramState(void *);
extern void NNS_GfdSetFrmPlttVramState(void *);
extern int NNS_GfdFreeLnkPlttVram(u32);
extern void NNS_G3dGlbFlushP(void);

void *ParticleSystem_AllocSlot0(u32 size);
void *ParticleSystem_AllocSlot1(u32 size);
void *ParticleSystem_AllocSlot2(u32 size);
void *ParticleSystem_AllocSlot3(u32 size);
void *ParticleSystem_AllocSlot4(u32 size);
void *ParticleSystem_AllocSlot5(u32 size);
void *ParticleSystem_AllocSlot6(u32 size);
void *ParticleSystem_AllocSlot7(u32 size);
void *ParticleSystem_AllocSlot8(u32 size);
void *ParticleSystem_AllocSlot9(u32 size);
void *ParticleSystem_AllocSlot10(u32 size);
void *ParticleSystem_AllocSlot11(u32 size);
void *ParticleSystem_AllocSlot12(u32 size);
void *ParticleSystem_AllocSlot13(u32 size);
void *ParticleSystem_AllocSlot14(u32 size);
void *ParticleSystem_AllocSlot15(u32 size);
void ParticleSystem_LoadResource(ParticleSystem *system);
void ParticleSystem_LoadResourceTask(SysTask *task, ParticleSystem *system);
void *ParticleEmitter_FindField(SPLEmitter *emitter, int type);
void ParticleEmitter_SetGravity(SPLEmitter *emitter, VecFx16 *value);
void ParticleEmitter_SetMagnetPos(SPLEmitter *emitter, VecFx32 *value);
void ParticleEmitter_GetMagnetPos(SPLEmitter *emitter, VecFx32 *value);
void ParticleEmitter_SetMagnetForce(SPLEmitter *emitter, s16 *value);
void ParticleEmitter_GetMagnetForce(SPLEmitter *emitter, s16 *value);
void ParticleEmitter_SetSpinAngle(SPLEmitter *emitter, u16 *value);
void ParticleEmitter_GetSpinAngle(SPLEmitter *emitter, u16 *value);
void ParticleEmitter_SetSpinAxis(SPLEmitter *emitter, u16 *value);
void ParticleEmitter_GetSpinAxis(SPLEmitter *emitter, u16 *value);
void ParticleEmitter_SetConvergencePos(SPLEmitter *emitter, VecFx32 *value);
void ParticleEmitter_GetConvergencePos(SPLEmitter *emitter, VecFx32 *value);
void ParticleEmitter_SetConvergenceForce(SPLEmitter *emitter, s16 *value);
void ParticleEmitter_GetConvergenceForce(SPLEmitter *emitter, s16 *value);

void ParticleSystem_ZeroAll(void) {
    int i;
    for (i = 0; i < 16; i++) {
        gParticleSystems[i] = NULL;
    }
}

SPLEmitter *ParticleSystem_New(texAllocFun texAlloc, plttAllocFun plttAlloc, void *particleHeap, u32 workSize, BOOL makeCamera, enum HeapID heapID) {
    ParticleSystem *system;
    int slot;

    for (slot = 0; slot < 16; slot++) {
        if (gParticleSystems[slot] == NULL) {
            break;
        }
    }
    if (slot >= 16) {
        return NULL;
    }
    system = Heap_Alloc(heapID, sizeof(ParticleSystem));
    GF_ASSERT(system != NULL);
    memset(system, 0, sizeof(ParticleSystem));
    system->texAlloc = texAlloc;
    system->plttAlloc = plttAlloc;
    system->pos = gParticleCameraPos;
    system->up = gParticleCameraUp;
    system->target = gParticleCameraTarget;
    memset(particleHeap, 0, workSize);
    system->heapStart = particleHeap;
    system->heapCurrent = particleHeap;
    system->heapEnd = (u8 *)particleHeap + workSize;
    system->slot = slot;
    gParticleSystems[slot] = system;
    if (makeCamera == TRUE) {
        system->camera = Camera_New(heapID);
        system->unk_24 = 0;
        system->unk_28 = 0;
        system->unk_2C = 0;
        system->perspective = 2 * FX32_ONE;
        Camera_Init_FromTargetAndPos(&gParticleCameraTarget, &gParticleCameraPos, system->perspective, 0, FALSE, system->camera);
        system->perspectiveType = FALSE;
        Camera_SetStaticPtr(system->camera);
    }
    system->manager = SPL_Init(gParticleSystemAllocFuncs[slot], 20, 200, 5, 6, 63);
    ParticleSystem_SetCameraUp(system, &gParticleCameraUp);
    return (SPLEmitter *)system;
}

void ParticleSystem_Free(SPLEmitter *emitter) {
    ParticleSystem *system = (ParticleSystem *)emitter;
    int i;
    int j;

    ParticleSystem_DeleteAllEmitters(system);
    if (system->flags & 1) {
        NNS_GfdSetFrmTexVramState(system->texKeys);
    } else if (system->flags & 2) {
        for (i = 0; i < 16; i++) {
            if (system->texKeys[i] != 0) {
                NNS_GfdFreeLnkTexVram(system->texKeys[i]);
                system->texKeys[i] = 0;
            }
        }
    }
    if (system->flags & 4) {
        NNS_GfdSetFrmPlttVramState(system->plttKeys);
    } else if (system->flags & 8) {
        for (i = 0; i < 16; i++) {
            if (system->plttKeys[i] != 0) {
                NNS_GfdFreeLnkPlttVram(system->plttKeys[i]);
                system->plttKeys[i] = 0;
            }
        }
    }
    system->flags = 0;
    system->emitter = NULL;
    if (system->resource != NULL) {
        Heap_Free(system->resource);
        system->resource = NULL;
    }
    for (j = 0; j < 16; j++) {
        if (gParticleSystems[j] == system) {
            gParticleSystems[j] = NULL;
            break;
        }
    }
    if (system->camera != NULL) {
        Camera_Delete(system->camera);
    }
    Heap_Free(system);
}

void ParticleSystem_FreeAll(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (gParticleSystems[i] != NULL) {
            ParticleSystem_Free((SPLEmitter *)gParticleSystems[i]);
        }
    }
}

// SPL allocator of one slot: 4-aligned bump allocation in the system's heap
#define DEFINE_SLOT_ALLOC(name, slot)                                                        \
    void *name(u32 size) {                                                                   \
        ParticleSystem *system = (ParticleSystem *)gParticleSystemCallbackState[2 + (slot)]; \
        u8 *ret = system->heapCurrent;                                                       \
        u8 *next = ret + size;                                                               \
        if ((u32)next & 3) {                                                                 \
            next += 4 - ((u32)next & 3);                                                     \
        }                                                                                    \
        system->heapCurrent = next;                                                          \
        GF_ASSERT(next < (u8 *)system->heapEnd);                                             \
        return ret;                                                                          \
    }

DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot0, 0)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot1, 1)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot2, 2)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot3, 3)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot4, 4)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot5, 5)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot6, 6)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot7, 7)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot8, 8)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot9, 9)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot10, 10)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot11, 11)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot12, 12)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot13, 13)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot14, 14)
DEFINE_SLOT_ALLOC(ParticleSystem_AllocSlot15, 15)

void *ParticleSystem_LoadResourceFile(NarcId narcId, int fileId, enum HeapID heapID) {
    return AllocAndReadWholeNarcMemberByIdPair(narcId, fileId, heapID);
}

void ParticleSystem_SetResource(SPLEmitter *emitter, void *data, u32 flag, BOOL loadNow) {
    ParticleSystem *system = (ParticleSystem *)emitter;
    int i;
    int j;

    GF_ASSERT(system->manager != NULL);
    GF_ASSERT(system->resource == NULL);
    system->flags = flag;
    if (flag & 1) {
        NNS_GfdGetFrmTexVramState(system->texKeys);
    } else if (flag & 2) {
        for (i = 0; i < 16; i++) {
            system->texKeys[i] = 0;
        }
    }
    if (flag & 4) {
        NNS_GfdGetFrmPlttVramState(system->plttKeys);
    } else if (flag & 8) {
        for (j = 0; j < 16; j++) {
            system->plttKeys[j] = 0;
        }
    }
    system->resource = data;
    if (loadNow == TRUE) {
        ParticleSystem_LoadResource(system);
        return;
    }
    SysTask_CreateOnVWaitQueue((SysTaskFunc)ParticleSystem_LoadResourceTask, system, 5);
}

void ParticleSystem_LoadResource(ParticleSystem *system) {
    texAllocFun texAlloc;
    plttAllocFun plttAlloc;
    void *manager;

    SPL_Load(system->manager, system->resource);
    gParticleSystemCallbackState[0] = system;
    texAlloc = system->texAlloc;
    manager = system->manager;
    if (texAlloc == NULL) {
        SPL_LoadTexByVRAMManager(manager);
    } else {
        SPL_LoadTexByCallbackFunction(manager, texAlloc);
    }
    plttAlloc = system->plttAlloc;
    manager = system->manager;
    if (plttAlloc == NULL) {
        SPL_LoadTexPlttByVRAMManager(manager);
    } else {
        SPL_LoadTexPlttByCallbackFunction(manager, plttAlloc);
    }
    gParticleSystemCallbackState[0] = NULL;
}

void ParticleSystem_LoadResourceTask(SysTask *task, ParticleSystem *system) {
    ParticleSystem_LoadResource(system);
    SysTask_Destroy(task);
}

void ParticleSystem_RegisterTexKey(NNSGfdTexKey texKey) {
    int i;
    u8 *cursor;
    ParticleSystem *system;

    GF_ASSERT(texKey != 0);
    GF_ASSERT(gParticleSystemCallbackState[0] != NULL);
    system = gParticleSystemCallbackState[0];
    cursor = (u8 *)system;
    for (i = 0; i < 16; i++) {
        if (*(u32 *)(cursor + offsetof(ParticleSystem, texKeys)) == 0) { // system->texKeys[i]
            system->texKeys[i] = texKey;
            return;
        }
        cursor += sizeof(u32);
    }
    GF_ASSERT(FALSE);
}

void ParticleSystem_RegisterPlttKey(NNSGfdPlttKey plttKey) {
    int i;
    u8 *cursor;
    ParticleSystem *system;

    GF_ASSERT(plttKey != 0);
    GF_ASSERT(gParticleSystemCallbackState[0] != NULL);
    system = gParticleSystemCallbackState[0];
    cursor = (u8 *)system;
    for (i = 0; i < 16; i++) {
        if (*(u32 *)(cursor + offsetof(ParticleSystem, plttKeys)) == 0) { // system->plttKeys[i]
            system->plttKeys[i] = plttKey;
            return;
        }
        cursor += sizeof(u32);
    }
    GF_ASSERT(FALSE);
}

void ParticleSystem_Draw(ParticleSystem *system) {
    if (system->camera != NULL) {
        Camera_ApplyPerspectiveType(system->perspectiveType, system->camera);
        Camera_SetStaticPtr(system->camera);
        Camera_PushLookAtToNNSGlb();
    }
    NNS_G3dGlbFlushP();
    SPL_Draw(system->manager, &NNS_G3dGlb.cameraMtx);
    if (system->camera != NULL) {
        Camera_UnsetStaticPtr();
    }
    NNS_G3dGlbFlushP();
}

void ParticleSystem_Calc(ParticleSystem *system) {
    SPL_Calc(system->manager);
}

int ParticleSystem_CountActive(void) {
    int count = 0, i;
    for (i = 0; i < 16; i++) {
        if (gParticleSystems[i] != NULL) {
            count++;
        }
    }
    return count;
}

int ParticleSystem_DrawAll(void) {
    int count = 0, i;
    for (i = 0; i < 16; i++) {
        if (gParticleSystems[i] != NULL) {
            ParticleSystem_Draw(gParticleSystems[i]);
            count++;
        }
    }
    return count;
}

int ParticleSystem_CalcAll(void) {
    int count = 0, i;
    for (i = 0; i < 16; i++) {
        if (gParticleSystems[i] != NULL) {
            ParticleSystem_Calc(gParticleSystems[i]);
            count++;
        }
    }
    return count;
}

void ParticleSystem_CreateEmitter(ParticleSystem *system, int resNo, void (*callback)(SPLEmitter *)) {
    system->emitter = SPL_Create(system->manager, resNo, callback);
}

void ParticleSystem_CreateEmitterWithParam(SPLEmitter *emitter, int resNo, void (*callback)(SPLEmitter *), void *arg) {
    ParticleSystem *system = (ParticleSystem *)emitter;
    SPLEmitter *result;

    // volatile: keeps the original store order of the param and the emitter
    *(void *volatile *)&gParticleSystemCallbackState[1] = arg;
    result = SPL_CreateWithInitialize(system->manager, resNo, callback);
    *(void *volatile *)&gParticleSystemCallbackState[1] = NULL;
    *(void *volatile *)&system->emitter = result;
}

BOOL ParticleSystem_GetActiveEmitterCount(SPLEmitter *emitter) {
    return ((ParticleSystem *)emitter)->manager->act_emtr_list.node_num;
}

void ParticleSystem_DeleteAllEmitters(ParticleSystem *system) {
    SPL_DeleteAll(system->manager);
}

void ParticleSystem_DeleteEmitter(ParticleSystem *system, SPLEmitter *emitter) {
    SPL_Delete(system->manager, emitter);
}

void *ParticleSystem_GetHeap(SPLEmitter *emitter) {
    return ((ParticleSystem *)emitter)->heapStart;
}

void ParticleSystem_GetCameraUp(ParticleSystem *system, VecFx32 *out) {
    *out = system->up;
}

void ParticleSystem_SetCameraUp(ParticleSystem *system, const VecFx32 *up) {
    system->up = *up;
    Camera_SetLookAtCamUp((VecFx32 *)up, system->camera);
}

void *ParticleSystem_GetEmitterParam(void) {
    return gParticleSystemCallbackState[1];
}

void ParticleSystem_GetDefaultCameraUp(VecFx32 *out) {
    *out = gParticleCameraUp;
}

Camera *ParticleSystem_GetCamera(SPLEmitter *emitter) {
    return ((ParticleSystem *)emitter)->camera;
}

void ParticleSystem_SetPerspectiveType(SPLEmitter *emitter, int type) {
    ((ParticleSystem *)emitter)->perspectiveType = type;
}

int ParticleSystem_GetPerspectiveType(ParticleSystem *system) {
    return system->perspectiveType;
}

void ParticleEmitter_GetAxis(SPLEmitter *emitter, VecFx16 *axis) {
    axis->x = *(const u16 *)&emitter->axis.x;
    axis->y = *(const u16 *)&emitter->axis.y;
    axis->z = *(const u16 *)&emitter->axis.z;
}

void *ParticleEmitter_FindField(SPLEmitter *emitter, int type) {
    SPLResource *resource = emitter->p_res;
    register int j;
    int count = resource->fld_num;
    register SPLField *field;

    if (count == 0) {
        return NULL;
    }
    j = 0;
    if (count > 0) {
        field = resource->fld_ary;
        do {
            if (field == NULL) {
                goto next;
            }
            switch (type) {
            case PARTICLE_FIELD_GRAVITY:
                if (field->p_exec == spl_calc_gravity) {
                    return (void *)field->p_obj;
                }
                break;
            case PARTICLE_FIELD_RANDOM:
                if (field->p_exec == spl_calc_random) {
                    return (void *)field->p_obj;
                }
                break;
            case PARTICLE_FIELD_MAGNET:
                if (field->p_exec == spl_calc_magnet) {
                    return (void *)field->p_obj;
                }
                break;
            case PARTICLE_FIELD_SPIN:
                if (field->p_exec == spl_calc_spin) {
                    return (void *)field->p_obj;
                }
                break;
            case PARTICLE_FIELD_SCFIELD:
                if (field->p_exec == spl_calc_scfield) {
                    return (void *)field->p_obj;
                }
                break;
            case PARTICLE_FIELD_CONVERGENCE:
                if (field->p_exec == spl_calc_convergence) {
                    return (void *)field->p_obj;
                }
                break;
            default:
                return NULL;
            }
next:
            j++;
            field++;
        } while (j < count);
    }
    return NULL;
}

void ParticleEmitter_SetGravity(SPLEmitter *emitter, VecFx16 *value) {
    VecFx16 *gravity = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_GRAVITY);
    if (gravity != NULL) {
        *gravity = *value;
    }
}

void ParticleEmitter_SetMagnetPos(SPLEmitter *emitter, VecFx32 *value) {
    ParticleMagnetField *magnet = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_MAGNET);
    if (magnet != NULL) {
        magnet->pos = *value;
    }
}

void ParticleEmitter_GetMagnetPos(SPLEmitter *emitter, VecFx32 *value) {
    ParticleMagnetField *magnet = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_MAGNET);
    if (magnet == NULL) {
        VecFx32 zero = { 0, 0, 0 };
        *value = zero;
        return;
    }
    *value = magnet->pos;
}

void ParticleEmitter_SetMagnetForce(SPLEmitter *emitter, s16 *value) {
    ParticleMagnetField *magnet = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_MAGNET);
    if (magnet != NULL) {
        magnet->force = *value;
    }
}

void ParticleEmitter_GetMagnetForce(SPLEmitter *emitter, s16 *value) {
    ParticleMagnetField *magnet = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_MAGNET);
    if (magnet == NULL) {
        *value = 0;
    } else {
        *value = magnet->force;
    }
}

void ParticleEmitter_SetSpinAngle(SPLEmitter *emitter, u16 *value) {
    ParticleSpinField *spin = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_SPIN);
    if (spin != NULL) {
        spin->angle = *value;
    }
}

void ParticleEmitter_GetSpinAngle(SPLEmitter *emitter, u16 *value) {
    ParticleSpinField *spin = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_SPIN);
    if (spin == NULL) {
        *value = 0;
    } else {
        *value = spin->angle;
    }
}

void ParticleEmitter_SetSpinAxis(SPLEmitter *emitter, u16 *value) {
    ParticleSpinField *spin = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_SPIN);
    if (spin != NULL) {
        spin->axis = *value;
    }
}

void ParticleEmitter_GetSpinAxis(SPLEmitter *emitter, u16 *value) {
    ParticleSpinField *spin = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_SPIN);
    if (spin == NULL) {
        *value = 0;
    } else {
        *value = spin->axis;
    }
}

void ParticleEmitter_SetConvergencePos(SPLEmitter *emitter, VecFx32 *value) {
    ParticleConvergenceField *convergence = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_CONVERGENCE);
    if (convergence != NULL) {
        convergence->pos = *value;
    }
}

void ParticleEmitter_GetConvergencePos(SPLEmitter *emitter, VecFx32 *value) {
    ParticleConvergenceField *convergence = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_CONVERGENCE);
    if (convergence == NULL) {
        VecFx32 zero = { 0, 0, 0 };
        *value = zero;
        return;
    }
    *value = convergence->pos;
}

void ParticleEmitter_SetConvergenceForce(SPLEmitter *emitter, s16 *value) {
    ParticleConvergenceField *convergence = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_CONVERGENCE);
    if (convergence != NULL) {
        convergence->force = *value;
    }
}

void ParticleEmitter_GetConvergenceForce(SPLEmitter *emitter, s16 *value) {
    ParticleConvergenceField *convergence = ParticleEmitter_FindField(emitter, PARTICLE_FIELD_CONVERGENCE);
    if (convergence == NULL) {
        *value = 0;
    } else {
        *value = convergence->force;
    }
}
