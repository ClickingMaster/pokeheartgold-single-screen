#include "demo/legend.naix"
#include "field/overlay_01_021FB878.h"
#include "overlay_2/event_cutscene.h"

#include "follow_mon.h"
#include "math_util.h"
#include "field/field_motion_blur.h"
#include "field/map_object_render.h"
#include "field/field_follow_mon.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "task.h"
#include "map_object_movement_commands.h"
#include "field_effects.h"

typedef struct CelebiTimeTravelCutsceneTaskData {
    u16 unk0;
    u8 swirlTaskDone;
    u8 isSwirlFinished;
    FieldMotionBlur *motionBlur;
    FieldSystem *fieldSystem; // 0x8
    NNSFndAllocator alloc;    // 0xC
    Field3dModel model;       // 0x1C
    Field3DModelAnimation animations[3];
    Field3dObject object3d;
    s32 swirlRotation;
    u16 swirlStartAngle;
    u16 swirlFrame;
    u16 swirlDuration;
    u16 perspective;
    u16 swirlTargetPerspective;
    u16 swirlTaskState;
    u8 animateModel;
    u8 drawModel;
    u8 frameTimer;
    u8 unkF3;
} CelebiTimeTravelCutsceneTaskData;

typedef struct CelebiSwirlParams {
    u32 rotation;
    u32 duration;
    u16 perspective;
} CelebiSwirlParams;

static BOOL Task_CelebiTimeTravelCutscene(TaskManager *taskMan);
static void CelebiCutscene_StartSwirlTask(CelebiTimeTravelCutsceneTaskData *data);
static BOOL CelebiCutscene_IsSwirlTaskDone(CelebiTimeTravelCutsceneTaskData *data);
static void Task_CelebiCutsceneSwirl(SysTask *task, void *data);
static void CelebiCutscene_InitSwirlData(CelebiTimeTravelCutsceneTaskData *a0, CelebiSwirlParams *a1);
static BOOL CelebiCutscene_SwirlEffect(CelebiTimeTravelCutsceneTaskData *data);
static void Task_CelebiCutsceneSwirlEffect(SysTask *task, void *data);
static BOOL CelebiCutscene_IsSwirlFinished(CelebiTimeTravelCutsceneTaskData *data);
static void CelebiCutscene_LoadResources(CelebiTimeTravelCutsceneTaskData *data);
static void CelebiCutscene_UnloadResources(CelebiTimeTravelCutsceneTaskData *data);
static void CelebiCutsceneAnimations_FrameSet(Field3DModelAnimation *animations, u32 frame);
static BOOL CelebiCutsceneAnimations_FrameAdvanceAndCheck(Field3DModelAnimation *animations);

void FieldSystem_BeginCelebiTimeTravelCutsceneTask(FieldSystem *fieldSystem) {
    CelebiTimeTravelCutsceneTaskData *ptr = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(CelebiTimeTravelCutsceneTaskData));
    MI_CpuFill8(ptr, 0, sizeof(CelebiTimeTravelCutsceneTaskData));
    ptr->fieldSystem = fieldSystem;
    TaskManager_Call(fieldSystem->taskman, Task_CelebiTimeTravelCutscene, ptr);
}

typedef enum CelebiTimeTravelState {
    CTT_STATE_BEGIN_PALETTE_FADE,
    CTT_STATE_LOAD_RESOURCES,
    CTT_STATE_2,
    CTT_STATE_SWIRL_EFFECT,
    CTT_STATE_PALETTE_FADE_2,
    CTT_STATE_WAIT_UNLOAD_RESOURCES,
} CelebiTimeTravelState;

static BOOL Task_CelebiTimeTravelCutscene(TaskManager *taskMan) {
    int *state = TaskManager_GetStatePtr(taskMan);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    CelebiTimeTravelCutsceneTaskData *data = TaskManager_GetEnvironment(taskMan);
    switch (*state) {
    case CTT_STATE_BEGIN_PALETTE_FADE:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 4, 1, HEAP_ID_FIELD1);
        (*state)++;
        break;
    case CTT_STATE_LOAD_RESOURCES:
        if (IsPaletteFadeFinished()) {
            CelebiCutscene_LoadResources(data);
            FollowMon_HideUntilStep(FollowMon_GetMapObject(data->fieldSystem), 1);
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 4, 1, HEAP_ID_FIELD1);
            (*state)++;
        }
        break;
    case CTT_STATE_2:
        if (IsPaletteFadeFinished()) {
            MapObject_CallDrawUpdate(FollowMon_GetMapObject(data->fieldSystem));
            data->animateModel = 1;
            (*state)++;
        }
        break;
    case CTT_STATE_SWIRL_EFFECT:
        if (++data->frameTimer >= 30) {
            CelebiCutscene_StartSwirlTask(data);
            data->frameTimer = 0;
            (*state)++;
        }
        break;
    case CTT_STATE_PALETTE_FADE_2:
        if (++data->frameTimer >= 80) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 30, 1, HEAP_ID_FIELD1);
            (*state)++;
        }
        break;
    case CTT_STATE_WAIT_UNLOAD_RESOURCES:
        if (IsPaletteFadeFinished() && CelebiCutscene_IsSwirlTaskDone(data)) {
            CelebiCutscene_UnloadResources(data);
            FieldMotionBlur_Stop(&data->motionBlur);
            Heap_Free(data);
            return TRUE;
        }
    }
    if (data->animateModel) {
        CelebiCutsceneAnimations_FrameAdvanceAndCheck(&data->animations[0]);
    }
    if (data->drawModel) {
        Field3dObject_Draw(&data->object3d);
    }

    return FALSE;
}

