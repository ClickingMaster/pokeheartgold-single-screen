#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0187.h"

#include "bg_window.h"
#include "camera.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "hatch_egg_app_internal.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "particle_system.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "swap_3d_buffers.h"
#include "text.h"
#include "vram_transfer_manager.h"

typedef struct HatchEggSpriteFiles {
    u32 files[8];
} HatchEggSpriteFiles;

static void HatchEggApp_Init3D(void);
static u32 HatchEggApp_AllocParticleTexVram(u32 size, BOOL is4x4comp);
static u32 HatchEggApp_AllocParticlePlttVram(u32 size, BOOL is4pltt);
static SPLEmitter *HatchEggApp_CreateParticleSystem(enum HeapID heapId);
static SPLEmitter *HatchEggApp_CreateParticleSystemFromNarc(enum HeapID heapId, NarcId narcId, int fileId);
static void HatchEggParticles_EmitterCB(SPLEmitter *emitter);
static BOOL HatchEggApp_StepEggShake(HatchEggApp *app, int pattern);

u32 GetUserFrameGfx2PaletteMember(u32 frame);

static const GraphicsModes sHatchEggAppGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const OamCharTransferParam sHatchEggAppOamCharTransferParam = {
    .maxTasks = 96,
    .sizeMain = 0x10000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_64K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const SpriteResourceCountsListUnion sHatchEggAppSpriteResourceCounts = {
    .numChar = 96,
    .numPltt = 32,
    .numCell = 64,
    .numAnim = 64,
    .numMcel = 16,
    .numManm = 16,
};

static const HatchEggSpriteFiles sHatchEggSpriteFiles = {
    .files = { 4, 9, 3, 2, 4, 9, 3, 2 },
};

static const OamManagerParam sHatchEggAppOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const GraphicsBanks sHatchEggAppGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_32_H,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_01_BC,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

static const s16 sHatchEggShakeOffsetsX[][10] = {
    { -1, 1, 1, -1, -1, 1, 1,   -1,  255, 255 },
    { -2, 2, 2, -2, -2, 2, 2,   -2,  255, 255 },
    { -3, 3, 3, -3, -3, 3, 255, 255, 0,   0   },
    { -3, 3, 3, -3, -3, 3, 255, 255, 0,   0   },
};

static const s16 sHatchEggShakeOffsetsY[][10] = {
    { 0, 255, 0, 0, 0, 0,  0,  0,  0, 0   },
    { 0, 0,   1, 1, 0, -1, -1, 0,  0, 255 },
    { 0, 0,   1, 1, 1, -1, -1, -1, 0, 0   },
    { 0, 0,   1, 1, 1, -1, -1, -1, 0, 0   },
};

static const HatchEggBgTemplates sHatchEggBgTemplates = {
    .templates = {
                  {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x2000,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x1000,
            .charBase = GX_BG_CHARBASE_0x0c000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x1000,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x3000,
            .charBase = GX_BG_CHARBASE_0x10000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 2,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  },
};

static const f32 sHatchEggShakeScalesX[][10] = {
    { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 1.1f, 1.1f, 1.1f, 1.1f, 1.1f, 1.0f, 1.0f, 0.0f },
    { 1.0f, 1.1f, 1.1f, 1.2f, 1.2f, 1.2f, 1.1f, 1.1f, 1.0f, 0.0f },
    { 1.0f, 1.1f, 1.1f, 1.2f, 1.2f, 1.2f, 1.1f, 1.1f, 1.0f, 0.0f },
};

static const f32 sHatchEggShakeScalesY[][10] = {
    { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 1.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 0.9f, 0.8f, 0.7f, 0.7f, 0.8f, 0.9f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 0.9f, 0.8f, 0.7f, 0.7f, 0.8f, 0.9f, 1.0f, 1.0f },
};

void HatchEggApp_HideLayers(void) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
}

void HatchEggApp_InitBlend(void) {
    G2x_SetBlendAlpha_(0x04000050, 0, 14, 11, 7);
    G2x_SetBlendAlpha_(0x04001050, 0, 14, 7, 8);
}

GF3DVramMan *HatchEggApp_Create3DVramMan(void) {
    return GF_3DVramMan_Create(HEAP_ID_70, 0, 2, 0, 2, HatchEggApp_Init3D);
}

static void HatchEggApp_Init3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0, 0, 0x7FFF, 0x3F, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}

