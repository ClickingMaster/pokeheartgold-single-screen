#include "application/main_menu/mystery_gift_app.h"

#include <nitro/wm/wm_internal.h>

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
#include "comm_wireless.h"
#include "communication_core.h"
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
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "title_screen.h"
#include "wireless_status_icon.h"

// Main menu option "Mystery Gift": receives a gift (from a friend or a distribution over DS wireless, or
// over Nintendo Wi-Fi Connection), stores it in the save, and opens the Wonder Card viewers.

// The menu tables of the module
typedef struct MysteryGiftMenuTables {
    MysteryGiftMenuItem wfcPromptItems[2];      // 0x00
    MysteryGiftMenuItem wirelessPromptItems[2]; // 0x10
    MysteryGiftMenuItem unk_20[2];              // 0x20
    MysteryGiftMenuItem friendPromptItems[2];   // 0x30
    MysteryGiftMenuItem unk_40[2];              // 0x40
    ListMenuTemplate listMenuTemplate;          // 0x50
    MysteryGiftMenuItem mainMenuItems[2];       // 0x70
} MysteryGiftMenuTables;

enum MysteryGiftState {
    MYSTERY_GIFT_STATE_INIT,
    MYSTERY_GIFT_STATE_SETUP_SCREEN,
    MYSTERY_GIFT_STATE_WAIT_FADE,
    MYSTERY_GIFT_STATE_MAIN_MENU,
    MYSTERY_GIFT_STATE_RECEIVE_MENU,
    MYSTERY_GIFT_STATE_CONFIRM_CONNECT,
    MYSTERY_GIFT_STATE_SEARCH_FRIEND = 7,
    MYSTERY_GIFT_STATE_ACCEPT_FRIEND_GIFT,
    MYSTERY_GIFT_STATE_WAIT_FRIEND_LINK,
    MYSTERY_GIFT_STATE_WAIT_FRIEND_GIFT,
    MYSTERY_GIFT_STATE_SAVE_FRIEND_GIFT,
    MYSTERY_GIFT_STATE_WAIT_FRIEND_SAVE,
    MYSTERY_GIFT_STATE_WAIT_SAVE_SYNC,
    MYSTERY_GIFT_STATE_FRIEND_DISCONNECTED,
    MYSTERY_GIFT_STATE_FINISH_FRIEND_GIFT,
    MYSTERY_GIFT_STATE_PRINT_TURNED_DOWN,
    MYSTERY_GIFT_STATE_PRINT_NO_GIFT,
    MYSTERY_GIFT_STATE_WAIT_RETURN_TO_MENU,
    MYSTERY_GIFT_STATE_CHECK_GIFT,
    MYSTERY_GIFT_STATE_ACCEPT_GIFT,
    MYSTERY_GIFT_STATE_START_RECEIVE,
    MYSTERY_GIFT_STATE_SHOW_GIFT,
    MYSTERY_GIFT_STATE_WAIT_BEFORE_SAVE,
    MYSTERY_GIFT_STATE_WAIT_SAVE,
    MYSTERY_GIFT_STATE_WAIT_RECEIVED,
    MYSTERY_GIFT_STATE_WFC_DOWNLOAD = 27,
    MYSTERY_GIFT_STATE_ACCEPT_BEACON_GIFT = 31,
    MYSTERY_GIFT_STATE_BEACON_SEARCH,
    MYSTERY_GIFT_STATE_SAVE_BEACON_GIFT,
    MYSTERY_GIFT_STATE_WAIT_BEACON_SAVE,
    MYSTERY_GIFT_STATE_WAIT_GIFT_ANIM,
    MYSTERY_GIFT_STATE_STOP_BEACON_SEARCH,
    MYSTERY_GIFT_STATE_CANNOT_RECEIVE = 49,
    MYSTERY_GIFT_STATE_PRINT_MESSAGE,
    MYSTERY_GIFT_STATE_WAIT_RETURN_TO_MAIN_MENU,
    MYSTERY_GIFT_STATE_ASK_SHARED_GIFT,
    MYSTERY_GIFT_STATE_EXIT_TO_TITLE,
    MYSTERY_GIFT_STATE_EXIT_TO_ALBUM,
    MYSTERY_GIFT_STATE_EXIT_TO_LOCK_CAPSULE_CARD,
    MYSTERY_GIFT_STATE_WAIT_FINISH,
    MYSTERY_GIFT_STATE_SHOW_WONDER_CARD,
    MYSTERY_GIFT_STATE_RESET,
};

// The beacon watch and the wireless search continue the numbering of the app states.
enum MysteryGiftBeaconWatchState {
    MYSTERY_GIFT_BEACON_WATCH_START = 28,
    MYSTERY_GIFT_BEACON_WATCH_OFF,
    MYSTERY_GIFT_BEACON_WATCH_ACTIVE,
};

enum MysteryGiftLinkStatus {
    MYSTERY_GIFT_LINK_START_UP = 37,
    MYSTERY_GIFT_LINK_WAIT_START_UP,
    MYSTERY_GIFT_LINK_STARTED,
    MYSTERY_GIFT_LINK_SEARCHING,
    MYSTERY_GIFT_LINK_FOUND,
    MYSTERY_GIFT_LINK_COMPLETE,
    MYSTERY_GIFT_LINK_STOPPING,
    MYSTERY_GIFT_LINK_DECRYPT,
    MYSTERY_GIFT_LINK_RECEIVED,
    MYSTERY_GIFT_LINK_BAD_GIFT,
    MYSTERY_GIFT_LINK_STOPPED,
    MYSTERY_GIFT_LINK_ERROR,
};

enum MysteryGiftReceiveMode {
    MYSTERY_GIFT_RECEIVE_NONE,
    MYSTERY_GIFT_RECEIVE_FRIEND,
    MYSTERY_GIFT_RECEIVE_CARTRIDGE,
    MYSTERY_GIFT_RECEIVE_WIRELESS,
    MYSTERY_GIFT_RECEIVE_WFC,
};

