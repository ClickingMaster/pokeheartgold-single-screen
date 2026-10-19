#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"
#include "msgdata/msg.naix"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "pokepic.h"
#include "render_text.h"
#include "save_misc_data.h"
#include "sprite_resource_header.h"
#include "sprite_system.h"
#include "swap_3d_buffers.h"
#include "sys_task_api.h"
#include "system.h"
#include "touch_hitbox_controller.h"
#include "touchpad.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"

// VS Recorder app core: resources, BG / sprite setup, V-blank and the screen dispatcher.

void *WirelessStatusIcon_LoadPaletteFile(enum HeapID heapID);
void VsRecorderApp_CountMenuChoices(VsRecorderWork *work);

// per-screen step tables, indexed by VsRecorderWork.subState
extern VsRecorderScreenFunc sVsRecorderBoxDataSendSubStates[];
extern VsRecorderScreenFunc sVsRecorderBoxDataViewSubStates[];
extern VsRecorderScreenFunc sVsRecorderRankingsSubStates[];
extern VsRecorderScreenFunc sVsRecorderVideoListSubStates[];
extern VsRecorderScreenFunc sVsRecorderVideoSearchSubStates[];
extern VsRecorderScreenFunc sVsRecorderNumberSearchSubStates[];
extern VsRecorderScreenFunc sVsRecorderSendVideoSubStates[];
extern VsRecorderScreenFunc sVsRecorderDeleteSubStates[];

static void VsRecorder_InitBgLayers(BgConfig *bgConfig);
static void VsRecorder_InitSpriteSystem(VsRecorderWork *work);

static const GraphicsModes sVsRecorderGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const OamCharTransferParam sVsRecorderOamCharTransferParam = {
    .maxTasks = 128,
    .sizeMain = 0x10000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_128K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_128K,
};

static const SpriteResourceCountsListUnion sVsRecorderSpriteResourceCounts = { 128, 32, 128, 128, 16, 16 };

static const OamManagerParam sVsRecorderOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 4,
    .numOBJsub = 124,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const GraphicsBanks sVsRecorderGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_0_B,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

static const BgTemplate sVsRecorderBgTemplates[] = {
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0x2000,
     .charBase = GX_BG_CHARBASE_0x10000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 1,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0x2800,
     .charBase = GX_BG_CHARBASE_0x14000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 3,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0x3000,
     .charBase = GX_BG_CHARBASE_0x18000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 2,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0x3800,
     .charBase = GX_BG_CHARBASE_0x1c000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 1,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xc000,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 1,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xc800,
     .charBase = GX_BG_CHARBASE_0x04000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 3,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xd000,
     .charBase = GX_BG_CHARBASE_0x08000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 2,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xd800,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 1,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
};
void VsRecorder_InitResources(VsRecorderWork *work) {
    OrbitCursorArgs args;
    NNSG2dPaletteData *pltt;
    void *file;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    GF_CreateVramTransferManager(4, HEAP_ID_109);
    work->narc = NARC_New(NARC_a_1_9_1, HEAP_ID_109);
    work->bgConfig = BgConfig_Alloc(HEAP_ID_109);
    work->plttData = PaletteData_Init(HEAP_ID_109);
    work->vramMan = GF_3DVramMan_Create(HEAP_ID_109, 0, 1, 0, 4, NULL);
    work->pokepicManager = PokepicManager_Create(HEAP_ID_109);
    NNS_G2dSetupSoftwareSpriteCamera();
    PaletteData_SetAutoTransparent(work->plttData, TRUE);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_109);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_109);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_109);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_109);
    VsRecorder_InitBgLayers(work->bgConfig);
    VsRecorder_InitSpriteSystem(work);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    work->cursorHidden = 1;
    VsRecorder_InitMenuHitboxes(work);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    Main_SetVBlankIntrCB(VsRecorder_VBlankCB, work);
    if (work->mode != 0) {
        // Global Terminal: wireless icon and its palette in OBJ slot 7 of both screens
        VsRecorder_InitNet(work);
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DSUB, GX_OBJVRAMMODE_CHAR_1D_128K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DSUB);
        WirelessStatusIcon_Create();
        WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_109);
        file = WirelessStatusIcon_LoadPaletteFile(HEAP_ID_109);
        NNS_G2dGetUnpackedPaletteData(file, &pltt);
        PaletteData_LoadPalette(work->plttData, pltt->pRawData, PLTTBUF_MAIN_OBJ, 0xE0, 0x20);
        PaletteData_LoadPalette(work->plttData, pltt->pRawData, PLTTBUF_SUB_OBJ, 0xE0, 0x20);
        Heap_Free(file);
    }
    args.count = 12;
    args.heapId = HEAP_ID_109;
    args.resBase = 1;
    args.fixedPalette = work->mode;
    args.yOffset = FX32_CONST(256);
    work->orbitCursors[0] = OrbitCursor_New(args, work->spriteSystem, work->spriteManager, work->plttData);
    OrbitCursor_Start(work->orbitCursors[0], work->colorScheme);
    OrbitCursor_SetActive(work->orbitCursors[0], 0);
    OrbitCursor_SetVisible(work->orbitCursors[0], 0);
    args.resBase = 2;
    args.yOffset = FX32_CONST(256);
    work->orbitCursors[1] = OrbitCursor_New(args, work->spriteSystem, work->spriteManager, work->plttData);
    OrbitCursor_Start(work->orbitCursors[1], work->colorScheme);
    OrbitCursor_SetActive(work->orbitCursors[1], 0);
    OrbitCursor_SetVisible(work->orbitCursors[1], 0);
    work->fontSystem = FontSystem_NewInit(20, HEAP_ID_109);
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0013_bin, HEAP_ID_109);
    work->recordMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0026_bin, HEAP_ID_109);
    VsRecorder_LoadSavedVideos(work);
    VsRecorderApp_CountMenuChoices(work);
    VsRecorder_LoadFrameBg(work);
    VsRecorder_StartVBlankTask(work);
}

