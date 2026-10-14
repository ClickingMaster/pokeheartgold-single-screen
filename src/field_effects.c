#include "field_effects.h"

#include <string.h>

#include "global.h"

#include "heap.h"
#include "sys_task_api.h"

// A fixed-size pool of field effects. Each active effect runs as a main-queue SysTask that calls its update
// callback every frame; FieldEffectPool_DrawAll calls the draw callback of every started effect.

typedef struct FieldEffect FieldEffect;

typedef struct FieldEffectPool {
    int count;
    int activeCount;
    enum HeapID heapID;
    FieldEffect *effects;
} FieldEffectPool;

typedef void (*FieldEffectFunc)(FieldEffect *, void *);
typedef BOOL (*FieldEffectInitFunc)(FieldEffect *, void *);

#define FIELD_EFFECT_FLAG_ACTIVE  (1 << 0)
#define FIELD_EFFECT_FLAG_STARTED (1 << 1) // the init callback succeeded

struct FieldEffect {
    u32 flags;
    int parameter;
    u32 unk8;
    void *userData;
    SysTask *task;
    FieldEffectPool *pool;
    u8 padding[12];
    VecFx32 position;
    u8 state[128]; // per-effect work area handed to the callbacks
    FieldEffectInitFunc init;
    FieldEffectFunc update;
    FieldEffectFunc draw;
    FieldEffectFunc destroy;
    FieldEffectFunc extra1;
    FieldEffectFunc extra2;
};

typedef struct FieldEffectTemplate {
    u32 size; // bytes of the state area cleared before init
    FieldEffectInitFunc init;
    FieldEffectFunc destroy;
    FieldEffectFunc update;
    FieldEffectFunc draw;
    FieldEffectFunc extra1;
    FieldEffectFunc extra2;
} FieldEffectTemplate;

// FieldEffectTemplate without the two extra callbacks
typedef struct FieldEffectBasicTemplate {
    u32 size;
    FieldEffectInitFunc init;
    FieldEffectFunc destroy;
    FieldEffectFunc update;
    FieldEffectFunc draw;
} FieldEffectBasicTemplate;

FieldEffectPool *FieldEffectPool_New(enum HeapID heapID, int count);
void FieldEffectPool_Free(FieldEffectPool *pool);
void FieldEffectPool_Delete(FieldEffectPool *pool);
FieldEffect *FieldEffectPool_AddEffect(FieldEffectPool *pool, const FieldEffectTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority);
FieldEffect *FieldEffectPool_AddBasicEffect(FieldEffectPool *pool, const FieldEffectBasicTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority);
void FieldEffect_Destroy(int value);
void FieldEffectPool_DestroyAll(FieldEffectPool *pool);
void FieldEffectPool_DrawAll(FieldEffectPool *pool);
SysTask *FieldEffect_CreateTask(FieldEffect *effect, u32 priority);
void FieldEffect_Task(SysTask *task, void *data);
FieldEffectPool *FieldEffectPool_AllocHeader(enum HeapID heapID);
void FieldEffectPool_FreeHeader(FieldEffectPool *pool);
FieldEffect *FieldEffectPool_AllocEffects(enum HeapID heapID, int count);
void FieldEffectPool_FreeEffects(FieldEffectPool *pool);
void FieldEffect_Reset(FieldEffect *effect);
int FieldEffectPool_GetCount(FieldEffectPool *pool);
FieldEffect *FieldEffectPool_GetEffects(FieldEffectPool *pool);
BOOL FieldEffect_IsActive(FieldEffect *effect);
void FieldEffect_SetActive(FieldEffect *effect);
void FieldEffect_SetFlags(FieldEffect *effect, u32 flags);
u32 FieldEffect_TestFlags(FieldEffect *effect, u32 flags);
void FieldEffect_SetInitFunc(FieldEffect *effect, FieldEffectInitFunc callback);
BOOL FieldEffect_RunInit(FieldEffect *effect);
void FieldEffect_SetUpdateFunc(FieldEffect *effect, FieldEffectFunc callback);
void FieldEffect_RunUpdate(FieldEffect *effect);
void FieldEffect_SetDrawFunc(FieldEffect *effect, FieldEffectFunc callback);
void FieldEffect_RunDraw(FieldEffect *effect);
void FieldEffect_SetDestroyFunc(FieldEffect *effect, FieldEffectFunc callback);
void FieldEffect_RunDestroy(FieldEffect *effect);
void FieldEffect_SetExtraFunc1(FieldEffect *effect, FieldEffectFunc callback);
void FieldEffect_SetExtraFunc2(FieldEffect *effect, FieldEffectFunc callback);
void *FieldEffect_GetState(FieldEffect *effect);
void FieldEffect_ClearState(FieldEffect *effect, u32 size);
void FieldEffect_SetParam(FieldEffect *effect, int parameter);
int FieldEffect_GetParam(FieldEffect *effect);
void FieldEffect_SetUserData(FieldEffect *effect, void *userData);
FieldEffectUserData *FieldEffect_GetUserData(void *data);
void FieldEffect_SetTask(FieldEffect *effect, SysTask *task);
SysTask *FieldEffect_GetTask(FieldEffect *effect);
void FieldEffect_SetPool(FieldEffect *effect, FieldEffectPool *pool);
void FieldEffect_SetPosition(FieldEffect *effect, const VecFx32 *position);
void FieldEffect_GetPosition(void *data, VecFx32 *position);
BOOL FieldEffect_DefaultInit(FieldEffect *effect, void *state);
void FieldEffect_DefaultUpdate(FieldEffect *effect, void *state);
void FieldEffect_DefaultDraw(FieldEffect *effect, void *state);
void FieldEffect_DefaultDestroy(FieldEffect *effect, void *state);
void FieldEffect_DefaultExtra1(FieldEffect *effect, void *state);
void FieldEffect_DefaultExtra2(FieldEffect *effect, void *state);

