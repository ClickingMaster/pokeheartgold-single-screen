#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "frontier/battle_factory_app_internal.h"
#include "frontier/battle_factory_setup.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0194.h"

#include "battle_frontier_communication.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "communication_core.h"
#include "communication_player_state.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "grid_input_handler.h"
#include "heap.h"
#include "link_synchronization.h"
#include "list_menu_2d.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokemon_summary.h"
#include "pokepic.h"
#include "render_window.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "system.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"
#include "yes_no_prompt.h"

FS_EXTERN_OVERLAY(OVY_80);

// Main loop states
#define FACTORY_STATE_SLIDE_IN           0
#define FACTORY_STATE_SHOW_SCREEN        1
#define FACTORY_STATE_SUMMARY            2
#define FACTORY_STATE_SELECT_RENTALS     3
#define FACTORY_STATE_CONFIRM_RENTALS    4
#define FACTORY_STATE_CANCEL_LAST_RENTAL 5
#define FACTORY_STATE_SELECT_SWAP_OUT    6
#define FACTORY_STATE_CANCEL_SWAP        7
#define FACTORY_STATE_SELECT_SWAP_IN     8
#define FACTORY_STATE_CANCEL_SWAP_IN     9
#define FACTORY_STATE_SWITCH_PAGE        10
#define FACTORY_STATE_LINK_SEND_SWAP     11
#define FACTORY_STATE_LINK_SYNC          12
#define FACTORY_STATE_FADE_OUT           13
#define FACTORY_STATE_LINK_TRADE_RESULT  14

// Link commands (sendBuf[0])
#define FACTORY_LINK_PING    7
#define FACTORY_LINK_RENTALS 8
#define FACTORY_LINK_SWAP    10

typedef struct BattleFactoryAppArgs {
    SaveData *saveData;
    u8 frontierType;
    u8 levelMode;
    u8 screen;
    u8 unk_07;
    Party *party;
    Party *otherParty;
    u16 result[3];
} BattleFactoryAppArgs;

void Frontier_BufferPartnerName(MessageFormat *msgFmt, u32 field);

static BOOL BattleFactoryApp_SlideIn(BattleFactoryApp *app);
static BOOL BattleFactoryApp_ShowScreen(BattleFactoryApp *app);
static void BattleFactoryApp_SetupRentalScreen(BattleFactoryApp *app);
static void BattleFactoryApp_CreateRentalBalls(BattleFactoryApp *app);
static void BattleFactoryApp_CreateRentalPokepics(BattleFactoryApp *app);
static void BattleFactoryApp_SetupSwapOutScreen(BattleFactoryApp *app);
static void BattleFactoryApp_SetupSwapInScreen(BattleFactoryApp *app);
static void BattleFactoryApp_CreateSwapBalls(BattleFactoryApp *app);
static void BattleFactoryApp_CreateSwapPokepics(BattleFactoryApp *app);
static BOOL BattleFactoryApp_WaitSummary(BattleFactoryApp *app);
static BOOL BattleFactoryApp_SelectRentals(BattleFactoryApp *app);
static void BattleFactoryApp_UndoLastRental(BattleFactoryApp *app);
static void BattleFactoryApp_AddRental(BattleFactoryApp *app);
static void BattleFactoryApp_RemoveRental(BattleFactoryApp *app);
static BOOL BattleFactoryApp_ConfirmRentals(BattleFactoryApp *app);
static BOOL BattleFactoryApp_CancelLastRental(BattleFactoryApp *app);
static void BattleFactoryApp_DeselectCursorRental(BattleFactoryApp *app);
static BOOL BattleFactoryApp_SelectSwapOut(BattleFactoryApp *app);
static void BattleFactoryApp_PickSwapOut(BattleFactoryApp *app);
static BOOL BattleFactoryApp_ConfirmCancelSwap(BattleFactoryApp *app);
static void BattleFactoryApp_ResumeSwapOut(BattleFactoryApp *app);
static BOOL BattleFactoryApp_SelectSwapIn(BattleFactoryApp *app);
static void BattleFactoryApp_UnpickSwapOut(BattleFactoryApp *app);
static void BattleFactoryApp_PickSwapIn(BattleFactoryApp *app);
static void BattleFactoryApp_ShowSwapInPrompt(BattleFactoryApp *app);
static BOOL BattleFactoryApp_ConfirmCancelSwapIn(BattleFactoryApp *app);
static void BattleFactoryApp_ResumeSwapIn(BattleFactoryApp *app);
static BOOL BattleFactoryApp_SwitchPage(BattleFactoryApp *app);
static BOOL BattleFactoryApp_LinkSendSwap(BattleFactoryApp *app);
static BOOL BattleFactoryApp_LinkSync(BattleFactoryApp *app);
static BOOL BattleFactoryApp_FadeOut(BattleFactoryApp *app);
static BOOL BattleFactoryApp_ShowLinkTradeResult(BattleFactoryApp *app);
static void BattleFactoryApp_VBlankCallback(BattleFactoryApp *app);
static void BattleFactoryApp_SetGraphicsBanks(void);
static void BattleFactoryApp_InitBgLayers(BgConfig *bgConfig);
static void BattleFactoryApp_InitBgGraphics(BattleFactoryApp *app);
static void BattleFactoryApp_InitSprites(BattleFactoryApp *app);
static void BattleFactoryApp_FreeBgLayers(BgConfig *bgConfig);
static void BattleFactoryApp_SetMainState(BattleFactoryApp *app, int *state, int nextState);
static BOOL BattleFactoryApp_ScrollBallsIn(BattleFactoryApp *app);
static BOOL BattleFactoryApp_ScrollBallsOut(BattleFactoryApp *app);
static void BattleFactoryApp_AnimateBackground(BattleFactoryApp *app);
static void BattleFactoryApp_SetBgPalette(BattleFactoryApp *app, int bgId, u8 palette, u8 width, u8 height);
static void BattleFactoryApp_SetCenterAnim(BattleFactoryApp *app, int kind);
static void BattleFactoryApp_LoadBgGfx81(BattleFactoryApp *app, GFBgLayer bgId);
static void BattleFactoryApp_LoadBgGfx92(BattleFactoryApp *app, GFBgLayer bgId);
static void BattleFactoryApp_LoadBgGfx4(BattleFactoryApp *app, GFBgLayer bgId);
static void BattleFactoryApp_LoadBgPalette(void);
static void BattleFactoryApp_LoadSubScreenBg(BattleFactoryApp *app, GFBgLayer bgId);
static u8 BattleFactoryApp_PrintMessage(BattleFactoryApp *app, Window *window, u32 msgId, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId);
static u8 BattleFactoryApp_ShowMessage(BattleFactoryApp *app, u32 msgId);
static void BattleFactoryApp_BufferNumber(BattleFactoryApp *app, u32 field, s32 value);
static void BattleFactoryApp_BufferSpeciesName(BattleFactoryApp *app, u32 field, BoxPokemon *boxMon);
static void BattleFactoryApp_PrintPlayerName(BattleFactoryApp *app, Window *window, int x, int y, u8 fontId);
static void BattleFactoryApp_PrintPartnerName(BattleFactoryApp *app, Window *window, int x, int y, u8 fontId);
static void BattleFactoryApp_PrintMonName(BattleFactoryApp *app, Window *window, int monIndex, int x, int y, u8 fg, u8 shadow, u8 bg, u8 fontId, Party *party);
static void BattleFactoryApp_PrintPartnerMonName(BattleFactoryApp *app, Window *window, int x, int y, u8 fg, u8 shadow, u8 bg, u8 fontId, u16 species, u8 gender);
static void BattleFactoryApp_FreeGraphics(BattleFactoryApp *app);
static void BattleFactoryApp_ResumeAfterSummary(BattleFactoryApp *app);
static void BattleFactoryApp_DisableDisplay(void);
static void BattleFactoryApp_InitGraphics(BattleFactoryApp *app);
static void BattleFactoryApp_CreateSummaryArgs(BattleFactoryApp *app);
static BOOL BattleFactoryApp_IsScreen(BattleFactoryApp *app, u8 screen);
static int BattleFactory_GetRentalCount(int frontierType);
static BOOL BattleFactoryApp_IsCancelRequested(BattleFactoryApp *app);
static void BattleFactoryApp_SetCancelRequested(BattleFactoryApp *app, u8 value);
static void BattleFactoryApp_CreatePokepic(BattleFactoryApp *app, int index, Pokemon *mon, int x, int y, int hFlip);
static BOOL BattleFactoryApp_SendLinkCommand(BattleFactoryApp *app, int command, int arg);
static void BattleFactoryApp_PrepareLinkPing(BattleFactoryApp *app, int command);
static void BattleFactoryApp_PrepareLinkRentals(BattleFactoryApp *app, int command, int arg);
static void BattleFactoryApp_PrintPartnerRentals(BattleFactoryApp *app);
static void BattleFactoryApp_PrepareLinkSwap(BattleFactoryApp *app, int command, int accepted);
static BattleFactoryBall *BattleFactoryApp_CreateBall(BattleFactoryApp *app, int index);
static BattleFactorySprite *BattleFactoryApp_CreateCenterSprite(BattleFactoryApp *app, int animSeq);
static void BattleFactoryApp_DeleteCenterSprites(BattleFactoryApp *app);
static void BattleFactoryApp_InitWindows(BattleFactoryApp *app);
static void BattleFactoryApp_CloseWindow(Window *window);
static void BattleFactoryApp_OpenRentalMenu(BattleFactoryApp *app);
static void BattleFactoryApp_CloseRentalMenu(BattleFactoryApp *app);
static void BattleFactoryApp_PrintRentalMenu(BattleFactoryApp *app);
static void BattleFactoryApp_OpenSwapOutMenu(BattleFactoryApp *app);
static void BattleFactoryApp_CloseSwapOutMenu(BattleFactoryApp *app);
static void BattleFactoryApp_RedrawSwapOutButtons(BattleFactoryApp *app);
static void BattleFactoryApp_ClearSwapOutMenu(BattleFactoryApp *app);
static void BattleFactoryApp_PrintSwapOutMenu(BattleFactoryApp *app);
static void BattleFactoryApp_OpenSwapInMenu(BattleFactoryApp *app);
static void BattleFactoryApp_CloseSwapInMenu(BattleFactoryApp *app);
static void BattleFactoryApp_RedrawSwapInButtons(BattleFactoryApp *app);
static void BattleFactoryApp_ClearSwapInMenu(BattleFactoryApp *app);
static void BattleFactoryApp_PrintSwapInMenu(BattleFactoryApp *app);
static void BattleFactoryApp_CreateYesNo(YesNoPrompt **prompt);
static void BattleFactoryApp_DestroyYesNo(YesNoPrompt *prompt);
static void BattleFactoryApp_InitYesNo(YesNoPrompt *prompt, BgConfig *bgConfig);
static void BattleFactoryApp_UpdatePreviewPokepic(BattleFactoryApp *app);
static void BattleFactory_GetPokepicPos(int slot, int numPokepics, int *x, int *y);
static void BattleFactoryApp_GetCursorPokepicPos(BattleFactoryApp *app, int *x, int *y);
static void BattleFactoryApp_MovePicCursor(BattleFactoryApp *app);
static int BattleFactoryApp_GetCursorSlot(BattleFactoryApp *app);
static void BattleFactoryApp_UpdateSwapOutPicCursor(BattleFactoryApp *app);
static void BattleFactoryApp_UpdateSwapInPicCursor(BattleFactoryApp *app);
static void BattleFactoryApp_CreatePartyPokepics(BattleFactoryApp *app, int hFlip);
static void BattleFactoryApp_ReplacePokepic(BattleFactoryApp *app, int slot, int monIndex, int hFlip);
static void BattleFactoryApp_ReplacePokepicFromOther(BattleFactoryApp *app, int slot, int monIndex, int hFlip);
static void BattleFactoryApp_CreateSlotMarkers(BattleFactoryApp *app);
static void BattleFactoryApp_DeleteSlotMarkers(BattleFactoryApp *app);
static void BattleFactoryApp_UpdateSlotMarkers(BattleFactoryApp *app);
static void BattleFactoryApp_SetSlotMarkers(BattleFactoryApp *app, int highlightCursor);
static void BattleFactoryApp_CreateOrderSprites(BattleFactoryApp *app);
static void BattleFactoryApp_DeleteOrderSprites(BattleFactoryApp *app);
static void BattleFactoryApp_UpdateOrderSprites(BattleFactoryApp *app);
static void BattleFactoryApp_ShowMonIcon(BattleFactoryApp *app, Party *party, u32 index, u32 limit);
static void BattleFactoryApp_PrintRentalNames(BattleFactoryApp *app, u32 target, u32 limit);
static void BattleFactoryApp_PrintPartyNames(BattleFactoryApp *app, Party *party);
static void BattleFactoryApp_PrintSwapNames(BattleFactoryApp *app, u32 swapIn);
static void BattleFactoryApp_PrintSwapInName(BattleFactoryApp *app, u32 index);
static void BattleFactoryApp_LoadSubPalette(BattleFactoryApp *app);
static void BattleFactoryApp_UpdatePaletteBlink(BattleFactoryApp *app);
static void BattleFactoryApp_StartButtonFlash(BattleFactoryApp *app, u8 x, u8 y, u8 width, u8 height, u8 nextState);
static void BattleFactoryApp_FlashWideButton(BattleFactoryApp *app, u8 x, u8 y, u8 nextState);
static void BattleFactoryApp_FlashNarrowButton(BattleFactoryApp *app, u8 x, u8 y, u8 nextState);
static BOOL BattleFactoryApp_UpdateButtonFlash(BattleFactoryApp *app);
static void BattleFactoryApp_SetBallsTranslucent(BattleFactoryApp *app, int enable);

static const u8 sBattleFactoryAppSummaryPages[4] = { 0, 1, 3, 4 };

static const u16 sBattleFactoryBallPositions_Multi2[2][2] = {
    { 96,  64 },
    { 152, 64 }
};

static const u16 sBattleFactoryBallPositions_Single[3][2] = {
    { 64,  64 },
    { 120, 64 },
    { 176, 64 }
};

static const GraphicsModes sBattleFactoryAppGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const u16 sBattleFactoryBallPositions_Multi[4][2] = {
    { 40,  64 },
    { 96,  64 },
    { 152, 64 },
    { 208, 64 },
};

static const u16 sBattleFactoryBallPositions_Rental[6][2] = {
    { 24,  64 },
    { 64,  64 },
    { 104, 64 },
    { 144, 64 },
    { 184, 64 },
    { 224, 64 },
};

