#include "frontier/frontier_map.h"

#include "global.h"

#include "frontier/frontier.h"

#include "bg_window.h"
#include "field_bgm.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "options.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "sound_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "touchpad.h"
#include "sprite_resource_header.h"
#include "swap_3d_buffers.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"

// Battle Frontier map (overlay 80): the facility map behind the frontier scripts - BGs from the map
// parameter table, the scrolling camera, the overlay 42 map objects and up to 8 extra sprites, plus
// saving/restoring that state in the frontier application's work while another app runs.

typedef struct FrontierMapMainBgTemplates {
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;
} FrontierMapMainBgTemplates;

// Frontier application work (frontier_runtime.c)
FrontierMapObjectGfx *Frontier_GetMapObjectGfx(void *frontier);
FrontierMapObject *Frontier_GetMapObjects(void *frontier);
FrontierMapObject *Frontier_GetMapObject(void *frontier, int index);
FrontierMapSpriteState *Frontier_GetMapSpriteState(void *frontier);
void Frontier_ResetMapSpriteState(void *frontier);

// Other frontier overlay modules (no header yet)
u32 FrontierMap_GetParam(u32 mapId, u32 field);
void FrontierMap_InitEffect(void *map, void *effect, u32 mapId);
void FrontierMap_ExitEffect(void *map, void *effect, u32 mapId);
int Frontier_GetPlayerSpriteId(void *profile);
u32 FrontierParticles_New(enum HeapID heapID);
void FrontierParticles_Free(u32 resourceManager);
void FrontierParticles_Update(void);

// Not in frontier/frontier_map.h: the script commands declare these with u32 indexes (their code depends on it).
ManagedSprite *FrontierMap_CreateSprite(FrontierMap *map, u16 index, u16 resourceId);
void FrontierMap_SetSpriteAnimActive(FrontierMap *map, u16 index, u32 active);

extern const Field2DBgLayerTemplate gFrontierMapLayerTemplate;
extern const GraphicsModes gFrontierMapGraphicsModes;
extern const OamCharTransferParam gFrontierMapOamCharTransferParam;
extern const SpriteResourceCountsListUnion gFrontierMapSpriteResourceCounts;
extern const BgTemplate gFrontierMapSubBgTemplate;
extern const OamManagerParam gFrontierMapOamManagerParam;
extern const GraphicsBanks gFrontierMapGraphicsBanks;
extern const FrontierMapMainBgTemplates gFrontierMapMainBgTemplates;
extern const u8 gFrontierMapLayout[];

// Map parameter table fields (FrontierMap_GetParam)
#define MAP_PARAM_BG_MODE     0
#define MAP_PARAM_BGM         3
#define MAP_PARAM_BG_SIZE     4
#define MAP_PARAM_NARC        5
#define MAP_PARAM_BG3_SCREEN  6
#define MAP_PARAM_BG3_CHAR    7
#define MAP_PARAM_BG3_PALETTE 8
#define MAP_PARAM_BG2_SCREEN  9
#define MAP_PARAM_BG2_CHAR    10
#define MAP_PARAM_BG2_PALETTE 11
#define MAP_PARAM_SCROLL_MODE 12
#define MAP_PARAM_BG2_SCROLLS 13

void *FrontierMap_Init(void *frontier) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(frontier);
    PlayerProfile *profile = Save_PlayerData_GetProfile(args->saveData);
    FrontierMap *map;
    u8 mapId = args->frontierMapId;
    int i;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_BlendNone();
    G2S_BlendNone();
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK; // GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB)
    Heap_Create(HEAP_ID_3, HEAP_ID_101, 0x90000);
    map = Heap_Alloc(HEAP_ID_101, sizeof(FrontierMap));
    MI_CpuFill8(map, 0, sizeof(FrontierMap));
    map->frontier = frontier;
    map->mapId = mapId;
    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        map->spriteSet.loadedResourceIds[i] = FRONTIER_MAP_NO_ID;
    }
    map->vramMan = FrontierMap_Create3DVramMan(HEAP_ID_101);
    map->paletteData = PaletteData_Init(HEAP_ID_101);
    PaletteData_SetAutoTransparent(map->paletteData, TRUE);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_MAIN_OBJ, 0x1C0, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_101);
    map->bgConfig = BgConfig_Alloc(HEAP_ID_101);
    GF_CreateVramTransferManager(64, HEAP_ID_101);
    SetKeyRepeatTimers(4, 8);
    FrontierMap_SetVramBank(map->bgConfig, mapId);
    FrontierMap_LoadPaletteData(map);
    FrontierMap_LoadSubScreenGfx(map);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    FrontierMap_InitSpriteSystem(map);
    map->particleManager = FrontierParticles_New(HEAP_ID_101);
    FrontierMap_InitMapObjects(map, mapId, profile);
    map->objectTask = SysTask_CreateOnMainQueue((SysTaskFunc)FrontierMap_ObjectTask, map, 60000);
    map->eventTask = SysTask_CreateOnMainQueue((SysTaskFunc)FrontierMap_EventTask, map, 61000);
    map->updateTask = SysTask_CreateOnMainQueue((SysTaskFunc)FrontierMap_Update, map, 80000);
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    Sound_SetFieldBGM(FrontierMap_GetParam(mapId, MAP_PARAM_BGM));
    FieldBGM_Play(NULL, FrontierMap_GetParam(mapId, MAP_PARAM_BGM));
    TextFlags_SetAutoScrollParam(1);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    Main_SetVBlankIntrCB((GFIntrCB)FrontierMap_VBlank, map);
    map->vblankTask = SysTask_CreateOnVBlankQueue((SysTaskFunc)FrontierMap_VBlankTask, map, 10);
    FrontierMap_InitEffect(map, &map->mapEffect, map->mapId);
    WirelessStatusIcon_Create();
    return map;
}

