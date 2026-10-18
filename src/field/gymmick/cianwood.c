#include "field_system.h"
#include "overlay_04.h"

enum CianwoodWinchState {
    WINCH_STATE_PAN_TO_WINCH,
    WINCH_STATE_WAIT_PAN_TO_WINCH,
    WINCH_STATE_START_ANIM,
    WINCH_STATE_WAIT_ANIM,
    WINCH_STATE_PAN_BACK,
    WINCH_STATE_WAIT_PAN_BACK,
    WINCH_STATE_END,
};

typedef struct CianwoodGymmickLocalData {
    FieldSystem *fieldSystem;
} CianwoodGymmickLocalData;

typedef struct CianwoodGymmickWinchTaskData {
    int state;
    int unused;
    GFCameraTranslationWrapper *translationWrapper;
} CianwoodGymmickWinchTaskData;

static void CianwoodGymmick_LaunchWinchTask(CianwoodGymmickLocalData *localData);
static BOOL Task_CianwoodGymmick_LiftWinchCutscene(TaskManager *taskman);
static int CianwoodGymmick_RebindAnims(FieldSystem *fieldSystem);

void GymmickInit_Cianwood(FieldSystem *fieldSystem) {
    GymmickUnion *gymmickUnion = Save_Gymmick_AssertMagic_GetData(Save_GetGymmickPtr(FieldSystem_GetSaveData(fieldSystem)), GYMMICK_CIANWOOD);
    fieldSystem->mapSystems->localData = Heap_Alloc(HEAP_ID_FIELD1, sizeof(CianwoodGymmickLocalData));
    MI_CpuClear8(fieldSystem->mapSystems->localData, sizeof(CianwoodGymmickLocalData));
    CianwoodGymmickLocalData *localData = fieldSystem->mapSystems->localData;
    localData->fieldSystem = fieldSystem;

    if (gymmickUnion->cianwood.winch) {
        MapPropAnimationData *animData;
        u32 num = CianwoodGymmick_RebindAnims(fieldSystem);
        for (u8 i = 0; i < num; ++i) {
            animData = MapPropAnimationManager_GetAnimationData(174, i, fieldSystem->mapPropAnimationManager);
            MapPropAnimationData_SetAnimationPaused(animData, TRUE);
            MapPropAnimationData_GoToLastAnimationFrame(animData);
        }
        animData = MapPropAnimationManager_GetAnimationData(175, 0, fieldSystem->mapPropAnimationManager);
        MapPropAnimationData_SetAnimationPaused(animData, TRUE);
        MapPropAnimationData_GoToLastAnimationFrame(animData);
    }
}

void GymmickFree_Cianwood(FieldSystem *fieldSystem) {
    Heap_Free(fieldSystem->mapSystems->localData);
    fieldSystem->mapSystems->localData = NULL;
}

BOOL CianwoodGymmick_ActivateWinch(FieldSystem *fieldSystem) {
    Gymmick *gymmick = Save_GetGymmickPtr(FieldSystem_GetSaveData(fieldSystem));
    if (Save_Gymmick_GetType(gymmick) != GYMMICK_CIANWOOD) {
        return FALSE;
    }
    GymmickUnion *gymmickUnion = Save_Gymmick_AssertMagic_GetData(gymmick, GYMMICK_CIANWOOD);
    CianwoodGymmickLocalData *localData = fieldSystem->mapSystems->localData;
    if (localData == NULL) {
        return FALSE;
    }
    if (!gymmickUnion->cianwood.winch) {
        CianwoodGymmick_LaunchWinchTask(localData);
        return TRUE;
    }
    return FALSE;
}

static void CianwoodGymmick_LaunchWinchTask(CianwoodGymmickLocalData *localData) {
    GymmickUnion *gymmickUnion = Save_Gymmick_AssertMagic_GetData(Save_GetGymmickPtr(FieldSystem_GetSaveData(localData->fieldSystem)), GYMMICK_CIANWOOD);
    CianwoodGymmickWinchTaskData *taskData = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(CianwoodGymmickWinchTaskData));
    taskData->state = WINCH_STATE_PAN_TO_WINCH;
    gymmickUnion->cianwood.winch = TRUE;
    TaskManager_Call(localData->fieldSystem->taskman, Task_CianwoodGymmick_LiftWinchCutscene, taskData);
}

