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
static void Weather_Init2DGfxRes(GF_2DGfxResHeader *headerList, u32 index, u32 fileId);
static void WeatherDraw_Delete(WeatherDraw *weatherDraw);
static void WeatherDraw_SetSpriteMatrix(Sprite *sprite, VecFx32 *matrix);
static WeatherSystem *WeatherSystem_New(FieldSystem *fieldSystem);
static void WeatherSystem_Delete(WeatherSystem **pWeatherSystem);
static BOOL WeatherSystem_Process(WeatherSystem *weatherSystem, int state, int weather);
static u16 WeatherSystem_GetState(WeatherSystem *weatherSystem, int weather);
static void Weather_SetBlendAlpha(u32 ev1, u32 ev2);
static void WeatherLerp_Init(WeatherLerp *lerp, s32 start, s32 end, s32 duration);
static BOOL WeatherLerp_Step(WeatherLerp *lerp);
static void WeatherSystem_LoadSpriteResources(WeatherSystem *weatherSystem, s32 headerIndex, WeatherSpriteResources *spriteResources);
static SpriteResource *Weather_AddResObjFromOpenNarc(GF_2DGfxResHeader *headerList, s32 resourceType, s32 headerIndex, GF_2DGfxResMan *resMan, NARC *narc, BOOL atEnd);
static void WeatherSystem_FreeSpriteResources(WeatherSystem *weatherSystem, s32 headerIndex, WeatherSpriteResources *spriteResources);
static BOOL WeatherSystem_Init(WeatherSystem *weatherSystem, int weather);
static BOOL WeatherSystem_LoadAsync(WeatherSystem *weatherSystem, int weather);
static BOOL WeatherSystem_Start(WeatherSystem *weatherSystem, int weather, u32 state, u32 fogMode);
static void WeatherSystem_RequestStop(WeatherSystem *weatherSystem, int weather, u16 fogMode);
static void WeatherSystem_StopNow(WeatherSystem *weatherSystem, int weather);
static void WeatherSystem_Unload(WeatherSystem *weatherSystem, u32 weather);
static void WeatherEntry_EndTask(WeatherEntry *entry);
static void WeatherSystem_LoadBgGfx(WeatherSystem *weatherSystem, u16 bgGfxId);
static BOOL WeatherSystem_LoadSprites(WeatherSystem *weatherSystem, WeatherEntry *entry);
static void WeatherTask_LoadAsync(SysTask *task, void *data);
static BOOL WeatherSystem_AllocWork(WeatherSystem *weatherSystem, WeatherEntry *entry);
static BOOL WeatherEntry_AllocSpriteResources(WeatherEntry *entry);
static void WeatherSystem_LoadCellResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *spriteResources);
static void WeatherSystem_LoadAnimResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *spriteResources);
static void WeatherSystem_LoadCharResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *spriteResources);
static void WeatherSystem_LoadPlttResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *spriteResources);
static void WeatherSystem_InitSpriteTemplate(WeatherSystem *weatherSystem, WeatherEntry *entry);
static void WeatherWork_CreateSprites(WeatherWork *work);
static void WeatherWork_DeleteSprites(WeatherWork *work);
static void WeatherSystem_LoadBgPalette(WeatherSystem *weatherSystem, u16 bgGfxId);
static void WeatherSystem_LoadBgChars(WeatherSystem *weatherSystem, u16 bgGfxId);
static void WeatherSystem_LoadBgScreen(WeatherSystem *weatherSystem, u16 bgGfxId);
static void WeatherObject_InitData(WeatherObject *object, int size);
static void WeatherObject_ClearData(WeatherObject *object);
static void *WeatherSystem_AddObject(WeatherWork *work, int size);
static void WeatherSystem_InitSpriteResourcesHeader(SpriteResourcesHeader *header, WeatherSystem *weatherSystem, WeatherSpriteResources *spriteResources, u32 transfer, u32 priority);
static void WeatherObject_Delete(WeatherObject *object);
static void WeatherObject_DeleteAll(WeatherObject *head);
static void WeatherObject_ForEach(WeatherObject *head, WeatherObjectFunc func);
static void WeatherWork_Nop(void *data);
static VecFx32 WeatherObject_GetPosition(WeatherObject *object);
static void WeatherWork_GetCameraScroll(fx32 *x, fx32 *z, WeatherWork *work);
static void WeatherWork_ScrollObjectsInt(WeatherWork *work, int *xOut, int *zOut);
static void WeatherWork_ScrollObjects(WeatherWork *work, fx32 *x, fx32 *y);
static void WeatherSpawnRamp_Init(WeatherSpawnRamp *ramp, WeatherWork *work, s16 count, s16 interval, s32 targetCount, s32 targetInterval, s32 intervalStep, s32 countStepSpawns, s32 countStep, WeatherSpawnFunc spawnFunc);
static void WeatherSpawnRamp_SetTarget(WeatherSpawnRamp *ramp, s16 targetCount, s16 targetInterval, s16 intervalStep, s32 countStep);
static s32 WeatherSpawnRamp_Update(WeatherSpawnRamp *ramp);
static void WeatherFog_Start(WeatherFogChange *fogChange, WeatherFogTableFade *tableFade, FogData *fog, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb, s32 framesPerStep, s32 fogMode);
static BOOL WeatherFog_Update(WeatherFogChange *fogChange, WeatherFogTableFade *tableFade, s32 fogMode);
static void Weather_SetFog(FogData *fog, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb);
static void WeatherFogChange_Init(WeatherFogChange *fogChange, FogData *fog, s32 targetSlope, s32 targetOffset, GXRgb rgb, s32 duration);
static BOOL WeatherFogChange_Step(WeatherFogChange *fogChange);
static void WeatherFogTableFade_Clear(WeatherFogTableFade *tableFade);
static void WeatherFogTableFade_Start(WeatherFogTableFade *tableFade, s32 framesPerStep, s32 direction);
static s32 WeatherFogTableFade_Update(WeatherFogTableFade *tableFade);
static void WeatherFogTableFade_SetFull(WeatherFogTableFade *tableFade);
static s32 WeatherFogTableFade_Step(WeatherFogTableFade *tableFade);
static void WeatherFogTableFade_BuildTable(WeatherFogTableFade *tableFade);
static void Weather_SpawnAndPreAdvance(WeatherWork *work, WeatherSpawnFunc spawnFunc, int count, int groupSize, int advanceStep, WeatherObjectFunc updateFunc);
static WeatherObject *WeatherSystem_GetFreeObject(WeatherWork *work);
static void WeatherTask_Sunny(SysTask *task, void *data);
static void WeatherTask_Rain(SysTask *task, void *data);
static void Rain_AddDrops(WeatherWork *work, int count);
static void Rain_UpdateDrop(WeatherObject *object);
static void WeatherTask_Snow(SysTask *task, void *data);
static void Snow_AddFlakes(WeatherWork *work, int count);
static void Snow_UpdateFlake(WeatherObject *object);
static void WeatherTask_DiamondDust(SysTask *task, void *data);
static void DiamondDust_AddSparkles(WeatherWork *work, int count);
static void DiamondDust_UpdateSparkle(WeatherObject *object);
static void WeatherTask_RunFogOnly(WeatherWork *work, WeatherFogTaskData *taskData, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb, s32 framesPerStep, s32 stopFramesPerStep);
static void WeatherTask_Mist(SysTask *task, void *data);
static void WeatherTask_Flash(SysTask *task, void *data);
static void WeatherTask_Flash2(SysTask *task, void *data);
static void WeatherTask_LowLight(SysTask *task, void *data);
static void WeatherTask_DelayedRainSE(SysTask *task, void *data);
static void Weather_PlayLoopSE(WeatherWork *work, u32 sndSeq);
static void Weather_StopLoopSE(WeatherWork *work);

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