void VsRecorder_FreeResources(VsRecorderWork *work) {
    int i;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(work->bgConfig);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(work->plttData);
    if (work->mode == 0) {
        SaveMisc_SetVsRecorderColorScheme(Save_Misc_Get(work->saveData), work->colorScheme);
    }
    NARC_Delete(work->narc);
    SysTask_Destroy(work->task);
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
    WirelessStatusIcon_Destroy();
    GF_TouchpadStopSampling();
    TouchHitboxController_Destroy(work->hitboxController);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    {
        VsRecorderWork *cursor;
        i = 0;
        cursor = work;
        do {
            if (cursor->savedVideos[0]) {
                Heap_Free(cursor->savedVideos[0]);
            }
            if (cursor->videoOwners[0]) {
                Heap_Free(cursor->videoOwners[0]);
            }
            i++;
            cursor = (VsRecorderWork *)((u8 *)cursor + sizeof(void *));
        } while (i < 4);
    }
    FontSystem_Delete(work->fontSystem);
    DestroyMsgData(work->msgData);
    DestroyMsgData(work->recordMsgData);
    GF_3DVramMan_Delete(work->vramMan);
    PokepicManager_Delete(work->pokepicManager);
    VsRecorder_FreeNet(work);
    Main_SetVBlankIntrCB(NULL, NULL);
    GF_DestroyVramTransferManager();
}