FieldEffectPool *FieldEffectPool_New(enum HeapID heapID, int count) {
    FieldEffectPool *pool = FieldEffectPool_AllocHeader(heapID);
    pool->effects = FieldEffectPool_AllocEffects(heapID, count);
    pool->count = count;
    pool->heapID = heapID;
    return pool;
}

void FieldEffectPool_Free(FieldEffectPool *pool) {
    FieldEffectPool_FreeEffects(pool);
    FieldEffectPool_FreeHeader(pool);
}

void FieldEffectPool_Delete(FieldEffectPool *pool) {
    FieldEffectPool_DestroyAll(pool);
    FieldEffectPool_Free(pool);
}

// Takes a free slot, runs the template's init callback and starts the effect's task; NULL if the pool is full or
// init fails.
FieldEffect *FieldEffectPool_AddEffect(FieldEffectPool *pool, const FieldEffectTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority) {
    int i = 0;
    int count = FieldEffectPool_GetCount(pool);
    FieldEffect *effect = FieldEffectPool_GetEffects(pool);
    SysTask *task;
    do {
        if (!FieldEffect_IsActive(effect)) {
            break;
        }
        i++;
        effect++;
    } while (i < count);
    if (i >= count) {
        return NULL;
    }
    FieldEffect_SetActive(effect);
    FieldEffect_SetParam(effect, parameter);
    FieldEffect_SetUserData(effect, userData);
    FieldEffect_SetPool(effect, pool);
    if (position) {
        FieldEffect_SetPosition(effect, position);
    } else {
        VecFx32 zero = { 0, 0, 0 };
        FieldEffect_SetPosition(effect, &zero);
    }
    FieldEffect_ClearState(effect, template->size);
    FieldEffect_SetInitFunc(effect, template->init);
    FieldEffect_SetDestroyFunc(effect, template->destroy);
    FieldEffect_SetUpdateFunc(effect, template->update);
    FieldEffect_SetDrawFunc(effect, template->draw);
    FieldEffect_SetExtraFunc1(effect, template->extra1);
    FieldEffect_SetExtraFunc2(effect, template->extra2);
    task = FieldEffect_CreateTask(effect, priority);
    if (!task) {
        FieldEffect_Reset(effect);
        return NULL;
    }
    FieldEffect_SetTask(effect, task);
    if (!FieldEffect_RunInit(effect)) {
        SysTask_Destroy(task);
        FieldEffect_Reset(effect);
        return NULL;
    }
    FieldEffect_SetFlags(effect, FIELD_EFFECT_FLAG_STARTED);
    pool->activeCount++;
    return effect;
}

