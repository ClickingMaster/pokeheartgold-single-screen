#include "global.h"

#include "constants/weather.h"

#include "field/encounter_check.h"
#include "field/field_3d_object_task.h"
#include "field/weather_manager.h"

#include "assert.h"
#include "bg_window.h"
#include "field_system.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "map_header.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "script_manager.h"
#include "sound_playback.h"
#include "task.h"
#include "field_terrain.h"

// The darkening of Honey and Sweet Scent: a BG2 tint faded in (blend alpha), an encounter check, then faded out
// with a "nothing happened" script when no wild Pokemon appears. Field_UseItem (Honey) and the Sweet Scent field
// move run Task_HoneyOrSweetScent with a GetHoneySweetScentWorkSize() block.

// The data block of the fade Field3dObjectTasks.
typedef struct HoneySweetScentFade {
    int cur;
    int start;
    int delta;
    int counter;
    int max;
    int done;
} HoneySweetScentFade;

typedef struct HoneySweetScentEnv {
    Field3dObjectTask *task;
    int useWhiteFlash;
    u16 state;
    s16 timer;
} HoneySweetScentEnv;

enum HoneySweetScentState {
    HONEY_SWEET_SCENT_STATE_CHECK_WEATHER,
    HONEY_SWEET_SCENT_STATE_FADE_IN,
    HONEY_SWEET_SCENT_STATE_WAIT_FADE_IN,
    HONEY_SWEET_SCENT_STATE_WAIT_ENCOUNTER,
    HONEY_SWEET_SCENT_STATE_FADE_OUT,
    HONEY_SWEET_SCENT_STATE_WAIT_FADE_OUT,
    HONEY_SWEET_SCENT_STATE_ENCOUNTER,
    HONEY_SWEET_SCENT_STATE_WAIT_BAD_WEATHER,
    HONEY_SWEET_SCENT_STATE_NOTHING_HAPPENED,
    HONEY_SWEET_SCENT_STATE_END,
};

void HoneySweetScentFade_InitIn(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void HoneySweetScentFade_InitOut(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void HoneySweetScentFade_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void HoneySweetScentFade_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void HoneySweetScentFade_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
Field3dObjectTask *HoneySweetScent_StartFadeIn(FieldSystem *fieldSystem);
void HoneySweetScent_DeleteFade(Field3dObjectTask *task);
int HoneySweetScent_IsFadeDone(Field3dObjectTask *task);
Field3dObjectTask *HoneySweetScent_StartFadeOut(FieldSystem *fieldSystem);
void HoneySweetScent_LoadBg2Tint(BgConfig *bgConfig);
void HoneySweetScentFade_Set(HoneySweetScentFade *anim, int start, int end, int frames);
BOOL HoneySweetScentFade_Step(HoneySweetScentFade *anim);
void HoneySweetScent_SetBg2MaskBlack(FieldSystem *fieldSystem);
void HoneySweetScent_SetBg2MaskWhite(FieldSystem *fieldSystem);
void HoneySweetScent_ShowWhiteBg2(FieldSystem *fieldSystem);
void HoneySweetScent_ShowWhiteBg3(FieldSystem *fieldSystem);
int HoneySweetScent_UseWhiteFlash(FieldSystem *fieldSystem);
BOOL HoneySweetScent_IsWeatherSuitable(FieldSystem *fieldSystem);
void HoneySweetScentEnv_Free(HoneySweetScentEnv *env);

// Weather in which Honey and Sweet Scent fail.
const u32 gHoneySweetScentBlockingWeather[] = { WEATHER_RAIN, WEATHER_SNOW, WEATHER_UNKNOWN_9 };

void HoneySweetScentFade_InitIn(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    HoneySweetScentFade *anim = data;
    HoneySweetScent_LoadBg2Tint(fieldSystem->bgConfig);
    HoneySweetScentFade_Set(anim, 0, 10, 19);
    anim->done = 0;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD, 0, 16);
    SetBgPriority(2, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    PlaySE(SEQ_SE_DP_FW230);
}

void HoneySweetScentFade_InitOut(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    HoneySweetScentFade *anim = data;
    HoneySweetScent_LoadBg2Tint(fieldSystem->bgConfig);
    HoneySweetScentFade_Set(anim, 10, 0, 15);
    anim->done = 0;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 10, 6);
}

void HoneySweetScentFade_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
}

