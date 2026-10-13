#include "application/main_menu/mystery_gift_app.h"

#include "global.h"

#include "constants/gx.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "application/main_menu/lock_capsule_card.h"
#include "application/main_menu/main_menu.h"
#include "application/main_menu/main_menu_internal.h"
#include "application/main_menu/main_menu_shared.h"
#include "application/main_menu/main_menu_util.h"
#include "application/main_menu/mystery_gift_app_internal.h"
#include "application/main_menu/mystery_gift_beacon.h"
#include "application/main_menu/mystery_gift_wfc.h"
#include "application/main_menu/mystery_gift_wireless.h"
#include "application/main_menu/wonder_card_album.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0247.h"

#include "bg_window.h"
#include "font.h"
#include "geometry_math.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "link_synchronization.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "main.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "render_window_internal.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "title_screen.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "comm_wireless.h"
#include "communication_core.h"
#include "wireless_status_icon.h"

// Main menu option "Mystery Gift": receives a gift (from a friend or a distribution over DS wireless, or
// over Nintendo Wi-Fi Connection), stores it in the save, and opens the Wonder Card viewers.

// The menu tables of the module (still in asm data); the main menu's first entries are addressed from the
// start of the block.
typedef struct MysteryGiftMenuTables {
    MysteryGiftMenuItem wfcPromptItems[2];      // 0x00
    MysteryGiftMenuItem wirelessPromptItems[2]; // 0x10
    MysteryGiftMenuItem unk_20[2];              // 0x20
    MysteryGiftMenuItem friendPromptItems[2];   // 0x30
    MysteryGiftMenuItem unk_40[2];              // 0x40
    ListMenuTemplate listMenuTemplate;          // 0x50
    MysteryGiftMenuItem mainMenuItems[2];       // 0x70
} MysteryGiftMenuTables;

// Link state shared with the overlay's link helpers.
typedef struct MysteryGiftLinkState {
    OverlayManager *manager;          // 0x00, the Mystery Gift app (MysteryGiftApp_GetOverlayManager)
    void *linkWork;                   // 0x04
    MysteryGiftMenuFunc selectedFunc; // 0x08, last list menu entry chosen
    u32 count;                        // 0x0C
    u32 state;                        // 0x10
} MysteryGiftLinkState;

// Other modules (not in a header yet).
extern BOOL CTRDG_IsExisting();
extern int WM_GetLinkLevel(void);
extern fx32 FX_Sqrt(fx32 value);
extern void CommWireless_Terminate(void);
extern void WirelessStatusIcon_SetLevel(int a0);
extern void CommState_StartBeaconWatch(SaveData *saveData);
extern void CommState_StopBeaconWatch(void);
extern u32 CommState_GetSeenGames(void);
extern void CommState_SetErrorWatch(s8 a0, s8 a1);

extern MysteryGiftLinkState ov74_0223D0A8;

extern const GraphicsModes _0223B390;
extern const BgTemplate _0223B3A0;
extern const MysteryGiftMenuTables ov74_0223C220;
extern const MysteryGiftMenuItem ov74_0223C230[];
extern const MysteryGiftMenuItem ov74_0223C240[];
extern const MysteryGiftMenuItem ov74_0223C250[];
extern const MysteryGiftMenuItem ov74_0223C260[];
extern const ListMenuTemplate ov74_0223C270;
extern const OverlayManagerTemplate _0223B410;
extern const OverlayManagerTemplate _0223B420;
extern const MysteryGiftMenuItem ov74_0223C2A0[];
extern const MysteryGiftMenuItem ov74_0223C2B0[];

#define MYSTERY_GIFT_PARTICLE_COUNT 80

// A sprite of the gift animation flying to (destX, destY), then circling it (MysteryGiftAnim_UpdateParticle).
struct MysteryGiftParticle {
    u32 unk_00;       // 0x00
    int frames;       // 0x04
    BOOL done;        // 0x08
    Sprite *sprite;   // 0x0C
    fx32 destX;       // 0x10
    fx32 destY;       // 0x14
    u32 unk_18;       // 0x18
    fx32 speed;       // 0x1C
    fx32 minDistance; // 0x20
    int state;        // 0x24, 0: flying, 1: circling
    int angle;        // 0x28
    int kind;         // 0x2C, path shape while circling
    int delay;        // 0x30, frames before it appears
    int phase;        // 0x34
    int amplitude;    // 0x38
    int grow;         // 0x3C
    int radiusX;      // 0x40
    int radiusY;      // 0x44
    SysTask *task;    // 0x48
}; // size: 0x4C

// Work of the task that plays the gift animation (MysteryGiftAnim_Task).
struct MysteryGiftAnimData {
    int state;                                                 // 0x0000
    u32 unk_0004;                                              // 0x0004
    s8 brightness;                                             // 0x0008
    MainMenuUtilSprites *spriteSystem;                         // 0x000C
    SpriteResourcesHeader resHeaders[2];                       // 0x0010, main and sub screen
    SpriteResource *mainResources[4];                          // 0x0058
    u8 unk_0068[0x8];                                          // 0x0068
    SpriteResource *subResources[4];                           // 0x0070
    u8 unk_0080[0x8];                                          // 0x0080
    MysteryGiftParticle sparkles[MYSTERY_GIFT_PARTICLE_COUNT]; // 0x0088, main screen
    MysteryGiftParticle ribbons[MYSTERY_GIFT_PARTICLE_COUNT];  // 0x1848, sub screen
    MysteryGiftParticle giftMain;                              // 0x3008
    MysteryGiftParticle giftSub;                               // 0x3054
    int *status;                                               // 0x30A0, MysteryGiftAppData.animStatus
}; // size: 0x30A4

// The particle loops walk a MysteryGiftAnimData pointer one particle at a time and access element 0 of the
// arrays through it (matching).
#define NEXT_PARTICLE(cursor) ((MysteryGiftAnimData *)((u8 *)(cursor) + sizeof(MysteryGiftParticle)))

typedef struct MysteryGiftDelayTable {
    int values[21];
} MysteryGiftDelayTable;

extern const MysteryGiftDelayTable sMysteryGiftSparkleDelays;

typedef struct MysteryGiftVec2 {
    float x;
    float y;
} MysteryGiftVec2;

#pragma force_active on

void MysteryGiftApp_SetWaitingIconVisible(MysteryGiftAppData *data, BOOL visible) {
    if (visible == TRUE) {
        if (data->waitingIcon == NULL) {
            data->waitingIcon = WaitingIcon_New(&data->messageWindow, 1);
        }
    } else {
        if (data->waitingIcon != NULL) {
            WaitingIcon_DeleteWithoutRestore(data->waitingIcon);
        }
        data->waitingIcon = NULL;
    }
}

#pragma force_active reset

void ov74_02229E60(MysteryGiftAppData *data, BOOL visible) {
    MysteryGiftApp_SetWaitingIconVisible(data, visible);
}

#pragma force_active on

void MysteryGiftApp_FreeGraphics(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    int i;

    MainMenuUtil_FreeSprites();
    if (data->listMenuItems != NULL) {
        ListMenuItems_Delete(data->listMenuItems);
    }
    if (data->listMenu != NULL) {
        DestroyListMenu(data->listMenu, NULL, NULL);
    }
    ClearWindowTilemapAndCopyToVram(&data->messageWindow);
    RemoveWindow(&data->messageWindow);
    if (WindowIsInUse(&data->titleWindow)) {
        ClearWindowTilemapAndCopyToVram(&data->titleWindow);
        RemoveWindow(&data->titleWindow);
    }
    for (i = 0; i < 3; i++) {
        if (data->windows[i].bgConfig != NULL) {
            ClearWindowTilemapAndCopyToVram(&data->windows[i]);
            RemoveWindow(&data->windows[i]);
        }
    }
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(data->bgConfig);
}

#pragma force_active reset

void MysteryGiftApp_InitSprites(MysteryGiftAppData *data) {
    MainMenuUtil_InitObjTransfer();
    MainMenuUtil_InitSprites();
    MainMenuUtil_LoadSpriteResources(NARC_a_1_1_3, 14, 11, 13, 12, 0);
}

void MysteryGiftApp_SetSpriteAnim(MysteryGiftAppData *data, int animSeq) {
    if (animSeq != -1) {
        data->sprite = MainMenuUtil_CreateSprite(0, data->sprite, 128, 100, animSeq);
    } else {
        Sprite_SetDrawFlag(data->sprite, FALSE);
    }
}

void MysteryGiftApp_EraseWindowIfInUse(Window *window, BOOL dontCopyToVram) {
    if (WindowIsInUse(window) == TRUE) {
        EraseFrameAndWindow1(window, dontCopyToVram);
    }
}

void MysteryGiftApp_ClearWindowIfInUse(Window *window, BOOL dontCopyToVram) {
    if (WindowIsInUse(window) == TRUE) {
        ClearFrameAndWindow2(window, dontCopyToVram);
    }
}

int MysteryGiftApp_StartSearch(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (data->receiveMode != 2) {
        MysteryGiftApp_PrintMessage(manager, &data->messageWindow, 3);
    }
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    if (data->receiveMode != 2) {
        MysteryGiftComm_StartChild(data, 15);
        data->timeout = 7200;
        WirelessStatusIcon_Create();
        MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
        return 7;
    }
    data->giftCheckSource = 19;
    return 19;
}

int MysteryGiftApp_AcceptGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    int state = data->giftCheckResult;

    if (state == 1) {
        return 49;
    }
    if ((u32)(state - 2) <= 2) {
        return 49;
    }
    if (state == 5) {
        return 49;
    }
    if (data->receiveMode == 1) {
        MysteryGiftComm_ConnectToChannel(data->channel);
    }
    MysteryGiftApp_PrintMessage(manager, &data->messageWindow, 6);
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    if (data->receiveMode == 1) {
        MysteryGiftApp_SetSpriteAnim(data, 0);
        MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
    }
    if (data->receiveMode == 1) {
        LinkSync_StartSync(171);
        return 9;
    }
    return 21;
}

#pragma force_active on

