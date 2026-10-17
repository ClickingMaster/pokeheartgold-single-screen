#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "demo/legend.naix"
#include "field/area_light.h"
#include "field/field_3d_model.h"
#include "msgdata/msg.naix"

#include "bg_window.h"
#include "camera.h"
#include "field_system.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "msgdata.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "sound_playback.h"
#include "sys_task.h"
#include "system.h"
#include "task.h"
#include "text.h"

// A field model with a set of animations loaded from one archive.
typedef struct FieldAnimModelTemplate {
    u16 narcId;
    u8 modelFileId;
    u8 animCount;
    const u16 *animFileIds;
} FieldAnimModelTemplate;

typedef struct FieldAnimModel {
    Field3dModel model;
    Field3dObject object;
    Field3DModelAnimation *animations;
    NNSFndAllocator allocator;
    u8 animCount;
    u16 speed;
    enum HeapID heapId;
} FieldAnimModel;

typedef struct TrapEffectTemplate {
    FieldAnimModelTemplate model;
    u16 se;
} TrapEffectTemplate;

typedef struct LighthouseLightUpEnv {
    enum HeapID heapId;
    FieldSystem *fieldSystem;
    u16 state;
    u16 timer;
    Field3dModel model;
    Field3DModelAnimation anims[2];
    Field3dObject object;
    NNSFndAllocator allocator;
    s32 seTimer;
} LighthouseLightUpEnv;

typedef struct TrapEffectEnv {
    enum HeapID heapId;
    FieldSystem *fieldSystem;
    FieldAnimModel *model;
    u16 state;
    u16 unk_0E;
    u16 kind;
} TrapEffectEnv;

typedef struct LegendaryWingEnv {
    enum HeapID heapId;
    FieldSystem *fieldSystem;
    FieldAnimModel *model;
    SysTask *animTask;
    u8 kind;
    u16 state;
    u8 filler_14[4];
} LegendaryWingEnv;

typedef struct HideoutAlarmEnv {
    enum HeapID heapId;
    FieldSystem *fieldSystem;
    u8 state;
    u8 flashState;
    u8 flashStep;
    u8 flashWait;
    u8 flashCount;
    u8 blend;
    u8 bg0Priority;
    u8 bg1Priority;
} HideoutAlarmEnv;

typedef struct CameraShake {
    enum HeapID heapId;
    FieldSystem *fieldSystem;
    s16 amplitudeX;
    s16 amplitudeY;
    u16 unk_0C;
    volatile u16 repeats;
    u16 ticks;
    u16 period;
    fx32 angleStep;
    Camera *camera;
    u8 filler_1C[4];
    VecFx32 target;
    VecFx32 base;
} CameraShake;

typedef struct ScreenShakeEnv {
    enum HeapID heapId;
    FieldSystem *fieldSystem;
    CameraShake *shake;
} ScreenShakeEnv;

typedef struct AlphHiddenRoomEnv {
    enum HeapID heapId;
    FieldSystem *fieldSystem;
    Window window;
    u8 state;
    u8 msgNo;
    u8 bg1Priority;
} AlphHiddenRoomEnv;

typedef struct LanceFliesAwayEnv {
    enum HeapID heapId;
    u32 state;
    FieldSystem *fieldSystem;
    Pokemon *dragonite;
    LocalMapObject *lance;
    LocalMapObject *dragoniteObject;
    SysTask *flyTask;
} LanceFliesAwayEnv;