void HoneySweetScentFade_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    HoneySweetScentFade *anim = data;
    if (anim->done != TRUE) {
        if (HoneySweetScentFade_Step(anim)) {
            anim->done = TRUE;
        }
        reg_G2_BLDALPHA = anim->cur | ((16 - anim->cur) << 8);
    }
}

void HoneySweetScentFade_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
}

const Field3dObjectTaskTemplate gHoneySweetScentFadeInTemplate = {
    0x400,
    sizeof(HoneySweetScentFade),
    HoneySweetScentFade_InitIn,
    HoneySweetScentFade_Destroy,
    HoneySweetScentFade_Update,
    HoneySweetScentFade_Render,
};

const Field3dObjectTaskTemplate gHoneySweetScentFadeOutTemplate = {
    0x400,
    sizeof(HoneySweetScentFade),
    HoneySweetScentFade_InitOut,
    HoneySweetScentFade_Destroy,
    HoneySweetScentFade_Update,
    HoneySweetScentFade_Render,
};

Field3dObjectTask *HoneySweetScent_StartFadeIn(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->mapSystems->field3dObjectTaskManager, &gHoneySweetScentFadeInTemplate);
}

void HoneySweetScent_DeleteFade(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}

int HoneySweetScent_IsFadeDone(Field3dObjectTask *task) {
    HoneySweetScentFade *anim = Field3dObjectTask_GetData(task);
    return anim->done;
}

Field3dObjectTask *HoneySweetScent_StartFadeOut(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->mapSystems->field3dObjectTaskManager, &gHoneySweetScentFadeOutTemplate);
}

void HoneySweetScent_LoadBg2Tint(BgConfig *bgConfig) {
    u16 color = 0x5D5F;
    void *buf;
    BG_LoadPlttData(2, &color, 2, 0xc2);
    buf = Heap_Alloc(HEAP_ID_FIELD1, 0x20);
    memset(buf, 0x11, 0x20);
    BG_LoadCharTilesData(bgConfig, 2, buf, 0x20, 1);
    Heap_Free(buf);
    BgFillTilemapBufferAndCommit(bgConfig, 2, 0x6001);
}

void HoneySweetScentFade_Set(HoneySweetScentFade *anim, int start, int end, int frames) {
    anim->cur = start;
    anim->start = start;
    anim->delta = end - start;
    anim->max = frames;
    anim->counter = 0;
}

BOOL HoneySweetScentFade_Step(HoneySweetScentFade *anim) {
    int offset = anim->delta * anim->counter / anim->max;
    anim->cur = offset + anim->start;
    if (anim->counter + 1 <= anim->max) {
        anim->counter = anim->counter + 1;
        return FALSE;
    }
    anim->counter = anim->max;
    return TRUE;
}

void HoneySweetScent_SetBg2MaskBlack(FieldSystem *fieldSystem) {
    BG_SetMaskColor(2, 0);
}

void HoneySweetScent_SetBg2MaskWhite(FieldSystem *fieldSystem) {
    BG_SetMaskColor(2, 0x7FFF);
}

void HoneySweetScent_ShowWhiteBg2(FieldSystem *fieldSystem) {
    u16 color = 0x7FFF;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    reg_G2_BG2CNT = (reg_G2_BG2CNT & ~3) | 3;
    BG_LoadPlttData(2, &color, 2, 0xc4);
    BG_FillCharDataRange(fieldSystem->bgConfig, GF_BG_LYR_MAIN_2, 2, 1, 2);
    BgFillTilemapBufferAndCommit(fieldSystem->bgConfig, 2, 0x6002);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    HoneySweetScent_SetBg2MaskBlack(fieldSystem);
}

void HoneySweetScent_ShowWhiteBg3(FieldSystem *fieldSystem) {
    u16 color = 0x7FFF;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    reg_G2_BG3CNT = (reg_G2_BG3CNT & ~3) | 3;
    BG_LoadPlttData(3, &color, 2, 0xc4);
    BG_FillCharDataRange(fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 2, 1, 2);
    BgFillTilemapBufferAndCommit(fieldSystem->bgConfig, 3, 0x6002);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    HoneySweetScent_SetBg2MaskBlack(fieldSystem);
}

// Never on in this game: the white BG flash after the darkening is unused.
int HoneySweetScent_UseWhiteFlash(FieldSystem *fieldSystem) {
    return 0;
}

