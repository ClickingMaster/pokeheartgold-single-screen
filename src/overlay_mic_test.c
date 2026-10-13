#include "overlay_mic_test.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0234.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "main.h"
#include "math_util.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_system.h"
#include "system.h"
#include "text.h"
#include "title_screen.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "oam_manager.h"
#include "vram_transfer_manager.h"

typedef struct MicTestData MicTestData;
typedef struct MicTestTaskManager MicTestTaskManager;

typedef void (*MicTestTask)(MicTestTaskManager *, u32 *);

typedef struct MicTestInput {
    MICAutoParam mic;
    void *bufferAlloc;
    s32 curVolume;
    u32 volumeRose;
    u32 isSampling;
    u32 resumePending;
    u32 paused;
    u32 lastMicCounterFull;
} MicTestInput;

typedef struct MicTestDisplay {
    BgConfig *bgConfig;
    Window window[3];
    MsgData *msgData;
} MicTestDisplay;

struct MicTestTaskManager {
    MicTestData *micTest;
    MicTestTask task;
    u32 state;
    u32 isFinished;
};

typedef struct MicTestFloatingSprite {
    ManagedSprite *sprite;
    u32 delay;
    VecFx32 pos;
    VecFx32 velocity;
    fx32 wobbleAngle; // angle?
    fx32 wobbleSpeed; // dTheta?
    fx32 wobbleAmplitude; // magnitude?
    SysTask *task;
} MicTestFloatingSprite;

struct MicTestData {
    SpriteSystem *spriteRenderer;
    SpriteManager *gfxHandler;
    ManagedSprite *sprites[7];
    MicTestFloatingSprite floatingSprites[3];
    u32 spawnTimer;
    MicTestDisplay display;
    MicTestInput input;
    MicTestTaskManager taskMan;
    enum HeapID heapID;
};

#define TS_HITBOX_MIC_TEST_RETURN 0

static BOOL MicTest_Init(OverlayManager *overlayMan, int *state);
static BOOL MicTest_Exit(OverlayManager *overlayMan, int *state);
static BOOL MicTest_Main(OverlayManager *overlayMan, int *state);
static void MicTest_StartTask(MicTestTaskManager *a0, MicTestData *data, MicTestTask a2);
static void MicTestTaskMan_Run(MicTestTaskManager *a0);
static void MicTest_SetTask(MicTestTaskManager *a0, MicTestTask a1);
static MicTestData *MicTestTaskMan_GetMicTestData(MicTestTaskManager *a0);
static u32 MicTestTaskMan_IsFinished(MicTestTaskManager *a0);
static void MicTestTaskMan_Finish(MicTestTaskManager *a0);
static void MicTestTask_FadeIn(MicTestTaskManager *a0, u32 *state);
static void MicTestTask_Listen(MicTestTaskManager *a0, u32 *state);
static void MicTestTask_ReturnButtonAnim(MicTestTaskManager *a0, u32 *state);
static void MicTestTask_FadeOut(MicTestTaskManager *taskMan, u32 *state);
static void MicTestTask_End(MicTestTaskManager *taskMan, u32 *state);
static void MicTest_InitGraphics(enum HeapID heapID);
static void MicTest_FreeGraphics();
static void MicTest_SetBanks();
static void MicTest_VBlankIntrCB(void *data);
static void MicTest_InitSpriteRenderer(MicTestData *micTest, enum HeapID heapID);
static void MicTest_DeleteSpriteRenderer(MicTestData *micTest);
static void MicTest_UpdateAnimations(MicTestData *micTest);
static void MicTest_LoadResources(MicTestData *micTest);
static void MicTest_FreeSprites(MicTestData *micTest);
static void MicTest_TransferOam(MicTestData *micTest);
static void MicTest_InitBgs(MicTestDisplay *a0, enum HeapID heapID);
static void MicTest_FreeBgs(MicTestDisplay *a0);
static void ov62_021E6048(MicTestDisplay *a0);
static void MicTest_LoadTextResources(MicTestDisplay *a0, enum HeapID heapID);
static void MicTest_UpdateBgGpu(MicTestDisplay *a0);
static void MicTest_InitWindows(MicTestDisplay *a0, enum HeapID heapID);
static void MicTest_FreeWindows(MicTestDisplay *a0);
static void MicTestInput_Init(MicTestInput *input, enum HeapID heapID, MICCallback a2, MicTestData *micTest);
static void MicTestInput_Free(MicTestInput *input);
static void MicTestInput_Update(MicTestInput *input);
static void MicTestInput_StartSampling(MicTestInput *input);
static void MicTestInput_StopSampling(MicTestInput *input);
static s32 MicTest_AverageMicInput(MicTestInput *input);
static void MicTest_MicrophoneCallback(MICResult result, void *data);
static u32 MicTest_GetVolumeBracket(u8);
static u32 MicTest_AreFloatingSpritesIdle(MicTestData *micTest);
static BOOL MicTest_SpawnFloatingSprite(MicTestData *micTest, enum HeapID heapID, s16 x, s16 y);
static void MicTestFloatingSprite_Start(ManagedSprite *a0, MicTestFloatingSprite *args, s16 x, s16 y, s32 z, s32 r1, s32 r2, BOOL a7);
static void Task_MicTestFloatingSprite(SysTask *, void *);
static void MicTest_EndTasks(MicTestData *micTest);
static int MicTest_CheckReturn(MicTestData *data);

