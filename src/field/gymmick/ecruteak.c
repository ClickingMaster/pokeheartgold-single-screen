#include "constants/sprites.h"

#include "fielddata/script/scr_seq/event_T27GYM0101.h"

#include "field_system.h"
#include "field/map_object_billboard_motion.h"
#include "field/map_object_render.h"
#include "overlay_04.h"
#include "sound_playback.h"
#include "billboard_lists.h"

#define NUM_CANDLES 4

typedef struct EcruteakGymmickLocalData {
    int candleMapProps[NUM_CANDLES];
    int candleIdx;
    SysTask *sysTask;
    LocalMapObject *mapObject;
} EcruteakGymmickLocalData; // size: 0x1C

typedef struct EcruteakExtinguishCandleTaskData {
    int state;
    int scaleFactor;
    int delayTimer;
    FieldSystem *fieldSystem;
} EcruteakExtinguishCandleTaskData;

static void SysTask_MoveCandleToFollowMapObject(SysTask *task, void *data);
static BOOL Task_ExtinguishCandle(TaskManager *taskman);
static u8 MortyGymTrainerObjectToCandleIdx(LocalMapObject *mapObject);
static void ApplyFogToMapObjects(FieldSystem *fieldSystem);

void GymmickInit_Ecruteak(FieldSystem *fieldSystem) {
    GymmickUnion *gymmickUnion = Save_Gymmick_AssertMagic_GetData(Save_GetGymmickPtr(FieldSystem_GetSaveData(fieldSystem)), GYMMICK_ECRUTEAK);
    fieldSystem->mapSystems->localData = Heap_Alloc(HEAP_ID_FIELD1, sizeof(EcruteakGymmickLocalData));
    MI_CpuClear8(fieldSystem->mapSystems->localData, sizeof(EcruteakGymmickLocalData));
    EcruteakGymmickLocalData *localData = fieldSystem->mapSystems->localData;
    fieldSystem->unk11C = FX32_CONST(0.0625);
    Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
    Fog_SetColor(fieldSystem->fog, -1, 0, 31);

    s8 fogTable[32];
    for (int i = 0; i < 32; ++i) {
        fogTable[i] = -1;
    }
    Fog_SetTable(fieldSystem->fog, (u32 *)fogTable);
    ApplyFogToMapObjects(fieldSystem);

    for (int i = 0; i < NUM_CANDLES; ++i) {
        VecFx32 sp08;
        MapObject_CopyPositionVector(MapObjectManager_GetFirstActiveObjectByID(fieldSystem->mapObjectManager, obj_T27GYM0101_itako + i), &sp08);
        localData->candleMapProps[i] = MapPropManager_LoadOne(fieldSystem->mapPropManager, 128, &sp08, NULL, fieldSystem->mapPropAnimationManager);
        if (gymmickUnion->ecruteak.candles[i]) {
            MapProp_SetCulled(MapPropManager_GetMapPropByIndex(fieldSystem->mapPropManager, localData->candleMapProps[i]), TRUE);
        }
    }
}

void GymmickFree_Ecruteak(FieldSystem *fieldSystem) {
    Heap_Free(fieldSystem->mapSystems->localData);
    fieldSystem->mapSystems->localData = NULL;
}

void EcruteakGymmick_BindCandleToTrainerObject(FieldSystem *fieldSystem) {
    EcruteakGymmickLocalData *localData = fieldSystem->mapSystems->localData;

    LocalMapObject **attrPtr = FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_ENGAGED_TRAINER_0_EVENT);
    u8 candleIdx = MortyGymTrainerObjectToCandleIdx(*attrPtr);
    if (candleIdx == NUM_CANDLES) {
        GF_ASSERT(FALSE);
        return;
    }
    localData->candleIdx = candleIdx;
    localData->mapObject = *attrPtr;
    localData->sysTask = SysTask_CreateOnMainQueue(SysTask_MoveCandleToFollowMapObject, fieldSystem, 0);
}

void EcruteakGymmick_UnbindCandleFromTrainerObject(FieldSystem *fieldSystem) {
    EcruteakGymmickLocalData *localData = fieldSystem->mapSystems->localData;
    SysTask_Destroy(localData->sysTask);
}

