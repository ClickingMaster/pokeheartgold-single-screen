#ifndef POKEHEARTGOLD_FIELD_MAP_PROP_RENDER_H
#define POKEHEARTGOLD_FIELD_MAP_PROP_RENDER_H

#include <nnsys.h>

#include "gf_rtc.h"
#include "heap.h"

// Todo: move back probably
typedef struct MapPropAnimation {
    NNSG3dAnmObj *animObj;
    void *res;
    int loopCount;
    BOOL paused;
    BOOL reversed;
    u32 inUse;
    int loopsDone;
    BOOL looping;
} MapPropAnimation;

typedef struct MapPropAnimationPool MapPropAnimationPool;

typedef struct AreaMapAnimation {
    MapPropAnimationPool *pool;
    MapPropAnimation *mapPropAnimation;
} AreaMapAnimation;

struct MapPropAnimationPool {
    NNSFndAllocator allocator;
    MapPropAnimation *animations;
    MapPropAnimation **freeList;
    MapPropAnimationPool *next;
    void *loadFunc;
    u16 capacity;
    u16 numUsed;
};

typedef struct MapPropAnimationPoolList {
    MapPropAnimationPool *unk0;
    MapPropAnimationPool *head;
    MapPropAnimationPool *tail;
    int numPools;
} MapPropAnimationPoolList;

typedef struct Field3dRenderObj {
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *model;
    int index;
} Field3dRenderObj;

typedef struct Field3dRenderObjManager {
    u8 *modelNumToLoadedSlot;
    NNSG3dResFileHeader **resFileHeaders;
    Field3dRenderObj *objectHeap;
    Field3dRenderObj **objects;
    int numObjects;
    int indexMax;
    int objectMax;
} Field3dRenderObjManager;

// TimeOfDayVisual?
typedef struct TimeOfDayPropAnimSet {
    u32 active;
    int numAnims;
    MapPropAnimation *animations[4];
    NNSG3dRenderObj *renderObj;
} TimeOfDayPropAnimSet; // Size: 0x1C

// TimeOfDayVisualManager?
typedef struct TimeOfDayPropAnimManager {
    TIMEOFDAY timeOfDay;
    int numSets;
    TimeOfDayPropAnimSet sets[4];
} TimeOfDayPropAnimManager; // Size: 0x78

typedef struct ResAnim_4004 {
    u32 unk0;
    u32 unk4;
    u8 unk8;
    u8 numMapData;
} ResAnim_4004;

Field3dRenderObjManager *Field3dObjectList_New(enum HeapID heapID, int indexMax, int objectMax, NNSG3dResFileHeader **fileResHeader);
void Field3dObjectList_Free(Field3dRenderObjManager *renderObjManager);
Field3dRenderObj *Field3dObjectList_AddShared(Field3dRenderObjManager *renderObjManager, int index);
Field3dRenderObj *Field3dObjectList_GetRenderObjectByID(Field3dRenderObjManager *renderObjManager, int index);
Field3dRenderObj *Field3dObjectList_GetOrAddInstance(Field3dRenderObjManager *renderObjManager, int index);
BOOL Field3dObjectList_ReleaseInstance(Field3dRenderObjManager *renderObjManager, Field3dRenderObj *object);
BOOL Field3dObjectList_HasModelFile(Field3dRenderObjManager *renderObjManager, int index);
MapPropAnimationPoolList *MapPropAnimationPoolList_New(enum HeapID heapID);
MapPropAnimationPool *MapPropAnimationPoolList_AddPool(MapPropAnimationPoolList *poolList, enum HeapID heapID, u16 count);
void MapPropAnimationPoolList_Free(MapPropAnimationPoolList *poolList);
MapPropAnimation *MapPropAnimationPool_Alloc(MapPropAnimationPool *pool);
void MapPropAnimationPool_Release(MapPropAnimationPool *pool, MapPropAnimation *animation);
void MapPropAnimationPoolList_Update(MapPropAnimationPoolList *unkSubC8);
void MapPropAnimationPoolList_PauseAll(MapPropAnimationPoolList *poolList);
void MapPropAnimationPoolList_RewindAll(MapPropAnimationPoolList *poolList);
void MapPropAnimationPool_LoadAnimObj(MapPropAnimationPool *pool, MapPropAnimation *animation, void *res, NNSG3dResMdl *model, NNSG3dResTex *texture);
void MapPropAnimation_Init(MapPropAnimation *animation, int loopCount, BOOL paused, BOOL reversed);
void MapPropAnimation_GoToFirstFrame(MapPropAnimation *animation);
void MapPropAnimationPool_FreeAnimObj(MapPropAnimationPool *pool, MapPropAnimation *animation);
void MapPropAnimation_AddToRenderObj(NNSG3dRenderObj *renderObj, MapPropAnimation *animation);
BOOL MapPropAnimation_RemoveFromRenderObj(NNSG3dRenderObj *renderObj, MapPropAnimation *animation);
u16 MapPropAnimation_GetLoopCount(MapPropAnimation *animation);
void MapPropAnimation_SetPaused(MapPropAnimation *animation, BOOL paused);
BOOL MapPropAnimation_IsLoopFinished(MapPropAnimation *animation);
void MapPropAnimation_GoToLastFrame(MapPropAnimation *animation);
void MapPropAnimation_SetLoopCount(MapPropAnimation *animation, const int loopCount);
AreaMapAnimation *AreaMapAnimation_New(MapPropAnimationPoolList *poolList);
void AreaMapAnimation_Free(AreaMapAnimation *areaMapAnimation);
void AreaMapAnimation_Load(AreaMapAnimation *areaMapAnimation, int fileID);
void AreaMapAnimation_AddToRenderObj(AreaMapAnimation *areaMapAnimation, NNSG3dRenderObj *renderObj);
void AreaMapAnimation_RemoveFromRenderObj(AreaMapAnimation *areaMapAnimation, NNSG3dRenderObj *renderObj);
BOOL AreaMapAnimation_IsLoaded(AreaMapAnimation *areaMapAnimation);
TimeOfDayPropAnimManager *TimeOfDayPropAnimManager_New(enum HeapID heapID);
void TimeOfDayPropAnimManager_Free(TimeOfDayPropAnimManager *manager);
void TimeOfDayPropAnimManager_AddProp(TimeOfDayPropAnimManager *manager, NNSG3dRenderObj *renderObj, MapPropAnimation **animation, int count);
void TimeOfDayPropAnimManager_Update(TimeOfDayPropAnimManager *manager);
u8 TimeOfDayPropAnimManager_GetVisualState(TimeOfDayPropAnimManager *manager);

// These are nnsys things that do not seem to be in the project right now or are not decompiled in this project.
void *NNS_FndAllocFromAllocator(NNSFndAllocator *pAllocator, u32 size);

typedef struct NNSG3dMatAnmResult NNSG3dMatAnmResult;
typedef void (*NNSG3dFuncAnmMat)(NNSG3dMatAnmResult *, const NNSG3dAnmObj *, u32);
// void NNSi_G3dAnmCalcNsBta(NNSG3dMatAnmResult *pResult, const NNSG3dAnmObj *pAnmObj, u32 dataIdx);
extern NNSG3dFuncAnmMat gAnmCalcFuncNsBta; // NNS_G3dFuncAnmMatNsBtaDefault = &NNSi_G3dAnmCalcNsBta

#endif // POKEHEARTGOLD_FIELD_MAP_PROP_RENDER_H