static LighthouseLightUpEnv *LighthouseLightUp_CreateTaskEnv(FieldSystem *fieldSystem);
static BOOL Task_ShowLighthouseLightUp(TaskManager *taskManager);
static void LighthouseLightUp_Load(LighthouseLightUpEnv *env);
static void LighthouseLightUp_Unload(LighthouseLightUpEnv *env);
static void LighthouseLightUp_Animate(LighthouseLightUpEnv *env);
static FieldAnimModel *FieldAnimModel_New(const FieldAnimModelTemplate *template, enum HeapID heapId);
static void FieldAnimModel_Delete(FieldAnimModel *animModel);
static void FieldAnimModel_SetAnimations(FieldAnimModel *animModel, NarcId narcId, const u16 *fileIds, u8 count);
static BOOL FieldAnimModel_Animate(FieldAnimModel *animModel);
static void FieldAnimModel_AnimateLoop(FieldAnimModel *animModel);
static TrapEffectEnv *TrapEffect_CreateTaskEnv(FieldSystem *fieldSystem, u32 kind);
static BOOL Task_ShowTrapEffect(TaskManager *taskManager);
static LegendaryWingEnv *LegendaryWing_CreateTaskEnv(FieldSystem *fieldSystem, u32 kind);
static BOOL Task_ShowLegendaryWing(TaskManager *taskManager);
static void LegendaryWing_AnimateLoop(SysTask *task, void *data);
static HideoutAlarmEnv *HideoutAlarm_CreateTaskEnv(FieldSystem *fieldSystem);
static BOOL Task_ShowHideoutAlarm(TaskManager *taskManager);
static void HideoutAlarm_Load(HideoutAlarmEnv *env);
static void HideoutAlarm_Unload(HideoutAlarmEnv *env);
static BOOL HideoutAlarm_Flash(HideoutAlarmEnv *env);
static CameraShake *CameraShake_New(FieldSystem *fieldSystem, enum HeapID heapId);
static void CameraShake_Delete(CameraShake *shake);
static void CameraShake_Start(CameraShake *shake, int amplitudeX, int amplitudeY, u32 repeats, u16 period);
static void CameraShake_RestoreCamera(CameraShake *shake);
static BOOL CameraShake_Update(CameraShake *shake);
static ScreenShakeEnv *ScreenShake_CreateTaskEnv(FieldSystem *fieldSystem, u32 amplitudeX, u32 amplitudeY, u32 repeats, u16 period);
static BOOL Task_ShakeScreen(TaskManager *taskManager);
static AlphHiddenRoomEnv *AlphHiddenRoom_CreateTaskEnv(FieldSystem *fieldSystem, u32 msgNo);
static BOOL Task_OpenAlphHiddenRoom(TaskManager *taskManager);
static void AlphHiddenRoom_ShowMessage(AlphHiddenRoomEnv *env);
static void AlphHiddenRoom_HideMessage(AlphHiddenRoomEnv *env);
static BOOL AlphHiddenRoom_IsAButtonPressed(void);
static LanceFliesAwayEnv *LanceFliesAway_CreateTaskEnv(FieldSystem *fieldSystem, LocalMapObject *lance, LocalMapObject *dragoniteObject);
static BOOL Task_LanceFliesAway(TaskManager *taskManager);

static const u16 sElectricTrapAnims[1] = { NARC_legend_legend_00000011_NSBCA };

static const u16 sPoisonTrapAnims[2] = { NARC_legend_legend_00000004_NSBCA, NARC_legend_legend_00000005_NSBMA };

static const u16 sLegendaryWingLoopAnims1[3] = { NARC_legend_legend_00000069_NSBCA, NARC_legend_legend_00000070_NSBMA, NARC_legend_legend_00000071_NSBTP };

static const u16 sRockTrapAnims[3] = { NARC_legend_legend_00000007_NSBCA, NARC_legend_legend_00000008_NSBMA, NARC_legend_legend_00000009_NSBTA };

static const u16 sLegendaryWingEndAnims0[3] = { NARC_legend_legend_00000062_NSBCA, NARC_legend_legend_00000063_NSBMA, NARC_legend_legend_00000064_NSBTP };

static const u16 sLegendaryWingEndAnims1[3] = { NARC_legend_legend_00000072_NSBCA, NARC_legend_legend_00000073_NSBMA, NARC_legend_legend_00000074_NSBTP };

static const u16 sLegendaryWingIntroAnims0[3] = { NARC_legend_legend_00000057_NSBCA, NARC_legend_legend_00000056_NSBMA, NARC_legend_legend_00000058_NSBTP };

static const u16 sLegendaryWingLoopAnims0[3] = { NARC_legend_legend_00000059_NSBCA, NARC_legend_legend_00000060_NSBMA, NARC_legend_legend_00000061_NSBTP };

static const u16 sLegendaryWingIntroAnims1[3] = { NARC_legend_legend_00000067_NSBCA, NARC_legend_legend_00000066_NSBMA, NARC_legend_legend_00000068_NSBTP };

static const WindowTemplate sAlphHiddenRoomWindowTemplate = {
    .bgId = GF_BG_LYR_MAIN_1,
    .left = 9,
    .top = 10,
    .width = 14,
    .height = 2,
    .palette = 0,
    .baseTile = 0x80,
};