void FrontierMap_Free(FrontierMap *map) {
    Frontier_GetLaunchArgs(map->frontier);
    FrontierMap_ExitEffect(map, &map->mapEffect, map->mapId);
    FrontierMap_FreeMapObjects(map);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(map->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(map->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(map->bgConfig, GF_BG_LYR_MAIN_3);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(map->bgConfig, GF_BG_LYR_SUB_0);
    FrontierMap_FreeSpriteSystem(map);
    FrontierParticles_Free(map->particleManager);
    GF_DestroyVramTransferManager();
    PaletteData_FreeBuffers(map->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(map->paletteData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(map->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(map->paletteData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(map->paletteData);
    Heap_Free(map->bgConfig);
    SysTask_Destroy(map->objectTask);
    SysTask_Destroy(map->eventTask);
    SysTask_Destroy(map->updateTask);
    SysTask_Destroy(map->vblankTask);
    FrontierMap_Delete3DVramMan(map->vramMan);
    GF_TouchpadStopSampling();
    Heap_Free(map);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    Heap_Destroy(HEAP_ID_101);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    WirelessStatusIcon_Destroy();
    MIi_CpuClear16(0x7FFF, (void *)HW_BG_PLTT, HW_BG_PLTT_SIZE);
    MIi_CpuClear16(0x7FFF, (void *)HW_OBJ_PLTT, HW_OBJ_PLTT_SIZE);
    MIi_CpuClear16(0x7FFF, (void *)HW_DB_BG_PLTT, HW_DB_BG_PLTT_SIZE);
    MIi_CpuClear16(0x7FFF, (void *)HW_DB_OBJ_PLTT, HW_DB_OBJ_PLTT_SIZE);
    G2_BlendNone();
    G2S_BlendNone();
}

// Copies the live objects' positions and state into their saved templates (before the map is freed).
void FrontierMap_SaveObjects(FrontierMap *map) {
    s32 i;
    FrontierMapObject *object;

    for (i = 0; i < FRONTIER_MAP_OBJECT_MAX; i++) {
        object = Frontier_GetMapObject(map->frontier, i);
        if (object->object != NULL) {
            object->template.unk_0A = Field2DObject_GetParam(object->object, 6);
            object->template.unk_02 = Field2DObject_GetParam(object->object, 5);
            object->template.x = Field2DObject_GetParam(object->object, 0);
            object->template.y = Field2DObject_GetParam(object->object, 1);
            object->template.unk_0B = Field2DObjectSprite_IsVisible(object->objectSprite);
        }
    }
    FrontierMap_SaveSprites(map);
}

#pragma force_active on
// Recreates the object graphics, objects and sprites saved by FrontierMap_SaveObjects.
void FrontierMap_RestoreObjects(FrontierMap *map) {
    FrontierMapObjectTemplate template;
    FrontierMapObjectGfx *gfx = Frontier_GetMapObjectGfx(map->frontier);
    int i;

    for (i = 0; i < FRONTIER_MAP_OBJECT_GFX_MAX; i++) {
        if (gfx[i].id != FRONTIER_MAP_NO_ID) {
            Field2DObjectSpriteManager_LoadChar(map->objectGfx, gfx[i].id, gfx[i].unk_02, HEAP_ID_101);
        }
    }
    for (i = 0; i < FRONTIER_MAP_OBJECT_MAX; i++) {
        FrontierMapObject *object = Frontier_GetMapObject(map->frontier, i);
        if (object->template.id != FRONTIER_MAP_NO_ID) {
            FrontierMapObject_GetTemplate(object, (u16 *)&template);
            FrontierMap_CreateObject(map, &template, i);
        }
    }
    FrontierMap_RestoreSprites(map);
}
#pragma force_active reset

void FrontierMap_VBlank(FrontierMap *map) {
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(map->paletteData);
    DoScheduledBgGpuUpdates(map->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void FrontierMap_VBlankTask(void) {
}

void FrontierMap_ObjectTask(SysTask *task, FrontierMap *map) {
#pragma unused(task)
    Field2DObjectManager_Update(map->objectManager);
}

void FrontierMap_EventTask(SysTask *task, FrontierMap *map) {
#pragma unused(task)
    FrontierMap_ProcessEvents(map);
}

void FrontierMap_Update(SysTask *task, FrontierMap *map) {
#pragma unused(task)
    s32 i;
    u32 mask;
    FrontierMapObject *player = Frontier_GetMapObject(map->frontier, FRONTIER_MAP_OBJECT_MAX - 1);

    if (player->object != NULL) {
        Field2DCamera_CenterOnObject(&map->camera, player->object);
    }
    FrontierMap_Scroll(map);
    Field2DObjectSpriteManager_Update(map->objectGfx);
    mask = map->spriteSet.animActiveMask;
    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        if (map->spriteSet.sprites[i] != NULL && (mask & 1)) {
            ManagedSprite_TickFrame(map->spriteSet.sprites[i]);
        }
        mask >>= 1;
    }
    SpriteSystem_DrawSprites(map->spriteManager);
    SpriteSystem_UpdateTransfer();
    FrontierParticles_Update();
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

void FrontierMap_Scroll(FrontierMap *map) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(map->frontier);

    switch (FrontierMap_GetParam(args->frontierMapId, MAP_PARAM_SCROLL_MODE)) {
    default:
    case 0:
        if (map->bg3Layer != NULL) {
            Field2DBgLayer_SetCamera(map->bg3Layer, &map->camera);
        }
        if (map->bg2Layer != NULL) {
            if (FrontierMap_GetParam(args->frontierMapId, MAP_PARAM_BG2_SCROLLS) == 1) {
                Field2DBgLayer_SetCamera(map->bg2Layer, &map->camera);
            }
        }
        break;
    case 1:
        FrontierMap_ScrollToCamera(map);
        break;
    }
}

// Scroll mode 1: the BGs and the sprite surface follow the camera directly.
void FrontierMap_ScrollToCamera(FrontierMap *map) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(map->frontier);
    s16 y = map->shake.y + Field2DCamera_GetY(&map->camera);
    s16 x = map->shake.x + Field2DCamera_GetX(&map->camera);
    f32 fy;
    f32 fx;

    if (y > 0) {
        fy = 0.5f + (f32)(y << FX32_SHIFT);
    } else {
        fy = (f32)(y << FX32_SHIFT) - 0.5f;
    }
    if (x > 0) {
        fx = 0.5f + (f32)(x << FX32_SHIFT);
    } else {
        fx = (f32)(x << FX32_SHIFT) - 0.5f;
    }
    G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(map->spriteSystem), (fx32)fx, (fx32)fy);
    ScheduleSetBgPosText(map->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, x);
    ScheduleSetBgPosText(map->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, y);
    if (FrontierMap_GetParam(args->frontierMapId, MAP_PARAM_BG2_SCREEN) != FRONTIER_MAP_NO_ID) {
        if (FrontierMap_GetParam(args->frontierMapId, MAP_PARAM_BG2_SCROLLS) == 1) {
            ScheduleSetBgPosText(map->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, x);
            ScheduleSetBgPosText(map->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, y);
        }
    }
}

void FrontierMap_ProcessEvents(FrontierMap *map) {
    Field2DCommand event;
    Field2DMove command;

    while (Field2DMoveQueue_Pop(map->unk30, (Field2DMoveQueueEntry *)&command) == 1) {
        Field2DObjectManager_StartMove(map->objectManager, &command);
    }
    while (Field2DCommandQueue_Pop(map->unk2C, &event) == 1) {
        if (Field2DObjectManager_TryCommand(map->unk18, map->objectManager, &event, &command) == 1) {
            Field2DObjectManager_StartMove(map->objectManager, &command);
        }
    }
}

#pragma force_active on
void FrontierMap_SetVramBank(BgConfig *bgConfig, u32 mapId) {
    GraphicsBanks banks;
    GraphicsModes graphicsModes;
    BgTemplate subTemplate;
    FrontierMapMainBgTemplates main;
    int bgMode = FrontierMap_GetParam(mapId, MAP_PARAM_BG_MODE);
    u16 bgSize;

    GfGfx_DisableEngineAPlanes();
    banks = gFrontierMapGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    graphicsModes = gFrontierMapGraphicsModes;
    graphicsModes.bgMode = (GXBGMode)bgMode;
    SetBothScreensModesAndDisable(&graphicsModes);
    main = gFrontierMapMainBgTemplates;
    if (bgMode == GX_BGMODE_0) {
        main.bg2.colorMode = GX_BG_COLORMODE_16;
        main.bg3.colorMode = GX_BG_COLORMODE_16;
        main.bg2.bgExtPltt = GX_BG_EXTPLTT_01;
        main.bg3.bgExtPltt = GX_BG_EXTPLTT_01;
    }
    bgSize = FrontierMap_GetParam(mapId, MAP_PARAM_BG_SIZE);
    main.bg3.size = bgSize;
    if (FrontierMap_GetParam(mapId, MAP_PARAM_BG2_SCREEN) != FRONTIER_MAP_NO_ID) {
        main.bg2.size = bgSize;
    }
    if (bgMode == GX_BGMODE_0) {
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main.bg1, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main.bg2, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main.bg3, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    } else {
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main.bg1, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main.bg2, GF_BG_TYPE_256x16PLTT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main.bg3, GF_BG_TYPE_256x16PLTT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    }
    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    subTemplate = gFrontierMapSubBgTemplate;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subTemplate, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, 0);
}
#pragma force_active reset

void FrontierMap_LoadPaletteData(FrontierMap *map) {
    PaletteData_LoadNarc(map->paletteData, NARC_graphic_font, 7, HEAP_ID_101, PLTTBUF_MAIN_BG, 0x20, 0xE0);
    PaletteData_LoadNarc(map->paletteData, NARC_graphic_font, 8, HEAP_ID_101, PLTTBUF_MAIN_BG, 0x20, 0xD0);
    LoadUserFrameGfx2(map->bgConfig, GF_BG_LYR_MAIN_1, 0x3E2, 11, Options_GetFrame(Frontier_GetLaunchArgs(map->frontier)->options), HEAP_ID_101);
    PaletteData_LoadPaletteSlotFromHardware(map->paletteData, PLTTBUF_MAIN_BG, 0xB0, 0x20);
    LoadUserFrameGfx1(map->bgConfig, GF_BG_LYR_MAIN_1, 0x3D9, 12, 0, HEAP_ID_101);
    PaletteData_LoadPaletteSlotFromHardware(map->paletteData, PLTTBUF_MAIN_BG, 0xC0, 0x20);
}

void FrontierMap_LoadSubScreenGfx(FrontierMap *map) {
    NARC *narc = NARC_New(NARC_a_1_8_3, HEAP_ID_101);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 129, map->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_101);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 130, map->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_101);
    PaletteData_LoadNarc(map->paletteData, NARC_a_1_8_3, 190, HEAP_ID_101, PLTTBUF_SUB_BG, 0x20, 0);
    NARC_Delete(narc);
}

// Creates the overlay 42 object system for the map and loads the map BGs (BG3, optional BG2).
void FrontierMap_InitMapObjects(FrontierMap *map, u32 mapId, void *profile) {
    Field2DBgLayerTemplate layerTemplate;
    NNSG2dPaletteData *bg3Pltt;
    NNSG2dPaletteData *bg2Pltt;
    void *plttFile;
    NarcId narcId;
    NARC *narc;
    u32 scrollMode;
    u32 bgMode;

    map->objectManager = Field2DObjectManager_New(32, HEAP_ID_101);
    map->unk18 = Field2DMap_New(16, 16, HEAP_ID_101);
    Field2DCamera_Init(&map->camera);
    map->objectGfx = Field2DObjectSpriteManager_NewWithShadow(SpriteManager_GetSpriteList(map->spriteManager), map->paletteData, 32, Frontier_GetPlayerSpriteId(profile), 0, 1, HEAP_ID_101);
    layerTemplate = gFrontierMapLayerTemplate;
    layerTemplate.narcId = FrontierMap_GetParam(mapId, MAP_PARAM_NARC);
    layerTemplate.fileId = FrontierMap_GetParam(mapId, MAP_PARAM_BG3_SCREEN);
    scrollMode = FrontierMap_GetParam(mapId, MAP_PARAM_SCROLL_MODE);
    if (scrollMode == 0) {
        map->bg3Layer = Field2DBgLayer_New(SpriteSystem_GetRenderer(map->spriteSystem), map->bgConfig, &layerTemplate, HEAP_ID_101);
    }
    if (FrontierMap_GetParam(mapId, MAP_PARAM_BG2_SCREEN) != FRONTIER_MAP_NO_ID) {
        layerTemplate.fileId = FrontierMap_GetParam(mapId, MAP_PARAM_BG2_SCREEN);
        layerTemplate.unk_1 = GF_BG_LYR_MAIN_2;
        layerTemplate.screenBase = GX_BG_SCRBASE_0x0800;
        layerTemplate.charBase = GX_BG_CHARBASE_0x20000;
        layerTemplate.priority = 1;
        if (scrollMode == 0) {
            map->bg2Layer = Field2DBgLayer_New(SpriteSystem_GetRenderer(map->spriteSystem), map->bgConfig, &layerTemplate, HEAP_ID_101);
        }
    }
    map->unk2C = Field2DCommandQueue_New(128, HEAP_ID_101);
    map->unk30 = (Field2DMoveQueue *)Field2DMoveQueue_New(128, HEAP_ID_101);
    Field2DMap_Load(map->unk18, gFrontierMapLayout);
    bgMode = FrontierMap_GetParam(mapId, MAP_PARAM_BG_MODE);
    narcId = (NarcId)FrontierMap_GetParam(mapId, MAP_PARAM_NARC);
    narc = NARC_New(narcId, HEAP_ID_101);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, FrontierMap_GetParam(mapId, MAP_PARAM_BG3_CHAR), map->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_101);
    if (bgMode == GX_BGMODE_0) {
        PaletteData_LoadNarc(map->paletteData, narcId, FrontierMap_GetParam(mapId, MAP_PARAM_BG3_PALETTE), HEAP_ID_101, PLTTBUF_MAIN_BG, 0x160, 0);
    } else {
        plttFile = GfGfxLoader_GetPlttDataFromOpenNarc(narc, FrontierMap_GetParam(mapId, MAP_PARAM_BG3_PALETTE), &bg3Pltt, HEAP_ID_101);
        DC_FlushRange(bg3Pltt->pRawData, bg3Pltt->szByte);
        GX_BeginLoadBGExtPltt();
        GX_LoadBGExtPltt(bg3Pltt->pRawData, 0x6000, 0x2000);
        GX_EndLoadBGExtPltt();
        Heap_Free(plttFile);
    }
    PaletteData_FillPaletteInBuffer(map->paletteData, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0, 0, 1);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, FrontierMap_GetParam(mapId, MAP_PARAM_BG3_SCREEN), map->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_101);
    if (FrontierMap_GetParam(mapId, MAP_PARAM_BG2_SCREEN) != FRONTIER_MAP_NO_ID) {
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, FrontierMap_GetParam(mapId, MAP_PARAM_BG2_CHAR), map->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_101);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, FrontierMap_GetParam(mapId, MAP_PARAM_BG2_SCREEN), map->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_101);
        if (bgMode != GX_BGMODE_0) {
            void *bg2PlttFile = GfGfxLoader_GetPlttDataFromOpenNarc(narc, FrontierMap_GetParam(mapId, MAP_PARAM_BG2_PALETTE), &bg2Pltt, HEAP_ID_101);
            DC_FlushRange(bg2Pltt->pRawData, bg2Pltt->szByte);
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(bg2Pltt->pRawData, 0x4000, 0x2000);
            GX_EndLoadBGExtPltt();
            Heap_Free(bg2PlttFile);
        }
    }
    ScheduleBgTilemapBufferTransfer(map->bgConfig, GF_BG_LYR_MAIN_3);
    NARC_Delete(narc);
}