WonderCard *MysteryGiftApp_GetReceivedCard(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    WonderCard *card = &data->receivedCard;
    WonderCardInfo *header = &data->receivedHeader;

    switch (data->receiveMode) {
    case 1:
        memcpy(card, MysteryGiftComm_GetRecvBuffer(0, NULL, 0), sizeof(WonderCard));
        MainMenuUtil_DecryptWonderCard(&data->receivedHeader, &data->receivedCard, HEAP_ID_84);
        break;
    case 3: {
        u8 *src = data->recvBuffer;
        memcpy(card, src + sizeof(WonderCardInfo), sizeof(WonderCard));
        memcpy(header, src, sizeof(WonderCardInfo));
        break;
    }
    }
    return card;
}

#pragma force_active reset

BOOL ov74_0222A0E4(WonderCard *card) {
    int eggLocation;

    switch (card->tag) {
    case MG_TAG_POKEMON_MOVIE:
        return TRUE;
    case MG_TAG_ITEM:
        if (((u32 *)card->data.raw)[1] == 1) {
            return TRUE;
        }
        break;
    case MG_TAG_POKEMON:
    case MG_TAG_EGG:
        eggLocation = GetMonData(&card->data.pokemon.mon, MON_DATA_EGG_LOCATION, NULL);
        if ((eggLocation >= 3 && eggLocation <= 14) || (eggLocation >= 65 && eggLocation <= 71)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// Callback while the received gift is being saved.
void MysteryGiftApp_WaitForSave(MysteryGiftAppData *data) {
    if (data->saveCancelled == 1) {
        data->callback = NULL;
    }
    if ((u32)(MainMenuUtil_RunSave() - 2) <= 1) {
        if (ov74_0222A0E4(&data->receivedCard) != TRUE) {
            PlaySE(SEQ_SE_DP_SAVE);
        }
        data->callback = NULL;
    }
}

#pragma force_active on

void MysteryGiftApp_SaveReceivedGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    WonderCardInfo *header = &data->receivedHeader;
    WonderCard *card = &data->receivedCard;
    SaveData *saveData = ((MainOverlayArgs *)OverlayManager_GetArgs(manager))->saveData;
    BOOL isCard;
    MysteryGiftSave *mysteryGift;
    RTCDate date;

    if (header->unk8E_2 == 0) {
        isCard = FALSE;
    } else {
        card->shareCount = 0;
        isCard = TRUE;
        if (header->unk8E_4 == 0) {
            card->shareMax = 0;
        }
        GF_RTC_CopyDate(&date);
        card->receiveDate = RTC_ConvertDateToDay(&date);
    }
    mysteryGift = Save_MysteryGift_Get(saveData);
    SaveMysteryGift_ReceivedFlagSet(mysteryGift, header->id);
    if (header->unk8E_2 == 1 && card->tag == MG_TAG_ITEM && card->data.item == ITEM_LOCK_CAPSULE) {
        SaveMysteryGift_TrySetSpecialCard(mysteryGift, card);
    } else if (isCard == FALSE) {
        SaveMysteryGift_TryInsertGift(mysteryGift, (MysteryGift *)card, 3);
    } else {
        SaveMysteryGift_TryInsertCard(mysteryGift, card);
    }
    MainMenuUtil_StartSave(saveData);
    data->saveCancelled = 0;
    data->callback = MysteryGiftApp_WaitForSave;
}

#pragma force_active reset

void MysteryGiftApp_ShowReceivePrompt(OverlayManager *manager, int mode) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    int msgId;
    Window *window;

    if (mode == 0) {
        msgId = 4;
    } else {
        msgId = -1;
    }
    window = &data->windows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, ov74_0223C260, 2, window, msgId);
}

int MysteryGiftApp_DeclineSearch(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
    if (data->receiveMode == 1) {
        MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
    }
    WirelessStatusIcon_Destroy();
    return MysteryGiftApp_SelectReceiveGift(manager);
}

#pragma force_active on

int MysteryGiftApp_SelectGetFromFriend(OverlayManager *manager) {
    Window *window;
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    data->receiveMode = MainMenuUtil_GetCartridgeGiftSize() ? 2 : 1;
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[1], FALSE);
    window = &data->windows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, ov74_0223C250, 2, window, msg_0247_00002);
    return 5;
}

int MysteryGiftApp_SelectGetViaWfc(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    data->receiveMode = 4;
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[1], FALSE);
    Window *window = &data->windows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, ov74_0223C220.wfcPromptItems, 2, window, msg_0247_00080);
    return 5;
}

int MysteryGiftApp_SelectGetViaWireless(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    data->receiveMode = 3;
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[1], FALSE);
    Window *window = &data->windows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, ov74_0223C230, 2, window, msg_0247_00079);
    return 5;
}

void MysteryGiftApp_AskReceiveGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    Window *window = &data->windows[2];

    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, ov74_0223C240, 2, window, msg_0247_00004);
}

#pragma force_active reset

int MysteryGiftApp_DeclineAcceptGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
    WirelessStatusIcon_Destroy();
    if (data->receiveMode == 1) {
        MysteryGiftComm_Free();
    }
    return MysteryGiftApp_SelectReceiveGift(manager);
}

#pragma force_active on

int MysteryGiftApp_ConfirmWirelessSearch(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00003);
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
    ov74_0223D0A8.state = 37;
    return 32;
}

#pragma force_active reset

int MysteryGiftApp_ConfirmReceiveGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if ((u32)(data->giftCheckResult - 2) <= 2) {
        return 49;
    }
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00007);
    return 33;
}

#pragma force_active on

int MysteryGiftApp_DeclineReceiveGift(OverlayManager *manager) {
    switch (ov74_0223D0A8.state) {
    case 40:
    case 41:
    case 48:
        if (MysteryGiftBeacon_Stop()) {
            ov74_0223D0A8.state = 43;
        }
        break;
    }
    return 36;
}

#pragma force_active reset

int MysteryGiftApp_ConfirmWfcSearch(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00003);
    MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
    data->wfcState = 0x1000;
    data->wfcCallback = NULL;
    return 27;
}

int MysteryGiftApp_CancelReceiveMenu(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_EraseWindowIfInUse(&data->windows[1], FALSE);
    MysteryGiftApp_ShowMainMenu(manager, 196, msg_0247_00000);
    return 3;
}

#pragma force_active on

int MysteryGiftApp_SelectReceiveGift(OverlayManager *manager) {
    Window *window;
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    Save_MysteryGift_Get(data->saveData);
    if (data->beaconWatchState == 30) {
        data->beaconWatchTimer = 1;
        MysteryGiftApp_UpdateBeaconWatch(data);
    }
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[0], FALSE);
    window = &data->windows[1];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 15, 9, 16, 8, 0, 292);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, ov74_0223C2B0, 4, window, msg_0247_00001);
    return 4;
}

#pragma force_active reset

int MysteryGiftApp_SelectCheckCard(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (data->beaconWatchState == 30) {
        data->beaconWatchTimer = 1;
        MysteryGiftApp_UpdateBeaconWatch(data);
    }
    MainMenuUtil_StartFade(0, 54, data->mainState, 2);
    return -1;
}

int MysteryGiftApp_SelectViewLockCapsuleCard(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (data->beaconWatchState == 30) {
        data->beaconWatchTimer = 1;
        MysteryGiftApp_UpdateBeaconWatch(data);
    }
    MainMenuUtil_StartFade(0, 55, data->mainState, 2);
    return -1;
}

int MysteryGiftApp_SelectExit(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (data->beaconWatchState == 30) {
        data->beaconWatchTimer = 1;
    }
    MainMenuUtil_SetFadeWhite(1);
    MainMenuUtil_StartFade(0, 53, data->mainState, 2);
    return -1;
}

void MysteryGiftApp_InitBgLayer(BgConfig *bgConfig, GFBgLayer bgId, u32 screenOffset, u32 charOffset) {
    BgTemplate template = _0223B3A0;

    template.screenBase = screenOffset >> 11;
    template.charBase = charOffset >> 14;
    InitBgFromTemplate(bgConfig, bgId, &template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, bgId);
}

void MysteryGiftApp_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes = _0223B390;

    SetBothScreensModesAndDisable(&modes);
    MysteryGiftApp_InitBgLayer(bgConfig, GF_BG_LYR_MAIN_0, 0xF000, 0);
    MysteryGiftApp_InitBgLayer(bgConfig, GF_BG_LYR_MAIN_1, 0xF800, 0x8000);
    MysteryGiftApp_InitBgLayer(bgConfig, GF_BG_LYR_SUB_0, 0x3800, 0);
    MysteryGiftApp_InitBgLayer(bgConfig, GF_BG_LYR_SUB_1, 0x7800, 0x4000);
}

void MysteryGiftApp_LoadSubScreenBg(BgConfig *bgConfig) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 0x20, HEAP_ID_84);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, bgConfig, GF_BG_LYR_SUB_1, 0, 0x1400, TRUE, HEAP_ID_84);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, TRUE, HEAP_ID_84);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_SUB_1, 0, 0, 32, 24, 8);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_SUB_1);
}

void MysteryGiftApp_LoadMainScreenBg(BgConfig *bgConfig) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_8_OFFSET, 0x20, HEAP_ID_84);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x1400, TRUE, HEAP_ID_84);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_84);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 32, 24, 8);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_1);
    MysteryGiftApp_LoadSubScreenBg(bgConfig);
}

#pragma force_active on

void MysteryGiftApp_CreateListMenu(OverlayManager *manager, const MysteryGiftMenuItem *items, int count, Window *window, int msgId) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (data->listMenuItems != NULL) {
        ListMenuItems_Delete(data->listMenuItems);
    }
    data->listMenuItems = ListMenuItems_New(count, HEAP_ID_84);
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_84);
    int i = 0;
    if (count > 0) {
        do {
            ListMenuItems_AppendFromMsgData(data->listMenuItems, data->msgData, items->msgId, (int)items->func);
            i++;
            items++;
        } while (i < count);
    }
    DestroyMsgData(data->msgData);
    ListMenuTemplate template = ov74_0223C270;
    template.items = data->listMenuItems;
    template.maxShowed = count;
    template.totalItems = template.maxShowed;
    template.window = window;
    if (data->listMenu != NULL) {
        DestroyListMenu(data->listMenu, NULL, NULL);
    }
    data->listMenu = ListMenuInit(&template, 0, 0, HEAP_ID_84);
    if (msgId != -1) {
        MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msgId);
    }
}

