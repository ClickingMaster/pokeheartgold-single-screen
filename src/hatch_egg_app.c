#include "hatch_egg_app.h"

#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "hatch_egg_task.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokepic.h"
#include "pokepic_anim_internal.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sound_chatot.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "particle_system.h"
#include "swap_3d_buffers.h"
#include "field_environment_sound.h"
#include "vram_transfer_manager.h"

// Layout of HatchEggTaskData (hatch_egg_task.c), which the task passes as the overlay's args.
typedef struct HatchEggAppArgs {
    u32 taskState;
    BOOL wantToNameNewMon; // set by the nickname prompt
    void *namingScreenArgs;
    HatchEggArgs hatch;
} HatchEggAppArgs;

typedef struct HatchEggParticlesParam {
    enum HeapID heapId;
    u32 fileId; // NARC_a_1_1_6 member: 0 = egg, 1 = Manaphy egg
} HatchEggParticlesParam;

typedef struct HatchEggParticles {
    enum HeapID heapId;
    u32 fileId;
    u32 unk_08;
    SPLEmitter *particleSystem;
} HatchEggParticles;

// The "Give a nickname?" yes/no buttons on the touch screen.
typedef struct HatchEggYesNoMenu {
    enum HeapID heapId;           // 0x00
    SpriteSystem *spriteSystem;   // 0x04
    SpriteManager *spriteManager; // 0x08
    BgConfig *bgConfig;           // 0x0C
    ManagedSprite *cursor;        // 0x10
    MessageFormat *msgFormat;     // 0x14
    u8 state;                     // 0x18
    u8 selection;                 // 0x19: 0 = none, 1 = yes, 2 = no
    Window buttons[2];            // 0x1C
} HatchEggYesNoMenu;              // size: 0x3C

typedef struct HatchEggApp {
    HatchEggAppArgs *args;          // 0x00
    BgConfig *bgConfig;             // 0x04
    PaletteData *paletteData;       // 0x08
    u32 printerId;                  // 0x0C
    u32 textSpeed;                  // 0x10
    u32 frame;                      // 0x14
    Window window;                  // 0x18
    u8 filler_28[0x10];             // 0x28
    GF3DVramMan *vramMan;           // 0x38
    PokepicManager *pokepicManager; // 0x3C
    NARC *narc;                     // 0x40
    HatchEggParticles *particles;   // 0x44
    u8 filler_48[8];                // 0x48
    SpriteManager *spriteManager;   // 0x50
    SpriteSystem *spriteSystem;     // 0x54
    PokepicAnimManager *picManager;         // 0x58
    u32 sceneType;                  // 0x5C: index into gHatchEggSceneFuncs
    u32 state;                      // 0x60
    s32 timer;                      // 0x64
    u32 subState;                   // 0x68
    u8 animParams[4];               // 0x6C
    Pokepic *pokepic;               // 0x70
    ManagedSprite *eggSprite;       // 0x74
    ManagedSprite *topBar;          // 0x78
    ManagedSprite *bottomBar;       // 0x7C
    int shakeState;                 // 0x80
    int shakeFrame;                 // 0x84
    HatchEggYesNoMenu *yesNoMenu;   // 0x88
} HatchEggApp;                      // size: 0x8C

// HatchEggApp_InitSpriteSystem and HatchEggApp_FreeSpriteSystem are passed &app->bgConfig and address
// the sprite system relative to it.
typedef struct HatchEggAppFromBgConfig {
    u8 filler_00[0x4C];
    SpriteManager *spriteManager; // HatchEggApp 0x50
    SpriteSystem *spriteSystem;   // HatchEggApp 0x54
} HatchEggAppFromBgConfig;

typedef struct HatchEggBgTemplates {
    BgTemplate templates[3];
} HatchEggBgTemplates;

typedef struct HatchEggSpriteFiles {
    u32 files[8];
} HatchEggSpriteFiles;

typedef BOOL (*HatchEggSceneFunc)(HatchEggApp *app);

extern const GraphicsModes gHatchEggAppGraphicsModes;
extern const OamCharTransferParam gHatchEggAppOamCharTransferParam;
extern const SpriteResourceCountsListUnion gHatchEggAppSpriteResourceCounts;
extern const OamManagerParam gHatchEggAppOamManagerParam;
extern const HatchEggSpriteFiles gHatchEggSpriteFiles;
extern const GraphicsBanks gHatchEggAppGraphicsBanks;
extern const s16 gHatchEggShakeOffsetsX[][10];
extern const s16 gHatchEggShakeOffsetsY[][10];
extern const HatchEggBgTemplates gHatchEggBgTemplates;
extern const f32 gHatchEggShakeScalesX[][10];
extern const f32 gHatchEggShakeScalesY[][10];
extern const HatchEggSceneFunc gHatchEggSceneFuncs[];
extern const s16 gHatchEggYesNoCursorPositions[2][2];
extern const TouchscreenHitbox gHatchEggYesNoHitboxes[];
extern const ManagedSpriteTemplate gHatchEggYesNoCursorTemplate;
extern const HatchEggBgTemplates gHatchEggYesNoBgTemplates;

u32 GetUserFrameGfx2PaletteMember(u32 frame);
void NNS_G2dSetupSoftwareSpriteCamera(void);