// Link state shared with the overlay's link helpers.
typedef struct MysteryGiftLinkState {
    OverlayManager *manager;          // 0x00, the Mystery Gift app (MysteryGiftApp_GetOverlayManager)
    void *linkWork;                   // 0x04
    MysteryGiftMenuFunc selectedFunc; // 0x08, last list menu entry chosen
    u32 count;                        // 0x0C
    u32 state;                        // 0x10
} MysteryGiftLinkState;

extern BOOL CTRDG_IsExisting();
extern fx32 FX_Sqrt(fx32 value);
extern void CommWireless_Terminate(void);
extern void WirelessStatusIcon_SetLevel(int level);
extern void CommState_StartBeaconWatch(SaveData *saveData);
extern void CommState_StopBeaconWatch(void);
extern u32 CommState_GetSeenGames(void);
extern void CommState_SetErrorWatch(s8 watchMembers, s8 reportErrors);

extern MysteryGiftLinkState sMysteryGiftLinkState;

extern const GraphicsModes gMysteryGiftAppGraphicsModes;
extern const BgTemplate sMysteryGiftAppBgTemplate;
extern const MysteryGiftMenuTables sMysteryGiftMenuTables;
extern const MysteryGiftMenuItem sMysteryGiftWirelessMenuItems[];
extern const MysteryGiftMenuItem sMysteryGiftAskReceiveMenuItems[];
extern const MysteryGiftMenuItem sMysteryGiftFromFriendMenuItems[];
extern const MysteryGiftMenuItem sMysteryGiftReceivePromptMenuItems[];
extern const ListMenuTemplate sMysteryGiftListMenuTemplate;
extern const OverlayManagerTemplate sWonderCardAlbumOverlayTemplate;
extern const OverlayManagerTemplate sLockCapsuleCardOverlayTemplate;
extern const MysteryGiftMenuItem sMysteryGiftMainMenuExtraItems[];
extern const MysteryGiftMenuItem sMysteryGiftReceiveMenuItems[];

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

// The next particle of an animation cursor
#define NEXT_PARTICLE(cursor) ((MysteryGiftAnimData *)((u8 *)(cursor) + sizeof(MysteryGiftParticle)))

typedef struct MysteryGiftDelayTable {
    int values[21];
} MysteryGiftDelayTable;

extern const MysteryGiftDelayTable sMysteryGiftSparkleDelays;

typedef struct MysteryGiftVec2 {
    float x;
    float y;
} MysteryGiftVec2;

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

void thunk_MysteryGiftApp_SetWaitingIconVisible(MysteryGiftAppData *data, BOOL visible) {
    MysteryGiftApp_SetWaitingIconVisible(data, visible);
}

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

    if (data->receiveMode != MYSTERY_GIFT_RECEIVE_CARTRIDGE) {
        MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00003);
    }
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    if (data->receiveMode != MYSTERY_GIFT_RECEIVE_CARTRIDGE) {
        MysteryGiftComm_StartChild(data, 15);
        data->timeout = 7200;
        WirelessStatusIcon_Create();
        MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
        return MYSTERY_GIFT_STATE_SEARCH_FRIEND;
    }
    data->giftCheckSource = MYSTERY_GIFT_STATE_CHECK_GIFT;
    return MYSTERY_GIFT_STATE_CHECK_GIFT;
}

int MysteryGiftApp_AcceptGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    int state = data->giftCheckResult;

    if (state == 1) {
        return MYSTERY_GIFT_STATE_CANNOT_RECEIVE;
    }
    if ((u32)(state - 2) <= 2) {
        return MYSTERY_GIFT_STATE_CANNOT_RECEIVE;
    }
    if (state == 5) {
        return MYSTERY_GIFT_STATE_CANNOT_RECEIVE;
    }
    if (data->receiveMode == MYSTERY_GIFT_RECEIVE_FRIEND) {
        MysteryGiftComm_ConnectToChannel(data->channel);
    }
    MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00006);
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    if (data->receiveMode == MYSTERY_GIFT_RECEIVE_FRIEND) {
        MysteryGiftApp_SetSpriteAnim(data, 0);
        MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
    }
    if (data->receiveMode == MYSTERY_GIFT_RECEIVE_FRIEND) {
        LinkSync_StartSync(171);
        return MYSTERY_GIFT_STATE_WAIT_FRIEND_LINK;
    }
    return MYSTERY_GIFT_STATE_START_RECEIVE;
}

WonderCard *MysteryGiftApp_GetReceivedCard(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    WonderCard *card = &data->receivedCard;
    WonderCardInfo *header = &data->receivedHeader;

    switch (data->receiveMode) {
    case MYSTERY_GIFT_RECEIVE_FRIEND:
        memcpy(card, MysteryGiftComm_GetRecvBuffer(0, NULL, 0), sizeof(WonderCard));
        MainMenuUtil_DecryptWonderCard(&data->receivedHeader, &data->receivedCard, HEAP_ID_84);
        break;
    case MYSTERY_GIFT_RECEIVE_WIRELESS: {
        u8 *src = data->recvBuffer;
        memcpy(card, src + sizeof(WonderCardInfo), sizeof(WonderCard));
        memcpy(header, src, sizeof(WonderCardInfo));
        break;
    }
    }
    return card;
}

BOOL MysteryGiftApp_IsSpecialCard(WonderCard *card) {
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
        if (MysteryGiftApp_IsSpecialCard(&data->receivedCard) != TRUE) {
            PlaySE(SEQ_SE_DP_SAVE);
        }
        data->callback = NULL;
    }
}

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

void MysteryGiftApp_ShowReceivePrompt(OverlayManager *manager, int mode) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    int msgId;
    Window *window;

    if (mode == 0) {
        msgId = msg_0247_00004;
    } else {
        msgId = -1;
    }
    window = &data->windows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, sMysteryGiftReceivePromptMenuItems, 2, window, msgId);
}

