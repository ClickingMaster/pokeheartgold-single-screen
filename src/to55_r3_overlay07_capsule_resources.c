#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "pokemon.h"
#include "seal_case.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02091054.h"

typedef struct Ov07Input {
    u32 unk0;
    u32 fallbackSpecies;
    Pokemon *mon;
} Ov07Input;

typedef struct Ov07Work {
    enum HeapID heapId;
    Ov07Input input;
    int count;
    void *resource[9];
    void *seal[9];
    u32 unused[9];
    u8 pad80[8];
    void *unk88;
    NARC *narc;
    int state;
    int useCapsule;
    u32 species;
    s32 form;
    CAPSULE capsule;
} Ov07Work;

Ov07Work *ov07_02232670(enum HeapID heapId);
Ov07Work *ov07_02232694(enum HeapID heapId, const Ov07Input *input);
void ov07_022326C0(Ov07Work *work, const Ov07Input *input);
void ov07_02232730(SysTask *task, Ov07Work *work);
void ov07_022329B0(Ov07Work *work);

extern void *ov07_022325F4(u32 species);
extern void *BattleAnimSystem_NewParticleSystemFromNarc(enum HeapID heapId, int narcId, void *data, int unused);
extern void *BattleAnimSystem_NewParticleSystemFromOpenNarc(NARC *narc, enum HeapID heapId, u8 resourceId, int unused);

Ov07Work *ov07_02232670(enum HeapID heapId) {
    Ov07Work *work = Heap_Alloc(heapId, sizeof(Ov07Work));
    if (work == NULL) {
        GF_AssertFail();
        return NULL;
    }
    work->heapId = heapId;
    work->unk88 = NULL;
    return work;
}

Ov07Work *ov07_02232694(enum HeapID heapId, const Ov07Input *input) {
    Ov07Work *work = ov07_02232670(heapId);
    int i;
    ov07_022326C0(work, input);
    work->count = 0;
    for (i = 0; i < 9; i++) {
        work->resource[i] = NULL;
        work->seal[i] = NULL;
        work->unused[i] = 0;
    }
    return work;
}

void ov07_022326C0(Ov07Work *work, const Ov07Input *input) {
    work->input = *input;
    work->form = GetMonData(work->input.mon, 0xA2, NULL);
    if (work->form >= 13) {
        GF_AssertFail();
    }
    if (work->form == 0) {
        work->useCapsule = 0;
        if (work->input.fallbackSpecies != 0) {
            work->species = work->input.fallbackSpecies;
        } else {
            work->species = GetMonData(work->input.mon, 0x9B, NULL);
        }
    } else {
        work->useCapsule = 1;
        GetMonData(work->input.mon, 0xAB, &work->capsule);
    }
}

void ov07_02232730(SysTask *task, Ov07Work *work) {
    u8 resourceId0, resourceId3, resourceId2, resourceId1;
    switch (work->state) {
    case 0: {
        int i;
        work->narc = NARC_New((NarcId)0x5F, work->heapId);
        for (i = 0; i < 2; i++) {
            SEAL *seal = CapsuleGetSealI(&work->capsule, i);
            if (seal != NULL) {
                s32 sealId = SealOnCapsuleGetID(seal);
                if (sealId != 0 && sealId < 0x51) {
                    resourceId0 = sub_02091084((u8)sealId);
                    work->seal[work->count] = seal;
                    if (work->resource[work->count] != NULL) {
                        GF_AssertFail();
                    }
                    work->resource[work->count] = BattleAnimSystem_NewParticleSystemFromOpenNarc(work->narc, work->heapId, resourceId0, 0);
                    {
                        void *resource = work->resource[work->count];
                        if (resource == NULL) {
                            GF_ASSERT(resource != NULL);
                        } else {
                            work->count++;
                        }
                    }
                }
            }
        }
        work->state++;
        return;
    }
    case 1: {
        int i;
        for (i = 2; i < 4; i++) {
            SEAL *seal = CapsuleGetSealI(&work->capsule, i);
            if (seal != NULL) {
                s32 sealId = SealOnCapsuleGetID(seal);
                if (sealId != 0 && sealId < 0x51) {
                    resourceId1 = sub_02091084((u8)sealId);
                    work->seal[work->count] = seal;
                    if (work->resource[work->count] != NULL) {
                        GF_AssertFail();
                    }
                    work->resource[work->count] = BattleAnimSystem_NewParticleSystemFromOpenNarc(work->narc, work->heapId, resourceId1, 0);
                    {
                        void *resource = work->resource[work->count];
                        if (resource == NULL) {
                            GF_ASSERT(resource != NULL);
                        } else {
                            work->count++;
                        }
                    }
                }
            }
        }
        work->state++;
        return;
    }
    case 2: {
        int i;
        for (i = 4; i < 6; i++) {
            SEAL *seal = CapsuleGetSealI(&work->capsule, i);
            if (seal != NULL) {
                s32 sealId = SealOnCapsuleGetID(seal);
                if (sealId != 0 && sealId < 0x51) {
                    resourceId2 = sub_02091084((u8)sealId);
                    work->seal[work->count] = seal;
                    if (work->resource[work->count] != NULL) {
                        GF_AssertFail();
                    }
                    work->resource[work->count] = BattleAnimSystem_NewParticleSystemFromOpenNarc(work->narc, work->heapId, resourceId2, 0);
                    {
                        void *resource = work->resource[work->count];
                        if (resource == NULL) {
                            GF_ASSERT(resource != NULL);
                        } else {
                            work->count++;
                        }
                    }
                }
            }
        }
        work->state++;
        return;
    }
    case 3: {
        int i;
        for (i = 6; i < 8; i++) {
            SEAL *seal = CapsuleGetSealI(&work->capsule, i);
            if (seal != NULL) {
                s32 sealId = SealOnCapsuleGetID(seal);
                if (sealId != 0 && sealId < 0x51) {
                    resourceId3 = sub_02091084((u8)sealId);
                    work->seal[work->count] = seal;
                    if (work->resource[work->count] != NULL) {
                        GF_AssertFail();
                    }
                    work->resource[work->count] = BattleAnimSystem_NewParticleSystemFromOpenNarc(work->narc, work->heapId, resourceId3, 0);
                    {
                        void *resource = work->resource[work->count];
                        if (resource == NULL) {
                            GF_ASSERT(resource != NULL);
                        } else {
                            work->count++;
                        }
                    }
                }
            }
        }
        work->state = 0xFF;
        NARC_Delete(work->narc);
        SysTask_Destroy(task);
        return;
    }
    }
}

void ov07_022329B0(Ov07Work *work) {
    if (work->useCapsule == 0) {
        void *data = ov07_022325F4(work->species);
        work->count = 1;
        work->resource[0] = BattleAnimSystem_NewParticleSystemFromNarc(work->heapId, 0x5F, data, 0);
        work->state = 0xFF;
    } else {
        SysTask *task;
        work->count = 0;
        work->state = 0;
        task = SysTask_CreateOnMainQueue((SysTaskFunc)ov07_02232730, work, 1000);
        ov07_02232730(task, work);
    }
}