void FieldTextureManager_KeepTextures(FieldTextureManager *textureManager) {
    textureManager->keepTextures = 1;
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

static void Weather_Init2DGfxRes(GF_2DGfxResHeader *headerList, u32 index, u32 fileId) {
    GF_2DGfxResHeader *header = GF2DGfxResHeader_GetByIndex(headerList, index);
    GF_2DGfxResHeaderNarcList *headerNarcList = GfGfxLoader_LoadFromNarc(NARC_a_0_6_3, fileId, 0, HEAP_ID_FIELD1, 1);
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
        WeatherEntry *playEntry = &weatherSystem->entries[weather];
        if (playEntry->work && playEntry->work->loopSEPlaying == TRUE) {
            PlaySE(playEntry->work->sndSeq);
        }
        break;
    case 10:
        WeatherEntry *stopEntry = &weatherSystem->entries[weather];
        if (stopEntry->work && stopEntry->work->loopSEPlaying == TRUE) {
            StopSE(stopEntry->work->sndSeq, 0);
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
    WeatherEntry *entry = &weatherSystem->entries[weather];
    return entry->state;
}

static void Weather_SetBlendAlpha(u32 ev1, u32 ev2) {
    G2x_SetBlendAlpha_(0x04000050, 4, 0x21, ev1, ev2);
}

static void WeatherLerp_Init(WeatherLerp *lerp, s32 start, s32 end, s32 duration) {
    lerp->value = start;
    lerp->start = start;
    lerp->delta = end - start;
    lerp->duration = duration;
    lerp->frame = 0;
}

static BOOL WeatherLerp_Step(WeatherLerp *lerp) {
    int progress = lerp->delta * lerp->frame;
    progress /= lerp->duration;

    lerp->value = progress + lerp->start;

    if (lerp->frame + 1 <= lerp->duration) {
        lerp->frame++;
        return FALSE;
    }

    lerp->frame = lerp->duration;
    return TRUE;
}

static void WeatherSystem_LoadSpriteResources(WeatherSystem *weatherSystem, s32 headerIndex, WeatherSpriteResources *spriteResources) {
    WeatherSystem_LoadCellResource(weatherSystem, headerIndex, spriteResources);
    WeatherSystem_LoadAnimResource(weatherSystem, headerIndex, spriteResources);
    WeatherSystem_LoadCharResource(weatherSystem, headerIndex, spriteResources);
    WeatherSystem_LoadPlttResource(weatherSystem, headerIndex, spriteResources);
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

static void WeatherSystem_FreeSpriteResources(WeatherSystem *weatherSystem, s32 headerIndex, WeatherSpriteResources *spriteResources) {
    if (headerIndex != 0xFFFF) {
        if (spriteResources->charResObj[GF_GFX_RES_TYPE_CHAR] != 0) {
            SpriteTransfer_DeleteCharTransferTask(spriteResources->charResObj[GF_GFX_RES_TYPE_CHAR]);
        }
        if (spriteResources->charResObj[GF_GFX_RES_TYPE_PLTT] != 0) {
            SpriteTransfer_DeletePlttTransferTask(spriteResources->charResObj[GF_GFX_RES_TYPE_PLTT]);
        }

        for (int i = 0; i < 4; i++) {
            if (spriteResources->charResObj[i] != 0) {
                DestroySingle2DGfxResObj(weatherSystem->weatherDraw.resMan[i], spriteResources->charResObj[i]);
            }
        }
    }
}

static BOOL WeatherSystem_Init(WeatherSystem *weatherSystem, int weather) {
    WeatherEntry *entry = &weatherSystem->entries[weather];
    if (entry->work == 0) {
        if (WeatherSystem_AllocWork(weatherSystem, entry) == FALSE) {
            return FALSE;
        }

        if (WeatherSystem_LoadSprites(weatherSystem, entry) == FALSE) {
            Heap_Free(entry->work);
            entry->work = NULL;
            return FALSE;
        }

        entry->work->spriteResources = entry->weatherSpriteResources;

        if (entry->spriteResId != 0xFFFF) {
            WeatherWork_CreateSprites(entry->work);
        }

        WeatherSystem_LoadBgGfx(weatherSystem, entry->bgGfxId);
        entry->state = 2;
    }

    return TRUE;
}

static BOOL WeatherSystem_LoadAsync(WeatherSystem *weatherSystem, int weather) {
    WeatherEntry *entry = &weatherSystem->entries[weather];
    if (entry->work) {
        return TRUE;
    }

    if (WeatherSystem_AllocWork(weatherSystem, entry) == 0) {
        return FALSE;
    }

    entry->task = SysTask_CreateOnMainQueue(WeatherTask_LoadAsync, entry, 1);
    entry->state = 1;
    entry->loadStep = 0;

    return TRUE;
}

static BOOL WeatherSystem_Start(WeatherSystem *weatherSystem, int weather, u32 state, u32 fogMode) {
    WeatherEntry *entry = &weatherSystem->entries[weather];

    if (entry->spriteResId != 0xFFFF && entry->weatherSpriteResources == 0) {
        return FALSE;
    }

    if (entry->work == NULL) {
        return FALSE;
    }

    if (entry->state != 2) {
        return FALSE;
    }

    entry->work->weatherTask = SysTask_CreateOnMainQueue(entry->taskFunc, entry->work, 4);

    if (entry->work->weatherTask == NULL) {
        return FALSE;
    }

    entry->state = 3;
    entry->work->state = state;
    entry->work->stopRequest = 0;
    entry->work->linkedListDummy.next = &entry->work->linkedListDummy;
    entry->work->linkedListDummy.prev = &entry->work->linkedListDummy;
    entry->work->fogMode = fogMode;
    entry->work->loopSEPlaying = 0;

    entry->work->cameraTarget = NNS_G3dGlb.camTarget;

    if (entry->taskDataSize > 0) {
        entry->work->taskData = Heap_Alloc(HEAP_ID_FIELD1, entry->taskDataSize);
        memset(entry->work->taskData, 0, entry->taskDataSize);
    } else {
        entry->work->taskData = NULL;
    }

    if (entry->bgGfxId != 0xFFFF) {
        GfGfx_EngineATogglePlanes(4, 0);
        G2_SetBG2Priority(1);
        G2_SetBG0Priority(2);
    }

    return TRUE;
}

static void WeatherSystem_RequestStop(WeatherSystem *weatherSystem, int weather, u16 fogMode) {
    WeatherEntry *entry = &weatherSystem->entries[weather];
    if (entry->state == 3) {
        entry->work->stopRequest = 5;
        entry->work->fogMode = fogMode;
    }
}

static void WeatherSystem_StopNow(WeatherSystem *weatherSystem, int weather) {
    WeatherEntry *entry = &weatherSystem->entries[weather];
    if (WeatherSystem_GetState(weatherSystem, weather) == 3) {
        entry->work->state = 5;
        entry->taskFunc(NULL, entry->work);
    }
}

static void WeatherSystem_Unload(WeatherSystem *weatherSystem, u32 weather) {
    WeatherEntry *entry = &weatherSystem->entries[weather];

    if (entry->bgGfxId != 0xFFFF) {
        GfGfx_EngineATogglePlanes(4, 0);

        G2_SetBG2Priority(3);
        G2_SetBG0Priority(1);
        G2_BlendNone();
    }

    if (entry->weatherSpriteResources) {
        WeatherSystem_FreeSpriteResources(weatherSystem, entry->spriteResId, entry->weatherSpriteResources);
        Heap_FreeExplicit(HEAP_ID_FIELD1, entry->weatherSpriteResources);
        entry->weatherSpriteResources = NULL;
        if (entry->task) {
            SysTask_Destroy(entry->task);
            entry->task = NULL;
        }
    }

    if (entry->work) {
        WeatherObject_DeleteAll(&entry->work->linkedListDummy);

        if (entry->spriteResId != 0xFFFF) {
            WeatherWork_DeleteSprites(entry->work);
        }

        if (entry->work->loopSEPlaying == 1) {
            Weather_StopLoopSE(entry->work);
        }

        if (entry->work->taskData) {
            Heap_FreeExplicit(HEAP_ID_FIELD1, entry->work->taskData);
            entry->work->taskData = NULL;
        }

        if (entry->state == 1) {
            if (entry->task) {
                SysTask_Destroy(entry->task);
            }
        } else if (entry->state == 3) {
            SysTask_Destroy(entry->work->weatherTask);
        }

        if (entry->work->seDelayTask) {
            SysTask_Destroy(entry->work->seDelayTask);
        }

        Heap_FreeExplicit(HEAP_ID_FIELD1, entry->work);
        entry->work = NULL;
    }

    Fog_Set(weatherSystem->fieldSystem->fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
}

static void WeatherEntry_EndTask(WeatherEntry *entry) {
    if (entry->bgGfxId != 0xFFFF) {
        GfGfx_EngineATogglePlanes(4, 0);
        G2_SetBG2Priority(3);
        G2_SetBG0Priority(1);
        G2_BlendNone();
    }

    if (entry->work != NULL) {
        WeatherObject_DeleteAll(&entry->work->linkedListDummy);

        if (entry->work->loopSEPlaying == 1) {
            Weather_StopLoopSE(entry->work);
        }

        if (entry->state == 3) {
            SysTask_Destroy(entry->work->weatherTask);
            entry->state = 2;
        }
    }
}

static void WeatherSystem_LoadBgGfx(WeatherSystem *weatherSystem, u16 bgGfxId) {
    WeatherSystem_LoadBgPalette(weatherSystem, bgGfxId);
    WeatherSystem_LoadBgChars(weatherSystem, bgGfxId);
    WeatherSystem_LoadBgScreen(weatherSystem, bgGfxId);
}

static BOOL WeatherSystem_LoadSprites(WeatherSystem *weatherSystem, WeatherEntry *entry) {
    if (entry->spriteResId != 0xFFFF && entry->weatherSpriteResources == NULL) {
        if (WeatherEntry_AllocSpriteResources(entry) == FALSE) {
            return FALSE;
        }
        WeatherSystem_LoadSpriteResources(weatherSystem, entry->spriteResId, entry->weatherSpriteResources);
        WeatherSystem_InitSpriteTemplate(weatherSystem, entry);
    }
    return TRUE;
}

static void WeatherTask_LoadAsync(SysTask *task, void *data) {
    WeatherEntry *entry = data;
    WeatherSystem *weatherSystem = entry->work->weatherSystem;
    switch (entry->loadStep) {
    case 0:
        GF_ASSERT(WeatherEntry_AllocSpriteResources(entry));
        entry->loadStep++;
        break;
    case 1:
        WeatherSystem_LoadCellResource(weatherSystem, entry->spriteResId, entry->weatherSpriteResources);
        entry->loadStep++;
        break;
    case 2:
        WeatherSystem_LoadBgPalette(weatherSystem, entry->bgGfxId);
        entry->loadStep++;
        break;
    case 3:
        WeatherSystem_LoadAnimResource(weatherSystem, entry->spriteResId, entry->weatherSpriteResources);
        entry->loadStep++;
        break;
    case 4:
        WeatherSystem_LoadBgChars(weatherSystem, entry->bgGfxId);
        entry->loadStep++;
        break;
    case 5:
        WeatherSystem_LoadCharResource(weatherSystem, entry->spriteResId, entry->weatherSpriteResources);
        entry->loadStep++;
        break;
    case 6:
        WeatherSystem_LoadBgScreen(weatherSystem, entry->bgGfxId);
        entry->loadStep++;
        break;
    case 7:
        WeatherSystem_LoadPlttResource(weatherSystem, entry->spriteResId, entry->weatherSpriteResources);
        entry->loadStep++;
        break;
    case 8:
        WeatherSystem_InitSpriteTemplate(weatherSystem, entry);
        entry->work->spriteResources = entry->weatherSpriteResources;
        if (entry->spriteResId != 0xFFFF) {
            WeatherWork_CreateSprites(entry->work);
        }
        entry->state = 2;
        entry->loadStep = 0;
        entry->task = 0;
        SysTask_Destroy(task);
    }
}

static BOOL WeatherSystem_AllocWork(WeatherSystem *weatherSystem, WeatherEntry *entry) {
    if (entry->work) {
        return TRUE;
    }

    entry->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(WeatherWork));
    if (entry->work == NULL) {
        return FALSE;
    }
    memset(entry->work, 0, sizeof(WeatherWork));

    entry->work->weatherSystem = weatherSystem;
    entry->work->state = 0;
    entry->work->stopRequest = 0;
    entry->work->linkedListDummy.next = &entry->work->linkedListDummy;
    entry->work->linkedListDummy.prev = &entry->work->linkedListDummy;
    entry->work->taskData = NULL;
    entry->work->entry = entry;
    entry->work->seDelayTask = NULL;

    return TRUE;
}

static BOOL WeatherEntry_AllocSpriteResources(WeatherEntry *entry) {
    if (entry->spriteResId != 0xFFFF) {
        if (entry->weatherSpriteResources != NULL) {
            return TRUE;
        }
        entry->weatherSpriteResources = Heap_Alloc(HEAP_ID_FIELD1, sizeof(WeatherSpriteResources));
        if (entry->weatherSpriteResources == NULL) {
            return FALSE;
        }
        memset(entry->weatherSpriteResources, 0, sizeof(WeatherSpriteResources));
    }
    return TRUE;
}

static void WeatherSystem_LoadCellResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *spriteResources) {
    if (headerIndex != 0xFFFF) {
        spriteResources->charResObj[GF_GFX_RES_TYPE_CELL] = Weather_AddResObjFromOpenNarc(weatherSystem->weatherDraw.header, GF_GFX_RES_TYPE_CELL, headerIndex, weatherSystem->weatherDraw.resMan[GF_GFX_RES_TYPE_CELL], weatherSystem->narc, 0);
    }
}

static void WeatherSystem_LoadAnimResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *spriteResources) {
    if (headerIndex != 0xFFFF) {
        spriteResources->charResObj[GF_GFX_RES_TYPE_ANIM] = Weather_AddResObjFromOpenNarc(weatherSystem->weatherDraw.header, GF_GFX_RES_TYPE_ANIM, headerIndex, weatherSystem->weatherDraw.resMan[GF_GFX_RES_TYPE_ANIM], weatherSystem->narc, 0);
    }
}

static void WeatherSystem_LoadCharResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *spriteResources) {
    if (headerIndex != 0xFFFF) {
        spriteResources->charResObj[GF_GFX_RES_TYPE_CHAR] = Weather_AddResObjFromOpenNarc(weatherSystem->weatherDraw.header, GF_GFX_RES_TYPE_CHAR, headerIndex, weatherSystem->weatherDraw.resMan[GF_GFX_RES_TYPE_CHAR], weatherSystem->narc, 1);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(spriteResources->charResObj[GF_GFX_RES_TYPE_CHAR]);
        GF2DGfxResObj_FreeData(spriteResources->charResObj[GF_GFX_RES_TYPE_CHAR]);
    }
}