static const BgTemplate sBattleFactoryAppBgTemplate_Sub0 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleFactoryAppBgTemplate_Main1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleFactoryAppBgTemplate_Main2 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x18000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleFactoryAppBgTemplate_Main3 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleFactoryAppBgTemplate_Sub2 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleFactoryAppBgTemplate_Sub1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattleFactoryAppBgTemplate_Sub3 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x18000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const GraphicsBanks sBattleFactoryAppGraphicsBanks = {
    .bg = GX_VRAM_BG_128_B,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_0_A,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

BOOL BattleFactoryApp_Init(OverlayManager *man, int *state) {
    BattleFactoryApp *app;
    BattleFactoryAppArgs *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    BattleFactoryApp_DisableDisplay();
    Heap_Create(HEAP_ID_3, HEAP_ID_100, 0x30000);
    app = OverlayManager_CreateAndGetData(man, sizeof(BattleFactoryApp), HEAP_ID_100);
    memset(app, 0, sizeof(BattleFactoryApp));
    app->vramMan = GF_3DVramMan_Create(HEAP_ID_100, 0, 2, 0, 2, BattleFactoryApp_Setup3D);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_100);
    app->manager = man;
    args = OverlayManager_GetArgs(man);
    app->saveData = args->saveData;
    app->frontierType = args->frontierType;
    app->levelMode = args->levelMode;
    app->screen = args->screen;
    app->party = args->party;
    app->otherParty = args->otherParty;
    app->resultOut = args->result;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->bgAnimTimer = 8;
    if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
        app->numBalls = 6;
        app->grid = BattleFactoryApp_CreateRentalGrid(app, 0);
    } else if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
        app->numBalls = 2;
        app->unk_01A = 3;
        app->unk_01B = 6;
        app->grid = BattleFactoryApp_CreateLinkSwapOutGrid(app, 0);
    } else {
        app->numBalls = 3;
        app->unk_01A = 4;
        app->unk_01B = 5;
        app->grid = BattleFactoryApp_CreateSwapOutGrid(app, 0);
    }
    if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
        app->numPokepics = 2;
    } else {
        app->numPokepics = 3;
    }
    BattleFactoryApp_CreateYesNo(&app->yesNoPrompt);
    BattleFactoryApp_InitGraphics(app);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
        FrontierLink_RegisterCommands(app);
    }
    *state = 0;
    return TRUE;
}

BOOL BattleFactoryApp_Main(OverlayManager *man, int *state) {
    BattleFactoryApp *app = OverlayManager_GetData(man);

    if (app->partnerSwapState == TRUE) {
        switch (*state) {
        case FACTORY_STATE_SELECT_SWAP_OUT:
        case FACTORY_STATE_CANCEL_SWAP:
        case FACTORY_STATE_SELECT_SWAP_IN:
        case FACTORY_STATE_CANCEL_SWAP_IN:
            if (app->summaryFading == FALSE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SEND_SWAP);
            }
            break;
        }
    }
    BattleFactoryApp_UpdatePaletteBlink(app);
    switch (*state) {
    case FACTORY_STATE_SLIDE_IN:
        if (BattleFactoryApp_SlideIn(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SHOW_SCREEN);
        }
        break;
    case FACTORY_STATE_SUMMARY:
        if (BattleFactoryApp_WaitSummary(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SHOW_SCREEN);
        } else {
            return FALSE;
        }
    case FACTORY_STATE_SHOW_SCREEN:
        if (BattleFactoryApp_ShowScreen(app) == TRUE) {
            if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_RENTALS);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_OUT);
            }
        }
        break;
    case FACTORY_STATE_SELECT_RENTALS:
        BattleFactoryApp_PrintPartnerRentals(app);
        if (BattleFactoryApp_SelectRentals(app) == TRUE) {
            if (app->summaryOpen == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SUMMARY);
            } else if (app->numPicked == BattleFactory_GetRentalCount(app->frontierType)) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_CONFIRM_RENTALS);
            } else if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_RENTALS);
            }
        }
        break;
    case FACTORY_STATE_CONFIRM_RENTALS:
        BattleFactoryApp_PrintPartnerRentals(app);
        if (BattleFactoryApp_ConfirmRentals(app) == TRUE) {
            if (app->numPicked == BattleFactory_GetRentalCount(app->frontierType)) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_CANCEL_LAST_RENTAL);
            }
        }
        break;
    case FACTORY_STATE_CANCEL_LAST_RENTAL:
        if (BattleFactoryApp_CancelLastRental(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_RENTALS);
        }
        break;
    case FACTORY_STATE_SELECT_SWAP_OUT:
        if (BattleFactoryApp_SelectSwapOut(app) == TRUE) {
            if (app->summaryOpen == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SUMMARY);
            } else if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetCancelRequested(app, FALSE);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_CANCEL_SWAP);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SWITCH_PAGE);
            }
        }
        break;
    case FACTORY_STATE_CANCEL_SWAP:
        if (BattleFactoryApp_ConfirmCancelSwap(app) == TRUE) {
            if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetCancelRequested(app, FALSE);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_OUT);
            } else if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->swapAccepted = FALSE;
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SEND_SWAP);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            }
        }
        break;
    case FACTORY_STATE_SELECT_SWAP_IN:
        if (BattleFactoryApp_SelectSwapIn(app) == TRUE) {
            if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetCancelRequested(app, FALSE);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_CANCEL_SWAP_IN);
            } else if (app->numPicked == 0) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SWITCH_PAGE);
            } else if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SEND_SWAP);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            }
        }
        break;
    case FACTORY_STATE_CANCEL_SWAP_IN:
        if (BattleFactoryApp_ConfirmCancelSwapIn(app) == TRUE) {
            if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetCancelRequested(app, FALSE);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_IN);
            } else if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->swapAccepted = FALSE;
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SEND_SWAP);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            }
        }
        break;
    case FACTORY_STATE_SWITCH_PAGE:
        if (BattleFactoryApp_SwitchPage(app) == TRUE) {
            if (app->numPicked == 0) {
                BattleFactoryApp_SetupSwapOutScreen(app);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_OUT);
            } else {
                BattleFactoryApp_SetupSwapInScreen(app);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_IN);
            }
        }
        break;
    case FACTORY_STATE_LINK_SEND_SWAP:
        if (BattleFactoryApp_LinkSendSwap(app) == TRUE) {
            if (app->partnerSwapState == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_TRADE_RESULT);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SYNC);
            }
        }
        break;
    case FACTORY_STATE_LINK_SYNC:
        if (BattleFactoryApp_LinkSync(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
        }
        break;
    case FACTORY_STATE_FADE_OUT:
        if (BattleFactoryApp_FadeOut(app) == TRUE) {
            return TRUE;
        }
        break;
    case FACTORY_STATE_LINK_TRADE_RESULT:
        if (BattleFactoryApp_ShowLinkTradeResult(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SYNC);
        }
        break;
    }
    SpriteList_RenderAndAnimateSprites(app->spriteGfx.spriteList);
    BattleFactoryApp_Draw3D(app->pokepicManager);
    return FALSE;
}

BOOL BattleFactoryApp_Exit(OverlayManager *man, int *state) {
#pragma unused(state)
    BattleFactoryApp *app = OverlayManager_GetData(man);
    int i;

    if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0) {
        PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(app->paletteData);
        app->paletteBlinkState = 0xFF;
        return FALSE;
    }
    if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
        for (i = 0; i < BattleFactory_GetRentalCount(app->frontierType); i++) {
            app->resultOut[i] = app->picked[i];
        }
    } else {
        for (i = 0; i < 2; i++) {
            app->resultOut[i] = app->picked[i];
        }
        if (app->swapAccepted == FALSE) {
            app->resultOut[0] = 0xFF;
            app->resultOut[1] = 0xFF;
        }
    }
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    BattleFactoryApp_DestroyYesNo(app->yesNoPrompt);
    BattleFactory_FreeGrid(app->grid);
    BattleFactoryApp_FreeGraphics(app);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_100);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return TRUE;
}

enum {
    FACTORY_SLIDE_IN_START_SYNC,
    FACTORY_SLIDE_IN_WAIT_SYNC,
    FACTORY_SLIDE_IN_FADE_IN,
    FACTORY_SLIDE_IN_WAIT_FADE,
    FACTORY_SLIDE_IN_SCROLL_BALLS,
    FACTORY_SLIDE_IN_WAIT_CENTER_ANIM,
    FACTORY_SLIDE_IN_SHOW_POKEPICS,
};

static BOOL BattleFactoryApp_SlideIn(BattleFactoryApp *app) {
    int i;
    u32 j;

    switch (app->subState) {
    case FACTORY_SLIDE_IN_START_SYNC:
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(237);
        }
        app->subState++;
        break;
    case FACTORY_SLIDE_IN_WAIT_SYNC:
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            if (LinkSync_IsSynced(237) == TRUE) {
                LinkSync_ClearPackets();
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case FACTORY_SLIDE_IN_FADE_IN:
        for (i = 0; i < app->numBalls; i++) {
            BattleFactoryBall_GetPosition(app->balls[i]);
            BattleFactoryBall_MoveOffscreen(app->balls[i]);
        }
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, 264);
        Pokepic_SetAttr(app->pokepics[0], POKEPIC_VANISHED, TRUE);
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 3, HEAP_ID_100);
        app->subState++;
        break;
    case FACTORY_SLIDE_IN_WAIT_FADE:
        if (!IsPaletteFadeFinished()) {
            break;
        }
        PlaySE(SEQ_SE_DP_ELEBETA2);
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SLIDE_IN_SCROLL_BALLS:
        if (BattleFactoryApp_ScrollBallsIn(app) != TRUE) {
            break;
        }
        StopSE(SEQ_SE_DP_ELEBETA2, 0);
        PlaySE(SEQ_SE_DP_KASYA);
        for (i = 0; i < app->numBalls; i++) {
            BattleFactoryBall_SetAnim(app->balls[i], 4);
        }
        BattleFactoryApp_SetCenterAnim(app, 0);
        app->bgAnimTimer = 8;
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SLIDE_IN_WAIT_CENTER_ANIM:
        if (BattleFactorySprite_IsAnimated(app->centerSprite) == TRUE) {
            break;
        }
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SLIDE_IN_SHOW_POKEPICS:
        if (app->timer == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
            if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
                Pokepic_SetAttr(app->pokepics[0], POKEPIC_VANISHED, FALSE);
                Pokepic_StartPaletteFadeAll(app->pokepicManager, 16, 0, 1, 0xFFFF);
            } else {
                for (j = 0; j < app->numPokepics; j++) {
                    Pokepic_SetAttr(app->pokepics[j], POKEPIC_VANISHED, FALSE);
                }
                Pokepic_StartPaletteFadeAll(app->pokepicManager, 16, 0, 1, 0xFFFF);
            }
        }
        Pokepic_ResumePaletteFade(app->pokepics[0]);
        app->timer++;
        if (app->timer >= 2) {
            app->timer = 0;
            BattleFactoryApp_InitWindows(app);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

enum {
    FACTORY_SHOW_SCREEN_SETUP,
    FACTORY_SHOW_SCREEN_WAIT_FADE,
};

static BOOL BattleFactoryApp_ShowScreen(BattleFactoryApp *app) {
    switch (app->subState) {
    case FACTORY_SHOW_SCREEN_SETUP:
        if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
            BattleFactoryApp_SetupRentalScreen(app);
        } else {
            BattleFactoryApp_SetupSwapOutScreen(app);
        }
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            BattleFactoryApp_PrintPartnerName(app, &app->windows[1], 0, 0, 0);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        }
        if (app->shown == TRUE) {
            BeginNormalPaletteFade(0, 1, 1, 0, 6, 3, HEAP_ID_100);
        }
        app->shown = TRUE;
        app->subState++;
        break;
    case FACTORY_SHOW_SCREEN_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void BattleFactoryApp_SetupRentalScreen(BattleFactoryApp *app) {
    int x;
    int y;
    int slot;
    int maxPartnerMons = BattleFactory_GetPartySize(app->frontierType);
    int i;

    BattleFactoryApp_SetCancelRequested(app, FALSE);
    BattleFactoryApp_PrintRentalMenu(app);
    BattleFactory_GetGridCursorPos(app->grid, app->cursorPos, (u32 *)&x, (u32 *)&y);
    app->cursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 2, 0, 0, x, y);
    BattleFactory_GetGridCursorPos(app->grid, 6, (u32 *)&x, (u32 *)&y);
    app->buttonCursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 3, 0, 0, x, y);
    BattleFactoryApp_GetCursorPokepicPos(app, &x, &y);
    app->picCursor = BattleFactorySprite_Create(&app->spriteGfx, 1, 0, 0, 0, x, y);
    BattleFactoryApp_CreateSlotMarkers(app);
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_CreateOrderSprites(app);
    BattleFactoryApp_UpdateOrderSprites(app);
    app->monIcons[0] = BattleFactorySprite_CreateSub(&app->spriteGfx, 4, 0, 0, 0, 80, 12);
    app->monIcons[1] = BattleFactorySprite_CreateSub(&app->spriteGfx, 5, 0, 0, 0, 80, 12);
    BattleFactoryApp_ShowMonIcon(app, app->party, app->cursorPos, 6);
    BattleFactoryApp_PrintRentalNames(app, app->cursorPos, 6);
    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
    BattleFactoryApp_PrintPlayerName(app, &app->windows[0], 0, 0, 0);
    for (i = 0; i < app->numPicked; i++) {
        BattleFactoryBall_Select(app->balls[app->picked[i]]);
        BattleFactoryBall_SetPaletteOverride(app->balls[app->picked[i]], 0);
        BattleFactoryBall_SetAnim(app->balls[app->picked[i]], 1);
    }
    BattleFactoryApp_DrawMessageFrame(&app->windows[7], Options_GetFrame(app->options));
    BattleFactoryApp_BufferNumber(app, 0, app->numPicked + 1);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    if (app->summaryOpen == TRUE) {
        slot = BattleFactoryApp_GetCursorSlot(app);
        for (i = 0; i < app->numPicked; i++) {
            Pokepic_SetAttr(app->pokepics[i], POKEPIC_VANISHED, FALSE);
        }
        Pokepic_SetAttr(app->pokepics[slot], POKEPIC_VANISHED, FALSE);
        BattleFactoryApp_ReplacePokepic(app, slot, app->cursorPos, 0);
        BattleFactoryApp_UpdateSlotMarkers(app);
        BattleFactory_GetPokepicPos(slot, app->numPokepics, &x, &y);
        BattleFactorySprite_SetPosition(app->picCursor, x, y);
        BattleFactoryApp_SetCenterAnim(app, 0xFF);
        BattleFactoryApp_OpenRentalMenu(app);
        if (app->numPartnerMons != 0) {
            for (i = 0; i < maxPartnerMons; i++) {
                FillWindowPixelBuffer(&app->windows[i + 5], 0);
                if (i < app->numPartnerMons) {
                    BattleFactoryApp_PrintPartnerMonName(app, &app->windows[i + 5], 0, 0, 15, 2, 0, 0, app->partnerSpecies[i], app->partnerGender[i]);
                }
                ScheduleWindowCopyToVram(&app->windows[i + 5]);
            }
        }
    } else {
        BattleFactoryApp_CloseRentalMenu(app);
    }
}