void FrontierMap_FreeMapObjects(FrontierMap *map) {
    s32 i;
    FrontierMapObject *object = Frontier_GetMapObjects(map->frontier);

    for (i = 0; i < FRONTIER_MAP_OBJECT_MAX; i++) {
        if (object->object != NULL) {
            Field2DObject_Delete(object->object);
            if (object->movementTask != NULL) {
                GF_AssertFail();
            }
        }
        object++;
    }
    Field2DObjectManager_Free(map->objectManager);
    Field2DMap_Free(map->unk18);
    Field2DObjectSpriteManager_Free(map->objectGfx);
    if (map->bg3Layer != NULL) {
        Field2DBgLayer_Free(map->bg3Layer);
    }
    if (map->bg2Layer != NULL) {
        Field2DBgLayer_Free(map->bg2Layer);
    }
    Field2DCommandQueue_Free(map->unk2C);
    Field2DMoveQueue_Free((u32 *)map->unk30);
}

GF3DVramMan *FrontierMap_Create3DVramMan(enum HeapID heapID) {
    return GF_3DVramMan_Create(heapID, 0, 1, 0, 1, FrontierMap_Init3D);
}

#pragma force_active on
void FrontierMap_Init3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7FFF, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}
#pragma force_active reset

void FrontierMap_Delete3DVramMan(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}

