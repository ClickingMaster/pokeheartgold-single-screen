#include "field/map_prop_render.h"

#include "global.h"

#include "filesystem.h"
#include "filesystem_files_def.h"

static BOOL Field3dObjectList_IsShared(Field3dRenderObjManager *unkC0, int index);
static Field3dRenderObj *Field3dObjectList_InitNextObject(Field3dRenderObjManager *unkC0, NNSG3dResFileHeader **resFileHeader, int index);
static void MapPropAnimation_AdvanceFrame(MapPropAnimation *animation);
static BOOL MapPropAnimation_IsOnLastFrame(MapPropAnimation *animation);
static void AreaMapAnimation_AllocAnimObj(NNSFndAllocator *pAllocator, NNSG3dAnmObj **pAlloc, void *res);
static void *AreaMapAnimation_AllocAnimObjMem(NNSFndAllocator *pAllocator, ResAnim_4004 *anim);
static void AreaMapAnimation_InitAnimObj(NNSG3dAnmObj *alloc, ResAnim_4004 *anim);

Field3dRenderObjManager *Field3dObjectList_New(enum HeapID heapID, int indexMax, int objectMax, NNSG3dResFileHeader **resFileHeaders) {
    Field3dRenderObjManager *unkC0 = Heap_Alloc(heapID, sizeof(Field3dRenderObjManager));
    unkC0->numObjects = 0;
    unkC0->indexMax = indexMax;
    unkC0->objectMax = objectMax;
    unkC0->resFileHeaders = resFileHeaders;
    unkC0->modelNumToLoadedSlot = Heap_Alloc(heapID, indexMax);
    int i;
    for (i = 0; i < indexMax; i++) {
        unkC0->modelNumToLoadedSlot[i] = 0xFF;
    }
    int size = objectMax * 0x5C;
    unkC0->objectHeap = Heap_Alloc(heapID, size);
    MIi_CpuClearFast(0, (u32 *)unkC0->objectHeap, size);
    unkC0->objects = Heap_Alloc(heapID, objectMax * 4);
    for (i = 0; i < objectMax; i++) {
        unkC0->objects[i] = &unkC0->objectHeap[i];
    }
    return unkC0;
}

void Field3dObjectList_Free(Field3dRenderObjManager *unkC0) {
    Heap_Free(unkC0->modelNumToLoadedSlot);
    Heap_Free(unkC0->objectHeap);
    Heap_Free(unkC0->objects);
    Heap_Free(unkC0);
}

Field3dRenderObj *Field3dObjectList_AddShared(Field3dRenderObjManager *unkC0, int index) {
    unkC0->modelNumToLoadedSlot[index] = unkC0->numObjects;
    NNSG3dResFileHeader *resFileHeader = unkC0->resFileHeaders[index];
    return Field3dObjectList_InitNextObject(unkC0, &resFileHeader, index);
}

static void Field3dObjectList_ReturnObject(Field3dRenderObjManager *unkC0, Field3dRenderObj *object) {
    unkC0->numObjects--;
    unkC0->objects[unkC0->numObjects] = object;
}

Field3dRenderObj *Field3dObjectList_GetRenderObjectByID(Field3dRenderObjManager *unkC0, int id) {
    u8 index = unkC0->modelNumToLoadedSlot[id];
    if (index != 0xFF) {
        return unkC0->objects[index];
    }
    return NULL;
}

static Field3dRenderObj *Field3dObjectList_AddInstance(Field3dRenderObjManager *unkC0, int index) {
    NNSG3dResFileHeader *resFileHeader = unkC0->resFileHeaders[index];
    return Field3dObjectList_InitNextObject(unkC0, &resFileHeader, index);
}

Field3dRenderObj *Field3dObjectList_GetOrAddInstance(Field3dRenderObjManager *unkC0, int index) {
    if (Field3dObjectList_IsShared(unkC0, index)) {
        return Field3dObjectList_GetRenderObjectByID(unkC0, index);
    }
    return Field3dObjectList_AddInstance(unkC0, index);
}

BOOL Field3dObjectList_ReleaseInstance(Field3dRenderObjManager *unkC0, Field3dRenderObj *object) {
    if (object == NULL) {
        return FALSE;
    }

    if (Field3dObjectList_IsShared(unkC0, object->index) == FALSE) {
        Field3dObjectList_ReturnObject(unkC0, object);
        return TRUE;
    }
    return FALSE;
}