static void BattleFactoryApp_CreateRentalBalls(BattleFactoryApp *app) {
    int i;

    for (i = 0; i < app->numBalls; i++) {
        app->balls[i] = BattleFactoryApp_CreateBall(app, i);
        BattleFactoryBall_SetVisible(app->balls[i], TRUE);
    }
}

static void BattleFactoryApp_CreateRentalPokepics(BattleFactoryApp *app) {
    BattleFactoryApp_CreatePartyPokepics(app, 0);
}

static void BattleFactoryApp_SetupSwapOutScreen(BattleFactoryApp *app) {
    u32 x;
    u32 y;
    u32 i;

    BattleFactoryApp_SetCancelRequested(app, FALSE);
    app->grid = BattleFactoryApp_RecreateSwapOutGrid(app, app->grid);
    BattleFactoryApp_PrintSwapOutMenu(app);
    BattleFactory_GetGridCursorPos(app->grid, app->cursorPos, &x, &y);
    app->cursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 2, 0, 0, x, y);
    app->picCursor = BattleFactorySprite_Create(&app->spriteGfx, 1, 0, 0, 0, 0, 0);
    BattleFactoryApp_UpdateSwapOutPicCursor(app);
    BattleFactory_GetGridCursorPos(app->grid, 4, &x, &y);
    app->buttonCursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 3, 0, 0, x, y);
    BattleFactoryApp_CreateSlotMarkers(app);
    BattleFactoryApp_SetSlotMarkers(app, FALSE);
    BattleFactoryApp_CreateOrderSprites(app);
    app->monIcons[0] = BattleFactorySprite_CreateSub(&app->spriteGfx, 4, 0, 0, 0, 80, 12);
    app->monIcons[1] = BattleFactorySprite_CreateSub(&app->spriteGfx, 5, 0, 0, 0, 80, 12);
    BattleFactoryApp_ShowMonIcon(app, app->party, app->cursorPos, 4);
    BattleFactoryApp_PrintPartyNames(app, app->party);
    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
    BattleFactoryApp_PrintPlayerName(app, &app->windows[0], 0, 0, 0);
    BattleFactoryApp_DrawMessageFrame(&app->windows[7], Options_GetFrame(app->options));
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00009);
    if (app->summaryOpen == TRUE) {
        for (i = 0; i < app->numPokepics; i++) {
            Pokepic_SetAttr(app->pokepics[i], POKEPIC_VANISHED, FALSE);
        }
        BattleFactory_GetPokepicPos(app->cursorPos, app->numPokepics, (int *)&x, (int *)&y);
        BattleFactorySprite_SetPosition(app->picCursor, x, y);
        BattleFactoryApp_SetCenterAnim(app, 0xFF);
        BattleFactoryApp_OpenSwapOutMenu(app);
        BattleFactorySprite_SetPalette(app->cursor, 1);
    } else {
        BattleFactoryApp_CloseSwapOutMenu(app);
        BattleFactorySprite_SetPalette(app->cursor, 0);
    }
}

static void BattleFactoryApp_SetupSwapInScreen(BattleFactoryApp *app) {
    u32 x;
    u32 y;

    app->grid = BattleFactoryApp_CreateSwapInGrid(app, app->grid);
    BattleFactoryApp_PrintSwapInMenu(app);
    BattleFactory_GetGridCursorPos(app->grid, 0, &x, &y);
    app->cursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 2, 0, 0, x, y);
    BattleFactory_GetPokepicPos(app->cursorPos, app->numPokepics, (int *)&x, (int *)&y);
    app->picCursor = BattleFactorySprite_Create(&app->spriteGfx, 1, 0, 0, 0, x, y);
    BattleFactory_GetGridCursorPos(app->grid, 6, &x, &y);
    app->buttonCursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 3, 0, 0, x, y);
    BattleFactoryApp_CreateSlotMarkers(app);
    BattleFactoryApp_SetSlotMarkers(app, TRUE);
    BattleFactoryApp_CreateOrderSprites(app);
    app->monIcons[0] = BattleFactorySprite_CreateSub(&app->spriteGfx, 4, 0, 0, 0, 80, 12);
    app->monIcons[1] = BattleFactorySprite_CreateSub(&app->spriteGfx, 5, 0, 0, 0, 80, 12);
    BattleFactoryApp_ShowMonIcon(app, app->otherParty, 0, 4);
    BattleFactoryApp_PrintSwapNames(app, 0);
    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
    BattleFactoryApp_PrintPlayerName(app, &app->windows[0], 0, 0, 0);
    app->cursorPos = 0;
    BattleFactoryApp_CloseSwapInMenu(app);
}

static void BattleFactoryApp_CreateSwapBalls(BattleFactoryApp *app) {
    int i;

    for (i = 0; i < app->numBalls; i++) {
        app->balls[i] = BattleFactoryApp_CreateBall(app, i);
        BattleFactoryBall_SetVisible(app->balls[i], TRUE);
        if (!app->swapOutPicked) {
            BattleFactoryBall_Select(app->balls[i]);
            BattleFactoryBall_SetPaletteOverride(app->balls[i], 0);
            BattleFactoryBall_SetAnim(app->balls[i], 0);
        }
    }
}

static void BattleFactoryApp_CreateSwapPokepics(BattleFactoryApp *app) {
    u32 i;

    BattleFactoryApp_CreatePartyPokepics(app, 1);
    for (i = 0; i < app->numPokepics; i++) {
        BattleFactoryApp_ReplacePokepic(app, i, i, 1);
        Pokepic_SetAttr(app->pokepics[i], POKEPIC_VANISHED, TRUE);
    }
}

static BOOL BattleFactoryApp_WaitSummary(BattleFactoryApp *app) {
    if (app->subState == 0 && OverlayManager_Run(app->summaryApp) == TRUE) {
        u8 partySlot = app->summaryArgs->partySlot;
        app->summaryResult = partySlot;
        app->cursorPos = app->summaryArgs->partySlot;
        Heap_Free(app->summaryArgs);
        Heap_Free(app->summaryApp);
        app->summaryApp = NULL;
        BattleFactoryApp_ResumeAfterSummary(app);
        app->summaryFading = FALSE;
        app->paletteBlinkState = 0;
        return TRUE;
    }
    return FALSE;
}

enum {
    FACTORY_RENTALS_INIT,
    FACTORY_RENTALS_INPUT,
    FACTORY_RENTALS_BUTTON_FLASH,
    FACTORY_RENTALS_OPEN_SUMMARY,
    FACTORY_RENTALS_WAIT_SUMMARY_CLOSED,
    FACTORY_RENTALS_WAIT_FADE,
    FACTORY_RENTALS_FADE_TO_SUMMARY,
    FACTORY_RENTALS_TOGGLE_RENTAL,
    FACTORY_RENTALS_CLOSE_MENU,
};

static BOOL BattleFactoryApp_SelectRentals(BattleFactoryApp *app) {
    u32 input;

    switch (app->subState) {
    case FACTORY_RENTALS_INIT:
        app->subState = FACTORY_RENTALS_INPUT;
        app->summaryOpen = FALSE;
        break;
    case FACTORY_RENTALS_INPUT:
        input = GridInputHandler_HandleInput_NoHold(app->grid);
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorPos = input;
            BattleFactoryApp_UpdatePreviewPokepic(app);
            BattleFactoryApp_ShowMonIcon(app, app->party, app->cursorPos, 6);
            BattleFactoryApp_PrintRentalNames(app, app->cursorPos, 6);
            BattleFactoryApp_OpenRentalMenu(app);
            break;
        case 6:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 2, 12, FACTORY_RENTALS_FADE_TO_SUMMARY);
            app->subState = FACTORY_RENTALS_BUTTON_FLASH;
            break;
        case 7:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 18, 12, FACTORY_RENTALS_TOGGLE_RENTAL);
            app->subState = FACTORY_RENTALS_BUTTON_FLASH;
            break;
        case 8:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 10, 15, FACTORY_RENTALS_CLOSE_MENU);
            app->subState = FACTORY_RENTALS_BUTTON_FLASH;
            break;
        case GRID_MENU_CURSOR_MOVE:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_UpdatePreviewPokepic(app);
            BattleFactoryApp_ShowMonIcon(app, app->party, GridInputHandler_GetNextInput(app->grid), 6);
            BattleFactoryApp_PrintRentalNames(app, GridInputHandler_GetNextInput(app->grid), 6);
            break;
        case GRID_MENU_CANCEL:
            if ((u32)GridInputHandler_GetNextInput(app->grid) >= 6) {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_CloseRentalMenu(app);
                return TRUE;
            }
            if (app->numPicked != 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_UndoLastRental(app);
                if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                    BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_RENTALS, 0);
                }
                return TRUE;
            }
            break;
        }
        break;
    case FACTORY_RENTALS_BUTTON_FLASH:
        BattleFactoryApp_UpdateButtonFlash(app);
        break;
    case FACTORY_RENTALS_OPEN_SUMMARY:
        if (IsPaletteFadeFinished() == TRUE) {
            if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0) {
                PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(app->paletteData);
                app->paletteBlinkState = 0xFF;
            } else {
                BattleFactoryApp_CreateSummaryArgs(app);
                BattleFactoryApp_FreeGraphics(app);
                app->summaryApp = OverlayManager_New(&gOverlayTemplate_PokemonSummary, app->summaryArgs, HEAP_ID_100);
                app->summaryOpen = TRUE;
                return TRUE;
            }
        }
        break;
    case FACTORY_RENTALS_WAIT_SUMMARY_CLOSED:
        if (app->summaryApp == NULL) {
            return TRUE;
        }
        break;
    case FACTORY_RENTALS_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            app->subState = FACTORY_RENTALS_INPUT;
        }
        break;
    case FACTORY_RENTALS_FADE_TO_SUMMARY:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_100);
        app->summaryFading = TRUE;
        app->subState = FACTORY_RENTALS_OPEN_SUMMARY;
        break;
    case FACTORY_RENTALS_TOGGLE_RENTAL:
        BattleFactoryApp_CloseRentalMenu(app);
        if (BattleFactoryBall_IsSelected(app->balls[app->cursorPos]) == 0) {
            BattleFactoryApp_AddRental(app);
        } else {
            BattleFactoryApp_RemoveRental(app);
        }
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_RENTALS, 0);
        }
        return TRUE;
    case FACTORY_RENTALS_CLOSE_MENU:
        BattleFactoryApp_CloseRentalMenu(app);
        return TRUE;
    }
    return FALSE;
}

static void BattleFactoryApp_UndoLastRental(BattleFactoryApp *app) {
    Pokepic_SetAttr(app->pokepics[app->numPicked], POKEPIC_VANISHED, TRUE);
    FillWindowPixelBuffer(&app->windows[app->numPicked + 2], 0);
    ScheduleWindowCopyToVram(&app->windows[app->numPicked + 2]);
    app->numPicked--;
    BattleFactoryApp_BufferNumber(app, 0, app->numPicked + 1);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    BattleFactoryBall_Deselect(app->balls[app->picked[app->numPicked]]);
    BattleFactoryBall_SetPaletteOverride(app->balls[app->picked[app->numPicked]], 1);
    BattleFactoryBall_SetAnim(app->balls[app->picked[app->numPicked]], 0);
    app->picked[app->numPicked] = 0;
    BattleFactoryApp_UpdatePreviewPokepic(app);
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_UpdateOrderSprites(app);
    BattleFactoryApp_PrintRentalNames(app, GridInputHandler_GetNextInput(app->grid), 6);
}

static void BattleFactoryApp_AddRental(BattleFactoryApp *app) {
    u32 numPicked;

    BattleFactoryBall_Select(app->balls[app->cursorPos]);
    BattleFactoryBall_SetPaletteOverride(app->balls[app->cursorPos], 0);
    BattleFactoryBall_SetAnim(app->balls[app->cursorPos], 1);
    app->picked[app->numPicked] = app->cursorPos;
    app->numPicked++;
    numPicked = app->numPicked;
    if (numPicked != BattleFactory_GetRentalCount(app->frontierType)) {
        BattleFactoryApp_BufferNumber(app, 0, numPicked + 1);
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    }
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_UpdateOrderSprites(app);
}

static void BattleFactoryApp_RemoveRental(BattleFactoryApp *app) {
    int count;
    int i;

    BattleFactoryBall_Deselect(app->balls[app->cursorPos]);
    BattleFactoryBall_SetPaletteOverride(app->balls[app->cursorPos], 1);
    BattleFactoryBall_SetAnim(app->balls[app->cursorPos], 0);
    if (app->numPicked >= 2 && app->picked[0] == app->cursorPos) {
        app->picked[0] = app->picked[1];
        BattleFactoryApp_ReplacePokepic(app, 0, app->picked[0], 0);
        Pokepic_SetAttr(app->pokepics[0], POKEPIC_VANISHED, FALSE);
    }
    app->numPicked--;
    app->picked[app->numPicked] = 0;
    count = BattleFactory_GetRentalCount(app->frontierType);
    for (i = 0; i < count; i++) {
        FillWindowPixelBuffer(&app->windows[i + 2], 0);
    }
    BattleFactoryApp_BufferNumber(app, 0, app->numPicked + 1);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    BattleFactoryApp_UpdatePreviewPokepic(app);
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_UpdateOrderSprites(app);
    BattleFactoryApp_PrintRentalNames(app, app->cursorPos, 6);
}

enum {
    FACTORY_CONFIRM_HIDE_CURSORS,
    FACTORY_CONFIRM_DELAY_1,
    FACTORY_CONFIRM_DELAY_2,
    FACTORY_CONFIRM_DELAY_3,
    FACTORY_CONFIRM_SHOW_PROMPT,
    FACTORY_CONFIRM_HANDLE_INPUT,
    FACTORY_CONFIRM_LINK_SEND,
    FACTORY_CONFIRM_WAIT_SYNC,
};