static const FieldAnimModelTemplate sLegendaryWingModels[2] = {
    {
     .narcId = NARC_demo_legend,
     .modelFileId = NARC_legend_legend_00000055_NSBMD,
     .animCount = 3,
     .animFileIds = sLegendaryWingIntroAnims0,
     },
    {
     .narcId = NARC_demo_legend,
     .modelFileId = NARC_legend_legend_00000065_NSBMD,
     .animCount = 3,
     .animFileIds = sLegendaryWingIntroAnims1,
     },
};

static const TrapEffectTemplate sTrapEffects[3] = {
    {
     .model = {
            .narcId = NARC_demo_legend,
            .modelFileId = NARC_legend_legend_00000010_NSBMD,
            .animCount = 1,
            .animFileIds = sElectricTrapAnims,
        },
     .se = SEQ_SE_GS_DENKI_TRAP,
     },
    {
     .model = {
            .narcId = NARC_demo_legend,
            .modelFileId = NARC_legend_legend_00000003_NSBMD,
            .animCount = 2,
            .animFileIds = sPoisonTrapAnims,
        },
     .se = SEQ_SE_GS_DOKU_TRAP,
     },
    {
     .model = {
            .narcId = NARC_demo_legend,
            .modelFileId = NARC_legend_legend_00000006_NSBMD,
            .animCount = 3,
            .animFileIds = sRockTrapAnims,
        },
     .se = SEQ_SE_GS_IWA_TRAP,
     },
};

static const u16 *sLegendaryWingLoopAnimLists[1] = {
    sLegendaryWingLoopAnims0,
};

static const u16 *sLegendaryWingEndAnimLists[3] = {
    sLegendaryWingEndAnims0,
    sLegendaryWingLoopAnims1,
    sLegendaryWingEndAnims1,
};

void ShowLighthouseLightUp(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    LighthouseLightUpEnv *env = LighthouseLightUp_CreateTaskEnv(fieldSystem);
    TaskManager_Call(taskManager, Task_ShowLighthouseLightUp, env);
}

static LighthouseLightUpEnv *LighthouseLightUp_CreateTaskEnv(FieldSystem *fieldSystem) {
    LighthouseLightUpEnv *env = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LighthouseLightUpEnv));
    if (env == NULL) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    MI_CpuFill8(env, 0, sizeof(LighthouseLightUpEnv));
    env->fieldSystem = fieldSystem;
    env->heapId = HEAP_ID_FIELD1;
    return env;
}