void HatchEggApp_InitParticleVram(void) {
    u32 tex = NNS_GfdAllocTexVram(0x8000, FALSE, 0);
    u32 pltt = NNS_GfdAllocPlttVram(0x80, FALSE, 0);
    if (tex == 0) {
        GF_AssertFail();
    }
    if (pltt == 0) {
        GF_AssertFail();
    }
    ParticleSystem_ZeroAll();
}

void HatchEggApp_InitBgs(BgConfig *bgConfig) {
    GraphicsBanks banks;
    GraphicsModes modes;
    HatchEggBgTemplates templates;
    GfGfx_DisableEngineAPlanes();
    banks = sHatchEggAppGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MI_CpuClear32((void *)HW_BG_VRAM, 0x80000);
    MI_CpuClear32((void *)HW_DB_BG_VRAM, 0x20000);
    MI_CpuClear32((void *)HW_OBJ_VRAM, 0x40000);
    MI_CpuClear32((void *)HW_DB_OBJ_VRAM, 0x20000);
    modes = sHatchEggAppGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    templates = sHatchEggBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &templates.templates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &templates.templates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &templates.templates[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    G2_SetBG0Priority(1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void HatchEggApp_VBlankCB(void *data) {
    HatchEggApp *app = data;
    PokepicManager_HandleLoadImgAndOrPltt(app->gfx.pokepicManager);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(app->gfx.paletteData);
    DoScheduledBgGpuUpdates(app->gfx.bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void HatchEggApp_Draw3D(void) {
    Thunk_G3X_Reset();
    if (ParticleSystem_DrawAll() > 0) {
        Thunk_G3X_Reset();
        NNS_G2dSetupSoftwareSpriteCamera();
    }
    ParticleSystem_CalcAll();
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

void HatchEggApp_FadeIn(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_70);
}

void HatchEggApp_FadeOut(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_70);
}

void HatchEggApp_LoadWindowGfx(BgConfig *bgConfig, PaletteData *paletteData, u32 frame) {
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_1, 0x14, 0xF, (u8)frame, HEAP_ID_70);
    PaletteData_LoadNarc(paletteData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_70, PLTTBUF_MAIN_BG, 0x20, 0xC0);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_1, 0x32, 0xD, 0, HEAP_ID_70);
    PaletteData_LoadNarc(paletteData, NARC_a_0_3_8, GetUserFrameGfx1PaletteMember(), HEAP_ID_70, PLTTBUF_MAIN_BG, 0x20, 0xD0);
    PaletteData_LoadNarc(paletteData, NARC_graphic_font, 8, HEAP_ID_70, PLTTBUF_MAIN_BG, 0x20, 0xE0);
}

void HatchEggApp_CreateMessageWindow(BgConfig *bgConfig, Window *window, int bgId, int x, int y, int width, int height, int baseTile, int paletteNum) {
    InitWindow(window);
    AddWindowParameterized(bgConfig, window, (u8)bgId, (u8)x, (u8)y, (u8)width, (u8)height, (u8)paletteNum, (u16)baseTile);
    DrawFrameAndWindow2(window, TRUE, 0x14, 0xC);
    FillWindowPixelBuffer(window, 0xF);
    CopyWindowToVram(window);
}

// Prints message msgId of the hatch scene's bank with the Pokemon's species name; returns the printer id.
int HatchEggApp_PrintMessage(Window *window, int msgId, Pokemon *mon, int textSpeed) {
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *template;
    String *string;
    int printerId;
    FillWindowPixelBuffer(window, 0xF);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0187_bin, HEAP_ID_70);
    msgFormat = MessageFormat_New(HEAP_ID_70);
    template = NewString_ReadMsgData(msgData, msgId);
    string = String_New(0xFF, HEAP_ID_70);
    BufferBoxMonSpeciesName(msgFormat, 0, Mon_GetBoxMon(mon));
    StringExpandPlaceholders(msgFormat, string, template);
    printerId = AddTextPrinterParameterized(window, 1, string, 0, 0, textSpeed, NULL);
    DestroyMsgData(msgData);
    String_Delete(string);
    String_Delete(template);
    MessageFormat_Delete(msgFormat);
    return printerId;
}

void HatchEggApp_RemoveMessageWindow(Window *window) {
    ClearWindowTilemapAndCopyToVram(window);
    RemoveWindow(window);
}

void HatchEggApp_LoadBackground(BgConfig *bgConfig, PaletteData *paletteData) {
    GfGfxLoader_LoadCharData(NARC_a_1_1_5, 0, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_70);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_5, 1, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_70);
    PaletteData_LoadNarc(paletteData, NARC_a_1_1_5, 8, HEAP_ID_70, PLTTBUF_MAIN_BG, 0x40, 0);
}