int MysteryGiftApp_DeclineSearch(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
    if (data->receiveMode == MYSTERY_GIFT_RECEIVE_FRIEND) {
        MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
    }
    WirelessStatusIcon_Destroy();
    return MysteryGiftApp_SelectReceiveGift(manager);
}

#pragma force_active on

int MysteryGiftApp_SelectGetFromFriend(OverlayManager *manager) {
    Window *window;
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    data->receiveMode = MainMenuUtil_GetCartridgeGiftSize() ? MYSTERY_GIFT_RECEIVE_CARTRIDGE : MYSTERY_GIFT_RECEIVE_FRIEND;
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[1], FALSE);
    window = &data->windows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, sMysteryGiftFromFriendMenuItems, 2, window, msg_0247_00002);
    return MYSTERY_GIFT_STATE_CONFIRM_CONNECT;
}

int MysteryGiftApp_SelectGetViaWfc(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    data->receiveMode = MYSTERY_GIFT_RECEIVE_WFC;
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[1], FALSE);
    Window *window = &data->windows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, sMysteryGiftMenuTables.wfcPromptItems, 2, window, msg_0247_00080);
    return MYSTERY_GIFT_STATE_CONFIRM_CONNECT;
}

int MysteryGiftApp_SelectGetViaWireless(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    data->receiveMode = MYSTERY_GIFT_RECEIVE_WIRELESS;
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[1], FALSE);
    Window *window = &data->windows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, sMysteryGiftWirelessMenuItems, 2, window, msg_0247_00079);
    return MYSTERY_GIFT_STATE_CONFIRM_CONNECT;
}

void MysteryGiftApp_AskReceiveGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    Window *window = &data->windows[2];

    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 23, 10, 6, 4, 0, 428);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, sMysteryGiftAskReceiveMenuItems, 2, window, msg_0247_00004);
}

#pragma force_active reset

int MysteryGiftApp_DeclineAcceptGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
    WirelessStatusIcon_Destroy();
    if (data->receiveMode == MYSTERY_GIFT_RECEIVE_FRIEND) {
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
    sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_START_UP;
    return MYSTERY_GIFT_STATE_BEACON_SEARCH;
}

#pragma force_active reset

int MysteryGiftApp_ConfirmReceiveGift(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if ((u32)(data->giftCheckResult - 2) <= 2) {
        return MYSTERY_GIFT_STATE_CANNOT_RECEIVE;
    }
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00007);
    return MYSTERY_GIFT_STATE_SAVE_BEACON_GIFT;
}

int MysteryGiftApp_DeclineReceiveGift(OverlayManager *manager) {
    switch (sMysteryGiftLinkState.state) {
    case MYSTERY_GIFT_LINK_SEARCHING:
    case MYSTERY_GIFT_LINK_FOUND:
    case MYSTERY_GIFT_LINK_ERROR:
        if (MysteryGiftBeacon_Stop()) {
            sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_STOPPING;
        }
        break;
    }
    return MYSTERY_GIFT_STATE_STOP_BEACON_SEARCH;
}

int MysteryGiftApp_ConfirmWfcSearch(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00003);
    MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
    MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
    data->wfcState = 0x1000;
    data->wfcCallback = NULL;
    return MYSTERY_GIFT_STATE_WFC_DOWNLOAD;
}

int MysteryGiftApp_CancelReceiveMenu(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    MysteryGiftApp_EraseWindowIfInUse(&data->windows[1], FALSE);
    MysteryGiftApp_ShowMainMenu(manager, 196, msg_0247_00000);
    return MYSTERY_GIFT_STATE_MAIN_MENU;
}

int MysteryGiftApp_SelectReceiveGift(OverlayManager *manager) {
    Window *window;
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    Save_MysteryGift_Get(data->saveData);
    if (data->beaconWatchState == MYSTERY_GIFT_BEACON_WATCH_ACTIVE) {
        data->beaconWatchTimer = 1;
        MysteryGiftApp_UpdateBeaconWatch(data);
    }
    MysteryGiftApp_EraseWindowIfInUse(&data->windows[0], FALSE);
    window = &data->windows[1];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_0, 15, 9, 16, 8, 0, 292);
    }
    DrawFrameAndWindow1(window, TRUE, 31, 3);
    MysteryGiftApp_CreateListMenu(manager, sMysteryGiftReceiveMenuItems, 4, window, msg_0247_00001);
    return MYSTERY_GIFT_STATE_RECEIVE_MENU;
}

int MysteryGiftApp_SelectCheckCard(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (data->beaconWatchState == MYSTERY_GIFT_BEACON_WATCH_ACTIVE) {
        data->beaconWatchTimer = 1;
        MysteryGiftApp_UpdateBeaconWatch(data);
    }
    MainMenuUtil_StartFade(0, MYSTERY_GIFT_STATE_EXIT_TO_ALBUM, data->mainState, MYSTERY_GIFT_STATE_WAIT_FADE);
    return -1;
}

int MysteryGiftApp_SelectViewLockCapsuleCard(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (data->beaconWatchState == MYSTERY_GIFT_BEACON_WATCH_ACTIVE) {
        data->beaconWatchTimer = 1;
        MysteryGiftApp_UpdateBeaconWatch(data);
    }
    MainMenuUtil_StartFade(0, MYSTERY_GIFT_STATE_EXIT_TO_LOCK_CAPSULE_CARD, data->mainState, MYSTERY_GIFT_STATE_WAIT_FADE);
    return -1;
}

int MysteryGiftApp_SelectExit(OverlayManager *manager) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);

    if (data->beaconWatchState == MYSTERY_GIFT_BEACON_WATCH_ACTIVE) {
        data->beaconWatchTimer = 1;
    }
    MainMenuUtil_SetFadeWhite(1);
    MainMenuUtil_StartFade(0, MYSTERY_GIFT_STATE_EXIT_TO_TITLE, data->mainState, MYSTERY_GIFT_STATE_WAIT_FADE);
    return -1;
}