void EcruteakGymmick_ExtinguishCandle(FieldSystem *fieldSystem, ScriptEnvField field) {
    EcruteakGymmickLocalData *localData = fieldSystem->mapSystems->localData;
    LocalMapObject **attrPtr = FieldSysGetAttrAddr(fieldSystem, field);
    localData->candleIdx = MortyGymTrainerObjectToCandleIdx(*attrPtr);
    localData->mapObject = *attrPtr;
    EcruteakExtinguishCandleTaskData *taskData = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(EcruteakExtinguishCandleTaskData));
    taskData->state = 0;
    taskData->scaleFactor = 1;
    taskData->delayTimer = 0;
    taskData->fieldSystem = fieldSystem;
    TaskManager_Call(fieldSystem->taskman, Task_ExtinguishCandle, taskData);
}

static void SysTask_MoveCandleToFollowMapObject(SysTask *task, void *data) {
    FieldSystem *fieldSystem = data;
    EcruteakGymmickLocalData *localData = fieldSystem->mapSystems->localData;
    VecFx32 objectPos;
    MapObject_CopyPositionVector(localData->mapObject, &objectPos);
    MapProp_SetTranslation(MapPropManager_GetMapPropByIndex(fieldSystem->mapPropManager, localData->candleMapProps[localData->candleIdx]), &objectPos);
}

static BOOL Task_ExtinguishCandle(TaskManager *taskman) {
    EcruteakExtinguishCandleTaskData *taskData = TaskManager_GetEnvironment(taskman);
    EcruteakGymmickLocalData *localData = taskData->fieldSystem->mapSystems->localData;

    switch (taskData->state) {
    case 0:
        if (taskData->delayTimer++ >= 30) {
            PlaySE(SEQ_SE_GS_ROUSOKU_KIERU);
            ++taskData->state;
        }
        break;
    case 1: {
        MapProp *mapProp = MapPropManager_GetMapPropByIndex(taskData->fieldSystem->mapPropManager, localData->candleMapProps[localData->candleIdx]);
        if (taskData->scaleFactor >= 4) {
            MapProp_SetCulled(mapProp, TRUE);
            GymmickUnion *gymmickUnion = Save_Gymmick_AssertMagic_GetData(Save_GetGymmickPtr(FieldSystem_GetSaveData(taskData->fieldSystem)), GYMMICK_ECRUTEAK);
            gymmickUnion->ecruteak.candles[localData->candleIdx] = TRUE;
            MapObject_ChangeBillboardSprite(localData->mapObject, SPRITE_ITAKO_);
            ++taskData->state;
        } else {
            ++taskData->scaleFactor;
        }
        VecFx32 *scale = MapProp_GetScale(mapProp);
        scale->x = FX32_ONE / taskData->scaleFactor;
        scale->y = FX32_ONE / taskData->scaleFactor;
        scale->z = FX32_ONE / taskData->scaleFactor;
        break;
    }
    case 2:
        if (!MapObject_IsBillboardPending(localData->mapObject)) {
            ApplyFogToMapObjects(taskData->fieldSystem);
            Heap_Free(taskData);
            return TRUE;
        }
        break;
    }

    return FALSE;
}

static u8 MortyGymTrainerObjectToCandleIdx(LocalMapObject *mapObject) {
    static const u32 sMortyGymTrainerObjectIds[NUM_CANDLES] = {
        obj_T27GYM0101_itako,
        obj_T27GYM0101_itako_2,
        obj_T27GYM0101_itako_3,
        obj_T27GYM0101_itako_4,
    };

    u32 mapObjectId = MapObject_GetID(mapObject);
    u8 i;
    for (i = 0; i < NUM_CANDLES; ++i) {
        if (sMortyGymTrainerObjectIds[i] == mapObjectId) {
            break;
        }
    }

    return i;
}

static void ApplyFogToMapObjects(FieldSystem *fieldSystem) {
    int i;
    int numObjects = MapObjectManager_GetObjectCount(fieldSystem->mapObjectManager);
    LocalMapObject *objects = MapObjectManager_GetObjects(fieldSystem->mapObjectManager);
    for (i = 0; i < numObjects; ++i) {
        if (MapObject_CheckActive(objects) == TRUE) {
            NNS_G3dMdlSetMdlFogEnableFlagAll(BillboardObject_GetModel(MapObject_GetBillboard(objects)), FALSE);
        }
        MapObjectArray_NextObject2(&objects);
    }
}