static void WeatherSystem_LoadPlttResource(WeatherSystem *weatherSystem, u32 headerIndex, WeatherSpriteResources *spriteResources) {
    if (headerIndex != 0xFFFF) {
        spriteResources->charResObj[GF_GFX_RES_TYPE_PLTT] = Weather_AddResObjFromOpenNarc(weatherSystem->weatherDraw.header, GF_GFX_RES_TYPE_PLTT, headerIndex, weatherSystem->weatherDraw.resMan[GF_GFX_RES_TYPE_PLTT], weatherSystem->narc, 1);
        SpriteTransfer_CreatePlttTransferTask(spriteResources->charResObj[GF_GFX_RES_TYPE_PLTT]);
        GF2DGfxResObj_FreeData(spriteResources->charResObj[GF_GFX_RES_TYPE_PLTT]);
    }
}

static void WeatherSystem_InitSpriteTemplate(WeatherSystem *weatherSystem, WeatherEntry *entry) {
    if (entry->spriteResId != 0xFFFF) {
        WeatherSystem_InitSpriteResourcesHeader(&entry->weatherSpriteResources->spriteResHeader, weatherSystem, entry->weatherSpriteResources, 0, 1);
        memset(&entry->weatherSpriteResources->spriteTemplate, 0, sizeof(SpriteTemplate));
        entry->weatherSpriteResources->spriteTemplate.spriteList = weatherSystem->weatherDraw.spriteList;
        entry->weatherSpriteResources->spriteTemplate.header = &entry->weatherSpriteResources->spriteResHeader;
        entry->weatherSpriteResources->spriteTemplate.scale.x = 1 << 12;
        entry->weatherSpriteResources->spriteTemplate.scale.y = 1 << 12;
        entry->weatherSpriteResources->spriteTemplate.scale.z = 1 << 12;
        entry->weatherSpriteResources->spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    }
}

static void WeatherWork_CreateSprites(WeatherWork *work) {
    for (int i = 0; i < 64; i++) {
        work->linkedList[i].sprite = Sprite_CreateAffine(&work->spriteResources->spriteTemplate);
        Sprite_SetDrawFlag(work->linkedList[i].sprite, 0);
        GF_ASSERT(work->linkedList[i].sprite);
    }
}

static void WeatherWork_DeleteSprites(WeatherWork *work) {
    for (int i = 0; i < 64; i++) {
        if (work->linkedList[i].sprite) {
            Sprite_Delete(work->linkedList[i].sprite);
            work->linkedList[i].sprite = NULL;
        }
    }
}