void MysteryGiftApp_ShowMainMenu(OverlayManager *manager, int baseTile, int msgId) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    int count = 0;
    MysteryGiftSave *mysteryGift = Save_MysteryGift_Get(data->saveData);
    MysteryGiftMenuItem items[4];
    Window *window = &data->windows[0];

    items[count++] = ov74_0223C220.mainMenuItems[0];
    if (SaveMysteryGift_HasAnyCard(mysteryGift)) {
        items[count++] = ov74_0223C220.mainMenuItems[1];
    }
    if (SaveMysteryGift_SpecialCardTagIsValid(mysteryGift)) {
        items[count++] = ov74_0223C2A0[0];
    }
    items[count] = ov74_0223C2A0[1];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 6, 7, 20, (u8)(2 * (count + 1)), 0, (u16)baseTile);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, items, count + 1, window, msgId);
}

void MysteryGiftApp_PrintMessage(OverlayManager *manager, Window *window, int msgId) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_84);
    data->msgFormat = MessageFormat_New(HEAP_ID_84);
    FillWindowPixelBuffer(window, GetFontAttribute(1, 6));
    String *string = data->string;
    if (string == NULL) {
        string = ReadMsgData_ExpandPlaceholders(data->msgFormat, data->msgData, msgId, HEAP_ID_84);
    }
    data->textPrinterId = AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, data->textSpeed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    if (data->string == NULL) {
        String_Delete(string);
    }
    DrawFrameAndWindow2(window, FALSE, 1, 2);
    DestroyMsgData(data->msgData);
    MessageFormat_Delete(data->msgFormat);
    data->textSpeed = 255;
}

#pragma force_active reset

void MysteryGiftApp_PrintGiftTitle(OverlayManager *manager, Window *window, const u16 *text) {
    String *string;

    OverlayManager_GetData(manager);
    string = String_New(37, HEAP_ID_84);
    CopyU16ArrayToStringN(string, text, 36);
    FillWindowPixelBuffer(window, GetFontAttribute(0, 6));
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    DrawFrameAndWindow1(window, FALSE, 31, 3);
    String_Delete(string);
}

void MysteryGiftApp_HandleListMenuInput(OverlayManager *manager, int *state, MysteryGiftMenuFunc cancelFunc) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    u32 input = ListMenu_ProcessInput(data->listMenu);
    int nextState;

    switch (input) {
    case LIST_CANCEL:
        PlaySE(SEQ_SE_DP_SELECT);
        if (cancelFunc != NULL) {
            nextState = cancelFunc(manager);
            if (nextState != -1) {
                *state = nextState;
                return;
            }
        }
        break;
    case LIST_NOTHING_CHOSEN:
        break;
    default:
        PlaySE(SEQ_SE_DP_SELECT);
        if (input != 0) {
            ov74_0223D0A8.selectedFunc = (MysteryGiftMenuFunc)input;
            nextState = ((MysteryGiftMenuFunc)input)(manager);
            if (nextState != -1) {
                *state = nextState;
                return;
            }
        }
        break;
    }
}

#pragma force_active on

BOOL MysteryGiftApp_SetupScreen(OverlayManager *manager, MysteryGiftAppData *data) {
    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, HEAP_ID_84);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_84);
    LoadUserFrameGfx2(data->bgConfig, GF_BG_LYR_MAIN_0, 1, 2, Options_GetFrame(data->options), HEAP_ID_84);
    LoadUserFrameGfx1(data->bgConfig, GF_BG_LYR_MAIN_0, 31, 3, 1, HEAP_ID_84);
    *(vu16 *)HW_BG_PLTT = RGB(12, 12, 31);
    if (!WindowIsInUse(&data->messageWindow)) {
        AddWindowParameterized(data->bgConfig, &data->messageWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 0, 40);
    }
    MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00000);
    MysteryGiftApp_ShowMainMenu(manager, 196, msg_0247_00000);
    MysteryGiftApp_LoadMainScreenBg(data->bgConfig);
    return TRUE;
}

void MysteryGiftApp_RunWfcDownload(OverlayManager *manager, int *state) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    switch (MysteryGiftWfc_Run(manager, &data->wfcState)) {
    case 1:
        memcpy(&data->receivedHeader, data->wfcRecvBuffer, sizeof(WonderCardInfo) + sizeof(WonderCard));
        MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
        data->giftCheckSource = 27;
        *state = 19;
        data->resultDelay = 0;
        break;
    case 2:
    case 3:
        MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
        data->resultDelay = 0;
        *state = 17;
        break;
    case 4:
        ClearWindowTilemapAndCopyToVram(&data->wfcErrorWindow);
        RemoveWindow(&data->wfcErrorWindow);
        BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_MAIN_0);
        PlaySE(SEQ_SE_DP_SELECT);
        MysteryGiftApp_ShowMainMenu(manager, 196, msg_0247_00000);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        *state = 3;
        break;
    }
}

void MysteryGiftApp_ShowWfcError(MysteryGiftAppData *data) {
    MainMenuTextPrinter printer;
    int msgId = data->wfcErrorMsgId;
    MessageFormat *msgFormat;

    if (msgId == -1) {
        msgId = 11;
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_MAIN_0);
    msgFormat = MessageFormat_New(HEAP_ID_84);
    BufferIntegerAsString(msgFormat, 0, data->wfcErrorCode, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    MainMenuTextPrinter_Init(&printer, &data->wfcErrorWindow, 1, NARC_msg_msg_0800_bin, 31, 3);
    MainMenuTextPrinter_SetSize(&printer, 24, 16, 100);
    MainMenuTextPrinter_SetFrameAndFont(&printer, 0, 1);
    printer.msgFormat = msgFormat;
    MainMenuTextPrinter_AddWindowAndPrint(data->bgConfig, &printer, 4, 4, msgId);
    MessageFormat_Delete(msgFormat);
}

#pragma force_active reset

int MysteryGiftApp_ShowCannotReceiveMessage(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    u32 result = data->giftCheckResult;
    int msgId;

    if (result == 3) {
        msgId = msg_0247_00022;
    } else if (result == 4) {
        msgId = msg_0247_00026;
    } else if (result == 2) {
        msgId = msg_0247_00023;
    } else if (result == 1) {
        msgId = msg_0247_00028;
    } else if (result == 5) {
        return MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00009, 52);
    }
    return MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msgId, 51);
}

int MysteryGiftApp_PrintMessageAndWait(OverlayManager *manager, Window *window, int msgId, int nextState) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (window != NULL && msgId != 0) {
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, HEAP_ID_84);
        MessageFormat *msgFormat = MessageFormat_New(HEAP_ID_84);
        data->string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, HEAP_ID_84);
        data->textSpeed = 1;
        MysteryGiftApp_PrintMessage(manager, window, msgId);
        data->stateAfterMessage = nextState;
        MessageFormat_Delete(msgFormat);
        DestroyMsgData(msgData);
    } else if (!TextPrinterCheckActive((u8)data->textPrinterId)) {
        String_Delete(data->string);
        data->string = NULL;
        data->textSpeed = 0;
        return data->stateAfterMessage;
    }
    return 50;
}

void MysteryGiftApp_StopReceiving(MysteryGiftAppData *data) {
    switch (data->receiveMode) {
    case 1:
        MysteryGiftComm_Free();
        WirelessStatusIcon_Destroy();
        break;
    case 0:
    case 2:
    case 3:
    case 4:
        break;
    }
}

BOOL MysteryGiftApp_Init(OverlayManager *manager, int *state) {
    MysteryGiftAppData *data;
    MainOverlayArgs *args;

    MysteryGiftApp_SetOverlayManager(manager);
    Heap_Create(HEAP_ID_3, HEAP_ID_84, 0x30000);
    data = OverlayManager_CreateAndGetData(manager, sizeof(MysteryGiftAppData), HEAP_ID_84);
    memset(data, 0, sizeof(MysteryGiftAppData));
    data->bgConfig = BgConfig_Alloc(HEAP_ID_84);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    WonderCard_SetGraphicsBanks();
    MysteryGiftApp_InitBgLayers(data->bgConfig);
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    Sound_SetSceneAndPlayBGM(10, SEQ_GS_WIFI_PRESENT, 1);
    MainMenuUtil_SetCryptoHeap(HEAP_ID_84);
    if (MainMenuUtil_GetCartridgeGiftSize()) {
        data->receiveMode = 2;
        Sys_SetAgbCartridgeVersion(gGameVersion);
        MainMenuUtil_EnableCartridgePullOutCheck(TRUE);
    }
    data->beaconWatchState = 29;
    args = OverlayManager_GetArgs(manager);
    data->saveData = args->saveData;
    data->options = Save_PlayerData_GetOptionsAddr(data->saveData);
    data->textSpeed = 255;
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570);
    return TRUE;
}

#pragma force_active on