static u32 HatchEggApp_AllocParticleTexVram(u32 size, BOOL is4x4comp) {
    NNSGfdTexKey key = NNS_GfdAllocTexVram(size, is4x4comp, 0);
    ParticleSystem_RegisterTexKey(key);
    return NNS_GfdGetTexKeyAddr(key);
}

static u32 HatchEggApp_AllocParticlePlttVram(u32 size, BOOL is4pltt) {
    NNSGfdPlttKey key = NNS_GfdAllocPlttVram(size, is4pltt, 0);
    ParticleSystem_RegisterPlttKey(key);
    return NNS_GfdGetPlttKeyAddr(key);
}

static SPLEmitter *HatchEggApp_CreateParticleSystem(enum HeapID heapId) {
    void *particleHeap = Heap_Alloc(heapId, 0x4800);
    SPLEmitter *particleSystem = ParticleSystem_New(HatchEggApp_AllocParticleTexVram, HatchEggApp_AllocParticlePlttVram, particleHeap, 0x4800, TRUE, heapId);
    Camera *camera = ParticleSystem_GetCamera(particleSystem);
    if (camera != NULL) {
        Camera_SetPerspectiveClippingPlane(FX32_CONST(1), FX32_CONST(900), camera);
    }
    return particleSystem;
}

static SPLEmitter *HatchEggApp_CreateParticleSystemFromNarc(enum HeapID heapId, NarcId narcId, int fileId) {
    SPLEmitter *particleSystem = HatchEggApp_CreateParticleSystem(heapId);
    void *resource = ParticleSystem_LoadResourceFile(narcId, fileId, heapId);
    ParticleSystem_SetResource(particleSystem, resource, 10, TRUE);
    return particleSystem;
}

void HatchEggApp_DestroyParticleSystem(SPLEmitter *particleSystem) {
    void *particleHeap = ParticleSystem_GetHeap(particleSystem);
    ParticleSystem_Free(particleSystem);
    Heap_Free(particleHeap);
}

static void HatchEggParticles_EmitterCB(SPLEmitter *emitter) {
    VecFx32 pos = { 0, 0, 0 };
    SPL_SetEmitterPosition(emitter, &pos);
}

HatchEggParticles *HatchEggParticles_New(HatchEggParticlesParam *param) {
    HatchEggParticles *particles = Heap_Alloc(param->heapId, sizeof(HatchEggParticles));
    if (particles == NULL) {
        GF_AssertFail();
    }
    {
        volatile HatchEggParticlesParam *input = param;
        enum HeapID heapId = input->heapId;
        u32 fileId = input->fileId;
        particles->heapId = heapId;
        particles->fileId = fileId;
        particles->particleSystem = HatchEggApp_CreateParticleSystemFromNarc(particles->heapId, NARC_a_1_1_6, fileId);
    }
    ParticleSystem_SetPerspectiveType(particles->particleSystem, 1);
    return particles;
}