static BOOL BattleFactoryApp_ConfirmRentals(BattleFactoryApp *app) {
    int result = BattleFactory_GetPartySize(app->frontierType);

    switch (app->subState) {
    case FACTORY_CONFIRM_HIDE_CURSORS:
        BattleFactorySprite_SetVisible(app->cursor, FALSE);
        BattleFactorySprite_SetVisible(app->picCursor, FALSE);
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_CONFIRM_DELAY_1:
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_CONFIRM_DELAY_2:
        app->subState++;
        break;
    case FACTORY_CONFIRM_DELAY_3:
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_CONFIRM_SHOW_PROMPT:
        BattleFactoryApp_BufferNumber(app, 0, result);
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00001);
        BattleFactoryApp_InitYesNo(app->yesNoPrompt, app->bgConfig);
        BattleFactoryApp_SetBallsTranslucent(app, TRUE);
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_CONFIRM_HANDLE_INPUT:
        result = YesNoPrompt_HandleInput(app->yesNoPrompt);
        switch (result) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactorySprite_SetVisible(app->cursor, TRUE);
            BattleFactorySprite_SetVisible(app->picCursor, TRUE);
            app->numPicked--;
            app->picked[app->numPicked] = 0;
            if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_RENTALS, 0);
            }
            return TRUE;
        }
        break;
    case FACTORY_CONFIRM_LINK_SEND:
        if (BattleFactory_IsMultiType(app->frontierType) == FALSE) {
            return TRUE;
        }
        if (BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_RENTALS, 0) == TRUE) {
            app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00002);
            LinkSync_ClearPackets();
            LinkSync_StartSync(164);
            app->subState++;
        }
        break;
    case FACTORY_CONFIRM_WAIT_SYNC:
        if (LinkSync_IsSynced(164) == TRUE) {
            LinkSync_ClearPackets();
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL BattleFactoryApp_CancelLastRental(BattleFactoryApp *app) {
    BattleFactoryApp_DeselectCursorRental(app);
    return TRUE;
}

static void BattleFactoryApp_DeselectCursorRental(BattleFactoryApp *app) {
    BattleFactory_GetPartySize(app->frontierType);
    BattleFactoryBall_Deselect(app->balls[app->cursorPos]);
    BattleFactoryBall_SetPaletteOverride(app->balls[app->cursorPos], 1);
    BattleFactoryBall_SetAnim(app->balls[app->cursorPos], 0);
    BattleFactoryApp_BufferNumber(app, 0, app->numPicked + 1);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_UpdateOrderSprites(app);
}

enum {
    FACTORY_SWAP_OUT_INIT,
    FACTORY_SWAP_OUT_INPUT,
    FACTORY_SWAP_OUT_BUTTON_FLASH,
    FACTORY_SWAP_OUT_OPEN_SUMMARY,
    FACTORY_SWAP_OUT_WAIT_SUMMARY_CLOSED,
    FACTORY_SWAP_OUT_WAIT_FADE,
    FACTORY_SWAP_OUT_CANCEL,
    FACTORY_SWAP_OUT_FADE_TO_SUMMARY,
    FACTORY_SWAP_OUT_PICK,
    FACTORY_SWAP_OUT_CLOSE_MENU,
};

static BOOL BattleFactoryApp_SelectSwapOut(BattleFactoryApp *app) {
    u32 input;

    switch (app->subState) {
    case FACTORY_SWAP_OUT_INIT:
        BattleFactoryApp_SetCancelRequested(app, FALSE);
        app->subState = FACTORY_SWAP_OUT_INPUT;
        app->summaryOpen = FALSE;
        break;
    case FACTORY_SWAP_OUT_INPUT:
        input = GridInputHandler_HandleInput_NoHold(app->grid);
        switch (input) {
        case 0:
        case 1:
        case 2:
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorPos = input;
            BattleFactoryApp_ShowMonIcon(app, app->party, app->cursorPos, 4);
            BattleFactoryApp_UpdateSwapOutPicCursor(app);
            BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
            BattleFactoryApp_OpenSwapOutMenu(app);
            BattleFactorySprite_SetPalette(app->cursor, 1);
            break;
        case 4:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 2, 12, FACTORY_SWAP_OUT_FADE_TO_SUMMARY);
            app->subState = FACTORY_SWAP_OUT_BUTTON_FLASH;
            break;
        case 5:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 18, 12, FACTORY_SWAP_OUT_PICK);
            app->subState = FACTORY_SWAP_OUT_BUTTON_FLASH;
            break;
        case 6:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 10, 15, FACTORY_SWAP_OUT_CLOSE_MENU);
            app->subState = FACTORY_SWAP_OUT_BUTTON_FLASH;
            break;
        case 3:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
            BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            BgCellManager_EraseCell(app->bgCellManager, 0);
            BattleFactoryApp_UpdateSwapOutPicCursor(app);
            BattleFactoryApp_FlashNarrowButton(app, 23, 15, FACTORY_SWAP_OUT_CANCEL);
            app->subState = FACTORY_SWAP_OUT_BUTTON_FLASH;
            break;
        case GRID_MENU_CURSOR_MOVE:
            PlaySE(SEQ_SE_DP_SELECT);
            {
                u32 next = GridInputHandler_GetNextInput(app->grid);
                if (next == 3) {
                    BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
                    BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
                    ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
                    BgCellManager_EraseCell(app->bgCellManager, 0);
                } else {
                    BattleFactoryApp_ShowMonIcon(app, app->party, next, 3);
                    ScheduleWindowCopyToVram(&app->windows[8]);
                    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
                }
                BattleFactoryApp_UpdateSwapOutPicCursor(app);
            }
            break;
        case GRID_MENU_CANCEL:
            if ((u32)GridInputHandler_GetNextInput(app->grid) >= 4) {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_CloseSwapOutMenu(app);
                BattleFactorySprite_SetPalette(app->cursor, 0);
            } else {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_ClearSwapOutMenu(app);
                BattleFactorySprite_SetPalette(app->cursor, 1);
                BattleFactoryApp_SetCancelRequested(app, TRUE);
                return TRUE;
            }
            break;
        }
        break;
    case FACTORY_SWAP_OUT_BUTTON_FLASH:
        BattleFactoryApp_UpdateButtonFlash(app);
        break;
    case FACTORY_SWAP_OUT_OPEN_SUMMARY:
        if (IsPaletteFadeFinished() == TRUE) {
            if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0) {
                PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(app->paletteData);
                app->paletteBlinkState = 0xFF;
            } else {
                BattleFactoryApp_CreateSummaryArgs(app);
                BattleFactoryApp_FreeGraphics(app);
                app->summaryApp = OverlayManager_New(&gOverlayTemplate_PokemonSummary, app->summaryArgs, HEAP_ID_100);
                app->summaryOpen = TRUE;
                return TRUE;
            }
        }
        break;
    case FACTORY_SWAP_OUT_WAIT_SUMMARY_CLOSED:
        if (app->summaryApp == NULL) {
            return TRUE;
        }
        break;
    case FACTORY_SWAP_OUT_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            app->subState = FACTORY_SWAP_OUT_INPUT;
        }
        break;
    case FACTORY_SWAP_OUT_CANCEL:
        BattleFactoryApp_ClearSwapOutMenu(app);
        BattleFactoryApp_SetCancelRequested(app, TRUE);
        return TRUE;
    case FACTORY_SWAP_OUT_FADE_TO_SUMMARY:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_100);
        app->summaryFading = TRUE;
        app->subState = FACTORY_SWAP_OUT_OPEN_SUMMARY;
        break;
    case FACTORY_SWAP_OUT_PICK:
        BattleFactoryApp_PickSwapOut(app);
        BattleFactoryApp_CloseSwapOutMenu(app);
        BattleFactoryApp_ClearSwapOutMenu(app);
        return TRUE;
    case FACTORY_SWAP_OUT_CLOSE_MENU:
        BattleFactoryApp_CloseSwapOutMenu(app);
        BattleFactorySprite_SetPalette(app->cursor, 0);
        app->subState = FACTORY_SWAP_OUT_INPUT;
        break;
    }
    return FALSE;
}

static void BattleFactoryApp_PickSwapOut(BattleFactoryApp *app) {
    app->picked[app->numPicked] = app->cursorPos;
    app->numPicked++;
    app->swapOutPicked = TRUE;
}

enum {
    FACTORY_CANCEL_SWAP_SHOW_PROMPT,
    FACTORY_CANCEL_SWAP_HANDLE_INPUT,
    FACTORY_CANCEL_SWAP_DONE,
};

static BOOL BattleFactoryApp_ConfirmCancelSwap(BattleFactoryApp *app) {
    switch (app->subState) {
    case FACTORY_CANCEL_SWAP_SHOW_PROMPT:
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00010);
        BattleFactoryApp_InitYesNo(app->yesNoPrompt, app->bgConfig);
        BattleFactoryApp_SetBallsTranslucent(app, TRUE);
        app->subState++;
        break;
    case FACTORY_CANCEL_SWAP_HANDLE_INPUT:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00002);
            }
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactoryApp_ResumeSwapOut(app);
            BattleFactorySprite_SetPalette(app->cursor, 0);
            app->subState++;
            break;
        }
        break;
    case FACTORY_CANCEL_SWAP_DONE:
        return TRUE;
    }
    return FALSE;
}

static void BattleFactoryApp_ResumeSwapOut(BattleFactoryApp *app) {
    BattleFactoryApp_RedrawSwapOutButtons(app);
    BattleFactoryApp_SetCancelRequested(app, TRUE);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00009);
}

enum {
    FACTORY_SWAP_IN_INIT,
    FACTORY_SWAP_IN_INPUT,
    FACTORY_SWAP_IN_HANDLE_YES_NO,
    FACTORY_SWAP_IN_DONE,
    FACTORY_SWAP_IN_BUTTON_FLASH,
    FACTORY_SWAP_IN_BACK,
    FACTORY_SWAP_IN_CANCEL,
    FACTORY_SWAP_IN_SHOW_PROMPT,
    FACTORY_SWAP_IN_CLOSE_MENU,
};

static BOOL BattleFactoryApp_SelectSwapIn(BattleFactoryApp *app) {
    u32 input;

    switch (app->subState) {
    case FACTORY_SWAP_IN_INIT:
        BattleFactoryApp_DrawMessageFrame(&app->windows[7], Options_GetFrame(app->options));
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00013);
        app->subState = FACTORY_SWAP_IN_INPUT;
        break;
    case FACTORY_SWAP_IN_INPUT:
        input = GridInputHandler_HandleInput_NoHold(app->grid);
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorPos = input;
            BattleFactoryApp_ShowMonIcon(app, app->otherParty, app->cursorPos, 4);
            BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
            BattleFactoryApp_UpdateSwapInPicCursor(app);
            BattleFactoryApp_PrintSwapInName(app, app->cursorPos);
            BattleFactoryApp_OpenSwapInMenu(app);
            BattleFactorySprite_SetPalette(app->cursor, 1);
            break;
        case 4:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
            BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
            Pokepic_SetAttr(app->pokepics[app->picked[0]], POKEPIC_VANISHED, TRUE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[app->picked[0] + 2]);
            BgCellManager_EraseCell(app->bgCellManager, 0);
            BattleFactoryApp_FlashNarrowButton(app, 2, 15, FACTORY_SWAP_IN_BACK);
            app->subState = FACTORY_SWAP_IN_BUTTON_FLASH;
            break;
        case 5:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
            BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
            Pokepic_SetAttr(app->pokepics[app->picked[0]], POKEPIC_VANISHED, TRUE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[app->picked[0] + 2]);
            BgCellManager_EraseCell(app->bgCellManager, 0);
            BattleFactoryApp_FlashNarrowButton(app, 23, 15, FACTORY_SWAP_IN_CANCEL);
            app->subState = FACTORY_SWAP_IN_BUTTON_FLASH;
            break;
        case 6:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 2, 15, FACTORY_SWAP_IN_SHOW_PROMPT);
            app->subState = FACTORY_SWAP_IN_BUTTON_FLASH;
            break;
        case 7:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 18, 15, FACTORY_SWAP_IN_CLOSE_MENU);
            app->subState = FACTORY_SWAP_IN_BUTTON_FLASH;
            break;
        case GRID_MENU_CURSOR_MOVE:
            PlaySE(SEQ_SE_DP_SELECT);
            input = GridInputHandler_GetNextInput(app->grid);
            if ((u32)(input - 4) <= 1) {
                BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
                BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
                Pokepic_SetAttr(app->pokepics[app->picked[0]], POKEPIC_VANISHED, TRUE);
                ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
                ClearWindowTilemapAndScheduleTransfer(&app->windows[app->picked[0] + 2]);
                BgCellManager_EraseCell(app->bgCellManager, 0);
            } else {
                switch (input) {
                case 6:
                case 7:
                    break;
                default:
                    BattleFactoryApp_ShowMonIcon(app, app->otherParty, input, 4);
                    ScheduleWindowCopyToVram(&app->windows[8]);
                    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
                    BattleFactoryApp_UpdateSwapInPicCursor(app);
                    BattleFactoryApp_PrintSwapInName(app, input);
                    break;
                }
            }
            break;
        case GRID_MENU_CANCEL:
            if ((u32)GridInputHandler_GetNextInput(app->grid) >= 6) {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_CloseSwapInMenu(app);
                BattleFactorySprite_SetPalette(app->cursor, 0);
            } else {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_ClearSwapInMenu(app);
                BattleFactorySprite_SetPalette(app->cursor, 1);
                BattleFactoryApp_SetCancelRequested(app, TRUE);
                return TRUE;
            }
            break;
        }
        break;
    case FACTORY_SWAP_IN_HANDLE_YES_NO:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactoryApp_PickSwapIn(app);
            if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00002);
            }
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactoryApp_ShowSwapInPrompt(app);
            BattleFactoryApp_RedrawSwapInButtons(app);
            BattleFactorySprite_SetPalette(app->cursor, 0);
            app->subState = FACTORY_SWAP_IN_INPUT;
            break;
        }
        break;
    case FACTORY_SWAP_IN_DONE:
        return TRUE;
    case FACTORY_SWAP_IN_BUTTON_FLASH:
        BattleFactoryApp_UpdateButtonFlash(app);
        break;
    case FACTORY_SWAP_IN_BACK:
        BattleFactoryApp_ClearSwapInMenu(app);
        BattleFactoryApp_UnpickSwapOut(app);
        return TRUE;
    case FACTORY_SWAP_IN_CANCEL:
        BattleFactoryApp_ClearSwapInMenu(app);
        BattleFactoryApp_SetCancelRequested(app, TRUE);
        return TRUE;
    case FACTORY_SWAP_IN_SHOW_PROMPT:
        BattleFactoryApp_CloseSwapInMenu(app);
        BattleFactoryApp_ClearSwapInMenu(app);
        BattleFactoryApp_InitYesNo(app->yesNoPrompt, app->bgConfig);
        BattleFactoryApp_SetBallsTranslucent(app, TRUE);
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00014);
        app->subState = FACTORY_SWAP_IN_HANDLE_YES_NO;
        break;
    case FACTORY_SWAP_IN_CLOSE_MENU:
        BattleFactoryApp_CloseSwapInMenu(app);
        BattleFactorySprite_SetPalette(app->cursor, 0);
        app->subState = FACTORY_SWAP_IN_INPUT;
        break;
    }
    return FALSE;
}

static void BattleFactoryApp_UnpickSwapOut(BattleFactoryApp *app) {
    app->swapOutPicked = FALSE;
    app->numPicked--;
    app->picked[0] = 0;
    app->cursorPos = 0;
    GridInputHandler_SetNextInput(app->grid, 0);
}

static void BattleFactoryApp_PickSwapIn(BattleFactoryApp *app) {
    app->swapAccepted = TRUE;
    app->picked[app->numPicked] = app->cursorPos;
    app->numPicked++;
}