void MysteryGiftAnim_LoadResources(MysteryGiftAnimData *anim) {
    anim->mainResources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(anim->spriteSystem->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_1_1_3, 43, TRUE, 20001, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_84);
    anim->mainResources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(anim->spriteSystem->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_1_1_3, 40, FALSE, 20001, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_84);
    anim->mainResources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(anim->spriteSystem->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_1_1_3, 42, TRUE, 20001, GF_GFX_RES_TYPE_CELL, HEAP_ID_84);
    anim->mainResources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(anim->spriteSystem->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_1_1_3, 41, TRUE, 20001, GF_GFX_RES_TYPE_ANIM, HEAP_ID_84);
    anim->subResources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(anim->spriteSystem->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_1_1_3, 43, TRUE, 20002, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_84);
    anim->subResources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(anim->spriteSystem->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_1_1_3, 40, FALSE, 20002, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_84);
    anim->subResources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(anim->spriteSystem->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_1_1_3, 42, TRUE, 20002, GF_GFX_RES_TYPE_CELL, HEAP_ID_84);
    anim->subResources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(anim->spriteSystem->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_1_1_3, 41, TRUE, 20002, GF_GFX_RES_TYPE_ANIM, HEAP_ID_84);
    SpriteTransfer_CreateCharTransferTask(anim->mainResources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreateCharTransferTask(anim->subResources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(anim->mainResources[GF_GFX_RES_TYPE_PLTT]);
    SpriteTransfer_CreatePlttTransferTask(anim->subResources[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&anim->resHeaders[0], 20001, 20001, 20001, 20001, -1, -1, FALSE, 0, anim->spriteSystem->resMan[GF_GFX_RES_TYPE_CHAR], anim->spriteSystem->resMan[GF_GFX_RES_TYPE_PLTT], anim->spriteSystem->resMan[GF_GFX_RES_TYPE_CELL], anim->spriteSystem->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    SpriteResourcesHeader *headers = anim->resHeaders;
    CreateSpriteResourcesHeader(&headers[1], 20002, 20002, 20002, 20002, -1, -1, FALSE, 0, anim->spriteSystem->resMan[GF_GFX_RES_TYPE_CHAR], anim->spriteSystem->resMan[GF_GFX_RES_TYPE_PLTT], anim->spriteSystem->resMan[GF_GFX_RES_TYPE_CELL], anim->spriteSystem->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

#pragma force_active reset

Sprite *MysteryGiftAnim_CreateSprite(MysteryGiftAnimData *anim, NNS_G2D_VRAM_TYPE whichScreen) {
    SpriteTemplate template;
    Sprite *sprite;
    int headerIdx = whichScreen == NNS_G2D_VRAM_TYPE_2DMAIN ? 0 : 1;

    template.spriteList = anim->spriteSystem->spriteList;
    template.header = &anim->resHeaders[headerIdx];
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.position.x = FX32_CONST(128);
    template.position.y = FX32_CONST(32);
    template.drawPriority = 10;
    template.whichScreen = whichScreen;
    template.heapID = HEAP_ID_84;
    if (whichScreen == NNS_G2D_VRAM_TYPE_2DSUB) {
        template.position.y += FX32_CONST(256);
    }
    sprite = Sprite_CreateAffine(&template);
    if (sprite != NULL) {
        Sprite_SetAnimActiveFlag(sprite, TRUE);
        Sprite_SetPriority(sprite, 0);
        Sprite_SetAnimCtrlSeq(sprite, 0);
        Sprite_SetDrawFlag(sprite, TRUE);
    }
    return sprite;
}

void MysteryGiftAnim_CreateGiftMainSprite(MysteryGiftAnimData *anim) {
    VecFx32 *matrix;

    anim->giftMain.sprite = MysteryGiftAnim_CreateSprite(anim, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_TryChangeAnimSeq(anim->giftMain.sprite, 2);
    Sprite_SetAnimationFrame(anim->giftMain.sprite, 0);
    Sprite_SetDrawFlag(anim->giftMain.sprite, FALSE);
    Sprite_SetDrawPriority(anim->giftMain.sprite, 0);
    Sprite_SetAffineOverwriteMode(anim->giftMain.sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    matrix = Sprite_GetMatrixPtr(anim->giftMain.sprite);
    matrix->x = FX32_CONST(128);
    matrix->y = FX32_CONST(96);
}

void MysteryGiftAnim_CreateGiftSubSprite(MysteryGiftAnimData *anim) {
    VecFx32 *matrix;

    anim->giftSub.sprite = MysteryGiftAnim_CreateSprite(anim, NNS_G2D_VRAM_TYPE_2DSUB);
    Sprite_TryChangeAnimSeq(anim->giftSub.sprite, 4);
    Sprite_SetAnimationFrame(anim->giftSub.sprite, 0);
    Sprite_SetDrawFlag(anim->giftSub.sprite, FALSE);
    Sprite_SetDrawPriority(anim->giftSub.sprite, 0);
    Sprite_SetAffineOverwriteMode(anim->giftSub.sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    matrix = Sprite_GetMatrixPtr(anim->giftSub.sprite);
    matrix->x = FX32_CONST(128);
    matrix->y = FX32_CONST(256);
}

void MysteryGiftAnim_DeleteGiftMainSprite(MysteryGiftAnimData *anim) {
    Sprite_Delete(anim->giftMain.sprite);
    anim->giftMain.sprite = NULL;
}

void MysteryGiftAnim_DeleteGiftSubSprite(MysteryGiftAnimData *anim) {
    Sprite_Delete(anim->giftSub.sprite);
    anim->giftSub.sprite = NULL;
}

#pragma force_active on

void MysteryGiftAnim_UpdateSprites(MysteryGiftAnimData *anim) {
    int i;
    MysteryGiftAnimData *cursor;

    i = 0;
    cursor = anim;
    for (; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor)) {
        if (cursor->sparkles[0].sprite != NULL) {
            Sprite_UpdateAnim(cursor->sparkles[0].sprite, FX32_CONST(2));
        }
        if (cursor->ribbons[0].sprite != NULL) {
            Sprite_UpdateAnim(cursor->ribbons[0].sprite, FX32_CONST(2));
        }
    }
    if (anim->giftMain.sprite != NULL) {
        Sprite_UpdateAnim(anim->giftMain.sprite, FX32_CONST(2));
    }
    if (anim->giftSub.sprite != NULL) {
        Sprite_UpdateAnim(anim->giftSub.sprite, FX32_CONST(2));
    }
}

void MysteryGiftAnim_CreateSparkles(MysteryGiftAnimData *anim) {
    int i;
    MysteryGiftAnimData *cursor;

    i = 0;
    cursor = anim;
    for (; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor)) {
        cursor->sparkles[0].sprite = MysteryGiftAnim_CreateSprite(anim, NNS_G2D_VRAM_TYPE_2DMAIN);
        Sprite_TryChangeAnimSeq(cursor->sparkles[0].sprite, 1);
        Sprite_SetAnimationFrame(cursor->sparkles[0].sprite, 0);
    }
}

#pragma force_active reset

void ov74_0222B2C4(MysteryGiftAnimData *anim) {
    s32 i;
    MysteryGiftAnimData *cursor = anim;

    for (i = 0; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor)) {
        if (i < 20) {
            cursor->sparkles[0].sprite = MysteryGiftAnim_CreateSprite(anim, NNS_G2D_VRAM_TYPE_2DMAIN);
            Sprite_TryChangeAnimSeq(cursor->sparkles[0].sprite, 1);
            Sprite_SetAnimationFrame(cursor->sparkles[0].sprite, 0);
        } else {
            cursor->sparkles[0].sprite = NULL;
        }
    }
}

#pragma force_active on

void MysteryGiftAnim_CreateRibbons(MysteryGiftAnimData *anim) {
    int i = 0;
    MysteryGiftAnimData *cursor = anim;

    for (; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor)) {
        cursor->ribbons[0].sprite = MysteryGiftAnim_CreateSprite(anim, NNS_G2D_VRAM_TYPE_2DSUB);
        Sprite_TryChangeAnimSeq(cursor->ribbons[0].sprite, 5);
        Sprite_SetAnimationFrame(cursor->ribbons[0].sprite, 0);
    }
}

#pragma force_active reset

void MysteryGiftAnim_DeleteSparkles(MysteryGiftAnimData *anim) {
    s32 i;
    MysteryGiftAnimData *cursor = anim;

    for (i = 0; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor)) {
        if (cursor->sparkles[0].sprite != NULL) {
            Sprite_Delete(cursor->sparkles[0].sprite);
            cursor->sparkles[0].sprite = NULL;
            SysTask_Destroy(cursor->sparkles[0].task);
        }
    }
}

void MysteryGiftAnim_DeleteRibbons(MysteryGiftAnimData *anim) {
    s32 i;
    MysteryGiftAnimData *cursor = anim;

    for (i = 0; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor)) {
        Sprite_Delete(cursor->ribbons[0].sprite);
        cursor->ribbons[0].sprite = NULL;
        SysTask_Destroy(cursor->ribbons[0].task);
    }
}

#pragma force_active on
#pragma push
#pragma opt_repositioncode off

void ov74_0222B3A4(MysteryGiftAnimData *anim) {
    MysteryGiftDelayTable delaysCopy;
    u32 delayPtr;
    MysteryGiftDelayTable delays;
    VecFx32 pos;
    int i;
    // Walks the particles through a volatile pointer and reads the delay table through a copy (matching).
    volatile MysteryGiftAnimData *cursor = anim;
    MysteryGiftParticle *taskData;

    i = 0;
    delayPtr = (u32)delaysCopy.values;
    taskData = &anim->sparkles[0];
    delays = sMysteryGiftSparkleDelays;
    do {
        int value;
        float rounded;

        cursor->sparkles[0].destX = FX32_CONST(128);
        cursor->sparkles[0].destY = FX32_CONST(96);
        cursor->sparkles[0].speed = FX32_CONST(4);
        cursor->sparkles[0].minDistance = FX32_CONST(5);
        cursor->sparkles[0].kind = i % 4;
        cursor->sparkles[0].phase = 0;
        cursor->sparkles[0].frames = 0;
        value = i / 10 + 4;
        if (value > 0) {
            rounded = 0.5f + (float)(value << FX32_SHIFT);
        } else {
            rounded = (float)(value << FX32_SHIFT) - 0.5f;
        }
        cursor->sparkles[0].speed = (int)rounded;
        if (i > 50) {
            cursor->sparkles[0].delay = i + 260 + (int)LCRandom() % 5;
        } else if (i > 40) {
            cursor->sparkles[0].delay = i + 250 + (int)LCRandom() % 5;
        } else if (i > 30) {
            cursor->sparkles[0].delay = i + 240 + (int)LCRandom() % 5;
        } else if (i > 25) {
            cursor->sparkles[0].delay = i + 230 + (int)LCRandom() % 5;
        } else if (i > 20) {
            cursor->sparkles[0].delay = i + 220 + (int)LCRandom() % 5;
        } else {
            delaysCopy = delays;
            cursor->sparkles[0].delay = *(int *)delayPtr;
        }
        cursor->sparkles[0].delay = (int)(0.7 * ((cursor->sparkles[0].delay + 1) / 2));
        cursor->sparkles[0].delay++;
        cursor->sparkles[0].amplitude = 15;
        cursor->sparkles[0].radiusX = cursor->sparkles[0].amplitude;
        cursor->sparkles[0].radiusY = cursor->sparkles[0].amplitude;
        pos = *Sprite_GetMatrixPtr(cursor->sparkles[0].sprite);
        if ((int)LCRandom() % 224 + 16 > 0) {
            rounded = 0.5f + (float)(((int)LCRandom() % 224 + 16) << FX32_SHIFT);
        } else {
            rounded = (float)(((int)LCRandom() % 224 + 16) << FX32_SHIFT) - 0.5f;
        }
        pos.x = (int)rounded;
        pos.y = 0;
        Sprite_SetMatrix(cursor->sparkles[0].sprite, &pos);
        Sprite_SetDrawFlag(cursor->sparkles[0].sprite, FALSE);
        cursor->sparkles[0].task = SysTask_CreateOnMainQueue((SysTaskFunc)MysteryGiftAnim_UpdateParticle, taskData, 6);
        cursor = NEXT_PARTICLE(cursor);
        taskData++;
        delayPtr += sizeof(int);
        i++;
    } while (i < MYSTERY_GIFT_PARTICLE_COUNT);
}

#pragma pop
#pragma force_active reset

void MysteryGiftAnim_StartSparkleTasks(MysteryGiftAnimData *anim) {
    MysteryGiftAnimData *cursor = anim;
    MysteryGiftParticle *taskData = (MysteryGiftParticle *)cursor;
    int i;

    i = 0;
    taskData = (MysteryGiftParticle *)((u8 *)taskData + offsetof(MysteryGiftAnimData, sparkles));
    for (; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor), taskData++) {
        VecFx32 position;
        u32 angle;
        s32 radius;

        if (cursor->sparkles[0].sprite == NULL) {
            continue;
        }
        cursor->sparkles[0].destX = FX32_CONST(128);
        cursor->sparkles[0].destY = FX32_CONST(96);
        cursor->sparkles[0].speed = FX32_CONST(12);
        cursor->sparkles[0].minDistance = FX32_CONST(1);
        cursor->sparkles[0].kind = 255;
        cursor->sparkles[0].phase = 0;
        cursor->sparkles[0].frames = 0;
        cursor->sparkles[0].done = FALSE;
        if (i > 30) {
            cursor->sparkles[0].delay = (s32)(LCRandom() % 10) + 15;
        } else if (i > 20) {
            cursor->sparkles[0].delay = (s32)(LCRandom() % 10) + 10;
        } else if (i > 10) {
            cursor->sparkles[0].delay = (s32)(LCRandom() % 5) + 10;
        } else {
            cursor->sparkles[0].delay = (s32)(LCRandom() % 5) + 5;
        }
        cursor->sparkles[0].delay = 0;
        cursor->sparkles[0].radiusX = cursor->sparkles[0].amplitude;
        cursor->sparkles[0].radiusY = cursor->sparkles[0].amplitude;
        angle = LCRandom() % 360;
        radius = (s32)(LCRandom() % 32) + 64;
        position.x = cursor->sparkles[0].destX + GF_SinDeg((u16)angle) * radius;
        position.y = cursor->sparkles[0].destY + GF_CosDeg((u16)angle) * radius;
        Sprite_SetMatrix(cursor->sparkles[0].sprite, &position);
        Sprite_TryChangeAnimSeq(cursor->sparkles[0].sprite, 1);
        Sprite_SetDrawFlag(cursor->sparkles[0].sprite, FALSE);
        cursor->sparkles[0].task = SysTask_CreateOnMainQueue((SysTaskFunc)MysteryGiftAnim_UpdateParticle, taskData, 6);
    }
}

void MysteryGiftAnim_StartRibbonTasks(MysteryGiftAnimData *anim) {
    MysteryGiftParticle *taskData;
    MysteryGiftAnimData *cursor = anim;
    int i;

    taskData = (MysteryGiftParticle *)((u8 *)cursor + offsetof(MysteryGiftAnimData, ribbons));
    i = 0;
    for (; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor), taskData++) {
        VecFx32 position;

        cursor->ribbons[0].destX = FX32_CONST(128);
        cursor->ribbons[0].destY = FX32_CONST(384);
        cursor->ribbons[0].speed = FX32_CONST(4);
        cursor->ribbons[0].minDistance = FX32_CONST(5);
        cursor->ribbons[0].kind = i % 2 + 4;
        cursor->ribbons[0].phase = 0;
        cursor->ribbons[0].frames = 0;
        cursor->ribbons[0].grow = LCRandom() % 4 + 2;
        {
            s32 value = i / 30 + 4;
            float rounded;

            rounded = value > 0 ? (float)(value << FX32_SHIFT) + 0.5f : (float)(value << FX32_SHIFT) - 0.5f;
            cursor->ribbons[0].speed = (s32)rounded;
        }
        if (i > 60) {
            cursor->ribbons[0].amplitude = 5;
            cursor->ribbons[0].delay = LCRandom() % 10 + 70;
        } else if (i > 50) {
            cursor->ribbons[0].amplitude = 5;
            cursor->ribbons[0].delay = LCRandom() % 10 + 60;
        } else if (i > 40) {
            cursor->ribbons[0].amplitude = 5;
            cursor->ribbons[0].delay = LCRandom() % 5 + 50;
        } else if (i > 30) {
            cursor->ribbons[0].amplitude = 5;
            cursor->ribbons[0].delay = LCRandom() % 10 + 40;
        } else if (i > 20) {
            cursor->ribbons[0].amplitude = 5;
            cursor->ribbons[0].delay = LCRandom() % 10 + 30;
        } else if (i > 10) {
            cursor->ribbons[0].amplitude = 5;
            cursor->ribbons[0].delay = LCRandom() % 5 + 20;
        } else {
            cursor->ribbons[0].amplitude = 5;
            cursor->ribbons[0].delay = LCRandom() % 5 + 10;
        }
        cursor->ribbons[0].radiusX = cursor->ribbons[0].amplitude;
        cursor->ribbons[0].radiusY = cursor->ribbons[0].amplitude;
        position = *Sprite_GetMatrixPtr(cursor->ribbons[0].sprite);
        position.x = FX32_CONST(128);
        position.y = FX32_CONST(384);
        Sprite_SetMatrix(cursor->ribbons[0].sprite, &position);
        Sprite_SetDrawFlag(cursor->ribbons[0].sprite, TRUE);
        cursor->ribbons[0].task = SysTask_CreateOnMainQueue((SysTaskFunc)MysteryGiftAnim_UpdateParticle, taskData, 6);
    }
}

// Computes the step of length `speed` from (fromX, fromY) towards (x, y); FALSE once closer than speed or minDistance.
BOOL MysteryGiftAnim_StepTowards(s32 x, s32 y, float fromX, float fromY, float *outX, float *outY, float speed, int minDistance) {
    MysteryGiftVec2 result;
    MysteryGiftVec2 delta;
    MysteryGiftVec2 origin;
    float squared;
    float distance;

    origin.x = 0.0f;
    origin.y = 0.0f;
    delta.x = (float)x - fromX;
    delta.y = (float)y - fromY;
    result.x = 0.0f;
    result.y = 0.0f;
    squared = delta.x * delta.x + delta.y * delta.y;
    distance = (float)FX_Sqrt(FX_F32_TO_FX32(squared)) / FX32_ONE;
    // minDistance is read as an s16 from its stack slot (matching).
    if (distance < speed || (float)*(s16 *)&minDistance > distance || distance == 0.0f) {
        return FALSE;
    }
    result.x = delta.x * speed / distance;
    result.y = delta.y * speed / distance;
    *outX = result.x + origin.x;
    *outY = result.y + origin.y;
    return TRUE;
}

#pragma force_active on

void MysteryGiftAnim_UpdateParticle(SysTask *task, MysteryGiftParticle *particle) {
    VecFx32 pos;
    float dx;
    float dy;
    float x;
    float y;
    float speed;
    float rounded;
    float angleX;
    float angleY;
    int minDistance;

    if (particle->delay) {
        Sprite_SetAnimationFrame(particle->sprite, 0);
        --particle->delay;
        return;
    }
    Sprite_SetDrawFlag(particle->sprite, TRUE);
    pos = *Sprite_GetMatrixPtr(particle->sprite);
    x = (float)pos.x / 4096.0f;
    y = (float)pos.y / 4096.0f;
    speed = (float)particle->speed / 4096.0f;
    minDistance = (int)((float)particle->minDistance / 4096.0f);
    if (MysteryGiftAnim_StepTowards((s16)(particle->destX >> FX32_SHIFT), (s16)(particle->destY >> FX32_SHIFT), x, y, &dx, &dy, speed, minDistance) && particle->state == 0) {
        if (dx > 0) {
            rounded = 0.5f + 4096.0f * dx;
        } else {
            rounded = 4096.0f * dx - 0.5f;
        }
        pos.x += (int)rounded;
        if (dy > 0) {
            rounded = 0.5f + 4096.0f * dy;
        } else {
            rounded = 4096.0f * dy - 0.5f;
        }
        pos.y += (int)rounded;
        Sprite_SetMatrix(particle->sprite, &pos);
        return;
    }
    switch (particle->state) {
    case 0:
        if (particle->destX - pos.x > 0) {
            angleX = 0.5f + (float)((particle->destX - pos.x) << FX32_SHIFT);
        } else {
            angleX = (float)((particle->destX - pos.x) << FX32_SHIFT) - 0.5f;
        }
        if (particle->destY - pos.y > 0) {
            angleY = 0.5f + (float)((particle->destY - pos.y) << FX32_SHIFT);
        } else {
            angleY = (float)((particle->destY - pos.y) << FX32_SHIFT) - 0.5f;
        }
        particle->angle = FX_Atan2Idx((int)angleY, (int)angleX);
        ++particle->state;
        return;
    case 1:
        if (particle->frames < 20) {
            ++particle->frames;
        } else {
            particle->done = TRUE;
        }
        particle->angle += 8;
        if (particle->kind == 0) {
            particle->phase += 8;
            particle->phase %= 360;
            particle->radiusX = (GF_CosDeg((u16)particle->phase) * particle->amplitude) >> FX32_SHIFT;
            pos.x = particle->destX + GF_SinDeg((u16)particle->angle) * particle->radiusX;
            pos.y = particle->destY + GF_CosDeg((u16)particle->angle) * particle->radiusY;
        } else if (particle->kind == 1) {
            particle->phase += 8;
            particle->phase %= 360;
            particle->radiusY = (GF_SinDeg((u16)particle->phase) * particle->amplitude) >> FX32_SHIFT;
            pos.x = particle->destX + GF_SinDeg((u16)particle->angle) * particle->radiusX;
            pos.y = particle->destY + GF_CosDeg((u16)particle->angle) * particle->radiusY;
        } else if (particle->kind == 2) {
            particle->phase += 8;
            particle->phase %= 360;
            particle->radiusY = (GF_SinDeg((u16)particle->phase) * particle->amplitude) >> FX32_SHIFT;
            pos.x = particle->destX + -1 * (particle->radiusX * GF_SinDeg((u16)particle->angle));
            pos.y = particle->destY + -1 * (particle->radiusY * GF_CosDeg((u16)particle->angle));
        } else if (particle->kind == 3) {
            particle->phase += 8;
            particle->phase %= 360;
            particle->radiusX = (GF_CosDeg((u16)particle->phase) * particle->amplitude) >> FX32_SHIFT;
            pos.x = particle->destX + -1 * (particle->radiusX * GF_SinDeg((u16)particle->angle));
            pos.y = particle->destY + -1 * (particle->radiusY * GF_CosDeg((u16)particle->angle));
        } else if (particle->kind == 4) {
            if (particle->amplitude < 96) {
                particle->amplitude += particle->grow;
            }
            particle->radiusX = particle->amplitude;
            particle->radiusY = particle->amplitude;
            pos.x = particle->destX + GF_SinDeg((u16)particle->angle) * particle->radiusX;
            pos.y = particle->destY + GF_CosDeg((u16)particle->angle) * particle->radiusY;
        } else if (particle->kind == 5) {
            if (particle->amplitude < 96) {
                particle->amplitude += particle->grow;
            }
            particle->radiusX = particle->amplitude;
            particle->radiusY = particle->amplitude;
            pos.x = particle->destX + -1 * (particle->radiusX * GF_SinDeg((u16)particle->angle));
            pos.y = particle->destY + -1 * (particle->radiusY * GF_CosDeg((u16)particle->angle));
        } else {
            particle->frames = 10;
            particle->speed += FX32_HALF;
            x = (float)pos.x / 4096.0f;
            y = (float)pos.y / 4096.0f;
            speed = (float)particle->speed / 4096.0f;
            if (MysteryGiftAnim_StepTowards((s16)(particle->destX >> FX32_SHIFT), (s16)(particle->destY >> FX32_SHIFT), x, y, &dx, &dy, speed, 0)) {
                if (dx > 0) {
                    rounded = 0.5f + 4096.0f * dx;
                } else {
                    rounded = 4096.0f * dx - 0.5f;
                }
                pos.x += (int)rounded;
                if (dy > 0) {
                    rounded = 0.5f + 4096.0f * dy;
                } else {
                    rounded = 4096.0f * dy - 0.5f;
                }
                pos.y += (int)rounded;
                Sprite_SetMatrix(particle->sprite, &pos);
            } else {
                particle->done = TRUE;
            }
        }
        particle->angle %= 360;
        Sprite_SetMatrix(particle->sprite, &pos);
        break;
    }
}

#pragma force_active reset

void MysteryGiftAnim_ApplyBrightnessMain(MysteryGiftAnimData *anim) {
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BD, anim->brightness);
}