void HatchEggApp_HideLayers(void);
void HatchEggApp_InitBlend(void);
GF3DVramMan *HatchEggApp_Create3DVramMan(void);
void HatchEggApp_Init3D(void);
void HatchEggApp_InitParticleVram(void);
void HatchEggApp_InitBgs(BgConfig *bgConfig);
void HatchEggApp_VBlankCB(void *data);
void HatchEggApp_Draw3D(void);
void HatchEggApp_FadeIn(void);
void HatchEggApp_FadeOut(void);
void HatchEggApp_LoadWindowGfx(BgConfig *bgConfig, PaletteData *paletteData, u32 frame);
void HatchEggApp_CreateMessageWindow(BgConfig *bgConfig, Window *window, int bgId, int x, int y, int width, int height, int baseTile, int paletteNum);
int HatchEggApp_PrintMessage(Window *window, int msgId, Pokemon *mon, int textSpeed);
void HatchEggApp_RemoveMessageWindow(Window *window);
void HatchEggApp_LoadBackground(BgConfig *bgConfig, PaletteData *paletteData);
u32 HatchEggApp_AllocParticleTexVram(u32 size, BOOL is4x4comp);
u32 HatchEggApp_AllocParticlePlttVram(u32 size, BOOL is4pltt);
SPLEmitter *HatchEggApp_CreateParticleSystem(enum HeapID heapId);
SPLEmitter *HatchEggApp_CreateParticleSystemFromNarc(enum HeapID heapId, NarcId narcId, int fileId);
void HatchEggApp_DestroyParticleSystem(SPLEmitter *particleSystem);
void HatchEggParticles_EmitterCB(SPLEmitter *emitter);
HatchEggParticles *HatchEggParticles_New(HatchEggParticlesParam *param);
void HatchEggParticles_Emit(HatchEggParticles *particles, int resId);
BOOL HatchEggParticles_IsRunning(HatchEggParticles *particles);
void HatchEggParticles_Free(HatchEggParticles *particles);
void HatchEggApp_LoadSpriteResources(HatchEggApp *app);
void HatchEggApp_CreateSprites(HatchEggApp *app);
BOOL HatchEggApp_StepEggShake(HatchEggApp *app, int pattern);
int HatchEggApp_UpdateEggShake(HatchEggApp *app, int pattern);
void HatchEggApp_DeleteSprites(HatchEggApp *app);
void HatchEggApp_InitSpriteSystem(HatchEggAppFromBgConfig *app);
void HatchEggApp_FreeSpriteSystem(HatchEggAppFromBgConfig *app);
void HatchEggApp_CreatePokepic(HatchEggApp *app);
void HatchEggApp_StartPokepicAnim(HatchEggApp *app);
void HatchEggApp_DeletePokepic(HatchEggApp *app);
void HatchEggApp_SetPokepicVanished(HatchEggApp *app, int vanished);
BOOL HatchEggApp_RunScene(HatchEggApp *app);
BOOL HatchEggApp_RunEggScene(HatchEggApp *app);
BOOL HatchEggApp_RunManaphyEggScene(HatchEggApp *app);
BOOL HatchEggApp_WaitBeforeShake(HatchEggApp *app);
BOOL HatchEggApp_ShakeEggOnce(HatchEggApp *app);
BOOL HatchEggApp_ShakeEggTwice(HatchEggApp *app);
BOOL HatchEggApp_ShakeEggCrack(HatchEggApp *app);
BOOL HatchEggApp_HatchEgg(HatchEggApp *app);
BOOL HatchEggApp_HatchManaphyEgg(HatchEggApp *app);
void HatchEggYesNoMenu_InitBgs(BgConfig *bgConfig);
HatchEggYesNoMenu *HatchEggYesNoMenu_New(BgConfig *bgConfig, SpriteSystem *spriteSystem, SpriteManager *spriteManager, enum HeapID heapId);
void HatchEggYesNoMenu_Free(HatchEggYesNoMenu *menu);
void HatchEggYesNoMenu_LoadGfx(HatchEggYesNoMenu *menu, PaletteData *paletteData, int charResId, int plttResId, int cellResId, int animResId);
void HatchEggYesNoMenu_Show(HatchEggYesNoMenu *menu, u32 yesMsgId, u32 noMsgId);
int HatchEggYesNoMenu_Run(HatchEggYesNoMenu *menu);
void HatchEggYesNoMenu_AddWindows(HatchEggYesNoMenu *menu);
void HatchEggYesNoMenu_RemoveWindows(HatchEggYesNoMenu *menu);
void HatchEggYesNoMenu_PrintButton(HatchEggYesNoMenu *menu, Window *window, u32 msgId);
void HatchEggYesNoMenu_CreateCursor(HatchEggYesNoMenu *menu, int charResId, int plttResId, int cellResId, int animResId);
void HatchEggYesNoMenu_DeleteCursor(HatchEggYesNoMenu *menu);
void HatchEggYesNoMenu_UpdateCursor(HatchEggYesNoMenu *menu);
BOOL HatchEggYesNoMenu_HandleInput(HatchEggYesNoMenu *menu);
BOOL HatchEggYesNoMenu_IsPressAnimFinished(HatchEggYesNoMenu *menu);

void HatchEggApp_HideLayers(void) {
    // DISPCNT / DB_DISPCNT: turn off BG0-3 and OBJ
    volatile u32 *dispcntMain = (volatile u32 *)0x04000000, *dispcntSub = (volatile u32 *)0x04001000;
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *dispcntMain &= 0xffffe0ff;
    *dispcntSub &= 0xffffe0ff;
}

void HatchEggApp_InitBlend(void) {
    G2x_SetBlendAlpha_(0x04000050, 0, 14, 11, 7);
    G2x_SetBlendAlpha_(0x04001050, 0, 14, 7, 8);
}

GF3DVramMan *HatchEggApp_Create3DVramMan(void) {
    return GF_3DVramMan_Create(HEAP_ID_70, 0, 2, 0, 2, HatchEggApp_Init3D);
}