static BOOL Task_ShowLighthouseLightUp(TaskManager *taskManager) {
    LighthouseLightUpEnv *env = TaskManager_GetEnvironment(taskManager);
    switch (env->state) {
    case 0:
        LighthouseLightUp_Load(env);
        env->timer = 0;
        env->seTimer = 0;
        env->state++;
        break;
    case 1:
        if (env->seTimer != 255) {
            if (env->seTimer == 0 || env->seTimer == 30 || env->seTimer == 50 || env->seTimer == 60 || env->seTimer == 70 || env->seTimer == 80) {
                PlaySE(SEQ_SE_DP_FW411);
                if (env->seTimer == 50) {
                    PlaySE(SEQ_SE_DP_F207);
                }
                if (env->seTimer >= 60) {
                    PlaySE_SetPitch(SEQ_SE_DP_FW411, (s16)((u32)(env->seTimer / 10 - 5) * 64u));
                }
            }
            env->seTimer = (s32)((u32)env->seTimer + 1);
        }
        LighthouseLightUp_Animate(env);
        if (env->timer++ >= 90) {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 24, 2, env->heapId);
            env->state++;
        }
        break;
    case 2:
        LighthouseLightUp_Animate(env);
        if (IsPaletteFadeFinished()) {
            env->timer = 0;
            env->state++;
        }
        break;
    case 3:
        if (env->timer++ >= 60) {
            LighthouseLightUp_Unload(env);
            AreaLightManager_Free(&env->fieldSystem->areaLightManager);
            env->fieldSystem->areaLightManager = AreaLightManager_New(env->fieldSystem->modelAttributes, 4);
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 18, 1, env->heapId);
            env->state++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            Heap_Free(env);
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

static void LighthouseLightUp_Load(LighthouseLightUpEnv *env) {
    HeapExp_FndInitAllocator(&env->allocator, env->heapId, 32);
    Field3dModel_LoadFromFilesystem(&env->model, NARC_demo_legend, NARC_legend_legend_00000000_NSBMD, env->heapId);
    Field3dModelAnimation_LoadFromFilesystem(&env->anims[0], &env->model, NARC_demo_legend, NARC_legend_legend_00000001_NSBCA, env->heapId, &env->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&env->anims[1], &env->model, NARC_demo_legend, NARC_legend_legend_00000002_NSBTP, env->heapId, &env->allocator);
    Field3dObject_InitFromModel(&env->object, &env->model);
    Field3dObject_AddAnimation(&env->object, &env->anims[0]);
    Field3dObject_AddAnimation(&env->object, &env->anims[1]);
    Field3dObject_SetPosEx(&env->object, FX32_CONST(136), 0, FX32_CONST(160));
    Field3dObject_SetActiveFlag(&env->object, TRUE);
}

static void LighthouseLightUp_Unload(LighthouseLightUpEnv *env) {
    Field3dObject_SetActiveFlag(&env->object, FALSE);
    Field3dModelAnimation_Unload(&env->anims[1], &env->allocator);
    Field3dModelAnimation_Unload(&env->anims[0], &env->allocator);
    Field3dModel_Unload(&env->model);
}

static void LighthouseLightUp_Animate(LighthouseLightUpEnv *env) {
    Field3dModelAnimation_FrameAdvanceAndLoop(&env->anims[0], FX32_ONE);
    Field3dModelAnimation_FrameAdvanceAndLoop(&env->anims[1], FX32_ONE);
    Field3dObject_Draw(&env->object);
}

static FieldAnimModel *FieldAnimModel_New(const FieldAnimModelTemplate *template, enum HeapID heapId) {
    FieldAnimModel *animModel = Heap_Alloc(heapId, sizeof(FieldAnimModel));
    MI_CpuFill8(animModel, 0, sizeof(FieldAnimModel));
    animModel->heapId = heapId;
    HeapExp_FndInitAllocator(&animModel->allocator, heapId, 32);
    Field3dModel_LoadFromFilesystem(&animModel->model, (NarcId) template->narcId, template->modelFileId, heapId);
    Field3dObject_InitFromModel(&animModel->object, &animModel->model);
    FieldAnimModel_SetAnimations(animModel, (NarcId) template->narcId, template->animFileIds, template->animCount);
    Field3dObject_SetActiveFlag(&animModel->object, TRUE);
    animModel->speed = 1;
    return animModel;
}

static void FieldAnimModel_Delete(FieldAnimModel *animModel) {
    int i;

    Field3dObject_SetActiveFlag(&animModel->object, FALSE);
    for (i = 0; i < animModel->animCount; i++) {
        Field3dObject_RemoveAnimation(&animModel->object, &animModel->animations[i]);
        Field3dModelAnimation_Unload(&animModel->animations[i], &animModel->allocator);
    }
    animModel->animCount = 0;
    Field3dModel_Unload(&animModel->model);
    Heap_Free(animModel->animations);
    Heap_Free(animModel);
}

static void FieldAnimModel_SetAnimations(FieldAnimModel *animModel, NarcId narcId, const u16 *fileIds, u8 count) {
    int i;

    if (animModel->animCount != 0) {
        for (i = 0; i < animModel->animCount; i++) {
            Field3dObject_RemoveAnimation(&animModel->object, &animModel->animations[i]);
            Field3dModelAnimation_Unload(&animModel->animations[i], &animModel->allocator);
        }
        Heap_Free(animModel->animations);
    }
    animModel->animCount = count;
    animModel->animations = Heap_Alloc(animModel->heapId, animModel->animCount * sizeof(Field3DModelAnimation));
    MI_CpuFill8(animModel->animations, 0, animModel->animCount * sizeof(Field3DModelAnimation));
    for (i = 0; i < animModel->animCount; i++) {
        Field3dModelAnimation_LoadFromFilesystem(&animModel->animations[i], &animModel->model, narcId, *fileIds, animModel->heapId, &animModel->allocator);
        Field3dObject_AddAnimation(&animModel->object, &animModel->animations[i]);
        fileIds++;
    }
}

static BOOL FieldAnimModel_Animate(FieldAnimModel *animModel) {
    int finished = 0;
    int i;

    for (i = 0; i < animModel->animCount; i++) {
        finished += Field3dModelAnimation_FrameAdvanceAndCheck(&animModel->animations[i], FX32_CONST(animModel->speed));
    }
    Field3dObject_Draw(&animModel->object);
    return finished == animModel->animCount;
}

static void FieldAnimModel_AnimateLoop(FieldAnimModel *animModel) {
    int i;

    for (i = 0; i < animModel->animCount; i++) {
        Field3dModelAnimation_FrameAdvanceAndLoop(&animModel->animations[i], FX32_CONST(animModel->speed));
    }
    Field3dObject_Draw(&animModel->object);
}

void ShowTrapEffect(TaskManager *taskManager, u8 kind) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    TrapEffectEnv *env = TrapEffect_CreateTaskEnv(fieldSystem, kind);
    TaskManager_Call(taskManager, Task_ShowTrapEffect, env);
}

