#include "field/map_prop_render.h"

#include "global.h"

#include "filesystem.h"
#include "filesystem_files_def.h"

static BOOL Field3dObjectList_IsShared(Field3dRenderObjManager *renderObjManager, int index);
static Field3dRenderObj *Field3dObjectList_InitNextObject(Field3dRenderObjManager *renderObjManager, NNSG3dResFileHeader **resFileHeader, int index);
static void MapPropAnimation_AdvanceFrame(MapPropAnimation *animation);
static BOOL MapPropAnimation_IsOnLastFrame(MapPropAnimation *animation);
static void AreaMapAnimation_AllocAnimObj(NNSFndAllocator *pAllocator, NNSG3dAnmObj **pAlloc, void *res);
static void *AreaMapAnimation_AllocAnimObjMem(NNSFndAllocator *pAllocator, ResAnim_4004 *anim);
static void AreaMapAnimation_InitAnimObj(NNSG3dAnmObj *alloc, ResAnim_4004 *anim);

Field3dRenderObjManager *Field3dObjectList_New(enum HeapID heapID, int indexMax, int objectMax, NNSG3dResFileHeader **resFileHeaders) {
    Field3dRenderObjManager *renderObjManager = Heap_Alloc(heapID, sizeof(Field3dRenderObjManager));
    renderObjManager->numObjects = 0;
    renderObjManager->indexMax = indexMax;
    renderObjManager->objectMax = objectMax;
    renderObjManager->resFileHeaders = resFileHeaders;
    renderObjManager->modelNumToLoadedSlot = Heap_Alloc(heapID, indexMax);
    int i;
    for (i = 0; i < indexMax; i++) {
        renderObjManager->modelNumToLoadedSlot[i] = 0xFF;
    }
    int size = objectMax * 0x5C;
    renderObjManager->objectHeap = Heap_Alloc(heapID, size);
    MIi_CpuClearFast(0, (u32 *)renderObjManager->objectHeap, size);
    renderObjManager->objects = Heap_Alloc(heapID, objectMax * 4);
    for (i = 0; i < objectMax; i++) {
        renderObjManager->objects[i] = &renderObjManager->objectHeap[i];
    }
    return renderObjManager;
}

void Field3dObjectList_Free(Field3dRenderObjManager *renderObjManager) {
    Heap_Free(renderObjManager->modelNumToLoadedSlot);
    Heap_Free(renderObjManager->objectHeap);
    Heap_Free(renderObjManager->objects);
    Heap_Free(renderObjManager);
}

Field3dRenderObj *Field3dObjectList_AddShared(Field3dRenderObjManager *renderObjManager, int index) {
    renderObjManager->modelNumToLoadedSlot[index] = renderObjManager->numObjects;
    NNSG3dResFileHeader *resFileHeader = renderObjManager->resFileHeaders[index];
    return Field3dObjectList_InitNextObject(renderObjManager, &resFileHeader, index);
}

static void Field3dObjectList_ReturnObject(Field3dRenderObjManager *renderObjManager, Field3dRenderObj *object) {
    renderObjManager->numObjects--;
    renderObjManager->objects[renderObjManager->numObjects] = object;
}

Field3dRenderObj *Field3dObjectList_GetRenderObjectByID(Field3dRenderObjManager *renderObjManager, int id) {
    u8 index = renderObjManager->modelNumToLoadedSlot[id];
    if (index != 0xFF) {
        return renderObjManager->objects[index];
    }
    return NULL;
}

static Field3dRenderObj *Field3dObjectList_AddInstance(Field3dRenderObjManager *renderObjManager, int index) {
    NNSG3dResFileHeader *resFileHeader = renderObjManager->resFileHeaders[index];
    return Field3dObjectList_InitNextObject(renderObjManager, &resFileHeader, index);
}

Field3dRenderObj *Field3dObjectList_GetOrAddInstance(Field3dRenderObjManager *renderObjManager, int index) {
    if (Field3dObjectList_IsShared(renderObjManager, index)) {
        return Field3dObjectList_GetRenderObjectByID(renderObjManager, index);
    }
    return Field3dObjectList_AddInstance(renderObjManager, index);
}

BOOL Field3dObjectList_ReleaseInstance(Field3dRenderObjManager *renderObjManager, Field3dRenderObj *object) {
    if (object == NULL) {
        return FALSE;
    }

    if (Field3dObjectList_IsShared(renderObjManager, object->index) == FALSE) {
        Field3dObjectList_ReturnObject(renderObjManager, object);
        return TRUE;
    }
    return FALSE;
}