FieldEffect *FieldEffectPool_AddBasicEffect(FieldEffectPool *pool, const FieldEffectBasicTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority) {
    FieldEffectTemplate full;
    *(FieldEffectBasicTemplate *)&full = *template;
    full.extra1 = FieldEffect_DefaultExtra1;
    full.extra2 = FieldEffect_DefaultExtra2;
    return FieldEffectPool_AddEffect(pool, &full, position, parameter, userData, priority);
}

// value is a FieldEffect * (callers pass it as an int)
void FieldEffect_Destroy(int value) {
    FieldEffect *effect = (FieldEffect *)value;
    SysTask *task;
    GF_ASSERT(effect != NULL);
    if (FieldEffect_IsActive(effect)) {
        FieldEffect_RunDestroy(effect);
        effect->pool->activeCount--;
        task = FieldEffect_GetTask(effect);
        if (task) {
            SysTask_Destroy(task);
        }
        FieldEffect_Reset(effect);
    }
}

void FieldEffectPool_DestroyAll(FieldEffectPool *pool) {
    int count = FieldEffectPool_GetCount(pool);
    FieldEffect *effect = FieldEffectPool_GetEffects(pool);
    do {
        if (FieldEffect_IsActive(effect) == 1) {
            FieldEffect_Destroy((int)effect);
        }
        effect++;
    } while (--count);
}

void FieldEffectPool_DrawAll(FieldEffectPool *pool) {
    int count = FieldEffectPool_GetCount(pool);
    FieldEffect *effect = FieldEffectPool_GetEffects(pool);
    do {
        if (FieldEffect_TestFlags(effect, FIELD_EFFECT_FLAG_ACTIVE | FIELD_EFFECT_FLAG_STARTED) == (FIELD_EFFECT_FLAG_ACTIVE | FIELD_EFFECT_FLAG_STARTED)) {
            FieldEffect_RunDraw(effect);
        }
        effect++;
    } while (--count);
}

SysTask *FieldEffect_CreateTask(FieldEffect *effect, u32 priority) {
    SysTask *task = SysTask_CreateOnMainQueue(FieldEffect_Task, effect, priority);
    GF_ASSERT(task != NULL);
    return task;
}

void FieldEffect_Task(SysTask *task, void *data) {
    FieldEffect_RunUpdate(data);
}

FieldEffectPool *FieldEffectPool_AllocHeader(enum HeapID heapID) {
    FieldEffectPool *pool = Heap_Alloc(heapID, sizeof(*pool));
    u8 *bytes;
    u32 count;
    GF_ASSERT(pool != NULL);
    bytes = (u8 *)pool;
    count = sizeof(*pool);
    do {
        *bytes++ = 0;
    } while (--count);
    return pool;
}

void FieldEffectPool_FreeHeader(FieldEffectPool *pool) {
    Heap_FreeExplicit(pool->heapID, pool);
}

FieldEffect *FieldEffectPool_AllocEffects(enum HeapID heapID, int count) {
    FieldEffect *effects;
    count *= sizeof(FieldEffect);
    effects = Heap_Alloc(heapID, count);
    GF_ASSERT(effects != NULL);
    memset(effects, 0, count);
    return effects;
}

void FieldEffectPool_FreeEffects(FieldEffectPool *pool) {
    Heap_FreeExplicit(pool->heapID, pool->effects);
}

// Clears the slot and installs the no-op callbacks.
void FieldEffect_Reset(FieldEffect *effect) {
    memset(effect, 0, sizeof(*effect));
    FieldEffect_SetInitFunc(effect, FieldEffect_DefaultInit);
    FieldEffect_SetDestroyFunc(effect, FieldEffect_DefaultDestroy);
    FieldEffect_SetUpdateFunc(effect, FieldEffect_DefaultUpdate);
    FieldEffect_SetDrawFunc(effect, FieldEffect_DefaultDraw);
    FieldEffect_SetExtraFunc1(effect, FieldEffect_DefaultExtra1);
    FieldEffect_SetExtraFunc2(effect, FieldEffect_DefaultExtra2);
}

