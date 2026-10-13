#include "field/weather_manager.h"

#include "constants/weather.h"

#include "field_system.h"
#include "geometry_math.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "field/field_flash_darkness.h"
#include "sprite_transfer.h"
#include "sound_playback.h"
#include "oam_manager.h"
#include "unk_0200FA24.h"
#include "gf_g2d_renderer.h"

static void WeatherTask_ChangeWeather(SysTask *task, void *data);
static void WeatherTask_ChangeWeatherOverlapped(SysTask *task, void *data);
static BOOL Weather_ShouldOverlapChange(u32 weather, u32 nextWeather);
static void WeatherDraw_Init(WeatherDraw *weatherDraw);
static void WeatherDrawTask_RendAndAnimateSprites(SysTask *task, void *data);
static void Weather_Init2DGfxRes(GF_2DGfxResHeader *headerList, u32 a1, u32 a2);
static void WeatherDraw_Delete(WeatherDraw *weatherDraw);
static void WeatherDraw_SetSpriteMatrix(Sprite *sprite, VecFx32 *matrix);
static WeatherSystem *WeatherSystem_New(FieldSystem *fieldSystem);
static void WeatherSystem_Delete(WeatherSystem **pWeatherSystem);
static BOOL WeatherSystem_Process(WeatherSystem *weatherSystem, int state, int weather);
static u16 WeatherSystem_GetState(WeatherSystem *weatherSystem, int weather);
static void Weather_SetBlendAlpha(u32 arg0, u32 arg1);
static void WeatherLerp_Init(WeatherLerp *arg0, s32 arg1, s32 arg2, s32 arg3);
static BOOL WeatherLerp_Step(WeatherLerp *a0);
static void WeatherSystem_LoadSpriteResources(WeatherSystem *weatherSystem, s32 arg1, WeatherSpriteResources *arg2);
static SpriteResource *Weather_AddResObjFromOpenNarc(GF_2DGfxResHeader *headerList, s32 resourceType, s32 headerIndex, GF_2DGfxResMan *resMan, NARC *narc, BOOL atEnd);
static void WeatherSystem_FreeSpriteResources(WeatherSystem *weatherSystem, s32 a1, WeatherSpriteResources *a2);
static BOOL WeatherSystem_Init(WeatherSystem *weatherSystem, int weather);
static BOOL WeatherSystem_LoadAsync(WeatherSystem *weatherSystem, int weather);
static BOOL WeatherSystem_Start(WeatherSystem *weatherSystem, int weather, u32 a2, u32 a3);
static void WeatherSystem_RequestStop(WeatherSystem *weatherSystem, int weather, u16 a2);
static void WeatherSystem_StopNow(WeatherSystem *weatherSystem, int weather);
static void WeatherSystem_Unload(WeatherSystem *weatherSystem, u32 weather);
static void WeatherEntry_EndTask(WeatherEntry *a0);
static void WeatherSystem_LoadBgGfx(WeatherSystem *weatherSystem, u16 a1);
static BOOL WeatherSystem_LoadSprites(WeatherSystem *weatherSystem, WeatherEntry *a1);
static void WeatherTask_LoadAsync(SysTask *task, void *data);
static BOOL WeatherSystem_AllocWork(WeatherSystem *weatherSystem, WeatherEntry *a1);
static BOOL WeatherEntry_AllocSpriteResources(WeatherEntry *a0);
static void WeatherSystem_LoadCellResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *a2);
static void WeatherSystem_LoadAnimResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *a2);
static void WeatherSystem_LoadCharResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *a2);
static void WeatherSystem_LoadPlttResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *a2);
static void WeatherSystem_InitSpriteTemplate(WeatherSystem *weatherSystem, WeatherEntry *a1);
static void WeatherWork_CreateSprites(WeatherWork *a0);
static void WeatherWork_DeleteSprites(WeatherWork *a0);
static void WeatherSystem_LoadBgPalette(WeatherSystem *weatherSystem, u16 a1);
static void WeatherSystem_LoadBgChars(WeatherSystem *weatherSystem, u16 a1);
static void WeatherSystem_LoadBgScreen(WeatherSystem *weatherSystem, u16 a1);
static void WeatherObject_InitData(WeatherObject *a0, int a1);
static void WeatherObject_ClearData(WeatherObject *a0);
static void *WeatherSystem_AddObject(WeatherWork *a0, int a1);
static void WeatherSystem_InitSpriteResourcesHeader(SpriteResourcesHeader *header, WeatherSystem *weatherSystem, WeatherSpriteResources *a2, u32 a3, u32 a4);
static void WeatherObject_Delete(WeatherObject *a0);
static void WeatherObject_DeleteAll(WeatherObject *a0);
static void WeatherObject_ForEach(WeatherObject *a0, WeatherObjectFunc func);
static void WeatherWork_Nop(void *data);
static VecFx32 WeatherObject_GetPosition(WeatherObject *a0);
static void WeatherWork_GetCameraScroll(fx32 *x, fx32 *z, WeatherWork *a0);
static void WeatherWork_ScrollObjectsInt(WeatherWork *a0, int *xOut, int *zOut);
static void WeatherWork_ScrollObjects(WeatherWork *a0, fx32 *x, fx32 *y);
static void WeatherSpawnRamp_Init(WeatherSpawnRamp *arg0, WeatherWork *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, WeatherSpawnFunc arg9);
static void WeatherSpawnRamp_SetTarget(WeatherSpawnRamp *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4);
static s32 WeatherSpawnRamp_Update(WeatherSpawnRamp *arg0);
static void WeatherFog_Start(WeatherFogChange *arg0, WeatherFogTableFade *arg1, FogData *fog, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb, s32 arg6, s32 arg7);
static BOOL WeatherFog_Update(WeatherFogChange *arg0, WeatherFogTableFade *arg1, s32 arg2);
static void Weather_SetFog(FogData *fog, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb);
static void WeatherFogChange_Init(WeatherFogChange *fogChange, FogData *fog, s32 arg2, s32 arg3, GXRgb rgb, s32 arg5);
static BOOL WeatherFogChange_Step(WeatherFogChange *arg0);
static void WeatherFogTableFade_Clear(WeatherFogTableFade *a0);
static void WeatherFogTableFade_Start(WeatherFogTableFade *arg0, s32 arg1, s32 arg2);
static s32 WeatherFogTableFade_Update(WeatherFogTableFade *arg0);
static void WeatherFogTableFade_SetFull(WeatherFogTableFade *arg0);
static s32 WeatherFogTableFade_Step(WeatherFogTableFade *arg0);
static void WeatherFogTableFade_BuildTable(WeatherFogTableFade *arg0);
static void Weather_SpawnAndPreAdvance(WeatherWork *a0, WeatherSpawnFunc a1, int a2, int a3, int a4, WeatherObjectFunc a5);
static WeatherObject *WeatherSystem_GetFreeObject(WeatherWork *a0);
static void WeatherTask_Sunny(SysTask *task, void *data);
static void WeatherTask_Rain(SysTask *task, void *data);
static void Rain_AddDrops(WeatherWork *a0, int arg1);
static void Rain_UpdateDrop(WeatherObject *a0);
static void WeatherTask_Snow(SysTask *task, void *data);
static void Snow_AddFlakes(WeatherWork *a0, int a1);
static void Snow_UpdateFlake(WeatherObject *a0);
static void WeatherTask_DiamondDust(SysTask *task, void *data);
static void DiamondDust_AddSparkles(WeatherWork *a0, int a1);
static void DiamondDust_UpdateSparkle(WeatherObject *a0);
static void WeatherTask_RunFogOnly(WeatherWork *a0, WeatherFogTaskData *a1, GXFogSlope fogSlope, s32 arg3, GXRgb arg4, s32 arg5, s32 arg6);
static void WeatherTask_Mist(SysTask *task, void *data);
static void WeatherTask_Flash(SysTask *task, void *data);
static void WeatherTask_Flash2(SysTask *task, void *data);
static void WeatherTask_LowLight(SysTask *task, void *data);
static void WeatherTask_DelayedRainSE(SysTask *task, void *data);
static void Weather_PlayLoopSE(WeatherWork *a0, u32 sndSeq);
static void Weather_StopLoopSE(WeatherWork *arg0);

struct WeatherWork {
    WeatherSystem *weatherSystem;
    WeatherEntry *entry;
    WeatherSpriteResources *spriteResources;
    WeatherObject linkedListDummy;
    WeatherObject linkedList[64];
    SysTask *weatherTask;
    VecFx32 cameraTarget;
    void *taskData;
    u32 loopSEPlaying;
    u16 sndSeq;
    u16 state;
    u16 fogMode;
    u16 stopRequest;
    s32 seDelay;
    SysTask *seDelayTask;
};

struct WeatherEntry {
    u16 spriteResId;
    u16 bgGfxId;
    int taskDataSize;
    WeatherWork *work;
    WeatherSpriteResources *weatherSpriteResources;
    u16 state;
    u16 loadStep;
    SysTask *task;
    SysTaskFunc taskFunc;
};

struct WeatherSystem {
    WeatherEntry *entries;
    const WeatherGfxNarcData *bgGfx;
    WeatherDraw weatherDraw;
    FieldSystem *fieldSystem;
    NARC *narc;
};

void FieldTextureManager_KeepTextures(FieldTextureManager *a0) {
    a0->keepTextures = 1;
}

/**
 * @brief Creates the weather manager and initializes with default values
 *
 * @param fieldSystem
 *
 * @return WeatherManager *
 */
WeatherManager *WeatherManager_New(FieldSystem *fieldSystem) {
    WeatherManager *weatherManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(WeatherManager));

    memset(weatherManager, 0, sizeof(WeatherManager));

    weatherManager->weatherSystem = WeatherSystem_New(fieldSystem);
    weatherManager->weather = WEATHER_SUNNY;
    weatherManager->nextWeather = WEATHER_SUNNY;
    weatherManager->state = 6;
    weatherManager->task = NULL;
    weatherManager->queuedWeather = WEATHER_MAX;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);

    return weatherManager;
}

/**
 * @brief Deletes and frees the weather manager and embedded structs
 *
 * @param weatherManager
 */
void WeatherManager_Delete(WeatherManager *weatherManager) {
    if (weatherManager->task) {
        SysTask_Destroy(weatherManager->task);
    }

    WeatherSystem_Delete(&weatherManager->weatherSystem);

    memset(weatherManager, 0, sizeof(WeatherManager));

    Heap_Free(weatherManager);
}

void WeatherManager_SetWeather(WeatherManager *weatherManager, int weather) {
    GF_ASSERT(weatherManager->state == 6);
    GF_ASSERT(weather < WEATHER_MAX);

    if (weatherManager->weather != weather) {
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 8, weatherManager->weather));
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 0, weather));
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 3, weather));
        weatherManager->weather = weather;
    }
}