static BOOL Field3dObjectList_IsShared(Field3dRenderObjManager *renderObjManager, int index) {
    return renderObjManager->modelNumToLoadedSlot[index] != 0xFF;
}

BOOL Field3dObjectList_HasModelFile(Field3dRenderObjManager *renderObjManager, int index) {
    return renderObjManager->resFileHeaders[index] != NULL;
}

static Field3dRenderObj *Field3dObjectList_InitNextObject(Field3dRenderObjManager *renderObjManager, NNSG3dResFileHeader **resFileHeader, int index) {
    renderObjManager->objects[renderObjManager->numObjects]->model = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*resFileHeader), 0);

    NNS_G3dRenderObjInit(&renderObjManager->objects[renderObjManager->numObjects]->renderObj, renderObjManager->objects[renderObjManager->numObjects]->model);
    Field3dRenderObj *object = renderObjManager->objects[renderObjManager->numObjects];
    renderObjManager->numObjects++;
    object->index = index;
    return object;
}

MapPropAnimationPoolList *MapPropAnimationPoolList_New(enum HeapID heapID) {
    MapPropAnimationPoolList *poolList = Heap_Alloc(heapID, sizeof(MapPropAnimationPoolList));
    poolList->numPools = 0;
    poolList->unk0 = 0;
    poolList->head = 0;
    poolList->tail = 0;
}

MapPropAnimationPool *MapPropAnimationPoolList_AddPool(MapPropAnimationPoolList *poolList, enum HeapID heapID, u16 count) {
    MapPropAnimationPool *ret = Heap_Alloc(heapID, sizeof(MapPropAnimationPool));
    ret->numUsed = 0;
    ret->capacity = count;
    ret->loadFunc = NULL;
    HeapExp_FndInitAllocator(&ret->allocator, heapID, 4);
    ret->animations = Heap_Alloc(heapID, count << 5);
    MIi_CpuClearFast(0, (u32 *)(ret->animations), count << 5);
    ret->freeList = Heap_Alloc(heapID, count << 2);

    for (int i = 0; i < count; i++) {
        ret->animations[i].inUse = 0;
        ret->animations[i].res = NULL;
        ret->animations[i].animObj = NULL;
        ret->freeList[i] = &ret->animations[i];
    }
    ret->next = NULL;
    if (poolList->head == NULL) {
        poolList->head = ret;
        poolList->tail = ret;
        poolList->unk0 = ret;
    } else {
        poolList->tail->next = ret;
        poolList->tail = ret;
    }
    poolList->numPools++;
    return ret;
}

void MapPropAnimationPoolList_Free(MapPropAnimationPoolList *poolList) {
    if (poolList != NULL) {
        MapPropAnimationPool *pool = poolList->head;
        if (pool == NULL) {
            Heap_Free(poolList);
        } else {
            for (int i = 0; i < poolList->numPools; i++) {
                for (int j = 0; j < pool->capacity; j++) {
                    MapPropAnimation *animation = &pool->animations[j];
                    if (animation->inUse == 1) {
                        MapPropAnimationPool_FreeAnimObj(pool, animation);
                        MapPropAnimationPool_Release(pool, animation);
                    }
                }
                Heap_Free(pool->animations);
                Heap_Free(pool->freeList);
                MapPropAnimationPool *prev = pool;
                pool = pool->next;
                Heap_Free(prev);
            }
            Heap_Free(poolList);
        }
    }
}

MapPropAnimation *MapPropAnimationPool_Alloc(MapPropAnimationPool *pool) {
    u16 index = pool->numUsed;
    if (pool->numUsed >= pool->capacity) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    pool->numUsed++;
    return pool->freeList[index];
}

void MapPropAnimationPool_Release(MapPropAnimationPool *pool, MapPropAnimation *animation) {
    if (pool->numUsed == 0) {
        GF_AssertFail();
    } else {
        pool->numUsed--;
        if (animation->res != NULL) {
            Heap_Free(animation->res);
        }
        animation->animObj = NULL;
        animation->inUse = 0;
        pool->freeList[pool->numUsed] = animation;
    }
}

static u16 MapPropAnimationPool_GetNumUsed(MapPropAnimationPool *pool) {
    return pool->numUsed;
}

