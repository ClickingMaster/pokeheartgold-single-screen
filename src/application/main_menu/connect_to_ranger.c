#include "application/main_menu/connect_to_ranger.h"

#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/main_menu/main_menu_internal.h"
#include "application/main_menu/main_menu_shared.h"
#include "application/main_menu/ranger_link.h"
#include "application/main_menu/wonder_card_album.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0017.h"
#include "msgdata/msg/msg_0247.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "main.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"
#include "render_window.h"
#include "save_arrays.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "title_screen.h"
#include "comm_wireless.h"
#include "wireless_status_icon.h"

// Main menu option "Connect to Pokemon Ranger": receives the Manaphy Egg from Pokemon Ranger over DS
// wireless and stores it as a Wonder Card.

struct ConnectToRangerApp {
    enum HeapID heapID;             // 0x0000
    BgConfig *bgConfig;             // 0x0004
    SaveData *saveData;             // 0x0008
    Pokedex *pokedex;               // 0x000C
    PlayerProfile *profile;         // 0x0010
    Options *options;               // 0x0014
    Window messageWindow;           // 0x0018
    Window statusWindow;            // 0x0028
    Window playerInfoWindow;        // 0x0038
    MainMenuTextPrinter statusText; // 0x0048
    MainMenuTextPrinter titleText;  // 0x009C
    MainMenuTextPrinter playerText; // 0x00F0
    u32 nextFreeTile;               // 0x0144
    BOOL linkStarted;               // 0x0148
    int unk_14C;                    // 0x014C
    int connectResult;              // 0x0150
    int unk_154;                    // 0x0154
    BOOL linkInitialised;           // 0x0158
    int connectState;               // 0x015C
    int timer;                      // 0x0160
    int partnerIndex;               // 0x0164
    u8 filler_168[4];               // 0x0168
    u8 linkWork[0x3004];            // 0x016C
    Sprite *sprite;                 // 0x3170
    MysteryGiftSave *mysteryGift;   // 0x3174
    u8 filler_3178[4];              // 0x3178
    int textPrinterId;              // 0x317C
    WonderCard wonderCard;          // 0x3180
    WaitingIcon *waitingIcon;       // 0x34D8
}; // size: 0x34DC

// Connection states of ConnectToRanger_RunTransfer
enum {
    RANGER_CONNECT_START,
    RANGER_CONNECT_WAIT_LINK,
    RANGER_CONNECT_SEARCH,
    RANGER_CONNECT_WAIT_PARTNER,
    RANGER_CONNECT_CONNECT_PARTNER,
    RANGER_CONNECT_WAIT_CONNECTED = 7,
    RANGER_CONNECT_SEND_REQUEST,
    RANGER_CONNECT_WAIT_REQUEST_SENT,
    RANGER_CONNECT_WAIT_TOUCH,
    RANGER_CONNECT_SEND_PLAYER,
    RANGER_CONNECT_WAIT_PLAYER_SENT,
    RANGER_CONNECT_SAVE_DELAY,
    RANGER_CONNECT_SAVING,
    RANGER_CONNECT_RANDOM_DELAY,
    RANGER_CONNECT_SEND_SAVED,
    RANGER_CONNECT_WAIT_ACK,
    RANGER_CONNECT_FINISH_SAVE,
    RANGER_CONNECT_DONE_DELAY,
    RANGER_CONNECT_DONE,
    RANGER_CONNECT_LINK_LOST = 22,
    RANGER_CONNECT_TIMEOUT,
    RANGER_CONNECT_CANCEL = 26,
    RANGER_CONNECT_END,
    RANGER_CONNECT_ERROR,
    RANGER_CONNECT_FOUND,
    RANGER_CONNECT_WAIT_A,
};

u16 gConnectToRanger_PlayerName[8]; // player name in Pokemon Ranger's character set
u16 gConnectToRanger_TrainerId[10]; // trainer ID as five digits

extern const u16 sRangerCharTable[0x123][2]; // { Ranger character, game character }
extern const u8 ov74_0223BD5C[];
extern const u8 ov74_0223C1F4[];
extern const GraphicsModes _0223B330;
extern const GraphicsBanks _0223B340;