BOOL WeatherManager_ChangeWeather(WeatherManager *weatherManager, s32 weather) {
    GF_ASSERT(weather < WEATHER_MAX);

    if (weatherManager->state != 6) {
        weatherManager->queuedWeather = weather;
        return TRUE;
    }

    if (weatherManager->weather == weather) {
        return TRUE;
    }

    GF_ASSERT(weatherManager->task == NULL);

    weatherManager->nextWeather = weather;
    if (Weather_ShouldOverlapChange(weatherManager->weather, weatherManager->nextWeather) == 0) {
        weatherManager->state = 0;
        weatherManager->task = SysTask_CreateOnMainQueue(WeatherTask_ChangeWeather, weatherManager, 0);
    } else {
        weatherManager->state = 0;
        weatherManager->task = SysTask_CreateOnMainQueue(WeatherTask_ChangeWeatherOverlapped, weatherManager, 0);
    }

    return TRUE;
}

u32 WeatherManager_GetWeather(WeatherManager *weatherManager) {
    return weatherManager->weather;
}

static void WeatherTask_ChangeWeather(SysTask *task, void *data) {
    WeatherManager *weatherManager = data;
    switch (weatherManager->state) {
    case 0:
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 5, weatherManager->weather));
        weatherManager->state++;
        break;
    case 1:
        if (WeatherSystem_GetState(weatherManager->weatherSystem, weatherManager->weather) != 3) {
            GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 8, weatherManager->weather));
            weatherManager->state++;
        }
        break;
    case 2:
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 1, weatherManager->nextWeather));
        weatherManager->state++;
        break;
    case 3:
        if (WeatherSystem_GetState(weatherManager->weatherSystem, weatherManager->nextWeather) != 1) {
            weatherManager->state++;
        }
        break;
    case 4:
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 2, weatherManager->nextWeather));
        weatherManager->state++;
        weatherManager->weather = weatherManager->nextWeather;
        weatherManager->nextWeather = 0;
        break;
    case 5:
        weatherManager->state = 6;
        weatherManager->task = NULL;
        SysTask_Destroy(task);
        if (weatherManager->queuedWeather != 14) {
            WeatherManager_ChangeWeather(weatherManager, weatherManager->queuedWeather);
            weatherManager->queuedWeather = 14;
        }
        break;
    }
}

static void WeatherTask_ChangeWeatherOverlapped(SysTask *task, void *data) {
    WeatherManager *weatherManager = data;
    switch (weatherManager->state) {
    case 0:
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 1, weatherManager->nextWeather));
        weatherManager->state++;
        break;
    case 1:
        if (WeatherSystem_GetState(weatherManager->weatherSystem, weatherManager->nextWeather) != 1) {
            weatherManager->state++;
        }
        break;
    case 2:
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 7, weatherManager->weather));
        GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 4, weatherManager->nextWeather));
        weatherManager->state++;
        break;
    case 3:
        if (WeatherSystem_GetState(weatherManager->weatherSystem, weatherManager->weather) != 3) {
            GF_ASSERT(WeatherSystem_Process(weatherManager->weatherSystem, 8, weatherManager->weather));
            weatherManager->state++;
        }
        break;
    case 4:
        weatherManager->weather = weatherManager->nextWeather;
        weatherManager->nextWeather = 0;
        weatherManager->state = 6;
        weatherManager->task = NULL;
        SysTask_Destroy(task);
        if (weatherManager->queuedWeather != 14) {
            WeatherManager_ChangeWeather(weatherManager, weatherManager->queuedWeather);
            weatherManager->queuedWeather = 14;
        }
        break;
    }
}

static BOOL Weather_ShouldOverlapChange(u32 weather, u32 nextWeather) {
    return FALSE;
}

static void WeatherDraw_Init(WeatherDraw *weatherDraw) {
    GF_InitG2dRenderer(&weatherDraw->instance, 0xFFFFF000);

    NNSG2dViewRect rect;
    rect.posTopLeft.x = 0;
    rect.posTopLeft.y = 0;
    rect.sizeView.x = 0xFF000;
    rect.sizeView.y = 0xC0000;

    OamManager_InitRenderSurface(&weatherDraw->surface, &rect, NNS_G2D_VRAM_TYPE_2DMAIN, &weatherDraw->instance);

    for (int i = 0; i < 4; i++) {
        weatherDraw->resMan[i] = Create2DGfxResObjMan(14, (GfGfxResType)i, HEAP_ID_FIELD1);
    }

    weatherDraw->header = Heap_Alloc(HEAP_ID_FIELD1, 4 * GF2DGfxResHeader_sizeof());

    Weather_Init2DGfxRes(weatherDraw->header, 0, 0x39);
    Weather_Init2DGfxRes(weatherDraw->header, 1, 0x3A);
    Weather_Init2DGfxRes(weatherDraw->header, 2, 0x37);
    Weather_Init2DGfxRes(weatherDraw->header, 3, 0x38);

    SpriteListParam param;
    param.num = 0x40;
    param.rendererInstance = &weatherDraw->instance;
    param.heapID = HEAP_ID_FIELD1;

    weatherDraw->spriteList = SpriteList_Create(&param);

    weatherDraw->task = SysTask_CreateOnMainQueue(WeatherDrawTask_RendAndAnimateSprites, weatherDraw, 10);
}

static void WeatherDrawTask_RendAndAnimateSprites(SysTask *task, void *data) {
    WeatherDraw *weatherDraw = data;
    SpriteList_RenderAndAnimateSprites(weatherDraw->spriteList);
}

static void Weather_Init2DGfxRes(GF_2DGfxResHeader *headerList, u32 a1, u32 a2) {
    GF_2DGfxResHeader *header = GF2DGfxResHeader_GetByIndex(headerList, a1);
    GF_2DGfxResHeaderNarcList *headerNarcList = GfGfxLoader_LoadFromNarc(NARC_a_0_6_3, a2, 0, HEAP_ID_FIELD1, 1);
    GF2DGfxResHeader_Init(headerNarcList, header, HEAP_ID_FIELD1);
    Heap_Free(headerNarcList);
}

static void WeatherDraw_Delete(WeatherDraw *weatherDraw) {
    for (int i = 0; i < 4; i++) {
        GF2DGfxResHeader_Reset(GF2DGfxResHeader_GetByIndex(weatherDraw->header, i));
        Destroy2DGfxResObjMan(weatherDraw->resMan[i]);
    }

    Heap_Free(weatherDraw->header);
    weatherDraw->header = NULL;

    SpriteList_Delete(weatherDraw->spriteList);
    weatherDraw->spriteList = NULL;

    SysTask_Destroy(weatherDraw->task);
    weatherDraw->task = NULL;
}

static void WeatherDraw_SetSpriteMatrix(Sprite *sprite, VecFx32 *matrix) {
    if (matrix->x > 0x13F000) {
        matrix->x %= 0x13F000;
    } else if (matrix->x < (int)0xFFFC0000) {
        matrix->x += 0x13F000;
    }

    if (matrix->y > 0x100000) {
        matrix->y %= 0x100000;
    } else if (matrix->y < (int)0xFFFC0000) {
        matrix->y += 0x100000;
    }

    Sprite_SetMatrix(sprite, matrix);
}

static WeatherEntry sWeatherEntries[] = {
    {
     .spriteResId = 0xFFFF,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 4,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Sunny,
     },
    {
     .spriteResId = 0,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0xDC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Rain,
     },
    {
     .spriteResId = 0,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0xDC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Rain,
     },
    {
     .spriteResId = 0,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0xDC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Rain,
     },
    {
     .spriteResId = 1,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0xDC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Snow,
     },
    {
     .spriteResId = 1,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0xDC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Snow,
     },
    {
     .spriteResId = 1,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0xDC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Snow,
     },
    {
     .spriteResId = 0xFFFF,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 4,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Sunny,
     },
    {
     .spriteResId = 4,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0xDC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_DiamondDust,
     },
    {
     .spriteResId = 0xFFFF,
     .bgGfxId = 6,
     .taskDataSize = 0xAC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Mist,
     },
    {
     .spriteResId = 0xFFFF,
     .bgGfxId = 6,
     .taskDataSize = 0xAC,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Mist,
     },
    {
     .spriteResId = 0xFFFF,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0x634,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Flash,
     },
    {
     .spriteResId = 0xFFFF,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0x634,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_Flash2,
     },
    {
     .spriteResId = 0xFFFF,
     .bgGfxId = 0xFFFF,
     .taskDataSize = 0xC0,
     .work = NULL,
     .weatherSpriteResources = NULL,
     .state = 0,
     .loadStep = 0,
     .task = NULL,
     .taskFunc = WeatherTask_LowLight,
     },
};

static const WeatherGfxNarcData sWeatherBgGfx[] = {
    {
     .paletteId = 0x15,
     .charId = 0x24,
     .screenId = 0x26,
     },
    {
     .paletteId = 0x25,
     .charId = 0x24,
     .screenId = 0x26,
     },
    {
     .paletteId = 0x9,
     .charId = 0x8,
     .screenId = 0xA,
     },
    {
     .paletteId = 0x2A,
     .charId = 0x2C,
     .screenId = 0x2B,
     },
    {
     .paletteId = 0x1E,
     .charId = 0x1D,
     .screenId = 0x1F,
     },
    {
     .paletteId = 0x2E,
     .charId = 0x2D,
     .screenId = 0x2F,
     },
    {
     .paletteId = 0x33,
     .charId = 0x2D,
     .screenId = 0x2F,
     },
    {
     .paletteId = 0x34,
     .charId = 0x35,
     .screenId = 0x36,
     },
    {
     .paletteId = 0x31,
     .charId = 0x30,
     .screenId = 0x32,
     },
};

static WeatherSystem *WeatherSystem_New(FieldSystem *fieldSystem) {
    WeatherSystem *weatherSystem = Heap_Alloc(HEAP_ID_FIELD1, sizeof(WeatherSystem));

    weatherSystem->fieldSystem = fieldSystem;

    WeatherDraw_Init(&weatherSystem->weatherDraw);

    weatherSystem->entries = sWeatherEntries;
    weatherSystem->bgGfx = sWeatherBgGfx;

    weatherSystem->narc = NARC_New(NARC_a_0_6_3, HEAP_ID_FIELD1);

    return weatherSystem;
}