int FieldEffectPool_GetCount(FieldEffectPool *pool) {
    return pool->count;
}

FieldEffect *FieldEffectPool_GetEffects(FieldEffectPool *pool) {
    return pool->effects;
}

BOOL FieldEffect_IsActive(FieldEffect *effect) {
    return (effect->flags & FIELD_EFFECT_FLAG_ACTIVE) ? TRUE : FALSE;
}

void FieldEffect_SetActive(FieldEffect *effect) {
    effect->flags |= FIELD_EFFECT_FLAG_ACTIVE;
}

void FieldEffect_SetFlags(FieldEffect *effect, u32 flags) {
    effect->flags |= flags;
}

u32 FieldEffect_TestFlags(FieldEffect *effect, u32 flags) {
    return effect->flags & flags;
}

void FieldEffect_SetInitFunc(FieldEffect *effect, FieldEffectInitFunc callback) {
    effect->init = callback;
}

BOOL FieldEffect_RunInit(FieldEffect *effect) {
    void *state = FieldEffect_GetState(effect);
    return effect->init(effect, state);
}

void FieldEffect_SetUpdateFunc(FieldEffect *effect, FieldEffectFunc callback) {
    effect->update = callback;
}

void FieldEffect_RunUpdate(FieldEffect *effect) {
    void *state = FieldEffect_GetState(effect);
    effect->update(effect, state);
}

void FieldEffect_SetDrawFunc(FieldEffect *effect, FieldEffectFunc callback) {
    effect->draw = callback;
}

void FieldEffect_RunDraw(FieldEffect *effect) {
    void *state = FieldEffect_GetState(effect);
    effect->draw(effect, state);
}

void FieldEffect_SetDestroyFunc(FieldEffect *effect, FieldEffectFunc callback) {
    effect->destroy = callback;
}

void FieldEffect_RunDestroy(FieldEffect *effect) {
    void *state = FieldEffect_GetState(effect);
    effect->destroy(effect, state);
}

void FieldEffect_SetExtraFunc1(FieldEffect *effect, FieldEffectFunc callback) {
    effect->extra1 = callback;
}

void FieldEffect_SetExtraFunc2(FieldEffect *effect, FieldEffectFunc callback) {
    effect->extra2 = callback;
}

void *FieldEffect_GetState(FieldEffect *effect) {
    return effect->state;
}

void FieldEffect_ClearState(FieldEffect *effect, u32 size) {
    memset(FieldEffect_GetState(effect), 0, size);
}

void FieldEffect_SetParam(FieldEffect *effect, int parameter) {
    effect->parameter = parameter;
}

int FieldEffect_GetParam(FieldEffect *effect) {
    return effect->parameter;
}

void FieldEffect_SetUserData(FieldEffect *effect, void *userData) {
    effect->userData = userData;
}

FieldEffectUserData *FieldEffect_GetUserData(void *data) {
    return ((FieldEffect *)data)->userData;
}

void FieldEffect_SetTask(FieldEffect *effect, SysTask *task) {
    effect->task = task;
}

SysTask *FieldEffect_GetTask(FieldEffect *effect) {
    return effect->task;
}

void FieldEffect_SetPool(FieldEffect *effect, FieldEffectPool *pool) {
    effect->pool = pool;
}

void FieldEffect_SetPosition(FieldEffect *effect, const VecFx32 *position) {
    effect->position = *position;
}

void FieldEffect_GetPosition(void *data, VecFx32 *position) {
    *position = ((FieldEffect *)data)->position;
}

BOOL FieldEffect_DefaultInit(FieldEffect *effect, void *state) {
    return TRUE;
}

void FieldEffect_DefaultUpdate(FieldEffect *effect, void *state) {
}

void FieldEffect_DefaultDraw(FieldEffect *effect, void *state) {
}

void FieldEffect_DefaultDestroy(FieldEffect *effect, void *state) {
}

void FieldEffect_DefaultExtra1(FieldEffect *effect, void *state) {
}

void FieldEffect_DefaultExtra2(FieldEffect *effect, void *state) {
}