extern void CommWireless_Terminate(void);
extern void WirelessStatusIcon_SetLevel(int a0);

// Wireless link with Pokemon Ranger (link_status part of this overlay)

u16 ConnectToRanger_ConvertCharToRanger(u16 c) {
    u32 i;
    for (i = 0; i < NELEMS(sRangerCharTable); i++) {
        if (sRangerCharTable[i][1] == c) {
            return sRangerCharTable[i][0];
        }
    }
    return 0x3F;
}

void ConnectToRanger_FormatNumber5Digits(u16 *dst, int value) {
    dst[0] = value / 10000 + '0';
    value %= 10000;
    dst[1] = value / 1000 + '0';
    value %= 1000;
    dst[2] = value / 100 + '0';
    value %= 100;
    dst[3] = value / 10 + '0';
    value %= 10;
    dst[4] = value + '0';
}

#pragma force_active on

void ConnectToRanger_EncodePlayerData(ConnectToRangerApp *app) {
    int i;
    const u16 *src = PlayerProfile_GetNamePtr(app->profile);
    u16 *dst = gConnectToRanger_PlayerName;
    for (i = 0; i < 8; i++) {
        *dst = ConnectToRanger_ConvertCharToRanger(*src);
        src++;
        dst++;
    }
    ConnectToRanger_FormatNumber5Digits(gConnectToRanger_TrainerId, PlayerProfile_GetTrainerID_VisibleHalf(app->profile));
    gConnectToRanger_TrainerId[5] = 0;
}

void ConnectToRanger_SetupGraphics(ConnectToRangerApp *app) {
    GraphicsBanks banks = _0223B340;
    GraphicsModes modes = _0223B330;

    GfGfx_SetBanks(&banks);
    SetBothScreensModesAndDisable(&modes);
    MainMenuUtil_InitBg(app->bgConfig, GF_BG_LYR_MAIN_0, 1, 0xF000, 0);
    MainMenuUtil_InitBg(app->bgConfig, GF_BG_LYR_MAIN_1, 1, 0xF800, 0x4000);
    MainMenuUtil_InitBg(app->bgConfig, GF_BG_LYR_SUB_0, 1, 0x7800, 0);
    MainMenuUtil_InitBg(app->bgConfig, GF_BG_LYR_SUB_1, 1, 0x7000, 0x4000);
    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, app->heapID);
    LoadUserFrameGfx1(app->bgConfig, GF_BG_LYR_MAIN_0, 1, 1, 0, app->heapID);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_0, 10, 2, Options_GetFrame(app->options), app->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_8_OFFSET, 32, app->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x1400, TRUE, app->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, app->heapID);
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 32, 24, 8);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_1);
    ConnectToRanger_LoadSubScreenBg(app);
}

#pragma force_active reset

void ConnectToRanger_LoadSubScreenBg(ConnectToRangerApp *app) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 32, app->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, app->bgConfig, GF_BG_LYR_SUB_0, 0, 0x1400, TRUE, app->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, app->bgConfig, GF_BG_LYR_SUB_0, 0, 0x600, TRUE, app->heapID);
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 32, 24, 8);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_0);
}