static void WeatherSystem_Delete(WeatherSystem **pWeatherSystem) {
    if (*pWeatherSystem != NULL) {
        for (int i = 0; i < 14; i++) {
            WeatherSystem_Unload(*pWeatherSystem, i);
        }

        Fog_Set((*pWeatherSystem)->fieldSystem->fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);

        reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 1;

        GfGfx_EngineATogglePlanes(4, 0);

        WeatherDraw_Delete(&(*pWeatherSystem)->weatherDraw);

        NARC_Delete((*pWeatherSystem)->narc);
        Heap_FreeExplicit(HEAP_ID_FIELD1, *pWeatherSystem);

        *pWeatherSystem = NULL;
    }
}

static BOOL WeatherSystem_Process(WeatherSystem *weatherSystem, int state, int weather) {
    BOOL ret = TRUE;

    if (weather > 14) {
        return FALSE;
    }

    switch (state) {
    case WEATHER_SYS_PROC_INIT:
        ret = WeatherSystem_Init(weatherSystem, weather);
        break;
    case 1:
        ret = WeatherSystem_LoadAsync(weatherSystem, weather);
        break;
    case 2:
        ret = WeatherSystem_Start(weatherSystem, weather, 0, 1);
        break;
    case 3:
        ret = WeatherSystem_Start(weatherSystem, weather, 2, 1);
        break;
    case 4:
        if (Fog_IsEnabled(weatherSystem->fieldSystem->fog) == 1) {
            ret = WeatherSystem_Start(weatherSystem, weather, 0, 2);
        } else {
            ret = WeatherSystem_Start(weatherSystem, weather, 0, 1);
        }
        break;
    case 5:
        WeatherSystem_RequestStop(weatherSystem, weather, 1);
        break;
    case 6:
        WeatherSystem_StopNow(weatherSystem, weather);
        break;
    case 7:
        WeatherSystem_RequestStop(weatherSystem, weather, 0);
        break;
    case 8:
        WeatherSystem_Unload(weatherSystem, weather);
        break;
    case 9:
        WeatherEntry *v1 = &weatherSystem->entries[weather];
        if (v1->work && v1->work->loopSEPlaying == TRUE) {
            PlaySE(v1->work->sndSeq);
        }
        break;
    case 10:
        WeatherEntry *v2 = &weatherSystem->entries[weather];
        if (v2->work && v2->work->loopSEPlaying == TRUE) {
            StopSE(v2->work->sndSeq, 0);
        }
        break;
    default:
        break;
    }

    return ret;
}

static u16 WeatherSystem_GetState(WeatherSystem *weatherSystem, int weather) {
    if (weather >= 14) {
        return 0;
    }
    WeatherEntry *v0 = &weatherSystem->entries[weather];
    return v0->state;
}

static void Weather_SetBlendAlpha(u32 arg0, u32 arg1) {
    G2x_SetBlendAlpha_(0x04000050, 4, 0x21, arg0, arg1);
}

static void WeatherLerp_Init(WeatherLerp *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->value = arg1;
    arg0->start = arg1;
    arg0->delta = arg2 - arg1;
    arg0->duration = arg3;
    arg0->frame = 0;
}

static BOOL WeatherLerp_Step(WeatherLerp *a0) {
    int v0 = a0->delta * a0->frame;
    v0 /= a0->duration;

    a0->value = v0 + a0->start;

    if (a0->frame + 1 <= a0->duration) {
        a0->frame++;
        return FALSE;
    }

    a0->frame = a0->duration;
    return TRUE;
}

static void WeatherSystem_LoadSpriteResources(WeatherSystem *weatherSystem, s32 arg1, WeatherSpriteResources *arg2) {
    WeatherSystem_LoadCellResource(weatherSystem, arg1, arg2);
    WeatherSystem_LoadAnimResource(weatherSystem, arg1, arg2);
    WeatherSystem_LoadCharResource(weatherSystem, arg1, arg2);
    WeatherSystem_LoadPlttResource(weatherSystem, arg1, arg2);
}

static SpriteResource *Weather_AddResObjFromOpenNarc(GF_2DGfxResHeader *headerList, s32 resourceType, s32 headerIndex, GF_2DGfxResMan *resMan, NARC *narc, BOOL atEnd) {
    GF_2DGfxResHeader *header = GF2DGfxResHeader_GetByIndex(headerList, resourceType);
    u32 member = GF2DGfxResHeader_GetNarcMemberIdByIndex(header, headerIndex);
    u32 compressFlag = GF2DGfxResHeader_GetCompressFlagByIndex(header, headerIndex);
    u32 exDat0 = GF2DGfxResHeader_GetExDat0ByIndex(header, headerIndex);
    u32 exDat1 = GF2DGfxResHeader_GetExDat1ByIndex(header, headerIndex);
    u32 objID = GF2dGfxResHeader_GetObjIdByIndex(header, headerIndex);

    SpriteResource *ret;
    switch (resourceType) {
    case GF_GFX_RES_TYPE_CHAR:
        ret = AddCharResObjFromOpenNarcWithAtEndFlag(resMan, narc, member, compressFlag, objID, exDat0, HEAP_ID_FIELD1, atEnd);
        break;
    case GF_GFX_RES_TYPE_PLTT:
        ret = AddPlttResObjFromOpenNarcWithAtEndFlag(resMan, narc, member, compressFlag, objID, exDat0, exDat1, HEAP_ID_FIELD1, atEnd);
        break;
    case GF_GFX_RES_TYPE_CELL:
        ret = AddCellOrAnimResObjFromOpenNarc(resMan, narc, member, compressFlag, objID, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
        break;
    case GF_GFX_RES_TYPE_ANIM:
        ret = AddCellOrAnimResObjFromOpenNarc(resMan, narc, member, compressFlag, objID, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
        break;
    }

    return ret;
}

static void WeatherSystem_FreeSpriteResources(WeatherSystem *weatherSystem, s32 a1, WeatherSpriteResources *a2) {
    if (a1 != 0xFFFF) {
        if (a2->charResObj[GF_GFX_RES_TYPE_CHAR] != 0) {
            SpriteTransfer_DeleteCharTransferTask(a2->charResObj[GF_GFX_RES_TYPE_CHAR]);
        }
        if (a2->charResObj[GF_GFX_RES_TYPE_PLTT] != 0) {
            SpriteTransfer_DeletePlttTransferTask(a2->charResObj[GF_GFX_RES_TYPE_PLTT]);
        }

        for (int i = 0; i < 4; i++) {
            if (a2->charResObj[i] != 0) {
                DestroySingle2DGfxResObj(weatherSystem->weatherDraw.resMan[i], a2->charResObj[i]);
            }
        }
    }
}

static BOOL WeatherSystem_Init(WeatherSystem *weatherSystem, int weather) {
    WeatherEntry *v0 = &weatherSystem->entries[weather];
    if (v0->work == 0) {
        if (WeatherSystem_AllocWork(weatherSystem, v0) == FALSE) {
            return FALSE;
        }

        if (WeatherSystem_LoadSprites(weatherSystem, v0) == FALSE) {
            Heap_Free(v0->work);
            v0->work = NULL;
            return FALSE;
        }

        v0->work->spriteResources = v0->weatherSpriteResources;

        if (v0->spriteResId != 0xFFFF) {
            WeatherWork_CreateSprites(v0->work);
        }

        WeatherSystem_LoadBgGfx(weatherSystem, v0->bgGfxId);
        v0->state = 2;
    }

    return TRUE;
}

static BOOL WeatherSystem_LoadAsync(WeatherSystem *weatherSystem, int weather) {
    WeatherEntry *v0 = &weatherSystem->entries[weather];
    if (v0->work) {
        return TRUE;
    }

    if (WeatherSystem_AllocWork(weatherSystem, v0) == 0) {
        return FALSE;
    }

    v0->task = SysTask_CreateOnMainQueue(WeatherTask_LoadAsync, v0, 1);
    v0->state = 1;
    v0->loadStep = 0;

    return TRUE;
}

static BOOL WeatherSystem_Start(WeatherSystem *weatherSystem, int weather, u32 a2, u32 a3) {
    WeatherEntry *v0 = &weatherSystem->entries[weather];

    if (v0->spriteResId != 0xFFFF && v0->weatherSpriteResources == 0) {
        return FALSE;
    }

    if (v0->work == NULL) {
        return FALSE;
    }

    if (v0->state != 2) {
        return FALSE;
    }

    v0->work->weatherTask = SysTask_CreateOnMainQueue(v0->taskFunc, v0->work, 4);

    if (v0->work->weatherTask == NULL) {
        return FALSE;
    }

    v0->state = 3;
    v0->work->state = a2;
    v0->work->stopRequest = 0;
    v0->work->linkedListDummy.next = &v0->work->linkedListDummy;
    v0->work->linkedListDummy.prev = &v0->work->linkedListDummy;
    v0->work->fogMode = a3;
    v0->work->loopSEPlaying = 0;

    v0->work->cameraTarget = NNS_G3dGlb.camTarget;

    if (v0->taskDataSize > 0) {
        v0->work->taskData = Heap_Alloc(HEAP_ID_FIELD1, v0->taskDataSize);
        memset(v0->work->taskData, 0, v0->taskDataSize);
    } else {
        v0->work->taskData = NULL;
    }

    if (v0->bgGfxId != 0xFFFF) {
        GfGfx_EngineATogglePlanes(4, 0);
        G2_SetBG2Priority(1);
        G2_SetBG0Priority(2);
    }

    return TRUE;
}

static void WeatherSystem_RequestStop(WeatherSystem *weatherSystem, int weather, u16 a2) {
    WeatherEntry *v0 = &weatherSystem->entries[weather];
    if (v0->state == 3) {
        v0->work->stopRequest = 5;
        v0->work->fogMode = a2;
    }
}

static void WeatherSystem_StopNow(WeatherSystem *weatherSystem, int weather) {
    WeatherEntry *v0 = &weatherSystem->entries[weather];
    if (WeatherSystem_GetState(weatherSystem, weather) == 3) {
        v0->work->state = 5;
        v0->taskFunc(NULL, v0->work);
    }
}

static void WeatherSystem_Unload(WeatherSystem *weatherSystem, u32 weather) {
    WeatherEntry *v0 = &weatherSystem->entries[weather];

    if (v0->bgGfxId != 0xFFFF) {
        GfGfx_EngineATogglePlanes(4, 0);

        G2_SetBG2Priority(3);
        G2_SetBG0Priority(1);
        G2_BlendNone();
    }

    if (v0->weatherSpriteResources) {
        WeatherSystem_FreeSpriteResources(weatherSystem, v0->spriteResId, v0->weatherSpriteResources);
        Heap_FreeExplicit(HEAP_ID_FIELD1, v0->weatherSpriteResources);
        v0->weatherSpriteResources = NULL;
        if (v0->task) {
            SysTask_Destroy(v0->task);
            v0->task = NULL;
        }
    }

    if (v0->work) {
        WeatherObject_DeleteAll(&v0->work->linkedListDummy);

        if (v0->spriteResId != 0xFFFF) {
            WeatherWork_DeleteSprites(v0->work);
        }

        if (v0->work->loopSEPlaying == 1) {
            Weather_StopLoopSE(v0->work);
        }

        if (v0->work->taskData) {
            Heap_FreeExplicit(HEAP_ID_FIELD1, v0->work->taskData);
            v0->work->taskData = NULL;
        }

        if (v0->state == 1) {
            if (v0->task) {
                SysTask_Destroy(v0->task);
            }
        } else if (v0->state == 3) {
            SysTask_Destroy(v0->work->weatherTask);
        }

        if (v0->work->seDelayTask) {
            SysTask_Destroy(v0->work->seDelayTask);
        }

        Heap_FreeExplicit(HEAP_ID_FIELD1, v0->work);
        v0->work = NULL;
    }

    Fog_Set(weatherSystem->fieldSystem->fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
}

static void WeatherEntry_EndTask(WeatherEntry *a0) {
    if (a0->bgGfxId != 0xFFFF) {
        GfGfx_EngineATogglePlanes(4, 0);
        G2_SetBG2Priority(3);
        G2_SetBG0Priority(1);
        G2_BlendNone();
    }

    if (a0->work != NULL) {
        WeatherObject_DeleteAll(&a0->work->linkedListDummy);

        if (a0->work->loopSEPlaying == 1) {
            Weather_StopLoopSE(a0->work);
        }

        if (a0->state == 3) {
            SysTask_Destroy(a0->work->weatherTask);
            a0->state = 2;
        }
    }
}

static void WeatherSystem_LoadBgGfx(WeatherSystem *weatherSystem, u16 a1) {
    WeatherSystem_LoadBgPalette(weatherSystem, a1);
    WeatherSystem_LoadBgChars(weatherSystem, a1);
    WeatherSystem_LoadBgScreen(weatherSystem, a1);
}

static BOOL WeatherSystem_LoadSprites(WeatherSystem *weatherSystem, WeatherEntry *a1) {
    if (a1->spriteResId != 0xFFFF && a1->weatherSpriteResources == NULL) {
        if (WeatherEntry_AllocSpriteResources(a1) == FALSE) {
            return FALSE;
        }
        WeatherSystem_LoadSpriteResources(weatherSystem, a1->spriteResId, a1->weatherSpriteResources);
        WeatherSystem_InitSpriteTemplate(weatherSystem, a1);
    }
    return TRUE;
}

static void WeatherTask_LoadAsync(SysTask *task, void *data) {
    WeatherEntry *v0 = data;
    WeatherSystem *weatherSystem = v0->work->weatherSystem;
    switch (v0->loadStep) {
    case 0:
        GF_ASSERT(WeatherEntry_AllocSpriteResources(v0));
        v0->loadStep++;
        break;
    case 1:
        WeatherSystem_LoadCellResource(weatherSystem, v0->spriteResId, v0->weatherSpriteResources);
        v0->loadStep++;
        break;
    case 2:
        WeatherSystem_LoadBgPalette(weatherSystem, v0->bgGfxId);
        v0->loadStep++;
        break;
    case 3:
        WeatherSystem_LoadAnimResource(weatherSystem, v0->spriteResId, v0->weatherSpriteResources);
        v0->loadStep++;
        break;
    case 4:
        WeatherSystem_LoadBgChars(weatherSystem, v0->bgGfxId);
        v0->loadStep++;
        break;
    case 5:
        WeatherSystem_LoadCharResource(weatherSystem, v0->spriteResId, v0->weatherSpriteResources);
        v0->loadStep++;
        break;
    case 6:
        WeatherSystem_LoadBgScreen(weatherSystem, v0->bgGfxId);
        v0->loadStep++;
        break;
    case 7:
        WeatherSystem_LoadPlttResource(weatherSystem, v0->spriteResId, v0->weatherSpriteResources);
        v0->loadStep++;
        break;
    case 8:
        WeatherSystem_InitSpriteTemplate(weatherSystem, v0);
        v0->work->spriteResources = v0->weatherSpriteResources;
        if (v0->spriteResId != 0xFFFF) {
            WeatherWork_CreateSprites(v0->work);
        }
        v0->state = 2;
        v0->loadStep = 0;
        v0->task = 0;
        SysTask_Destroy(task);
    }
}

static BOOL WeatherSystem_AllocWork(WeatherSystem *weatherSystem, WeatherEntry *a1) {
    if (a1->work) {
        return TRUE;
    }

    a1->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(WeatherWork));
    if (a1->work == NULL) {
        return FALSE;
    }
    memset(a1->work, 0, sizeof(WeatherWork));

    a1->work->weatherSystem = weatherSystem;
    a1->work->state = 0;
    a1->work->stopRequest = 0;
    a1->work->linkedListDummy.next = &a1->work->linkedListDummy;
    a1->work->linkedListDummy.prev = &a1->work->linkedListDummy;
    a1->work->taskData = NULL;
    a1->work->entry = a1;
    a1->work->seDelayTask = NULL;

    return TRUE;
}

static BOOL WeatherEntry_AllocSpriteResources(WeatherEntry *a0) {
    if (a0->spriteResId != 0xFFFF) {
        if (a0->weatherSpriteResources != NULL) {
            return TRUE;
        }
        a0->weatherSpriteResources = Heap_Alloc(HEAP_ID_FIELD1, sizeof(WeatherSpriteResources));
        if (a0->weatherSpriteResources == NULL) {
            return FALSE;
        }
        memset(a0->weatherSpriteResources, 0, sizeof(WeatherSpriteResources));
    }
    return TRUE;
}

static void WeatherSystem_LoadCellResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *a2) {
    if (headerIndex != 0xFFFF) {
        a2->charResObj[GF_GFX_RES_TYPE_CELL] = Weather_AddResObjFromOpenNarc(weatherSystem->weatherDraw.header, GF_GFX_RES_TYPE_CELL, headerIndex, weatherSystem->weatherDraw.resMan[GF_GFX_RES_TYPE_CELL], weatherSystem->narc, 0);
    }
}

static void WeatherSystem_LoadAnimResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *a2) {
    if (headerIndex != 0xFFFF) {
        a2->charResObj[GF_GFX_RES_TYPE_ANIM] = Weather_AddResObjFromOpenNarc(weatherSystem->weatherDraw.header, GF_GFX_RES_TYPE_ANIM, headerIndex, weatherSystem->weatherDraw.resMan[GF_GFX_RES_TYPE_ANIM], weatherSystem->narc, 0);
    }
}