void FrontierMap_InitSpriteSystem(FrontierMap *map) {
    map->spriteSystem = SpriteSystem_Alloc(HEAP_ID_101);
    SpriteSystem_Init(map->spriteSystem, &gFrontierMapOamManagerParam, &gFrontierMapOamCharTransferParam, 32);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    map->spriteManager = SpriteManager_New(map->spriteSystem);
    SpriteSystem_InitSprites(map->spriteSystem, map->spriteManager, 128);
    SpriteSystem_InitManagerWithCapacities(map->spriteSystem, map->spriteManager, (SpriteResourceCountsListUnion *)&gFrontierMapSpriteResourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(map->spriteSystem), 0, FX32_CONST(512));
}

// The original was compiled against int-id prototypes of the four unload functions; calling them through
// this type keeps its code (the id constant is reloaded for each call).
typedef BOOL (*SpriteManagerUnloadFunc)(SpriteManager *spriteManager, int id);

#pragma force_active on
void FrontierMap_FreeSpriteSystem(FrontierMap *map) {
    int i;

    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        if (map->spriteSet.sprites[i] != NULL) {
            FrontierMapSprite_Delete(map->spriteSet.sprites[i]);
        }
    }
    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        if (map->spriteSet.loadedResourceIds[i] != FRONTIER_MAP_NO_ID) {
            FrontierMapSprite_UnloadResources(map->spriteManager, map->spriteSet.loadedResourceIds[i]);
        }
    }
    for (i = 0; i < 4; i++) {
        if (map->monSprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(map->monSprites[i]);
            ((SpriteManagerUnloadFunc)SpriteManager_UnloadCharObjById)(map->spriteManager, 50000 + i);
            ((SpriteManagerUnloadFunc)SpriteManager_UnloadPlttObjById)(map->spriteManager, 50000 + i);
            ((SpriteManagerUnloadFunc)SpriteManager_UnloadCellObjById)(map->spriteManager, 50000 + i);
            ((SpriteManagerUnloadFunc)SpriteManager_UnloadAnimObjById)(map->spriteManager, 50000 + i);
        }
    }
    SpriteSystem_FreeResourcesAndManager(map->spriteSystem, map->spriteManager);
    SpriteSystem_Free(map->spriteSystem);
}

