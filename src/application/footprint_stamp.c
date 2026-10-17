#include "application/footprint_stamp.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0754.h"

#include "filesystem.h"
#include "font.h"
#include "geometry_math.h"
#include "gf_3d_render.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "math_util.h"
#include "options.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sprite_resource_header.h"
#include "swap_3d_buffers.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"

void *WiFiPlazaWork_GetSaveData(void *lobby);
void *WiFiPlazaWork_IsArceusFootprintShown(void *lobby);
void WiFiPlazaWork_SetLowVolume(void *lobby, int value);
void WiFiPlazaWork_SetAppQuitForClosing(void *lobby);
u32 WiFiPlazaWork_IsClosing(void *lobby);
BOOL WiFiPlaza_HasConnectionFailed(void);
void WiFiPlazaLobbyClient_ClearChannelCommands(void);
void HBlankInterruptDisable(void);
int GetUserFrameGfx2PaletteMember(int frame);
void ModelResource_LoadFromNarc(FootprintStampModel *model, NARC *narc, int fileId, enum HeapID heapId);
void ModelResource_Free(FootprintStampModel *model);
void ModelInstance_Init(FootprintStampModelInstance *instance, FootprintStampModel *model);
void ModelInstance_Draw(FootprintStampModelInstance *instance);
void ModelInstance_SetVisible(FootprintStampModelInstance *instance, BOOL visible);
void ModelInstance_SetPosition(FootprintStampModelInstance *instance, fx32 x, fx32 y, fx32 z);
void ModelInstance_SetScale(FootprintStampModelInstance *instance, fx32 x, fx32 y, fx32 z);
u32 WiFiPlazaLobbyClient_GetOwnId(void *unused);
int WiFiPlazaLobbyClient_IdToChannelSlot(u32 playerId);
void SetMasterBrightness(PMLCDTarget screen, int brightness);

extern const FootprintStampEffectFuncs sFootprintStampEffectFuncs[];
extern BOOL (*const sFootprintStampCameraEffectFuncs[])(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);

typedef struct FootprintStampBgTemplates {
    BgTemplate items[3];
} FootprintStampBgTemplates;

// The lobby's player id list (WiFiPlazaLobbyClient_GetChannelPlayerList).
typedef struct FootprintStampPlayerList {
    u32 unk00;
    u32 *ids; // FOOTPRINT_STAMP_PLAYERS_MAX, -1 = empty
} FootprintStampPlayerList;

void WiFiPlazaLobbyClient_GetChannelPlayerList(FootprintStampPlayerList *list);

// Player list rows on the sub screen (tiles).
typedef struct FootprintStampRow {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} FootprintStampRow;

enum {
    FOOTPRINT_STAMP_STATE_INIT,
    FOOTPRINT_STAMP_STATE_WAIT_FADE_IN,
    FOOTPRINT_STAMP_STATE_STAMP,
    FOOTPRINT_STAMP_STATE_PRINT_QUIT_PROMPT,
    FOOTPRINT_STAMP_STATE_SHOW_YES_NO,
    FOOTPRINT_STAMP_STATE_QUIT_YES_NO,
    FOOTPRINT_STAMP_STATE_PRINT_CLOSED_MSG,
    FOOTPRINT_STAMP_STATE_WAIT_CLOSED_MSG,
    FOOTPRINT_STAMP_STATE_CLOSED_DELAY,
    FOOTPRINT_STAMP_STATE_FADE_OUT,
    FOOTPRINT_STAMP_STATE_WAIT_FADE_OUT,
};

static void FootprintStamp_MainTask(SysTask *task, void *data);
static void FootprintStamp_VBlank(void *data);
static void FootprintStamp_InitBgs(BgConfig *bgConfig);
static void FootprintStamp_FreeBgs(BgConfig *bgConfig);
static void FootprintStamp_LoadBgGfx(FootprintStamp *work, NARC *narc);
static void FootprintStamp_LoadPartySpriteGfx(FootprintStamp *work, NARC *narc);
static void FootprintStamp_CreatePartySprites(FootprintStamp *work);
static void FootprintStamp_FreePartySprites(FootprintStamp *work);
static void FootprintStamp_LoadPlayerSpriteGfx(FootprintStamp *work, NARC *narc);
static BOOL FootprintStamp_LoadFootprintIcon(int species, int form, ManagedSprite *sprite, NARC *narc, NARC *footprintNarc, int screen, void *unk19E0);
static void FootprintStamp_Setup3D(void);
static void FootprintStamp_HighlightPartySlot(FootprintStamp *work, int slot);
static void FootprintStamp_GetTextSize(String *string, int font, int *width, int *tiles);
static inline int FootprintStamp_InkGaugeY(int y);
static BOOL FootprintStampBoard_AddStamp(FootprintStamp *work, FootprintStampBoard *board, const FootprintStampMon *mon, NARC *narc, NARC *footprintNarc, void *unk19E0);
static void FootprintStampBoard_Draw(FootprintStampBoard *board);
static FootprintStampObj *FootprintStampBoard_CreateStamp(FootprintStamp *work, FootprintStampBoard *board, const FootprintStampMon *mon, NARC *narc, NARC *footprintNarc, void *unk19E0);
static BOOL FootprintStamp_LoadStampModel(FootprintStampBoard *board, FootprintStampModel *model, NARC *narc, NARC *footprintNarc, FootprintStampMon *mon, void *unk19E0);
static BOOL FootprintStamp_AllocTextureVram(NNSG3dResTex *texture);

static const s16 sFootprintStampPlayerFootprintPositions[FOOTPRINT_STAMP_PLAYERS_MAX][2] = {
    { 32,  48  },
    { 152, 48  },
    { 32,  88  },
    { 152, 88  },
    { 32,  128 },
    { 152, 128 },
    { 32,  168 },
    { 152, 168 }
};

static const OamCharTransferParam sFootprintStampOamCharTransfer = {
    .maxTasks = 96,
    .sizeMain = 0x10000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_128K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const SpriteResourceCountsListUnion sFootprintStampSpriteResourceCounts = {
    .numChar = 96,
    .numPltt = 32,
    .numCell = 64,
    .numAnim = 64,
    .numMcel = 8,
    .numManm = 8,
};

static const GraphicsBanks sFootprintStampBanks = {
    .bg = GX_VRAM_BG_128_C,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_32_H,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_01_AB,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

static const GraphicsModes sFootprintStampGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const FootprintStampBgTemplates sFootprintStampMainBgTemplates = {
    {
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x08000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0800,
            .charBase = GX_BG_CHARBASE_0x10000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x1000,
            .charBase = GX_BG_CHARBASE_0x10000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     },
};

static const FootprintStampBgTemplates sFootprintStampSubBgTemplates = {
    {
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x7000,
            .charBase = GX_BG_CHARBASE_0x00000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 2,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x7800,
            .charBase = GX_BG_CHARBASE_0x00000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x6800,
            .charBase = GX_BG_CHARBASE_0x00000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     },
};

static const ManagedSpriteTemplate sFootprintStampMyCursorTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 50,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2719, 0x2714, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 1,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sFootprintStampPlayerFootprintTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 51,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x271A, 0x2714, 0x2715, 0x2715, -1, -1 },
    .bgPriority = 1,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sFootprintStampInkSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 53,
    .pal = 1,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = { 0x2717, 0x2711, 0x2712, 0x2712, -1, -1 },
    .bgPriority = 1,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sFootprintStampSlotSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 54,
    .pal = 2,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = { 0x2717, 0x2711, 0x2712, 0x2712, -1, -1 },
    .bgPriority = 1,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sFootprintStampFootprintSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 52,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = { 0x2711, 0x2712, 0x2711, 0x2711, -1, -1 },
    .bgPriority = 1,
    .vramTransfer = 0,
};

static const OamManagerParam sFootprintStampOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const CameraAngle sFootprintStampCameraAngle = { 0, 0, 0, 0 };

static const u16 sFootprintStampEmptySlotTiles[4] = { 21, 53, 53, 85 };