void MysteryGiftApp_InitBgLayer(BgConfig *bgConfig, GFBgLayer bgId, u32 screenOffset, u32 charOffset) {
    BgTemplate template = sMysteryGiftAppBgTemplate;

    template.screenBase = screenOffset >> 11;
    template.charBase = charOffset >> 14;
    InitBgFromTemplate(bgConfig, bgId, &template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, bgId);
}

void MysteryGiftApp_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes = gMysteryGiftAppGraphicsModes;

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
    ListMenuTemplate template = sMysteryGiftListMenuTemplate;
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

    items[count++] = sMysteryGiftMenuTables.mainMenuItems[0];
    if (SaveMysteryGift_HasAnyCard(mysteryGift)) {
        items[count++] = sMysteryGiftMenuTables.mainMenuItems[1];
    }
    if (SaveMysteryGift_SpecialCardTagIsValid(mysteryGift)) {
        items[count++] = sMysteryGiftMainMenuExtraItems[0];
    }
    items[count] = sMysteryGiftMainMenuExtraItems[1];
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
            sMysteryGiftLinkState.selectedFunc = (MysteryGiftMenuFunc)input;
            nextState = ((MysteryGiftMenuFunc)input)(manager);
            if (nextState != -1) {
                *state = nextState;
                return;
            }
        }
        break;
    }
}

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
        data->giftCheckSource = MYSTERY_GIFT_STATE_WFC_DOWNLOAD;
        *state = MYSTERY_GIFT_STATE_CHECK_GIFT;
        data->resultDelay = 0;
        break;
    case 2:
    case 3:
        MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
        data->resultDelay = 0;
        *state = MYSTERY_GIFT_STATE_PRINT_NO_GIFT;
        break;
    case 4:
        ClearWindowTilemapAndCopyToVram(&data->wfcErrorWindow);
        RemoveWindow(&data->wfcErrorWindow);
        BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_MAIN_0);
        PlaySE(SEQ_SE_DP_SELECT);
        MysteryGiftApp_ShowMainMenu(manager, 196, msg_0247_00000);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        *state = MYSTERY_GIFT_STATE_MAIN_MENU;
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
        return MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00009, MYSTERY_GIFT_STATE_ASK_SHARED_GIFT);
    }
    return MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msgId, MYSTERY_GIFT_STATE_WAIT_RETURN_TO_MAIN_MENU);
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
    return MYSTERY_GIFT_STATE_PRINT_MESSAGE;
}

void MysteryGiftApp_StopReceiving(MysteryGiftAppData *data) {
    switch (data->receiveMode) {
    case MYSTERY_GIFT_RECEIVE_FRIEND:
        MysteryGiftComm_Free();
        WirelessStatusIcon_Destroy();
        break;
    case MYSTERY_GIFT_RECEIVE_NONE:
    case MYSTERY_GIFT_RECEIVE_CARTRIDGE:
    case MYSTERY_GIFT_RECEIVE_WIRELESS:
    case MYSTERY_GIFT_RECEIVE_WFC:
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
        data->receiveMode = MYSTERY_GIFT_RECEIVE_CARTRIDGE;
        Sys_SetAgbCartridgeVersion(gGameVersion);
        MainMenuUtil_EnableCartridgePullOutCheck(TRUE);
    }
    data->beaconWatchState = MYSTERY_GIFT_BEACON_WATCH_OFF;
    args = OverlayManager_GetArgs(manager);
    data->saveData = args->saveData;
    data->options = Save_PlayerData_GetOptionsAddr(data->saveData);
    data->textSpeed = 255;
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570);
    return TRUE;
}

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