static const GraphicsBanks sMicTestGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_128_B,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE
};

typedef struct MicTestBgTemplate {
    BgTemplate bgTemplate;
    u8 bgId;
    u8 bgType;
    u8 unk1E;
    u8 unk1F;
} MicTestBgTemplate;

static const MicTestBgTemplate sMicTestBgTemplates[] = {
    {
     .bgTemplate = {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = 1,
            .colorMode = 0,
            .screenBase = 0x1F,
            .charBase = 0,
            .bgExtPltt = 0,
            .priority = 0,
            .areaOver = 0,
            .dummy = 0,
            .mosaic = 0,
        },
     .bgId = 0,
     .bgType = 0,
     .unk1E = 0,
     .unk1F = 0,
     },
    {
     .bgTemplate = {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = 1,
            .colorMode = 0,
            .screenBase = 0x1E,
            .charBase = 1,
            .bgExtPltt = 0,
            .priority = 1,
            .areaOver = 0,
            .dummy = 0,
            .mosaic = 0,
        },
     .bgId = 1,
     .bgType = 0,
     .unk1E = 0,
     .unk1F = 0,
     },
    {
     .bgTemplate = {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = 1,
            .colorMode = 0,
            .screenBase = 0x1D,
            .charBase = 2,
            .bgExtPltt = 0,
            .priority = 2,
            .areaOver = 0,
            .dummy = 0,
            .mosaic = 0,
        },
     .bgId = 2,
     .bgType = 0,
     .unk1E = 0,
     .unk1F = 0,
     },
    {
     .bgTemplate = {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = 1,
            .colorMode = 0,
            .screenBase = 0x1F,
            .charBase = 0,
            .bgExtPltt = 0,
            .priority = 0,
            .areaOver = 0,
            .dummy = 0,
            .mosaic = 0,
        },
     .bgId = 4,
     .bgType = 0,
     .unk1E = 0,
     .unk1F = 0,
     },
    {
     .bgTemplate = {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = 1,
            .colorMode = 0,
            .screenBase = 0x1E,
            .charBase = 1,
            .bgExtPltt = 0,
            .priority = 1,
            .areaOver = 0,
            .dummy = 0,
            .mosaic = 0,
        },
     .bgId = 5,
     .bgType = 0,
     .unk1E = 0,
     .unk1F = 0,
     },
};