void MapPropAnimationPoolList_Update(MapPropAnimationPoolList *poolList) {
    if (poolList != NULL) {
        MapPropAnimationPool *pool = poolList->head;
        if (pool != NULL) {
            for (int i = 0; i < poolList->numPools; i++) {
                for (int j = 0; j < pool->capacity; j++) {
                    MapPropAnimation *animation = &pool->animations[j];
                    if (animation->inUse == 1 && animation->paused != TRUE && animation->looping) {
                        MapPropAnimation_AdvanceFrame(animation);
                        if (animation->loopCount != -1 && MapPropAnimation_IsOnLastFrame(animation)) {
                            if (animation->loopsDone + 1 >= animation->loopCount) {
                                animation->looping = FALSE;
                            } else {
                                animation->loopsDone++;
                            }
                        }
                    }
                }
                pool = pool->next;
            }
        }
    }
}

void MapPropAnimationPoolList_PauseAll(MapPropAnimationPoolList *poolList) {
    int i, j;
    if (poolList != NULL) {
        MapPropAnimationPool *pool = poolList->head;
        if (pool != NULL) {
            for (i = 0; i < poolList->numPools; i++) {
                for (j = 0; j < pool->capacity; j++) {
                    MapPropAnimation *animation = &pool->animations[j];
                    if (animation->inUse == 1 && animation->looping) {
                        animation->paused = TRUE;
                    }
                }
                pool = pool->next;
            }
        }
    }
}

void MapPropAnimationPoolList_RewindAll(MapPropAnimationPoolList *poolList) {
    if (poolList != NULL) {
        MapPropAnimationPool *pool = poolList->head;
        if (pool != NULL) {
            for (int i = 0; i < poolList->numPools; i++) {
                for (int j = 0; j < pool->capacity; j++) {
                    MapPropAnimation *animation = &pool->animations[j];
                    if (animation->inUse == 1 && animation->looping) {
                        MapPropAnimation_GoToFirstFrame(animation);
                    }
                }
                pool = pool->next;
            }
        }
    }
}

void MapPropAnimationPool_LoadAnimObj(MapPropAnimationPool *pool, MapPropAnimation *animation, void *res, NNSG3dResMdl *model, NNSG3dResTex *texture) {
    void *anim = NNS_G3dGetAnmByIdx(res, 0);
    if (anim == NULL) {
        GF_AssertFail();
    }
    NNSG3dAnmObj *animObj = NNS_G3dAllocAnmObj(&pool->allocator, anim, model);
    animation->animObj = animObj;
    if (animObj == NULL) {
        GF_AssertFail();
    }
    NNS_G3dAnmObjInit(animation->animObj, anim, model, texture);
    animation->res = res;
}

static void MapPropAnimationPool_RunLoadFunc(MapPropAnimationPool *pool, MapPropAnimation *animation, void *res, int arg3) {
    void (*loadFunc)() = pool->loadFunc;
    if (loadFunc != NULL) {
        loadFunc(pool, animation, res, arg3);
        animation->res = res;
    }
}

static void MapPropAnimationPool_SetLoadFunc(MapPropAnimationPool *pool, void *loadFunc) {
    pool->loadFunc = loadFunc;
}

void MapPropAnimation_Init(MapPropAnimation *animation, int loopCount, BOOL paused, BOOL reversed) {
    animation->loopsDone = 0;
    animation->looping = TRUE;
    animation->inUse = 1;
    animation->loopCount = loopCount;
    animation->paused = paused;
    animation->reversed = reversed;
}

void MapPropAnimation_GoToFirstFrame(MapPropAnimation *animation) {
    if (!animation->reversed) {
        animation->animObj->frame = 0;
    } else {
        animation->animObj->frame = NNS_G3dAnmObjGetNumFrame(animation->animObj) - FX32_ONE;
    }
}

void MapPropAnimationPool_FreeAnimObj(MapPropAnimationPool *pool, MapPropAnimation *animation) {
    NNS_G3dFreeAnmObj(&pool->allocator, animation->animObj);
}

void MapPropAnimation_AddToRenderObj(NNSG3dRenderObj *renderObj, MapPropAnimation *animation) {
    NNS_G3dRenderObjAddAnmObj(renderObj, animation->animObj);
}