static void BattleFactoryApp_ShowSwapInPrompt(BattleFactoryApp *app) {
    u32 frame = Options_GetFrame(app->options);
    BattleFactoryApp_DrawMessageFrame(&app->windows[7], frame);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00013);
}

enum {
    FACTORY_CANCEL_SWAP_IN_SHOW_PROMPT,
    FACTORY_CANCEL_SWAP_IN_HANDLE_INPUT,
    FACTORY_CANCEL_SWAP_IN_DONE,
};

static BOOL BattleFactoryApp_ConfirmCancelSwapIn(BattleFactoryApp *app) {
    switch (app->subState) {
    case FACTORY_CANCEL_SWAP_IN_SHOW_PROMPT:
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00010);
        BattleFactoryApp_InitYesNo(app->yesNoPrompt, app->bgConfig);
        BattleFactoryApp_SetBallsTranslucent(app, TRUE);
        app->subState++;
        break;
    case FACTORY_CANCEL_SWAP_IN_HANDLE_INPUT:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            app->swapAccepted = FALSE;
            if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00002);
            }
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactoryApp_ResumeSwapIn(app);
            BattleFactorySprite_SetPalette(app->cursor, 0);
            app->subState++;
            break;
        }
        break;
    case FACTORY_CANCEL_SWAP_IN_DONE:
        return TRUE;
    }
    return FALSE;
}

static void BattleFactoryApp_ResumeSwapIn(BattleFactoryApp *app) {
    BattleFactoryApp_RedrawSwapInButtons(app);
    BattleFactoryApp_SetCancelRequested(app, TRUE);
}

enum {
    FACTORY_SWITCH_PAGE_CLEAR_SCREEN,
    FACTORY_SWITCH_PAGE_DELETE_SPRITES,
    FACTORY_SWITCH_PAGE_LOAD_BG,
    FACTORY_SWITCH_PAGE_SCROLL_BALLS_OUT,
    FACTORY_SWITCH_PAGE_SCROLL_BALLS_IN,
    FACTORY_SWITCH_PAGE_WAIT_CENTER_ANIM,
    FACTORY_SWITCH_PAGE_SHOW_POKEPICS,
};

static BOOL BattleFactoryApp_SwitchPage(BattleFactoryApp *app) {
    int count;
    int i;

    BattleFactory_GetPartySize(app->frontierType);
    switch (app->subState) {
    case FACTORY_SWITCH_PAGE_CLEAR_SCREEN:
        app->switchingPage = TRUE;
        ClearWindowTilemapAndCopyToVram(&app->windows[1]);
        ClearWindowTilemapAndCopyToVram(&app->windows[5]);
        ClearWindowTilemapAndCopyToVram(&app->windows[6]);
        FillWindowPixelBuffer(&app->windows[0], 0);
        ScheduleWindowCopyToVram(&app->windows[0]);
        ClearWindowTilemapAndCopyToVram(&app->windows[2]);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        ClearWindowTilemapAndCopyToVram(&app->windows[8]);
        BgCellManager_EraseCell(app->bgCellManager, 0);
        ClearFrameAndWindow2(&app->windows[7], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[7]);
        BattleFactoryApp_RemoveWindows(app->windows);
        Pokepic_StartPaletteFadeAll(app->pokepicManager, 0, 16, 0, 0xFFFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SWITCH_PAGE_DELETE_SPRITES:
        app->timer++;
        if (app->timer < 2) {
            break;
        }
        for (i = 0; i < app->numPokepics; i++) {
            Pokepic_Delete(app->pokepics[i]);
        }
        BattleFactorySprite_Delete(app->cursor);
        BattleFactorySprite_Delete(app->buttonCursor);
        BattleFactorySprite_Delete(app->picCursor);
        BattleFactorySprite_Delete(app->monIcons[0]);
        BattleFactorySprite_Delete(app->monIcons[1]);
        BattleFactoryApp_DeleteSlotMarkers(app);
        BattleFactoryApp_DeleteOrderSprites(app);
        BattleFactorySprite_SetAnim(app->centerSprite, 2);
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SWITCH_PAGE_LOAD_BG:
        Pokepic_ResumePaletteFade(app->pokepics[0]);
        if (BattleFactorySprite_IsAnimated(app->centerSprite) == TRUE) {
            break;
        }
        BattleFactorySprite_SetVisible(app->centerSprite, FALSE);
        BattleFactoryApp_LoadBgGfx4(app, GF_BG_LYR_SUB_2);
        BattleFactoryApp_LoadBgGfx92(app, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, app->bgScrollX);
        PlaySE(SEQ_SE_DP_ELEBETA2);
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SWITCH_PAGE_SCROLL_BALLS_OUT:
        if (BattleFactoryApp_ScrollBallsOut(app) != TRUE) {
            break;
        }
        StopSE(SEQ_SE_DP_ELEBETA2, 0);
        PlaySE(SEQ_SE_DP_KASYA);
        app->bgAnimTimer = 8;
        for (count = 0; count < app->numBalls; count++) {
            app->balls[count] = BattleFactoryBall_Delete(app->balls[count]);
            app->balls[count] = NULL;
        }
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            if (app->numBalls == 2) {
                app->numBalls = 4;
            } else {
                app->numBalls = 2;
            }
        }
        BattleFactoryApp_CreateSwapBalls(app);
        for (i = 0; i < app->numBalls; i++) {
            BattleFactoryBall_MoveOffscreen(app->balls[i]);
        }
        PlaySE(SEQ_SE_DP_ELEBETA2);
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SWITCH_PAGE_SCROLL_BALLS_IN:
        if (BattleFactoryApp_ScrollBallsIn(app) != TRUE) {
            break;
        }
        StopSE(SEQ_SE_DP_ELEBETA2, 0);
        PlaySE(SEQ_SE_DP_KASYA);
        for (i = 0; i < app->numBalls; i++) {
            BattleFactoryBall_SetAnim(app->balls[i], 4);
        }
        app->bgAnimTimer = 8;
        BattleFactorySprite_SetVisible(app->centerSprite, TRUE);
        BattleFactorySprite_SetAnim(app->centerSprite, 1);
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SWITCH_PAGE_WAIT_CENTER_ANIM:
        if (BattleFactorySprite_IsAnimated(app->centerSprite) == TRUE) {
            break;
        }
        app->timer = 0;
        app->subState++;
        break;
    case FACTORY_SWITCH_PAGE_SHOW_POKEPICS:
        if (app->timer == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
            if (app->swapOutPicked == FALSE) {
                for (count = 0; count < app->numPokepics; count++) {
                    BattleFactoryApp_ReplacePokepic(app, count, count, 1);
                }
            } else {
                for (count = 0; count < app->numPokepics; count++) {
                    if (count == app->picked[0]) {
                        BattleFactoryApp_ReplacePokepicFromOther(app, count, 0, 0);
                    } else {
                        BattleFactoryApp_ReplacePokepic(app, count, count, 1);
                    }
                }
            }
            Pokepic_StartPaletteFadeAll(app->pokepicManager, 16, 0, 1, 0xFFFF);
        }
        Pokepic_ResumePaletteFade(app->pokepics[0]);
        app->timer++;
        if (app->timer < 21) {
            break;
        }
        BattleFactoryApp_InitWindows(app);
        app->switchingPage = FALSE;
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            app->partnerUpdated = TRUE;
            BattleFactoryApp_PrintPartnerRentals(app);
        }
        app->timer = 0;
        return TRUE;
    }
    return FALSE;
}

enum {
    FACTORY_SEND_SWAP_START,
    FACTORY_SEND_SWAP_WAIT_PARTNER,
};