void HatchEggApp_Init3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
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
    banks = gHatchEggAppGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MI_CpuClear32((void *)HW_BG_VRAM, 0x80000);
    MI_CpuClear32((void *)HW_DB_BG_VRAM, 0x20000);
    MI_CpuClear32((void *)HW_OBJ_VRAM, 0x40000);
    MI_CpuClear32((void *)HW_DB_OBJ_VRAM, 0x20000);
    modes = gHatchEggAppGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    templates = gHatchEggBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &templates.templates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &templates.templates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &templates.templates[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    G2_SetBG0Priority(1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
}

void HatchEggApp_VBlankCB(void *data) {
    // OS_IRQTABLE_INTR_CHECK: acknowledge the V-blank interrupt
    volatile u32 *dtcm = (volatile u32 *)0x027e0000;
    HatchEggApp *app = data;
    PokepicManager_HandleLoadImgAndOrPltt(app->pokepicManager);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(app->paletteData);
    DoScheduledBgGpuUpdates(app->bgConfig);
    dtcm[0x3ff8 / 4] |= 1;
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

#pragma force_active on
void HatchEggApp_LoadBackground(BgConfig *bgConfig, PaletteData *paletteData) {
    GfGfxLoader_LoadCharData(NARC_a_1_1_5, 0, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_70);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_5, 1, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_70);
    PaletteData_LoadNarc(paletteData, NARC_a_1_1_5, 8, HEAP_ID_70, PLTTBUF_MAIN_BG, 0x40, 0);
}
#pragma force_active reset

u32 HatchEggApp_AllocParticleTexVram(u32 size, BOOL is4x4comp) {
    NNSGfdTexKey key = NNS_GfdAllocTexVram(size, is4x4comp, 0);
    ParticleSystem_RegisterTexKey(key);
    return NNS_GfdGetTexKeyAddr(key);
}

u32 HatchEggApp_AllocParticlePlttVram(u32 size, BOOL is4pltt) {
    NNSGfdPlttKey key = NNS_GfdAllocPlttVram(size, is4pltt, 0);
    ParticleSystem_RegisterPlttKey(key);
    return NNS_GfdGetPlttKeyAddr(key);
}

SPLEmitter *HatchEggApp_CreateParticleSystem(enum HeapID heapId) {
    void *particleHeap = Heap_Alloc(heapId, 0x4800);
    SPLEmitter *particleSystem = ParticleSystem_New(HatchEggApp_AllocParticleTexVram, HatchEggApp_AllocParticlePlttVram, particleHeap, 0x4800, TRUE, heapId);
    Camera *camera = ParticleSystem_GetCamera(particleSystem);
    if (camera != NULL) {
        Camera_SetPerspectiveClippingPlane(FX32_CONST(1), FX32_CONST(900), camera);
    }
    return particleSystem;
}

SPLEmitter *HatchEggApp_CreateParticleSystemFromNarc(enum HeapID heapId, NarcId narcId, int fileId) {
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

void HatchEggParticles_EmitterCB(SPLEmitter *emitter) {
    VecFx32 pos = { 0, 0, 0 };
    SPL_SetEmitterPosition(emitter, &pos);
}

#pragma force_active on
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
#pragma force_active reset

BOOL HatchEggParticles_IsRunning(HatchEggParticles *particles) {
    return ParticleSystem_GetActiveEmitterCount(particles->particleSystem) != FALSE;
}

void HatchEggParticles_Free(HatchEggParticles *particles) {
    Heap_Free(particles);
}

void HatchEggApp_LoadSpriteResources(HatchEggApp *app) {
    HatchEggSpriteFiles eggFiles = gHatchEggSpriteFiles;
    SpriteSystem *spriteSystem = app->spriteSystem;
    SpriteManager *spriteManager = app->spriteManager;
    PaletteData *paletteData = app->paletteData;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_1_5, eggFiles.files[0], TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 20000);
    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_a_1_1_5, eggFiles.files[1], FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 20001);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_1_5, eggFiles.files[2], TRUE, 20002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_1_5, eggFiles.files[3], TRUE, 20003);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_1_5, 7, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 25000);
    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_a_1_1_5, 10, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 25001);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_1_5, 6, TRUE, 25002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_1_5, 5, TRUE, 25003);
}

#pragma force_active on
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
    app->eggSprite = SpriteSystem_NewSprite(app->spriteSystem, app->spriteManager, &template);
    ManagedSprite_TickFrame(app->eggSprite);
    ManagedSprite_SetAffineOverwriteMode(app->eggSprite, 2);
    template.x = 0;
    template.y = -16;
    template.bgPriority = 0;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 25000;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 25001;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 25002;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 25003;
    app->topBar = SpriteSystem_NewSprite(app->spriteSystem, app->spriteManager, &template);
    ManagedSprite_TickFrame(app->topBar);
    template.y = 144;
    app->bottomBar = SpriteSystem_NewSprite(app->spriteSystem, app->spriteManager, &template);
    ManagedSprite_TickFrame(app->bottomBar);
}
#pragma force_active reset

// Plays one frame of shake pattern `pattern` (tables gHatchEggShakeOffsetsX/762C: position offsets, 0xFF = end;
// gHatchEggShakeScalesX/7770: affine scale, 0 = end). Returns FALSE when the pattern has ended.
BOOL HatchEggApp_StepEggShake(HatchEggApp *app, int pattern) {
    int *shake = &app->shakeState;
    int finished;
    s16 x;
    s16 y;
    f32 scaleX;
    f32 scaleY;
    if (shake[0] == 0) {
        if (shake[1] >= 10) {
            shake[0]++;
            shake[1] = 0;
        } else {
            finished = 0;
            x = gHatchEggShakeOffsetsX[pattern][shake[1]];
            y = gHatchEggShakeOffsetsY[pattern][shake[1]];
            scaleX = gHatchEggShakeScalesX[pattern][shake[1]];
            scaleY = gHatchEggShakeScalesY[pattern][shake[1]];
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
                goto running;
            }
            shake[0]++;
        }
    }
    shake[0] = 0;
    shake[1] = 0;
    return FALSE;
running:
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

#pragma force_active on
void HatchEggApp_InitSpriteSystem(HatchEggAppFromBgConfig *app) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamTransferParam;
    SpriteResourceCountsListUnion resourceCounts;
    app->spriteSystem = SpriteSystem_Alloc(HEAP_ID_70);
    oamManagerParam = gHatchEggAppOamManagerParam;
    oamTransferParam = gHatchEggAppOamCharTransferParam;
    SpriteSystem_Init(app->spriteSystem, &oamManagerParam, &oamTransferParam, 32);
    resourceCounts = gHatchEggAppSpriteResourceCounts;
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    GF_ASSERT(SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 255));
    GF_ASSERT(SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->spriteManager, &resourceCounts));
}
#pragma force_active reset

void HatchEggApp_FreeSpriteSystem(HatchEggAppFromBgConfig *app) {
    SpriteSystem_FreeResourcesAndManager(app->spriteSystem, app->spriteManager);
    SpriteSystem_Free(app->spriteSystem);
}