// Shows "you need a Pokedex" until A or B is pressed. Returns FALSE once the message is closed.
BOOL ConnectToRanger_ShowPokedexRequiredMessage(ConnectToRangerApp *app) {
    MainMenuTextPrinter printer;
    if (WindowIsInUse(&app->messageWindow) == FALSE) {
        MainMenuTextPrinter_Init(&printer, &app->messageWindow, 0, NARC_msg_msg_0017_bin, 1, 1);
        MainMenuTextPrinter_SetSize(&printer, 22, 16, app->nextFreeTile);
        MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &printer, 5, 4, msg_0017_00002);
        return TRUE;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        EraseFrameAndWindow1(&app->messageWindow, FALSE);
        RemoveWindow(&app->messageWindow);
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on

void ConnectToRanger_PrintScreen(ConnectToRangerApp *app) {
    MainMenuTextPrinter_Init(&app->statusText, &app->statusWindow, 0, NARC_msg_msg_0247_bin, 10, 2);
    MainMenuTextPrinter_SetSize(&app->statusText, 27, 4, app->nextFreeTile);
    MainMenuTextPrinter_SetFrameAndFont(&app->statusText, 1, 1);
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00072);
    app->nextFreeTile += MainMenuTextPrinter_GetTileCount(&app->statusText);

    MainMenuTextPrinter_Init(&app->titleText, &app->messageWindow, 0, NARC_msg_msg_0247_bin, 1, 1);
    MainMenuTextPrinter_SetSize(&app->titleText, 30, 4, app->nextFreeTile);
    MainMenuTextPrinter_SetFrameAndFont(&app->titleText, 0, 1);
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->titleText, 1, 1, msg_0247_00011);
    app->nextFreeTile += MainMenuTextPrinter_GetTileCount(&app->titleText);

    MainMenuUtil_InitObjTransfer();
    MainMenuUtil_InitSprites();
    MainMenuUtil_LoadSpriteResources(NARC_a_1_1_3, 14, 11, 13, 12, 0);
    app->sprite = MainMenuUtil_CreateSprite(0, app->sprite, 128, 76, 1);
    ConnectToRanger_PrintPlayerInfo(app);
}

// The "Application No." shown to the player is derived from the console's MAC address.
typedef struct RangerApplicationNo {
    u32 value;
} RangerApplicationNo;

static inline RangerApplicationNo GetApplicationNumber(u8 *macAddress) {
    RangerApplicationNo applicationNo;
    applicationNo.value = RangerLink_ReadMacAddressTail(macAddress);
    return applicationNo;
}

void ConnectToRanger_PrintPlayerInfo(ConnectToRangerApp *app) {
    MessageFormat *msgFormat = MessageFormat_New(app->heapID);
    u32 high, low;
    u8 macAddress[6];
    RangerApplicationNo applicationNo;

    MainMenuTextPrinter_Init(&app->playerText, &app->playerInfoWindow, 0, NARC_msg_msg_0247_bin, 1, 1);
    MainMenuTextPrinter_SetSize(&app->playerText, 26, 4, app->nextFreeTile);
    MainMenuTextPrinter_SetFrameAndFont(&app->playerText, 0, 1);
    app->playerText.msgFormat = msgFormat;
    BufferPlayersName(msgFormat, 0, app->profile);
    BufferIntegerAsString(msgFormat, 1, PlayerProfile_GetTrainerID_VisibleHalf(app->profile), 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->playerText, 3, 13, msg_0247_00068);
    app->playerText.clearWindow = FALSE;
    MainMenuTextPrinter_SetTextPos(&app->playerText, 0, GetFontAttribute(0, 1));
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->playerText, -1, -1, msg_0247_00070);

    OS_GetMacAddress(macAddress);
    applicationNo = GetApplicationNumber(macAddress);
    RangerLink_SplitDecimal(applicationNo.value, &high, &low);
    BufferIntegerAsString(msgFormat, 0, high, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(msgFormat, 1, low, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    app->playerText.rightAlign = TRUE;
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->playerText, -1, -1, msg_0247_00071);
    MessageFormat_Delete(msgFormat);
}

#pragma force_active reset

void ConnectToRanger_FreeGraphics(OverlayManager *manager) {
    ConnectToRangerApp *app = OverlayManager_GetData(manager);
    MainMenuUtil_FreeSprites();
    if (WindowIsInUse(&app->messageWindow) == TRUE) {
        ClearWindowTilemapAndCopyToVram(&app->messageWindow);
        RemoveWindow(&app->messageWindow);
        ClearWindowTilemapAndCopyToVram(&app->statusWindow);
        RemoveWindow(&app->statusWindow);
        ClearWindowTilemapAndCopyToVram(&app->playerInfoWindow);
        RemoveWindow(&app->playerInfoWindow);
    }
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(app->bgConfig);
}