static const VecFx32 sFootprintStampCameraTarget = { 0, -0x8000, 0 };

static const u8 sFootprintStampPlayerNameWindowPositions[FOOTPRINT_STAMP_PLAYERS_MAX][2] = {
    { 7,  5  },
    { 22, 5  },
    { 7,  10 },
    { 22, 10 },
    { 7,  15 },
    { 22, 15 },
    { 7,  20 },
    { 22, 20 }
};

static const s16 sFootprintStampMyCursorPositions[FOOTPRINT_STAMP_PLAYERS_MAX][2] = {
    { 72,  48  },
    { 192, 48  },
    { 72,  88  },
    { 192, 88  },
    { 72,  128 },
    { 192, 128 },
    { 72,  168 },
    { 192, 168 }
};

static const ManagedSpriteTemplate sFootprintStampSelectEffectTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 50,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = { 0x2718, 0x2711, 0x2713, 0x2713, -1, -1 },
    .bgPriority = 0,
    .vramTransfer = 0,
};

static const FootprintStampRow sFootprintStampPlayerRows[FOOTPRINT_STAMP_PLAYERS_MAX] = {
    { 2,  4,  14, 4 },
    { 17, 4,  14, 4 },
    { 2,  9,  14, 4 },
    { 17, 9,  14, 4 },
    { 2,  14, 14, 4 },
    { 17, 14, 14, 4 },
    { 2,  19, 14, 4 },
    { 17, 19, 14, 4 },
};

BOOL FootprintStamp_Init(OverlayManager *man, int *state) {
    FootprintStamp *work;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    Heap_Create(HEAP_ID_3, HEAP_ID_125, 0x50000);
    work = OverlayManager_CreateAndGetData(man, sizeof(FootprintStamp), HEAP_ID_125);
    MI_CpuFill8(work, 0, sizeof(FootprintStamp));
    work->args = OverlayManager_GetArgs(man);
    work->saveData = WiFiPlazaWork_GetSaveData(work->args->lobby);
    work->showArceusFootprint = WiFiPlazaWork_IsArceusFootprintShown(work->args->lobby);
    FootprintStamp_InitLinkFilter(work);
    FootprintStamp_LoadParty(work->args->boardType, work->saveData, work->party);
    work->ink = FOOTPRINT_STAMP_INK_MAX;
    work->inkShown = work->ink;
    work->vramMan3d = FootprintStamp_Create3DVramMan(HEAP_ID_125);
    work->plttData = PaletteData_Init(HEAP_ID_125);
    PaletteData_SetAutoTransparent(work->plttData, TRUE);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_125);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_125);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_MAIN_OBJ, 0x1C0, HEAP_ID_125);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_125);
    PaletteData_SetAutoTransparent(work->plttData, TRUE);
    work->bgConfig = BgConfig_Alloc(HEAP_ID_125);
    GF_CreateVramTransferManager(0x40, HEAP_ID_125);
    SetKeyRepeatTimers(4, 8);
    FootprintStamp_InitBgs(work->bgConfig);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    FontID_Alloc(2, HEAP_ID_125);
    work->msgFormat = MessageFormat_New(HEAP_ID_125);
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0754_bin, HEAP_ID_125);
    work->fontSystem = FontSystem_NewInit(4, HEAP_ID_125);
    work->narc = NARC_New(NARC_a_2_0_8, HEAP_ID_125);
    work->footprintNarc = NARC_New(NARC_poketool_pokefoot_pokefoot, HEAP_ID_125);
    FootprintStamp_LoadBgGfx(work, work->narc);
    FootprintStamp_LoadBoardModel(work, work->narc);
    FootprintStamp_CreateCamera(work);
    FootprintStamp_CreateWindows(work);
    work->string = String_New(0x100, HEAP_ID_125);
    work->spriteSystem = SpriteSystem_Alloc(HEAP_ID_125);
    SpriteSystem_Init(work->spriteSystem, &sFootprintStampOamManagerParam, &sFootprintStampOamCharTransfer, 0x20);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    work->spriteManager = SpriteManager_New(work->spriteSystem);
    SpriteSystem_InitSprites(work->spriteSystem, work->spriteManager, 0x80);
    SpriteSystem_InitManagerWithCapacities(work->spriteSystem, work->spriteManager, (SpriteResourceCountsListUnion *)&sFootprintStampSpriteResourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, 0x110000);
    WirelessStatusIcon_Create();
    FootprintStampBoard_Init(&work->board, work->showArceusFootprint);
    FootprintStamp_LoadPartySpriteGfx(work, work->narc);
    FootprintStamp_LoadPlayerSpriteGfx(work, work->narc);
    FootprintStamp_CreatePartySprites(work);
    FootprintStamp_CreatePlayerSprites(work);
    FootprintStamp_ShowParty(work);
    work->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_125);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 1, HEAP_ID_125);
    if (work->args->lobby != NULL) {
        WiFiPlazaWork_SetLowVolume(work->args->lobby, 1);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    work->task = SysTask_CreateOnMainQueue(FootprintStamp_MainTask, work, 60000);
    Main_SetVBlankIntrCB(FootprintStamp_VBlank, work);
    return TRUE;
}