static void WeatherSystem_LoadCharResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *a2) {
    if (headerIndex != 0xFFFF) {
        a2->charResObj[GF_GFX_RES_TYPE_CHAR] = Weather_AddResObjFromOpenNarc(weatherSystem->weatherDraw.header, GF_GFX_RES_TYPE_CHAR, headerIndex, weatherSystem->weatherDraw.resMan[GF_GFX_RES_TYPE_CHAR], weatherSystem->narc, 1);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(a2->charResObj[GF_GFX_RES_TYPE_CHAR]);
        GF2DGfxResObj_FreeData(a2->charResObj[GF_GFX_RES_TYPE_CHAR]);
    }
}

static void WeatherSystem_LoadPlttResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *a2) {
    if (headerIndex != 0xFFFF) {
        a2->charResObj[GF_GFX_RES_TYPE_PLTT] = Weather_AddResObjFromOpenNarc(weatherSystem->weatherDraw.header, GF_GFX_RES_TYPE_PLTT, headerIndex, weatherSystem->weatherDraw.resMan[GF_GFX_RES_TYPE_PLTT], weatherSystem->narc, 1);
        SpriteTransfer_CreatePlttTransferTask(a2->charResObj[GF_GFX_RES_TYPE_PLTT]);
        GF2DGfxResObj_FreeData(a2->charResObj[GF_GFX_RES_TYPE_PLTT]);
    }
}

static void WeatherSystem_InitSpriteTemplate(WeatherSystem *weatherSystem, WeatherEntry *a1) {
    if (a1->spriteResId != 0xFFFF) {
        WeatherSystem_InitSpriteResourcesHeader(&a1->weatherSpriteResources->spriteResHeader, weatherSystem, a1->weatherSpriteResources, 0, 1);
        memset(&a1->weatherSpriteResources->spriteTemplate, 0, sizeof(SpriteTemplate));
        a1->weatherSpriteResources->spriteTemplate.spriteList = weatherSystem->weatherDraw.spriteList;
        a1->weatherSpriteResources->spriteTemplate.header = &a1->weatherSpriteResources->spriteResHeader;
        a1->weatherSpriteResources->spriteTemplate.scale.x = 1 << 12;
        a1->weatherSpriteResources->spriteTemplate.scale.y = 1 << 12;
        a1->weatherSpriteResources->spriteTemplate.scale.z = 1 << 12;
        a1->weatherSpriteResources->spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    }
}

static void WeatherWork_CreateSprites(WeatherWork *a0) {
    for (int i = 0; i < 64; i++) {
        a0->linkedList[i].sprite = Sprite_CreateAffine(&a0->spriteResources->spriteTemplate);
        Sprite_SetDrawFlag(a0->linkedList[i].sprite, 0);
        GF_ASSERT(a0->linkedList[i].sprite);
    }
}

static void WeatherWork_DeleteSprites(WeatherWork *a0) {
    for (int i = 0; i < 64; i++) {
        if (a0->linkedList[i].sprite) {
            Sprite_Delete(a0->linkedList[i].sprite);
            a0->linkedList[i].sprite = NULL;
        }
    }
}

static void WeatherSystem_LoadBgPalette(WeatherSystem *weatherSystem, u16 a1) {
    WeatherBgGfxFiles v0;
    if (a1 != 0xFFFF) {
        v0.paletteFile = NARC_AllocAndReadWholeMember(weatherSystem->narc, weatherSystem->bgGfx[a1].paletteId, HEAP_ID_FIELD1);
        NNS_G2dGetUnpackedPaletteData(v0.paletteFile, &v0.paletteData);
        BG_LoadPlttData(2, v0.paletteData->pRawData, 32, 0xc0);
        Heap_Free(v0.paletteFile);
        v0.paletteFile = NULL;
    }
}