BOOL ConnectToRanger_Init(OverlayManager *manager, int *state) {
    ConnectToRangerApp *app = MainMenuUtil_CreateAppData(manager, HEAP_ID_83, sizeof(ConnectToRangerApp), 0x20000);
    app->heapID = HEAP_ID_83;
    app->bgConfig = BgConfig_Alloc(HEAP_ID_83);
    app->saveData = ((UnkStruct_02111868_sub *)OverlayManager_GetArgs(manager))->saveData;
    app->profile = Save_PlayerData_GetProfile(app->saveData);
    app->pokedex = Save_Pokedex_Get(app->saveData);
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    MainMenuUtil_InitWork(app->heapID);
    app->mysteryGift = Save_MysteryGift_Get(app->saveData);
    app->unk_14C = 9;
    app->nextFreeTile = 40;
    app->unk_154 = 0;
    app->connectState = RANGER_CONNECT_START;
    app->linkInitialised = FALSE;
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570);
    Sound_SetSceneAndPlayBGM(9, SEQ_GS_WIFI_PRESENT, 1);
    return TRUE;
}

#pragma force_active on

BOOL ConnectToRanger_Main(OverlayManager *manager, int *state) {
    ConnectToRangerApp *app = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        ConnectToRanger_SetupGraphics(app);
        if (!Pokedex_IsEnabled(app->pokedex)) {
            MainMenuUtil_StartFade(1, 1, state, 13);
        } else {
            *state = 2;
        }
        break;
    case 1:
        if (!ConnectToRanger_ShowPokedexRequiredMessage(app)) {
            MainMenuUtil_SetFadeWhite(1);
            MainMenuUtil_StartFade(0, 12, state, 13);
        }
        break;
    case 2:
        ConnectToRanger_PrintScreen(app);
        if (!SaveMysteryGift_FindAvailable(app->mysteryGift)) {
            app->statusText.speed = 1;
            app->textPrinterId = MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00022);
            MainMenuUtil_StartFade(1, 3, state, 13);
        } else {
            app->unk_14C = 7;
            MainMenuUtil_StartFade(1, 4, state, 13);
        }
        break;
    case 3:
        if (!TextPrinterCheckActive((u8)app->textPrinterId)) {
            if (gSystem.newKeys & PAD_BUTTON_A) {
                MainMenuUtil_SetFadeWhite(1);
                MainMenuUtil_StartFade(0, 12, state, 13);
            }
        }
        break;
    case 4:
        app->connectResult = ConnectToRanger_RunTransfer(app);
        if (app->connectResult == 5) {
            MainMenuUtil_SetFadeWhite(1);
            MainMenuUtil_StartFade(0, 12, state, 13);
        } else if (app->connectResult == 4) {
            MainMenuUtil_SetFadeWhite(1);
            MainMenuUtil_StartFade(0, 12, state, 13);
        }
        break;
    case 5:
        ConnectToRanger_LoadSubScreenBg(app);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
        WonderCardAlbum_DrawCardOnBg(app->bgConfig, &app->wonderCard, HEAP_ID_83);
        MainMenuUtil_StartFade(1, 6, state, 13);
        app->wonderCard.unk104.unk8E_2 = FALSE;
        break;
    case 6:
        if (gSystem.newKeys) {
            MainMenuUtil_StartFade(0, 11, state, 13);
        }
        break;
    case 11:
        OS_ResetSystem(0);
        break;
    case 12:
        ConnectToRanger_FreeGraphics(manager);
        return TRUE;
    case 13:
        MainMenuUtil_WaitFade(state);
        break;
    }
    MainMenuUtil_UpdateSprites();
    return FALSE;
}

#pragma force_active reset

BOOL ConnectToRanger_Exit(OverlayManager *manager, int *state) {
    Heap_Destroy(HEAP_ID_89);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_83);
    return TRUE;
}

#pragma force_active on

// UnkWonderCardSubstruct_104 with its flag bits seen as one byte.
typedef struct WonderCardInfoFlags {
    u16 name[36];
    u32 version;
    u16 id;
    u8 flags;
} WonderCardInfoFlags;

#define WONDER_CARD_FLAG_BYTE(card) (((WonderCardInfoFlags *)&(card)->unk104)->flags)