BOOL FootprintStamp_Main(OverlayManager *man, int *state) {
    FootprintStamp *work = OverlayManager_GetData(man);
    FootprintStampMon dummy;
    YesNoPromptTemplate template;

    FootprintStampBoard_Update(&work->board, work->camera, work->phase, work->args->boardType);
    switch (*state) {
    case FOOTPRINT_STAMP_STATE_INIT:
        work->linkFilter.active = TRUE;
        FootprintStamp_RegisterLinkCommands(work);
        FootprintStampMon_InitDummy(work, &dummy);
        (*state)++;
        break;
    case FOOTPRINT_STAMP_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished() == TRUE) {
            work->phase = 1;
            (*state)++;
        }
        break;
    case FOOTPRINT_STAMP_STATE_STAMP: {
        int slot;
        if (gSystem.touchNew && gSystem.touchY < 0xA0 && FootprintStamp_UseInk(work, FOOTPRINT_STAMP_INK_PER_STAMP) == TRUE) {
            work->party[work->selectedSlot].x = gSystem.touchX;
            work->party[work->selectedSlot].y = gSystem.touchY;
            FootprintStamp_PlaceStamp(work, &work->party[work->selectedSlot], work->linkFilter.myId);
            FootprintStamp_SendStamp(&work->party[work->selectedSlot]);
        }
        slot = (int)FootprintStamp_GetTouchedPartySlot(work->party, work->selectedSlot);
        if (slot < FOOTPRINT_STAMP_PARTY_SLOTS) {
            work->selectedSlot = slot;
            FootprintStamp_StartSelectEffect(work, slot);
        } else if (*state == FOOTPRINT_STAMP_STATE_STAMP && slot == FOOTPRINT_STAMP_TOUCH_EXIT) {
            PlaySE(SEQ_SE_DP_BUTTON3);
            PaletteData_BlendPalette(work->plttData, PLTTBUF_MAIN_BG, 9, 1, 8, RGB_BLACK);
            PaletteData_BlendPalette(work->plttData, PLTTBUF_MAIN_OBJ, (u16)(work->cursorPlttSlot << 4), 16, 8, RGB_BLACK);
            *state = FOOTPRINT_STAMP_STATE_PRINT_QUIT_PROMPT;
        }
        break;
    }
    case FOOTPRINT_STAMP_STATE_PRINT_QUIT_PROMPT:
        FillWindowPixelBuffer(&work->msgWindow, 15);
        DrawFrameAndWindow2(&work->msgWindow, FALSE, 1, 14);
        ReadMsgDataIntoString(work->msgData, msg_0754_00002, work->string);
        work->printerId = AddTextPrinterParameterized(&work->msgWindow, 1, work->string, 0, 0, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(work->saveData)), NULL);
        (*state)++;
        break;
    case FOOTPRINT_STAMP_STATE_SHOW_YES_NO:
        if (!TextPrinterCheckActive(work->printerId)) {
            template.bgConfig = work->bgConfig;
            template.bgId = GF_BG_LYR_MAIN_1;
            template.tileStart = 31;
            template.plttSlot = 11;
            template.x = 25;
            template.y = 6;
            template.shapeParam = 0;
            YesNoPrompt_InitFromTemplate(work->yesNoPrompt, &template);
            work->yesNoActive = TRUE;
            (*state)++;
        }
        break;
    case FOOTPRINT_STAMP_STATE_QUIT_YES_NO:
        PaletteData_LoadPaletteSlotFromHardware(work->plttData, PLTTBUF_MAIN_BG, 0xB0, 0x40);
        switch (YesNoPrompt_HandleInput(work->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(work->yesNoPrompt);
            work->yesNoActive = FALSE;
            ClearFrameAndWindow2(&work->msgWindow, FALSE);
            work->linkFilter.active = FALSE;
            work->phase = 3;
            *state = FOOTPRINT_STAMP_STATE_FADE_OUT;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(work->yesNoPrompt);
            work->yesNoActive = FALSE;
            ClearFrameAndWindow2(&work->msgWindow, FALSE);
            PaletteData_BlendPalette(work->plttData, PLTTBUF_MAIN_BG, 9, 1, 0, RGB_BLACK);
            PaletteData_BlendPalette(work->plttData, PLTTBUF_MAIN_OBJ, (u16)(work->cursorPlttSlot << 4), 16, 0, RGB_BLACK);
            *state = FOOTPRINT_STAMP_STATE_STAMP;
            break;
        }
        break;
    case FOOTPRINT_STAMP_STATE_PRINT_CLOSED_MSG:
        FillWindowPixelBuffer(&work->msgWindow, 15);
        DrawFrameAndWindow2(&work->msgWindow, FALSE, 1, 14);
        ReadMsgDataIntoString(work->msgData, msg_0754_00003, work->string);
        work->printerId = AddTextPrinterParameterized(&work->msgWindow, 1, work->string, 0, 0, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(work->saveData)), NULL);
        (*state)++;
        break;
    case FOOTPRINT_STAMP_STATE_WAIT_CLOSED_MSG:
        if (!TextPrinterCheckActive(work->printerId)) {
            (*state)++;
        }
        break;
    case FOOTPRINT_STAMP_STATE_CLOSED_DELAY:
        work->closeTimer++;
        if (work->closeTimer > 90) {
            *state = FOOTPRINT_STAMP_STATE_FADE_OUT;
        }
        break;
    case FOOTPRINT_STAMP_STATE_FADE_OUT:
        if (!IsPaletteFadeFinished()) {
            StopPaletteFade();
        }
        BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_125);
        (*state)++;
        break;
    case FOOTPRINT_STAMP_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    default:
        WiFiPlazaLobbyClient_ClearChannelCommands();
        return TRUE;
    }
    FootprintStamp_UpdateSelectEffects(work);
    FootprintStamp_UpdateInkGauge(work);
    if (work->phase == 1) {
        work->nameTimer = (s32)((u32)work->nameTimer + 1);
        if (work->nameTimer > 30) {
            work->nameTimer = 0;
            FootprintStamp_UpdatePlayers(work);
        }
        // the lobby closes the attraction, or this player has to leave
        if ((WiFiPlaza_HasConnectionFailed() == TRUE || WiFiPlazaWork_IsClosing(work->args->lobby) == TRUE) && *state != FOOTPRINT_STAMP_STATE_SHOW_YES_NO) {
            if (work->yesNoActive == TRUE) {
                YesNoPrompt_Reset(work->yesNoPrompt);
            }
            work->linkFilter.active = FALSE;
            work->phase = 3;
            if (WiFiPlaza_HasConnectionFailed() == TRUE) {
                *state = FOOTPRINT_STAMP_STATE_FADE_OUT;
            } else {
                PlaySE(SEQ_SE_DP_PINPON);
                WiFiPlazaWork_SetAppQuitForClosing(work->args->lobby);
                *state = FOOTPRINT_STAMP_STATE_PRINT_CLOSED_MSG;
            }
        }
    }
    FootprintStamp_UpdateDummy(work);
    return FALSE;
}

BOOL FootprintStamp_Exit(OverlayManager *man, int *state) {
    FootprintStamp *work = OverlayManager_GetData(man);

    SysTask_Destroy(work->task);
    FootprintStamp_FreePartySprites(work);
    FootprintStamp_FreePlayerSprites(work);
    YesNoPrompt_Destroy(work->yesNoPrompt);
    FootprintStampBoard_Free(&work->board);
    String_Delete(work->string);
    FontID_Release(2);
    FontSystem_Delete(work->fontSystem);
    DestroyMsgData(work->msgData);
    MessageFormat_Delete(work->msgFormat);
    FootprintStamp_FreeWindows(work);
    FootprintStamp_FreeBgs(work->bgConfig);
    Heap_Free(work->bgConfig);
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(work->plttData);
    FootprintStamp_FreeBoardModel(work);
    FootprintStamp_FreeCamera(work);
    FootprintStamp_Free3DVramMan(work->vramMan3d);
    NARC_Delete(work->narc);
    NARC_Delete(work->footprintNarc);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GF_DestroyVramTransferManager();
    GF_TouchpadStopSampling();
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    WirelessStatusIcon_Destroy();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_125);
    return TRUE;
}

static void FootprintStamp_MainTask(SysTask *task, void *data) {
    FootprintStamp *work = data;

    FootprintStampBoard_ResolveChains(&work->board);
    FootprintStamp_Draw3D(work);
    SpriteSystem_DrawSprites(work->spriteManager);
    SpriteSystem_UpdateTransfer();
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

static void FootprintStamp_VBlank(void *data) {
    FootprintStamp *work = data;
    vu32 *base;

    FootprintStampBoard_VBlank(&work->board, work->phase);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(work->plttData);
    DoScheduledBgGpuUpdates(work->bgConfig);
    base = (vu32 *)0x027E0000;
    base[0x3FF8 / 4] |= 1; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

static void FootprintStamp_InitBgs(BgConfig *bgConfig) {
    FootprintStampBgTemplates mainTemplates;
    FootprintStampBgTemplates subTemplates;
    GraphicsBanks banks;
    GraphicsModes modes;

    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    banks = sFootprintStampBanks;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (u32 *)0x06000000, 0x80000);
    MIi_CpuClear32(0, (u32 *)0x06200000, 0x20000);
    MIi_CpuClear32(0, (u32 *)0x06400000, 0x40000);
    MIi_CpuClear32(0, (u32 *)0x06600000, 0x20000);
    modes = sFootprintStampGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    mainTemplates = sFootprintStampMainBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainTemplates.items[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainTemplates.items[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainTemplates.items[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    reg_G2_BG0CNT = (u16)((reg_G2_BG0CNT & ~3) | 2); // 3D layer priority 2
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    subTemplates = sFootprintStampSubBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subTemplates.items[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subTemplates.items[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &subTemplates.items[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_125);
}

static void FootprintStamp_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
}

static void FootprintStamp_LoadBgGfx(FootprintStamp *work, NARC *narc) {
    void *tilemap;
    int frame;

    PaletteData_LoadNarc(work->plttData, NARC_a_2_0_8, 19, HEAP_ID_125, PLTTBUF_MAIN_BG, 0x1C0, 0);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 18, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_125);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 17, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_125);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 20, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_125);
    PaletteData_LoadNarc(work->plttData, NARC_a_2_0_8, 23, HEAP_ID_125, PLTTBUF_SUB_BG, 0, 0);
    if (work->args->boardType == 0) {
        PaletteData_CopyPalette(work->plttData, PLTTBUF_SUB_BG, 0x10, PLTTBUF_SUB_BG, 0, 0x20);
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 22, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_125);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 21, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_125);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 24, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_125);
    // the player list is drawn into BG 6 one row at a time as players join
    tilemap = GetBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_2);
    MI_CpuCopy16(tilemap, work->subTilemap, sizeof(work->subTilemap));
    MIi_CpuClear16(0, tilemap, sizeof(work->subTilemap));
    frame = Options_GetFrame(Save_PlayerData_GetOptionsAddr(work->saveData));
    PaletteData_LoadNarc(work->plttData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_125, PLTTBUF_MAIN_BG, 0x20, 0xE0);
    LoadUserFrameGfx2(work->bgConfig, GF_BG_LYR_MAIN_1, 1, 14, frame, HEAP_ID_125);
    PaletteData_LoadNarc(work->plttData, NARC_graphic_font, 7, HEAP_ID_125, PLTTBUF_MAIN_BG, 0x20, 0xD0);
    if (work->args->boardType == 0) {
        PaletteData_LoadNarc(work->plttData, NARC_graphic_font, 7, HEAP_ID_125, PLTTBUF_SUB_BG, 0x20, 0xD0);
    } else {
        PaletteData_LoadNarc(work->plttData, NARC_a_2_0_8, 25, HEAP_ID_125, PLTTBUF_SUB_BG, 0x20, 0xD0);
    }
    ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_SUB_2);
}