static BOOL Task_CianwoodGymmick_LiftWinchCutscene(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    CianwoodGymmickWinchTaskData *taskData = TaskManager_GetEnvironment(taskman);
    LocalMapObject *playerObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);

    switch (taskData->state) {
    case WINCH_STATE_PAN_TO_WINCH: {
        CameraTranslationPathTemplate pathTemplate;
        CameraAngle cameraAngle = Camera_GetAngle(fieldSystem->camera);
        pathTemplate.angleX = cameraAngle.x;
        pathTemplate.perspectiveAngle = Camera_GetPerspectiveAngle(fieldSystem->camera);
        pathTemplate.position = (VecFx32) {
            50 * FX32_ONE,
            0,
            58 * FX32_ONE,
        };
        pathTemplate.distance = Camera_GetDistance(fieldSystem->camera);
        taskData->translationWrapper = CreateCameraTranslationWrapper(HEAP_ID_FIELD1, fieldSystem->camera);
        SetCameraTranslationPath(taskData->translationWrapper, &pathTemplate, 24);
        ++taskData->state;
    } break;
    case WINCH_STATE_WAIT_PAN_TO_WINCH:
        if (IsCameraTranslationFinished(taskData->translationWrapper)) {
            DeleteCameraTranslationWrapper(taskData->translationWrapper);
            ++taskData->state;
        }
        break;
    case WINCH_STATE_START_ANIM: {
        int num = CianwoodGymmick_RebindAnims(fieldSystem);
        MapPropAnimationData *animData;
        for (int i = 0; i < num; ++i) {
            animData = MapPropAnimationManager_GetAnimationData(174, i, fieldSystem->mapPropAnimationManager);
            MapPropAnimationData_SetAnimationLoopCount(animData, 1);
            MapPropAnimationData_GoToFirstAnimationFrame(animData);
        }
        animData = MapPropAnimationManager_GetAnimationData(175, 0, fieldSystem->mapPropAnimationManager);
        MapPropAnimationData_SetAnimationLoopCount(animData, 1);
        MapPropAnimationData_GoToFirstAnimationFrame(animData);
        ++taskData->state;
    } break;
    case WINCH_STATE_WAIT_ANIM:
        if (MapPropAnimationData_IsAnimationLoopFinished(MapPropAnimationManager_GetAnimationData(175, 0, fieldSystem->mapPropAnimationManager))) {
            ++taskData->state;
        }
        break;
    case WINCH_STATE_PAN_BACK: {
        CameraTranslationPathTemplate pathTemplate;
        VecFx32 centerPos = { 0, 0, 0 };
        CameraAngle angle = Camera_GetAngle(fieldSystem->camera);
        pathTemplate.angleX = angle.x;
        pathTemplate.perspectiveAngle = Camera_GetPerspectiveAngle(fieldSystem->camera);
        pathTemplate.position = centerPos;
        pathTemplate.distance = Camera_GetDistance(fieldSystem->camera);
        taskData->translationWrapper = CreateCameraTranslationWrapper(HEAP_ID_FIELD1, fieldSystem->camera);
        SetCameraTranslationPath(taskData->translationWrapper, &pathTemplate, 24);
        ++taskData->state;
    } break;
    case WINCH_STATE_WAIT_PAN_BACK:
        if (IsCameraTranslationFinished(taskData->translationWrapper)) {
            DeleteCameraTranslationWrapper(taskData->translationWrapper);
            ++taskData->state;
        }
        break;
    case WINCH_STATE_END:
        Heap_Free(taskData);
        return TRUE;
    }

    return FALSE;
}

static int CianwoodGymmick_RebindAnims(FieldSystem *fieldSystem) {
    Field3dRenderObj *renderObj_173 = Field3dObjectList_GetRenderObjectByID(fieldSystem->renderObjManager, 173);
    Field3dRenderObj *renderObj_174 = Field3dObjectList_GetRenderObjectByID(fieldSystem->renderObjManager, 174);
    Field3dRenderObj *renderObj_175 = Field3dObjectList_GetRenderObjectByID(fieldSystem->renderObjManager, 175);

    int numPropAnims = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, 173);
    for (int i = 0; i < numPropAnims; ++i) {
        MapPropAnimationManager_RemoveAnimationFromRenderObj(fieldSystem->mapPropAnimationManager, &renderObj_173->renderObj, 173, i);
    }

    numPropAnims = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, 174);
    for (int i = 0; i < numPropAnims; ++i) {
        MapPropAnimationManager_AddAnimationToRenderObj(174, i, TRUE, &renderObj_173->renderObj, fieldSystem->mapPropAnimationManager);
    }
    MapPropAnimationManager_AddAnimationToRenderObj(175, 0, TRUE, &renderObj_175->renderObj, fieldSystem->mapPropAnimationManager);
    return numPropAnims;
}