void MysteryGiftAnim_ApplyBrightnessSub(MysteryGiftAnimData *anim) {
    G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BD, anim->brightness);
}

void MysteryGiftAnim_ApplyBrightness(MysteryGiftAnimData *anim) {
    MysteryGiftAnim_ApplyBrightnessMain(anim);
    MysteryGiftAnim_ApplyBrightnessSub(anim);
}

void MysteryGiftAnim_SetMaskColorsBlack(void) {
    BG_SetMaskColor(GF_BG_LYR_MAIN_1, RGB_BLACK);
    BG_SetMaskColor(GF_BG_LYR_SUB_1, RGB_BLACK);
}

// Steps the brightness of both screens towards 0 (mode 0), -16 (mode 1) or 16 (mode 2); TRUE when reached.
BOOL MysteryGiftAnim_StepBrightness(MysteryGiftAnimData *anim, int mode, int step) {
    BOOL done = TRUE;
    int value;

    switch (mode) {
    case 0:
        value = anim->brightness;
        if (value > 0) {
            value -= step;
            if (value > 0) {
                anim->brightness = value;
                done = FALSE;
            } else {
                anim->brightness = 0;
            }
        } else if (value < 0) {
            value += step;
            if (value < 0) {
                anim->brightness = value;
                done = FALSE;
            } else {
                anim->brightness = 0;
            }
        }
        break;
    case 1:
        value = anim->brightness - step;
        if (value > -16) {
            anim->brightness = value;
            done = FALSE;
        } else {
            anim->brightness = -16;
        }
        break;
    case 2:
        value = anim->brightness + step;
        if (value < 16) {
            anim->brightness = value;
            done = FALSE;
        } else {
            anim->brightness = 16;
        }
        break;
    }
    MysteryGiftAnim_ApplyBrightness(anim);
    return done;
}