static void WeatherSystem_LoadBgChars(WeatherSystem *weatherSystem, u16 a1) {
    WeatherBgGfxFiles v0;
    if (a1 != 0xFFFF) {
        v0.charFile = NARC_AllocAndReadWholeMember(weatherSystem->narc, weatherSystem->bgGfx[a1].charId, HEAP_ID_FIELD1);
        NNS_G2dGetUnpackedCharacterData(v0.charFile, &v0.charData);
        BG_LoadCharTilesData(weatherSystem->fieldSystem->bgConfig, 2, v0.charData->pRawData, v0.charData->szByte, 0);
        Heap_Free(v0.charFile);
        v0.charFile = NULL;
    }
}

static void WeatherSystem_LoadBgScreen(WeatherSystem *weatherSystem, u16 a1) {
    WeatherBgGfxFiles v0;
    if (a1 != 0xFFFF) {
        GfGfx_EngineATogglePlanes(4, 0);
        v0.screenFile = NARC_AllocAndReadWholeMember(weatherSystem->narc, weatherSystem->bgGfx[a1].screenId, HEAP_ID_FIELD1);
        GF_ASSERT(v0.screenFile);
        NNS_G2dGetUnpackedScreenData(v0.screenFile, &v0.screenData);
        BgCopyOrUncompressTilemapBufferRangeToVram(weatherSystem->fieldSystem->bgConfig, 2, v0.screenData->rawData, v0.screenData->szByte, 0);
        BG_LoadScreenTilemapData(weatherSystem->fieldSystem->bgConfig, 2, v0.screenData->rawData, v0.screenData->szByte);
        BgTilemapRectChangePalette(weatherSystem->fieldSystem->bgConfig, 2, 0, 0, 32, 32, 6);
        BgCommitTilemapBufferToVram(weatherSystem->fieldSystem->bgConfig, 2);
        Heap_Free(v0.screenFile);
        v0.screenFile = NULL;
    }
}

static void WeatherObject_InitData(WeatherObject *a0, int a1) {
    GF_ASSERT(a0->data == 0);
    GF_ASSERT(a1 > 0);
    GF_ASSERT(a1 <= 0x28u);
    a0->data = a0->dataBuffer;
}

static void WeatherObject_ClearData(WeatherObject *a0) {
    a0->data = NULL;
    memset(a0->dataBuffer, 0, 10 * sizeof(u32));
}

static void *WeatherSystem_AddObject(WeatherWork *a0, int a1) {
    WeatherSystem *weatherSystem = a0->weatherSystem;
    WeatherObject *v0 = WeatherSystem_GetFreeObject(a0);
    if (v0 == NULL) {
        return NULL;
    }
    v0->weatherSystem = weatherSystem;
    v0->next = &a0->linkedListDummy;
    v0->prev = a0->linkedListDummy.prev;
    a0->linkedListDummy.prev->next = v0;
    a0->linkedListDummy.prev = v0;

    WeatherObject_InitData(v0, a1);
    if (v0->data == NULL) {
        return NULL;
    }

    GF_ASSERT(v0->sprite);
    Sprite_SetDrawFlag(v0->sprite, 1);
    return v0;
}

static void WeatherSystem_InitSpriteResourcesHeader(SpriteResourcesHeader *header, WeatherSystem *weatherSystem, WeatherSpriteResources *a2, u32 a3, u32 a4) {
    int resID[4];

    for (int i = 0; i < 4; i++) {
        resID[i] = GF2DGfxResObj_GetResID(a2->charResObj[i]);
    }

    CreateSpriteResourcesHeader(header, resID[0], resID[1], resID[2], resID[3], -1, -1, a3, a4, weatherSystem->weatherDraw.resMan[0], weatherSystem->weatherDraw.resMan[1], weatherSystem->weatherDraw.resMan[2], weatherSystem->weatherDraw.resMan[3], NULL, NULL);
}

static void WeatherObject_Delete(WeatherObject *a0) {
    a0->prev->next = a0->next;
    a0->next->prev = a0->prev;

    Sprite_SetDrawFlag(a0->sprite, 0);
    WeatherObject_ClearData(a0);

    Sprite *temp = a0->sprite;
    memset(a0, 0, sizeof(WeatherObject));
    a0->sprite = temp;
}

static void WeatherObject_DeleteAll(WeatherObject *a0) {
    WeatherObject *cur = a0->next;
    WeatherObject *next;

    while (cur != a0) {
        next = cur->next;
        WeatherObject_Delete(cur);
        cur = next;
    }
}

static void WeatherObject_ForEach(WeatherObject *a0, WeatherObjectFunc func) {
    WeatherObject *cur = a0->next;
    WeatherObject *next = cur->next;

    while (cur != a0) {
        func(cur);
        cur = next;
        next = cur->next;
    }
}

static void WeatherWork_Nop(void *data) {
}

static VecFx32 WeatherObject_GetPosition(WeatherObject *a0) {
    return *Sprite_GetMatrixPtr(a0->sprite);
}

static void WeatherWork_GetCameraScroll(fx32 *x, fx32 *z, WeatherWork *a0) {
    VecFx32 cameraTarget = NNS_G3dGlb.camTarget;
    fx32 xScale, zScale;
    fx32 dx, dz;
    fx32 xDist, zDist;
    int v7;
    fx32 aspect;

    xDist = (cameraTarget.x - a0->cameraTarget.x);
    zDist = (cameraTarget.z - a0->cameraTarget.z);

    aspect = FX_Div(FX32_CONST(4), FX32_CONST(3));

    Math_CalcPerspectiveViewSize(Camera_GetPerspectiveAngle(a0->weatherSystem->fieldSystem->camera), Camera_GetDistance(a0->weatherSystem->fieldSystem->camera), aspect, &dx, &dz);

    dx = FX_Div(dx, 256 * FX32_ONE);

    if (zDist <= 0) {
        dz = FX_Div(dz, 0xbe8d0);
    } else {
        dz = FX_Div(dz, 0xbe811);
    }

    v7 = FX32_ONE;
    if (xDist < 0) {
        v7 = -FX32_ONE;
        xDist = FX_MUL(xDist, -FX32_ONE);
    }
    xScale = FX_Div(xDist, dx);
    if (v7 < 0) {
        xScale = FX_MUL(xScale, v7);
    }

    v7 = FX32_ONE;
    if (zDist < 0) {
        v7 = -FX32_ONE;
        zDist = FX_MUL(zDist, -FX32_ONE);
    }
    zScale = FX_Div(zDist, dz);
    if (v7 < 0) {
        zScale = FX_MUL(zScale, v7);
    }

    if (xScale + zScale != 0) {
        a0->cameraTarget = cameraTarget;
    }

    *x = xScale;
    *z = zScale;
}

static void WeatherWork_ScrollObjectsInt(WeatherWork *a0, int *xOut, int *zOut) {
    fx32 x, z;
    WeatherWork_ScrollObjects(a0, &x, &z);

    if (xOut) {
        *xOut = x >> FX32_SHIFT;
        if (*xOut < 0) {
            *xOut += FX32_ONE;
        }
    }
    if (zOut) {
        *zOut = z >> FX32_SHIFT;
        if (*zOut < 0) {
            *zOut += 1;
        }
    }
}

static void WeatherWork_ScrollObjects(WeatherWork *a0, fx32 *x, fx32 *y) {
    WeatherObject *cur;
    fx32 xScale, yScale;
    VecFx32 matrix;
    WeatherWork_GetCameraScroll(&xScale, &yScale, a0);

    cur = a0->linkedListDummy.next;
    while (cur != &a0->linkedListDummy) {
        matrix = WeatherObject_GetPosition(cur);

        matrix.x -= xScale;
        matrix.y -= yScale;

        WeatherDraw_SetSpriteMatrix(cur->sprite, &matrix);

        cur = cur->next;
    }

    if (x != NULL) {
        *x = xScale;
    }
    if (y != NULL) {
        *y = yScale;
    }
}

static void WeatherSpawnRamp_Init(WeatherSpawnRamp *arg0, WeatherWork *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, WeatherSpawnFunc arg9) {
    arg0->work = arg1;
    arg0->count = arg2;
    arg0->timer = 0;
    arg0->interval = arg3;
    arg0->countTimer = 0;
    arg0->targetCount = (s16)arg4;
    arg0->targetInterval = (s16)arg5;
    arg0->intervalStep = (s16)arg6;
    arg0->countStepSpawns = (s16)arg7;
    arg0->countStep = arg8;
    arg0->spawnFunc = arg9;
}

static void WeatherSpawnRamp_SetTarget(WeatherSpawnRamp *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4) {
    arg0->targetCount = arg1;
    arg0->targetInterval = arg2;
    arg0->intervalStep = arg3;
    arg0->countStep = arg4;
}

static s32 WeatherSpawnRamp_Update(WeatherSpawnRamp *arg0) {
    s16 temp_r0;
    s16 temp_r0_2;
    s16 temp_r0_4;
    s16 temp_r1;
    s16 temp_r1_2;
    s32 temp_r0_3;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r4;

    temp_r0 = arg0->intervalStep;
    var_r4 = 0;
    if (temp_r0 == 0) {
        var_r4 |= 1;
    } else {
        if ((s32)temp_r0 <= 0) {
            var_r2 = 1;
        } else {
            var_r2 = 0;
        }
        temp_r1 = arg0->targetInterval;
        temp_r0_2 = arg0->interval;
        if ((((s32)temp_r0_2 >= (s32)temp_r1) && (var_r2 == 0)) || (((s32)temp_r0_2 <= (s32)temp_r1) && (var_r2 == 1))) {
            var_r4 |= 1;
            arg0->interval = temp_r1;
        }
    }
    temp_r0_3 = arg0->countStep;
    if (temp_r0_3 == 0) {
        var_r4 |= 2;
    } else {
        if (temp_r0_3 <= 0) {
            var_r2_2 = 1;
        } else {
            var_r2_2 = 0;
        }
        temp_r1_2 = arg0->targetCount;
        temp_r0_4 = arg0->count;
        if ((((s32)temp_r0_4 >= (s32)temp_r1_2) && (var_r2_2 == 0)) || (((s32)temp_r0_4 <= (s32)temp_r1_2) && (var_r2_2 == 1))) {
            var_r4 |= 2;
            arg0->count = temp_r1_2;
        }
    }
    arg0->timer = (s16)(arg0->timer - 1);
    if ((s32)arg0->timer <= 0) {
        arg0->spawnFunc(arg0->work, arg0->count);
        arg0->timer = (s16)arg0->interval;
        if (!(1 & var_r4)) {
            arg0->interval = (s16)(arg0->interval + arg0->intervalStep);
        }
        if (!(2 & var_r4)) {
            arg0->countTimer = (s16)(arg0->countTimer + 1);
            if ((s32)arg0->countTimer >= (s32)arg0->countStepSpawns) {
                arg0->countTimer = 0;
                arg0->count = (s16)(arg0->count + arg0->countStep);
            }
        }
    }
    return var_r4;
}