static BOOL BattleFactoryApp_LinkSendSwap(BattleFactoryApp *app) {
    switch (app->subState) {
    case FACTORY_SEND_SWAP_START:
        if (BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_SWAP, app->swapAccepted) == TRUE) {
            app->subState++;
        }
        break;
    case FACTORY_SEND_SWAP_WAIT_PARTNER:
        if (app->recvCount >= 2) {
            app->recvCount = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

enum {
    FACTORY_SYNC_START,
    FACTORY_SYNC_WAIT,
};

static BOOL BattleFactoryApp_LinkSync(BattleFactoryApp *app) {
    switch (app->subState) {
    case FACTORY_SYNC_START:
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00015);
        LinkSync_ClearPackets();
        LinkSync_StartSync(165);
        app->subState++;
        break;
    case FACTORY_SYNC_WAIT:
        if (LinkSync_IsSynced(165) == TRUE) {
            LinkSync_ClearPackets();
            return TRUE;
        }
        break;
    }
    return FALSE;
}

enum {
    FACTORY_FADE_OUT_START,
    FACTORY_FADE_OUT_WAIT,
};

static BOOL BattleFactoryApp_FadeOut(BattleFactoryApp *app) {
    switch (app->subState) {
    case FACTORY_FADE_OUT_START:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_100);
        app->subState++;
        break;
    case FACTORY_FADE_OUT_WAIT:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

enum {
    FACTORY_TRADE_RESULT_PRINT,
    FACTORY_TRADE_RESULT_WAIT_PRINT,
};

static BOOL BattleFactoryApp_ShowLinkTradeResult(BattleFactoryApp *app) {
    switch (app->subState) {
    case FACTORY_TRADE_RESULT_PRINT:
        if (app->unk_378[1] != NULL) {
            Delete2dMenu(app->unk_17C, NULL);
            BattleFactoryApp_CloseWindow(app->unk_174);
            BattleFactoryBall_Free(app->unk_378[1]);
            app->unk_378[1] = NULL;
        }
        Frontier_BufferPartnerName(app->msgFormat, 0);
        BattleFactoryApp_BufferSpeciesName(app, 1, Mon_GetBoxMon(Party_GetMonByIndex(app->party, app->picked[0] + 2)));
        BattleFactoryApp_BufferSpeciesName(app, 2, Mon_GetBoxMon(Party_GetMonByIndex(app->otherParty, app->picked[1])));
        BattleFactoryApp_DrawMessageFrame(&app->windows[7], Options_GetFrame(app->options));
        app->printerId = BattleFactoryApp_PrintMessage(app, &app->windows[7], msg_0194_00016, 1, 1, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(app->saveData)), 1, 2, 15, 1);
        ScheduleWindowCopyToVram(&app->windows[7]);
        app->timer = 80;
        app->subState++;
        break;
    case FACTORY_TRADE_RESULT_WAIT_PRINT:
        if (TextPrinterCheckActive(app->printerId) == 0) {
            app->timer = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void BattleFactoryApp_VBlankCallback(BattleFactoryApp *app) {
    if (app->summaryApp == NULL) {
        PokepicManager_HandleLoadImgAndOrPltt(app->pokepicManager);
        if (app->paletteData != NULL) {
            PaletteData_PushTransparentBuffers(app->paletteData);
        }
        DoScheduledBgGpuUpdates(app->bgConfig);
        GF_RunVramTransferTasks();
        OamManager_ApplyAndResetBuffers();
        {
            // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
            vu32 *dtcm = (vu32 *)0x027E0000;
            dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
        }
    }
}

static void BattleFactoryApp_SetGraphicsBanks(void) {
    GraphicsBanks banks = sBattleFactoryAppGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void BattleFactoryApp_InitBgLayers(BgConfig *bgConfig) {
    {
        GraphicsModes modes = sBattleFactoryAppGraphicsModes;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        BgTemplate template = sBattleFactoryAppBgTemplate_Main1;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
    }
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    {
        BgTemplate template = sBattleFactoryAppBgTemplate_Main2;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    {
        BgTemplate template = sBattleFactoryAppBgTemplate_Main3;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    {
        BgTemplate template = sBattleFactoryAppBgTemplate_Sub0;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
    }
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    {
        BgTemplate template = sBattleFactoryAppBgTemplate_Sub1;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
    }
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    {
        BgTemplate template = sBattleFactoryAppBgTemplate_Sub2;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    {
        BgTemplate template = sBattleFactoryAppBgTemplate_Sub3;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
}

static void BattleFactoryApp_InitBgGraphics(BattleFactoryApp *app) {
    BattleFactoryApp_SetGraphicsBanks();
    BattleFactoryApp_InitBgLayers(app->bgConfig);
    app->paletteData = PaletteData_Init(HEAP_ID_100);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_100);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_100);
    BattleFactoryApp_LoadBgPalette();
    BattleFactoryApp_LoadBgGfx4(app, GF_BG_LYR_SUB_2);
    BattleFactoryApp_LoadBgGfx81(app, GF_BG_LYR_MAIN_3);
    BattleFactoryApp_LoadBgGfx92(app, GF_BG_LYR_MAIN_2);
    if (app->shown) {
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, app->bgScrollX);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    BattleFactoryApp_LoadSubScreenBg(app, GF_BG_LYR_SUB_3);
}

static void BattleFactoryApp_InitSprites(BattleFactoryApp *app) {
    BattleFactorySpriteGfx_Init(&app->spriteGfx);
}

static void BattleFactoryApp_FreeBgLayers(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(bgConfig);
}

static void BattleFactoryApp_SetMainState(BattleFactoryApp *app, int *state, int nextState) {
    app->subState = 0;
    *state = nextState;
}

static BOOL BattleFactoryApp_ScrollBallsIn(BattleFactoryApp *app) {
    int done = FALSE;
    int i;
    VecFx32 pos;

    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_ADD_X, 8);
    app->bgScrollX = Bg_GetXpos(app->bgConfig, GF_BG_LYR_SUB_2);
    for (i = 0; i < app->numBalls; i++) {
        if (BattleFactoryBall_GetPosition(app->balls[i])->x / FX32_ONE - 8 <= BattleFactoryBall_GetX(app->balls[i])) {
            int x = BattleFactoryBall_GetX(app->balls[i]);
            int y = BattleFactoryBall_GetY(app->balls[i]);
            BattleFactoryBall_SetPosition(app->balls[i], x, y);
            done = TRUE;
        } else {
            BattleFactoryBall_Move(&pos, app->balls[i], -8, 0);
        }
    }
    BattleFactoryApp_AnimateBackground(app);
    if (done == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static BOOL BattleFactoryApp_ScrollBallsOut(BattleFactoryApp *app) {
    int hidden = 0;
    int i;
    VecFx32 pos;

    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_ADD_X, 8);
    app->bgScrollX = Bg_GetXpos(app->bgConfig, GF_BG_LYR_SUB_2);
    for (i = 0; i < app->numBalls; i++) {
        int x = BattleFactoryBall_GetPosition(app->balls[i])->x / FX32_ONE - 8;
        if (x < -24) {
            BattleFactoryBall_SetVisible(app->balls[i], FALSE);
            hidden++;
        } else {
            BattleFactoryBall_Move(&pos, app->balls[i], -8, 0);
        }
    }
    BattleFactoryApp_AnimateBackground(app);
    if (hidden == app->numBalls) {
        return TRUE;
    }
    return FALSE;
}

static void BattleFactoryApp_AnimateBackground(BattleFactoryApp *app) {
    u32 timer = app->bgAnimTimer;
    if ((timer & 3) == 0) {
        if ((timer >> 2) >= 4) {
            app->bgAnimTimer = 0;
        }
        BattleFactoryApp_SetBgPalette(app, GF_BG_LYR_SUB_3, (u8)((app->bgAnimTimer >> 2) + 2), 32, 32);
    }
    app->bgAnimTimer++;
}

static void BattleFactoryApp_SetBgPalette(BattleFactoryApp *app, int bgId, u8 palette, u8 width, u8 height) {
    BgTilemapRectChangePalette(app->bgConfig, bgId, 0, 0, width, height, palette);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, bgId);
}

static void BattleFactoryApp_SetCenterAnim(BattleFactoryApp *app, int kind) {
    int animSeq;

    switch (kind) {
    case 0:
        animSeq = 1;
        break;
    case 1:
        animSeq = 2;
        break;
    case 4:
        animSeq = 1;
        break;
    case 5:
        animSeq = 2;
        break;
    default:
        animSeq = 4;
        break;
    }
    app->centerSprite = BattleFactoryApp_CreateCenterSprite(app, animSeq);
}

static void BattleFactoryApp_LoadBgGfx81(BattleFactoryApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x81, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x82, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
}

static void BattleFactoryApp_LoadBgGfx92(BattleFactoryApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x92, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x91, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
}

static void BattleFactoryApp_LoadBgGfx4(BattleFactoryApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 4, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 8, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
}

static void BattleFactoryApp_LoadBgPalette(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0xC1, &plttData, HEAP_ID_100);
    DC_FlushRange(plttData->pRawData, 0x160);
    GX_LoadBGPltt(plttData->pRawData, 0, 0x160);
    Heap_Free(file);
}

static void BattleFactoryApp_LoadSubScreenBg(BattleFactoryApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 4, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 7, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_GXLoadPalFromOpenNarc(app->narc, 0x95, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x160, HEAP_ID_100);
}

static u8 BattleFactoryApp_PrintMessage(BattleFactoryApp *app, Window *window, u32 msgId, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    FillWindowPixelBuffer(window, bg);
    ReadMsgDataIntoString(app->msgData, msgId, app->rawString);
    StringExpandPlaceholders(app->msgFormat, app->expandedString, app->rawString);
    return AddTextPrinterParameterizedWithColor(window, fontId, app->expandedString, x, y, textSpeed, TEXT_COLOR(fg, shadow, bg), NULL);
}

static u8 BattleFactoryApp_ShowMessage(BattleFactoryApp *app, u32 msgId) {
    u8 printerId = BattleFactoryApp_PrintMessage(app, &app->windows[7], msgId, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, 1);
    ScheduleWindowCopyToVram(&app->windows[7]);
    return printerId;
}

static void BattleFactoryApp_BufferNumber(BattleFactoryApp *app, u32 field, s32 value) {
    BufferIntegerAsString(app->msgFormat, field, value, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
}

static void BattleFactoryApp_BufferSpeciesName(BattleFactoryApp *app, u32 field, BoxPokemon *boxMon) {
    BufferBoxMonSpeciesName(app->msgFormat, field, boxMon);
}

static void BattleFactoryApp_PrintPlayerName(BattleFactoryApp *app, Window *window, int x, int y, u8 fontId) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(app->saveData);
    String *string = String_New(PLAYER_NAME_LENGTH + 1, HEAP_ID_100);
    u32 color;

    FillWindowPixelBuffer(window, 0);
    CopyU16ArrayToString(string, PlayerProfile_GetNamePtr(profile));
    if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
        color = MAKE_TEXT_COLOR(7, 8, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

static void BattleFactoryApp_PrintPartnerName(BattleFactoryApp *app, Window *window, int x, int y, u8 fontId) {
    PlayerProfile *profile = CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId());
    String *string = String_New(PLAYER_NAME_LENGTH + 1, HEAP_ID_100);
    u32 color;

    FillWindowPixelBuffer(window, 0);
    PlayerName_FlatToString(profile, string);
    if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
        color = MAKE_TEXT_COLOR(7, 8, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

static void BattleFactoryApp_PrintMonName(BattleFactoryApp *app, Window *window, int monIndex, int x, int y, u8 fg, u8 shadow, u8 bg, u8 fontId, Party *party) {
    Pokemon *mon;
    String *name;
    String *format;
    int gender;
    u16 right;

    FillWindowPixelBuffer(window, bg);
    mon = Party_GetMonByIndex(party, monIndex);
    name = String_New(POKEMON_NAME_LENGTH + 1, HEAP_ID_100);
    format = NewString_ReadMsgData(app->msgData, msg_0194_00028);
    BufferBoxMonSpeciesName(app->msgFormat, 0, Mon_GetBoxMon(mon));
    StringExpandPlaceholders(app->msgFormat, name, format);
    AddTextPrinterParameterizedWithColor(window, fontId, name, x, (int)y, TEXT_SPEED_NOTRANSFER, TEXT_COLOR(fg, shadow, bg), NULL);
    String_Delete(format);
    String_Delete(name);
    if (GetMonData(mon, MON_DATA_NO_PRINT_GENDER, NULL) == TRUE) {
        gender = GetMonGender(mon);
        right = ((GetWindowWidth(window) - 1) << 3) - 4;
        if (gender == MON_MALE) {
            name = NewString_ReadMsgData(app->msgData, msg_0194_00026);
            AddTextPrinterParameterizedWithColor(window, fontId, name, right, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 0), NULL);
            String_Delete(name);
        } else if (gender == MON_FEMALE) {
            name = NewString_ReadMsgData(app->msgData, msg_0194_00027);
            AddTextPrinterParameterizedWithColor(window, fontId, name, right, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
            String_Delete(name);
        }
    }
    ScheduleWindowCopyToVram(window);
}

static void BattleFactoryApp_PrintPartnerMonName(BattleFactoryApp *app, Window *window, int x, int y, u8 fg, u8 shadow, u8 bg, u8 fontId, u16 species, u8 gender) {
    MsgData *speciesNames;
    String *string;
    u8 width;
    int genderMsg;
    int color;

    FillWindowPixelBuffer(window, bg);
    speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_100);
    string = NewString_ReadMsgData(speciesNames, (u16)species);
    DestroyMsgData(speciesNames);
    AddTextPrinterParameterizedWithColor(window, (u8)fontId, string, x, y, TEXT_SPEED_NOTRANSFER, TEXT_COLOR(fg, shadow, bg), NULL);
    width = GetWindowWidth(window) - 1;
    genderMsg = gender == MON_MALE ? msg_0194_00026 : msg_0194_00027;
    color = gender == MON_MALE ? MAKE_TEXT_COLOR(7, 8, 0) : MAKE_TEXT_COLOR(3, 4, 0);
    String_SetEmpty(string);
    if (gender != MON_GENDERLESS) {
        ReadMsgDataIntoString(app->msgData, genderMsg, string);
        AddTextPrinterParameterizedWithColor(window, (u8)fontId, string, width << 3, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    }
    String_Delete(string);
}

static void BattleFactoryApp_FreeGraphics(BattleFactoryApp *app) {
    BattleFactoryApp_FreeBgCellManager(app);
    {
        int i;
        for (i = 0; i < app->numBalls; i++) {
            if (app->balls[i] != NULL) {
                app->balls[i] = BattleFactoryBall_Delete(app->balls[i]);
            }
        }
    }
    BattleFactorySprite_Delete(app->cursor);
    BattleFactorySprite_Delete(app->buttonCursor);
    BattleFactorySprite_Delete(app->picCursor);
    BattleFactorySprite_Delete(app->monIcons[0]);
    BattleFactorySprite_Delete(app->monIcons[1]);
    BattleFactoryApp_DeleteCenterSprites(app);
    BattleFactoryApp_DeleteSlotMarkers(app);
    BattleFactoryApp_DeleteOrderSprites(app);
    WirelessStatusIcon_Destroy();
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->paletteData);
    app->paletteData = NULL;
    BattleFactorySpriteGfx_Free(&app->spriteGfx);
    {
        int i;
        for (i = 0; i < 3; i++) {
            if (app->pokepics[i] != NULL) {
                Pokepic_Delete(app->pokepics[i]);
            }
        }
    }
    PokepicManager_Delete(app->pokepicManager);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
    String_Delete(app->expandedString);
    String_Delete(app->rawString);
    {
        int i;
        for (i = 0; i < 4; i++) {
            String_Delete(app->strings[i]);
        }
    }
    FontID_Release(4);
    BattleFactoryApp_RemoveWindows(app->windows);
    BattleFactoryApp_FreeBgLayers(app->bgConfig);
    NARC_Delete(app->narc);
    GF_3DVramMan_Delete(app->vramMan);
}

static void BattleFactoryApp_ResumeAfterSummary(BattleFactoryApp *app) {
    BattleFactoryApp_DisableDisplay();
    app->vramMan = GF_3DVramMan_Create(HEAP_ID_100, 0, 2, 0, 2, BattleFactoryApp_Setup3D);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_100);
    BattleFactoryApp_InitGraphics(app);
    BattleFactoryApp_AddWindows(app->bgConfig, app->windows);
    if (app->numPokepics == 2) {
        BattleFactoryApp_SetLinkNameWindows(&app->windows[2]);
    }
}

static void BattleFactoryApp_DisableDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

static void BattleFactoryApp_InitGraphics(BattleFactoryApp *app) {
    int i;

    app->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_100);
    BattleFactoryApp_InitBgGraphics(app);
    BattleFactoryApp_InitSprites(app);
    FontID_Alloc(4, HEAP_ID_100);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0194_bin, HEAP_ID_100);
    app->msgFormat = MessageFormat_New(HEAP_ID_100);
    app->expandedString = String_New(800, HEAP_ID_100);
    app->rawString = String_New(800, HEAP_ID_100);
    for (i = 0; i < 4; i++) {
        app->strings[i] = String_New(64, HEAP_ID_100);
    }
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_100);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_100);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_100);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_100);
    app->pokepicManager = PokepicManager_Create(HEAP_ID_100);
    if (Comm_IsActive()) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        WirelessStatusIcon_Create();
    }
    if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
        BattleFactoryApp_CreateRentalBalls(app);
        BattleFactoryApp_CreateRentalPokepics(app);
    } else {
        BattleFactoryApp_CreateSwapBalls(app);
        BattleFactoryApp_CreateSwapPokepics(app);
    }
    BattleFactoryApp_LoadSubPalette(app);
    BattleFactoryApp_LoadButtonGraphics(app->bgConfig, app->narc);
    BattleFactoryApp_InitBgCellManager(app);
    GfGfx_BothDispOn();
    Main_SetVBlankIntrCB((GFIntrCB)BattleFactoryApp_VBlankCallback, app);
}

static void BattleFactoryApp_CreateSummaryArgs(BattleFactoryApp *app) {
    app->summaryArgs = Heap_Alloc(HEAP_ID_100, sizeof(PokemonSummaryArgs));
    memset(app->summaryArgs, 0, sizeof(PokemonSummaryArgs));
    app->summaryArgs->party = app->party;
    app->summaryArgs->options = app->options;
    app->summaryArgs->monDataType = 1;
    app->summaryArgs->mode = 1;
    app->summaryArgs->partyCount = app->numBalls;
    app->summaryArgs->partySlot = app->cursorPos;
    app->summaryArgs->moveToLearn = 0;
    app->summaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(app->saveData);
    app->summaryArgs->unk2C = 0;
    app->summaryArgs->isFlag982Set = 0;
    PokemonSummary_SetPages(app->summaryArgs, sBattleFactoryAppSummaryPages);
    PokemonSummaryArgs_SetPlayer(app->summaryArgs, Save_PlayerData_GetProfile(app->saveData));
}

static BOOL BattleFactoryApp_IsScreen(BattleFactoryApp *app, u8 screen) {
    return app->screen == screen;
}

static int BattleFactory_GetRentalCount(int frontierType) {
    return frontierType == 0 || frontierType == 1 ? 3 : 2;
}

static BOOL BattleFactoryApp_IsCancelRequested(BattleFactoryApp *app) {
    if (app->cancelRequested) {
        return TRUE;
    }
    return FALSE;
}

static void BattleFactoryApp_SetCancelRequested(BattleFactoryApp *app, u8 value) {
    app->cancelRequested = value;
}

static void BattleFactoryApp_CreatePokepic(BattleFactoryApp *app, int index, Pokemon *mon, int x, int y, int hFlip) {
    u32 species;

    app->pokepics[index] = BattleFactory_CreatePokepic(app->pokepicManager, 0, mon, x, y, 0);
    Pokepic_SetAttr(app->pokepics[index], POKEPIC_VANISHED, FALSE);
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    if (GetMonBaseStat_HandleAlternateForm(species, GetMonData(mon, MON_DATA_FORM, NULL), BASE_FLIP) == 0) {
        BattleFactory_SetPokepicHFlip(app->pokepics[index], hFlip);
    }
}