static const ManagedSpriteTemplate sMicTestSpriteTemplates[5] = {
    {
     .x = 0x40,
     .y = 0x60,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0xA01, 0xA03, 0xA02, 0xA04, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 0xC0,
     .y = 0x60,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0xA01, 0xA03, 0xA02, 0xA04, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 0x80,
     .y = 0x60,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0xB01, 0xB03, 0xB02, 0xB04, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 0xD8,
     .y = 0xB0,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .resIdList = { 0xD01, 0xD03, 0xD02, 0xD04, 1, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 0,
     .y = 0,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0xC01, 0xC03, 0xC02, 0xC04, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
};
static const u32 sMicTestSpriteTemplateIds[7] = { 0, 1, 2, 3, 4, 4, 4 };

static const TouchscreenHitbox sMicTestTouchscreenHitboxes[] = {
    { .rect = { 160, 192, 184, 248 } }, // Return
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const OamManagerParam sMicTestOamManParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32
};

static const OamCharTransferParam sMicTestOamCharTransferParam = {
    7,
    0x20000,
    0x4000,
    GX_OBJVRAMMODE_CHAR_1D_32K,
    GX_OBJVRAMMODE_CHAR_1D_32K
};

static const SpriteResourceCountsListUnion sMicTestResCountList = {
    .numChar = 32,
    .numPltt = 32,
    .numCell = 32,
    .numAnim = 32,
    .numMcel = 0,
    .numManm = 0
};

static const GraphicsModes sMicTestGraphicsMode = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D
};

typedef struct MicTestTextBox {
    u8 bgId;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palNum;
    u16 baseTile;
    u8 fontId;
    u32 color;
} MicTestTextBox;

static const MicTestTextBox sMicTestTextBoxes[3] = {
    { .bgId = 0,
     .x = 1,
     .y = 1,
     .width = 9,
     .height = 3,
     .palNum = 0,
     .baseTile = 1,
     .fontId = 0,
     .color = MAKE_TEXT_COLOR(15, 2,  0) },
    { .bgId = 4,
     .x = 5,
     .y = 8,
     .width = 22,
     .height = 5,
     .palNum = 0,
     .baseTile = 1,
     .fontId = 0,
     .color = MAKE_TEXT_COLOR(15, 2,  0) },
    { .bgId = 4,
     .x = 24,
     .y = 21,
     .width = 6,
     .height = 2,
     .palNum = 0,
     .baseTile = 0x6F,
     .fontId = 4,
     .color = MAKE_TEXT_COLOR(14, 15, 0) },
};

const OverlayManagerTemplate gApplication_MicTest = { MicTest_Init, MicTest_Main, MicTest_Exit, FS_OVERLAY_ID_NONE };

static BOOL MicTest_Init(OverlayManager *overlayMan, int *state) {
    Heap_Create(HEAP_ID_3, HEAP_ID_MIC_TEST, 0x30000);

    MicTestData *micTest = OverlayManager_CreateAndGetData(overlayMan, sizeof(MicTestData), HEAP_ID_MIC_TEST);

    MI_CpuFill8(micTest, 0, sizeof(MicTestData));

    micTest->heapID = HEAP_ID_MIC_TEST;

    MicTest_InitGraphics(micTest->heapID);
    MicTest_SetBanks();
    Main_SetVBlankIntrCB(MicTest_VBlankIntrCB, micTest);
    FontID_Alloc(4, HEAP_ID_MIC_TEST);
    MicTest_InitBgs(&micTest->display, micTest->heapID);
    MicTest_LoadTextResources(&micTest->display, micTest->heapID);
    MicTest_InitWindows(&micTest->display, micTest->heapID);
    MicTest_InitSpriteRenderer(micTest, micTest->heapID);
    MicTest_LoadResources(micTest);
    MicTestInput_Init(&micTest->input, micTest->heapID, MicTest_MicrophoneCallback, micTest);

    MicTest_StartTask(&micTest->taskMan, micTest, MicTestTask_FadeIn);

    GF_SndStartFadeOutBGM(0, 10);

    return TRUE;
}

static BOOL MicTest_Exit(OverlayManager *overlayMan, int *state) {
    MicTestData *micTest = OverlayManager_GetData(overlayMan);
    MicTestInput_Free(&micTest->input);
    MicTest_FreeSprites(micTest);
    MicTest_DeleteSpriteRenderer(micTest);
    MicTest_FreeWindows(&micTest->display);
    MicTest_FreeBgs(&micTest->display);
    MicTest_FreeGraphics();
    FontID_Release(4);
    OverlayManager_FreeData(overlayMan);
    Heap_Destroy(micTest->heapID);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    return TRUE;
}

static BOOL MicTest_Main(OverlayManager *overlayMan, int *state) {
    MicTestData *micTest = OverlayManager_GetData(overlayMan);
    if (MicTestTaskMan_IsFinished(&micTest->taskMan)) {
        return TRUE;
    }
    MicTestTaskMan_Run(&micTest->taskMan);
    MicTest_UpdateAnimations(micTest);
    ov62_021E6048(&micTest->display);
    MicTestInput_Update(&micTest->input);
    return FALSE;
}

static void MicTest_StartTask(MicTestTaskManager *taskMan, MicTestData *micTest, MicTestTask task) {
    taskMan->micTest = micTest;
    taskMan->isFinished = FALSE;
    MicTest_SetTask(taskMan, task);
}

static void MicTestTaskMan_Run(MicTestTaskManager *taskMan) {
    if (MicTestTaskMan_IsFinished(taskMan) == 0) {
        taskMan->task(taskMan, &taskMan->state);
    }
}

static void MicTest_SetTask(MicTestTaskManager *taskMan, MicTestTask task) {
    taskMan->task = task;
    taskMan->state = 0;
}

static MicTestData *MicTestTaskMan_GetMicTestData(MicTestTaskManager *taskMan) {
    return taskMan->micTest;
}

static u32 MicTestTaskMan_IsFinished(MicTestTaskManager *taskMan) {
    return taskMan->isFinished;
}

static void MicTestTaskMan_Finish(MicTestTaskManager *taskMan) {
    taskMan->isFinished = TRUE;
}

enum {
    MICTEST_FADE_IN_STATE_START,
    MICTEST_FADE_IN_STATE_WAIT,
    MICTEST_FADE_IN_STATE_END
};

static void MicTestTask_FadeIn(MicTestTaskManager *taskMan, u32 *state) {
    MicTestData *micTest = MicTestTaskMan_GetMicTestData(taskMan);

    switch (*state) {
    case MICTEST_FADE_IN_STATE_START:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, micTest->heapID);
        *state = MICTEST_FADE_IN_STATE_WAIT;
        break;
    case MICTEST_FADE_IN_STATE_WAIT:
        if (IsPaletteFadeFinished()) {
            *state = MICTEST_FADE_IN_STATE_END;
        }
        break;
    case MICTEST_FADE_IN_STATE_END:
        MicTest_SetTask(taskMan, MicTestTask_Listen);
        break;
    default:
        GF_ASSERT(FALSE);
    }
}