static void CelebiCutscene_StartSwirlTask(CelebiTimeTravelCutsceneTaskData *data) {
    data->motionBlur = FieldMotionBlur_Start(3, 13);
    SysTask_CreateOnMainQueue(Task_CelebiCutsceneSwirl, data, 0);
    data->swirlTaskDone = 0;
}

static BOOL CelebiCutscene_IsSwirlTaskDone(CelebiTimeTravelCutsceneTaskData *data) {
    return data->swirlTaskDone != 0;
}

static void Task_CelebiCutsceneSwirl(SysTask *task, void *_data) {
    CelebiTimeTravelCutsceneTaskData *data = _data;
    switch (data->swirlTaskState) {
    case 0:
        CelebiSwirlParams unk = { 0xFFFF0000, 0xB4, 0x500 };
        unk.perspective = Camera_GetPerspectiveAngle(data->fieldSystem->camera);
        CelebiCutscene_InitSwirlData(data, &unk);
        data->swirlTaskState++;
        break;
    case 1:
        if (CelebiCutscene_IsSwirlFinished(data)) {
            data->swirlTaskDone = 1;
            SysTask_Destroy(task);
        }
    }
}

static void CelebiCutscene_InitSwirlData(CelebiTimeTravelCutsceneTaskData *data, CelebiSwirlParams *a1) {
    data->isSwirlFinished = FALSE;
    data->swirlFrame = 0;
    data->swirlRotation = a1->rotation;
    data->swirlStartAngle = data->unk0;
    data->perspective = Camera_GetPerspectiveAngle(data->fieldSystem->camera);
    data->swirlTargetPerspective = a1->perspective;
    data->swirlDuration = a1->duration;
    SysTask_CreateOnMainQueue(Task_CelebiCutsceneSwirlEffect, data, 1);
}

static BOOL CelebiCutscene_SwirlEffect(CelebiTimeTravelCutsceneTaskData *data) {
    VecFx32 vec;
    MtxFx33 rotMatrix;
    const VecFx32 vec2 = { 0, 0x1000, 0 };
    Camera *camera = data->fieldSystem->camera;

    data->swirlFrame++;
    int e6 = data->swirlFrame;
    int e8 = data->swirlDuration;

    int angle = data->swirlTargetPerspective - data->perspective;
    angle *= e6;
    angle /= e8;
    angle += data->perspective;
    u16 index = ((data->swirlRotation * e6) / e8) + data->swirlStartAngle;
    MTX_RotZ33(&rotMatrix, FX_SinIdx(index), FX_CosIdx(index));
    MTX_MultVec33(&vec2, &rotMatrix, &vec);
    Camera_SetLookAtCamUp(&vec, camera);
    Camera_SetPerspectiveAngle(angle, camera);
    return data->swirlFrame >= data->swirlDuration;
}

static void Task_CelebiCutsceneSwirlEffect(SysTask *task, void *_data) {
    CelebiTimeTravelCutsceneTaskData *data = _data;
    if (CelebiCutscene_SwirlEffect(data)) {
        data->isSwirlFinished = TRUE;
        SysTask_Destroy(task);
    }
}

static BOOL CelebiCutscene_IsSwirlFinished(CelebiTimeTravelCutsceneTaskData *data) {
    return data->isSwirlFinished != FALSE;
}

static void CelebiCutscene_LoadResources(CelebiTimeTravelCutsceneTaskData *data) {
    const u32 files[3] = { NARC_legend_legend_00000076_NSBCA, NARC_legend_legend_00000078_NSBTP, NARC_legend_legend_00000077_NSBTA };
    HeapExp_FndInitAllocator(&data->alloc, HEAP_ID_FIELD1, 32);
    Field3dModel_LoadFromFilesystem(&data->model, NARC_demo_legend, NARC_legend_legend_00000075_NSBMD, HEAP_ID_FIELD1);
    for (u8 i = 0; i < NELEMS(data->animations); i++) {
        Field3dModelAnimation_LoadFromFilesystem(&data->animations[i], &data->model, NARC_demo_legend, files[i], HEAP_ID_FIELD1, &data->alloc);
    }
    Field3dObject_InitFromModel(&data->object3d, &data->model);
    for (u8 i = 0; i < NELEMS(data->animations); i++) {
        Field3dObject_AddAnimation(&data->object3d, &data->animations[i]);
    }
    Field3dObject_SetActiveFlag(&data->object3d, 1);
    CelebiCutsceneAnimations_FrameSet(&data->animations[0], 0);
    LocalMapObject *followMonObj = FollowMon_GetMapObject(data->fieldSystem);
    VecFx32 pos;
    MapObject_CopyPositionVector(followMonObj, &pos);
    Field3dObject_SetPosEx(&data->object3d, pos.x, pos.y, pos.z);
    data->drawModel = 1;
}

static void CelebiCutscene_UnloadResources(CelebiTimeTravelCutsceneTaskData *data) {
    for (u8 i = 0; i < NELEMS(data->animations); i++) {
        Field3dModelAnimation_Unload(&data->animations[i], &data->alloc);
    }
    Field3dModel_Unload(&data->model);
}

static void CelebiCutsceneAnimations_FrameSet(Field3DModelAnimation *animations, u32 frame) {
    for (u8 i = 0; i < 3; i++) {
        Field3dModelAnimation_FrameSet(&animations[i], frame);
    }
}

static BOOL CelebiCutsceneAnimations_FrameAdvanceAndCheck(Field3DModelAnimation *animations) {
    u8 i;
    u8 cnt = 0;
    for (i = 0; i < 3; i++) {
        if (Field3dModelAnimation_FrameAdvanceAndCheck(&animations[i], 0x1000) != 0) {
            cnt++;
        }
    }

    return cnt == 3;
}