void HatchEggParticles_Emit(HatchEggParticles *particles, int resId) {
    ParticleSystem_CreateEmitterWithParam(particles->particleSystem, resId, HatchEggParticles_EmitterCB, particles);
    ParticleSystem_SetPerspectiveType(particles->particleSystem, 1);
}

BOOL HatchEggParticles_IsRunning(HatchEggParticles *particles) {
    return ParticleSystem_GetActiveEmitterCount(particles->particleSystem) != FALSE;
}

void HatchEggParticles_Free(HatchEggParticles *particles) {
    Heap_Free(particles);
}

void HatchEggApp_LoadSpriteResources(HatchEggApp *app) {
    HatchEggSpriteFiles eggFiles = sHatchEggSpriteFiles;
    SpriteSystem *spriteSystem = app->gfx.spriteSystem;
    SpriteManager *spriteManager = app->gfx.spriteManager;
    PaletteData *paletteData = app->gfx.paletteData;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_1_5, eggFiles.files[0], TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 20000);
    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_a_1_1_5, eggFiles.files[1], FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 20001);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_1_5, eggFiles.files[2], TRUE, 20002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_1_5, eggFiles.files[3], TRUE, 20003);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_1_5, 7, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 25000);
    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_a_1_1_5, 10, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 25001);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_1_5, 6, TRUE, 25002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_1_5, 5, TRUE, 25003);
}

void HatchEggApp_CreateSprites(HatchEggApp *app) {
    ManagedSpriteTemplate template;
    u32 zero = 0;
    int bgPriority = 2;
    template.x = 128;
    template.y = 120;
    template.z = zero;
    template.animation = zero;
    template.drawPriority = zero;
    template.pal = zero;
    template.vramTransfer = zero;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = bgPriority;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 20000;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 20001;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 20002;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 20003;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = bgPriority - 3;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = bgPriority - 3;
    app->eggSprite = SpriteSystem_NewSprite(app->gfx.spriteSystem, app->gfx.spriteManager, &template);
    ManagedSprite_TickFrame(app->eggSprite);
    ManagedSprite_SetAffineOverwriteMode(app->eggSprite, 2);
    template.x = 0;
    template.y = -16;
    template.bgPriority = 0;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 25000;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 25001;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 25002;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 25003;
    app->topBar = SpriteSystem_NewSprite(app->gfx.spriteSystem, app->gfx.spriteManager, &template);
    ManagedSprite_TickFrame(app->topBar);
    template.y = 144;
    app->bottomBar = SpriteSystem_NewSprite(app->gfx.spriteSystem, app->gfx.spriteManager, &template);
    ManagedSprite_TickFrame(app->bottomBar);
}

// Plays one frame of shake pattern `pattern` (tables sHatchEggShakeOffsetsX/Y: position offsets, 0xFF = end;
// sHatchEggShakeScalesX/Y: affine scale, 0 = end). Returns FALSE when the pattern has ended.
static BOOL HatchEggApp_StepEggShake(HatchEggApp *app, int pattern) {
    int *shake = &app->shakeState;
    int finished;
    s16 x;
    s16 y;
    f32 scaleX;
    f32 scaleY;
    switch (shake[0]) {
    case 0:
        if (shake[1] >= 10) {
            shake[0]++;
            shake[1] = 0;
        } else {
            finished = 0;
            x = sHatchEggShakeOffsetsX[pattern][shake[1]];
            y = sHatchEggShakeOffsetsY[pattern][shake[1]];
            scaleX = sHatchEggShakeScalesX[pattern][shake[1]];
            scaleY = sHatchEggShakeScalesY[pattern][shake[1]];
            if (x == 0xFF) {
                x = 0;
                finished++;
            }
            if (y == 0xFF) {
                y = 0;
                finished++;
            }
            ManagedSprite_OffsetPositionXY(app->eggSprite, x, y);
            if (scaleX != 0.0f) {
                ManagedSprite_SetAffineScale(app->eggSprite, scaleX, scaleY);
            } else {
                finished++;
            }
            if (finished != 3) {
                shake[1]++;
                break;
            }
            shake[0]++;
        }
        // fallthrough
    default:
        shake[0] = 0;
        shake[1] = 0;
        return FALSE;
    }
    return TRUE;
}