static void FootprintStamp_LoadPartySpriteGfx(FootprintStamp *work, NARC *narc) {
    int i;
    u16 color;
    u8 plttNum;

    plttNum = SpriteSystem_LoadPaletteBufferFromOpenNarc(work->plttData, PLTTBUF_MAIN_OBJ, work->spriteSystem, work->spriteManager, narc, 3, FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 10001);
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 4, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 10007);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 5, FALSE, 10002);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 6, FALSE, 10002);
    for (i = 0; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 16, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 10001 + i);
    }
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 15, FALSE, 10001);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 14, FALSE, 10001);
    plttNum = SpriteSystem_LoadPaletteBufferFromOpenNarc(work->plttData, PLTTBUF_MAIN_OBJ, work->spriteSystem, work->spriteManager, narc, 3, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 10002);
    if (work->args->boardType == 0) {
        color = RGB_BLACK;
    } else {
        color = RGB_WHITE;
    }
    PaletteData_FillPaletteInBuffer(work->plttData, PLTTBUF_MAIN_OBJ, PLTTSEL_BOTH, color, (u16)(plttNum << 4), (u16)((plttNum << 4) + 16));
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 2, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 10008);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 1, FALSE, 10003);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 0, FALSE, 10003);
    work->cursorPlttSlot = SpriteSystem_LoadPaletteBuffer(work->plttData, PLTTBUF_MAIN_OBJ, work->spriteSystem, work->spriteManager, NARC_a_2_0_8, 26, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 10003);
}

static void FootprintStamp_CreatePartySprites(FootprintStamp *work) {
    ManagedSpriteTemplate template;
    int i;
    String *string;

    template = sFootprintStampInkSpriteTemplate;
    for (i = 0; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++) {
        template.x = i * 32 + 16;
        template.y = 176;
        work->inkSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_SetAnim(work->inkSprites[i], i);
        Sprite_TickFrame(work->inkSprites[i]->sprite);
    }
    template = sFootprintStampSlotSpriteTemplate;
    for (i = 0; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++) {
        template.x = i * 32 + 16;
        template.y = 176;
        work->slotSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_SetAnim(work->slotSprites[i], i);
        Sprite_TickFrame(work->slotSprites[i]->sprite);
    }
    template = sFootprintStampFootprintSpriteTemplate;
    for (i = 0; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++) {
        template.x = i * 32 + 16;
        template.y = 176;
        template.resIdList[0] = 10001 + i;
        work->footprintSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        Sprite_TickFrame(work->footprintSprites[i]->sprite);
    }
    string = NewString_ReadMsgData(work->msgData, msg_0754_00001);
    FootprintStamp_CreateTextSprite(work, &work->exitText, string, 2, MAKE_TEXT_COLOR(1, 2, 3), 0, 10003, 224, 176, TRUE);
    TextOBJ_SetSpritesDrawFlag(work->exitText.textObj, TRUE);
    String_Delete(string);
}

static void FootprintStamp_FreePartySprites(FootprintStamp *work) {
    {
        int i = 0;
        FootprintStamp *slot = work;
        for (; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++, slot = (FootprintStamp *)((u8 *)slot + 4)) {
            Sprite_DeleteAndFreeResources(slot->inkSprites[0]);
        }
    }
    {
        FootprintStamp *slot;
        int i;
        i = 0;
        slot = work;
        for (; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++, slot = (FootprintStamp *)((u8 *)slot + 4)) {
            Sprite_DeleteAndFreeResources(slot->slotSprites[0]);
        }
    }
    {
        FootprintStamp *slot;
        int i;
        i = 0;
        slot = work;
        for (; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++, slot = (FootprintStamp *)((u8 *)slot + 4)) {
            Sprite_DeleteAndFreeResources(slot->footprintSprites[0]);
        }
    }
    FootprintStamp_FreeTextSprite(&work->exitText);
}

static void FootprintStamp_LoadPlayerSpriteGfx(FootprintStamp *work, NARC *narc) {
    int i;

    SpriteSystem_LoadPaletteBufferFromOpenNarc(work->plttData, PLTTBUF_SUB_OBJ, work->spriteSystem, work->spriteManager, narc, 10, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 10004);
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 9, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 10009);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 8, FALSE, 10004);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 7, FALSE, 10004);
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 13, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 10010 + i);
    }
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 12, FALSE, 10005);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 11, FALSE, 10005);
}

#pragma opt_common_subs off
#pragma opt_strength_reduction off
#pragma opt_rotateloops off

void FootprintStamp_CreatePlayerSprites(FootprintStamp *work) {
    ManagedSpriteTemplate template;
    int i;

    work->myCursorSprite = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &sFootprintStampMyCursorTemplate);
    Sprite_TickFrame(work->myCursorSprite->sprite);
    ManagedSprite_SetDrawFlag(work->myCursorSprite, FALSE);
    template = sFootprintStampPlayerFootprintTemplate;
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        template.resIdList[0] = 10010 + i;
        work->playerFootprintSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_SetPositionXYWithSubscreenOffset(work->playerFootprintSprites[i], sFootprintStampPlayerFootprintPositions[i][0], sFootprintStampPlayerFootprintPositions[i][1], 0x110000);
        Sprite_TickFrame(work->playerFootprintSprites[i]->sprite);
        ManagedSprite_SetDrawFlag(work->playerFootprintSprites[i], FALSE);
    }
}

#pragma opt_common_subs reset
#pragma opt_strength_reduction reset
#pragma opt_rotateloops reset

void FootprintStamp_FreePlayerSprites(FootprintStamp *work) {
    int i;
    FootprintStamp *slot = work;

    Sprite_DeleteAndFreeResources(work->myCursorSprite);
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++, slot = (FootprintStamp *)((u8 *)slot + 4)) {
        Sprite_DeleteAndFreeResources(slot->playerFootprintSprites[0]);
    }
}

// Shows the party in the bar at the bottom: the footprint of each Pokemon in its ink color, empty slots hidden.