static void WeatherSystem_LoadBgPalette(WeatherSystem *weatherSystem, u16 bgGfxId) {
    WeatherBgGfxFiles files;
    if (bgGfxId != 0xFFFF) {
        files.paletteFile = NARC_AllocAndReadWholeMember(weatherSystem->narc, weatherSystem->bgGfx[bgGfxId].paletteId, HEAP_ID_FIELD1);
        NNS_G2dGetUnpackedPaletteData(files.paletteFile, &files.paletteData);
        BG_LoadPlttData(2, files.paletteData->pRawData, 32, 0xc0);
        Heap_Free(files.paletteFile);
        files.paletteFile = NULL;
    }
}

static void WeatherSystem_LoadBgChars(WeatherSystem *weatherSystem, u16 bgGfxId) {
    WeatherBgGfxFiles files;
    if (bgGfxId != 0xFFFF) {
        files.charFile = NARC_AllocAndReadWholeMember(weatherSystem->narc, weatherSystem->bgGfx[bgGfxId].charId, HEAP_ID_FIELD1);
        NNS_G2dGetUnpackedCharacterData(files.charFile, &files.charData);
        BG_LoadCharTilesData(weatherSystem->fieldSystem->bgConfig, 2, files.charData->pRawData, files.charData->szByte, 0);
        Heap_Free(files.charFile);
        files.charFile = NULL;
    }
}

static void WeatherSystem_LoadBgScreen(WeatherSystem *weatherSystem, u16 bgGfxId) {
    WeatherBgGfxFiles files;
    if (bgGfxId != 0xFFFF) {
        GfGfx_EngineATogglePlanes(4, 0);
        files.screenFile = NARC_AllocAndReadWholeMember(weatherSystem->narc, weatherSystem->bgGfx[bgGfxId].screenId, HEAP_ID_FIELD1);
        GF_ASSERT(files.screenFile);
        NNS_G2dGetUnpackedScreenData(files.screenFile, &files.screenData);
        BgCopyOrUncompressTilemapBufferRangeToVram(weatherSystem->fieldSystem->bgConfig, 2, files.screenData->rawData, files.screenData->szByte, 0);
        BG_LoadScreenTilemapData(weatherSystem->fieldSystem->bgConfig, 2, files.screenData->rawData, files.screenData->szByte);
        BgTilemapRectChangePalette(weatherSystem->fieldSystem->bgConfig, 2, 0, 0, 32, 32, 6);
        BgCommitTilemapBufferToVram(weatherSystem->fieldSystem->bgConfig, 2);
        Heap_Free(files.screenFile);
        files.screenFile = NULL;
    }
}

static void WeatherObject_InitData(WeatherObject *object, int size) {
    GF_ASSERT(object->data == 0);
    GF_ASSERT(size > 0);
    GF_ASSERT(size <= 0x28u);
    object->data = object->dataBuffer;
}

static void WeatherObject_ClearData(WeatherObject *object) {
    object->data = NULL;
    memset(object->dataBuffer, 0, 10 * sizeof(u32));
}

static void *WeatherSystem_AddObject(WeatherWork *work, int size) {
    WeatherSystem *weatherSystem = work->weatherSystem;
    WeatherObject *object = WeatherSystem_GetFreeObject(work);
    if (object == NULL) {
        return NULL;
    }
    object->weatherSystem = weatherSystem;
    object->next = &work->linkedListDummy;
    object->prev = work->linkedListDummy.prev;
    work->linkedListDummy.prev->next = object;
    work->linkedListDummy.prev = object;

    WeatherObject_InitData(object, size);
    if (object->data == NULL) {
        return NULL;
    }

    GF_ASSERT(object->sprite);
    Sprite_SetDrawFlag(object->sprite, 1);
    return object;
}

static void WeatherSystem_InitSpriteResourcesHeader(SpriteResourcesHeader *header, WeatherSystem *weatherSystem, WeatherSpriteResources *spriteResources, u32 transfer, u32 priority) {
    int resID[4];

    for (int i = 0; i < 4; i++) {
        resID[i] = GF2DGfxResObj_GetResID(spriteResources->charResObj[i]);
    }

    CreateSpriteResourcesHeader(header, resID[0], resID[1], resID[2], resID[3], -1, -1, transfer, priority, weatherSystem->weatherDraw.resMan[0], weatherSystem->weatherDraw.resMan[1], weatherSystem->weatherDraw.resMan[2], weatherSystem->weatherDraw.resMan[3], NULL, NULL);
}

static void WeatherObject_Delete(WeatherObject *object) {
    object->prev->next = object->next;
    object->next->prev = object->prev;

    Sprite_SetDrawFlag(object->sprite, 0);
    WeatherObject_ClearData(object);

    Sprite *temp = object->sprite;
    memset(object, 0, sizeof(WeatherObject));
    object->sprite = temp;
}

static void WeatherObject_DeleteAll(WeatherObject *head) {
    WeatherObject *cur = head->next;
    WeatherObject *next;

    while (cur != head) {
        next = cur->next;
        WeatherObject_Delete(cur);
        cur = next;
    }
}

static void WeatherObject_ForEach(WeatherObject *head, WeatherObjectFunc func) {
    WeatherObject *cur = head->next;
    WeatherObject *next = cur->next;

    while (cur != head) {
        func(cur);
        cur = next;
        next = cur->next;
    }
}

static void WeatherWork_Nop(void *data) {
}

static VecFx32 WeatherObject_GetPosition(WeatherObject *object) {
    return *Sprite_GetMatrixPtr(object->sprite);
}

static void WeatherWork_GetCameraScroll(fx32 *x, fx32 *z, WeatherWork *work) {
    VecFx32 cameraTarget = NNS_G3dGlb.camTarget;
    fx32 xScale, zScale;
    fx32 dx, dz;
    fx32 xDist, zDist;
    int sign;
    fx32 aspect;

    xDist = (cameraTarget.x - work->cameraTarget.x);
    zDist = (cameraTarget.z - work->cameraTarget.z);

    aspect = FX_Div(FX32_CONST(4), FX32_CONST(3));

    Math_CalcPerspectiveViewSize(Camera_GetPerspectiveAngle(work->weatherSystem->fieldSystem->camera), Camera_GetDistance(work->weatherSystem->fieldSystem->camera), aspect, &dx, &dz);

    dx = FX_Div(dx, 256 * FX32_ONE);

    if (zDist <= 0) {
        dz = FX_Div(dz, 0xbe8d0);
    } else {
        dz = FX_Div(dz, 0xbe811);
    }

    sign = FX32_ONE;
    if (xDist < 0) {
        sign = -FX32_ONE;
        xDist = FX_MUL(xDist, -FX32_ONE);
    }
    xScale = FX_Div(xDist, dx);
    if (sign < 0) {
        xScale = FX_MUL(xScale, sign);
    }

    sign = FX32_ONE;
    if (zDist < 0) {
        sign = -FX32_ONE;
        zDist = FX_MUL(zDist, -FX32_ONE);
    }
    zScale = FX_Div(zDist, dz);
    if (sign < 0) {
        zScale = FX_MUL(zScale, sign);
    }

    if (xScale + zScale != 0) {
        work->cameraTarget = cameraTarget;
    }

    *x = xScale;
    *z = zScale;
}