// Returns 2 when the shake has ended, 1 on its sixth frame (the hit) and 0 otherwise.
int HatchEggApp_UpdateEggShake(HatchEggApp *app, int pattern) {
    if (HatchEggApp_StepEggShake(app, pattern) == FALSE) {
        return 2;
    }
    if (pattern == 4) {
        if (app->shakeFrame == 6) {
            return 1;
        }
    } else if (app->shakeFrame == 6) {
        return 1;
    }
    return 0;
}

void HatchEggApp_DeleteSprites(HatchEggApp *app) {
    Sprite_DeleteAndFreeResources(app->eggSprite);
    Sprite_DeleteAndFreeResources(app->topBar);
    Sprite_DeleteAndFreeResources(app->bottomBar);
}

void HatchEggApp_InitSpriteSystem(HatchEggAppGraphics *gfx) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamTransferParam;
    SpriteResourceCountsListUnion resourceCounts;
    gfx->spriteSystem = SpriteSystem_Alloc(HEAP_ID_70);
    oamManagerParam = sHatchEggAppOamManagerParam;
    oamTransferParam = sHatchEggAppOamCharTransferParam;
    SpriteSystem_Init(gfx->spriteSystem, &oamManagerParam, &oamTransferParam, 32);
    resourceCounts = sHatchEggAppSpriteResourceCounts;
    gfx->spriteManager = SpriteManager_New(gfx->spriteSystem);
    GF_ASSERT(SpriteSystem_InitSprites(gfx->spriteSystem, gfx->spriteManager, 255));
    GF_ASSERT(SpriteSystem_InitManagerWithCapacities(gfx->spriteSystem, gfx->spriteManager, &resourceCounts));
}

void HatchEggApp_FreeSpriteSystem(HatchEggAppGraphics *gfx) {
    SpriteSystem_FreeResourcesAndManager(gfx->spriteSystem, gfx->spriteManager);
    SpriteSystem_Free(gfx->spriteSystem);
}

void HatchEggApp_CreatePokepic(HatchEggApp *app) {
    Pokemon *mon = app->args->args.mon;
    u32 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u32 yOffset = GetBoxMonPicHeight((BoxPokemon *)mon, 2);
    u32 isEgg = FALSE;
    PokepicTemplate template;
    PokepicAnimScript animScript[10];
    SetMonData(mon, MON_DATA_IS_EGG, &isEgg);
    GetPokemonSpriteCharAndPlttNarcIds(&template, mon, 2);
    NARC_ReadPokepicAnimScript(app->gfx.narc, animScript, (u16)species, 1);
    app->pokepic = PokepicManager_CreatePokepic(app->gfx.pokepicManager, &template, 128, yOffset + 96, 0, 0, animScript, NULL);
}

void HatchEggApp_StartPokepicAnim(HatchEggApp *app) {
    u32 species = GetMonData(app->args->args.mon, MON_DATA_SPECIES, NULL);
    GetMonNature(app->args->args.mon);
    Pokepic_StartAnim(app->pokepic, 1);
    NARC_StartPokepicAnim(app->gfx.narc, app->picManager, app->pokepic, (u16)species, 2, 0, 0);
}

void HatchEggApp_DeletePokepic(HatchEggApp *app) {
    Pokepic_Delete(app->pokepic);
}

void HatchEggApp_SetPokepicVanished(HatchEggApp *app, int vanished) {
    Pokepic_SetAttr(app->pokepic, POKEPIC_VANISHED, vanished);
}