BOOL HoneySweetScent_IsWeatherSuitable(FieldSystem *fieldSystem) {
    int i;
    u32 value = WeatherManager_GetWeather(fieldSystem->mapSystems->weatherManager);
    for (i = 0; i < 3; i++) {
        if (value == gHoneySweetScentBlockingWeather[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

void HoneySweetScentEnv_Free(HoneySweetScentEnv *env) {
    if (env->task != NULL) {
        HoneySweetScent_DeleteFade(env->task);
        env->task = NULL;
    }
    Heap_Free(env);
}

u32 GetHoneySweetScentWorkSize(void) {
    return sizeof(HoneySweetScentEnv);
}

BOOL Task_HoneyOrSweetScent(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    HoneySweetScentEnv *env = TaskManager_GetEnvironment(taskManager);
    u32 x;
    u32 z;
    u8 behavior;

    switch (env->state) {
    case HONEY_SWEET_SCENT_STATE_CHECK_WEATHER:
        if (HoneySweetScent_IsWeatherSuitable(fieldSystem) == TRUE) {
            env->state = HONEY_SWEET_SCENT_STATE_FADE_IN;
            env->useWhiteFlash = HoneySweetScent_UseWhiteFlash(fieldSystem);
            if (env->useWhiteFlash != 0) {
                HoneySweetScent_SetBg2MaskWhite(fieldSystem);
            }
        } else {
            env->state = HONEY_SWEET_SCENT_STATE_WAIT_BAD_WEATHER;
            env->timer = 20;
        }
        break;
    case HONEY_SWEET_SCENT_STATE_FADE_IN:
        env->task = HoneySweetScent_StartFadeIn(fieldSystem);
        env->state = HONEY_SWEET_SCENT_STATE_WAIT_FADE_IN;
        SetBgPriority(2, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        break;
    case HONEY_SWEET_SCENT_STATE_WAIT_FADE_IN:
        if (HoneySweetScent_IsFadeDone(env->task) != 0) {
            env->timer = 22;
            env->state = HONEY_SWEET_SCENT_STATE_WAIT_ENCOUNTER;
        }
        break;
    case HONEY_SWEET_SCENT_STATE_WAIT_ENCOUNTER:
        env->timer--;
        if (MapHeader_HasWildEncounters(fieldSystem->location->mapId)) {
            if (env->timer < 0) {
                x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
                z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
                behavior = GetMetatileBehavior(fieldSystem, x, z);
                if (FieldSystem_CanGenerateStepEncounter(fieldSystem, behavior)) {
                    env->state = HONEY_SWEET_SCENT_STATE_ENCOUNTER;
                } else {
                    env->state = HONEY_SWEET_SCENT_STATE_FADE_OUT;
                }
            }
        } else {
            env->state = HONEY_SWEET_SCENT_STATE_FADE_OUT;
        }
        break;
    case HONEY_SWEET_SCENT_STATE_FADE_OUT:
        HoneySweetScent_DeleteFade(env->task);
        env->task = HoneySweetScent_StartFadeOut(fieldSystem);
        env->state = HONEY_SWEET_SCENT_STATE_WAIT_FADE_OUT;
        break;
    case HONEY_SWEET_SCENT_STATE_WAIT_FADE_OUT:
        if (HoneySweetScent_IsFadeDone(env->task) != 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            reg_G2_BLDCNT = 0;
            SetBgPriority(2, 3);
            env->state = HONEY_SWEET_SCENT_STATE_NOTHING_HAPPENED;
        }
        break;
    case HONEY_SWEET_SCENT_STATE_ENCOUNTER:
        HoneySweetScentEnv_Free(env);
        if (env->useWhiteFlash != 0) {
            HoneySweetScent_ShowWhiteBg3(fieldSystem);
        }
        GF_ASSERT(FieldSystem_PerformSweetScentEncounterCheck(fieldSystem, taskManager));
        break;
    case HONEY_SWEET_SCENT_STATE_WAIT_BAD_WEATHER:
        env->timer--;
        if (env->timer < 0) {
            QueueScript(taskManager, 2019, NULL, NULL); // the weather is not suitable
            env->state = HONEY_SWEET_SCENT_STATE_END;
        }
        break;
    case HONEY_SWEET_SCENT_STATE_NOTHING_HAPPENED:
        QueueScript(taskManager, 2018, NULL, NULL); // nothing happened
        env->state = HONEY_SWEET_SCENT_STATE_END;
        break;
    case HONEY_SWEET_SCENT_STATE_END:
        HoneySweetScentEnv_Free(env);
        if (env->useWhiteFlash != 0) {
            HoneySweetScent_ShowWhiteBg2(fieldSystem);
        }
        BgFillTilemapBufferAndCommit(fieldSystem->bgConfig, 2, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        return TRUE;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}