void FrontierMap_AddObjectGfx(FrontierMap *map, const FrontierMapObjectGfx *gfx) {
    FrontierMapObjectGfx *table = Frontier_GetMapObjectGfx(map->frontier);
    int i;
    FrontierMapObjectGfx *entry;
    int slot;
    u16 id = gfx->id;

    i = 0;
    entry = table;
    do {
        if (id == entry->id) {
            return;
        }
        entry++;
        i++;
    } while (i < FRONTIER_MAP_OBJECT_GFX_MAX);
    slot = 0;
    entry = table;
    do {
        if (entry->id == FRONTIER_MAP_NO_ID) {
            break;
        }
        slot++;
        entry++;
    } while (slot < FRONTIER_MAP_OBJECT_GFX_MAX);
    if (slot == FRONTIER_MAP_OBJECT_GFX_MAX) {
        GF_AssertFail();
    }
    table[slot] = *gfx;
    Field2DObjectSpriteManager_LoadChar(map->objectGfx, gfx->id, gfx->unk_02, HEAP_ID_101);
}
#pragma force_active reset

void FrontierMap_RemoveObjectGfx(FrontierMap *map, u16 id) {
    s32 i;
    FrontierMapObjectGfx *table = Frontier_GetMapObjectGfx(map->frontier);

    for (i = 0; i < FRONTIER_MAP_OBJECT_GFX_MAX; i++) {
        if (id == table[i].id) {
            Field2DObjectSpriteManager_UnloadChar(map->objectGfx, id);
            table[i].id = FRONTIER_MAP_NO_ID;
            return;
        }
    }
}