static TrapEffectEnv *TrapEffect_CreateTaskEnv(FieldSystem *fieldSystem, u32 kind) {
    TrapEffectEnv *env = Heap_Alloc(HEAP_ID_FIELD1, sizeof(TrapEffectEnv));
    if (env == NULL) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    MI_CpuFill8(env, 0, sizeof(TrapEffectEnv));
    env->fieldSystem = fieldSystem;
    env->kind = kind;
    env->heapId = HEAP_ID_FIELD1;
    return env;
}

static BOOL Task_ShowTrapEffect(TaskManager *taskManager) {
    TrapEffectEnv *env = TaskManager_GetEnvironment(taskManager);
    VecFx32 pos;

    switch (env->state) {
    case 0:
        env->model = FieldAnimModel_New(&sTrapEffects[env->kind].model, env->heapId);
        PlayerAvatar_CopyPositionVector(env->fieldSystem->playerAvatar, &pos);
        Field3dObject_SetPosEx(&env->model->object, pos.x, pos.y, pos.z);
        PlaySE(sTrapEffects[env->kind].se);
        env->state++;
        break;
    case 1:
        if (FieldAnimModel_Animate(env->model)) {
            env->state++;
        }
        break;
    case 2:
        FieldAnimModel_Delete(env->model);
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

void ShowLegendaryWing(TaskManager *taskManager, u8 kind) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    LegendaryWingEnv *env = LegendaryWing_CreateTaskEnv(fieldSystem, kind);
    TaskManager_Call(taskManager, Task_ShowLegendaryWing, env);
}

static LegendaryWingEnv *LegendaryWing_CreateTaskEnv(FieldSystem *fieldSystem, u32 kind) {
    LegendaryWingEnv *env = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LegendaryWingEnv));
    if (env == NULL) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    MI_CpuFill8(env, 0, sizeof(LegendaryWingEnv));
    env->fieldSystem = fieldSystem;
    env->kind = kind;
    env->heapId = HEAP_ID_FIELD1;
    return env;
}