void HatchEggApp_CreatePokepic(HatchEggApp *app) {
    Pokemon *mon = app->args->hatch.mon;
    u32 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u32 yOffset = GetBoxMonPicHeight((BoxPokemon *)mon, 2);
    u32 isEgg = FALSE;
    PokepicTemplate template;
    PokepicAnimScript animScript[10];
    SetMonData(mon, MON_DATA_IS_EGG, &isEgg);
    GetPokemonSpriteCharAndPlttNarcIds(&template, mon, 2);
    NARC_ReadPokepicAnimScript(app->narc, animScript, (u16)species, 1);
    app->pokepic = PokepicManager_CreatePokepic(app->pokepicManager, &template, 128, yOffset + 96, 0, 0, animScript, NULL);
}

void HatchEggApp_StartPokepicAnim(HatchEggApp *app) {
    u32 species = GetMonData(app->args->hatch.mon, MON_DATA_SPECIES, NULL);
    GetMonNature(app->args->hatch.mon);
    // The call passes an argument Pokepic_StartAnim does not take.
    ((void (*)(Pokepic *, int))Pokepic_StartAnim)(app->pokepic, 1);
    NARC_StartPokepicAnim(app->narc, app->picManager, app->pokepic, (u16)species, 2, 0, 0);
}

void HatchEggApp_DeletePokepic(HatchEggApp *app) {
    Pokepic_Delete(app->pokepic);
}

void HatchEggApp_SetPokepicVanished(HatchEggApp *app, int vanished) {
    Pokepic_SetAttr(app->pokepic, POKEPIC_VANISHED, vanished);
}

BOOL HatchEggApp_RunScene(HatchEggApp *app) {
    return gHatchEggSceneFuncs[app->sceneType](app);
}

BOOL HatchEggApp_RunEggScene(HatchEggApp *app) {
    BOOL ret = TRUE;
    switch (app->state) {
    case 0: {
        HatchEggParticlesParam param;
        HatchEggApp_LoadBackground(app->bgConfig, app->paletteData);
        HatchEggApp_LoadWindowGfx(app->bgConfig, app->paletteData, app->frame);
        HatchEggApp_CreateMessageWindow(app->bgConfig, &app->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 59, 14);
        HatchEggApp_LoadSpriteResources(app);
        HatchEggApp_CreateSprites(app);
        HatchEggYesNoMenu_LoadGfx(app->yesNoMenu, app->paletteData, 21000, 21001, 21002, 21003);
        HatchEggApp_CreatePokepic(app);
        HatchEggApp_SetPokepicVanished(app, TRUE);
        param.heapId = HEAP_ID_70;
        param.fileId = 0;
        app->particles = HatchEggParticles_New(&param);
        app->state++;
        break;
    }
    case 1:
        HatchEggApp_FadeIn();
        app->state++;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            Sound_Stop();
            Sound_SetSceneAndPlayBGM(13, SEQ_GS_SHINKA, 1);
            app->state++;
        }
        break;
    case 3:
        if (HatchEggApp_WaitBeforeShake(app) == FALSE) {
            app->state++;
        }
        break;
    case 4:
        if (HatchEggApp_ShakeEggOnce(app) == FALSE) {
            app->state++;
        }
        break;
    case 5:
        if (HatchEggApp_ShakeEggTwice(app) == FALSE) {
            app->state++;
        }
        break;
    case 6:
        if (HatchEggApp_ShakeEggCrack(app) == FALSE) {
            app->state++;
        }
        break;
    case 7:
        if (HatchEggApp_HatchEgg(app) == FALSE) {
            HatchEggApp_StartPokepicAnim(app);
            PlayCry(GetMonData(app->args->hatch.mon, MON_DATA_SPECIES, NULL), GetMonData(app->args->hatch.mon, MON_DATA_FORM, NULL));
            app->printerId = HatchEggApp_PrintMessage(&app->window, 0, app->args->hatch.mon, app->textSpeed);
            app->state++;
        }
        break;
    case 8:
        if (IsCryFinished()) {
            break;
        }
        PlayFanfare(SEQ_ME_SHINKAOME);
        app->state++;
    case 9:
        if (IsFanfarePlaying()) {
            break;
        }
        app->state++;
    case 10:
        if (TextPrinterCheckActive(app->printerId)) {
            break;
        }
        app->printerId = HatchEggApp_PrintMessage(&app->window, 1, app->args->hatch.mon, app->textSpeed);
        app->state++;
    case 11:
        if (TextPrinterCheckActive(app->printerId)) {
            break;
        }
        if (PokepicAnimManager_GetAnimState(app->picManager, 0) != TRUE) {
            break;
        }
        if (Pokepic_IsAnimFinished(app->pokepic)) {
            break;
        }
        HatchEggYesNoMenu_Show(app->yesNoMenu, 2, 3);
        app->state++;
        break;
    case 12:
        switch (HatchEggYesNoMenu_Run(app->yesNoMenu)) {
        case 1:
            app->args->wantToNameNewMon = TRUE;
            app->state++;
            break;
        case 2:
            app->args->wantToNameNewMon = FALSE;
            app->state++;
            break;
        }
        break;
    case 13:
        app->state++;
        break;
    case 14:
        app->state++;
        break;
    case 15:
        HatchEggApp_FadeOut();
        app->state++;
    case 16:
        if (IsPaletteFadeFinished() == TRUE) {
            app->state++;
        }
        break;
    case 17:
    default:
        HatchEggApp_DeletePokepic(app);
        HatchEggApp_DeleteSprites(app);
        HatchEggApp_RemoveMessageWindow(&app->window);
        HatchEggApp_DestroyParticleSystem(app->particles->particleSystem);
        HatchEggParticles_Free(app->particles);
        ret = FALSE;
        break;
    }
    SpriteSystem_DrawSprites(app->spriteManager);
    return ret;
}