// Builds the Wonder Card of the Manaphy Egg received from Pokemon Ranger, stores it and shows it.
void ConnectToRanger_StoreManaphyEggCard(ConnectToRangerApp *app) {
    WonderCard *wonderCard = &app->wonderCard;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    u32 heartGold, soulSilver, diamond, pearl, platinum;
    RTCDate date;

    MI_CpuFill8(wonderCard, 0, sizeof(WonderCard));
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, app->heapID);
    msgFormat = MessageFormat_New(app->heapID);
    wonderCard->tag = 7;
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0247_00078, app->heapID);
    CopyStringToU16Array(string, wonderCard->unk104.name, NELEMS(wonderCard->unk104.name));
    String_Delete(string);
    heartGold = ov74_02235A68(8);
    soulSilver = ov74_02235A68(7);
    diamond = ov74_02235A68(12);
    pearl = ov74_02235A68(10);
    platinum = ov74_02235A68(11);
    wonderCard->unk104.version = soulSilver | (pearl | platinum | diamond) | heartGold;
    wonderCard->unk104.id = 1;
    wonderCard->unk104.unique = FALSE;
    wonderCard->unk104.unk8E_1 = FALSE;
    // unk8E_2 = unk8E_3 = TRUE, written to the flag byte (the bitfield stores do not match)
    WONDER_CARD_FLAG_BYTE(wonderCard) = WONDER_CARD_FLAG_BYTE(wonderCard) | 4;
    WONDER_CARD_FLAG_BYTE(wonderCard) = WONDER_CARD_FLAG_BYTE(wonderCard) | 8;
    wonderCard->unk104.unk8E_5 = FALSE;
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0247_00077, app->heapID);
    CopyStringToU16Array(string, wonderCard->text, NELEMS(wonderCard->text));
    String_Delete(string);
    wonderCard->shareMax = 0;
    wonderCard->monIcon[0] = SPECIES_MANAPHY;
    wonderCard->monIcon[1] = 0;
    wonderCard->monIcon[2] = 0;
    wonderCard->shareCount = 0;
    GF_RTC_CopyDate(&date);
    wonderCard->receiveDate = RTC_ConvertDateToDay(&date);
    MessageFormat_Delete(msgFormat);
    DestroyMsgData(msgData);
    // The card's header has the layout of a MysteryGift.
    SaveMysteryGift_TryInsertGift(app->mysteryGift, (MysteryGift *)wonderCard, 3);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    MainMenuUtil_ShowCardSubScreen(app->bgConfig, wonderCard);
}

#pragma force_active reset

void ConnectToRanger_SetTimeout(ConnectToRangerApp *app, int frames) {
    app->timer = frames;
}