static void MicTestTask_Listen(MicTestTaskManager *taskMan, u32 *state) {
    MicTestData *micTest = MicTestTaskMan_GetMicTestData(taskMan);

    switch (*state) {
    case 0:
        MicTestInput_StartSampling(&micTest->input);
        (*state) = 1;
        break;
    case 1:
        if (MicTest_CheckReturn(micTest)) {
            PlaySE(SEQ_SE_DP_SELECT);
            (*state) = 2;
        }
        break;
    case 2:
        MicTestInput_StopSampling(&micTest->input);
        (*state) = 3;
        break;
    case 3:
        MicTest_SetTask(taskMan, MicTestTask_ReturnButtonAnim);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

static void MicTestTask_ReturnButtonAnim(MicTestTaskManager *taskMan, u32 *state) {
    MicTestData *micTest = MicTestTaskMan_GetMicTestData(taskMan);

    switch (*state) {
    case 0:
        ManagedSprite_SetAnim(micTest->sprites[3], 1);
        (*state) = 1;
        break;
    case 1:
        if (!ManagedSprite_IsAnimated(micTest->sprites[3])) {
            (*state) = 2;
        }
        break;
    case 2:
        MicTest_SetTask(taskMan, MicTestTask_FadeOut);
        break;
    default:
        GF_ASSERT(FALSE);
    }
}

enum {
    MICTEST_FADE_OUT_STATE_START,
    MICTEST_FADE_OUT_STATE_WAIT,
    MICTEST_FADE_OUT_STATE_END,
};

static void MicTestTask_FadeOut(MicTestTaskManager *taskMan, u32 *state) {
    MicTestData *micTest = MicTestTaskMan_GetMicTestData(taskMan);

    switch (*state) {
    case MICTEST_FADE_OUT_STATE_START:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, micTest->heapID);
        (*state) = MICTEST_FADE_OUT_STATE_WAIT;
        break;
    case MICTEST_FADE_OUT_STATE_WAIT:
        if (IsPaletteFadeFinished()) {
            (*state) = MICTEST_FADE_OUT_STATE_END;
        }
        break;
    case MICTEST_FADE_OUT_STATE_END:
        MicTest_SetTask(taskMan, MicTestTask_End);
        break;
    default:
        GF_ASSERT(FALSE);
    }
}

static void MicTestTask_End(MicTestTaskManager *taskMan, u32 *state) {
    MicTestData *micTest = MicTestTaskMan_GetMicTestData(taskMan);
    MicTest_EndTasks(micTest);
    MicTestTaskMan_Finish(taskMan);
}

static void MicTest_InitGraphics(enum HeapID heapID) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    GF_CreateVramTransferManager(0x20, heapID);
}