BOOL HatchEggApp_RunManaphyEggScene(HatchEggApp *app) {
    BOOL ret = TRUE;
    switch (app->state) {
    case 0: {
        HatchEggParticlesParam param;
        HatchEggApp_LoadBackground(app->bgConfig, app->paletteData);
        HatchEggApp_LoadWindowGfx(app->bgConfig, app->paletteData, app->frame);
        HatchEggApp_CreateMessageWindow(app->bgConfig, &app->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 59, 14);
        HatchEggApp_LoadSpriteResources(app);
        HatchEggApp_CreateSprites(app);
        ManagedSprite_SetDrawFlag(app->eggSprite, FALSE);
        HatchEggYesNoMenu_LoadGfx(app->yesNoMenu, app->paletteData, 21000, 21001, 21002, 21003);
        HatchEggApp_CreatePokepic(app);
        HatchEggApp_SetPokepicVanished(app, TRUE);
        param.heapId = HEAP_ID_70;
        param.fileId = 1;
        app->particles = HatchEggParticles_New(&param);
        app->state++;
        break;
    }
    case 1:
        HatchEggApp_FadeIn();
        app->state++;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            Sound_SetSceneAndPlayBGM(13, SEQ_GS_SHINKA, 1);
            app->state++;
        }
        break;
    case 3:
        if (HatchEggApp_HatchManaphyEgg(app) == FALSE) {
            HatchEggApp_StartPokepicAnim(app);
            PlayCry(GetMonData(app->args->hatch.mon, MON_DATA_SPECIES, NULL), GetMonData(app->args->hatch.mon, MON_DATA_FORM, NULL));
            app->printerId = HatchEggApp_PrintMessage(&app->window, 0, app->args->hatch.mon, app->textSpeed);
            app->state = 8;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        GF_AssertFail();
        break;
    case 8:
        if (IsCryFinished()) {
            break;
        }
        PlayFanfare(SEQ_ME_SHINKAOME);
        app->state++;
    case 9:
        if (IsFanfarePlaying()) {
            break;
        }
        app->state++;
    case 10:
        if (TextPrinterCheckActive(app->printerId)) {
            break;
        }
        app->printerId = HatchEggApp_PrintMessage(&app->window, 1, app->args->hatch.mon, app->textSpeed);
        app->state++;
    case 11:
        if (TextPrinterCheckActive(app->printerId)) {
            break;
        }
        if (PokepicAnimManager_GetAnimState(app->picManager, 0) != TRUE) {
            break;
        }
        if (Pokepic_IsAnimFinished(app->pokepic)) {
            break;
        }
        HatchEggYesNoMenu_Show(app->yesNoMenu, 2, 3);
        app->state++;
        break;
    case 12:
        switch (HatchEggYesNoMenu_Run(app->yesNoMenu)) {
        case 1:
            app->args->wantToNameNewMon = TRUE;
            app->state++;
            break;
        case 2:
            app->args->wantToNameNewMon = FALSE;
            app->state++;
            break;
        }
        break;
    case 13:
        app->state++;
        break;
    case 14:
        app->state++;
        break;
    case 15:
        HatchEggApp_FadeOut();
        app->state++;
    case 16:
        if (IsPaletteFadeFinished() == TRUE) {
            app->state++;
        }
        break;
    case 17:
    default:
        HatchEggApp_DeletePokepic(app);
        HatchEggApp_DeleteSprites(app);
        HatchEggApp_RemoveMessageWindow(&app->window);
        HatchEggApp_DestroyParticleSystem(app->particles->particleSystem);
        HatchEggParticles_Free(app->particles);
        ret = FALSE;
        break;
    }
    SpriteSystem_DrawSprites(app->spriteManager);
    return ret;
}