void MysteryGiftAnim_CreateSparkleSprites(MysteryGiftAnimData *anim) {
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

void MysteryGiftAnim_CreateRibbons(MysteryGiftAnimData *anim) {
    int i = 0;
    MysteryGiftAnimData *cursor = anim;

    for (; i < MYSTERY_GIFT_PARTICLE_COUNT; i++, cursor = NEXT_PARTICLE(cursor)) {
        cursor->ribbons[0].sprite = MysteryGiftAnim_CreateSprite(anim, NNS_G2D_VRAM_TYPE_2DSUB);
        Sprite_TryChangeAnimSeq(cursor->ribbons[0].sprite, 5);
        Sprite_SetAnimationFrame(cursor->ribbons[0].sprite, 0);
    }
}

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

#pragma push
#pragma opt_repositioncode off

void MysteryGiftAnim_InitSparkles(MysteryGiftAnimData *anim) {
    MysteryGiftDelayTable delaysCopy;
    u32 delayPtr;
    MysteryGiftDelayTable delays;
    VecFx32 pos;
    int i;
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
    if (distance < speed || (float)*(s16 *)&minDistance > distance || distance == 0.0f) {
        return FALSE;
    }
    result.x = delta.x * speed / distance;
    result.y = delta.y * speed / distance;
    *outX = result.x + origin.x;
    *outY = result.y + origin.y;
    return TRUE;
}

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
    MysteryGiftAnim_InitSparkles(anim);
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
            MysteryGiftAnim_CreateSparkleSprites(anim);
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
    if (data->beaconWatchState != MYSTERY_GIFT_BEACON_WATCH_OFF) {
        MysteryGiftApp_UpdateBeaconWatch(data);
    }
    switch (*state) {
    case MYSTERY_GIFT_STATE_INIT:
        MainMenuUtil_InitWork(HEAP_ID_84);
        data->mainState = state;
        *state = MYSTERY_GIFT_STATE_SETUP_SCREEN;
        break;
    case MYSTERY_GIFT_STATE_SETUP_SCREEN:
        MysteryGiftApp_SetupScreen(manager, data);
        MysteryGiftApp_InitSprites(data);
        MainMenuUtil_StartFade(1, MYSTERY_GIFT_STATE_MAIN_MENU, state, MYSTERY_GIFT_STATE_WAIT_FADE);
        break;
    case MYSTERY_GIFT_STATE_WAIT_FADE:
        MainMenuUtil_WaitFade(state);
        break;
    case MYSTERY_GIFT_STATE_MAIN_MENU:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_SelectExit);
        break;
    case MYSTERY_GIFT_STATE_RECEIVE_MENU:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_CancelReceiveMenu);
        break;
    case MYSTERY_GIFT_STATE_CONFIRM_CONNECT:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_DeclineSearch);
        break;
    case MYSTERY_GIFT_STATE_WFC_DOWNLOAD:
        MysteryGiftApp_RunWfcDownload(manager, state);
        break;
    case MYSTERY_GIFT_STATE_BEACON_SEARCH:
        MysteryGiftApp_UpdateWirelessSearch(data);
        if (sMysteryGiftLinkState.state == MYSTERY_GIFT_LINK_RECEIVED) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftApp_GetReceivedCard(manager);
            data->giftCheckResult = MysteryGift_CheckCanReceive(data->saveData, &data->receivedHeader);
            if (data->giftCheckResult == 1) {
                MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
                *state = MYSTERY_GIFT_STATE_CANNOT_RECEIVE;
            } else {
                if (!WindowIsInUse(&data->titleWindow)) {
                    AddWindowParameterized(data->bgConfig, &data->titleWindow, GF_BG_LYR_MAIN_0, 3, 2, 26, 4, 0, 452);
                }
                MysteryGiftApp_PrintGiftTitle(manager, &data->titleWindow, MysteryGiftBeacon_GetTitle());
                MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00004);
                MysteryGiftApp_AskReceiveGift(manager);
                *state = MYSTERY_GIFT_STATE_ACCEPT_BEACON_GIFT;
            }
        }
        if ((u32)(sMysteryGiftLinkState.state - MYSTERY_GIFT_LINK_BAD_GIFT) <= 1) {
            MysteryGiftApp_StopWirelessLink();
            *state = MYSTERY_GIFT_STATE_PRINT_NO_GIFT;
        }
        break;
    case MYSTERY_GIFT_STATE_ACCEPT_BEACON_GIFT:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_DeclineReceiveGift);
        MysteryGiftApp_UpdateWirelessSearch(data);
        break;
    case MYSTERY_GIFT_STATE_SAVE_BEACON_GIFT:
        card = MysteryGiftApp_GetReceivedCard(manager);
        MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
        MysteryGiftApp_SetSpriteAnim(data, 1);
        if (MysteryGiftApp_IsSpecialCard(card) == TRUE) {
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
        *state = MYSTERY_GIFT_STATE_WAIT_BEACON_SAVE;
        break;
    case MYSTERY_GIFT_STATE_WAIT_GIFT_ANIM:
        if (!data->animStatus) {
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00008, MYSTERY_GIFT_STATE_WAIT_FINISH);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        }
        break;
    case MYSTERY_GIFT_STATE_WAIT_BEACON_SAVE:
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
            if (MysteryGiftApp_IsSpecialCard(card) == TRUE) {
                MainMenuUtil_ShowCardSubScreen(data->bgConfig, card);
                data->animStatus = 2;
            } else {
                PlaySE(SEQ_SE_DP_UG_020);
            }
            *state = MYSTERY_GIFT_STATE_WAIT_GIFT_ANIM;
        } else if (result == 3) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftApp_SetSpriteAnim(data, -1);
            data->receivedHeader.unk8E_2 = 0;
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00027, MYSTERY_GIFT_STATE_WAIT_FINISH);
        }
        break;
    case MYSTERY_GIFT_STATE_STOP_BEACON_SEARCH:
        MysteryGiftApp_UpdateWirelessSearch(data);
        result = sMysteryGiftLinkState.state;
        if ((u32)(result - MYSTERY_GIFT_LINK_RECEIVED) <= 2) {
            if (result == MYSTERY_GIFT_LINK_STOPPED) {
                MysteryGiftApp_StopWirelessLink();
            }
            *state = MysteryGiftApp_DeclineAcceptGift(manager);
        }
        break;
    case MYSTERY_GIFT_STATE_SEARCH_FRIEND:
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
            *state = MYSTERY_GIFT_STATE_ACCEPT_FRIEND_GIFT;
        }
        if ((gSystem.newKeys & PAD_BUTTON_B) || --data->timeout == 0) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            *state = MYSTERY_GIFT_STATE_PRINT_NO_GIFT;
        }
        break;
    case MYSTERY_GIFT_STATE_ACCEPT_FRIEND_GIFT:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_DeclineAcceptGift);
        break;
    case MYSTERY_GIFT_STATE_WAIT_FRIEND_LINK:
        result = Comm_GetLocalNetId();
        if (result && Comm_IsNetIdConnected((u16)result)) {
            if (LinkSync_IsSynced(171) == TRUE) {
                MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
                CommState_SetErrorWatch(1, 1);
                MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00007);
                MysteryGiftApp_SetSpriteAnim(data, 1);
                MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
                data->timeout = 600;
                *state = MYSTERY_GIFT_STATE_WAIT_FRIEND_GIFT;
            }
        } else if (result && !Comm_IsNetIdConnected((u16)result)) {
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            MysteryGiftApp_SetSpriteAnim(data, -1);
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            *state = MYSTERY_GIFT_STATE_PRINT_TURNED_DOWN;
            break;
        }
        if ((gSystem.newKeys & PAD_BUTTON_B) || --data->timeout == 0) {
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            MysteryGiftApp_SetSpriteAnim(data, -1);
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            *state = MYSTERY_GIFT_STATE_PRINT_NO_GIFT;
        }
        break;
    case MYSTERY_GIFT_STATE_WAIT_FRIEND_GIFT:
        if (MysteryGiftComm_IsReceived()) {
            *state = MYSTERY_GIFT_STATE_SAVE_FRIEND_GIFT;
        }
        result = Comm_GetLocalNetId();
        if ((gSystem.newKeys & PAD_BUTTON_B) || --data->timeout == 0 || (result && !Comm_IsNetIdConnected((u16)result))) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            *state = MYSTERY_GIFT_STATE_PRINT_NO_GIFT;
        }
        break;
    case MYSTERY_GIFT_STATE_SAVE_FRIEND_GIFT:
        card = MysteryGiftApp_GetReceivedCard(manager);
        MainMenuUtil_ShowCardSubScreen(data->bgConfig, card);
        CommState_SetErrorWatch(0, 0);
        MysteryGiftApp_SaveReceivedGift(manager);
        *state = MYSTERY_GIFT_STATE_WAIT_FRIEND_SAVE;
        break;
    case MYSTERY_GIFT_STATE_WAIT_FRIEND_SAVE:
        result = Comm_GetLocalNetId();
        if (result && !Comm_IsNetIdConnected((u16)result)) {
            MysteryGiftComm_Free();
            data->saveCancelled = 1;
            MainMenuUtil_CancelSave();
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            WirelessStatusIcon_Destroy();
            MysteryGiftApp_SetSpriteAnim(data, -1);
            *state = MYSTERY_GIFT_STATE_FRIEND_DISCONNECTED;
        } else if (MainMenuUtil_GetSaveState() == 4) {
            LinkSync_StartSync(147);
            *state = MYSTERY_GIFT_STATE_WAIT_SAVE_SYNC;
            data->timer = 120;
        } else if (MainMenuUtil_GetSaveState() == 3) {
            MainMenuUtil_CancelSave();
            data->receivedHeader.unk8E_2 = 0;
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00027, MYSTERY_GIFT_STATE_WAIT_FINISH);
        }
        break;
    case MYSTERY_GIFT_STATE_WAIT_SAVE_SYNC:
        if (LinkSync_IsSynced(147) == TRUE) {
            MainMenuUtil_ResumeSave();
            data->timer = 10;
            *state = MYSTERY_GIFT_STATE_FINISH_FRIEND_GIFT;
        } else if (--data->timer == 0) {
            MainMenuUtil_CancelSave();
            data->receivedHeader.unk8E_2 = 0;
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00027, MYSTERY_GIFT_STATE_WAIT_FINISH);
        }
        break;
    case MYSTERY_GIFT_STATE_FRIEND_DISCONNECTED:
        MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
        data->receivedHeader.unk8E_2 = 0;
        *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00029, MYSTERY_GIFT_STATE_WAIT_FINISH);
        break;
    case MYSTERY_GIFT_STATE_FINISH_FRIEND_GIFT:
        if (--data->timer == 0) {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftComm_Free();
            WirelessStatusIcon_Destroy();
            PlaySE(SEQ_SE_DP_UG_020);
            MysteryGiftApp_SetSpriteAnim(data, -1);
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00008, MYSTERY_GIFT_STATE_WAIT_FINISH);
        }
        break;
    case MYSTERY_GIFT_STATE_PRINT_TURNED_DOWN:
        PlaySE(SEQ_SE_DP_SELECT);
        MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00025);
        MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
        *state = MYSTERY_GIFT_STATE_WAIT_RETURN_TO_MENU;
        break;
    case MYSTERY_GIFT_STATE_PRINT_NO_GIFT:
        if (data->resultDelay) {
            data->resultDelay--;
        } else {
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            PlaySE(SEQ_SE_DP_SELECT);
            MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00024);
            MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
            *state = MYSTERY_GIFT_STATE_WAIT_RETURN_TO_MENU;
        }
        break;
    case MYSTERY_GIFT_STATE_WAIT_RETURN_TO_MENU:
        if (gSystem.newKeys) {
            PlaySE(SEQ_SE_DP_SELECT);
            MysteryGiftApp_ShowMainMenu(manager, 196, msg_0247_00000);
            *state = MYSTERY_GIFT_STATE_MAIN_MENU;
        }
        break;
    case MYSTERY_GIFT_STATE_CHECK_GIFT:
        if (data->resultDelay) {
            data->resultDelay--;
        } else {
            if (data->giftCheckSource != MYSTERY_GIFT_STATE_WFC_DOWNLOAD) {
                MainMenuUtil_ReadCartridgeGift(&data->receivedHeader, sizeof(WonderCardInfo) + sizeof(WonderCard));
            }
            data->giftCheckResult = MysteryGift_CheckCanReceive(data->saveData, &data->receivedHeader);
            if (data->giftCheckResult == 1) {
                MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
                *state = MYSTERY_GIFT_STATE_CANNOT_RECEIVE;
            } else {
                if (!WindowIsInUse(&data->titleWindow)) {
                    AddWindowParameterized(data->bgConfig, &data->titleWindow, GF_BG_LYR_MAIN_0, 3, 2, 26, 4, 0, 452);
                }
                MysteryGiftApp_PrintGiftTitle(manager, &data->titleWindow, data->receivedHeader.name);
                MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00004);
                MysteryGiftApp_ShowReceivePrompt(manager, 0);
                *state = MYSTERY_GIFT_STATE_ACCEPT_GIFT;
            }
        }
        break;
    case MYSTERY_GIFT_STATE_ACCEPT_GIFT:
        MysteryGiftApp_HandleListMenuInput(manager, state, MysteryGiftApp_DeclineAcceptGift);
        break;
    case MYSTERY_GIFT_STATE_START_RECEIVE:
        *state = MYSTERY_GIFT_STATE_SHOW_GIFT;
        break;
    case MYSTERY_GIFT_STATE_SHOW_GIFT:
        card = MysteryGiftApp_GetReceivedCard(manager);
        MainMenuUtil_ShowCardSubScreen(data->bgConfig, card);
        MysteryGiftApp_PrintMessage(manager, &data->messageWindow, msg_0247_00007);
        MysteryGiftApp_SetSpriteAnim(data, 1);
        MysteryGiftApp_SetWaitingIconVisible(data, TRUE);
        data->timer = 60;
        *state = MYSTERY_GIFT_STATE_WAIT_BEFORE_SAVE;
        break;
    case MYSTERY_GIFT_STATE_WAIT_BEFORE_SAVE:
        if (--data->timer == 0) {
            MysteryGiftApp_SaveReceivedGift(manager);
            *state = MYSTERY_GIFT_STATE_WAIT_SAVE;
        }
        break;
    case MYSTERY_GIFT_STATE_WAIT_SAVE:
        if (MainMenuUtil_GetSaveState() == 4) {
            MainMenuUtil_ResumeSave();
        }
        if (MainMenuUtil_GetSaveState() == 2) {
            data->timer = 1;
            MysteryGiftApp_SetWaitingIconVisible(data, FALSE);
            MysteryGiftApp_SetSpriteAnim(data, -1);
            PlaySE(SEQ_SE_DP_UG_020);
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00008, MYSTERY_GIFT_STATE_WAIT_RECEIVED);
        } else if (MainMenuUtil_GetSaveState() == 3) {
            MysteryGiftApp_SetSpriteAnim(data, -1);
            data->receivedHeader.unk8E_2 = 0;
            *state = MysteryGiftApp_PrintMessageAndWait(manager, &data->messageWindow, msg_0247_00027, MYSTERY_GIFT_STATE_WAIT_FINISH);
        }
        break;
    case MYSTERY_GIFT_STATE_WAIT_RECEIVED:
        if (--data->timer == 0) {
            data->timer = 0x10000;
            *state = MYSTERY_GIFT_STATE_WAIT_FINISH;
        }
        break;
    case MYSTERY_GIFT_STATE_CANNOT_RECEIVE:
        if (data->giftCheckResult != 5) {
            MysteryGiftApp_StopReceiving(data);
        }
        *state = MysteryGiftApp_ShowCannotReceiveMessage(manager);
        MysteryGiftApp_EraseWindowIfInUse(&data->windows[2], FALSE);
        break;
    case MYSTERY_GIFT_STATE_PRINT_MESSAGE:
        *state = MysteryGiftApp_PrintMessageAndWait(manager, NULL, msg_0247_00000, *state);
        break;
    case MYSTERY_GIFT_STATE_WAIT_RETURN_TO_MAIN_MENU:
        if (gSystem.newKeys) {
            MysteryGiftApp_EraseWindowIfInUse(&data->titleWindow, FALSE);
            *state = MysteryGiftApp_CancelReceiveMenu(manager);
        }
        break;
    case MYSTERY_GIFT_STATE_ASK_SHARED_GIFT:
        MysteryGiftApp_ShowReceivePrompt(manager, 1);
        *state = MYSTERY_GIFT_STATE_ACCEPT_FRIEND_GIFT;
        data->giftCheckResult = 0;
        break;
    case MYSTERY_GIFT_STATE_EXIT_TO_TITLE:
        MysteryGiftApp_FreeGraphics(manager);
        data->exitTarget = 0;
        return TRUE;
    case MYSTERY_GIFT_STATE_EXIT_TO_ALBUM:
        MysteryGiftApp_FreeGraphics(manager);
        data->exitTarget = 1;
        return TRUE;
    case MYSTERY_GIFT_STATE_EXIT_TO_LOCK_CAPSULE_CARD:
        MysteryGiftApp_FreeGraphics(manager);
        data->exitTarget = 2;
        return TRUE;
    case MYSTERY_GIFT_STATE_WAIT_FINISH:
        if (gSystem.newKeys) {
            PlaySE(SEQ_SE_DP_SELECT);
            if (data->receivedHeader.unk8E_2 == 1) {
                MainMenuUtil_StartFade(0, MYSTERY_GIFT_STATE_SHOW_WONDER_CARD, data->mainState, MYSTERY_GIFT_STATE_WAIT_FADE);
            } else {
                MainMenuUtil_SetFadeWhite(1);
                MainMenuUtil_StartFade(0, MYSTERY_GIFT_STATE_RESET, data->mainState, MYSTERY_GIFT_STATE_WAIT_FADE);
            }
        }
        break;
    case MYSTERY_GIFT_STATE_SHOW_WONDER_CARD:
        MysteryGiftApp_LoadSubScreenBg(data->bgConfig);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        if (data->receivedHeader.unk8E_2 == 1 && data->receivedCard.tag == MG_TAG_ITEM && data->receivedCard.data.item == ITEM_LOCK_CAPSULE) {
            LockCapsuleCard_DrawCardOnBg(data->bgConfig, &data->receivedCard, HEAP_ID_84);
        } else {
            WonderCardAlbum_DrawCardOnBg(data->bgConfig, &data->receivedCard, HEAP_ID_84);
        }
        MainMenuUtil_StartFade(1, MYSTERY_GIFT_STATE_WAIT_FINISH, data->mainState, MYSTERY_GIFT_STATE_WAIT_FADE);
        data->receivedHeader.unk8E_2 = 0;
        break;
    case MYSTERY_GIFT_STATE_RESET:
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
    return sMysteryGiftLinkState.manager;
}