static void WeatherFog_Start(WeatherFogChange *arg0, WeatherFogTableFade *arg1, FogData *fog, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb, s32 arg6, s32 arg7) {
    arg1->fog = fog;
    if (arg7 != 0) {
        if (arg7 == 1) {
            Weather_SetFog(fog, fogSlope, fogOffset, rgb);
            WeatherFogTableFade_Clear(arg1);
            WeatherFogTableFade_Start(arg1, arg6, 1);
        } else if (arg7 == 2) {
            WeatherFogChange_Init(arg0, fog, fogSlope, fogOffset, rgb, 0x7F * arg6);
        }
    }
}

static BOOL WeatherFog_Update(WeatherFogChange *arg0, WeatherFogTableFade *arg1, s32 arg2) {
    s32 var_r4;

    var_r4 = 1;
    if (arg2 != 0) {
        if (arg2 == 1) {
            var_r4 = WeatherFogTableFade_Update(arg1);
        } else if (arg2 == 2) {
            if (WeatherFogChange_Step(arg0) == 0) {
                var_r4 = 0;
            }
        }
    }
    return var_r4;
}

static void Weather_SetFog(FogData *fog, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb) {
    Fog_Set(fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, fogSlope, fogOffset);
    Fog_SetColor(fog, -1, rgb, 0x1F);
}

static void WeatherFogChange_Init(WeatherFogChange *fogChange, FogData *fog, s32 arg2, s32 arg3, GXRgb rgb, s32 arg5) {
    int slope = Fog_GetSlope(fog);
    int offset = Fog_GetOffset(fog);
    GXRgb rgb2 = Fog_GetColor(fog);

    fogChange->fog = fog;

    WeatherLerp_Init(&fogChange->offset, offset, arg3, arg5);
    WeatherLerp_Init(&fogChange->red, rgb2 & GX_RGB_R_MASK, rgb & GX_RGB_R_MASK, arg5);
    WeatherLerp_Init(&fogChange->green, (rgb2 >> GX_RGB_G_SHIFT) & 0x1F, (rgb >> GX_RGB_G_SHIFT) & 0x1F, arg5);
    WeatherLerp_Init(&fogChange->blue, (rgb2 >> GX_RGB_B_SHIFT) & 0x1F, (rgb >> GX_RGB_B_SHIFT) & 0x1F, arg5);
    WeatherLerp_Init(&fogChange->slope, slope, arg2, arg5);
}

static BOOL WeatherFogChange_Step(WeatherFogChange *arg0) {
    BOOL ret = WeatherLerp_Step(&arg0->offset);
    WeatherLerp_Step(&arg0->red);
    WeatherLerp_Step(&arg0->green);
    WeatherLerp_Step(&arg0->blue);
    WeatherLerp_Step(&arg0->slope);
    Weather_SetFog(arg0->fog, (GXFogSlope)arg0->slope.value, arg0->offset.value, GX_RGB(arg0->red.value, arg0->green.value, arg0->blue.value));

    return ret;
}

static void WeatherFogTableFade_Clear(WeatherFogTableFade *a0) {
    for (int i = 0; i < 32; i++) {
        a0->table[i] = 0;
    }
    Fog_SetTable(a0->fog, a0->table);
}

static void WeatherFogTableFade_Start(WeatherFogTableFade *arg0, s32 arg1, s32 arg2) {
    arg0->stepsLeft = 0x7F;
    arg0->frameTimer = 0;
    arg0->framesPerStep = arg1;
    if (arg2 == 1) {
        arg0->direction = 1;
        return;
    }
    arg0->direction = -1;
}

static s32 WeatherFogTableFade_Update(WeatherFogTableFade *arg0) {
    s32 temp_r4;

    temp_r4 = WeatherFogTableFade_Step(arg0);
    if (arg0->frameTimer == 0) {
        Fog_SetTable(arg0->fog, arg0->table);
    }
    return temp_r4;
}

static void WeatherFogTableFade_SetFull(WeatherFogTableFade *arg0) {
    arg0->stepsLeft = 1;
    arg0->frameTimer = 0;
    arg0->framesPerStep = 0;
    arg0->direction = 1;
    WeatherFogTableFade_BuildTable(arg0);
    Fog_SetTable(arg0->fog, arg0->table);
}

static s32 WeatherFogTableFade_Step(WeatherFogTableFade *arg0) {
    s32 temp_r0;
    s32 temp_r2;
    s32 var_r4;

    var_r4 = 0;
    if (arg0->direction == 0) {
        var_r4 = 1;
    } else if ((s32)arg0->stepsLeft <= 0) {
        var_r4 = 1;
    } else {
        temp_r2 = arg0->frameTimer + 1;
        arg0->frameTimer = temp_r2;
        if (temp_r2 >= (s32)arg0->framesPerStep) {
            arg0->frameTimer = 0;
            WeatherFogTableFade_BuildTable(arg0);
            temp_r0 = arg0->stepsLeft - 1;
            arg0->stepsLeft = temp_r0;
            if (temp_r0 <= 0) {
                var_r4 = 1;
            }
        }
    }
    return var_r4;
}

static void WeatherFogTableFade_BuildTable(WeatherFogTableFade *arg0) {
    int i;
    int v0;
    int v1;

    if (arg0->direction < 0) {
        v1 = 0x7F - arg0->stepsLeft;
    } else {
        v1 = arg0->stepsLeft;
    }

    v0 = v1 / 4;
    for (i = 31; i > v0; i--) {
        arg0->table[i] = (i - v0) * 4;
    }
}

static void Weather_SpawnAndPreAdvance(WeatherWork *a0, WeatherSpawnFunc a1, int a2, int a3, int a4, WeatherObjectFunc a5) {
    a1(a0, a2);

    int i, j;
    int v0 = 0;
    WeatherObject *cur = a0->linkedListDummy.next;
    WeatherObject *next = cur->next;

    for (i = 0; i < a2; i++) {
        if (cur == &a0->linkedListDummy) {
            break;
        }

        for (j = 0; j < v0; j++) {
            a5(cur);

            if (cur->data == NULL) {
                break;
            }
        }

        cur = next;
        next = cur->next;

        if (i >= a3 && (i % a3) == 0) {
            v0 += a4;
        }
    }
}

static WeatherObject *WeatherSystem_GetFreeObject(WeatherWork *a0) {
    for (int i = 0; i < 64; i++) {
        if (a0->linkedList[i].weatherSystem == NULL) {
            return &a0->linkedList[i];
        }
    }
    return NULL;
}

static void WeatherTask_Sunny(SysTask *task, void *data) {
    WeatherWork *arg1 = data;
    u16 temp_r2 = arg1->state;
    switch (temp_r2) {
    case 0:
        arg1->state = 1;
        break;
    case 1:
        arg1->state = 3;
        break;
    case 2:
        arg1->state = 3;
        break;
    case 3:
        if (arg1->stopRequest == 5) {
            arg1->state = 4;
        }
        break;
    case 4:
        arg1->state = 5;
        break;
    case 5:
        WeatherEntry_EndTask(arg1->entry);
        break;
    }
}