static BOOL Field3dObjectList_IsShared(Field3dRenderObjManager *unkC0, int index) {
    return unkC0->modelNumToLoadedSlot[index] != 0xFF;
}

BOOL Field3dObjectList_HasModelFile(Field3dRenderObjManager *unkC0, int index) {
    return unkC0->resFileHeaders[index] != NULL;
}

static Field3dRenderObj *Field3dObjectList_InitNextObject(Field3dRenderObjManager *unkC0, NNSG3dResFileHeader **resFileHeader, int index) {
    unkC0->objects[unkC0->numObjects]->model = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*resFileHeader), 0);

    NNS_G3dRenderObjInit(&unkC0->objects[unkC0->numObjects]->renderObj, unkC0->objects[unkC0->numObjects]->model);
    Field3dRenderObj *object = unkC0->objects[unkC0->numObjects];
    unkC0->numObjects++;
    object->index = index;
    return object;
}

MapPropAnimationPoolList *MapPropAnimationPoolList_New(enum HeapID heapID) {
    MapPropAnimationPoolList *unkC8 = Heap_Alloc(heapID, sizeof(MapPropAnimationPoolList));
    unkC8->numPools = 0;
    unkC8->unk0 = 0;
    unkC8->head = 0;
    unkC8->tail = 0;
}

MapPropAnimationPool *MapPropAnimationPoolList_AddPool(MapPropAnimationPoolList *unkC8, enum HeapID heapID, u16 count) {
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
    if (unkC8->head == NULL) {
        unkC8->head = ret;
        unkC8->tail = ret;
        unkC8->unk0 = ret;
    } else {
        unkC8->tail->next = ret;
        unkC8->tail = ret;
    }
    unkC8->numPools++;
    return ret;
}

void MapPropAnimationPoolList_Free(MapPropAnimationPoolList *unkSubC8) {
    if (unkSubC8 != NULL) {
        MapPropAnimationPool *unk4 = unkSubC8->head;
        if (unk4 == NULL) {
            Heap_Free(unkSubC8);
        } else {
            for (int i = 0; i < unkSubC8->numPools; i++) {
                for (int j = 0; j < unk4->capacity; j++) {
                    MapPropAnimation *animation = &unk4->animations[j];
                    if (animation->inUse == 1) {
                        MapPropAnimationPool_FreeAnimObj(unk4, animation);
                        MapPropAnimationPool_Release(unk4, animation);
                    }
                }
                Heap_Free(unk4->animations);
                Heap_Free(unk4->freeList);
                MapPropAnimationPool *prev = unk4;
                unk4 = unk4->next;
                Heap_Free(prev);
            }
            Heap_Free(unkSubC8);
        }
    }
}

MapPropAnimation *MapPropAnimationPool_Alloc(MapPropAnimationPool *unkCC_Sub0) {
    u16 index = unkCC_Sub0->numUsed;
    if (unkCC_Sub0->numUsed >= unkCC_Sub0->capacity) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    unkCC_Sub0->numUsed++;
    return unkCC_Sub0->freeList[index];
}

void MapPropAnimationPool_Release(MapPropAnimationPool *unkCC_Sub0, MapPropAnimation *animation) {
    if (unkCC_Sub0->numUsed == 0) {
        GF_AssertFail();
    } else {
        unkCC_Sub0->numUsed--;
        if (animation->res != NULL) {
            Heap_Free(animation->res);
        }
        animation->animObj = NULL;
        animation->inUse = 0;
        unkCC_Sub0->freeList[unkCC_Sub0->numUsed] = animation;
    }
}

static u16 MapPropAnimationPool_GetNumUsed(MapPropAnimationPool *unkCC_Sub0) {
    return unkCC_Sub0->numUsed;
}