BOOL MapPropAnimation_RemoveFromRenderObj(NNSG3dRenderObj *renderObj, MapPropAnimation *animation) {
    if (animation == NULL) {
        return FALSE;
    }
    if (animation->animObj == NULL) {
        return FALSE;
    }
    if (renderObj == NULL) {
        return FALSE;
    }
    if (renderObj->anmMat == NULL && renderObj->anmJnt == NULL && renderObj->anmVis == NULL) {
        return FALSE;
    }

    NNS_G3dRenderObjRemoveAnmObj(renderObj, animation->animObj);
    return TRUE;
}

u16 MapPropAnimation_GetLoopCount(MapPropAnimation *animation) {
    return animation->loopCount;
}

void MapPropAnimation_SetPaused(MapPropAnimation *animation, BOOL paused) {
    animation->paused = paused;
}

BOOL MapPropAnimation_IsLoopFinished(MapPropAnimation *animation) {
    return animation->looping == FALSE;
}

void MapPropAnimation_GoToLastFrame(MapPropAnimation *animation) {
    if (!animation->reversed) {
        animation->animObj->frame = NNS_G3dAnmObjGetNumFrame(animation->animObj) - FX32_ONE;
    } else {
        animation->animObj->frame = 0;
    }
}

void MapPropAnimation_SetLoopCount(MapPropAnimation *animation, const int loopCount) {
    animation->loopCount = loopCount;
}

static void MapPropAnimation_AdvanceFrame(MapPropAnimation *animation) {
    if (!animation->reversed) {
        animation->animObj->frame += FX32_ONE;
        if (animation->animObj->frame == NNS_G3dAnmObjGetNumFrame(animation->animObj)) {
            animation->animObj->frame = 0;
        }
    } else {
        if (animation->animObj->frame <= 0) {
            animation->animObj->frame = NNS_G3dAnmObjGetNumFrame(animation->animObj) - FX32_ONE;
        } else {
            animation->animObj->frame -= FX32_ONE;
        }
    }
}

static BOOL MapPropAnimation_IsOnLastFrame(MapPropAnimation *animation) {
    if (!animation->reversed) {
        return animation->animObj->frame >= NNS_G3dAnmObjGetNumFrame(animation->animObj) - FX32_ONE;
    }
    return animation->animObj->frame == 0;
}

AreaMapAnimation *AreaMapAnimation_New(MapPropAnimationPoolList *poolList) {
    AreaMapAnimation *areaMapAnimation = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AreaMapAnimation));
    MapPropAnimationPool *pool = MapPropAnimationPoolList_AddPool(poolList, HEAP_ID_FIELD1, 1);
    areaMapAnimation->pool = pool;
    MapPropAnimationPool_SetLoadFunc(pool, &AreaMapAnimation_AllocAnimObj);
    return areaMapAnimation;
}

void AreaMapAnimation_Free(AreaMapAnimation *areaMapAnimation) {
    Heap_Free(areaMapAnimation);
}

void AreaMapAnimation_Load(AreaMapAnimation *areaMapAnimation, int fileID) {
    MapPropAnimation *animation = MapPropAnimationPool_Alloc(areaMapAnimation->pool);
    if (animation == NULL) {
        GF_AssertFail();
    }
    MapPropAnimation_Init(animation, -1, FALSE, FALSE);
    MapPropAnimationPool_RunLoadFunc(areaMapAnimation->pool, animation, AllocAndReadWholeNarcMemberByIdPair(NARC_a_1_4_0, fileID, HEAP_ID_FIELD1), 0);
    areaMapAnimation->mapPropAnimation = animation;
}

void AreaMapAnimation_AddToRenderObj(AreaMapAnimation *areaMapAnimation, NNSG3dRenderObj *renderObj) {
    MapPropAnimation_AddToRenderObj(renderObj, areaMapAnimation->mapPropAnimation);
}

void AreaMapAnimation_RemoveFromRenderObj(AreaMapAnimation *areaMapAnimation, NNSG3dRenderObj *renderObj) {
    MapPropAnimation_RemoveFromRenderObj(renderObj, areaMapAnimation->mapPropAnimation);
}

BOOL AreaMapAnimation_IsLoaded(AreaMapAnimation *areaMapAnimation) {
    return MapPropAnimationPool_GetNumUsed(areaMapAnimation->pool);
}

static void AreaMapAnimation_AllocAnimObj(NNSFndAllocator *pAllocator, NNSG3dAnmObj **pAlloc, void *res) {
    void *anim = NNS_G3dGetAnmByIdx(res, 0);
    NNSG3dAnmObj *alloc = AreaMapAnimation_AllocAnimObjMem(pAllocator, anim);
    *pAlloc = alloc;
    AreaMapAnimation_InitAnimObj(alloc, anim);
}