#pragma force_active on
// index -1: first free object slot.
Field2DObject *FrontierMap_CreateObject(FrontierMap *map, const FrontierMapObjectTemplate *template, int index) {
    FrontierMapObject *slot = Frontier_GetMapObjects(map->frontier);
    Field2DObjectTemplate args;
    Field2DObject *object;
    Field2DObjectSprite *objectSprite;

    if (index == -1) {
        for (index = 0; index < FRONTIER_MAP_OBJECT_MAX; index++, slot++) {
            if (slot->object == NULL) {
                break;
            }
        }
        if (index == FRONTIER_MAP_OBJECT_MAX) {
            GF_AssertFail();
        }
    }
    args.unk0 = template->x;
    args.unk2 = template->y;
    args.unk4 = template->id;
    args.unk6 = template->unk_02;
    args.unk8 = template->unk_0A;
    args.unkA = template->spriteId;
    object = Field2DObjectManager_AddObject(map->objectManager, &args);
    objectSprite = Field2DObjectSpriteManager_CreateSprite(map->objectGfx, object, 0, HEAP_ID_101);
    Field2DObjectSprite_SetVisible(objectSprite, template->unk_0B);
    FrontierMap_SetObject(map->frontier, index, object, objectSprite, template);
    return object;
}
#pragma force_active reset