static void MicTest_FreeGraphics() {
    GF_DestroyVramTransferManager();
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

static void MicTest_SetBanks() {
    GfGfx_SetBanks(&sMicTestGraphicsBanks);
}

static void MicTest_VBlankIntrCB(void *data) {
    MicTestData *micTest = data;
    NNS_GfdDoVramTransfer();
    MicTest_TransferOam(micTest);
    MicTest_UpdateBgGpu(&micTest->display);
    OS_SetIrqCheckFlag(OS_IE_VBLANK);
}

static void MicTest_InitSpriteRenderer(MicTestData *micTest, enum HeapID heapID) {
    SpriteSystem *spriteRender = SpriteSystem_Alloc(heapID);

    micTest->spriteRenderer = spriteRender;
    SpriteSystem_Init(spriteRender, &sMicTestOamManParam, &sMicTestOamCharTransferParam, 0x20);
    thunk_ClearMainOAM(heapID);
    thunk_ClearSubOAM(heapID);
    micTest->gfxHandler = SpriteManager_New(micTest->spriteRenderer);
    SpriteSystem_InitSprites(micTest->spriteRenderer, micTest->gfxHandler, 7);
    SpriteSystem_InitManagerWithCapacities(micTest->spriteRenderer, micTest->gfxHandler, (SpriteResourceCountsListUnion *)&sMicTestResCountList);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void MicTest_DeleteSpriteRenderer(MicTestData *micTest) {
    SpriteSystem_Free(micTest->spriteRenderer);
    micTest->spriteRenderer = NULL;
}

static void MicTest_UpdateAnimations(MicTestData *micTest) {
    for (u16 i = 0; i < 7; i++) {
        ManagedSprite_TickFrame(micTest->sprites[i]);
    }
    SpriteSystem_DrawSprites(micTest->gfxHandler);
}

static void MicTest_LoadResources(MicTestData *micTest) {
    SpriteSystem_LoadPlttResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 6, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0xA03);
    SpriteSystem_LoadCellResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 8, 0, 0xA02);
    SpriteSystem_LoadAnimResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 9, 0, 0xA04);
    SpriteSystem_LoadCharResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 7, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0xa01);

    SpriteSystem_LoadPlttResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 10, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB03);
    SpriteSystem_LoadCellResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 12, 0, 0xB02);
    SpriteSystem_LoadAnimResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 13, 0, 0xB04);
    SpriteSystem_LoadCharResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 11, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB01);

    SpriteSystem_LoadPlttResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 18, 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xD03);
    SpriteSystem_LoadCellResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 20, 0, 0xD02);
    SpriteSystem_LoadAnimResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 21, 0, 0xD04);
    SpriteSystem_LoadCharResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 19, 0, NNS_G2D_VRAM_TYPE_2DSUB, 0xD01);

    SpriteSystem_LoadPlttResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 14, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0xC03);
    SpriteSystem_LoadCellResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 16, 0, 0xC02);
    SpriteSystem_LoadAnimResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 17, 0, 0xC04);
    SpriteSystem_LoadCharResObj(micTest->spriteRenderer, micTest->gfxHandler, NARC_a_1_7_6, 15, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0xC01);

    for (u16 i = 0; i < 7; i++) {
        micTest->sprites[i] = SpriteSystem_NewSprite(micTest->spriteRenderer, micTest->gfxHandler, &sMicTestSpriteTemplates[sMicTestSpriteTemplateIds[i]]);
        ManagedSprite_SetAnimateFlag(micTest->sprites[i], TRUE);
    }

    for (u16 i = 0; i < 3; i++) {
        ManagedSprite_SetDrawFlag(micTest->sprites[4 + i], 0);
    }

    ManagedSprite_SetFlipMode(micTest->sprites[0], 1);
    ManagedSprite_SetPriority(micTest->sprites[3], 1);
    micTest->spawnTimer = 10;
}