void MysteryGiftApp_SetOverlayManager(OverlayManager *manager) {
    sMysteryGiftLinkState.manager = manager;
}

#pragma force_active on

BOOL MysteryGiftApp_Exit(OverlayManager *manager, int *state) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    int exitTarget = data->exitTarget;

    if (exitTarget == 0) {
        RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    } else if (exitTarget == 1) {
        RegisterMainOverlay(FS_OVERLAY_ID(OVY_74), &sWonderCardAlbumOverlayTemplate);
    } else if (exitTarget == 2) {
        RegisterMainOverlay(FS_OVERLAY_ID(OVY_74), &sLockCapsuleCardOverlayTemplate);
    }
    Heap_Destroy(HEAP_ID_89);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_84);
    if (data->receiveMode == MYSTERY_GIFT_RECEIVE_CARTRIDGE) {
        Sys_SetAgbCartridgeVersion(0);
    }
    MainMenuUtil_EnableCartridgePullOutCheck(FALSE);
    return TRUE;
}

void MysteryGiftApp_UpdateBeaconWatch(MysteryGiftAppData *data) {
    switch (data->beaconWatchState) {
    case MYSTERY_GIFT_BEACON_WATCH_START:
        CommState_StartBeaconWatch(data->saveData);
        data->beaconWatchTimer = 120;
        data->beaconWatchState = MYSTERY_GIFT_BEACON_WATCH_ACTIVE;
        break;
    case MYSTERY_GIFT_BEACON_WATCH_OFF:
        break;
    case MYSTERY_GIFT_BEACON_WATCH_ACTIVE:
        if (CommState_GetSeenGames() & 1) {
            data->giftBeaconSeen = 1;
        }
        data->beaconWatchTimer--;
        if (data->beaconWatchTimer == 0) {
            CommState_StopBeaconWatch();
            data->beaconWatchState = MYSTERY_GIFT_BEACON_WATCH_OFF;
        }
        break;
    }
}