// As MysteryGiftAnim_StepBrightness, main screen only.
BOOL MysteryGiftAnim_StepBrightnessMain(MysteryGiftAnimData *anim, int mode, int step) {
    BOOL done = TRUE;
    int value;

    switch (mode) {
    case 0:
        value = anim->brightness;
        if (value > 0) {
            value -= step;
            if (value > 0) {
                anim->brightness = value;
                done = FALSE;
            } else {
                anim->brightness = 0;
            }
        } else if (value < 0) {
            value += step;
            if (value < 0) {
                anim->brightness = value;
                done = FALSE;
            } else {
                anim->brightness = 0;
            }
        }
        break;
    case 1:
        value = anim->brightness - step;
        if (value > -16) {
            anim->brightness = value;
            done = FALSE;
        } else {
            anim->brightness = -16;
        }
        break;
    case 2:
        value = anim->brightness + step;
        if (value < 16) {
            anim->brightness = value;
            done = FALSE;
        } else {
            anim->brightness = 16;
        }
        break;
    }
    MysteryGiftAnim_ApplyBrightnessMain(anim);
    return done;
}

void MysteryGiftAnim_Init(MysteryGiftAnimData *anim) {
    anim->brightness = 0;
    MysteryGiftAnim_ApplyBrightness(anim);
    MysteryGiftAnim_LoadResources(anim);
    MysteryGiftAnim_CreateSparkles(anim);
    ov74_0222B3A4(anim);
    MysteryGiftAnim_CreateGiftMainSprite(anim);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

// Task playing the gift animation while the gift is received.
void MysteryGiftAnim_Task(SysTask *task, MysteryGiftAnimData *anim) {
    int count;
    int allDone;
    int i;
    MysteryGiftAnimData *cursor;
    VecFx32 *mainMatrix;
    VecFx32 *subMatrix;

    switch (anim->state) {
    case 0:
        MysteryGiftAnim_Init(anim);
        anim->state = 1;
        break;
    case 1:
        if (MysteryGiftAnim_StepBrightness(anim, 1, 2)) {
            anim->state = 2;
        }
        break;
    case 2:
        count = 0;
        allDone = TRUE;
        i = 0;
        cursor = anim;
        while (i < MYSTERY_GIFT_PARTICLE_COUNT) {
            if (cursor->sparkles[0].done) {
                count++;
            } else {
                allDone = FALSE;
            }
            i++;
            cursor = NEXT_PARTICLE(cursor);
        }
        if (count > 50) {
            Sprite_TryChangeAnimSeq(anim->giftMain.sprite, 4);
        } else if (count > 30) {
            Sprite_TryChangeAnimSeq(anim->giftMain.sprite, 3);
        } else if (count > 7) {
            Sprite_SetDrawFlag(anim->giftMain.sprite, TRUE);
        }
        if (allDone || count == 79) {
            anim->state = 3;
            MysteryGiftAnim_DeleteSparkles(anim);
        }
        break;
    case 3:
        if (*anim->status == 2) {
            ov74_0222B2C4(anim);
            MysteryGiftAnim_StartSparkleTasks(anim);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
            MysteryGiftAnim_SetMaskColorsBlack();
            anim->state = 4;
        }
        break;
    case 4:
        count = 0;
        allDone = TRUE;
        i = 0;
        cursor = anim;
        while (i < MYSTERY_GIFT_PARTICLE_COUNT) {
            if (cursor->sparkles[0].sprite != NULL) {
                if (cursor->sparkles[0].done) {
                    count++;
                } else {
                    allDone = FALSE;
                }
            }
            i++;
            cursor = NEXT_PARTICLE(cursor);
        }
        MysteryGiftAnim_StepBrightnessMain(anim, 2, 2);
        if (allDone || count == 19) {
            PlaySE(SEQ_SE_DP_SAVE);
            anim->state = 5;
            MysteryGiftAnim_DeleteSparkles(anim);
            MysteryGiftAnim_CreateGiftSubSprite(anim);
        }
        break;
    case 5:
        if (MysteryGiftAnim_StepBrightnessMain(anim, 2, 2)) {
            MysteryGiftAnim_StepBrightnessMain(anim, 1, 2);
            anim->state = 6;
        }
        break;
    case 6:
        if (MysteryGiftAnim_StepBrightnessMain(anim, 1, 2)) {
            mainMatrix = Sprite_GetMatrixPtr(anim->giftMain.sprite);
            subMatrix = Sprite_GetMatrixPtr(anim->giftSub.sprite);
            if (mainMatrix->y < FX32_CONST(224)) {
                mainMatrix->y += FX32_CONST(8);
            }
            if (mainMatrix->y - FX32_CONST(192) > 0) {
                if (subMatrix->y < FX32_CONST(384)) {
                    subMatrix->y += FX32_CONST(8);
                    if (!Sprite_GetDrawFlag(anim->giftSub.sprite)) {
                        Sprite_SetDrawFlag(anim->giftSub.sprite, TRUE);
                    }
                } else {
                    anim->state = 7;
                    MysteryGiftAnim_DeleteGiftMainSprite(anim);
                    MysteryGiftAnim_CreateRibbons(anim);
                    MysteryGiftAnim_StartRibbonTasks(anim);
                    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
                }
            }
        }
        break;
    case 7:
        count = 0;
        allDone = TRUE;
        i = 0;
        cursor = anim;
        while (i < MYSTERY_GIFT_PARTICLE_COUNT) {
            if (cursor->ribbons[0].done) {
                count++;
            } else {
                allDone = FALSE;
            }
            i++;
            cursor = NEXT_PARTICLE(cursor);
        }
        if (count > 7) {
            MysteryGiftAnim_StepBrightness(anim, 2, 1);
        }
        if (count > 50) {
            Sprite_SetDrawFlag(anim->giftSub.sprite, FALSE);
        } else if (count > 30) {
            Sprite_TryChangeAnimSeq(anim->giftSub.sprite, 2);
        } else if (count > 7) {
            Sprite_TryChangeAnimSeq(anim->giftSub.sprite, 3);
        }
        if (allDone) {
            anim->state = 8;
            MysteryGiftAnim_DeleteRibbons(anim);
            MysteryGiftAnim_DeleteGiftSubSprite(anim);
        }
        break;
    case 8:
        if (MysteryGiftAnim_StepBrightness(anim, 2, 1)) {
            anim->state = 9;
        }
        break;
    case 9:
        if (MysteryGiftAnim_StepBrightness(anim, 0, 2)) {
            anim->state = 255;
        }
        break;
    default:
        *anim->status = 0;
        SysTask_Destroy(task);
        Heap_Free(anim);
        return;
    }
    MysteryGiftAnim_UpdateSprites(anim);
}

#pragma force_active on

BOOL MysteryGiftApp_Main(OverlayManager *manager, int *state) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    WonderCard *card;
    int result;

    CTRDG_IsExisting();
    if (data->beaconWatchState != 29) {
        MysteryGiftApp_UpdateBeaconWatch(data);
    }
    switch (*state) {
    case 0:
        MainMenuUtil_InitWork(HEAP_ID_84);
        data->mainState = state;
        *state = 1;
        break;
    case 1:
        MysteryGiftApp_SetupScreen(manager, data);
        MysteryGiftApp_InitSprites(data);
        MainMenuUtil_StartFade(1, 3, state, 2);
        break;
    case 2:
        MainMenuUtil_WaitFade(state);
        break;
    case 3:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_SelectExit);
        break;
    case 4:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_CancelReceiveMenu);
        break;
    case 5:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_DeclineSearch);
        break;
    case 27:
        MysteryGiftApp_RunWfcDownload(manager, state);
        break;
    case 32:
        MysteryGiftApp_UpdateWirelessSearch(data);
        if (ov74_0223D0A8.state == 45) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftApp_GetReceivedCard(manager);
            data->giftCheckResult = MysteryGift_CheckCanReceive(data->saveData, &data->receivedHeader);
            if (data->giftCheckResult == 1) {
                MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
                *state = 49;
            } else {
                if (!WindowIsInUse(&data->titleWindow)) {
                    AddWindowParameterized(data->bgConfig, &data->titleWindow, GF_BG_LYR_MAIN_0, 3, 2, 26, 4, 0, 452);
                }
                MysteryGiftApp_PrintGiftTitle(manager, &data->titleWindow, MysteryGiftBeacon_GetTitle());
                MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00004);
                MysteryGiftApp_AskReceiveGift(manager);
                *state = 31;
            }
        }
        if ((u32)(ov74_0223D0A8.state - 46) <= 1) {
            MysteryGiftApp_StopWirelessLink();
            *state = 17;
        }
        break;
    case 31:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_DeclineReceiveGift);
        MysteryGiftApp_UpdateWirelessSearch(data);
        break;
    case 33:
        card = MysteryGiftApp_GetReceivedCard(manager);
        MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
        MysteryGiftApp_SetSpriteAnim(data, 1);
        if (ov74_0222A0E4(card) == TRUE) {
            MysteryGiftAnimData *anim;

            MysteryGiftApp_SaveReceivedGift(manager);
            data->animStatus = 1;
            MysteryGiftApp_SetSpriteAnim(data, -1);
            MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
            anim = Heap_Alloc(HEAP_ID_84, sizeof(MysteryGiftAnimData));
            memset(anim, 0, sizeof(MysteryGiftAnimData));
            anim->status = &data->animStatus;
            anim->spriteSystem = MainMenuUtil_GetSpriteList();
            SysTask_CreateOnMainQueue((SysTaskFunc)MysteryGiftAnim_Task, anim, 5);
        } else {
            MainMenuUtil_ShowCardSubScreen(data->bgConfig, card);
            MysteryGiftApp_SaveReceivedGift(manager);
        }
        *state = 34;
        break;
    case 35:
        if (!data->animStatus) {
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00008, 56);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        }
        break;
    case 34:
        result = MainMenuUtil_GetSaveState();
        if (result == 4) {
            MainMenuUtil_ResumeSave();
        }
        if (result == 2) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftApp_SetSpriteAnim(data, -1);
            MysteryGiftApp_ClearWindowIfInUse(&data->messageWindow, FALSE);
            BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_MAIN_0);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
            card = &data->receivedCard;
            if (ov74_0222A0E4(card) == TRUE) {
                MainMenuUtil_ShowCardSubScreen(data->bgConfig, card);
                data->animStatus = 2;
            } else {
                PlaySE(SEQ_SE_DP_UG_020);
            }
            *state = 35;
        } else if (result == 3) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftApp_SetSpriteAnim(data, -1);
            data->receivedHeader.unk8E_2 = 0;
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00027, 56);
        }
        break;
    case 36:
        MysteryGiftApp_UpdateWirelessSearch(data);
        result = ov74_0223D0A8.state;
        if ((u32)(result - 45) <= 2) {
            if (result == 47) {
                MysteryGiftApp_StopWirelessLink();
            }
            *state = MysteryGiftApp_DeclineAcceptGift(manager);
        }
        break;
    case 7:
        data->channel = MysteryGiftComm_GetFirstParentInfo(data);
        if (data->channel != -1) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            data->giftCheckResult = MysteryGift_CheckCanReceive(data->saveData, &data->receivedHeader);
            if (!WindowIsInUse(&data->titleWindow)) {
                AddWindowParameterized(data->bgConfig, &data->titleWindow, GF_BG_LYR_MAIN_0, 3, 2, 26, 4, 0, 452);
            }
            MysteryGiftApp_PrintGiftTitle(manager, &data->titleWindow, data->receivedHeader.name);
            MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00004);
            MysteryGiftApp_ShowReceivePrompt(manager, 0);
            *state = 8;
        }
        if ((gSystem.newKeys & PAD_BUTTON_B) || --data->timeout == 0) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            *state = 17;
        }
        break;
    case 8:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_DeclineAcceptGift);
        break;
    case 9:
        result = Comm_GetLocalNetId();
        if (result && Comm_IsNetIdConnected((u16)result)) {
            if (LinkSync_IsSynced(171) == TRUE) {
                MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
                CommState_SetErrorWatch(1, 1);
                MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00007);
                MysteryGiftApp_SetSpriteAnim(data, 1);
                MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
                data->timeout = 600;
                *state = 10;
            }
        } else if (result && !Comm_IsNetIdConnected((u16)result)) {
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            MysteryGiftApp_SetSpriteAnim(data, -1);
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            *state = 16;
            break;
        }
        if ((gSystem.newKeys & PAD_BUTTON_B) || --data->timeout == 0) {
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            MysteryGiftApp_SetSpriteAnim(data, -1);
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            *state = 17;
        }
        break;
    case 10:
        if (MysteryGiftComm_IsReceived()) {
            *state = 11;
        }
        result = Comm_GetLocalNetId();
        if ((gSystem.newKeys & PAD_BUTTON_B) || --data->timeout == 0 || (result && !Comm_IsNetIdConnected((u16)result))) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            *state = 17;
        }
        break;
    case 11:
        card = MysteryGiftApp_GetReceivedCard(manager);
        MainMenuUtil_ShowCardSubScreen(data->bgConfig, card);
        CommState_SetErrorWatch(0, 0);
        MysteryGiftApp_SaveReceivedGift(manager);
        *state = 12;
        break;
    case 12:
        result = Comm_GetLocalNetId();
        if (result && !Comm_IsNetIdConnected((u16)result)) {
            MysteryGiftComm_Free();
            data->saveCancelled = 1;
            MainMenuUtil_CancelSave();
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            WirelessStatusIcon_Destroy();
            MysteryGiftApp_SetSpriteAnim(data, -1);
            *state = 14;
        } else if (MainMenuUtil_GetSaveState() == 4) {
            LinkSync_StartSync(147);
            *state = 13;
            data->timer = 120;
        } else if (MainMenuUtil_GetSaveState() == 3) {
            MainMenuUtil_CancelSave();
            data->receivedHeader.unk8E_2 = 0;
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00027, 56);
        }
        break;
    case 13:
        if (LinkSync_IsSynced(147) == TRUE) {
            MainMenuUtil_ResumeSave();
            data->timer = 10;
            *state = 15;
        } else if (--data->timer == 0) {
            MainMenuUtil_CancelSave();
            data->receivedHeader.unk8E_2 = 0;
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00027, 56);
        }
        break;
    case 14:
        MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
        data->receivedHeader.unk8E_2 = 0;
        *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00029, 56);
        break;
    case 15:
        if (--data->timer == 0) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            PlaySE(SEQ_SE_DP_UG_020);
            MysteryGiftApp_SetSpriteAnim(data, -1);
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00008, 56);
        }
        break;
    case 16:
        PlaySE(SEQ_SE_DP_SELECT);
        MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00025);
        MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
        *state = 18;
        break;
    case 17:
        if (data->resultDelay) {
            data->resultDelay--;
        } else {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            PlaySE(SEQ_SE_DP_SELECT);
            MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00024);
            MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
            *state = 18;
        }
        break;
    case 18:
        if (gSystem.newKeys) {
            PlaySE(SEQ_SE_DP_SELECT);
            MysteryGiftApp_ShowMainMenu(manager, 196, msg_0247_00000);
            *state = 3;
        }
        break;
    case 19:
        if (data->resultDelay) {
            data->resultDelay--;
        } else {
            if (data->giftCheckSource != 27) {
                MainMenuUtil_ReadCartridgeGift(&data->receivedHeader, sizeof(WonderCardInfo) + sizeof(WonderCard));
            }
            data->giftCheckResult = MysteryGift_CheckCanReceive(data->saveData, &data->receivedHeader);
            if (data->giftCheckResult == 1) {
                MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
                *state = 49;
            } else {
                if (!WindowIsInUse(&data->titleWindow)) {
                    AddWindowParameterized(data->bgConfig, &data->titleWindow, GF_BG_LYR_MAIN_0, 3, 2, 26, 4, 0, 452);
                }
                MysteryGiftApp_PrintGiftTitle(manager, &data->titleWindow, data->receivedHeader.name);
                MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00004);
                MysteryGiftApp_ShowReceivePrompt(manager, 0);
                *state = 20;
            }
        }
        break;
    case 20:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_DeclineAcceptGift);
        break;
    case 21:
        *state = 22;
        break;
    case 22:
        card = MysteryGiftApp_GetReceivedCard(manager);
        MainMenuUtil_ShowCardSubScreen(data->bgConfig, card);
        MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00007);
        MysteryGiftApp_SetSpriteAnim(data, 1);
        MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
        data->timer = 60;
        *state = 23;
        break;
    case 23:
        if (--data->timer == 0) {
            MysteryGiftApp_SaveReceivedGift(manager);
            *state = 24;
        }
        break;
    case 24:
        if (MainMenuUtil_GetSaveState() == 4) {
            MainMenuUtil_ResumeSave();
        }
        if (MainMenuUtil_GetSaveState() == 2) {
            data->timer = 1;
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftApp_SetSpriteAnim(data, -1);
            PlaySE(SEQ_SE_DP_UG_020);
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00008, 25);
        } else if (MainMenuUtil_GetSaveState() == 3) {
            MysteryGiftApp_SetSpriteAnim(data, -1);
            data->receivedHeader.unk8E_2 = 0;
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00027, 56);
        }
        break;
    case 25:
        if (--data->timer == 0) {
            data->timer = 0x10000;
            *state = 56;
        }
        break;
    case 49:
        if (data->giftCheckResult != 5) {
            MysteryGiftApp_StopReceiving(data);
        }
        *state = MysteryGiftApp_ShowCannotReceiveMessage(manager);
        MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
        break;
    case 50:
        *state = MysteryGiftApp_PrintMessageAndWait(manager, NULL, 0, *state);
        break;
    case 51:
        if (gSystem.newKeys) {
            MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
            *state = MysteryGiftApp_CancelReceiveMenu(manager);
        }
        break;
    case 52:
        MysteryGiftApp_ShowReceivePrompt(manager, 1);
        *state = 8;
        data->giftCheckResult = 0;
        break;
    case 53:
        MysteryGiftApp_FreeGraphics(manager);
        data->exitTarget = 0;
        return TRUE;
    case 54:
        MysteryGiftApp_FreeGraphics(manager);
        data->exitTarget = 1;
        return TRUE;
    case 55:
        MysteryGiftApp_FreeGraphics(manager);
        data->exitTarget = 2;
        return TRUE;
    case 56:
        if (gSystem.newKeys) {
            PlaySE(SEQ_SE_DP_SELECT);
            if (data->receivedHeader.unk8E_2 == 1) {
                MainMenuUtil_StartFade(0, 57, data->mainState, 2);
            } else {
                MainMenuUtil_SetFadeWhite(1);
                MainMenuUtil_StartFade(0, 58, data->mainState, 2);
            }
        }
        break;
    case 57:
        MysteryGiftApp_LoadSubScreenBg(data->bgConfig);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        if (data->receivedHeader.unk8E_2 == 1 && data->receivedCard.tag == MG_TAG_ITEM && data->receivedCard.data.item == ITEM_LOCK_CAPSULE) {
            LockCapsuleCard_DrawCardOnBg(data->bgConfig, &data->receivedCard, HEAP_ID_84);
        } else {
            WonderCardAlbum_DrawCardOnBg(data->bgConfig, &data->receivedCard, HEAP_ID_84);
        }
        MainMenuUtil_StartFade(1, 56, data->mainState, 2);
        data->receivedHeader.unk8E_2 = 0;
        break;
    case 58:
        OS_ResetSystem(0);
        break;
    default:
        break;
    }
    if (data->callback != NULL) {
        data->callback(data);
    }
    MainMenuUtil_UpdateGiftSprite(data->animStatus);
    return FALSE;
}