BOOL HatchEggApp_WaitBeforeShake(HatchEggApp *app) {
    switch (app->subState) {
    case 0:
        ManagedSprite_SetAnimationFrame(app->eggSprite, 0);
        app->subState++;
        break;
    case 1:
        app->timer++;
        if (app->timer >= 25) {
            PlaySE(SEQ_SE_DP_EGG01);
            app->subState++;
        }
        break;
    default:
        app->subState = 0;
        app->timer = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL HatchEggApp_ShakeEggOnce(HatchEggApp *app) {
    switch (app->subState) {
    case 0:
        if (HatchEggParticles_IsRunning(app->particles) != TRUE) {
            ManagedSprite_SetAnimationFrame(app->eggSprite, 1);
            app->subState++;
        }
        break;
    case 1:
        if (HatchEggApp_UpdateEggShake(app, 0) == 2) {
            PlaySE(SEQ_SE_DP_EGG01);
            ManagedSprite_SetAnimationFrame(app->eggSprite, 2);
            app->subState++;
            app->timer = 0;
        }
        break;
    default:
        if (HatchEggApp_UpdateEggShake(app, 0) == 2) {
            ManagedSprite_SetAnimationFrame(app->eggSprite, 2);
            app->subState = 0;
            app->timer = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL HatchEggApp_ShakeEggTwice(HatchEggApp *app) {
    switch (app->subState) {
    case 0:
        app->subState++;
        break;
    case 1: {
        u32 status = HatchEggApp_UpdateEggShake(app, 1);
        if (status == 1) {
            PlaySE(SEQ_SE_DP_EGG01);
            ManagedSprite_SetAnimationFrame(app->eggSprite, 3);
            HatchEggParticles_Emit(app->particles, 0);
        }
        if (status == 2) {
            app->timer = 0;
            app->subState++;
        }
        break;
    }
    default:
        app->subState = 0;
        app->timer = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL HatchEggApp_ShakeEggCrack(HatchEggApp *app) {
    switch (app->subState) {
    case 0:
        if (HatchEggParticles_IsRunning(app->particles) != TRUE) {
            app->subState++;
        }
        break;
    case 1: {
        u32 status = HatchEggApp_UpdateEggShake(app, 2);
        if (status == 1) {
            PlaySE(SEQ_SE_DP_EGG01);
            ManagedSprite_SetAnimationFrame(app->eggSprite, 4);
            HatchEggParticles_Emit(app->particles, 1);
        }
        if (status == 2) {
            app->timer = 0;
            app->subState++;
        }
        break;
    }
    default:
        app->subState = 0;
        app->timer = 0;
        return FALSE;
    }
    return TRUE;
}

// The egg bursts, the screen flashes white and the bars slide off-screen to reveal the Pokemon.
BOOL HatchEggApp_HatchEgg(HatchEggApp *app) {
    struct {
        s16 y;
        s16 x;
    } pos;
    int done;
    u32 status;
    u32 plttOffset;
    switch (app->subState) {
    case 0:
        if (HatchEggParticles_IsRunning(app->particles) == TRUE) {
            break;
        }
        ManagedSprite_SetAnimationFrame(app->eggSprite, 4);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
        app->subState++;
        break;
    case 1:
        status = HatchEggApp_UpdateEggShake(app, 3);
        if (status == 1) {
            ManagedSprite_SetAnimationFrame(app->eggSprite, 5);
            PlaySE(SEQ_SE_DP_BOWA3);
            HatchEggParticles_Emit(app->particles, 2);
            HatchEggParticles_Emit(app->particles, 3);
        }
        if (status == 2) {
            ManagedSprite_SetDrawFlag(app->eggSprite, FALSE);
            app->timer = 0;
            app->subState++;
        }
        break;
    case 2:
        app->timer++;
        if (app->timer < 10) {
            break;
        }
        app->timer = 0;
        PaletteData_BeginPaletteFade(app->paletteData, 1, 0xffff, 0, 0, 16, 0xffff);
        plttOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(app->topBar->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
        PaletteData_BeginPaletteFade(app->paletteData, 4, (u16)(0xffff ^ (1 << plttOffset)), 0, 0, 16, 0xffff);
        Pokepic_StartPaletteFade(app->pokepic, 0, 16, 0, 0xffff);
        app->subState++;
        break;
    case 3:
        done = 0;
        ManagedSprite_GetPositionXY(app->topBar, &pos.x, &pos.y);
        if (pos.y > -48) {
            ManagedSprite_OffsetPositionXY(app->topBar, 0, -4);
        } else {
            done++;
        }
        ManagedSprite_GetPositionXY(app->bottomBar, &pos.x, &pos.y);
        if (pos.y < 192) {
            ManagedSprite_OffsetPositionXY(app->bottomBar, 0, 4);
        } else {
            done++;
        }
        if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0 || done != 2 || Pokepic_ResumePaletteFade(app->pokepic) != FALSE) {
            break;
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
        HatchEggApp_SetPokepicVanished(app, FALSE);
        ManagedSprite_SetDrawFlag(app->eggSprite, FALSE);
        ManagedSprite_SetDrawFlag(app->topBar, FALSE);
        ManagedSprite_SetDrawFlag(app->bottomBar, FALSE);
        app->subState++;
        break;
    case 4:
        app->timer++;
        if (app->timer < 25) {
            break;
        }
        app->timer = 0;
        Pokepic_StartPaletteFade(app->pokepic, 16, 0, 0, 0xffff);
        PaletteData_BeginPaletteFade(app->paletteData, 1, 0xffff, 0, 16, 0, 0xffff);
        PaletteData_BeginPaletteFade(app->paletteData, 4, 0xffff, 0, 16, 0, 0xffff);
        app->subState++;
        break;
    case 5:
        if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0 || Pokepic_ResumePaletteFade(app->pokepic) != FALSE) {
            break;
        }
        app->subState++;
        break;
    default:
        if (HatchEggParticles_IsRunning(app->particles) == TRUE) {
            break;
        }
        app->subState = 0;
        app->timer = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL HatchEggApp_HatchManaphyEgg(HatchEggApp *app) {
    struct {
        s16 y;
        s16 x;
    } pos;
    int done;
    u32 plttOffset;
    switch (app->subState) {
    case 0:
        HatchEggParticles_Emit(app->particles, 0);
        HatchEggParticles_Emit(app->particles, 1);
        HatchEggParticles_Emit(app->particles, 2);
        HatchEggParticles_Emit(app->particles, 3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
        app->subState++;
        break;
    case 1:
        switch ((u32)app->timer) {
        case 25:
            PlaySE(SEQ_SE_DP_W278);
            break;
        case 80:
            PlaySE(SEQ_SE_DP_W080C);
            break;
        case 142:
            PlaySE(SEQ_SE_DP_W361);
            break;
        }
        app->timer++;
        if (app->timer < 160) {
            break;
        }
        app->timer = 0;
        PaletteData_BeginPaletteFade(app->paletteData, 1, 0xffff, 0, 0, 16, 0xffff);
        plttOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(app->topBar->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
        PaletteData_BeginPaletteFade(app->paletteData, 4, (u16)(0xffff ^ (1 << plttOffset)), 0, 0, 16, 0xffff);
        Pokepic_StartPaletteFade(app->pokepic, 0, 16, 0, 0xffff);
        app->subState++;
        break;
    case 2:
        done = 0;
        ManagedSprite_GetPositionXY(app->topBar, &pos.x, &pos.y);
        if (pos.y > -48) {
            ManagedSprite_OffsetPositionXY(app->topBar, 0, -4);
        } else {
            done++;
        }
        ManagedSprite_GetPositionXY(app->bottomBar, &pos.x, &pos.y);
        if (pos.y < 192) {
            ManagedSprite_OffsetPositionXY(app->bottomBar, 0, 4);
        } else {
            done++;
        }
        if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0 || done != 2 || Pokepic_ResumePaletteFade(app->pokepic) != FALSE) {
            break;
        }
        HatchEggApp_SetPokepicVanished(app, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
        ManagedSprite_SetDrawFlag(app->eggSprite, FALSE);
        ManagedSprite_SetDrawFlag(app->topBar, FALSE);
        ManagedSprite_SetDrawFlag(app->bottomBar, FALSE);
        app->subState++;
        break;
    case 3:
        app->timer++;
        if (app->timer < 25) {
            break;
        }
        app->timer = 0;
        Pokepic_StartPaletteFade(app->pokepic, 16, 0, 0, 0xffff);
        PaletteData_BeginPaletteFade(app->paletteData, 1, 0xffff, 0, 16, 0, 0xffff);
        PaletteData_BeginPaletteFade(app->paletteData, 4, 0xffff, 0, 16, 0, 0xffff);
        app->subState++;
        break;
    case 4:
        if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0 || Pokepic_ResumePaletteFade(app->pokepic) != FALSE) {
            break;
        }
        app->subState++;
        break;
    default:
        app->subState = 0;
        app->timer = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL HatchEggApp_Init(OverlayManager *man, int *state) {
    HatchEggApp *app;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    Heap_Create(HEAP_ID_3, HEAP_ID_70, 0x40000);
    app = OverlayManager_CreateAndGetData(man, sizeof(HatchEggApp), HEAP_ID_70);
    memset(app, 0, sizeof(HatchEggApp));
    app->args = OverlayManager_GetArgs(man);
    app->textSpeed = Options_GetTextFrameDelay(app->args->hatch.options);
    app->frame = Options_GetFrame(app->args->hatch.options);
    app->vramMan = HatchEggApp_Create3DVramMan();
    app->pokepicManager = PokepicManager_Create(HEAP_ID_70);
    app->narc = NARC_New(NARC_a_1_8_0, HEAP_ID_70);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_70);
    GF_CreateVramTransferManager(64, HEAP_ID_70);
    app->picManager = PokepicAnimManager_New(HEAP_ID_70, 1, 0);
    PaletteData_SetAutoTransparent(app->paletteData = PaletteData_Init(HEAP_ID_70), TRUE);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_70);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_70);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_70);
    PaletteData_LoadNarc(app->paletteData, NARC_graphic_font, 9, HEAP_ID_70, PLTTBUF_SUB_BG, 0x20, 0xF0);
    HatchEggApp_HideLayers();
    HatchEggApp_InitBlend();
    HatchEggApp_InitParticleVram();
    HatchEggApp_InitBgs(app->bgConfig);
    HatchEggYesNoMenu_InitBgs(app->bgConfig);
    HatchEggApp_InitSpriteSystem((HatchEggAppFromBgConfig *)&app->bgConfig);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    app->yesNoMenu = HatchEggYesNoMenu_New(app->bgConfig, app->spriteSystem, app->spriteManager, HEAP_ID_70);
    Sound_Stop();
    Main_SetVBlankIntrCB(HatchEggApp_VBlankCB, app);
    return TRUE;
}

BOOL HatchEggApp_Main(OverlayManager *man, int *state) {
    HatchEggApp *app = OverlayManager_GetData(man);
    switch (*state) {
    case 0: {
        u32 species;
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        species = GetMonData(app->args->hatch.mon, MON_DATA_SPECIES, NULL);
        NARC_ReadPokepicCryDelay(app->narc, app->animParams, (u16)species, 1);
        if (species == SPECIES_MANAPHY) {
            app->sceneType = 1;
        } else {
            app->sceneType = 0;
        }
        break;
    }
    case 1:
        if (HatchEggApp_RunScene(app) == FALSE) {
            (*state)++;
        } else {
            PokepicManager_DrawAll(app->pokepicManager);
            HatchEggApp_Draw3D();
        }
        break;
    default:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL HatchEggApp_Exit(OverlayManager *man, int *state) {
    HatchEggApp *app = OverlayManager_GetData(man);
    HatchEggYesNoMenu_Free(app->yesNoMenu);
    GF_3DVramMan_Delete(app->vramMan);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 0);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_SUB_BG);
    PaletteData_Free(app->paletteData);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
    Heap_Free(app->bgConfig);
    GF_DestroyVramTransferManager();
    PokepicManager_Delete(app->pokepicManager);
    PokepicAnimManager_Delete(app->picManager);
    NARC_Delete(app->narc);
    HatchEggApp_FreeSpriteSystem((HatchEggAppFromBgConfig *)&app->bgConfig);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_70);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    Field_ForceEnvironmentSoundRecheck();
    return TRUE;
}

void HatchEggYesNoMenu_InitBgs(BgConfig *bgConfig) {
    HatchEggBgTemplates templates = gHatchEggYesNoBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &templates.templates[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &templates.templates[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &templates.templates[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
}

#pragma force_active on
HatchEggYesNoMenu *HatchEggYesNoMenu_New(BgConfig *bgConfig, SpriteSystem *spriteSystem, SpriteManager *spriteManager, enum HeapID heapId) {
    HatchEggYesNoMenu *menu;
    if (!bgConfig) {
        GF_AssertFail();
    }
    if (!spriteSystem) {
        GF_AssertFail();
    }
    if (!spriteManager) {
        GF_AssertFail();
    }
    menu = Heap_Alloc(heapId, sizeof(HatchEggYesNoMenu));
    MI_CpuFill8(menu, 0, sizeof(HatchEggYesNoMenu));
    menu->heapId = heapId;
    menu->bgConfig = bgConfig;
    menu->spriteSystem = spriteSystem;
    menu->spriteManager = spriteManager;
    menu->msgFormat = MessageFormat_New(heapId);
    FontID_Alloc(4, menu->heapId);
    HatchEggYesNoMenu_AddWindows(menu);
    return menu;
}
#pragma force_active reset

void HatchEggYesNoMenu_Free(HatchEggYesNoMenu *menu) {
    if (!menu) {
        GF_AssertFail();
    }
    HatchEggYesNoMenu_RemoveWindows(menu);
    HatchEggYesNoMenu_DeleteCursor(menu);
    FontID_Release(4);
    MessageFormat_Delete(menu->msgFormat);
    FreeBgTilemapBuffer(menu->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(menu->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(menu->bgConfig, GF_BG_LYR_SUB_2);
    Heap_Free(menu);
}

#pragma force_active on
void HatchEggYesNoMenu_LoadGfx(HatchEggYesNoMenu *menu, PaletteData *paletteData, int charResId, int plttResId, int cellResId, int animResId) {
    SpriteSystem *spriteSystem = menu->spriteSystem;
    SpriteManager *spriteManager = menu->spriteManager;
    NARC *narc;
    GF_ASSERT(menu);
    GF_ASSERT(spriteSystem);
    GF_ASSERT(spriteManager);
    narc = NARC_New(NARC_a_2_3_7, menu->heapId);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, narc, 11, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, plttResId);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 12, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, charResId);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 13, FALSE, cellResId);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 14, FALSE, animResId);
    HatchEggYesNoMenu_CreateCursor(menu, charResId, plttResId, cellResId, animResId);
    PaletteData_LoadNarc(paletteData, NARC_a_2_3_7, 0, HEAP_ID_70, PLTTBUF_SUB_BG, 0xA0, 0);
    PaletteData_LoadNarc(paletteData, NARC_graphic_font, 9, HEAP_ID_70, PLTTBUF_SUB_BG, 0x20, 0xF0);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, menu->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_70);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, menu->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_70);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, menu->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_70);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, menu->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_70);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    NARC_Delete(narc);
}
#pragma force_active reset

void HatchEggYesNoMenu_Show(HatchEggYesNoMenu *menu, u32 yesMsgId, u32 noMsgId) {
    HatchEggYesNoMenu_PrintButton(menu, &menu->buttons[0], yesMsgId);
    HatchEggYesNoMenu_PrintButton(menu, &menu->buttons[1], noMsgId);
    menu->state = 0;
    menu->selection = 1;
    HatchEggYesNoMenu_UpdateCursor(menu);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
    ManagedSprite_SetDrawFlag(menu->cursor, TRUE);
}

// Returns the selection (1 = yes, 2 = no) once a button was pressed and its animation has ended, else 0.
#pragma force_active on
int HatchEggYesNoMenu_Run(HatchEggYesNoMenu *menu) {
    switch (menu->state) {
    case 0:
        if (HatchEggYesNoMenu_HandleInput(menu)) {
            menu->state++;
        }
        break;
    case 1:
        if (HatchEggYesNoMenu_IsPressAnimFinished(menu)) {
            GF_ASSERT(menu->selection);
            ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
            ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);
            ManagedSprite_SetDrawFlag(menu->cursor, FALSE);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 0);
            return menu->selection;
        }
        break;
    default:
        GF_ASSERT(0);
        break;
    }
    return 0;
}
#pragma force_active reset

void HatchEggYesNoMenu_AddWindows(HatchEggYesNoMenu *menu) {
    AddWindowParameterized(menu->bgConfig, &menu->buttons[0], GF_BG_LYR_SUB_2, 4, 8, 24, 2, 15, 0x50);
    AddWindowParameterized(menu->bgConfig, &menu->buttons[1], GF_BG_LYR_SUB_2, 4, 14, 24, 2, 15, 0x80);
}

void HatchEggYesNoMenu_RemoveWindows(HatchEggYesNoMenu *menu) {
    int i;
    Window *window;
    GF_ASSERT(menu != NULL);
    i = 0;
    window = menu->buttons;
    for (; i < 2; i++) {
        RemoveWindow(window);
        window++;
    }
}

void HatchEggYesNoMenu_PrintButton(HatchEggYesNoMenu *menu, Window *window, u32 msgId) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0187_bin, menu->heapId);
    String *string = ReadMsgData_ExpandPlaceholders(menu->msgFormat, msgData, msgId, menu->heapId);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 4, string, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    CopyWindowToVram(window);
    DestroyMsgData(msgData);
    String_Delete(string);
}