void MapPropAnimationPoolList_Update(MapPropAnimationPoolList *unkC8) {
    if (unkC8 != NULL) {
        MapPropAnimationPool *unkCC_Sub0 = unkC8->head;
        if (unkCC_Sub0 != NULL) {
            for (int i = 0; i < unkC8->numPools; i++) {
                for (int j = 0; j < unkCC_Sub0->capacity; j++) {
                    MapPropAnimation *animation = &unkCC_Sub0->animations[j];
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
                unkCC_Sub0 = unkCC_Sub0->next;
            }
        }
    }
}

void MapPropAnimationPoolList_PauseAll(MapPropAnimationPoolList *unkSubC8) {
    int i, j;
    if (unkSubC8 != NULL) {
        MapPropAnimationPool *unk4 = unkSubC8->head;
        if (unk4 != NULL) {
            for (i = 0; i < unkSubC8->numPools; i++) {
                for (j = 0; j < unk4->capacity; j++) {
                    MapPropAnimation *animation = &unk4->animations[j];
                    if (animation->inUse == 1 && animation->looping) {
                        animation->paused = TRUE;
                    }
                }
                unk4 = unk4->next;
            }
        }
    }
}

void MapPropAnimationPoolList_RewindAll(MapPropAnimationPoolList *unkSubC8) {
    if (unkSubC8 != NULL) {
        MapPropAnimationPool *unkCC_Sub0 = unkSubC8->head;
        if (unkCC_Sub0 != NULL) {
            for (int i = 0; i < unkSubC8->numPools; i++) {
                for (int j = 0; j < unkCC_Sub0->capacity; j++) {
                    MapPropAnimation *animation = &unkCC_Sub0->animations[j];
                    if (animation->inUse == 1 && animation->looping) {
                        MapPropAnimation_GoToFirstFrame(animation);
                    }
                }
                unkCC_Sub0 = unkCC_Sub0->next;
            }
        }
    }
}

void MapPropAnimationPool_LoadAnimObj(MapPropAnimationPool *unkCC_Sub0, MapPropAnimation *animation, void *res, NNSG3dResMdl *model, NNSG3dResTex *texture) {
    void *anim = NNS_G3dGetAnmByIdx(res, 0);
    if (anim == NULL) {
        GF_AssertFail();
    }
    NNSG3dAnmObj *animObj = NNS_G3dAllocAnmObj(&unkCC_Sub0->allocator, anim, model);
    animation->animObj = animObj;
    if (animObj == NULL) {
        GF_AssertFail();
    }
    NNS_G3dAnmObjInit(animation->animObj, anim, model, texture);
    animation->res = res;
}

static void MapPropAnimationPool_RunLoadFunc(MapPropAnimationPool *unkCC_Sub0, MapPropAnimation *animation, void *res, int arg3) {
    void (*unkFunc)() = unkCC_Sub0->loadFunc;
    if (unkFunc != NULL) {
        unkFunc(unkCC_Sub0, animation, res, arg3);
        animation->res = res;
    }
}

static void MapPropAnimationPool_SetLoadFunc(MapPropAnimationPool *unkCC_Sub0, void *loadFunc) {
    unkCC_Sub0->loadFunc = loadFunc;
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

void MapPropAnimationPool_FreeAnimObj(MapPropAnimationPool *unkCC_Sub0, MapPropAnimation *animation) {
    NNS_G3dFreeAnmObj(&unkCC_Sub0->allocator, animation->animObj);
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

AreaMapAnimation *AreaMapAnimation_New(MapPropAnimationPoolList *unkSubC8) {
    AreaMapAnimation *unkCC = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AreaMapAnimation));
    MapPropAnimationPool *unkCC_Sub0 = MapPropAnimationPoolList_AddPool(unkSubC8, HEAP_ID_FIELD1, 1);
    unkCC->pool = unkCC_Sub0;
    MapPropAnimationPool_SetLoadFunc(unkCC_Sub0, &AreaMapAnimation_AllocAnimObj);
    return unkCC;
}

void AreaMapAnimation_Free(AreaMapAnimation *unkCC) {
    Heap_Free(unkCC);
}

void AreaMapAnimation_Load(AreaMapAnimation *unkCC, int fileID) {
    MapPropAnimation *animation = MapPropAnimationPool_Alloc(unkCC->pool);
    if (animation == NULL) {
        GF_AssertFail();
    }
    MapPropAnimation_Init(animation, -1, FALSE, FALSE);
    MapPropAnimationPool_RunLoadFunc(unkCC->pool, animation, AllocAndReadWholeNarcMemberByIdPair(NARC_a_1_4_0, fileID, HEAP_ID_FIELD1), 0);
    unkCC->mapPropAnimation = animation;
}

void AreaMapAnimation_AddToRenderObj(AreaMapAnimation *unkCC, NNSG3dRenderObj *renderObj) {
    MapPropAnimation_AddToRenderObj(renderObj, unkCC->mapPropAnimation);
}

void AreaMapAnimation_RemoveFromRenderObj(AreaMapAnimation *unkCC, NNSG3dRenderObj *renderObj) {
    MapPropAnimation_RemoveFromRenderObj(renderObj, unkCC->mapPropAnimation);
}

void AreaMapAnimation_IsLoaded(AreaMapAnimation *unkCC) {
    MapPropAnimationPool_GetNumUsed(unkCC->pool);
}

static void AreaMapAnimation_AllocAnimObj(NNSFndAllocator *pAllocator, NNSG3dAnmObj **pAlloc, void *res) {
    void *anim = NNS_G3dGetAnmByIdx(res, 0);
    NNSG3dAnmObj *alloc = AreaMapAnimation_AllocAnimObjMem(pAllocator, anim);
    *pAlloc = alloc;
    AreaMapAnimation_InitAnimObj(alloc, anim);
}

static int AreaMapAnimation_GetAnimObjSize(ResAnim_4004 *anim) { // Returns some sort of size
    return ((anim->numMapData * 2) + 0x1C) & ~3;
}

static void *AreaMapAnimation_AllocAnimObjMem(NNSFndAllocator *pAllocator, ResAnim_4004 *anim) {
    return NNS_FndAllocFromAllocator(pAllocator, AreaMapAnimation_GetAnimObjSize(anim));
}

static void AreaMapAnimation_InitAnimObjMapData(NNSG3dAnmObj *alloc, ResAnim_4004 *anim) {
    alloc->funcAnm = gAnmCalcFuncNsBta; // NNS_G3dFuncAnmMatNsBtaDefault
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
    TimeOfDayPropAnimManager *unk104 = Heap_Alloc(heapID, sizeof(TimeOfDayPropAnimManager));
    MIi_CpuClearFast(0, (u32 *)unk104, sizeof(TimeOfDayPropAnimManager));
    unk104->timeOfDay = GF_RTC_GetTimeOfDay();
    return unk104;
}

void TimeOfDayPropAnimManager_Free(TimeOfDayPropAnimManager *unk104) {
    Heap_Free(unk104);
}

void TimeOfDayPropAnimManager_AddProp(TimeOfDayPropAnimManager *unk104, NNSG3dRenderObj *renderObj, MapPropAnimation **animation, int count) {
    if (unk104->numSets < 4) {
        if (unk104->sets[unk104->numSets].active) {
            GF_AssertFail();
        }
        if (count > 4) {
            GF_AssertFail();
        }
        TimeOfDayPropAnimSet *unkSub = &unk104->sets[unk104->numSets];
        unkSub->active = 1;
        unkSub->numAnims = count;
        if (count > 4) {
            unkSub->numAnims = 4;
        }
        for (u8 i = 0;  i < unkSub->numAnims; i++) {
            unkSub->animations[i] = animation[i];
        }
        unkSub->renderObj = renderObj;
        unk104->numSets++;
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

void TimeOfDayPropAnimManager_Update(TimeOfDayPropAnimManager *unk104) {
    u8 currentTimeOfDay = (u8)GF_RTC_GetTimeOfDay();
    if (unk104->timeOfDay != currentTimeOfDay) {
        u8 previousTimeOfDay = unk104->timeOfDay;
        unk104->timeOfDay = (TIMEOFDAY)currentTimeOfDay;
        for (int i = 0; i < 4; i++) {
            if (unk104->sets[i].active != 0) {
                TimeOfDayPropAnimSet *sub = &unk104->sets[i];
                MapPropAnimation_RemoveFromRenderObj(sub->renderObj, sub->animations[sTimeOfDayVisualState[previousTimeOfDay]]);
                MapPropAnimation_AddToRenderObj(sub->renderObj, sub->animations[sTimeOfDayVisualState[currentTimeOfDay]]);
            }
        }
    }
}

u8 TimeOfDayPropAnimManager_GetVisualState(TimeOfDayPropAnimManager *unk104) {
    return sTimeOfDayVisualState[unk104->timeOfDay];
}