static void MicTest_FreeSprites(MicTestData *micTest) {
    for (int i = 0; i < 7; i++) {
        if (micTest->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(micTest->sprites[i]);
        }
    }
    SpriteSystem_FreeResourcesAndManager(micTest->spriteRenderer, micTest->gfxHandler);
}

static void MicTest_TransferOam(MicTestData *micTest) {
    if (micTest->spriteRenderer != NULL) {
        SpriteSystem_TransferOam();
    }
}

static void MicTest_InitBgs(MicTestDisplay *a0, enum HeapID heapID) {
    a0->bgConfig = BgConfig_Alloc(heapID);
    SetBothScreensModesAndDisable(&sMicTestGraphicsMode);

    for (u32 i = 0; i < 5; i++) {
        InitBgFromTemplate(a0->bgConfig, sMicTestBgTemplates[i].bgId, &sMicTestBgTemplates[i].bgTemplate, sMicTestBgTemplates[i].bgType);
        BgClearTilemapBufferAndCommit(a0->bgConfig, sMicTestBgTemplates[i].bgId);
        BG_ClearCharDataRange(sMicTestBgTemplates[i].bgId, 32, 0, heapID);
    }
}

static void MicTest_FreeBgs(MicTestDisplay *a0) {
    for (u32 i = 0; i < 5; i++) {
        FreeBgTilemapBuffer(a0->bgConfig, sMicTestBgTemplates[i].bgId);
    }
    Heap_Free(a0->bgConfig);
}

static void ov62_021E6048(MicTestDisplay *a0) {
}

static void MicTest_LoadTextResources(MicTestDisplay *a0, enum HeapID heapID) {
    GfGfxLoader_GXLoadPal(NARC_a_1_7_6, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_7_6, 4, a0->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_6, 5, a0->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 0, heapID);

    GfGfxLoader_GXLoadPal(NARC_a_1_7_6, 1, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 32, heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_7_6, 2, a0->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_6, 3, a0->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, heapID);
}

static void MicTest_UpdateBgGpu(MicTestDisplay *a0) {
    if (a0->bgConfig != NULL) {
        DoScheduledBgGpuUpdates(a0->bgConfig);
    }
}

static void MicTest_InitWindows(MicTestDisplay *a0, enum HeapID heapID) {
    a0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0234_bin, heapID);

    for (int i = 0; i < 3; i++) {
        AddWindowParameterized(a0->bgConfig, &a0->window[i], sMicTestTextBoxes[i].bgId, sMicTestTextBoxes[i].x, sMicTestTextBoxes[i].y, sMicTestTextBoxes[i].width, sMicTestTextBoxes[i].height, sMicTestTextBoxes[i].palNum, sMicTestTextBoxes[i].baseTile);
        FillWindowPixelBuffer(&a0->window[i], 0);
        ClearWindowTilemap(&a0->window[i]);
        String *str = NewString_ReadMsgData(a0->msgData, i);
        AddTextPrinterParameterizedWithColor(&a0->window[i], sMicTestTextBoxes[i].fontId, str, 0, 0, 0xFF, sMicTestTextBoxes[i].color, 0);
        String_Delete(str);
        CopyWindowToVram(&a0->window[i]);
    }
}

static void MicTest_FreeWindows(MicTestDisplay *a0) {
    for (int i = 0; i < 3; i++) {
        ClearWindowTilemapAndCopyToVram(&a0->window[i]);
        FillWindowPixelBuffer(&a0->window[i], 0);
        ClearWindowTilemap(&a0->window[i]);
        RemoveWindow(&a0->window[i]);
    }
    DestroyMsgData(a0->msgData);
}