static void VsRecorder_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes;
    GraphicsBanks banks;
    GfGfx_DisableEngineAPlanes();
    modes = sVsRecorderGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    banks = sVsRecorderGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (void *)HW_BG_VRAM, 0x80000);
    MIi_CpuClear32(0, (void *)HW_DB_BG_VRAM, 0x20000);
    MIi_CpuClear32(0, (void *)HW_OBJ_VRAM, 0x40000);
    MIi_CpuClear32(0, (void *)HW_DB_OBJ_VRAM, 0x20000);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &sVsRecorderBgTemplates[GF_BG_LYR_MAIN_0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &sVsRecorderBgTemplates[GF_BG_LYR_MAIN_1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &sVsRecorderBgTemplates[GF_BG_LYR_MAIN_2], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &sVsRecorderBgTemplates[GF_BG_LYR_MAIN_3], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sVsRecorderBgTemplates[GF_BG_LYR_SUB_0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sVsRecorderBgTemplates[GF_BG_LYR_SUB_1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sVsRecorderBgTemplates[GF_BG_LYR_SUB_2], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sVsRecorderBgTemplates[GF_BG_LYR_SUB_3], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    VsRecorder_SetScreensFlipped(TRUE);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 7, 8);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 7, 8);
}

void VsRecorder_SetScreensFlipped(int flipped) {
    gSystem.screensFlipped = flipped;
    GfGfx_SwapDisplay();
}

void VsRecorder_ResetBg2(VsRecorderWork *work) {
    InitBgFromTemplate(work->bgConfig, GF_BG_LYR_MAIN_2, &sVsRecorderBgTemplates[GF_BG_LYR_MAIN_2], GF_BG_TYPE_TEXT);
}

static void VsRecorder_InitSpriteSystem(VsRecorderWork *work) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion counts;
    work->spriteSystem = SpriteSystem_Alloc(HEAP_ID_109);
    oamManagerParam = sVsRecorderOamManagerParam;
    oamCharTransferParam = sVsRecorderOamCharTransferParam;
    SpriteSystem_Init(work->spriteSystem, &oamManagerParam, &oamCharTransferParam, 0x20);
    counts = sVsRecorderSpriteResourceCounts;
    work->spriteManager = SpriteManager_New(work->spriteSystem);
    if (!SpriteSystem_InitSprites(work->spriteSystem, work->spriteManager, 0xC0)) {
        GF_AssertFail();
    }
    if (!SpriteSystem_InitManagerWithCapacities(work->spriteSystem, work->spriteManager, &counts)) {
        GF_AssertFail();
    }
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, FX32_CONST(256));
}

void VsRecorder_VBlankCB(void *data) {
    vu32 *dtcm = (vu32 *)0x027E0000;
    VsRecorderWork *work = data;
    GF_RunVramTransferTasks();
    PaletteData_PushTransparentBuffers(work->plttData);
    DoScheduledBgGpuUpdates(work->bgConfig);
    dtcm[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

// runs one step of the current screen; TRUE once the screen is past the last one (exit)
BOOL VsRecorder_Main(VsRecorderWork *work, int *screen) {
    BOOL done;
    switch (*screen) {
    case VS_RECORDER_SCREEN_TITLE:
        done = gVsRecorderTitleSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, done, screen);
        if (done == FALSE) {
            break;
        }
        if (work->cursorHidden == 1) {
            VsRecorder_SetSubState(work, 0);
        } else {
            VsRecorder_SetSubState(work, 1);
        }
        break;
    case VS_RECORDER_SCREEN_MENU:
        done = gVsRecorderMenuSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_SAVED_VIDEO:
        done = gVsRecorderSavedVideoSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_FRONTIER:
        done = gVsRecorderFrontierSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_DRESS_UP_SEND:
        done = gVsRecorderDressUpSendSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_DRESS_UP_VIEW:
        done = gVsRecorderDressUpViewSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_BOX_DATA_SEND:
        done = sVsRecorderBoxDataSendSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_BOX_DATA_VIEW:
        done = sVsRecorderBoxDataViewSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_RANKINGS:
    case VS_RECORDER_SCREEN_RANKINGS_LAST:
        done = sVsRecorderRankingsSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_COLOSSEUM_TOP:
    case VS_RECORDER_SCREEN_FRONTIER_TOP:
        done = sVsRecorderVideoListSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_NEWEST:
        done = sVsRecorderVideoListSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_SEARCH:
        done = sVsRecorderVideoSearchSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_SEARCH_NUMBER:
        done = sVsRecorderNumberSearchSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_SEND_VIDEO:
        done = sVsRecorderSendVideoSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_EXIT:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_NONE, TRUE, screen);
        break;
    case VS_RECORDER_SCREEN_DELETE:
        done = sVsRecorderDeleteSubStates[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_MESSAGE:
        VsRecorder_MessageScreen(work);
        break;
    default:
        return TRUE;
    }
    Thunk_G3X_Reset();
    PokepicManager_DrawAll(work->pokepicManager);
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
    VsRecorder_UpdateNet(work);
    return FALSE;
}