// Counts down the timeout and ends the link on timeout, link loss (if checkLink) or a press of cancelKeys.
BOOL ConnectToRanger_CheckTimeoutOrCancel(ConnectToRangerApp *app, u32 *connectState, BOOL checkLink, u32 cancelKeys) {
    app->timer--;
    if (app->timer == 0) {
        RangerLink_RequestEnd();
        *connectState = RANGER_CONNECT_TIMEOUT;
        return FALSE;
    }
    if (checkLink && !RangerLink_IsConnected()) {
        RangerLink_RequestEnd();
        *connectState = RANGER_CONNECT_LINK_LOST;
        return FALSE;
    }
    if (cancelKeys && (gSystem.newKeys & cancelKeys)) {
        RangerLink_RequestEnd();
        *connectState = RANGER_CONNECT_CANCEL;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
#pragma push
#pragma opt_lifetimes off
#pragma opt_propagation off
#pragma opt_common_subs off

int ConnectToRanger_RunTransfer(ConnectToRangerApp *app) {
    // connectState and linkInitialised are read through offsets computed at run time (needed to match).
    s32 stateOffset = (s32) & ((ConnectToRangerApp *)NULL)->connectState;
    s32 linkInitialisedOffset = stateOffset - 4;
#define CONNECT_STATE (*(u32 *)((u8 *)app + stateOffset))

    if (*(BOOL *)((u8 *)app + linkInitialisedOffset) == TRUE) {
        WirelessStatusIcon_SetLevel(3 - RangerLink_GetLinkLevel());
        RangerLink_Update();
        RangerLink_SetChannel(1);
    }

    switch (CONNECT_STATE) {
    case RANGER_CONNECT_START:
        ConnectToRanger_EncodePlayerData(app);
        CommWireless_StartUp();
        app->linkStarted = TRUE;
        CONNECT_STATE = RANGER_CONNECT_WAIT_LINK;
        break;
    case RANGER_CONNECT_WAIT_LINK:
        if (CommWireless_IsStartedUp()) {
            RangerLink_Init(ov74_0223BD5C, app->linkWork);
            app->linkInitialised = TRUE;
            WirelessStatusIcon_Create();
            CONNECT_STATE = RANGER_CONNECT_SEARCH;
        }
        break;
    case RANGER_CONNECT_SEARCH:
        RangerLink_StartChild();
        ConnectToRanger_SetTimeout(app, 1600);
        app->waitingIcon = WaitingIcon_New(&app->statusWindow, 10);
        CONNECT_STATE = RANGER_CONNECT_WAIT_PARTNER;
        break;
    case RANGER_CONNECT_WAIT_PARTNER: {
        int index = RangerLink_CountPeers();
        if (index) {
            app->partnerIndex = index - 1;
            ConnectToRanger_SetTimeout(app, 1800);
            CONNECT_STATE = RANGER_CONNECT_CONNECT_PARTNER;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, FALSE, PAD_BUTTON_B);
        break;
    }
    case RANGER_CONNECT_CONNECT_PARTNER:
        if (RangerLink_CanSelectParent(app->partnerIndex)) {
            RangerLink_SelectParent(app->partnerIndex);
            CONNECT_STATE = RANGER_CONNECT_WAIT_CONNECTED;
            app->timer = 1800;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, FALSE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_WAIT_CONNECTED:
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, FALSE, PAD_BUTTON_B);
        if (RangerLink_IsConnected()) {
            CONNECT_STATE = RANGER_CONNECT_FOUND;
        }
        break;
    case RANGER_CONNECT_FOUND:
        MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00031);
        Sprite_SetDrawFlag(app->sprite, FALSE);
        WaitingIcon_Delete(app->waitingIcon);
        app->waitingIcon = NULL;
        PlaySE(SEQ_SE_DP_SELECT);
        app->timer = 1800;
        CONNECT_STATE = RANGER_CONNECT_WAIT_A;
        break;
    case RANGER_CONNECT_WAIT_A:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            CONNECT_STATE = RANGER_CONNECT_SEND_REQUEST;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_SEND_REQUEST:
        RangerLink_StartSendList();
        CONNECT_STATE = RANGER_CONNECT_WAIT_REQUEST_SENT;
        app->timer = 1800;
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_WAIT_REQUEST_SENT:
        if (RangerLink_IsSendDone()) {
            RangerLink_StartSendNames();
            CONNECT_STATE = RANGER_CONNECT_WAIT_TOUCH;
            app->timer = 3200;
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00035);
            PlaySE(SEQ_SE_DP_SELECT);
            app->waitingIcon = NULL;
            return 1;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_WAIT_TOUCH:
        if (RangerLink_IsSendDone()) {
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00073);
            Sprite_SetDrawFlag(app->sprite, TRUE);
            ConnectToRanger_PrintPlayerInfo(app);
            CONNECT_STATE = RANGER_CONNECT_SEND_PLAYER;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_SEND_PLAYER:
        RangerLink_StartCommand(ov74_0223C1F4, 42, 1);
        CONNECT_STATE = RANGER_CONNECT_WAIT_PLAYER_SENT;
        app->timer = 3200;
        break;
    case RANGER_CONNECT_WAIT_PLAYER_SENT:
        if (RangerLink_IsCommandDone()) {
            CONNECT_STATE = RANGER_CONNECT_SAVE_DELAY;
            app->timer = 60;
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00073);
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_SAVE_DELAY:
        if (--app->timer == 0) {
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00074);
            app->waitingIcon = WaitingIcon_New(&app->statusWindow, 10);
            ConnectToRanger_StoreManaphyEggCard(app);
            MainMenuUtil_StartSave(app->saveData);
            CONNECT_STATE = RANGER_CONNECT_SAVING;
        }
        break;
    case RANGER_CONNECT_SAVING:
        if (MainMenuUtil_RunSave() == 1) {
            CONNECT_STATE = RANGER_CONNECT_RANDOM_DELAY;
            app->timer = (s32)OS_GetTickLo() % 54 + 6;
        }
        break;
    case RANGER_CONNECT_RANDOM_DELAY:
        if (--app->timer == 0) {
            CONNECT_STATE = RANGER_CONNECT_SEND_SAVED;
        }
        break;
    case RANGER_CONNECT_SEND_SAVED:
        RangerLink_StartCommand(ov74_0223C1F4, 42, 2);
        CONNECT_STATE = RANGER_CONNECT_WAIT_ACK;
        app->timer = 1200;
        break;
    case RANGER_CONNECT_WAIT_ACK:
        if (RangerLink_IsCommandAcknowledged()) {
            MainMenuUtil_ResumeSave();
            CONNECT_STATE = RANGER_CONNECT_FINISH_SAVE;
            app->timer = 800;
        } else {
            if (--app->timer == 0) {
                RangerLink_RequestEnd();
                CONNECT_STATE = RANGER_CONNECT_TIMEOUT;
                app->timer = 120;
            }
            if (!RangerLink_IsConnected()) {
                RangerLink_RequestEnd();
                CONNECT_STATE = RANGER_CONNECT_LINK_LOST;
                app->timer = 120;
            }
        }
        break;
    case RANGER_CONNECT_FINISH_SAVE:
        if (MainMenuUtil_GetSaveState() == 3) {
            RangerLink_RequestEnd();
            CONNECT_STATE = RANGER_CONNECT_LINK_LOST;
            app->timer = 120;
        } else if (MainMenuUtil_GetSaveState() != 2) {
            MainMenuUtil_RunSave();
        } else if (RangerLink_IsCommandDone() || !RangerLink_IsConnected()) {
            CONNECT_STATE = RANGER_CONNECT_DONE_DELAY;
            app->timer = 1;
            RangerLink_RequestEnd();
            break;
        }
        if (--app->timer == 0) {
            RangerLink_RequestEnd();
            CONNECT_STATE = RANGER_CONNECT_TIMEOUT;
            app->timer = 10;
        }
        // fallthrough
    case RANGER_CONNECT_DONE_DELAY:
        if (--app->timer == 0) {
            CONNECT_STATE = RANGER_CONNECT_DONE;
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00075);
            Sprite_SetDrawFlag(app->sprite, FALSE);
            WaitingIcon_Delete(app->waitingIcon);
            PlaySE(SEQ_SE_DP_SELECT);
            app->waitingIcon = NULL;
        }
        break;
    case RANGER_CONNECT_DONE:
        if (RangerLink_IsFinished()) {
            if (app->linkStarted == TRUE) {
                CommWireless_Terminate();
                WirelessStatusIcon_Destroy();
                app->linkStarted = FALSE;
            }
            if (gSystem.newKeys & PAD_BUTTON_A) {
                CONNECT_STATE = RANGER_CONNECT_END;
                return 4;
            }
        }
        break;
    case RANGER_CONNECT_LINK_LOST:
        CONNECT_STATE = RANGER_CONNECT_TIMEOUT;
        // fallthrough
    case RANGER_CONNECT_TIMEOUT:
        MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00076);
        Sprite_SetDrawFlag(app->sprite, FALSE);
        if (app->waitingIcon) {
            WaitingIcon_Delete(app->waitingIcon);
        }
        app->waitingIcon = NULL;
        CONNECT_STATE = RANGER_CONNECT_ERROR;
        break;
    case RANGER_CONNECT_ERROR:
        if (RangerLink_IsFinished()) {
            if (app->linkStarted == TRUE) {
                CommWireless_Terminate();
                WirelessStatusIcon_Destroy();
                app->linkStarted = FALSE;
            }
            if (gSystem.newKeys & PAD_BUTTON_A) {
                OS_ResetSystem(0);
            }
        }
        break;
    case RANGER_CONNECT_CANCEL:
        if (RangerLink_IsFinished() && app->linkStarted == TRUE) {
            CommWireless_Terminate();
            WirelessStatusIcon_Destroy();
            app->linkStarted = FALSE;
            if (app->waitingIcon) {
                WaitingIcon_Delete(app->waitingIcon);
            }
            return 5;
        }
        break;
    }
    return 0;
}

#undef CONNECT_STATE

#pragma pop