void HatchEggYesNoMenu_CreateCursor(HatchEggYesNoMenu *menu, int charResId, int plttResId, int cellResId, int animResId) {
    ManagedSpriteTemplate template = gHatchEggYesNoCursorTemplate;
    ManagedSprite *sprite;
    SpriteSystem *spriteSystem = menu->spriteSystem;
    SpriteManager *spriteManager = menu->spriteManager;
    GF_ASSERT(menu != NULL);
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template.x = 128;
    template.y = 72;
    template.animation = 1;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = charResId;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = plttResId;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = cellResId;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = animResId;
    sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    menu->cursor = sprite;
}

void HatchEggYesNoMenu_DeleteCursor(HatchEggYesNoMenu *menu) {
    Sprite_DeleteAndFreeResources(menu->cursor);
}

#pragma force_active on
void HatchEggYesNoMenu_UpdateCursor(HatchEggYesNoMenu *menu) {
    int index;
    if (!menu->cursor) {
        GF_AssertFail();
    }
    switch (menu->selection) {
    case 1:
        index = 0;
        break;
    case 2:
        index = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ManagedSprite_SetPositionXY(menu->cursor, gHatchEggYesNoCursorPositions[index][0], gHatchEggYesNoCursorPositions[index][1]);
}

BOOL HatchEggYesNoMenu_HandleInput(HatchEggYesNoMenu *menu) {
    BOOL pressed = FALSE;
    int hit = TouchscreenHitbox_FindRectAtTouchNew(gHatchEggYesNoHitboxes);
    if (hit != -1) {
        PlaySE(SEQ_SE_DP_SELECT);
        switch (hit) {
        case 0:
            menu->selection = 1;
            pressed = TRUE;
            break;
        case 1:
            menu->selection = 2;
            pressed = TRUE;
            break;
        default:
            GF_AssertFail();
            break;
        }
    } else {
        u32 keys = gSystem.newKeys;
        if (keys & PAD_KEY_UP) {
            if (menu->selection != 1) {
                menu->selection = 1;
                HatchEggYesNoMenu_UpdateCursor(menu);
                PlaySE(SEQ_SE_DP_SELECT);
            }
        } else if (keys & PAD_KEY_DOWN) {
            if (menu->selection != 2) {
                menu->selection = 2;
                HatchEggYesNoMenu_UpdateCursor(menu);
                PlaySE(SEQ_SE_DP_SELECT);
            }
        } else if (keys & PAD_BUTTON_A) {
            if (menu->selection == 0) {
                GF_AssertFail();
                menu->selection = 2;
            }
            pressed = TRUE;
        } else if (keys & PAD_BUTTON_B) {
            menu->selection = 2;
            pressed = TRUE;
        }
    }
    if (pressed) {
        HatchEggYesNoMenu_UpdateCursor(menu);
        ManagedSprite_SetAnim(menu->cursor, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return pressed;
}
#pragma force_active reset

BOOL HatchEggYesNoMenu_IsPressAnimFinished(HatchEggYesNoMenu *menu) {
    if (ManagedSprite_GetActiveAnim(menu->cursor) != 3) {
        GF_AssertFail();
    }
    return ManagedSprite_IsAnimated(menu->cursor) == FALSE;
}