static BOOL BattleFactoryApp_SendLinkCommand(BattleFactoryApp *app, int command, int arg) {
    int linkCommand;

    if (!BattleFactory_IsMultiType(app->frontierType)) {
        return FALSE;
    }
    switch (command) {
    case FACTORY_LINK_PING:
        linkCommand = 29;
        BattleFactoryApp_PrepareLinkPing(app, command);
        break;
    case FACTORY_LINK_RENTALS:
        linkCommand = 30;
        BattleFactoryApp_PrepareLinkRentals(app, command, arg);
        break;
    case FACTORY_LINK_SWAP:
        linkCommand = 32;
        BattleFactoryApp_PrepareLinkSwap(app, command, arg);
        break;
    }
    if (Comm_SendCommand(linkCommand, app->sendBuf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void BattleFactoryApp_PrepareLinkPing(BattleFactoryApp *app, int command) {
    Save_PlayerData_GetProfile(app->saveData);
    app->sendBuf[0] = command;
}

void BattleFactory_ReceivePing(int netId) {
    if (netId != Comm_GetLocalNetId()) {
        return;
    }
}

static void BattleFactoryApp_PrepareLinkRentals(BattleFactoryApp *app, int command, int arg) {
    int i = 0;
    int pos = i;

    for (; i < 60; i++) {
        app->sendBuf[i] = 0;
    }
    app->sendBuf[0] = app->numPicked;
    pos++;
    for (i = 0; i < app->numPicked; i++) {
        app->sendBuf[pos + i] = (u16)GetMonData(Party_GetMonByIndex(app->party, app->picked[i]), MON_DATA_SPECIES, NULL);
    }
    pos += app->numPicked;
    for (i = 0; i < app->numPicked; i++) {
        app->sendBuf[pos + i] = (u16)GetMonData(Party_GetMonByIndex(app->party, app->picked[i]), MON_DATA_GENDER, NULL);
    }
}

void BattleFactory_ReceiveRentals(int netId, int size, const u16 *data, BattleFactoryApp *app) {
    int i;
    int pos = 0;

    BattleFactory_GetPartySize(app->frontierType);
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    app->numPartnerMons = data[0];
    pos++;
    for (i = 0; i < app->numPartnerMons; i++) {
        app->partnerSpecies[i] = data[pos + i];
    }
    pos += app->numPartnerMons;
    for (i = 0; i < app->numPartnerMons; i++) {
        app->partnerGender[i] = data[pos + i];
    }
    app->partnerUpdated = TRUE;
}

void BattleFactory_ReceiveUnused(void) {
    GF_AssertFail();
}

static void BattleFactoryApp_PrintPartnerRentals(BattleFactoryApp *app) {
    int count = BattleFactory_GetPartySize(app->frontierType);

    if (app->switchingPage == TRUE) {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[1]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[5]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[6]);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        return;
    }
    if (app->partnerUpdated == TRUE) {
        int i;
        for (i = 0; i < count; i++) {
            Window *window = &app->windows[i + 5];
            FillWindowPixelBuffer(window, 0);
            if (i < app->numPartnerMons) {
                BattleFactoryApp_PrintPartnerMonName(app, window, 0, 0, 15, 2, 0, 0, app->partnerSpecies[i], app->partnerGender[i]);
            }
            ScheduleWindowCopyToVram(window);
        }
        BattleFactoryApp_PrintPartnerName(app, &app->windows[1], 0, 0, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    }
    app->partnerUpdated = FALSE;
}

static void BattleFactoryApp_PrepareLinkSwap(BattleFactoryApp *app, int command, int accepted) {
    app->sendBuf[0] = command;
    app->sendBuf[1] = accepted;
    app->sendBuf[2] = app->picked[0];
    app->sendBuf[3] = app->picked[1];
}

void BattleFactory_ReceiveSwap(int netId, int size, const BattleFactoryLinkMessage *msg, BattleFactoryApp *app) {
#pragma unused(size)
    app->recvCount++;
    if (netId != Comm_GetLocalNetId()) {
        app->partnerSwapState = (u8)msg->accepted;
        if (Comm_GetLocalNetId() == 0) {
            if (app->swapAccepted == TRUE) {
                app->partnerSwapState = FALSE;
                return;
            }
            if (app->partnerSwapState == TRUE) {
                app->picked[0] = (u8)msg->picked0;
                app->picked[1] = (u8)msg->picked1;
            }
        } else if (app->partnerSwapState == TRUE) {
            app->swapAccepted = FALSE;
            app->picked[0] = (u8)msg->picked0;
            app->picked[1] = (u8)msg->picked1;
        }
    }
}

static BattleFactoryBall *BattleFactoryApp_CreateBall(BattleFactoryApp *app, int index) {
    u16 x;
    u16 y;

    if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
        x = sBattleFactoryBallPositions_Rental[index][0];
        y = sBattleFactoryBallPositions_Rental[index][1];
    } else if (BattleFactory_IsMultiType(app->frontierType) == FALSE) {
        x = sBattleFactoryBallPositions_Single[index][0];
        y = sBattleFactoryBallPositions_Single[index][1];
    } else if (app->numBalls == 2) {
        x = sBattleFactoryBallPositions_Multi2[index][0];
        y = sBattleFactoryBallPositions_Multi2[index][1];
    } else {
        x = sBattleFactoryBallPositions_Multi[index][0];
        y = sBattleFactoryBallPositions_Multi[index][1];
    }
    return BattleFactoryBall_Create(&app->spriteGfx, x, y, HEAP_ID_100);
}

static BattleFactorySprite *BattleFactoryApp_CreateCenterSprite(BattleFactoryApp *app, int animSeq) {
    return BattleFactorySprite_Create(&app->spriteGfx, 1, animSeq, 2, 10, 128, 124);
}

static void BattleFactoryApp_DeleteCenterSprites(BattleFactoryApp *app) {
    if (app->centerSprite != NULL) {
        BattleFactorySprite_Delete(app->centerSprite);
        app->centerSprite = NULL;
    }
    if (app->unk_384 != NULL) {
        BattleFactorySprite_Delete(app->unk_384);
        app->unk_384 = NULL;
    }
}

static void BattleFactoryApp_InitWindows(BattleFactoryApp *app) {
    SetBgPriority(GF_BG_LYR_MAIN_1, 1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BattleFactoryApp_AddWindows(app->bgConfig, app->windows);
    if (app->numPokepics == 2) {
        BattleFactoryApp_SetLinkNameWindows(&app->windows[2]);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
}

static void BattleFactoryApp_CloseWindow(Window *window) {
    EraseFrameAndWindow1(window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(window);
}

void BattleFactoryApp_MoveRentalCursor(BattleFactoryApp *app, int target) {
    u32 x;
    u32 y;

    BattleFactory_GetGridCursorPos(app->grid, target, &x, &y);
    BattleFactorySprite_SetPositionSub(app->cursor, x, y);
}

void BattleFactoryApp_MoveButtonCursor(BattleFactoryApp *app, int target) {
    u32 x;
    u32 y;

    BattleFactory_GetGridCursorPos(app->grid, target, &x, &y);
    BattleFactorySprite_SetAnim(app->buttonCursor, 3);
    BattleFactorySprite_SetPositionSub(app->buttonCursor, x, y);
}

static void BattleFactoryApp_OpenRentalMenu(BattleFactoryApp *app) {
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 0);
    ScheduleWindowCopyToVram(&app->windows[11]);
    ScheduleWindowCopyToVram(&app->windows[14]);
    if (!BattleFactoryBall_IsSelected(app->balls[app->cursorPos])) {
        ScheduleWindowCopyToVram(&app->windows[12]);
    } else {
        ScheduleWindowCopyToVram(&app->windows[13]);
    }
    GridInputHandler_SetNextInput(app->grid, 6);
    BattleFactoryApp_MoveButtonCursor(app, 6);
    BattleFactory_SetRentalGridMenu(app->grid, TRUE);
    BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
    BattleFactorySprite_SetPalette(app->cursor, 1);
}

static void BattleFactoryApp_CloseRentalMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 0);
    GridInputHandler_SetNextInput(app->grid, (u8)app->cursorPos);
    BattleFactory_SetRentalGridMenu(app->grid, FALSE);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
    BattleFactorySprite_SetPalette(app->cursor, 0);
}

static void BattleFactoryApp_PrintRentalMenu(BattleFactoryApp *app) {
    int width;

    FillWindowPixelBuffer(&app->windows[11], 2);
    FillWindowPixelBuffer(&app->windows[12], 2);
    FillWindowPixelBuffer(&app->windows[13], 2);
    FillWindowPixelBuffer(&app->windows[14], 2);
    width = GetWindowWidth(&app->windows[11]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[11], app->msgData, msg_0194_00005, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[12]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[12], app->msgData, msg_0194_00006, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[13]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[13], app->msgData, msg_0194_00008, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[14]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[14], app->msgData, msg_0194_00007, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[11]);
    CopyWindowPixelsToVram_TextMode(&app->windows[12]);
    CopyWindowPixelsToVram_TextMode(&app->windows[13]);
    CopyWindowPixelsToVram_TextMode(&app->windows[14]);
}

void BattleFactoryApp_MoveSwapOutCursor(BattleFactoryApp *app, int target) {
    u32 x;
    u32 y;

    BattleFactory_GetGridCursorPos(app->grid, target, &x, &y);
    if (target == 3) {
        BattleFactorySprite_SetVisible(app->cursor, FALSE);
        BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
        BattleFactorySprite_SetAnim(app->buttonCursor, 6);
        BattleFactorySprite_SetPositionSub(app->buttonCursor, x, y);
    } else {
        BattleFactorySprite_SetVisible(app->cursor, TRUE);
        BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
        BattleFactorySprite_SetPositionSub(app->cursor, x, y);
    }
}

void BattleFactoryApp_MoveSwapInCursor(BattleFactoryApp *app, int target) {
    u32 x;
    u32 y;

    BattleFactory_GetGridCursorPos(app->grid, target, &x, &y);
    if ((u32)(target - 4) <= 1) {
        BattleFactorySprite_SetVisible(app->cursor, FALSE);
        BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
        BattleFactorySprite_SetAnim(app->buttonCursor, 6);
        BattleFactorySprite_SetPositionSub(app->buttonCursor, x, y);
    } else {
        BattleFactorySprite_SetVisible(app->cursor, TRUE);
        BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
        BattleFactorySprite_SetPositionSub(app->cursor, x, y);
    }
}

static void BattleFactoryApp_OpenSwapOutMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 3);
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 0);
    ScheduleWindowCopyToVram(&app->windows[11]);
    ScheduleWindowCopyToVram(&app->windows[15]);
    ScheduleWindowCopyToVram(&app->windows[14]);
    GridInputHandler_SetNextInput(app->grid, 4);
    BattleFactoryApp_MoveButtonCursor(app, 4);
    BattleFactory_SetSwapOutGridMenu(app->grid, TRUE, app->frontierType);
    BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
}

static void BattleFactoryApp_CloseSwapOutMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 0);
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 3);
    ScheduleWindowCopyToVram(&app->windows[10]);
    GridInputHandler_SetNextInput(app->grid, (u8)app->cursorPos);
    BattleFactory_SetSwapOutGridMenu(app->grid, FALSE, app->frontierType);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
}

static void BattleFactoryApp_RedrawSwapOutButtons(BattleFactoryApp *app) {
    int target = GridInputHandler_GetNextInput(app->grid);

    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 3);
    ScheduleWindowCopyToVram(&app->windows[10]);
    if (target == 3) {
        BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
    }
}

static void BattleFactoryApp_ClearSwapOutMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 3);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
}

static void BattleFactoryApp_PrintSwapOutMenu(BattleFactoryApp *app) {
    FillWindowPixelBuffer(&app->windows[11], 2);
    FillWindowPixelBuffer(&app->windows[15], 2);
    FillWindowPixelBuffer(&app->windows[14], 2);
    FillWindowPixelBuffer(&app->windows[10], 2);
    BattleFactoryApp_PrintMsgAligned(&app->windows[11], app->msgData, msg_0194_00020, GetWindowWidth(&app->windows[11]) * 8 / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    BattleFactoryApp_PrintMsgAligned(&app->windows[15], app->msgData, msg_0194_00021, GetWindowWidth(&app->windows[15]) * 8 / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    BattleFactoryApp_PrintMsgAligned(&app->windows[14], app->msgData, msg_0194_00022, GetWindowWidth(&app->windows[14]) * 8 / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    BattleFactoryApp_PrintMsgAligned(&app->windows[10], app->msgData, msg_0194_00019, GetWindowWidth(&app->windows[10]) * 8 / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[11]);
    CopyWindowPixelsToVram_TextMode(&app->windows[15]);
    CopyWindowPixelsToVram_TextMode(&app->windows[14]);
    CopyWindowPixelsToVram_TextMode(&app->windows[10]);
}

static void BattleFactoryApp_OpenSwapInMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 2);
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 1);
    ScheduleWindowCopyToVram(&app->windows[16]);
    ScheduleWindowCopyToVram(&app->windows[17]);
    GridInputHandler_SetNextInput(app->grid, 6);
    BattleFactoryApp_MoveButtonCursor(app, 6);
    BattleFactory_SetSwapInGridMenu(app->grid, TRUE, app->frontierType);
    BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
}

static void BattleFactoryApp_CloseSwapInMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 1);
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 2);
    ScheduleWindowCopyToVram(&app->windows[9]);
    ScheduleWindowCopyToVram(&app->windows[10]);
    GridInputHandler_SetNextInput(app->grid, (u8)app->cursorPos);
    BattleFactory_SetSwapInGridMenu(app->grid, FALSE, app->frontierType);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
}

static void BattleFactoryApp_RedrawSwapInButtons(BattleFactoryApp *app) {
    int target = GridInputHandler_GetNextInput(app->grid);

    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 2);
    ScheduleWindowCopyToVram(&app->windows[9]);
    ScheduleWindowCopyToVram(&app->windows[10]);
    if ((u32)target >= 4) {
        BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
    }
}

static void BattleFactoryApp_ClearSwapInMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 2);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
}

static void BattleFactoryApp_PrintSwapInMenu(BattleFactoryApp *app) {
    int width;

    FillWindowPixelBuffer(&app->windows[16], 2);
    FillWindowPixelBuffer(&app->windows[17], 2);
    FillWindowPixelBuffer(&app->windows[9], 2);
    FillWindowPixelBuffer(&app->windows[10], 2);
    width = GetWindowWidth(&app->windows[16]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[16], app->msgData, msg_0194_00025, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[17]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[17], app->msgData, msg_0194_00022, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[9]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[9], app->msgData, msg_0194_00023, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[10]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[10], app->msgData, msg_0194_00024, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[16]);
    CopyWindowPixelsToVram_TextMode(&app->windows[17]);
    CopyWindowPixelsToVram_TextMode(&app->windows[9]);
    CopyWindowPixelsToVram_TextMode(&app->windows[10]);
}

static void BattleFactoryApp_CreateYesNo(YesNoPrompt **prompt) {
    *prompt = YesNoPrompt_Create(HEAP_ID_100);
}

static void BattleFactoryApp_DestroyYesNo(YesNoPrompt *prompt) {
    YesNoPrompt_Destroy(prompt);
}

static void BattleFactoryApp_InitYesNo(YesNoPrompt *prompt, BgConfig *bgConfig) {
    YesNoPromptTemplate template;

    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_SUB_0;
    template.tileStart = 0x380;
    template.plttSlot = 14;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = FALSE;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
}

static void BattleFactoryApp_UpdatePreviewPokepic(BattleFactoryApp *app) {
    u32 target = GridInputHandler_GetNextInput(app->grid);

    if (target < 6) {
        int count = app->numPicked;
        u16 i = 0;
        for (; i < count; i++) {
            if (target == app->picked[i]) {
                break;
            }
        }
        if (count != 0 && i != count) {
            Pokepic_SetAttr(app->pokepics[count], POKEPIC_VANISHED, TRUE);
        } else {
            BattleFactoryApp_ReplacePokepic(app, count, target, 0);
            Pokepic_SetAttr(app->pokepics[app->numPicked], POKEPIC_VANISHED, FALSE);
        }
        BattleFactoryApp_MovePicCursor(app);
    }
}

static void BattleFactory_GetPokepicPos(int slot, int numPokepics, int *x, int *y) {
    if (numPokepics == 3) {
        *x = slot * 80 + 48;
    } else {
        *x = slot * 112 + 72;
    }
    *y = 144;
}

static void BattleFactoryApp_GetCursorPokepicPos(BattleFactoryApp *app, int *x, int *y) {
    u32 target = GridInputHandler_GetNextInput(app->grid);
    u16 i;

    if (target < 6) {
        for (i = 0; i < app->numPicked; i++) {
            if (target == app->picked[i]) {
                break;
            }
        }
        BattleFactory_GetPokepicPos(i, app->numPokepics, x, y);
    }
}

