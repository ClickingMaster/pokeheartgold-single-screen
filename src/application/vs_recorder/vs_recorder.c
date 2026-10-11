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
#include "palette.h"
#include "pokepic.h"
#include "render_text.h"
#include "save_misc_data.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "touch_hitbox_controller.h"
#include "touchpad.h"
#include "unk_02009D48.h"
#include "unk_02026E30.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"

// VS Recorder app core: resources, BG / sprite setup, V-blank and the screen dispatcher.

// arguments of the banner object (OrbitCursor_New), passed by value
typedef struct VsRecorderBannerArgs {
    int count;
    enum HeapID heapID;
    int resBase;
    int mode;
    fx32 yOffset;
} VsRecorderBannerArgs;

void *OrbitCursor_New(VsRecorderBannerArgs args, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PaletteData *plttData);
BOOL OrbitCursor_Start(void *banner, int colorScheme);
BOOL OrbitCursor_SetActive(void *banner, int a1);
BOOL OrbitCursor_SetVisible(void *banner, int a1);
void sub_0203A948(int a0, enum HeapID heapID);
void *sub_0203A4AC(enum HeapID heapID);
void sub_02088030(VsRecorderWork *work);

extern const GraphicsModes _02244C44;
extern const OamCharTransferParam ov40_02244C54;
extern const SpriteResourceCountsListUnion ov40_02244C68;
extern const OamManagerParam ov40_02244C80;
extern const GraphicsBanks ov40_02244CA0;
extern const BgTemplate ov40_02244CC8;
extern const BgTemplate ov40_02244CE4;
extern const BgTemplate ov40_02244D00;
extern const BgTemplate ov40_02244D1C;
extern const BgTemplate ov40_02244D38;
extern const BgTemplate ov40_02244D54;
extern const BgTemplate ov40_02244D70;
extern const BgTemplate ov40_02244D8C;

// per-screen step tables, indexed by VsRecorderWork.subState
extern VsRecorderScreenFunc ov40_022450F0[];
extern VsRecorderScreenFunc ov40_02245108[];
extern VsRecorderScreenFunc ov40_02245140[];
extern VsRecorderScreenFunc ov40_02245168[];
extern VsRecorderScreenFunc ov40_02245220[];
extern VsRecorderScreenFunc ov40_02245238[];
extern VsRecorderScreenFunc ov40_0224533C[];
extern VsRecorderScreenFunc ov40_02245368[];
extern VsRecorderScreenFunc ov40_02245470[];
extern VsRecorderScreenFunc ov40_022455F4[];
extern VsRecorderScreenFunc ov40_02245B98[];
extern VsRecorderScreenFunc ov40_02245B44[];
extern VsRecorderScreenFunc ov40_02245B30[];
extern VsRecorderScreenFunc ov40_02245CA8[];

#pragma force_active on

void VsRecorder_InitResources(VsRecorderWork *work) {
    VsRecorderBannerArgs args;
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
    work->unk_0044 = 1;
    VsRecorder_InitMenuHitboxes(work);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    Main_SetVBlankIntrCB(VsRecorder_VBlankCB, work);
    if (work->mode != 0) {
        // Global Terminal: wireless icon and its palette in OBJ slot 7 of both screens
        ov40_0223D544(work);
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DSUB, GX_OBJVRAMMODE_CHAR_1D_128K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DSUB);
        WirelessStatusIcon_Create();
        sub_0203A948(1, HEAP_ID_109);
        file = sub_0203A4AC(HEAP_ID_109);
        NNS_G2dGetUnpackedPaletteData(file, &pltt);
        PaletteData_LoadPalette(work->plttData, pltt->pRawData, PLTTBUF_MAIN_OBJ, 0xE0, 0x20);
        PaletteData_LoadPalette(work->plttData, pltt->pRawData, PLTTBUF_SUB_OBJ, 0xE0, 0x20);
        Heap_Free(file);
    }
    args.count = 12;
    args.heapID = HEAP_ID_109;
    args.resBase = 1;
    args.mode = work->mode;
    args.yOffset = FX32_CONST(256);
    work->banners[0] = OrbitCursor_New(args, work->spriteSystem, work->spriteManager, work->plttData);
    OrbitCursor_Start(work->banners[0], work->colorScheme);
    OrbitCursor_SetActive(work->banners[0], 0);
    OrbitCursor_SetVisible(work->banners[0], 0);
    args.resBase = 2;
    args.yOffset = FX32_CONST(256);
    work->banners[1] = OrbitCursor_New(args, work->spriteSystem, work->spriteManager, work->plttData);
    OrbitCursor_Start(work->banners[1], work->colorScheme);
    OrbitCursor_SetActive(work->banners[1], 0);
    OrbitCursor_SetVisible(work->banners[1], 0);
    work->fontSystem = FontSystem_NewInit(20, HEAP_ID_109);
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0013_bin, HEAP_ID_109);
    work->recordMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0026_bin, HEAP_ID_109);
    ov40_0222FCCC(work);
    sub_02088030(work);
    VsRecorder_LoadFrameBg(work);
    ov40_0222FBF8(work);
}