static void MicTestInput_Init(MicTestInput *input, enum HeapID heapID, MICCallback a2, MicTestData *micTest) {
    Sys_SetSleepDisableFlag(8);

    void *data = Heap_Alloc(heapID, 0x120);
    MI_CpuFill8(data, 0, 0x120);
    input->bufferAlloc = data;

    input->mic.type = MIC_SAMPLING_TYPE_8BIT;
    input->mic.buffer = (void *)((((u32)data) + 0x1F) & ~0x1F); // alignment to 32-bit boundary
    input->mic.size = 256;
    input->mic.rate = 0x1040;
    input->mic.loop_enable = TRUE;
    input->mic.full_callback = a2;
    input->mic.full_arg = micTest;

    input->lastMicCounterFull = Sound_IsMicCounterFull();
}

static void MicTestInput_Free(MicTestInput *input) {
    Heap_Free(input->bufferAlloc);
    Sys_ClearSleepDisableFlag(8);
}

static void MicTestInput_Update(MicTestInput *input) {
    u32 next = Sound_IsMicCounterFull();

    if (next == 0 || input->lastMicCounterFull == 0) {
        if (input->paused == 0) {
            if (input->isSampling) {
                MicTestInput_StopSampling(input);
                input->resumePending = 1;
            }
            input->paused = 1;
        }

        if (next != 0 && input->lastMicCounterFull == 0) {
            input->paused = 0;

            if (input->resumePending) {
                MicTestInput_StartSampling(input);
                input->resumePending = 0;
            }
        }
    }

    input->lastMicCounterFull = next;
}

static void MicTestInput_StartSampling(MicTestInput *input) {
    if (input->paused == 0) {
        GF_MIC_StartAutoSampling(&input->mic);
        input->isSampling = 1;
    } else {
        input->resumePending = 1;
    }
}

static void MicTestInput_StopSampling(MicTestInput *input) {
    GF_MIC_StopAutoSampling();
    input->isSampling = 0;
}

static s32 MicTest_AverageMicInput(MicTestInput *input) {
    int i;
    u8 *buffer;

    int cnt = 0;
    int size = input->mic.size - 1;
    buffer = input->mic.buffer;

    for (i = 0; i < size; i++) {
        if (buffer[i] < 0x80) {
            cnt += 0x100 - buffer[i];
        } else {
            cnt += buffer[i];
        }
    }
    return cnt / size;
}

static struct {
    u32 unk0;
} sMicTestCallbackCount;

static void MicTest_MicrophoneCallback(MICResult result, void *data) {
    MicTestData *micTest = data;

    if (result != MIC_RESULT_SUCCESS) {
        return;
    }

    MicTestInput *unkF0 = &micTest->input;

    s32 volumeAvg = MicTest_AverageMicInput(unkF0);
    u32 volume;

    if (unkF0->curVolume < volumeAvg) {
        volume = MicTest_GetVolumeBracket(volumeAvg);
        unkF0->curVolume = volumeAvg;
        unkF0->volumeRose = 1;
    } else {
        volume = MicTest_GetVolumeBracket(unkF0->curVolume);
        unkF0->curVolume -= 2;
    }

    if (volume) {
        if (unkF0->volumeRose) {
            ManagedSprite_SetAnim(micTest->sprites[2], 1);
            unkF0->volumeRose = 0;
        }

        if (MicTest_AreFloatingSpritesIdle(micTest)) {
            MicTest_SpawnFloatingSprite(micTest, micTest->heapID, 0x80, 0x40);
        } else {
            micTest->spawnTimer--;
            if (micTest->spawnTimer == 0) {
                MicTest_SpawnFloatingSprite(micTest, micTest->heapID, 0x80, 0x40);
                micTest->spawnTimer = 10;
            }
        }

        ManagedSprite_SetAnim(micTest->sprites[0], volume);
        ManagedSprite_SetAnim(micTest->sprites[1], volume);
    } else if (volume == 0) {
        ManagedSprite_SetAnim(micTest->sprites[2], 0);
        ManagedSprite_SetAnim(micTest->sprites[0], 0);
        ManagedSprite_SetAnim(micTest->sprites[1], 0);
    }

    sMicTestCallbackCount.unk0++;
}

static u32 MicTest_GetVolumeBracket(u8 volume) {
    u32 ret;

    if (volume <= 140) {
        ret = 0;
    } else if (volume >= 141 && volume <= 152) {
        ret = 1;
    } else if (volume >= 153 && volume <= 166) {
        ret = 2;
    } else if (volume >= 167 && volume <= 187) {
        ret = 3;
    } else if (volume >= 188) {
        ret = 4;
    } else {
        GF_ASSERT(FALSE);
    }

    return ret;
}