#pragma force_active reset

OverlayManager *MysteryGiftApp_GetOverlayManager(void) {
    return ov74_0223D0A8.manager;
}

void MysteryGiftApp_SetOverlayManager(OverlayManager *manager) {
    ov74_0223D0A8.manager = manager;
}

#pragma force_active on

BOOL MysteryGiftApp_Exit(OverlayManager *manager, int *state) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    int exitTarget = data->exitTarget;

    if (exitTarget == 0) {
        RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    } else if (exitTarget == 1) {
        RegisterMainOverlay(FS_OVERLAY_ID(OVY_74), &_0223B410);
    } else if (exitTarget == 2) {
        RegisterMainOverlay(FS_OVERLAY_ID(OVY_74), &_0223B420);
    }
    Heap_Destroy(HEAP_ID_89);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_84);
    if (data->receiveMode == 2) {
        Sys_SetAgbCartridgeVersion(0);
    }
    MainMenuUtil_EnableCartridgePullOutCheck(FALSE);
    return TRUE;
}

void MysteryGiftApp_UpdateBeaconWatch(MysteryGiftAppData *data) {
    switch (data->beaconWatchState) {
    case 28:
        CommState_StartBeaconWatch(data->saveData);
        data->beaconWatchTimer = 120;
        data->beaconWatchState = 30;
        break;
    case 29:
        break;
    case 30:
        if (CommState_GetSeenGames() & 1) {
            data->giftBeaconSeen = 1;
        }
        data->beaconWatchTimer--;
        if (data->beaconWatchTimer == 0) {
            CommState_StopBeaconWatch();
            data->beaconWatchState = 29;
        }
        break;
    }
}