static int AreaMapAnimation_GetAnimObjSize(ResAnim_4004 *anim) {
    return ((anim->numMapData * 2) + 0x1C) & ~3;
}

static void *AreaMapAnimation_AllocAnimObjMem(NNSFndAllocator *pAllocator, ResAnim_4004 *anim) {
    return NNS_FndAllocFromAllocator(pAllocator, AreaMapAnimation_GetAnimObjSize(anim));
}

static void AreaMapAnimation_InitAnimObjMapData(NNSG3dAnmObj *alloc, ResAnim_4004 *anim) {
    alloc->funcAnm = gAnmCalcFuncNsBta;
    u8 numMapData = anim->numMapData;
    alloc->numMapData = numMapData;
    MIi_CpuClear16(0, alloc->mapData, numMapData * 2);
    for (u32 i = 0; i < anim->numMapData; i++) {
        alloc->mapData[i] = i | 0x100;
    }
}

static void AreaMapAnimation_InitAnimObj(NNSG3dAnmObj *alloc, ResAnim_4004 *anim) {
    alloc->frame = 0;
    alloc->resAnm = anim;
    alloc->next = NULL;
    alloc->priority = 0x7F;
    alloc->ratio = 0x1000;
    alloc->resTex = NULL;
    AreaMapAnimation_InitAnimObjMapData(alloc, anim);
}

TimeOfDayPropAnimManager *TimeOfDayPropAnimManager_New(enum HeapID heapID) {
    TimeOfDayPropAnimManager *manager = Heap_Alloc(heapID, sizeof(TimeOfDayPropAnimManager));
    MIi_CpuClearFast(0, (u32 *)manager, sizeof(TimeOfDayPropAnimManager));
    manager->timeOfDay = GF_RTC_GetTimeOfDay();
    return manager;
}

void TimeOfDayPropAnimManager_Free(TimeOfDayPropAnimManager *manager) {
    Heap_Free(manager);
}

void TimeOfDayPropAnimManager_AddProp(TimeOfDayPropAnimManager *manager, NNSG3dRenderObj *renderObj, MapPropAnimation **animation, int count) {
    if (manager->numSets < 4) {
        if (manager->sets[manager->numSets].active) {
            GF_AssertFail();
        }
        if (count > 4) {
            GF_AssertFail();
        }
        TimeOfDayPropAnimSet *set = &manager->sets[manager->numSets];
        set->active = 1;
        set->numAnims = count;
        if (count > 4) {
            set->numAnims = 4;
        }
        for (u8 i = 0; i < set->numAnims; i++) {
            set->animations[i] = animation[i];
        }
        set->renderObj = renderObj;
        manager->numSets++;
    } else {
        GF_AssertFail();
    }
}

static const u8 sTimeOfDayVisualState[RTC_TIMEOFDAY_COUNT] = {
    [RTC_TIMEOFDAY_MORN] = 0,
    [RTC_TIMEOFDAY_DAY] = 1,
    [RTC_TIMEOFDAY_EVE] = 2,
    [RTC_TIMEOFDAY_NITE] = 3,
    [RTC_TIMEOFDAY_LATE] = 3
};

void TimeOfDayPropAnimManager_Update(TimeOfDayPropAnimManager *manager) {
    u8 currentTimeOfDay = (u8)GF_RTC_GetTimeOfDay();
    if (manager->timeOfDay != currentTimeOfDay) {
        u8 previousTimeOfDay = manager->timeOfDay;
        manager->timeOfDay = (TIMEOFDAY)currentTimeOfDay;
        for (int i = 0; i < 4; i++) {
            if (manager->sets[i].active != 0) {
                TimeOfDayPropAnimSet *sub = &manager->sets[i];
                MapPropAnimation_RemoveFromRenderObj(sub->renderObj, sub->animations[sTimeOfDayVisualState[previousTimeOfDay]]);
                MapPropAnimation_AddToRenderObj(sub->renderObj, sub->animations[sTimeOfDayVisualState[currentTimeOfDay]]);
            }
        }
    }
}

u8 TimeOfDayPropAnimManager_GetVisualState(TimeOfDayPropAnimManager *manager) {
    return sTimeOfDayVisualState[manager->timeOfDay];
}