void MysteryGiftApp_BeaconCallback(int choice) {
    switch (choice) {
    case MYSTERY_GIFT_BEACON_EVENT_SEARCHING:
        sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_SEARCHING;
        break;
    case MYSTERY_GIFT_BEACON_EVENT_FOUND:
        sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_FOUND;
        break;
    case MYSTERY_GIFT_BEACON_EVENT_COMPLETE:
        sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_COMPLETE;
        break;
    case MYSTERY_GIFT_BEACON_EVENT_STOPPED:
        if (sMysteryGiftLinkState.state == MYSTERY_GIFT_LINK_COMPLETE) {
            sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_DECRYPT;
        } else {
            sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_STOPPED;
        }
        break;
    case MYSTERY_GIFT_BEACON_EVENT_ERROR:
        sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_ERROR;
        break;
    }
}

#pragma force_active reset

void MysteryGiftApp_StopWirelessLink(void) {
    CommWireless_Terminate();
    Heap_Free(sMysteryGiftLinkState.linkWork);
    WirelessStatusIcon_Destroy();
    sMysteryGiftLinkState.linkWork = NULL;
}

void MysteryGiftApp_UpdateWirelessSearch(MysteryGiftAppData *data) {
    u32 state;

    if (sMysteryGiftLinkState.state == MYSTERY_GIFT_LINK_DECRYPT) {
        MysteryGiftApp_StopWirelessLink();
        if (MysteryGiftBeacon_DecryptGift()) {
            sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_RECEIVED;
        } else {
            sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_BAD_GIFT;
        }
    }
    state = sMysteryGiftLinkState.state;
    if (state == MYSTERY_GIFT_LINK_START_UP || state == MYSTERY_GIFT_LINK_WAIT_START_UP || state == MYSTERY_GIFT_LINK_STARTED || state == MYSTERY_GIFT_LINK_SEARCHING || state == MYSTERY_GIFT_LINK_FOUND || state == MYSTERY_GIFT_LINK_STOPPING) {
        WirelessStatusIcon_SetLevel(3 - WM_GetLinkLevel());
    }
    switch (sMysteryGiftLinkState.state) {
    case MYSTERY_GIFT_LINK_START_UP:
        CommWireless_StartUp();
        sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_WAIT_START_UP;
        break;
    case MYSTERY_GIFT_LINK_WAIT_START_UP:
        if (CommWireless_IsStartedUp() == TRUE) {
            void *linkWork;

            sMysteryGiftLinkState.count = 0;
            linkWork = Heap_Alloc(HEAP_ID_84, MysteryGiftBeacon_GetWorkSize());
            sMysteryGiftLinkState.linkWork = linkWork;
            MysteryGiftBeacon_Init(data->recvBuffer, MysteryGiftApp_BeaconCallback, linkWork);
            sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_STARTED;
            WirelessStatusIcon_Create();
        }
        break;
    case MYSTERY_GIFT_LINK_FOUND:
        sMysteryGiftLinkState.count++;
        break;
    case MYSTERY_GIFT_LINK_STOPPED:
        return;
    case MYSTERY_GIFT_LINK_STARTED:
    case MYSTERY_GIFT_LINK_SEARCHING:
    case MYSTERY_GIFT_LINK_COMPLETE:
    case MYSTERY_GIFT_LINK_STOPPING:
    case MYSTERY_GIFT_LINK_DECRYPT:
    case MYSTERY_GIFT_LINK_RECEIVED:
    case MYSTERY_GIFT_LINK_BAD_GIFT:
    case MYSTERY_GIFT_LINK_ERROR:
        break;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        switch (sMysteryGiftLinkState.state) {
        case MYSTERY_GIFT_LINK_SEARCHING:
        case MYSTERY_GIFT_LINK_FOUND:
        case MYSTERY_GIFT_LINK_ERROR:
            if (MysteryGiftBeacon_Stop()) {
                sMysteryGiftLinkState.state = MYSTERY_GIFT_LINK_STOPPING;
            }
            break;
        case MYSTERY_GIFT_LINK_COMPLETE:
        case MYSTERY_GIFT_LINK_STOPPING:
        case MYSTERY_GIFT_LINK_DECRYPT:
        case MYSTERY_GIFT_LINK_RECEIVED:
        case MYSTERY_GIFT_LINK_BAD_GIFT:
        case MYSTERY_GIFT_LINK_STOPPED:
            break;
        }
    }
}