void FootprintStamp_ShowParty(FootprintStamp *work) {
    int i;
    u16 *unfaded;
    int found = 0;
    FootprintStamp *mon;
    FootprintStamp *slot;
    int column;
    u16 *pltt;
    u16 *faded;
    int j;

    unfaded = PaletteData_GetUnfadedBuf(work->plttData, PLTTBUF_MAIN_OBJ);
    faded = PaletteData_GetFadedBuf(work->plttData, PLTTBUF_MAIN_OBJ);
    for (i = 0, column = 0, mon = work, slot = work, pltt = unfaded; i < FOOTPRINT_STAMP_PARTY_SLOTS; mon = (FootprintStamp *)((u8 *)mon + sizeof(FootprintStampMon)), slot = (FootprintStamp *)((u8 *)slot + 4), column += 4, pltt++, faded++, i++) {
        if (mon->party[0].species == SPECIES_NONE || mon->party[0].species > NATIONAL_DEX_COUNT) {
            ManagedSprite_SetDrawFlag(slot->inkSprites[0], FALSE);
            ManagedSprite_SetDrawFlag(slot->slotSprites[0], FALSE);
            ManagedSprite_SetDrawFlag(slot->footprintSprites[0], FALSE);
            for (j = 0; j < 4; j++) {
                FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_2, sFootprintStampEmptySlotTiles[j], column, j + 20, 4, 1, 17);
            }
        } else {
            FootprintStamp_LoadFootprintIcon(mon->party[0].species, mon->party[0].form, slot->footprintSprites[0], work->narc, work->footprintNarc, 1, work->showArceusFootprint);
            pltt[17] = mon->party[0].color;
            faded[17] = mon->party[0].color;
            BlendPalette(unfaded + (i + 17), unfaded + (i + 33), 1, 12, RGB_BLACK);
            faded[33] = pltt[33];
            if (!found) {
                found++;
                work->selectedSlot = (u8)i;
            }
        }
    }
    FootprintStamp_HighlightPartySlot(work, 0);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_MAIN_2);
}

// Copies the footprint of a Pokemon (or the generic one) into the 2x2-tile character of a sprite.
static BOOL FootprintStamp_LoadFootprintIcon(int species, int form, ManagedSprite *sprite, NARC *narc, NARC *footprintNarc, int screen, void *unk19E0) {
    void *file;
    NNSG2dCharacterData *charData;
    u8 *bottom;
    u8 *top;
    u32 vram;

    if (species == SPECIES_NONE || species > NATIONAL_DEX_COUNT) {
        return FALSE;
    }
    if (FootprintStamp_GetFootprintType(species, form, (u32)unk19E0) == 1) {
        file = GfGfxLoader_LoadFromOpenNarc(footprintNarc, species + 3, TRUE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
        DC_FlushRange(charData->pRawData, 0x100);
        top = charData->pRawData;
        bottom = top + 0x80;
    } else {
        file = GfGfxLoader_LoadFromOpenNarc(narc, 16, FALSE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
        DC_FlushRange(charData->pRawData, 0x80);
        top = charData->pRawData;
        bottom = top;
        top += 0x40;
    }
    if (screen == NNS_G2D_VRAM_TYPE_2DMAIN) {
        vram = HW_OBJ_VRAM;
    } else {
        vram = HW_DB_OBJ_VRAM;
    }
    {
        NNSG2dImageProxy *proxy = Sprite_GetImageProxy(sprite->sprite);
        MIi_CpuCopy16((u16 *)bottom, (u16 *)(vram + proxy->vramLocation.baseAddrOfVram[screen]), 0x40);
        MIi_CpuCopy16((u16 *)top, (u16 *)(vram + 0x40 + proxy->vramLocation.baseAddrOfVram[screen]), 0x40);
    }
    Heap_Free(file);
    return TRUE;
}

void FootprintStamp_CreateCamera(FootprintStamp *work) {
    VecFx32 target = sFootprintStampCameraTarget;
    fx32 width, height;
    Camera *camera;

    camera = Camera_New(HEAP_ID_125);
    work->camera = camera;
    Camera_Init_FromTargetDistanceAndAngle(&target, 0x7C000, &sFootprintStampCameraAngle, 0xFA4, 0, FALSE, work->camera);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, 0x384000, work->camera);
    Camera_SetStaticPtr(work->camera);
    Math_CalcPerspectiveViewSize(Camera_GetPerspectiveAngle(work->camera), Camera_GetDistance(work->camera), 0x1555, &width, &height);
    work->viewWidth = width;
    work->viewHeight = height;
}

void FootprintStamp_FreeCamera(FootprintStamp *work) {
    Camera_Delete(work->camera);
}

void FootprintStamp_LoadBoardModel(FootprintStamp *work, NARC *narc) {
    int fileId = work->args->boardType == 0 ? 28 : 27;

    ModelResource_LoadFromNarc(&work->boardModel, narc, fileId, HEAP_ID_125);
    ModelInstance_Init(&work->boardInstance, &work->boardModel);
    ModelInstance_SetPosition(&work->boardInstance, 0, 0, 0);
    ModelInstance_SetScale(&work->boardInstance, FX32_ONE, FX32_ONE, FX32_ONE);
    ModelInstance_SetVisible(&work->boardInstance, TRUE);
}

void FootprintStamp_FreeBoardModel(FootprintStamp *work) {
    ModelResource_Free(&work->boardModel);
}

void FootprintStamp_Draw3D(FootprintStamp *work) {
    VecFx32 scale, trans;
    MtxFx33 rot;
    u32 op;

    scale.x = FX32_ONE;
    scale.y = FX32_ONE;
    scale.z = FX32_ONE;
    trans.x = 0;
    trans.y = 0;
    trans.z = 0;
    MTX_Identity33_(&rot);
    Thunk_G3X_Reset();
    Camera_SetStaticPtr(work->camera);
    Camera_ApplyPerspectiveType(0, work->camera);
    Camera_PushLookAtToNNSGlb();
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0, -FX32_ONE, 0);
    NNS_G3dGlbLightColor(GX_LIGHTID_0, GX_RGB(28, 28, 28));
    NNS_G3dGlbMaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    NNS_G3dGlbSetBaseTrans(&trans);
    NNS_G3dGlbSetBaseRot(&rot);
    NNS_G3dGlbSetBaseScale(&scale);
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    ModelInstance_Draw(&work->boardInstance);
    FootprintStampBoard_Draw(&work->board);
    op = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &op, 1);
}

#pragma opt_common_subs off
#pragma opt_strength_reduction off
#pragma opt_rotateloops off

void FootprintStamp_CreateWindows(FootprintStamp *work) {
    Window *windows;
    int i;
    const u8(*pos)[2];

    AddWindowParameterized(work->bgConfig, &work->msgWindow, GF_BG_LYR_MAIN_1, 2, 1, 27, 4, 13, 0x5B);
    FillWindowPixelBuffer(&work->msgWindow, 15);
    i = 0;
    pos = sFootprintStampPlayerNameWindowPositions;
    windows = work->playerNameWindows;
    for (; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        AddWindowParameterized(work->bgConfig, &windows[i], GF_BG_LYR_SUB_1, pos[i][0], pos[i][1], 8, 2, 13, 0x200 + i * 16);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

#pragma opt_common_subs reset
#pragma opt_strength_reduction reset
#pragma opt_rotateloops reset

void FootprintStamp_FreeWindows(FootprintStamp *work) {
    int i;
    Window *windows;

    RemoveWindow(&work->msgWindow);
    windows = work->playerNameWindows;
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        RemoveWindow(&windows[i]);
    }
}

GF3DVramMan *FootprintStamp_Create3DVramMan(enum HeapID heapId) {
    return GF_3DVramMan_Create(heapId, 0, 2, 0, 2, FootprintStamp_Setup3D);
}

static void FootprintStamp_Setup3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7FFF, 63, FALSE);
    *(vu32 *)0x04000580 = 0xBFFF0000; // G3_ViewPort(0, 0, 255, 191)
}

void FootprintStamp_Free3DVramMan(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}

void FootprintStamp_InitLinkFilter(FootprintStamp *work) {
    work->linkFilter.myId = WiFiPlazaLobbyClient_GetOwnId(work);
}

void FootprintStampMon_InitDummy(void *unused, FootprintStampMon *mon) {
    MI_CpuFill8(mon, 0, sizeof(FootprintStampMon));
    mon->species = SPECIES_MEWTWO;
    mon->color = 31;
    mon->personality = 257;
    mon->form = 0;
}

FootprintStampLinkFilter *FootprintStamp_GetLinkFilter(FootprintStamp *work) {
    return &work->linkFilter;
}