void MysteryGiftApp_BeaconCallback(int choice) {
    switch (choice) {
    case 0:
        ov74_0223D0A8.state = 40;
        break;
    case 1:
        ov74_0223D0A8.state = 41;
        break;
    case 2:
        ov74_0223D0A8.state = 42;
        break;
    case 3:
        if (ov74_0223D0A8.state == 42) {
            ov74_0223D0A8.state = 44;
        } else {
            ov74_0223D0A8.state = 47;
        }
        break;
    case 4:
        ov74_0223D0A8.state = 48;
        break;
    }
}

#pragma force_active reset

void MysteryGiftApp_StopWirelessLink(void) {
    CommWireless_Terminate();
    Heap_Free(ov74_0223D0A8.linkWork);
    WirelessStatusIcon_Destroy();
    ov74_0223D0A8.linkWork = NULL;
}

#pragma force_active on

void MysteryGiftApp_UpdateWirelessSearch(MysteryGiftAppData *data) {
    u32 state;

    if (ov74_0223D0A8.state == 44) {
        MysteryGiftApp_StopWirelessLink();
        if (MysteryGiftBeacon_DecryptGift()) {
            ov74_0223D0A8.state = 45;
        } else {
            ov74_0223D0A8.state = 46;
        }
    }
    state = ov74_0223D0A8.state;
    if (state == 37 || state == 38 || state == 39 || state == 40 || state == 41 || state == 43) {
        WirelessStatusIcon_SetLevel(3 - WM_GetLinkLevel());
    }
    switch (ov74_0223D0A8.state) {
    case 37:
        CommWireless_StartUp();
        ov74_0223D0A8.state = 38;
        break;
    case 38:
        if (CommWireless_IsStartedUp() == TRUE) {
            void *linkWork;

            ov74_0223D0A8.count = 0;
            linkWork = Heap_Alloc(HEAP_ID_84, MysteryGiftBeacon_GetWorkSize());
            ov74_0223D0A8.linkWork = linkWork;
            MysteryGiftBeacon_Init(data->recvBuffer, MysteryGiftApp_BeaconCallback, linkWork);
            ov74_0223D0A8.state = 39;
            WirelessStatusIcon_Create();
        }
        break;
    case 41:
        ov74_0223D0A8.count++;
        break;
    case 47:
        return;
    case 39:
    case 40:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 48:
        break;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        switch (ov74_0223D0A8.state) {
        case 40:
        case 41:
        case 48:
            if (MysteryGiftBeacon_Stop()) {
                ov74_0223D0A8.state = 43;
            }
            break;
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
            break;
        }
    }
}

#pragma force_active reset