#pragma force_active reset

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
        sub_0202AC1C(Save_Misc_Get(work->saveData), work->colorScheme);
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
    ov40_0223D600(work);
    Main_SetVBlankIntrCB(NULL, NULL);
    GF_DestroyVramTransferManager();
}

#pragma force_active on

void VsRecorder_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes;
    GraphicsBanks banks;
    GfGfx_DisableEngineAPlanes();
    modes = _02244C44;
    SetBothScreensModesAndDisable(&modes);
    banks = ov40_02244CA0;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (void *)HW_BG_VRAM, 0x80000);
    MIi_CpuClear32(0, (void *)HW_DB_BG_VRAM, 0x20000);
    MIi_CpuClear32(0, (void *)HW_OBJ_VRAM, 0x40000);
    MIi_CpuClear32(0, (void *)HW_DB_OBJ_VRAM, 0x20000);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &ov40_02244CC8, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &ov40_02244CE4, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &ov40_02244D00, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &ov40_02244D1C, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &ov40_02244D38, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &ov40_02244D54, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &ov40_02244D70, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &ov40_02244D8C, GF_BG_TYPE_TEXT);
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

#pragma force_active reset

void VsRecorder_SetScreensFlipped(int flipped) {
    gSystem.screensFlipped = flipped;
    GfGfx_SwapDisplay();
}

void VsRecorder_ResetBg2(VsRecorderWork *work) {
    InitBgFromTemplate(work->bgConfig, GF_BG_LYR_MAIN_2, &ov40_02244D00, GF_BG_TYPE_TEXT);
}

void VsRecorder_InitSpriteSystem(VsRecorderWork *work) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion counts;
    work->spriteSystem = SpriteSystem_Alloc(HEAP_ID_109);
    oamManagerParam = ov40_02244C80;
    oamCharTransferParam = ov40_02244C54;
    SpriteSystem_Init(work->spriteSystem, &oamManagerParam, &oamCharTransferParam, 0x20);
    counts = ov40_02244C68;
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
    vu32 *dtcm = (vu32 *)0x027E0000; // DTCM as a constant, for OS_SetIrqCheckFlag below
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
        done = ov40_022450F0[work->subState](work);
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, done, screen);
        if (done == FALSE) {
            break;
        }
        if (work->unk_0044 == 1) {
            VsRecorder_SetSubState(work, 0);
        } else {
            VsRecorder_SetSubState(work, 1);
        }
        break;
    case VS_RECORDER_SCREEN_MENU:
        done = ov40_02245108[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_SAVED_VIDEO:
        done = ov40_02245140[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_FRONTIER:
        done = ov40_02245168[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_DRESS_UP_SEND:
        done = ov40_02245220[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_DRESS_UP_VIEW:
        done = ov40_02245238[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_BOX_DATA_SEND:
        done = ov40_0224533C[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_BOX_DATA_VIEW:
        done = ov40_02245368[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_RANKINGS:
    case VS_RECORDER_SCREEN_RANKINGS_LAST:
        done = ov40_02245470[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_COLOSSEUM_TOP:
    case VS_RECORDER_SCREEN_FRONTIER_TOP:
        done = ov40_022455F4[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_NEWEST:
        done = ov40_022455F4[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_SEARCH:
        done = ov40_02245B98[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_SEARCH_NUMBER:
        done = ov40_02245B44[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_SEND_VIDEO:
        done = ov40_02245B30[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_EXIT:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_NONE, TRUE, screen);
        break;
    case VS_RECORDER_SCREEN_DELETE:
        done = ov40_02245CA8[work->subState](work);
        VsRecorder_SetScreenIfDone(work, 0x10, done, screen);
        break;
    case VS_RECORDER_SCREEN_MESSAGE:
        ov40_02230D20(work);
        break;
    default:
        return TRUE;
    }
    Thunk_G3X_Reset();
    PokepicManager_DrawAll(work->pokepicManager);
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
    ov40_0223D5E8(work);
    return FALSE;
}