static u32 MicTest_AreFloatingSpritesIdle(MicTestData *micTest) {
    for (int i = 0; i < 3; i++) {
        if (micTest->floatingSprites[i].task) {
            return 0;
        }
    }
    return 1;
}

static BOOL MicTest_SpawnFloatingSprite(MicTestData *micTest, enum HeapID heapID, s16 x, s16 y) {
    ManagedSprite *flag = 0;
    MicTestFloatingSprite *args = NULL;
    for (int i = 0; i < 3; i++) {
        if (micTest->floatingSprites[i].task == NULL) {
            args = &micTest->floatingSprites[i];
            flag = micTest->sprites[i + 4];
            break;
        }
    }

    if (args != NULL) {
        s32 rand1 = (LCRandom() * 4098 / 0x10000) + 4915;
        s32 rand2 = (LCRandom() * 62 / 0x10000) + 60;
        s32 rand3 = (LCRandom() * 16 / 0x10000) + 6;
        MicTestFloatingSprite_Start(flag, args, x, y, rand1, rand2, rand3, 1);
        return TRUE;
    }

    return FALSE;
}

static void MicTestFloatingSprite_Start(ManagedSprite *a0, MicTestFloatingSprite *args, s16 x, s16 y, s32 z, s32 r1, s32 r2, BOOL a7) {
    args->task = SysTask_CreateOnMainQueue(Task_MicTestFloatingSprite, args, 0);
    MicTestFloatingSprite *data = SysTask_GetData(args->task);

    data->sprite = a0;
    data->delay = a7;

    data->pos.x = FX32_CONST(x);
    data->pos.y = FX32_CONST(y);
    data->pos.z = 0;

    s32 sine = GF_SinDegNoWrap(r1 + 90);
    data->velocity.x = FX_MUL(z, sine);
    sine = GF_SinDegNoWrap(r1);
    data->velocity.y = -FX_MUL(z, sine);
    data->velocity.z = 0;

    data->wobbleAngle = 0;
    data->wobbleSpeed = FX32_CONST(6);
    data->wobbleAmplitude = r2;

    ManagedSprite_SetPositionXY(data->sprite, x, y);
}

static void Task_MicTestFloatingSprite(SysTask *task, void *_data) {
    MicTestFloatingSprite *data = _data;

    if (data->delay) {
        if (--data->delay == 0) {
            ManagedSprite_SetDrawFlag(data->sprite, TRUE);
        }
        return;
    }

    VEC_Add(&data->pos, &data->velocity, &data->pos);

    s16 x = data->pos.x >> FX32_SHIFT;
    s16 y = data->pos.y >> FX32_SHIFT;
    data->wobbleAngle = data->wobbleAngle + data->wobbleSpeed;

    while (data->wobbleAngle > FX32_CONST(360)) {
        data->wobbleAngle -= FX32_CONST(360);
    }

    x += GF_SinDegNoWrap(data->wobbleAngle >> FX32_SHIFT) * data->wobbleAmplitude >> FX32_SHIFT;

    ManagedSprite_SetPositionXY(data->sprite, x, y);

    if (y < -16) {
        ManagedSprite_SetDrawFlag(data->sprite, FALSE);
        SysTask_Destroy(task);
        data->task = NULL;
    }
}

static void MicTest_EndTasks(MicTestData *micTest) {
    for (int i = 0; i < 3; i++) {
        if (micTest->floatingSprites[i].task != NULL) {
            SysTask_Destroy(micTest->floatingSprites[i].task);
            micTest->floatingSprites[i].task = NULL;
        }
    }
}

static int MicTest_CheckTouchscreenInput() {
    return TouchscreenHitbox_FindRectAtTouchNew(&sMicTestTouchscreenHitboxes[0]);
}

static int MicTest_CheckReturn(MicTestData *data) {
    if (MicTest_CheckTouchscreenInput() == TS_HITBOX_MIC_TEST_RETURN || (gSystem.newKeys & PAD_BUTTON_B)) {
        return TRUE;
    }
    return FALSE;
}