static void WeatherWork_ScrollObjectsInt(WeatherWork *work, int *xOut, int *zOut) {
    fx32 x, z;
    WeatherWork_ScrollObjects(work, &x, &z);

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

static void WeatherWork_ScrollObjects(WeatherWork *work, fx32 *x, fx32 *y) {
    WeatherObject *cur;
    fx32 xScale, yScale;
    VecFx32 matrix;
    WeatherWork_GetCameraScroll(&xScale, &yScale, work);

    cur = work->linkedListDummy.next;
    while (cur != &work->linkedListDummy) {
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

static void WeatherSpawnRamp_Init(WeatherSpawnRamp *ramp, WeatherWork *work, s16 count, s16 interval, s32 targetCount, s32 targetInterval, s32 intervalStep, s32 countStepSpawns, s32 countStep, WeatherSpawnFunc spawnFunc) {
    ramp->work = work;
    ramp->count = count;
    ramp->timer = 0;
    ramp->interval = interval;
    ramp->countTimer = 0;
    ramp->targetCount = (s16)targetCount;
    ramp->targetInterval = (s16)targetInterval;
    ramp->intervalStep = (s16)intervalStep;
    ramp->countStepSpawns = (s16)countStepSpawns;
    ramp->countStep = countStep;
    ramp->spawnFunc = spawnFunc;
}

static void WeatherSpawnRamp_SetTarget(WeatherSpawnRamp *ramp, s16 targetCount, s16 targetInterval, s16 intervalStep, s32 countStep) {
    ramp->targetCount = targetCount;
    ramp->targetInterval = targetInterval;
    ramp->intervalStep = intervalStep;
    ramp->countStep = countStep;
}

static s32 WeatherSpawnRamp_Update(WeatherSpawnRamp *ramp) {
    s16 intervalStep;
    s16 interval;
    s16 count;
    s16 targetInterval;
    s16 targetCount;
    s32 countStep;
    s32 intervalDecreasing;
    s32 countDecreasing;
    s32 doneFlags;

    intervalStep = ramp->intervalStep;
    doneFlags = 0;
    if (intervalStep == 0) {
        doneFlags |= 1;
    } else {
        if ((s32)intervalStep <= 0) {
            intervalDecreasing = 1;
        } else {
            intervalDecreasing = 0;
        }
        targetInterval = ramp->targetInterval;
        interval = ramp->interval;
        if ((((s32)interval >= (s32)targetInterval) && (intervalDecreasing == 0)) || (((s32)interval <= (s32)targetInterval) && (intervalDecreasing == 1))) {
            doneFlags |= 1;
            ramp->interval = targetInterval;
        }
    }
    countStep = ramp->countStep;
    if (countStep == 0) {
        doneFlags |= 2;
    } else {
        if (countStep <= 0) {
            countDecreasing = 1;
        } else {
            countDecreasing = 0;
        }
        targetCount = ramp->targetCount;
        count = ramp->count;
        if ((((s32)count >= (s32)targetCount) && (countDecreasing == 0)) || (((s32)count <= (s32)targetCount) && (countDecreasing == 1))) {
            doneFlags |= 2;
            ramp->count = targetCount;
        }
    }
    ramp->timer = (s16)(ramp->timer - 1);
    if ((s32)ramp->timer <= 0) {
        ramp->spawnFunc(ramp->work, ramp->count);
        ramp->timer = (s16)ramp->interval;
        if (!(1 & doneFlags)) {
            ramp->interval = (s16)(ramp->interval + ramp->intervalStep);
        }
        if (!(2 & doneFlags)) {
            ramp->countTimer = (s16)(ramp->countTimer + 1);
            if ((s32)ramp->countTimer >= (s32)ramp->countStepSpawns) {
                ramp->countTimer = 0;
                ramp->count = (s16)(ramp->count + ramp->countStep);
            }
        }
    }
    return doneFlags;
}

static void WeatherFog_Start(WeatherFogChange *fogChange, WeatherFogTableFade *tableFade, FogData *fog, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb, s32 framesPerStep, s32 fogMode) {
    tableFade->fog = fog;
    if (fogMode != 0) {
        if (fogMode == 1) {
            Weather_SetFog(fog, fogSlope, fogOffset, rgb);
            WeatherFogTableFade_Clear(tableFade);
            WeatherFogTableFade_Start(tableFade, framesPerStep, 1);
        } else if (fogMode == 2) {
            WeatherFogChange_Init(fogChange, fog, fogSlope, fogOffset, rgb, 0x7F * framesPerStep);
        }
    }
}

static BOOL WeatherFog_Update(WeatherFogChange *fogChange, WeatherFogTableFade *tableFade, s32 fogMode) {
    s32 done;

    done = 1;
    if (fogMode != 0) {
        if (fogMode == 1) {
            done = WeatherFogTableFade_Update(tableFade);
        } else if (fogMode == 2) {
            if (WeatherFogChange_Step(fogChange) == 0) {
                done = 0;
            }
        }
    }
    return done;
}

static void Weather_SetFog(FogData *fog, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb) {
    Fog_Set(fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, fogSlope, fogOffset);
    Fog_SetColor(fog, -1, rgb, 0x1F);
}

static void WeatherFogChange_Init(WeatherFogChange *fogChange, FogData *fog, s32 targetSlope, s32 targetOffset, GXRgb rgb, s32 duration) {
    int slope = Fog_GetSlope(fog);
    int offset = Fog_GetOffset(fog);
    GXRgb rgb2 = Fog_GetColor(fog);

    fogChange->fog = fog;

    WeatherLerp_Init(&fogChange->offset, offset, targetOffset, duration);
    WeatherLerp_Init(&fogChange->red, rgb2 & GX_RGB_R_MASK, rgb & GX_RGB_R_MASK, duration);
    WeatherLerp_Init(&fogChange->green, (rgb2 >> GX_RGB_G_SHIFT) & 0x1F, (rgb >> GX_RGB_G_SHIFT) & 0x1F, duration);
    WeatherLerp_Init(&fogChange->blue, (rgb2 >> GX_RGB_B_SHIFT) & 0x1F, (rgb >> GX_RGB_B_SHIFT) & 0x1F, duration);
    WeatherLerp_Init(&fogChange->slope, slope, targetSlope, duration);
}

static BOOL WeatherFogChange_Step(WeatherFogChange *fogChange) {
    BOOL ret = WeatherLerp_Step(&fogChange->offset);
    WeatherLerp_Step(&fogChange->red);
    WeatherLerp_Step(&fogChange->green);
    WeatherLerp_Step(&fogChange->blue);
    WeatherLerp_Step(&fogChange->slope);
    Weather_SetFog(fogChange->fog, (GXFogSlope)fogChange->slope.value, fogChange->offset.value, GX_RGB(fogChange->red.value, fogChange->green.value, fogChange->blue.value));

    return ret;
}

static void WeatherFogTableFade_Clear(WeatherFogTableFade *tableFade) {
    for (int i = 0; i < 32; i++) {
        tableFade->table[i] = 0;
    }
    Fog_SetTable(tableFade->fog, tableFade->table);
}

static void WeatherFogTableFade_Start(WeatherFogTableFade *tableFade, s32 framesPerStep, s32 direction) {
    tableFade->stepsLeft = 0x7F;
    tableFade->frameTimer = 0;
    tableFade->framesPerStep = framesPerStep;
    if (direction == 1) {
        tableFade->direction = 1;
        return;
    }
    tableFade->direction = -1;
}

static s32 WeatherFogTableFade_Update(WeatherFogTableFade *tableFade) {
    s32 done;

    done = WeatherFogTableFade_Step(tableFade);
    if (tableFade->frameTimer == 0) {
        Fog_SetTable(tableFade->fog, tableFade->table);
    }
    return done;
}

static void WeatherFogTableFade_SetFull(WeatherFogTableFade *tableFade) {
    tableFade->stepsLeft = 1;
    tableFade->frameTimer = 0;
    tableFade->framesPerStep = 0;
    tableFade->direction = 1;
    WeatherFogTableFade_BuildTable(tableFade);
    Fog_SetTable(tableFade->fog, tableFade->table);
}

static s32 WeatherFogTableFade_Step(WeatherFogTableFade *tableFade) {
    s32 stepsLeft;
    s32 frameTimer;
    s32 done;

    done = 0;
    if (tableFade->direction == 0) {
        done = 1;
    } else if ((s32)tableFade->stepsLeft <= 0) {
        done = 1;
    } else {
        frameTimer = tableFade->frameTimer + 1;
        tableFade->frameTimer = frameTimer;
        if (frameTimer >= (s32)tableFade->framesPerStep) {
            tableFade->frameTimer = 0;
            WeatherFogTableFade_BuildTable(tableFade);
            stepsLeft = tableFade->stepsLeft - 1;
            tableFade->stepsLeft = stepsLeft;
            if (stepsLeft <= 0) {
                done = 1;
            }
        }
    }
    return done;
}

static void WeatherFogTableFade_BuildTable(WeatherFogTableFade *tableFade) {
    int i;
    int minIndex;
    int steps;

    if (tableFade->direction < 0) {
        steps = 0x7F - tableFade->stepsLeft;
    } else {
        steps = tableFade->stepsLeft;
    }

    minIndex = steps / 4;
    for (i = 31; i > minIndex; i--) {
        tableFade->table[i] = (i - minIndex) * 4;
    }
}

static void Weather_SpawnAndPreAdvance(WeatherWork *work, WeatherSpawnFunc spawnFunc, int count, int groupSize, int advanceStep, WeatherObjectFunc updateFunc) {
    spawnFunc(work, count);

    int i, j;
    int advanceCount = 0;
    WeatherObject *cur = work->linkedListDummy.next;
    WeatherObject *next = cur->next;

    for (i = 0; i < count; i++) {
        if (cur == &work->linkedListDummy) {
            break;
        }

        for (j = 0; j < advanceCount; j++) {
            updateFunc(cur);

            if (cur->data == NULL) {
                break;
            }
        }

        cur = next;
        next = cur->next;

        if (i >= groupSize && (i % groupSize) == 0) {
            advanceCount += advanceStep;
        }
    }
}

static WeatherObject *WeatherSystem_GetFreeObject(WeatherWork *work) {
    for (int i = 0; i < 64; i++) {
        if (work->linkedList[i].weatherSystem == NULL) {
            return &work->linkedList[i];
        }
    }
    return NULL;
}

static void WeatherTask_Sunny(SysTask *task, void *data) {
    WeatherWork *work = data;
    u16 state = work->state;
    switch (state) {
    case 0:
        work->state = 1;
        break;
    case 1:
        work->state = 3;
        break;
    case 2:
        work->state = 3;
        break;
    case 3:
        if (work->stopRequest == 5) {
            work->state = 4;
        }
        break;
    case 4:
        work->state = 5;
        break;
    case 5:
        WeatherEntry_EndTask(work->entry);
        break;
    }
}

static void WeatherTask_Rain(SysTask *task, void *data) {
    WeatherWork *work = data;

    WeatherParticleTaskData *taskData = work->taskData;

    switch (work->state) {
    case 0:
        WeatherSpawnRamp_Init(&taskData->spawnRamp, work, 1, 8, 20, 1, -1, 1, 2, Rain_AddDrops);
        WeatherFog_Start(&taskData->fogChange, &taskData->fogTableFade, work->weatherSystem->fieldSystem->fog, GX_FOGSLOPE_0x1000, 0x726F, 0x6b5a, 1, work->fogMode);

        taskData->data[0] = 0;
        work->seDelayTask = SysTask_CreateOnMainQueue(WeatherTask_DelayedRainSE, work, 100);
        work->seDelay = 30;
        work->state = 1;
        break;
    case 1:
        int rampDone = WeatherSpawnRamp_Update(&taskData->spawnRamp);
        if (taskData->data[0] > 0) {
            taskData->data[0]--;
        } else {
            int fogDone = WeatherFog_Update(&taskData->fogChange, &taskData->fogTableFade, work->fogMode);
            if (fogDone == 1 && rampDone == 3) {
                work->state = 3;
            }
        }
        break;
    case 2:
        WeatherSpawnRamp_Init(&taskData->spawnRamp, work, 20, 1, 20, 1, -1, 1, 2, Rain_AddDrops);

        if (work->fogMode) {
            taskData->fogTableFade.fog = work->weatherSystem->fieldSystem->fog;
            Weather_SetFog(taskData->fogTableFade.fog, GX_FOGSLOPE_0x1000, 0x726f, GX_RGB(26, 26, 26));
            WeatherFogTableFade_SetFull(&taskData->fogTableFade);
        }

        Weather_SpawnAndPreAdvance(work, Rain_AddDrops, 20, 10, 1, Rain_UpdateDrop);

        work->seDelayTask = SysTask_CreateOnMainQueue(WeatherTask_DelayedRainSE, work, 100);
        work->seDelay = 30;
        work->state = 3;
        break;
    case 3:
        if (taskData->spawnRamp.timer-- <= 0) {
            Rain_AddDrops(work, taskData->spawnRamp.count);
            taskData->spawnRamp.timer = taskData->spawnRamp.interval;
        }

        if (work->stopRequest == 5) {
            WeatherSpawnRamp_SetTarget(&taskData->spawnRamp, 0, 8, 1, -2);

            if (work->fogMode) {
                WeatherFogTableFade_Start(&taskData->fogTableFade, 1, 0);
            }
            taskData->data[0] = 0;
            work->state = 4;
            Weather_StopLoopSE(work);
        }
        break;
    case 4:
        int ret = WeatherSpawnRamp_Update(&taskData->spawnRamp);
        if (taskData->data[0] > 0) {
            taskData->data[0]--;
        } else {
            int vUnk;
            if (work->fogMode) {
                vUnk = WeatherFogTableFade_Update(&taskData->fogTableFade);
            } else {
                vUnk = 1;
            }

            if (vUnk == 1 && ret == 3) {
                if (work->linkedListDummy.next == &work->linkedListDummy) {
                    work->state = 5;
                }
            }
        }
        break;
    case 5:
        if (work->fogMode) {
            Fog_Set(taskData->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(work->entry);
        break;
    }

    if (work->state != 5 && work->state != 0) {
        WeatherObject_ForEach(&work->linkedListDummy, Rain_UpdateDrop);
        WeatherWork_ScrollObjectsInt(work, NULL, NULL);
        WeatherWork_Nop(work);
    }
}

static void Rain_AddDrops(WeatherWork *work, int count) {
    for (int i = 0; i < count; i++) {
        WeatherObject *object = WeatherSystem_AddObject(work, 32);
        if (object == NULL) {
            break;
        }

        u32 *data = object->data;
        u32 rand = MTRandom();

        data[0] = 0;
        int frame = rand % 3;
        Sprite_SetAnimationFrame(object->sprite, frame);

        int jitter = rand % 20;
        data[2] = 10 * (frame + 1) + jitter;
        if (frame == 2) {
            data[2] += 10;
        }

        jitter /= -5;
        data[4] = -5 * (frame + 1) + jitter;
        if (frame == 2) {
            data[4] -= 5;
        }
        data[3] = 0;
        data[1] = 1 + (rand % 3);

        VecFx32 vec;

        vec.x = (15 * frame + (rand % 270)) << FX32_SHIFT;
        vec.y = 0xFFFA0 << FX32_SHIFT;
        vec.z = 0;
        WeatherDraw_SetSpriteMatrix(object->sprite, &vec);
    }
}

static void Rain_UpdateDrop(WeatherObject *object) {
    int i;
    s32 *data = object->data;
    VecFx32 vec;
    vec = WeatherObject_GetPosition(object);
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
                    Sprite_SetAnimationFrame(object->sprite, 3);
                }
            }
        }
        WeatherDraw_SetSpriteMatrix(object->sprite, &vec);
        break;
    case 1:
        if (data[0]-- <= 0) {
            data[3] = 2;
        }
        break;
    case 2:
        WeatherObject_Delete(object);
        break;
    }
}

static void WeatherTask_Snow(SysTask *task, void *data) {
    WeatherWork *work = data;

    WeatherParticleTaskData *taskData = work->taskData;

    switch (work->state) {
    case 0:
        WeatherSpawnRamp_Init(&taskData->spawnRamp, work, 1, 30, 6, 3, -5, 2, 1, Snow_AddFlakes);

        WeatherFog_Start(&taskData->fogChange, &taskData->fogTableFade, work->weatherSystem->fieldSystem->fog, GX_FOGSLOPE_0x1000, 0x726F, GX_RGB(24, 24, 24), 2, work->fogMode);

        taskData->data[0] = 8;
        taskData->data[1] = 0;
        work->state = 1;
        break;
    case 1:
        int rampDone = WeatherSpawnRamp_Update(&taskData->spawnRamp);
        if (taskData->data[0] > 0) {
            taskData->data[0]--;
        } else {
            int fogDone = WeatherFog_Update(&taskData->fogChange, &taskData->fogTableFade, work->fogMode);
            if (fogDone == 1 && rampDone == 3) {
                work->state = 3;
            }
        }
        break;
    case 2:
        WeatherSpawnRamp_Init(&taskData->spawnRamp, work, 6, 3, 6, 3, -5, 2, 1, Snow_AddFlakes);

        if (work->fogMode) {
            taskData->fogTableFade.fog = work->weatherSystem->fieldSystem->fog;
            Weather_SetFog(taskData->fogTableFade.fog, GX_FOGSLOPE_0x1000, 0x726f, GX_RGB(24, 24, 24));
            WeatherFogTableFade_SetFull(&taskData->fogTableFade);
        }

        taskData->data[1] = 0;

        Weather_SpawnAndPreAdvance(work, Snow_AddFlakes, 20, 2, 3, Snow_UpdateFlake);

        work->state = 3;
        break;
    case 3:
        if (taskData->spawnRamp.timer-- <= 0) {
            Snow_AddFlakes(work, taskData->spawnRamp.count);
            taskData->spawnRamp.timer = taskData->spawnRamp.interval;
        }

        if (work->stopRequest == 5) {
            WeatherSpawnRamp_SetTarget(&taskData->spawnRamp, 0, 30, 5, -3);

            if (work->fogMode) {
                WeatherFogTableFade_Start(&taskData->fogTableFade, 1, 0);
            }
            taskData->data[0] = 0;
            work->state = 4;
        }
        break;
    case 4:
        int ret = WeatherSpawnRamp_Update(&taskData->spawnRamp);
        if (taskData->data[0] > 0) {
            taskData->data[0]--;
        } else {
            int vUnk;
            if (work->fogMode) {
                vUnk = WeatherFogTableFade_Update(&taskData->fogTableFade);
            } else {
                vUnk = 1;
            }

            if (vUnk == 1 && ret == 3) {
                if (work->linkedListDummy.next == &work->linkedListDummy) {
                    work->state = 5;
                }
            }
        }
        break;
    case 5:
        if (work->fogMode) {
            Fog_Set(taskData->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(work->entry);
        break;
    }

    if (work->state != 5 && work->state != 0) {
        WeatherObject_ForEach(&work->linkedListDummy, Snow_UpdateFlake);
        WeatherWork_ScrollObjectsInt(work, NULL, NULL);
        WeatherWork_Nop(work);
    }
}

static const sSnowFallSpeeds[] = { 2, 2, 2, 2 };
static const sSnowSlowdownIntervals[] = { 16, 32, 16, 10 };

static void Snow_AddFlakes(WeatherWork *work, int count) {
    int table0[4];
    table0 = sSnowSlowdownIntervals;
    int table1[4];
    table1 = sSnowFallSpeeds;
    WeatherParticleTaskData *taskData = work->taskData;

    for (int i = 0; i < count; i++) {
        WeatherObject *object = WeatherSystem_AddObject(work, 32);
        if (object == NULL) {
            break;
        }

        s32 *data = object->data;

        taskData->data[1]++;
        if (taskData->data[1] >= 800) {
            taskData->data[1] = 0;
        }
        int index = taskData->data[1] / 200;
        data[5] = table0[index];

        data[0] = 0;
        data[1] = 4 + (MTRandom() % 42);

        int frame = (data[1] - 4) / 15;
        Sprite_SetAnimationFrame(object->sprite, frame);

        data[4] = -(frame + 1);
        data[2] = table1[index] * (frame + 1);
        data[3] = 0;

        VecFx32 vec = WeatherObject_GetPosition(object);

        vec.x = -20 + (frame * 20) + (MTRandom() % 420);
        vec.y = -8;
        vec.z = 0;
        vec.x <<= FX32_SHIFT;
        vec.y <<= FX32_SHIFT;

        WeatherDraw_SetSpriteMatrix(object->sprite, &vec);
    }
}

static void Snow_UpdateFlake(WeatherObject *object) {
    s32 *data = object->data;
    VecFx32 vec = WeatherObject_GetPosition(object);

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

        WeatherDraw_SetSpriteMatrix(object->sprite, &vec);
        break;
    case 1:
        WeatherObject_Delete(object);
        break;
    }
}

static void WeatherTask_DiamondDust(SysTask *task, void *data) {
    WeatherWork *work = data;

    WeatherParticleTaskData *taskData = work->taskData;

    switch (work->state) {
    case 0:
        WeatherSpawnRamp_Init(&taskData->spawnRamp, work, 2, 16, 20, 2, -2, 4, 2, DiamondDust_AddSparkles);

        WeatherFog_Start(&taskData->fogChange, &taskData->fogTableFade, work->weatherSystem->fieldSystem->fog, GX_FOGSLOPE_0x1000, 0x716F, GX_RGB(26, 26, 26), 1, work->fogMode);

        taskData->data[0] = 0;
        work->state = 1;
        break;
    case 1:
        int rampDone = WeatherSpawnRamp_Update(&taskData->spawnRamp);
        if (taskData->data[0] > 0) {
            taskData->data[0]--;
        } else {
            int fogDone = WeatherFog_Update(&taskData->fogChange, &taskData->fogTableFade, work->fogMode);
            if (fogDone == 1 && rampDone == 3) {
                work->state = 3;
            }
        }
        break;
    case 2:
        WeatherSpawnRamp_Init(&taskData->spawnRamp, work, 20, 2, 20, 2, -2, 4, 2, DiamondDust_AddSparkles);

        if (work->fogMode) {
            taskData->fogTableFade.fog = work->weatherSystem->fieldSystem->fog;
            Weather_SetFog(taskData->fogTableFade.fog, GX_FOGSLOPE_0x1000, 0x716f, GX_RGB(26, 26, 26));
            WeatherFogTableFade_SetFull(&taskData->fogTableFade);
        }

        Weather_SpawnAndPreAdvance(work, DiamondDust_AddSparkles, 20, 10, 1, DiamondDust_UpdateSparkle);

        work->state = 3;
        break;
    case 3:
        if (taskData->spawnRamp.timer-- <= 0) {
            DiamondDust_AddSparkles(work, taskData->spawnRamp.count);
            taskData->spawnRamp.timer = taskData->spawnRamp.interval;
        }

        if (work->stopRequest == 5) {
            WeatherSpawnRamp_SetTarget(&taskData->spawnRamp, 0, 16, 6, -10);

            if (work->fogMode) {
                WeatherFogTableFade_Start(&taskData->fogTableFade, 1, 0);
            }
            taskData->data[0] = 20;
            work->state = 4;
        }
        break;
    case 4:
        int ret = WeatherSpawnRamp_Update(&taskData->spawnRamp);
        if (taskData->data[0] > 0) {
            taskData->data[0]--;
        } else {
            int vUnk = WeatherFog_Update(&taskData->fogChange, &taskData->fogTableFade, work->fogMode);

            if (vUnk == 1 && ret == 3) {
                if (work->linkedListDummy.next == &work->linkedListDummy) {
                    work->state = 5;
                }
            }
        }
        break;
    case 5:
        if (work->fogMode) {
            Fog_Set(taskData->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(work->entry);
        break;
    }

    if (work->state != 5 && work->state != 0) {
        WeatherObject_ForEach(&work->linkedListDummy, DiamondDust_UpdateSparkle);
        WeatherWork_ScrollObjectsInt(work, NULL, NULL);
        WeatherWork_Nop(work);
    }
}

static void DiamondDust_AddSparkles(WeatherWork *work, int count) {
    VecFx32 vec;
    for (int i = 0; i < count; i++) {
        WeatherObject *object = WeatherSystem_AddObject(work, 32);
        if (object == NULL) {
            break;
        }

        u32 *data = object->data;

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

        vec = WeatherObject_GetPosition(object);
        vec.x = -64 + (MTRandom() % 384);
        vec.y = -8 + ((u32)MTRandom() % 256);
        vec.x <<= FX32_SHIFT;
        vec.y <<= FX32_SHIFT;
        vec.z = 0;
        WeatherDraw_SetSpriteMatrix(object->sprite, &vec);
        vec.x >>= FX32_SHIFT;
        vec.y >>= FX32_SHIFT;

        int bandStart = 50 - vec.x / 3;
        int bandRange = 206 - vec.x / 3;
        int bandEnd;
        if (bandRange < 0) {
            bandRange *= -1;
            bandEnd = bandStart - (MTRandom() % bandRange);
        } else {
            bandEnd = bandStart + (MTRandom() % bandRange);
        }

        if (bandStart <= vec.y && bandEnd >= vec.y) {
            data[1] *= 2;
        } else {
            frame = MTRandom() & 3;
        }
        Sprite_SetAnimationFrame(object->sprite, frame);
    }
}

static void DiamondDust_UpdateSparkle(WeatherObject *object) {
    s32 *data = object->data;
    WeatherObject_GetPosition(object);
    if (++data[0] >= data[1]) {
        WeatherObject_Delete(object);
    }
}

static void WeatherTask_RunFogOnly(WeatherWork *work, WeatherFogTaskData *taskData, GXFogSlope fogSlope, s32 fogOffset, GXRgb rgb, s32 framesPerStep, s32 stopFramesPerStep) {
    switch (work->state) {
    case 0:
        WeatherFog_Start(&taskData->fogChange, &taskData->fogTableFade, work->weatherSystem->fieldSystem->fog, fogSlope, fogOffset, rgb, framesPerStep, work->fogMode);
        work->state = 1;
        break;
    case 1:
        int fogDone = WeatherFog_Update(&taskData->fogChange, &taskData->fogTableFade, work->fogMode);
        if (fogDone == 1) {
            work->state = 3;
        }
        break;
    case 2:
        if (work->fogMode != 0) {
            taskData->fogTableFade.fog = work->weatherSystem->fieldSystem->fog;
            Weather_SetFog(taskData->fogTableFade.fog, fogSlope, fogOffset, rgb);
            WeatherFogTableFade_SetFull(&taskData->fogTableFade);
        }
        work->state = 3;
        break;
    case 3:
        if (work->stopRequest == 5) {
            if (work->fogMode != 0) {
                WeatherFogTableFade_Start(&taskData->fogTableFade, stopFramesPerStep, 0);
            }
            work->state = 4;
        }
        break;
    case 4:
        int ret;
        if (work->fogMode != 0) {
            ret = WeatherFogTableFade_Update(&taskData->fogTableFade);
        } else {
            ret = 1;
        }

        if (ret == 1) {
            work->state = 5;
        }
        break;
    case 5:
        if (work->fogMode != 0) {
            Fog_Set(taskData->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(work->entry);
        break;
    }
}

static void WeatherTask_Mist(SysTask *task, void *data) {
    WeatherWork *work = data;
    WeatherMistTaskData *taskData = work->taskData;

    switch (work->state) {
    case 0:
        WeatherFog_Start(&taskData->fogChange, &taskData->fogTableFade, work->weatherSystem->fieldSystem->fog, GX_FOGSLOPE_0x0200, 0x7555, 0x7fff, 1, work->fogMode);
        WeatherLerp_Init(&taskData->blendLerp, 0, 9, 30);
        Weather_SetBlendAlpha(0, 16);
        GfGfx_EngineATogglePlanes(4, 1);
        work->state = 1;
        break;
    case 1:
        int fogDone = WeatherFogTableFade_Update(&taskData->fogTableFade);
        int blendDone = WeatherLerp_Step(&taskData->blendLerp);
        Weather_SetBlendAlpha(taskData->blendLerp.value, 16 - taskData->blendLerp.value);
        if (fogDone == 1 && blendDone == 1) {
            work->state = 3;
        }
        break;
    case 2:
        if (work->fogMode != 0) {
            taskData->fogTableFade.fog = work->weatherSystem->fieldSystem->fog;
            Weather_SetFog(taskData->fogTableFade.fog, GX_FOGSLOPE_0x0200, 0x7555, 0x7fff);
            WeatherFogTableFade_SetFull(&taskData->fogTableFade);
        }
        Weather_SetBlendAlpha(9, 7);
        GfGfx_EngineATogglePlanes(4, 1);
        work->state = 3;
        break;
    case 3:
        if (work->stopRequest == 5) {
            if (work->fogMode != 0) {
                WeatherFogTableFade_Start(&taskData->fogTableFade, 1, 0);
            }
            WeatherLerp_Init(&taskData->blendLerp, 9, 0, 30);
            work->state = 4;
        }
        break;
    case 4:
        int fogOutDone;
        if (work->fogMode != 0) {
            fogOutDone = WeatherFogTableFade_Update(&taskData->fogTableFade);
        } else {
            fogOutDone = 1;
        }

        int blendOutDone = WeatherLerp_Step(&taskData->blendLerp);
        Weather_SetBlendAlpha(taskData->blendLerp.value, 16 - taskData->blendLerp.value);
        if (fogOutDone == 1 && blendOutDone == 1) {
            work->state = 5;
        }
        break;
    case 5:
        if (work->fogMode != 0) {
            Fog_Set(taskData->fogTableFade.fog, 1, FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
        }
        WeatherEntry_EndTask(work->entry);
        break;
    }
}

static void WeatherTask_Flash(SysTask *task, void *data) {
    WeatherWork *work = data;
    WeatherSystem *weatherSystem = work->weatherSystem;
    FieldSystem *fieldSystem = weatherSystem->fieldSystem;
    WeatherFlashTaskData *taskData = work->taskData;

    switch (work->state) {
    case 0:
        taskData->darkness = FlashDarkness_Show(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
        Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
        Fog_SetColor(fieldSystem->fog, -1, 0, 0);
        {
            s8 fogTable[32];
            for (int i = 0; i < 32; i++) {
                fogTable[i] = -1;
            }
            Fog_SetTable(fieldSystem->fog, fogTable);
        }
        taskData->scaleFrame = 0;
        work->state = 1;
        break;
    case 1:
        work->state = 3;
        break;
    case 2:
        taskData->darkness = FlashDarkness_Show(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
        Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
        Fog_SetColor(fieldSystem->fog, -1, 0, 0);
        {
            s8 fogTable[32];
            for (int i = 0; i < 32; i++) {
                fogTable[i] = -1;
            }
            Fog_SetTable(fieldSystem->fog, fogTable);
        }
        taskData->scaleFrame = 0;
        work->state = 3;
        break;
    case 3:
        if (work->stopRequest == 5) {
            taskData->endStep = 0;
            work->state = 4;
        }
        break;
    case 4:
        switch (taskData->endStep) {
        case 0:
            BeginNormalPaletteFade(3, 0, 0, 0x7FFF, 6, 1, HEAP_ID_FIELD1);
            taskData->endStep++;
            break;
        case 1:
            if (IsPaletteFadeFinished()) {
                BeginNormalPaletteFade(3, 1, 1, 0x7FFF, 6, 1, HEAP_ID_FIELD1);
                taskData->endStep++;
            }
            break;
        case 2:
            if (IsPaletteFadeFinished()) {
                taskData->endStep++;
            }
            break;
        case 3:
            taskData->scaleFrame++;
            float scale = 1.0 + (taskData->scaleFrame * 3.0) / 24.0;
            FlashDarkness_SetScale(taskData->darkness, scale);
            if (scale >= 4.0) {
                work->state = 5;
            }
            break;
        }
        break;
    case 5:
        WeatherEntry_EndTask(work->entry);
        break;
    }
}

static void WeatherTask_Flash2(SysTask *task, void *data) {
    WeatherWork *work = data;
    WeatherSystem *weatherSystem = work->weatherSystem;
    FieldSystem *fieldSystem = weatherSystem->fieldSystem;
    WeatherFlashTaskData *taskData = work->taskData;

    switch (work->state) {
    case 0:
        taskData->darkness = FlashDarkness_Show(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
        Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
        Fog_SetColor(fieldSystem->fog, -1, 0, 0);
        {
            s8 fogTable[32];
            for (int i = 0; i < 32; i++) {
                fogTable[i] = -1;
            }
            Fog_SetTable(fieldSystem->fog, fogTable);
        }
        FlashDarkness_SetScale(taskData->darkness, 4);
        work->state = 1;
        break;
    case 1:
        work->state = 3;
        break;
    case 2:
        taskData->darkness = FlashDarkness_Show(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
        Fog_Set(fieldSystem->fog, -1, TRUE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x0020, 0);
        Fog_SetColor(fieldSystem->fog, -1, 0, 0);
        {
            s8 fogTable[32];
            for (int i = 0; i < 32; i++) {
                fogTable[i] = -1;
            }
            Fog_SetTable(fieldSystem->fog, fogTable);
        }
        FlashDarkness_SetScale(taskData->darkness, 4);
        work->state = 3;
        break;
    case 3:
        if (work->stopRequest == 5) {
            work->state = 4;
        }
        break;
    case 4:
        work->state = 5;
        break;
    case 5:
        WeatherEntry_EndTask(work->entry);
        break;
    }
}

static void WeatherTask_LowLight(SysTask *task, void *data) {
    WeatherWork *work = data;
    WeatherTask_RunFogOnly(work, work->taskData, GX_FOGSLOPE_0x4000, 0x4B6F, 0x421, 0, 0);
}

static void WeatherTask_DelayedRainSE(SysTask *task, void *data) {
    WeatherWork *work = data;
    if (--work->seDelay <= 0) {
        Weather_PlayLoopSE(work, SEQ_SE_DP_T_AME);
        SysTask_Destroy(task);
        work->seDelayTask = 0;
    }
}

static void Weather_PlayLoopSE(WeatherWork *work, u32 sndSeq) {
    GF_ASSERT(work->loopSEPlaying == 0);

    work->loopSEPlaying = 1;
    work->sndSeq = sndSeq;
    PlaySE(sndSeq);
}

static void Weather_StopLoopSE(WeatherWork *work) {
    StopSE(work->sndSeq, 0);
    work->loopSEPlaying = 0;
}