static void WeatherTask_Rain(SysTask *task, void *data) {
    WeatherWork *v0 = data;

    WeatherParticleTaskData *v1 = v0->taskData;

    switch (v0->state) {
    case 0:
        WeatherSpawnRamp_Init(&v1->spawnRamp, v0, 1, 8, 20, 1, -1, 1, 2, Rain_AddDrops);
        WeatherFog_Start(&v1->fogChange, &v1->fogTableFade, v0->weatherSystem->fieldSystem->fog, GX_FOGSLOPE_0x1000, 0x726F, 0x6b5a, 1, v0->fogMode);

        v1->data[0] = 0;
        v0->seDelayTask = SysTask_CreateOnMainQueue(WeatherTask_DelayedRainSE, v0, 100);
        v0->seDelay = 30;
        v0->state = 1;
        break;
    case 1:
        int v2 = WeatherSpawnRamp_Update(&v1->spawnRamp);
        if (v1->data[0] > 0) {
            v1->data[0]--;
        } else {
            int v3 = WeatherFog_Update(&v1->fogChange, &v1->fogTableFade, v0->fogMode);
            if (v3 == 1 && v2 == 3) {
                v0->state = 3;
            }
        }
        break;
    case 2:
        WeatherSpawnRamp_Init(&v1->spawnRamp, v0, 20, 1, 20, 1, -1, 1, 2, Rain_AddDrops);

        if (v0->fogMode) {
            v1->fogTableFade.fog = v0->weatherSystem->fieldSystem->fog;
            Weather_SetFog(v1->fogTableFade.fog, GX_FOGSLOPE_0x1000, 0x726f, GX_RGB(26, 26, 26));
            WeatherFogTableFade_SetFull(&v1->fogTableFade);
        }

        Weather_SpawnAndPreAdvance(v0, Rain_AddDrops, 20, 10, 1, Rain_UpdateDrop);

        v0->seDelayTask = SysTask_CreateOnMainQueue(WeatherTask_DelayedRainSE, v0, 100);
        v0->seDelay = 30;
        v0->state = 3;
        break;
    case 3:
        if (v1->spawnRamp.timer-- <= 0) {
            Rain_AddDrops(v0, v1->spawnRamp.count);
            v1->spawnRamp.timer = v1->spawnRamp.interval;
        }

        if (v0->stopRequest == 5) {
            WeatherSpawnRamp_SetTarget(&v1->spawnRamp, 0, 8, 1, -2);

            if (v0->fogMode) {
                WeatherFogTableFade_Start(&v1->fogTableFade, 1, 0);
            }
            v1->data[0] = 0;
            v0->state = 4;
            Weather_StopLoopSE(v0);
        }
        break;
    case 4:
        int ret = WeatherSpawnRamp_Update(&v1->spawnRamp);
        if (v1->data[0] > 0) {
            v1->data[0]--;
        } else {
            int vUnk;
            if (v0->fogMode) {
                vUnk = WeatherFogTableFade_Update(&v1->fogTableFade);
            } else {
                vUnk = 1;
            }

            if (vUnk == 1 && ret == 3) {
                if (v0->linkedListDummy.next == &v0->linkedListDummy) {
                    v0->state = 5;
                }
            }
        }
        break;
    case 5:
        if (v0->fogMode) {
            Fog_Set(v1->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(v0->entry);
        break;
    }

    if (v0->state != 5 && v0->state != 0) {
        WeatherObject_ForEach(&v0->linkedListDummy, Rain_UpdateDrop);
        WeatherWork_ScrollObjectsInt(v0, NULL, NULL);
        WeatherWork_Nop(v0);
    }
}

static void Rain_AddDrops(WeatherWork *a0, int arg1) {
    for (int i = 0; i < arg1; i++) {
        WeatherObject *v0 = WeatherSystem_AddObject(a0, 32);
        if (v0 == NULL) {
            break;
        }

        u32 *data = v0->data;
        u32 rand = MTRandom();

        data[0] = 0;
        int frame = rand % 3;
        Sprite_SetAnimationFrame(v0->sprite, frame);

        int v1 = rand % 20;
        data[2] = 10 * (frame + 1) + v1;
        if (frame == 2) {
            data[2] += 10;
        }

        v1 /= -5;
        data[4] = -5 * (frame + 1) + v1;
        if (frame == 2) {
            data[4] -= 5;
        }
        data[3] = 0;
        data[1] = 1 + (rand % 3);

        VecFx32 vec;

        vec.x = (15 * frame + (rand % 270)) << FX32_SHIFT;
        vec.y = 0xFFFA0 << FX32_SHIFT;
        vec.z = 0;
        WeatherDraw_SetSpriteMatrix(v0->sprite, &vec);
    }
}

static void Rain_UpdateDrop(WeatherObject *a0) {
    int i;
    s32 *data = a0->data;
    VecFx32 vec;
    vec = WeatherObject_GetPosition(a0);
    switch (data[3]) {
    case 0:
        for (i = 0; i < 2; i++) {
            vec.x += data[4] << FX32_SHIFT;
            vec.y += data[2] << FX32_SHIFT;

            if (data[0]++ > data[1]) {
                if (((u32)MTRandom() % 10) < 7) {
                    data[3] = 2;
                } else {
                    data[3] = 1;
                    data[0] = 4;
                    Sprite_SetAnimationFrame(a0->sprite, 3);
                }
            }
        }
        WeatherDraw_SetSpriteMatrix(a0->sprite, &vec);
        break;
    case 1:
        if (data[0]-- <= 0) {
            data[3] = 2;
        }
        break;
    case 2:
        WeatherObject_Delete(a0);
        break;
    }
}

static void WeatherTask_Snow(SysTask *task, void *data) {
    WeatherWork *v0 = data;

    WeatherParticleTaskData *v1 = v0->taskData;

    switch (v0->state) {
    case 0:
        WeatherSpawnRamp_Init(&v1->spawnRamp, v0, 1, 30, 6, 3, -5, 2, 1, Snow_AddFlakes);

        WeatherFog_Start(&v1->fogChange, &v1->fogTableFade, v0->weatherSystem->fieldSystem->fog, GX_FOGSLOPE_0x1000, 0x726F, GX_RGB(24, 24, 24), 2, v0->fogMode);

        v1->data[0] = 8;
        v1->data[1] = 0;
        v0->state = 1;
        break;
    case 1:
        int v2 = WeatherSpawnRamp_Update(&v1->spawnRamp);
        if (v1->data[0] > 0) {
            v1->data[0]--;
        } else {
            int v3 = WeatherFog_Update(&v1->fogChange, &v1->fogTableFade, v0->fogMode);
            if (v3 == 1 && v2 == 3) {
                v0->state = 3;
            }
        }
        break;
    case 2:
        WeatherSpawnRamp_Init(&v1->spawnRamp, v0, 6, 3, 6, 3, -5, 2, 1, Snow_AddFlakes);

        if (v0->fogMode) {
            v1->fogTableFade.fog = v0->weatherSystem->fieldSystem->fog;
            Weather_SetFog(v1->fogTableFade.fog, GX_FOGSLOPE_0x1000, 0x726f, GX_RGB(24, 24, 24));
            WeatherFogTableFade_SetFull(&v1->fogTableFade);
        }

        v1->data[1] = 0;

        Weather_SpawnAndPreAdvance(v0, Snow_AddFlakes, 20, 2, 3, Snow_UpdateFlake);

        v0->state = 3;
        break;
    case 3:
        if (v1->spawnRamp.timer-- <= 0) {
            Snow_AddFlakes(v0, v1->spawnRamp.count);
            v1->spawnRamp.timer = v1->spawnRamp.interval;
        }

        if (v0->stopRequest == 5) {
            WeatherSpawnRamp_SetTarget(&v1->spawnRamp, 0, 30, 5, -3);

            if (v0->fogMode) {
                WeatherFogTableFade_Start(&v1->fogTableFade, 1, 0);
            }
            v1->data[0] = 0;
            v0->state = 4;
        }
        break;
    case 4:
        int ret = WeatherSpawnRamp_Update(&v1->spawnRamp);
        if (v1->data[0] > 0) {
            v1->data[0]--;
        } else {
            int vUnk;
            if (v0->fogMode) {
                vUnk = WeatherFogTableFade_Update(&v1->fogTableFade);
            } else {
                vUnk = 1;
            }

            if (vUnk == 1 && ret == 3) {
                if (v0->linkedListDummy.next == &v0->linkedListDummy) {
                    v0->state = 5;
                }
            }
        }
        break;
    case 5:
        if (v0->fogMode) {
            Fog_Set(v1->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(v0->entry);
        break;
    }

    if (v0->state != 5 && v0->state != 0) {
        WeatherObject_ForEach(&v0->linkedListDummy, Snow_UpdateFlake);
        WeatherWork_ScrollObjectsInt(v0, NULL, NULL);
        WeatherWork_Nop(v0);
    }
}

static const sSnowFallSpeeds[] = { 2, 2, 2, 2 };
static const sSnowSlowdownIntervals[] = { 16, 32, 16, 10 };

static void Snow_AddFlakes(WeatherWork *a0, int a1) {
    int table0[4];
    table0 = sSnowSlowdownIntervals;
    int table1[4];
    table1 = sSnowFallSpeeds;
    WeatherParticleTaskData *v0 = a0->taskData;

    for (int i = 0; i < a1; i++) {
        WeatherObject *v1 = WeatherSystem_AddObject(a0, 32);
        if (v1 == NULL) {
            break;
        }

        s32 *data = v1->data;

        v0->data[1]++;
        if (v0->data[1] >= 800) {
            v0->data[1] = 0;
        }
        int index = v0->data[1] / 200;
        data[5] = table0[index];

        data[0] = 0;
        data[1] = 4 + (MTRandom() % 42);

        int frame = (data[1] - 4) / 15;
        Sprite_SetAnimationFrame(v1->sprite, frame);

        data[4] = -(frame + 1);
        data[2] = table1[index] * (frame + 1);
        data[3] = 0;

        VecFx32 vec = WeatherObject_GetPosition(v1);

        vec.x = -20 + (frame * 20) + (MTRandom() % 420);
        vec.y = -8;
        vec.z = 0;
        vec.x <<= FX32_SHIFT;
        vec.y <<= FX32_SHIFT;

        WeatherDraw_SetSpriteMatrix(v1->sprite, &vec);
    }
}

static void Snow_UpdateFlake(WeatherObject *a0) {
    s32 *data = a0->data;
    VecFx32 vec = WeatherObject_GetPosition(a0);

    switch (data[3]) {
    case 0:
        vec.x += data[4] << FX32_SHIFT;
        vec.y += data[2] << FX32_SHIFT;

        if (data[0]++ > data[1]) {
            data[3] = 1;
        }

        if (data[0] % data[5] == 0) {
            data[4]--;

            if (data[2] > 1) {
                data[2]--;
            }
        }

        WeatherDraw_SetSpriteMatrix(a0->sprite, &vec);
        break;
    case 1:
        WeatherObject_Delete(a0);
        break;
    }
}

static void WeatherTask_DiamondDust(SysTask *task, void *data) {
    WeatherWork *v0 = data;

    WeatherParticleTaskData *v1 = v0->taskData;

    switch (v0->state) {
    case 0:
        WeatherSpawnRamp_Init(&v1->spawnRamp, v0, 2, 16, 20, 2, -2, 4, 2, DiamondDust_AddSparkles);

        WeatherFog_Start(&v1->fogChange, &v1->fogTableFade, v0->weatherSystem->fieldSystem->fog, GX_FOGSLOPE_0x1000, 0x716F, GX_RGB(26, 26, 26), 1, v0->fogMode);

        v1->data[0] = 0;
        v0->state = 1;
        break;
    case 1:
        int v2 = WeatherSpawnRamp_Update(&v1->spawnRamp);
        if (v1->data[0] > 0) {
            v1->data[0]--;
        } else {
            int v3 = WeatherFog_Update(&v1->fogChange, &v1->fogTableFade, v0->fogMode);
            if (v3 == 1 && v2 == 3) {
                v0->state = 3;
            }
        }
        break;
    case 2:
        WeatherSpawnRamp_Init(&v1->spawnRamp, v0, 20, 2, 20, 2, -2, 4, 2, DiamondDust_AddSparkles);

        if (v0->fogMode) {
            v1->fogTableFade.fog = v0->weatherSystem->fieldSystem->fog;
            Weather_SetFog(v1->fogTableFade.fog, GX_FOGSLOPE_0x1000, 0x716f, GX_RGB(26, 26, 26));
            WeatherFogTableFade_SetFull(&v1->fogTableFade);
        }

        Weather_SpawnAndPreAdvance(v0, DiamondDust_AddSparkles, 20, 10, 1, DiamondDust_UpdateSparkle);

        v0->state = 3;
        break;
    case 3:
        if (v1->spawnRamp.timer-- <= 0) {
            DiamondDust_AddSparkles(v0, v1->spawnRamp.count);
            v1->spawnRamp.timer = v1->spawnRamp.interval;
        }

        if (v0->stopRequest == 5) {
            WeatherSpawnRamp_SetTarget(&v1->spawnRamp, 0, 16, 6, -10);

            if (v0->fogMode) {
                WeatherFogTableFade_Start(&v1->fogTableFade, 1, 0);
            }
            v1->data[0] = 20;
            v0->state = 4;
        }
        break;
    case 4:
        int ret = WeatherSpawnRamp_Update(&v1->spawnRamp);
        if (v1->data[0] > 0) {
            v1->data[0]--;
        } else {
            int vUnk = WeatherFog_Update(&v1->fogChange, &v1->fogTableFade, v0->fogMode);

            if (vUnk == 1 && ret == 3) {
                if (v0->linkedListDummy.next == &v0->linkedListDummy) {
                    v0->state = 5;
                }
            }
        }
        break;
    case 5:
        if (v0->fogMode) {
            Fog_Set(v1->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(v0->entry);
        break;
    }

    if (v0->state != 5 && v0->state != 0) {
        WeatherObject_ForEach(&v0->linkedListDummy, DiamondDust_UpdateSparkle);
        WeatherWork_ScrollObjectsInt(v0, NULL, NULL);
        WeatherWork_Nop(v0);
    }
}

static void DiamondDust_AddSparkles(WeatherWork *a0, int a1) {
    VecFx32 vec;
    for (int i = 0; i < a1; i++) {
        WeatherObject *v0 = WeatherSystem_AddObject(a0, 32);
        if (v0 == NULL) {
            break;
        }

        u32 *data = v0->data;

        data[0] = 0;
        data[1] = 7 + (MTRandom() % 5);

        int rand = MTRandom() % 1000;

        if ((rand % 2) == 0) {
            data[2] = 1;
        } else {
            data[2] = -1;
        }

        data[3] = 1;
        data[4] = 3 + (MTRandom() % 6);
        data[5] = 4 + (MTRandom() % 5);

        int frame = MTRandom() % 20;

        vec = WeatherObject_GetPosition(v0);
        vec.x = -64 + (MTRandom() % 384);
        vec.y = -8 + ((u32)MTRandom() % 256);
        vec.x <<= FX32_SHIFT;
        vec.y <<= FX32_SHIFT;
        vec.z = 0;
        WeatherDraw_SetSpriteMatrix(v0->sprite, &vec);
        vec.x >>= FX32_SHIFT;
        vec.y >>= FX32_SHIFT;

        int v1 = 50 - vec.x / 3;
        int v2 = 206 - vec.x / 3;
        int v3;
        if (v2 < 0) {
            v2 *= -1;
            v3 = v1 - (MTRandom() % v2);
        } else {
            v3 = v1 + (MTRandom() % v2);
        }

        if (v1 <= vec.y && v3 >= vec.y) {
            data[1] *= 2;
        } else {
            frame = MTRandom() & 3;
        }
        Sprite_SetAnimationFrame(v0->sprite, frame);
    }
}

static void DiamondDust_UpdateSparkle(WeatherObject *a0) {
    s32 *data = a0->data;
    WeatherObject_GetPosition(a0);
    if (++data[0] >= data[1]) {
        WeatherObject_Delete(a0);
    }
}

static void WeatherTask_RunFogOnly(WeatherWork *a0, WeatherFogTaskData *a1, GXFogSlope fogSlope, s32 arg3, GXRgb arg4, s32 arg5, s32 arg6) {
    switch (a0->state) {
    case 0:
        WeatherFog_Start(&a1->fogChange, &a1->fogTableFade, a0->weatherSystem->fieldSystem->fog, fogSlope, arg3, arg4, arg5, a0->fogMode);
        a0->state = 1;
        break;
    case 1:
        int v3 = WeatherFog_Update(&a1->fogChange, &a1->fogTableFade, a0->fogMode);
        if (v3 == 1) {
            a0->state = 3;
        }
        break;
    case 2:
        if (a0->fogMode != 0) {
            a1->fogTableFade.fog = a0->weatherSystem->fieldSystem->fog;
            Weather_SetFog(a1->fogTableFade.fog, fogSlope, arg3, arg4);
            WeatherFogTableFade_SetFull(&a1->fogTableFade);
        }
        a0->state = 3;
        break;
    case 3:
        if (a0->stopRequest == 5) {
            if (a0->fogMode != 0) {
                WeatherFogTableFade_Start(&a1->fogTableFade, arg6, 0);
            }
            a0->state = 4;
        }
        break;
    case 4:
        int ret;
        if (a0->fogMode != 0) {
            ret = WeatherFogTableFade_Update(&a1->fogTableFade);
        } else {
            ret = 1;
        }

        if (ret == 1) {
            a0->state = 5;
        }
        break;
    case 5:
        if (a0->fogMode != 0) {
            Fog_Set(a1->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(a0->entry);
        break;
    }
}

static void WeatherTask_Mist(SysTask *task, void *data) {
    WeatherWork *v0 = data;
    WeatherMistTaskData *v1 = v0->taskData;

    switch (v0->state) {
    case 0:
        WeatherFog_Start(&v1->fogChange, &v1->fogTableFade, v0->weatherSystem->fieldSystem->fog, GX_FOGSLOPE_0x0200, 0x7555, 0x7fff, 1, v0->fogMode);
        WeatherLerp_Init(&v1->blendLerp, 0, 9, 30);
        Weather_SetBlendAlpha(0, 16);
        GfGfx_EngineATogglePlanes(4, 1);
        v0->state = 1;
        break;
    case 1:
        int v2 = WeatherFogTableFade_Update(&v1->fogTableFade);
        int v3 = WeatherLerp_Step(&v1->blendLerp);
        Weather_SetBlendAlpha(v1->blendLerp.value, 16 - v1->blendLerp.value);
        if (v2 == 1 && v3 == 1) {
            v0->state = 3;
        }
        break;
    case 2:
        if (v0->fogMode != 0) {
            v1->fogTableFade.fog = v0->weatherSystem->fieldSystem->fog;
            Weather_SetFog(v1->fogTableFade.fog, GX_FOGSLOPE_0x0200, 0x7555, 0x7fff);
            WeatherFogTableFade_SetFull(&v1->fogTableFade);
        }
        Weather_SetBlendAlpha(9, 7);
        GfGfx_EngineATogglePlanes(4, 1);
        v0->state = 3;
        break;
    case 3:
        if (v0->stopRequest == 5) {
            if (v0->fogMode != 0) {
                WeatherFogTableFade_Start(&v1->fogTableFade, 1, 0);
            }
            WeatherLerp_Init(&v1->blendLerp, 9, 0, 30);
            v0->state = 4;
        }
        break;
    case 4:
        int v4;
        if (v0->fogMode != 0) {
            v4 = WeatherFogTableFade_Update(&v1->fogTableFade);
        } else {
            v4 = 1;
        }

        int v5 = WeatherLerp_Step(&v1->blendLerp);
        Weather_SetBlendAlpha(v1->blendLerp.value, 16 - v1->blendLerp.value);
        if (v4 == 1 && v5 == 1) {
            v0->state = 5;
        }
        break;
    case 5:
        if (v0->fogMode != 0) {
            Fog_Set(v1->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(v0->entry);
        break;
    }
}

static void WeatherTask_Flash(SysTask *task, void *data) {
    WeatherWork *v0 = data;
    WeatherSystem *weatherSystem = v0->weatherSystem;
    FieldSystem *fieldSystem = weatherSystem->fieldSystem;
    WeatherFlashTaskData *v1 = v0->taskData;

    switch (v0->state) {
    case 0:
        v1->darkness = FlashDarkness_Show(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
        Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
        Fog_SetColor(fieldSystem->fog, -1, 0, 0);
        {
            s8 v2[32];
            for (int i = 0; i < 32; i++) {
                v2[i] = -1;
            }
            Fog_SetTable(fieldSystem->fog, v2);
        }
        v1->scaleFrame = 0;
        v0->state = 1;
        break;
    case 1:
        v0->state = 3;
        break;
    case 2:
        v1->darkness = FlashDarkness_Show(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
        Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
        Fog_SetColor(fieldSystem->fog, -1, 0, 0);
        {
            s8 v2[32];
            for (int i = 0; i < 32; i++) {
                v2[i] = -1;
            }
            Fog_SetTable(fieldSystem->fog, v2);
        }
        v1->scaleFrame = 0;
        v0->state = 3;
        break;
    case 3:
        if (v0->stopRequest == 5) {
            v1->endStep = 0;
            v0->state = 4;
        }
        break;
    case 4:
        switch (v1->endStep) {
        case 0:
            BeginNormalPaletteFade(3, 0, 0, 0x7FFF, 6, 1, HEAP_ID_FIELD1);
            v1->endStep++;
            break;
        case 1:
            if (IsPaletteFadeFinished()) {
                BeginNormalPaletteFade(3, 1, 1, 0x7FFF, 6, 1, HEAP_ID_FIELD1);
                v1->endStep++;
            }
            break;
        case 2:
            if (IsPaletteFadeFinished()) {
                v1->endStep++;
            }
            break;
        case 3:
            v1->scaleFrame++;
            float v3 = 1.0 + (v1->scaleFrame * 3.0) / 24.0;
            FlashDarkness_SetScale(v1->darkness, v3);
            if (v3 >= 4.0) {
                v0->state = 5;
            }
            break;
        }
        break;
    case 5:
        WeatherEntry_EndTask(v0->entry);
        break;
    }
}

static void WeatherTask_Flash2(SysTask *task, void *data) {
    WeatherWork *v0 = data;
    WeatherSystem *weatherSystem = v0->weatherSystem;
    FieldSystem *fieldSystem = weatherSystem->fieldSystem;
    WeatherFlashTaskData *v1 = v0->taskData;

    switch (v0->state) {
    case 0:
        v1->darkness = FlashDarkness_Show(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
        Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
        Fog_SetColor(fieldSystem->fog, -1, 0, 0);
        {
            s8 v2[32];
            for (int i = 0; i < 32; i++) {
                v2[i] = -1;
            }
            Fog_SetTable(fieldSystem->fog, v2);
        }
        FlashDarkness_SetScale(v1->darkness, 4);
        v0->state = 1;
        break;
    case 1:
        v0->state = 3;
        break;
    case 2:
        v1->darkness = FlashDarkness_Show(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
        Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
        Fog_SetColor(fieldSystem->fog, -1, 0, 0);
        {
            s8 v2[32];
            for (int i = 0; i < 32; i++) {
                v2[i] = -1;
            }
            Fog_SetTable(fieldSystem->fog, v2);
        }
        FlashDarkness_SetScale(v1->darkness, 4);
        v0->state = 3;
        break;
    case 3:
        if (v0->stopRequest == 5) {
            v0->state = 4;
        }
        break;
    case 4:
        v0->state = 5;
        break;
    case 5:
        WeatherEntry_EndTask(v0->entry);
        break;
    }
}

static void WeatherTask_LowLight(SysTask *task, void *data) {
    WeatherWork *v0 = data;
    WeatherTask_RunFogOnly(v0, v0->taskData, GX_FOGSLOPE_0x4000, 0x4B6F, 0x421, 0, 0);
}

static void WeatherTask_DelayedRainSE(SysTask *task, void *data) {
    WeatherWork *v0 = data;
    if (--v0->seDelay <= 0) {
        Weather_PlayLoopSE(v0, SEQ_SE_DP_T_AME);
        SysTask_Destroy(task);
        v0->seDelayTask = 0;
    }
}

static void Weather_PlayLoopSE(WeatherWork *a0, u32 sndSeq) {
    GF_ASSERT(a0->loopSEPlaying == 0);

    a0->loopSEPlaying = 1;
    a0->sndSeq = sndSeq;
    PlaySE(sndSeq);
}

static void Weather_StopLoopSE(WeatherWork *arg0) {
    StopSE(arg0->sndSeq, 0);
    arg0->loopSEPlaying = 0;
}