void FootprintStamp_LoadParty(u32 inkColumn, SaveData *saveData, FootprintStampMon *party) {
    Party *playerParty;
    Pokemon *mon;
    int count;
    int i;

    MI_CpuFill8(party, 0, FOOTPRINT_STAMP_PARTY_SLOTS * sizeof(FootprintStampMon));
    playerParty = SaveArray_Party_Get(saveData);
    count = Party_GetCount(playerParty);
    i = 0;
    if (count > 0) {
        do {
            mon = Party_GetMonByIndex(playerParty, i);
            party->species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            party->personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
            party->form = GetMonData(mon, MON_DATA_FORM, NULL);
            party->color = FootprintStamp_GetInkColor(inkColumn, GetMonData(mon, MON_DATA_OT_ID, NULL));
            i++;
            party++;
        } while (i < count);
    }
}

// Puts a stamp on the board (own or received) and shows its footprint next to the player's name.
BOOL FootprintStamp_PlaceStamp(FootprintStamp *work, FootprintStampMon *mon, u32 playerId) {
    BOOL result;
    int index;
    u8 *sprites;
    u32 offset;

    result = FootprintStampBoard_AddStamp(work, &work->board, mon, work->narc, work->footprintNarc, work->showArceusFootprint);
    if (result == TRUE) {
        index = WiFiPlazaLobbyClient_IdToChannelSlot(playerId);
        if (index != -1) {
            if (mon->species != work->playerSpecies[index] || mon->color != work->playerColors[index]) {
                work->playerSpecies[index] = mon->species;
                work->playerColors[index] = mon->color;
                sprites = (u8 *)work->playerFootprintSprites;
                offset = index * 4;
                FootprintStamp_LoadFootprintIcon(mon->species, mon->form, *(ManagedSprite **)(sprites + offset), work->narc, work->footprintNarc, NNS_G2D_VRAM_TYPE_2DSUB, work->showArceusFootprint);
                ManagedSprite_SetDrawFlag(*(ManagedSprite **)(sprites + offset), TRUE);
                PaletteData_FillPaletteInBuffer(work->plttData, PLTTBUF_SUB_BG, PLTTSEL_BOTH, mon->color, index + 33, index + 34);
            }
        }
        PlaySE(SEQ_SE_DP_BOX02);
    }
    return result;
}

#pragma opt_common_subs off
#pragma opt_strength_reduction off
#pragma opt_rotateloops off
#pragma opt_lifetimes off
#pragma opt_propagation off
// Syncs the player list with the lobby: prints joined players' names, clears the rows of players who left.
// Returns 1 if someone joined, 2 if someone left, else 0.

int FootprintStamp_UpdatePlayers(FootprintStamp *work) {
    int joined;
    int left;
    FootprintStampPlayerList list;
    const FootprintStampRow *rows;
    long i;
    u16 *tilemap;
    int count;
    const FootprintStampRow *row;
    u32 size;
    int y;

    joined = 0;
    left = 0;
    WiFiPlazaLobbyClient_GetChannelPlayerList(&list);
    rows = sFootprintStampPlayerRows;
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        if (work->playerIds[i] != list.ids[i]) {
            if (list.ids[i] != (u32)-1) {
                FootprintStamp_PrintPlayerName(work->msgData, work->msgFormat, work->playerNameWindows, work->args->lobby, list.ids[i]);
                tilemap = GetBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_2);
                row = &rows[i];
                for (y = row->y; y < row->y + row->height; y++) {
                    size = row->width * 2;
                    MIi_CpuCopy16(&work->subTilemap[y * 32 + ((const u16 *)sFootprintStampPlayerRows)[i * 4]], &tilemap[y * 32 + ((const u16 *)sFootprintStampPlayerRows)[i * 4]], size);
                }
                ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_SUB_2);
                joined++;
            } else {
                FootprintStamp_ClearPlayerNameWindow(work->playerNameWindows, i);
                work->playerSpecies[i] = SPECIES_NONE;
                work->playerColors[i] = 0;
                ManagedSprite_SetDrawFlag(work->playerFootprintSprites[i], FALSE);
                FillBgTilemapRect(work->bgConfig, GF_BG_LYR_SUB_2, 0, sFootprintStampPlayerRows[i].x, sFootprintStampPlayerRows[i].y, sFootprintStampPlayerRows[i].width, sFootprintStampPlayerRows[i].height, 16);
                ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_SUB_2);
                left++;
            }
            if (work->linkFilter.myId == list.ids[i]) {
                ManagedSprite_SetPositionXYWithSubscreenOffset(work->myCursorSprite, sFootprintStampMyCursorPositions[i][0], sFootprintStampMyCursorPositions[i][1], 0x110000);
                ManagedSprite_SetDrawFlag(work->myCursorSprite, TRUE);
            }
        }
        work->playerIds[i] = list.ids[i];
    }
    count = 0;
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        if (work->playerIds[i] != (u32)-1) {
            count++;
        }
    }
    work->board.playerCount = count;
    if (joined > 0) {
        PlaySE(SEQ_SE_DP_BUTTON9);
        return 1;
    }
    if (left > 0) {
        PlaySE(SEQ_SE_DP_BUTTON9);
        return 2;
    }
    return 0;
}

#pragma opt_common_subs reset
#pragma opt_strength_reduction reset
#pragma opt_rotateloops reset
#pragma opt_lifetimes reset
#pragma opt_propagation reset

void FootprintStamp_ShowPlayerName(FootprintStamp *work, u32 playerId) {
    FootprintStamp_PrintPlayerName(work->msgData, work->msgFormat, work->playerNameWindows, work->args->lobby, playerId);
}

void FootprintStamp_ClearPlayerName(FootprintStamp *work, u32 index) {
    FootprintStamp_ClearPlayerNameWindow(work->playerNameWindows, index);
}

void FootprintStamp_StartSelectEffect(FootprintStamp *work, int slot) {
    ManagedSpriteTemplate template = sFootprintStampSelectEffectTemplate;
    int i;
    FootprintStamp *effect;

    i = 0;
    effect = work;
    for (; i < 3; i++, effect = (FootprintStamp *)((u8 *)effect + 4)) {
        if (effect->selectSprites[0] == NULL) {
            template.x = (slot << 5) + 16;
            template.y = 176;
            work->selectSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
            break;
        }
    }
    FootprintStamp_HighlightPartySlot(work, slot);
    PlaySE(SEQ_SE_DP_BUTTON3);
}

static void FootprintStamp_HighlightPartySlot(FootprintStamp *work, int slot) {
    u16 *unfaded = PaletteData_GetUnfadedBuf(work->plttData, PLTTBUF_MAIN_OBJ);
    u16 *faded = PaletteData_GetFadedBuf(work->plttData, PLTTBUF_MAIN_OBJ);

    MIi_CpuCopy16(unfaded + 17, faded + 17, FOOTPRINT_STAMP_PARTY_SLOTS * sizeof(u16));
    slot += 17;
    BlendPalette(unfaded + slot, faded + slot, 1, 6, RGB_BLACK);
}

void FootprintStamp_UpdateSelectEffects(FootprintStamp *work) {
    int i;
    FootprintStamp *effect = work;

    for (i = 0; i < 3; i++, effect = (FootprintStamp *)((u8 *)effect + 4)) {
        ManagedSprite *sprite = effect->selectSprites[0];
        if (sprite != NULL) {
            if (!ManagedSprite_IsAnimated(sprite)) {
                Sprite_DeleteAndFreeResources(effect->selectSprites[0]);
                effect->selectSprites[0] = NULL;
            } else {
                Sprite_TickFrame(effect->selectSprites[0]->sprite);
            }
        }
    }
}