static void BattleFactoryApp_MovePicCursor(BattleFactoryApp *app) {
    int x;
    int y;

    BattleFactoryApp_GetCursorPokepicPos(app, &x, &y);
    BattleFactorySprite_SetPosition(app->picCursor, x, y);
}

static int BattleFactoryApp_GetCursorSlot(BattleFactoryApp *app) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (app->cursorPos == app->picked[i]) {
            return i;
        }
    }
    return app->numPicked;
}

static void BattleFactoryApp_UpdateSwapOutPicCursor(BattleFactoryApp *app) {
    int x;
    int y;
    u32 target = GridInputHandler_GetNextInput(app->grid);

    if (target >= 4) {
        return;
    }
    if (target == 3) {
        BattleFactorySprite_SetVisible(app->picCursor, FALSE);
        return;
    }
    BattleFactorySprite_SetVisible(app->picCursor, TRUE);
    BattleFactory_GetPokepicPos(target, app->numPokepics, &x, &y);
    BattleFactorySprite_SetPosition(app->picCursor, x, y);
}

static void BattleFactoryApp_UpdateSwapInPicCursor(BattleFactoryApp *app) {
    u32 target = GridInputHandler_GetNextInput(app->grid);

    if (target >= 6) {
        return;
    }
    if (target >= 4) {
        BattleFactorySprite_SetVisible(app->picCursor, FALSE);
        return;
    }
    BattleFactorySprite_SetVisible(app->picCursor, TRUE);
    BattleFactoryApp_ReplacePokepicFromOther(app, app->picked[0], target, 0);
}

static void BattleFactoryApp_CreatePartyPokepics(BattleFactoryApp *app, int hFlip) {
    u32 offset;
    u32 x;
    u32 step;
    u32 i;
    u8 *slot;
    u8 *pic;

    if (app->numPokepics == 3) {
        x = 48;
        step = 80;
    } else {
        x = 72;
        step = 112;
    }
    i = 0;
    if (i < app->numPokepics) {
        offset = i;
        slot = (u8 *)app;
        pic = (u8 *)app;
        do {
            Pokemon *mon = Party_GetMonByIndex(app->party, ((BattleFactoryApp *)slot)->picked[0]);
            BattleFactoryApp_CreatePokepic(app, (u8)i, mon, x + offset, 144, hFlip);
            Pokepic_SetAttr(((BattleFactoryApp *)pic)->pokepics[0], POKEPIC_VANISHED, TRUE);
            i++;
            offset += step;
            pic += sizeof(Pokepic *);
            slot += sizeof(u16);
        } while (i < app->numPokepics);
    }
}

static void BattleFactoryApp_ReplacePokepic(BattleFactoryApp *app, int slot, int monIndex, int hFlip) {
    Pokepic_Delete(app->pokepics[slot]);
    if (app->numPokepics == 3) {
        Pokemon *mon = Party_GetMonByIndex(app->party, monIndex);
        BattleFactoryApp_CreatePokepic(app, (u8)slot, mon, slot * 80 + 48, 144, hFlip);
    } else {
        Pokemon *mon = Party_GetMonByIndex(app->party, monIndex);
        BattleFactoryApp_CreatePokepic(app, (u8)slot, mon, slot * 112 + 72, 144, hFlip);
    }
}

static void BattleFactoryApp_ReplacePokepicFromOther(BattleFactoryApp *app, int slot, int monIndex, int hFlip) {
    Pokepic_Delete(app->pokepics[slot]);
    if (app->numPokepics == 3) {
        Pokemon *mon = Party_GetMonByIndex(app->otherParty, monIndex);
        BattleFactoryApp_CreatePokepic(app, (u8)slot, mon, slot * 80 + 48, 144, hFlip);
    } else {
        Pokemon *mon = Party_GetMonByIndex(app->otherParty, monIndex);
        BattleFactoryApp_CreatePokepic(app, (u8)slot, mon, slot * 112 + 72, 144, hFlip);
    }
}

static void BattleFactoryApp_CreateSlotMarkers(BattleFactoryApp *app) {
    u32 x;
    u32 step;
    u32 i;

    if (app->numPokepics == 3) {
        x = 48;
        step = 80;
    } else {
        x = 72;
        step = 112;
    }
    for (i = 0; i < app->numPokepics; i++) {
        app->slotMarkers[i] = BattleFactorySprite_Create(&app->spriteGfx, 1, 3, 1, 0, x + i * step, 144);
        BattleFactorySprite_SetVisible(app->slotMarkers[i], FALSE);
    }
}

static void BattleFactoryApp_DeleteSlotMarkers(BattleFactoryApp *app) {
    u32 i;

    for (i = 0; i < app->numPokepics; i++) {
        BattleFactorySprite_Delete(app->slotMarkers[i]);
    }
}

static void BattleFactoryApp_UpdateSlotMarkers(BattleFactoryApp *app) {
    u32 i;

    for (i = 0; i < app->numPokepics; i++) {
        if (i == app->numPicked) {
            BattleFactorySprite_SetPaletteOverride(app->slotMarkers[i], 3);
            BattleFactorySprite_SetVisible(app->slotMarkers[i], TRUE);
        } else if (i < app->numPicked) {
            BattleFactorySprite_SetPaletteOverride(app->slotMarkers[i], 2);
            BattleFactorySprite_SetVisible(app->slotMarkers[i], TRUE);
        } else {
            BattleFactorySprite_SetVisible(app->slotMarkers[i], FALSE);
        }
    }
}

static void BattleFactoryApp_SetSlotMarkers(BattleFactoryApp *app, int highlightCursor) {
    u32 i;

    for (i = 0; i < app->numPokepics; i++) {
        if (i == app->cursorPos && highlightCursor == TRUE) {
            BattleFactorySprite_SetPaletteOverride(app->slotMarkers[i], 3);
        } else {
            BattleFactorySprite_SetPaletteOverride(app->slotMarkers[i], 2);
        }
        BattleFactorySprite_SetVisible(app->slotMarkers[i], TRUE);
    }
}

static void BattleFactoryApp_CreateOrderSprites(BattleFactoryApp *app) {
    u16 i;
    u16 animSeq;
    int step;
    int x;

    if (app->numPokepics == 3) {
        x = 16;
        step = 80;
    } else {
        x = 40;
        step = 112;
    }
    animSeq = 0;
    i = 0;
    do {
        app->orderSprites[i] = BattleFactorySprite_Create(&app->spriteGfx, 2, animSeq, 0, 0, x + step * i, 176);
        animSeq++;
        i++;
    } while (i < 3);
    if (app->numPokepics == 2) {
        BattleFactorySprite_SetVisible(app->orderSprites[2], FALSE);
    }
    animSeq = 0;
    while (i < 6) {
        app->orderSprites[i] = BattleFactorySprite_CreateSub(&app->spriteGfx, 3, animSeq, 0, 0, 0, 0);
        BattleFactorySprite_SetVisible(app->orderSprites[i], FALSE);
        animSeq++;
        i++;
    }
}

static void BattleFactoryApp_DeleteOrderSprites(BattleFactoryApp *app) {
    u32 i = 0;

    for (; i < 6; i++) {
        BattleFactorySprite_Delete(app->orderSprites[i]);
    }
}

static void BattleFactoryApp_UpdateOrderSprites(BattleFactoryApp *app) {
    u16 i = 3;

    do {
        u8 n = (u8)(i - 3);
        if (n < app->numPicked) {
            VecFx32 pos = *BattleFactoryBall_GetPosition(app->balls[app->picked[n]]);
            pos.x -= 4 * FX32_ONE;
            pos.y += 4 * FX32_ONE;
            BattleFactorySprite_SetMatrix(app->orderSprites[i], &pos);
            BattleFactorySprite_SetVisible(app->orderSprites[i], TRUE);
        } else {
            BattleFactorySprite_SetVisible(app->orderSprites[i], FALSE);
        }
        i++;
    } while (i < 6);
}

static void BattleFactoryApp_ShowMonIcon(BattleFactoryApp *app, Party *party, u32 index, u32 limit) {
    Pokemon *mon;
    u16 species;
    u16 form;
    u32 fileId;
    u32 palette;
    void *buffer;
    NNSG2dCharacterData *charData;
    BattleFactorySprite *icon;

    if (index >= limit) {
        return;
    }
    mon = Party_GetMonByIndex(party, index);
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    form = GetMonData(mon, MON_DATA_FORM, NULL);
    fileId = GetMonIconNaixEx(species, FALSE, form);
    buffer = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, fileId, FALSE, &charData, HEAP_ID_100);
    icon = app->monIcons[app->monIconIndex];
    BattleFactorySprite_LoadCharData(icon, charData->pRawData, 0x200, 2);
    Heap_Free(buffer);
    palette = GetMonIconPaletteEx(species, form, FALSE);
    BattleFactorySprite_SetPalette(app->monIcons[app->monIconIndex], palette);
    BattleFactorySprite_SetVisible(app->monIcons[app->monIconIndex], TRUE);
    app->monIconIndex ^= 1;
    icon = app->monIcons[app->monIconIndex];
    BattleFactorySprite_SetVisible(icon, FALSE);
    BattleFactoryApp_PrintMonName(app, &app->windows[8], (u8)index, 4, 0, 15, 2, 0, 0, party);
}

static void BattleFactoryApp_PrintRentalNames(BattleFactoryApp *app, u32 target, u32 limit) {
    u16 i;
    volatile BOOL found;
    s32 count;

    if (target >= limit) {
        return;
    }
    i = 0;
    found = FALSE;
    count = *(volatile u8 *)&app->numPicked;
    if (count > 0) {
        do {
            u16 picked = app->picked[i];
            if (target == picked) {
                found = TRUE;
            }
            BattleFactoryApp_PrintMonName(app, &app->windows[i + 2], (u8)picked, 4, 0, 15, 2, 0, 0, app->party);
            i++;
        } while (i < app->numPicked);
    }
    if (!found) {
        BattleFactoryApp_PrintMonName(app, &app->windows[i + 2], (u8)target, 4, 0, 15, 2, 0, 0, app->party);
        i++;
    }
    for (; i < app->numPokepics; i++) {
        FillWindowPixelBuffer(&app->windows[i + 2], 0);
        ScheduleWindowCopyToVram(&app->windows[i + 2]);
    }
}

static void BattleFactoryApp_PrintPartyNames(BattleFactoryApp *app, Party *party) {
    u32 i;

    for (i = 0; i < app->numPokepics; i++) {
        BattleFactoryApp_PrintMonName(app, &app->windows[i + 2], (u8)i, 4, 0, 15, 2, 0, 0, party);
    }
}

static void BattleFactoryApp_PrintSwapNames(BattleFactoryApp *app, u32 swapIn) {
    u32 i = 0;

    if (i < app->numPokepics) {
        Window *windows = app->windows;
        u8 swapInIndex = swapIn;
        do {
            if (app->picked[0] == i) {
                BattleFactoryApp_PrintMonName(app, &windows[i + 2], swapInIndex, 4, 0, 15, 2, 0, 0, app->otherParty);
            } else {
                BattleFactoryApp_PrintMonName(app, &windows[i + 2], (u8)i, 4, 0, 15, 2, 0, 0, app->party);
            }
            i++;
        } while (i < app->numPokepics);
    }
}

static void BattleFactoryApp_PrintSwapInName(BattleFactoryApp *app, u32 index) {
    u32 slot = app->picked[0] + 2;

    if (index >= 4) {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[slot]);
    } else {
        BattleFactoryApp_PrintMonName(app, &app->windows[slot], (u8)index, 4, 0, 15, 2, 0, 0, app->otherParty);
        ScheduleWindowCopyToVram(&app->windows[slot]);
    }
}

static void BattleFactoryApp_LoadSubPalette(BattleFactoryApp *app) {
    PaletteData_LoadPaletteSlotFromHardware(app->paletteData, PLTTBUF_MAIN_OBJ, 0, 0x200);
}

static void BattleFactoryApp_UpdatePaletteBlink(BattleFactoryApp *app) {
    if (app->paletteData != NULL && app->paletteBlinkState != 0xFF && PaletteData_GetSelectedBuffersBitmask(app->paletteData) == 0) {
        if (app->paletteBlinkState == 0) {
            PaletteData_BeginPaletteFade(app->paletteData, PLTTBUF_MAIN_OBJ_F, 8, 2, 0, 16, 0xFFFF);
        } else {
            PaletteData_BeginPaletteFade(app->paletteData, PLTTBUF_MAIN_OBJ_F, 8, 2, 16, 0, 0xFFFF);
        }
        app->paletteBlinkState ^= 1;
    }
}

static void BattleFactoryApp_StartButtonFlash(BattleFactoryApp *app, u8 x, u8 y, u8 width, u8 height, u8 nextState) {
    app->flashX = x;
    app->flashY = y;
    app->flashWidth = width;
    app->flashHeight = height;
    app->flashNextState = nextState;
    app->flashCount = 0;
    app->flashTimer = 0;
    app->flashPalette = 0;
}

static void BattleFactoryApp_FlashWideButton(BattleFactoryApp *app, u8 x, u8 y, u8 nextState) {
    BattleFactoryApp_StartButtonFlash(app, x, y, 12, 2, nextState);
}

static void BattleFactoryApp_FlashNarrowButton(BattleFactoryApp *app, u8 x, u8 y, u8 nextState) {
    BattleFactoryApp_StartButtonFlash(app, x, y, 7, 2, nextState);
}

static BOOL BattleFactoryApp_UpdateButtonFlash(BattleFactoryApp *app) {
    if (app->flashTimer == 0) {
        if (app->flashPalette == 0) {
            BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_1, app->flashX, app->flashY, app->flashWidth, app->flashHeight, 10);
        } else {
            BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_1, app->flashX, app->flashY, app->flashWidth, app->flashHeight, 9);
        }
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_1);
        app->flashPalette ^= 1;
    }
    app->flashTimer++;
    if (app->flashTimer == 2) {
        app->flashCount++;
        if (app->flashCount == 4) {
            app->subState = app->flashNextState;
            return FALSE;
        }
        app->flashTimer = 0;
    }
    return TRUE;
}

static void BattleFactoryApp_SetBallsTranslucent(BattleFactoryApp *app, int enable) {
    u32 i;

    if (enable == TRUE) {
        G2S_SetBlendAlpha(0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 6, 10);
        for (i = 0; i < app->numBalls; i++) {
            if (app->balls[i] != NULL) {
                BattleFactoryBall_SetOamMode(app->balls[i], GX_OAM_MODE_XLU);
            }
        }
    } else {
        G2_BlendNone();
        for (i = 0; i < app->numBalls; i++) {
            if (app->balls[i] != NULL) {
                BattleFactoryBall_SetOamMode(app->balls[i], GX_OAM_MODE_NORMAL);
            }
        }
    }
}