static BOOL Task_ShowLegendaryWing(TaskManager *taskManager) {
    LegendaryWingEnv *env = TaskManager_GetEnvironment(taskManager);
    switch (env->state) {
    case 0: {
        VecFx32 pos;
        env->model = FieldAnimModel_New(&sLegendaryWingModels[env->kind], env->heapId);
        PlayerAvatar_CopyPositionVector(env->fieldSystem->playerAvatar, &pos);
        Field3dObject_SetPosEx(&env->model->object, pos.x, pos.y, pos.z);
        PlaySE(SEQ_SE_GS_STOPPERKAIJO);
        env->state++;
        break;
    }
    case 1:
        if (FieldAnimModel_Animate(env->model)) {
            FieldAnimModel_SetAnimations(env->model, NARC_demo_legend, sLegendaryWingLoopAnimLists[env->kind * 2], 3);
            env->animTask = SysTask_CreateOnMainQueue(LegendaryWing_AnimateLoop, env, 0);
            QueueScript(taskManager, 3, NULL, NULL);
            env->state++;
        }
        break;
    case 2:
        SysTask_Destroy(env->animTask);
        env->state++;
    case 3:
        if (!FieldAnimModel_Animate(env->model)) {
            break;
        }
        FieldAnimModel_SetAnimations(env->model, NARC_demo_legend, sLegendaryWingEndAnimLists[env->kind * 2], 3);
        env->state++;
    case 4:
        if (!FieldAnimModel_Animate(env->model)) {
            break;
        }
        FieldAnimModel_Delete(env->model);
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

static void LegendaryWing_AnimateLoop(SysTask *task, void *data) {
    LegendaryWingEnv *env = data;
    FieldAnimModel_AnimateLoop(env->model);
}

void ShowHideoutAlarm(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    HideoutAlarmEnv *env = HideoutAlarm_CreateTaskEnv(fieldSystem);
    TaskManager_Call(taskManager, Task_ShowHideoutAlarm, env);
}

static HideoutAlarmEnv *HideoutAlarm_CreateTaskEnv(FieldSystem *fieldSystem) {
    HideoutAlarmEnv *env = Heap_Alloc(HEAP_ID_FIELD1, sizeof(HideoutAlarmEnv));
    if (env == NULL) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    MI_CpuFill8(env, 0, sizeof(HideoutAlarmEnv));
    env->fieldSystem = fieldSystem;
    env->heapId = HEAP_ID_FIELD1;
    return env;
}

static BOOL Task_ShowHideoutAlarm(TaskManager *taskManager) {
    HideoutAlarmEnv *env = TaskManager_GetEnvironment(taskManager);
    switch (env->state) {
    case 0:
        HideoutAlarm_Load(env);
        PlaySE(SEQ_SE_GS_AJITO_SIREN);
        env->state++;
        break;
    case 1:
        if (HideoutAlarm_Flash(env)) {
            u8 count = env->flashCount++;
            if (count >= 1) {
                env->state++;
            }
        }
        break;
    case 2:
        if (!IsSEPlaying(SEQ_SE_GS_AJITO_SIREN)) {
            HideoutAlarm_Unload(env);
            Heap_Free(env);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void HideoutAlarm_Load(HideoutAlarmEnv *env) {
    NARC *narc;

    env->blend = 0;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2, env->blend, 31 - env->blend);
    narc = NARC_New(NARC_demo_legend, env->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, NARC_legend_legend_00000012_NCLR, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, env->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, NARC_legend_legend_00000013_NCGR, env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, 0, 64, FALSE, env->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, NARC_legend_legend_00000014_NSCR, env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, env->heapId);
    env->bg0Priority = GetBgPriority(env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_0);
    env->bg1Priority = GetBgPriority(env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_1, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    NARC_Delete(narc);
}

static void HideoutAlarm_Unload(HideoutAlarmEnv *env) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    SetBgPriority(GF_BG_LYR_MAIN_0, env->bg0Priority);
    SetBgPriority(GF_BG_LYR_MAIN_1, env->bg1Priority);
    BgClearTilemapBufferAndCommit(env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 64, 0, env->heapId);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 0);
}

static BOOL HideoutAlarm_Flash(HideoutAlarmEnv *env) {
    switch (env->flashState) {
    case 0:
        env->flashWait = 0;
        env->flashStep = 0;
        env->blend = 0;
        env->flashState++;
        break;
    case 1: {
        u8 wait = env->flashWait++;
        if (wait == 0) {
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2, env->blend, 31 - env->blend);
            {
                u8 step = env->flashStep++;
                if (step < 12) {
                    env->blend++;
                } else {
                    env->blend--;
                }
            }
            env->flashWait = 0;
            if (env->flashStep > 24) {
                env->flashState++;
            }
        }
    } break;
    default:
        env->flashStep = 0;
        env->flashWait = 0;
        env->flashState = 0;
        return TRUE;
    }
    return FALSE;
}

static CameraShake *CameraShake_New(FieldSystem *fieldSystem, enum HeapID heapId) {
    CameraShake *shake = Heap_Alloc(heapId, sizeof(CameraShake));
    MI_CpuFill8(shake, 0, sizeof(CameraShake));
    shake->heapId = heapId;
    shake->fieldSystem = fieldSystem;
    shake->camera = Camera_New(heapId);
    return shake;
}

static void CameraShake_Delete(CameraShake *shake) {
    Camera_Delete(shake->camera);
    Heap_Free(shake);
}

static void CameraShake_Start(CameraShake *shake, int amplitudeX, int amplitudeY, u32 repeats, u16 period) {
    VecFx32 target;
    shake->amplitudeX = (fx32)(amplitudeX > 0 ? 0.5f + (float)(amplitudeX << 12) : (float)(amplitudeX << 12) - 0.5f);
    shake->amplitudeY = (fx32)(amplitudeY > 0 ? 0.5f + (float)(amplitudeY << 12) : (float)(amplitudeY << 12) - 0.5f);
    shake->repeats = repeats;
    shake->period = period;
    shake->ticks = 0;
    shake->angleStep = FX_Div(90 << 14, (fx32)(shake->period > 0 ? 0.5f + (float)(shake->period << 12) : (float)(shake->period << 12) - 0.5f));
    Camera_Copy(shake->fieldSystem->camera, shake->camera);
    target = Camera_GetLookAtCamTarget(shake->camera);
    shake->target = target;
    shake->base = shake->target;
    Camera_SetFixedTarget(&shake->target, shake->camera);
    Camera_SetStaticPtr(shake->camera);
}

static void CameraShake_RestoreCamera(CameraShake *shake) {
    shake->ticks = 0;
    Camera_SetStaticPtr(shake->fieldSystem->camera);
}

static BOOL CameraShake_Update(CameraShake *shake) {
    s32 angle;
    s32 sine;
    float degrees;

    if (shake->ticks++) {
        degrees = 0.5f + (float)(shake->ticks++ << 12);
    } else {
        degrees = (float)(shake->ticks++ << 12) - 0.5f;
    }
    angle = (s32)degrees;
    sine = GF_SinDegFX32(FX_MUL(shake->angleStep, angle));
    shake->target.x = shake->base.x + FX_MUL(sine, shake->amplitudeX);
    shake->target.y = shake->base.y + FX_MUL(sine, shake->amplitudeY);
    if (shake->ticks < shake->period) {
        return FALSE;
    }
    shake->target.x = shake->base.x;
    shake->target.y = shake->base.y;
    shake->ticks = 0;
    shake->repeats--;
    if (!shake->repeats) {
        return TRUE;
    }
    return FALSE;
}

void ShakeScreen(TaskManager *taskManager, u32 amplitudeX, u32 amplitudeY, u32 repeats, u16 period) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    ScreenShakeEnv *env = ScreenShake_CreateTaskEnv(fieldSystem, amplitudeX, amplitudeY, repeats, period);
    TaskManager_Call(taskManager, Task_ShakeScreen, env);
}

static ScreenShakeEnv *ScreenShake_CreateTaskEnv(FieldSystem *fieldSystem, u32 amplitudeX, u32 amplitudeY, u32 repeats, u16 period) {
    ScreenShakeEnv *env = Heap_Alloc(HEAP_ID_FIELD1, sizeof(ScreenShakeEnv));
    if (env == NULL) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    MI_CpuFill8(env, 0, sizeof(ScreenShakeEnv));
    env->fieldSystem = fieldSystem;
    env->heapId = HEAP_ID_FIELD1;
    env->shake = CameraShake_New(env->fieldSystem, HEAP_ID_FIELD1);
    CameraShake_Start(env->shake, (s16)amplitudeX, (s16)amplitudeY, repeats, period);
    return env;
}

static BOOL Task_ShakeScreen(TaskManager *taskManager) {
    ScreenShakeEnv *env = TaskManager_GetEnvironment(taskManager);
    if (!CameraShake_Update(env->shake)) {
        return FALSE;
    }
    CameraShake_RestoreCamera(env->shake);
    CameraShake_Delete(env->shake);
    Heap_Free(env);
    return TRUE;
}

void OpenAlphHiddenRoom(TaskManager *taskManager, u32 msgNo) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    AlphHiddenRoomEnv *env = AlphHiddenRoom_CreateTaskEnv(fieldSystem, (u8)msgNo);
    TaskManager_Call(taskManager, Task_OpenAlphHiddenRoom, env);
}