void FootprintStamp_CreateTextSprite(FootprintStamp *work, FootprintStampTextSprite *text, String *string, int font, int color, int priority, int plttResId, int x, int y, BOOL centered) {
    TextOBJTemplate template;
    Window window;
    ObjCharSpaceReservation charTransfer;
    int width, tiles;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    TextOBJ *textObj;

    if (text->textObj != NULL) {
        GF_AssertFail();
    }
    bgConfig = work->bgConfig;
    spriteManager = work->spriteManager;
    FootprintStamp_GetTextSize(string, font, &width, &tiles);
    InitWindow(&window);
    AddTextWindowTopLeftCorner(bgConfig, &window, (u8)tiles, 2, 0, 0);
    AddTextPrinterParameterizedWithColorAndSpacing(&window, font, string, 0, 0, TEXT_SPEED_NOTRANSFER, color, 0, 0, NULL);
    ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_125), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &charTransfer);
    if (centered == TRUE) {
        x -= width / 2;
    }
    template.fontSystem = work->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(spriteManager, plttResId);
    template.sprite = NULL;
    template.offset = charTransfer.offset;
    template.x = x;
    template.y = y - 8;
    template.bgPriority = 0;
    template.drawPriority = 51;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_125;
    textObj = FontOAM_Create(&template);
    TextOBJ_SetPalOffsetRespectVramOffset(textObj, priority);
    TextOBJ_SetPosition(textObj, x, y - 8);
    RemoveWindow(&window);
    text->textObj = textObj;
    text->charTransfer = charTransfer;
    text->width = width;
}

void FootprintStamp_FreeTextSprite(FootprintStampTextSprite *text) {
    FontOAM_Delete(text->textObj);
    ObjCharTransfer_FreeCharSpace(&text->charTransfer);
}

static void FootprintStamp_GetTextSize(String *string, int font, int *width, int *tiles) {
    int textWidth = FontID_String_GetWidth(font, string, 0);
    int textTiles = textWidth / 8;

    if (FX_ModS32(textWidth, 8) != 0) {
        textTiles++;
    }
    *width = textWidth;
    *tiles = textTiles;
}

static inline int FootprintStamp_InkGaugeY(int y) {
    return y + 176;
}

// Refills the ink and moves the ink level shown above the party slots towards it.

void FootprintStamp_UpdateInkGauge(FootprintStamp *work) {
    int i, x;
    s16 y;
    FootprintStamp *slot;

    if (work->ink == work->inkShown) {
        work->ink += 0x88;
        if (work->ink > FOOTPRINT_STAMP_INK_MAX) {
            work->ink = FOOTPRINT_STAMP_INK_MAX;
        }
        work->inkShown = work->ink;
    } else {
        work->inkShown -= work->inkStep;
        if (work->inkShown < work->ink) {
            work->inkShown = work->ink;
        }
    }
    i = 0;
    y = FootprintStamp_InkGaugeY(32 - (work->inkShown >> 8));
    x = 16;
    slot = work;
    do {
        ManagedSprite_SetPositionXYWithSubscreenOffset(slot->inkSprites[0], x, y, 0x110000);
        i++;
        x += 32;
        slot = (FootprintStamp *)((u8 *)slot + 4);
    } while (i < FOOTPRINT_STAMP_PARTY_SLOTS);
}

BOOL FootprintStamp_UseInk(FootprintStamp *work, fx32 amount) {
    fx32 diff;

    if (work->ink < amount) {
        return FALSE;
    }
    if (work->inkShown < work->ink) {
        GF_ASSERT(FALSE);
        work->inkShown = work->ink;
    }
    work->ink -= amount;
    diff = work->inkShown - work->ink;
    work->inkStep = diff / 4;
    return TRUE;
}

void FootprintStamp_UpdateDummy(FootprintStamp *work) {
}

void FootprintStampBoard_Init(FootprintStampBoard *board, void *unk19E0) {
    MI_CpuFill8(board, 0, sizeof(FootprintStampBoard));
}

void FootprintStampBoard_Free(FootprintStampBoard *board) {
    int i;
    FootprintStampBoard *slot = board;

    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++, slot = (FootprintStampBoard *)((u8 *)slot + 4)) {
        if (slot->stamps[0] != NULL) {
            FootprintStamp_DeleteStamp(slot->stamps[0]);
        }
    }
}

static BOOL FootprintStampBoard_AddStamp(FootprintStamp *work, FootprintStampBoard *board, const FootprintStampMon *mon, NARC *narc, NARC *footprintNarc, void *unk19E0) {
    int i;

    if (board->cameraEffect != FOOTPRINT_STAMP_EFFECT_NONE) {
        return FALSE;
    }
    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
        if (board->stamps[i] == NULL) {
            board->stamps[i] = FootprintStampBoard_CreateStamp(work, board, mon, narc, footprintNarc, unk19E0);
            if (board->stamps[i] != NULL) {
                return TRUE;
            }
            return FALSE;
        }
    }
    return FALSE;
}

void FootprintStampBoard_Update(FootprintStampBoard *board, Camera *camera, int phase, u32 inkColumn) {
    int i;

    if (board->pendingCameraEffect) {
        FootprintStampBoard_Flash(board, &board->flash, phase, inkColumn);
    }
    if (board->cameraEffect) {
        if (sFootprintStampCameraEffectFuncs[board->cameraEffect](board, board, camera) == TRUE) {
            board->cameraEffect = FOOTPRINT_STAMP_EFFECT_NONE;
        }
    }
    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
        FootprintStampObj *stamp = board->stamps[i];
        if (stamp) {
            if (stamp->effectId == FOOTPRINT_STAMP_EFFECT_NONE && stamp->pendingEffect != FOOTPRINT_STAMP_EFFECT_NONE) {
                stamp->effectId = stamp->pendingEffect;
            }
            if (sFootprintStampEffectFuncs[stamp->effectId].update(board, stamp) == TRUE) {
                FootprintStampBoard_RemoveStamp(board, stamp, i);
            }
        }
    }
}

static void FootprintStampBoard_Draw(FootprintStampBoard *board) {
    int i;

    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
        FootprintStampObj *stamp = board->stamps[i];
        if (stamp != NULL) {
            u32 effect = stamp->effectId;
            if (sFootprintStampEffectFuncs[effect].draw != NULL) {
                sFootprintStampEffectFuncs[effect].draw(board, stamp);
            } else {
                ModelInstance_Draw(&stamp->instance);
            }
        }
    }
}

// Idle stamps touched by a stamp playing an effect take over that effect and join its chain; once a chain
// is long enough, the other stamps are removed and the chain's camera move starts.

void FootprintStampBoard_ResolveChains(FootprintStampBoard *board) {
    int i;
    int numActive;
    int numIdle;
    int m;
    int j;
    FootprintStampObj *stamp;
    FootprintStampObj *other;

    numActive = 0;
    numIdle = 0;
    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
        stamp = board->stamps[i];
        if (stamp != NULL) {
            if (stamp->effectId != FOOTPRINT_STAMP_EFFECT_NONE) {
                board->sorted[numActive] = stamp;
                numActive++;
            } else {
                board->sorted[FOOTPRINT_STAMP_STAMPS_MAX - 1 - numIdle] = stamp;
                numIdle++;
            }
        }
    }
    for (j = 0; j < numIdle; j++) {
        other = board->sorted[FOOTPRINT_STAMP_STAMPS_MAX - 1 - j];
        for (m = 0; m < numActive; m++) {
            stamp = board->sorted[m];
            if (sFootprintStampEffectFuncs[stamp->effectId].hit(board, stamp, other) == TRUE && FootprintStamp_SetEffect(other, stamp->effectId) == TRUE && other->chain == 0xFF) {
                other->chain = stamp->chain;
                FootprintStampBoard_AddToChain(board, stamp->chain, stamp->effectId);
                break;
            }
        }
    }
    if (board->pendingCameraEffect != FOOTPRINT_STAMP_EFFECT_NONE && board->flashDone == TRUE) {
        for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
            stamp = board->stamps[i];
            if (stamp != NULL && stamp->effectId != board->pendingCameraEffect && stamp->pendingEffect != board->pendingCameraEffect && FootprintStamp_SetEffect(stamp, board->pendingCameraEffect) == FALSE) {
                FootprintStampBoard_RemoveStamp(board, stamp, i);
            }
        }
        FootprintStampBoard_StartCameraEffect(board);
    }
}