void FrontierMap_DeleteObject(FrontierMap *map, Field2DObject *object) {
    s32 i;
    FrontierMapObject *objects = Frontier_GetMapObjects(map->frontier);

    for (i = 0; i < FRONTIER_MAP_OBJECT_MAX; i++) {
        if (objects[i].object == object) {
            Field2DObject_Delete(objects[i].object);
            Field2DObjectSprite_Delete(objects[i].objectSprite);
            if (objects[i].movementTask != NULL) {
                GF_AssertFail();
            }
            MI_CpuFill8(&objects[i], 0, sizeof(FrontierMapObject));
            objects[i].template.id = FRONTIER_MAP_NO_ID;
            return;
        }
    }
}

void FrontierMap_GetObjectById(FrontierMap *map, u16 id, Field2DObject **object, Field2DObjectSprite **objectSprite) {
    s32 i;
    FrontierMapObject *objects = Frontier_GetMapObjects(map->frontier);

    for (i = 0; i < FRONTIER_MAP_OBJECT_MAX; i++) {
        if (id == objects[i].template.id) {
            if (object != NULL) {
                *object = objects[i].object;
            }
            if (objectSprite != NULL) {
                *objectSprite = objects[i].objectSprite;
            }
            return;
        }
    }
    GF_AssertFail();
}

void FrontierMap_AddLoadedResource(FrontierMap *map, u16 resourceId) {
    s32 i;

    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        if (map->spriteSet.loadedResourceIds[i] == FRONTIER_MAP_NO_ID) {
            map->spriteSet.loadedResourceIds[i] = resourceId;
            return;
        }
    }
    GF_AssertFail();
}

void FrontierMap_RemoveLoadedResource(FrontierMap *map, u16 resourceId) {
    s32 i;

    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        if (resourceId == map->spriteSet.loadedResourceIds[i]) {
            map->spriteSet.loadedResourceIds[i] = FRONTIER_MAP_NO_ID;
            return;
        }
    }
}

ManagedSprite *FrontierMap_CreateSprite(FrontierMap *map, u16 index, u16 resourceId) {
    ManagedSprite *sprite;

    if (index >= FRONTIER_MAP_SPRITE_MAX) {
        GF_AssertFail();
    }
    if (map->spriteSet.sprites[index] != NULL) {
        GF_AssertFail();
    }
    sprite = FrontierMapSprite_Create(map->spriteSystem, map->spriteManager, resourceId);
    map->spriteSet.sprites[index] = sprite;
    map->spriteSet.resourceIds[index] = resourceId;
    FrontierMap_SetSpriteAnimActive(map, index, FALSE);
    return sprite;
}

void FrontierMap_DeleteSprite(FrontierMap *map, u32 index) {
    if (index >= FRONTIER_MAP_SPRITE_MAX) {
        GF_AssertFail();
    }
    if (map->spriteSet.sprites[index] == NULL) {
        GF_AssertFail();
    }
    FrontierMapSprite_Delete(map->spriteSet.sprites[index]);
    map->spriteSet.sprites[index] = NULL;
}

ManagedSprite *FrontierMap_GetSprite(FrontierMap *map, u32 index) {
    return map->spriteSet.sprites[index];
}