static AlphHiddenRoomEnv *AlphHiddenRoom_CreateTaskEnv(FieldSystem *fieldSystem, u32 msgNo) {
    AlphHiddenRoomEnv *env = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AlphHiddenRoomEnv));
    if (env == NULL) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    MI_CpuFill8(env, 0, sizeof(AlphHiddenRoomEnv));
    env->fieldSystem = fieldSystem;
    env->msgNo = msgNo;
    env->heapId = HEAP_ID_FIELD1;
    return env;
}

static BOOL Task_OpenAlphHiddenRoom(TaskManager *taskManager) {
    AlphHiddenRoomEnv *env = TaskManager_GetEnvironment(taskManager);
    switch (env->state) {
    case 0:
        AlphHiddenRoom_ShowMessage(env);
        PlaySE(SEQ_SE_DP_SELECT);
        env->state++;
        break;
    case 1:
        if (AlphHiddenRoom_IsAButtonPressed()) {
            PlaySE(SEQ_SE_DP_SELECT);
            env->state++;
            AlphHiddenRoom_HideMessage(env);
            Heap_Free(env);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void AlphHiddenRoom_ShowMessage(AlphHiddenRoomEnv *env) {
    NARC *narc;
    MsgData *msgData;
    String *string;
    u8 width;
    int x;

    narc = NARC_New(NARC_demo_legend, env->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, NARC_legend_legend_00000015_NCLR, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, env->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, NARC_legend_legend_00000016_NCGR, env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, env->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, NARC_legend_legend_00000017_NSCR, env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, env->heapId);
    env->bg1Priority = GetBgPriority(env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
    SetBgPriority(GF_BG_LYR_MAIN_1, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    NARC_Delete(narc);
    AddWindow(env->fieldSystem->bgConfig, &env->window, &sAlphHiddenRoomWindowTemplate);
    FillWindowPixelBuffer(&env->window, 9);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0198_bin, env->heapId);
    string = NewString_ReadMsgData(msgData, env->msgNo);
    width = FontID_String_GetWidth(3, string, 0);
    x = 112 - width;
    AddTextPrinterParameterizedWithColor(&env->window, 3, string, x / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 9), NULL);
    ScheduleWindowCopyToVram(&env->window);
    String_Delete(string);
    DestroyMsgData(msgData);
}

static void AlphHiddenRoom_HideMessage(AlphHiddenRoomEnv *env) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    SetBgPriority(GF_BG_LYR_MAIN_1, env->bg1Priority);
    ClearWindowTilemapAndScheduleTransfer(&env->window);
    RemoveWindow(&env->window);
    BgClearTilemapBufferAndCommit(env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x1000, 0, env->heapId);
}

static BOOL AlphHiddenRoom_IsAButtonPressed(void) {
    return (gSystem.newKeys & PAD_BUTTON_A) != 0;
}

void LanceFliesAway(TaskManager *taskManager, LocalMapObject *lance, LocalMapObject *dragoniteObject) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    LanceFliesAwayEnv *env = LanceFliesAway_CreateTaskEnv(fieldSystem, lance, dragoniteObject);
    TaskManager_Call(taskManager, Task_LanceFliesAway, env);
}

static LanceFliesAwayEnv *LanceFliesAway_CreateTaskEnv(FieldSystem *fieldSystem, LocalMapObject *lance, LocalMapObject *dragoniteObject) {
    LanceFliesAwayEnv *env = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LanceFliesAwayEnv));
    if (env == NULL) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    MI_CpuFill8(env, 0, sizeof(LanceFliesAwayEnv));
    env->fieldSystem = fieldSystem;
    env->lance = lance;
    env->dragoniteObject = dragoniteObject;
    env->heapId = HEAP_ID_FIELD1;
    env->dragonite = AllocMonZeroed(HEAP_ID_FIELD1);
    ZeroMonData(env->dragonite);
    CreateMon(env->dragonite, SPECIES_DRAGONITE, 100, 32, FALSE, 0, OT_ID_RANDOM_NO_SHINY, 0);
    return env;
}

static BOOL Task_LanceFliesAway(TaskManager *taskManager) {
    LanceFliesAwayEnv *env = TaskManager_GetEnvironment(taskManager);
    switch (env->state) {
    case 0:
        env->flyTask = FieldMoveCutIn_StartFlyWithObject(env->fieldSystem, env->dragonite, env->lance, env->dragoniteObject);
        env->state++;
        break;
    case 1:
        if (FieldMoveCutIn_IsFinished(env->flyTask)) {
            FieldMoveCutIn_Delete(env->flyTask);
            Heap_Free(env->dragonite);
            Heap_Free(env);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