// Uploads the textures of new or redrawn stamps, and the flash brightness.
void FootprintStampBoard_VBlank(FootprintStampBoard *board, int phase) {
    int i;
    NNSG3dResTex **texture = board->pendingTextures;

    for (i = 0; i < FOOTPRINT_STAMP_TEXTURES_MAX; i++, texture++) {
        if (*texture != NULL) {
            DC_FlushRange(*texture, (*texture)->header.size);
            NNS_G3dTexLoad(*texture, TRUE);
            NNS_G3dPlttLoad(*texture, TRUE);
            *texture = NULL;
        }
    }
    if (board->brightnessChanged != FALSE && phase != 3) {
        SetMasterBrightness(PM_LCD_TOP, board->brightness);
        board->brightnessChanged = FALSE;
    }
}

static FootprintStampObj *FootprintStampBoard_CreateStamp(FootprintStamp *work, FootprintStampBoard *board, const FootprintStampMon *mon, NARC *narc, NARC *footprintNarc, void *unk19E0) {
    FootprintStampObj *stamp = Heap_Alloc(HEAP_ID_125, sizeof(FootprintStampObj));
    fx32 x, y;

    MI_CpuFill8(stamp, 0, sizeof(FootprintStampObj));
    stamp->chain = 0xFF;
    stamp->mon = *mon;
    if (stamp->mon.species > NATIONAL_DEX_COUNT) {
        GF_AssertFail();
        stamp->mon.species = SPECIES_DITTO;
    }
    if (!FootprintStamp_LoadStampModel(board, &stamp->model, narc, footprintNarc, &stamp->mon, unk19E0)) {
        return NULL;
    }
    ModelInstance_Init(&stamp->instance, &stamp->model);
    FootprintStamp_TouchToBoardPos(work, mon->x, mon->y, &x, &y);
    ModelInstance_SetPosition(&stamp->instance, x, y, 0x10000);
    ModelInstance_SetScale(&stamp->instance, FX32_ONE, FX32_ONE, FX32_ONE);
    ModelInstance_SetVisible(&stamp->instance, FALSE);
    return stamp;
}

static BOOL FootprintStamp_LoadStampModel(FootprintStampBoard *board, FootprintStampModel *model, NARC *narc, NARC *footprintNarc, FootprintStampMon *mon, void *unk19E0) {
    model->file = GfGfxLoader_LoadFromOpenNarc(narc, 29, FALSE, HEAP_ID_125, FALSE);
    GF_ASSERT(model->file);
    model->set = NNS_G3dGetMdlSet(model->file);
    model->model = NNS_G3dGetMdlByIdx(model->set, 0);
    model->texture = NNS_G3dGetTex(model->file);
    if (model->texture) {
        int allocated;
        FootprintStamp_DrawFootprintTexture(model->texture, narc, footprintNarc, mon, (u32)unk19E0);
        allocated = FootprintStamp_AllocTextureVram(model->texture);
        if (!allocated || !FootprintStampBoard_QueueTexture(board, model->texture)) {
            if (allocated == TRUE) {
                NNSG3dTexKey texKey, tex4x4Key;
                NNS_G3dTexReleaseTexKey(model->texture, &texKey, &tex4x4Key);
                NNS_GfdDefaultFuncFreeTexVram(texKey);
                NNS_GfdDefaultFuncFreeTexVram(tex4x4Key);
                NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(model->texture));
            }
            if (model->file) {
                Heap_Free(model->file);
            }
            memset(model, 0, sizeof(FootprintStampModel));
            return FALSE;
        }
        GF3dRender_BindModelSet(model->file, model->texture);
    }
    return TRUE;
}

BOOL FootprintStampBoard_QueueTexture(FootprintStampBoard *board, NNSG3dResTex *texture) {
    int i;

    for (i = 0; i < FOOTPRINT_STAMP_TEXTURES_MAX; i++) {
        if (board->pendingTextures[i] == NULL) {
            board->pendingTextures[i] = texture;
            return TRUE;
        }
    }
    return FALSE;
}

void FootprintStampBoard_UnqueueTexture(FootprintStampBoard *board, NNSG3dResTex *texture) {
    int i;

    for (i = 0; i < FOOTPRINT_STAMP_TEXTURES_MAX; i++) {
        if (board->pendingTextures[i] == texture) {
            board->pendingTextures[i] = NULL;
            return;
        }
    }
}

static BOOL FootprintStamp_AllocTextureVram(NNSG3dResTex *texture) {
    u32 texSize;
    u32 plttSize;
    NNSG3dTexKey texKey;
    NNSG3dPlttKey plttKey;

    texture->texInfo.vramKey = 0;
    texSize = NNS_G3dTexGetRequiredSize(texture);
    plttSize = NNS_G3dPlttGetRequiredSize(texture);
    texKey = NNS_GfdDefaultFuncAllocTexVram(texSize, FALSE, 0);
    if (texKey == 0) {
        return FALSE;
    }
    plttKey = NNS_GfdDefaultFuncAllocPlttVram(plttSize, texture->tex4x4Info.flag & NNS_G3D_RESPLTT_USEPLTT4, 0);
    if (plttKey == 0) {
        NNS_GfdDefaultFuncFreeTexVram(texKey);
        return FALSE;
    }
    NNS_G3dTexSetTexKey(texture, texKey, 0);
    NNS_G3dPlttSetPlttKey(texture, plttKey);
    return TRUE;
}

// Draws a footprint (the Pokemon's own, or the generic one) into the 16x16 2bpp texture of a stamp model and sets
// the stamp's ink color as palette color 1.
void FootprintStamp_DrawFootprintTexture(NNSG3dResTex *texture, NARC *narc, NARC *footprintNarc, FootprintStampMon *mon, u32 unk19E0) {
    int shift;
    u8 *src;
    NNSG2dCharacterData *charData;
    u16 *dst;
    u16 *bitmap;
    void *file;
    u32 type;
    int k;
    int x;
    int y;
    int i;
    u8 *chars;

    type = FootprintStamp_GetFootprintType(mon->species, mon->form, unk19E0);
    bitmap = (u16 *)((u8 *)texture + texture->texInfo.ofsTex);
    if (type == 1) {
        file = GfGfxLoader_LoadFromOpenNarc(footprintNarc, mon->species + 3, TRUE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
    } else {
        file = GfGfxLoader_LoadFromOpenNarc(narc, 16, FALSE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
    }
    MI_CpuClear16(bitmap, 0x40);
    chars = charData->pRawData;
    src = chars;
    dst = bitmap;
    // the four 8x8 tiles of the footprint, in the order top left, top right, bottom left, bottom right
    for (i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            switch (type) {
            case 1:
                src = chars + 0x80;
                break;
            default:
                src = chars;
                break;
            }
            dst = bitmap;
            break;
        case 1:
            switch (type) {
            case 1:
                src = chars + 0xA0;
                break;
            default:
                src = chars + 0x20;
                break;
            }
            dst = bitmap + 1;
            break;
        case 2:
            switch (type) {
            case 1:
                src = chars;
                break;
            default:
                src = chars + 0x40;
                break;
            }
            dst = bitmap + 0x10;
            break;
        case 3:
            switch (type) {
            case 1:
                src = chars + 0x20;
                break;
            default:
                src = chars + 0x60;
                break;
            }
            dst = bitmap + 0x11;
            break;
        }
        // 4bpp pixels -> 2bpp: any non-zero pixel becomes color 1
        k = 0;
        for (y = 0; y < 8; y++) {
            for (shift = 0, x = 0; x < 4; x++, shift += 4, k++) {
                if (src[k] & 0xF) {
                    *dst |= 1 << shift;
                }
                if (src[k] & 0xF0) {
                    *dst |= 1 << (shift + 2);
                }
            }
            dst += 2;
        }
    }
    *(u16 *)((u8 *)texture + texture->plttInfo.ofsPlttData + 2) = mon->color;
    Heap_Free(file);
}