void FrontierMap_SetSpriteAnimActive(FrontierMap *map, u16 index, u32 active) {
    if (active == TRUE) {
        map->spriteSet.animActiveMask = map->spriteSet.animActiveMask | (1 << index);
    } else {
        map->spriteSet.animActiveMask = ((1 << index) ^ 0xFFFFFFFF) & map->spriteSet.animActiveMask;
    }
}

u32 FrontierMap_IsSpriteAnimActive(FrontierMap *map, u16 index) {
    return (map->spriteSet.animActiveMask >> index) & 1;
}

#pragma force_active on
void FrontierMap_SaveSprites(FrontierMap *map) {
    FrontierMapSpriteState *state = Frontier_GetMapSpriteState(map->frontier);
    FrontierMapSpriteSet *spriteSet = &map->spriteSet;
    int i;

    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        u16 resourceId = spriteSet->loadedResourceIds[i];
        if (resourceId != FRONTIER_MAP_NO_ID) {
            state->resourceIds[i] = resourceId;
            i++; // skips the next slot (as the original does)
        }
    }
    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        if (spriteSet->sprites[i] != NULL) {
            state->sprites[i].anim = ManagedSprite_GetActiveAnim(spriteSet->sprites[i]);
            state->sprites[i].frame = ManagedSprite_GetAnimationFrame(spriteSet->sprites[i]);
            state->sprites[i].animActive = (u16)FrontierMap_IsSpriteAnimActive(map, i);
            state->sprites[i].draw = (u16)ManagedSprite_GetDrawFlag(spriteSet->sprites[i]);
            state->sprites[i].resourceId = spriteSet->resourceIds[i];
            ManagedSprite_GetPositionXY(spriteSet->sprites[i], &state->sprites[i].x, &state->sprites[i].y);
            state->sprites[i].active = TRUE;
        }
    }
}

void FrontierMap_RestoreSprites(FrontierMap *map) {
    FrontierMapSpriteState *state = Frontier_GetMapSpriteState(map->frontier);
    NARC *narc = NARC_New(NARC_a_1_8_4, HEAP_ID_101);
    int i;
    ManagedSprite *sprite;

    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        if (state->resourceIds[i] != FRONTIER_MAP_NO_ID) {
            FrontierMapSprite_LoadResources(map->spriteSystem, map->spriteManager, narc, map->paletteData, state->resourceIds[i]);
            FrontierMap_AddLoadedResource(map, state->resourceIds[i]);
        }
    }
    for (i = 0; i < FRONTIER_MAP_SPRITE_MAX; i++) {
        if (state->sprites[i].active == TRUE) {
            sprite = FrontierMap_CreateSprite(map, i, state->sprites[i].resourceId);
            ManagedSprite_SetPositionXY(sprite, state->sprites[i].x, state->sprites[i].y);
            ManagedSprite_SetDrawFlag(sprite, state->sprites[i].draw);
            FrontierMap_SetSpriteAnimActive(map, i, state->sprites[i].animActive);
            ManagedSprite_SetAnim(sprite, state->sprites[i].anim);
            ManagedSprite_SetAnimationFrame(sprite, state->sprites[i].frame);
        }
    }
    NARC_Delete(narc);
    Frontier_ResetMapSpriteState(map->frontier);
}
#pragma force_active reset

void FrontierMap_GetCameraPos(FrontierMap *map, u16 *x, u16 *y) {
    *y = Field2DCamera_GetY(&map->camera);
    *x = Field2DCamera_GetX(&map->camera);
}

void FrontierMapObject_GetTemplate(FrontierMapObject *object, u16 *dest) {
    u16 *src = (u16 *)&object->template;
    u32 i = sizeof(FrontierMapObjectTemplate) / 2;

    do {
        *dest++ = *src++;
    } while (--i != 0);
}

#pragma force_active on
void FrontierMap_SetObject(void *frontier, u32 index, Field2DObject *object, Field2DObjectSprite *objectSprite, const FrontierMapObjectTemplate *template) {
    FrontierMapObject *slot = Frontier_GetMapObject(frontier, index);

    slot->object = object;
    slot->objectSprite = objectSprite;
    slot->template = *template;
}
#pragma force_active reset

FrontierMapObject *FrontierMap_FindObject(void *frontier, u32 id) {
    s32 i;
    FrontierMapObject *object = Frontier_GetMapObjects(frontier);

    for (i = 0; i < FRONTIER_MAP_OBJECT_MAX; i++) {
        if (object->object != NULL && object->template.id == id) {
            return object;
        }
        object++;
    }
    GF_AssertFail();
    return NULL;
}
