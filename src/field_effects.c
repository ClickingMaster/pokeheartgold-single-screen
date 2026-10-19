#include "field_effects.h"

#include <string.h>

#include "global.h"

#include "field/field_effect_manager.h"

#include "heap.h"
#include "sys_task_api.h"

// A fixed-size pool of field effects. Each active effect runs as a main-queue SysTask that calls its update
// callback every frame; FieldEffectPool_DrawAll calls the draw callback of every started effect.

typedef struct FieldEffectPool {
    int count;
    int activeCount;
    enum HeapID heapID;
    FieldEffect *effects;
} FieldEffectPool;

#define FIELD_EFFECT_FLAG_ACTIVE  (1 << 0)
#define FIELD_EFFECT_FLAG_STARTED (1 << 1) // the init callback succeeded

struct FieldEffect {
    u32 flags;
    int parameter;
    u32 unk_08;
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

// A FieldEffectBasicTemplate with the two extra callbacks
typedef struct FieldEffectTemplate {
    FieldEffectBasicTemplate basic;
    FieldEffectFunc extra1;
    FieldEffectFunc extra2;
} FieldEffectTemplate;

FieldEffectPool *FieldEffectPool_New(enum HeapID heapID, int count);
static void FieldEffectPool_Free(FieldEffectPool *pool);
void FieldEffectPool_Delete(FieldEffectPool *pool);
static FieldEffect *FieldEffectPool_AddEffect(FieldEffectPool *pool, const FieldEffectTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority);
FieldEffect *FieldEffectPool_AddBasicEffect(FieldEffectPool *pool, const FieldEffectBasicTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority);
void FieldEffect_Destroy(int value);
static void FieldEffectPool_DestroyAll(FieldEffectPool *pool);
void FieldEffectPool_DrawAll(FieldEffectPool *pool);
static SysTask *FieldEffect_CreateTask(FieldEffect *effect, u32 priority);
static void FieldEffect_Task(SysTask *task, void *data);
static FieldEffectPool *FieldEffectPool_AllocHeader(enum HeapID heapID);
static void FieldEffectPool_FreeHeader(FieldEffectPool *pool);
static FieldEffect *FieldEffectPool_AllocEffects(enum HeapID heapID, int count);
static void FieldEffectPool_FreeEffects(FieldEffectPool *pool);
static void FieldEffect_Reset(FieldEffect *effect);
static int FieldEffectPool_GetCount(FieldEffectPool *pool);
static FieldEffect *FieldEffectPool_GetEffects(FieldEffectPool *pool);
BOOL FieldEffect_IsActive(FieldEffect *effect);
static void FieldEffect_SetActive(FieldEffect *effect);
static void FieldEffect_SetFlags(FieldEffect *effect, u32 flags);
static u32 FieldEffect_TestFlags(FieldEffect *effect, u32 flags);
static void FieldEffect_SetInitFunc(FieldEffect *effect, FieldEffectInitFunc callback);
static BOOL FieldEffect_RunInit(FieldEffect *effect);
static void FieldEffect_SetUpdateFunc(FieldEffect *effect, FieldEffectFunc callback);
void FieldEffect_RunUpdate(FieldEffect *effect);
static void FieldEffect_SetDrawFunc(FieldEffect *effect, FieldEffectFunc callback);
static void FieldEffect_RunDraw(FieldEffect *effect);
static void FieldEffect_SetDestroyFunc(FieldEffect *effect, FieldEffectFunc callback);
static void FieldEffect_RunDestroy(FieldEffect *effect);
static void FieldEffect_SetExtraFunc1(FieldEffect *effect, FieldEffectFunc callback);
static void FieldEffect_SetExtraFunc2(FieldEffect *effect, FieldEffectFunc callback);
void *FieldEffect_GetState(FieldEffect *effect);
static void FieldEffect_ClearState(FieldEffect *effect, u32 size);
static void FieldEffect_SetParam(FieldEffect *effect, int parameter);
int FieldEffect_GetParam(FieldEffect *effect);
static void FieldEffect_SetUserData(FieldEffect *effect, void *userData);
FieldEffectUserData *FieldEffect_GetUserData(void *data);
static void FieldEffect_SetTask(FieldEffect *effect, SysTask *task);
static SysTask *FieldEffect_GetTask(FieldEffect *effect);
static void FieldEffect_SetPool(FieldEffect *effect, FieldEffectPool *pool);
void FieldEffect_SetPosition(FieldEffect *effect, const VecFx32 *position);
void FieldEffect_GetPosition(void *data, VecFx32 *position);
static BOOL FieldEffect_DefaultInit(FieldEffect *effect, void *state);
static void FieldEffect_DefaultUpdate(FieldEffect *effect, void *state);
void FieldEffect_DefaultDraw(FieldEffect *effect, void *state);
void FieldEffect_DefaultDestroy(FieldEffect *effect, void *state);
static void FieldEffect_DefaultExtra1(FieldEffect *effect, void *state);
static void FieldEffect_DefaultExtra2(FieldEffect *effect, void *state);

FieldEffectPool *FieldEffectPool_New(enum HeapID heapID, int count) {
    FieldEffectPool *pool = FieldEffectPool_AllocHeader(heapID);
    pool->effects = FieldEffectPool_AllocEffects(heapID, count);
    pool->count = count;
    pool->heapID = heapID;
    return pool;
}

static void FieldEffectPool_Free(FieldEffectPool *pool) {
    FieldEffectPool_FreeEffects(pool);
    FieldEffectPool_FreeHeader(pool);
}

void FieldEffectPool_Delete(FieldEffectPool *pool) {
    FieldEffectPool_DestroyAll(pool);
    FieldEffectPool_Free(pool);
}

// Takes a free slot, runs the template's init callback and starts the effect's task; NULL if the pool is full or
// init fails.
static FieldEffect *FieldEffectPool_AddEffect(FieldEffectPool *pool, const FieldEffectTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority) {
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
    FieldEffect_ClearState(effect, template->basic.stateSize);
    FieldEffect_SetInitFunc(effect, template->basic.init);
    FieldEffect_SetDestroyFunc(effect, template->basic.destroy);
    FieldEffect_SetUpdateFunc(effect, template->basic.update);
    FieldEffect_SetDrawFunc(effect, template->basic.draw);
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
    full.basic = *template;
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

static void FieldEffectPool_DestroyAll(FieldEffectPool *pool) {
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

static SysTask *FieldEffect_CreateTask(FieldEffect *effect, u32 priority) {
    SysTask *task = SysTask_CreateOnMainQueue(FieldEffect_Task, effect, priority);
    GF_ASSERT(task != NULL);
    return task;
}

static void FieldEffect_Task(SysTask *task, void *data) {
    FieldEffect_RunUpdate(data);
}

static FieldEffectPool *FieldEffectPool_AllocHeader(enum HeapID heapID) {
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

static void FieldEffectPool_FreeHeader(FieldEffectPool *pool) {
    Heap_FreeExplicit(pool->heapID, pool);
}

static FieldEffect *FieldEffectPool_AllocEffects(enum HeapID heapID, int count) {
    FieldEffect *effects;
    count *= sizeof(FieldEffect);
    effects = Heap_Alloc(heapID, count);
    GF_ASSERT(effects != NULL);
    memset(effects, 0, count);
    return effects;
}

static void FieldEffectPool_FreeEffects(FieldEffectPool *pool) {
    Heap_FreeExplicit(pool->heapID, pool->effects);
}

// Clears the slot and installs the no-op callbacks.
static void FieldEffect_Reset(FieldEffect *effect) {
    memset(effect, 0, sizeof(*effect));
    FieldEffect_SetInitFunc(effect, FieldEffect_DefaultInit);
    FieldEffect_SetDestroyFunc(effect, FieldEffect_DefaultDestroy);
    FieldEffect_SetUpdateFunc(effect, FieldEffect_DefaultUpdate);
    FieldEffect_SetDrawFunc(effect, FieldEffect_DefaultDraw);
    FieldEffect_SetExtraFunc1(effect, FieldEffect_DefaultExtra1);
    FieldEffect_SetExtraFunc2(effect, FieldEffect_DefaultExtra2);
}

static int FieldEffectPool_GetCount(FieldEffectPool *pool) {
    return pool->count;
}

static FieldEffect *FieldEffectPool_GetEffects(FieldEffectPool *pool) {
    return pool->effects;
}

BOOL FieldEffect_IsActive(FieldEffect *effect) {
    return (effect->flags & FIELD_EFFECT_FLAG_ACTIVE) ? TRUE : FALSE;
}

static void FieldEffect_SetActive(FieldEffect *effect) {
    effect->flags |= FIELD_EFFECT_FLAG_ACTIVE;
}

static void FieldEffect_SetFlags(FieldEffect *effect, u32 flags) {
    effect->flags |= flags;
}

static u32 FieldEffect_TestFlags(FieldEffect *effect, u32 flags) {
    return effect->flags & flags;
}

static void FieldEffect_SetInitFunc(FieldEffect *effect, FieldEffectInitFunc callback) {
    effect->init = callback;
}

static BOOL FieldEffect_RunInit(FieldEffect *effect) {
    void *state = FieldEffect_GetState(effect);
    return effect->init(effect, state);
}

static void FieldEffect_SetUpdateFunc(FieldEffect *effect, FieldEffectFunc callback) {
    effect->update = callback;
}

void FieldEffect_RunUpdate(FieldEffect *effect) {
    void *state = FieldEffect_GetState(effect);
    effect->update(effect, state);
}

static void FieldEffect_SetDrawFunc(FieldEffect *effect, FieldEffectFunc callback) {
    effect->draw = callback;
}

static void FieldEffect_RunDraw(FieldEffect *effect) {
    void *state = FieldEffect_GetState(effect);
    effect->draw(effect, state);
}

static void FieldEffect_SetDestroyFunc(FieldEffect *effect, FieldEffectFunc callback) {
    effect->destroy = callback;
}

static void FieldEffect_RunDestroy(FieldEffect *effect) {
    void *state = FieldEffect_GetState(effect);
    effect->destroy(effect, state);
}

static void FieldEffect_SetExtraFunc1(FieldEffect *effect, FieldEffectFunc callback) {
    effect->extra1 = callback;
}

static void FieldEffect_SetExtraFunc2(FieldEffect *effect, FieldEffectFunc callback) {
    effect->extra2 = callback;
}

void *FieldEffect_GetState(FieldEffect *effect) {
    return effect->state;
}

static void FieldEffect_ClearState(FieldEffect *effect, u32 size) {
    memset(FieldEffect_GetState(effect), 0, size);
}

static void FieldEffect_SetParam(FieldEffect *effect, int parameter) {
    effect->parameter = parameter;
}

int FieldEffect_GetParam(FieldEffect *effect) {
    return effect->parameter;
}

static void FieldEffect_SetUserData(FieldEffect *effect, void *userData) {
    effect->userData = userData;
}

FieldEffectUserData *FieldEffect_GetUserData(void *data) {
    return ((FieldEffect *)data)->userData;
}

static void FieldEffect_SetTask(FieldEffect *effect, SysTask *task) {
    effect->task = task;
}

static SysTask *FieldEffect_GetTask(FieldEffect *effect) {
    return effect->task;
}

static void FieldEffect_SetPool(FieldEffect *effect, FieldEffectPool *pool) {
    effect->pool = pool;
}

void FieldEffect_SetPosition(FieldEffect *effect, const VecFx32 *position) {
    effect->position = *position;
}

void FieldEffect_GetPosition(void *data, VecFx32 *position) {
    *position = ((FieldEffect *)data)->position;
}

static BOOL FieldEffect_DefaultInit(FieldEffect *effect, void *state) {
    return TRUE;
}

static void FieldEffect_DefaultUpdate(FieldEffect *effect, void *state) {
}

void FieldEffect_DefaultDraw(FieldEffect *effect, void *state) {
}

void FieldEffect_DefaultDestroy(FieldEffect *effect, void *state) {
}

static void FieldEffect_DefaultExtra1(FieldEffect *effect, void *state) {
}

static void FieldEffect_DefaultExtra2(FieldEffect *effect, void *state) {
}
