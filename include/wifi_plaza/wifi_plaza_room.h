#ifndef POKEHEARTGOLD_WIFI_PLAZA_ROOM_H
#define POKEHEARTGOLD_WIFI_PLAZA_ROOM_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "touch_hitbox_controller.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"
#include "vram_transfer_manager.h"
#include "wifi_plaza/wifi_plaza_camera.h"
#include "wifi_plaza/wifi_plaza_instrument.h"
#include "wifi_plaza/wifi_plaza_map.h"
#include "wifi_plaza/wifi_plaza_models.h"
#include "wifi_plaza/wifi_plaza_obj.h"

// The walkable 3D room of the Wi-Fi Plaza: the top screen shows the room, the touch screen an info panel (own
// trainer card, the card of a player the own avatar talks to, or the Tap Toy of the own player).

typedef struct WiFiPlazaWork WiFiPlazaWork;
typedef struct WiFiPlazaPlayer WiFiPlazaPlayer;
typedef struct WiFiPlazaRoomWork WiFiPlazaRoomWork;

enum WiFiPlazaYesNoResult {
    WIFI_PLAZA_YES_NO_YES,
    WIFI_PLAZA_YES_NO_NO,
    WIFI_PLAZA_YES_NO_OPEN,
};

// What the plaza does after the room closes
enum WiFiPlazaRoomResult {
    WIFI_PLAZA_ROOM_RESULT_DISCONNECT,
    WIFI_PLAZA_ROOM_RESULT_VISITOR_PROFILES,
    WIFI_PLAZA_ROOM_RESULT_PLAZA_NEWS,
    WIFI_PLAZA_ROOM_RESULT_SWALOT_PLOP,
    WIFI_PLAZA_ROOM_RESULT_MIME_JR_TOP,
    WIFI_PLAZA_ROOM_RESULT_WOBBUFFET_POP,
    WIFI_PLAZA_ROOM_RESULT_FOOTPRINT_STAMP_BOARD_0,
    WIFI_PLAZA_ROOM_RESULT_FOOTPRINT_STAMP_BOARD_1,
    WIFI_PLAZA_ROOM_RESULT_PLAZA_CLOSED,
    WIFI_PLAZA_ROOM_RESULT_SURVEY_ANSWER,
    WIFI_PLAZA_ROOM_RESULT_SURVEY_RESULTS,
};

// What a player is doing in the plaza
enum WiFiPlazaActivity {
    WIFI_PLAZA_ACTIVITY_NONE,
    WIFI_PLAZA_ACTIVITY_FREE,
    WIFI_PLAZA_ACTIVITY_SWALOT_PLOP,
    WIFI_PLAZA_ACTIVITY_MIME_JR_TOP,
    WIFI_PLAZA_ACTIVITY_WOBBUFFET_POP,
    WIFI_PLAZA_ACTIVITY_FOOTPRINT_STAMP_BOARD_0,
    WIFI_PLAZA_ACTIVITY_FOOTPRINT_STAMP_BOARD_1,
    WIFI_PLAZA_ACTIVITY_VISITOR_PROFILES,
    WIFI_PLAZA_ACTIVITY_PLAZA_NEWS,
    WIFI_PLAZA_ACTIVITY_TALK,
    WIFI_PLAZA_ACTIVITY_PARADE_FLOAT,
    WIFI_PLAZA_ACTIVITY_EXIT,
    WIFI_PLAZA_ACTIVITY_GAME_ENTRY,
    WIFI_PLAZA_ACTIVITY_SURVEY,
};

typedef struct WiFiPlazaRoomArgs {
    SaveData *save;
    int group; // room layout group
    int variant;
    u16 startParams[6]; // startParams[3] == 0: the info panel starts hidden
    u32 result;         // copied from WiFiPlazaRoomWork.result on exit
    WiFiPlazaWork *lobby;
} WiFiPlazaRoomArgs;

// Shared 2D graphics of both screens.
typedef struct WiFiPlazaRoomGraphics {
    BgConfig *bgConfig; // main bg 1, sub bgs 4-6
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resManagers[4]; // char, pltt, cell, anim
    NNSG2dCellTransferState *cellTransfer;
    GF3DVramMan *vramManager;
} WiFiPlazaRoomGraphics;

// Message window on the top screen (the sign window uses the same type).
typedef struct WiFiPlazaMsgWindow {
    Window window;
    u32 printerId;
    u32 textSpeed;
    String *string;
    WaitingIcon *waitingIcon;
} WiFiPlazaMsgWindow;

// Scrolling list menu with up / down arrow sprites.
typedef struct WiFiPlazaListMenu {
    ListMenuTemplate template;
    Window window;
    ListMenu *listMenu;
    ListMenuItem *items;
    u16 itemCount;
    u16 totalItems;
    BOOL showArrows;
    SpriteResource *arrowResources[4];
    Sprite *arrows[2]; // up, down
} WiFiPlazaListMenu;

typedef struct WiFiPlazaYesNoMenu {
    ListMenuItem *items;
    ListMenuTemplate template;
} WiFiPlazaYesNoMenu;

// Message banks (NARC msgdata, 4 files) and the expansion buffers.
typedef struct WiFiPlazaRoomMsg {
    MessageFormat *format;
    MsgData *msgData[4];
    String *formatted;
    String *raw;
} WiFiPlazaRoomMsg;

// Card shown on the touch screen: name, ID, country, greeting or Pokemon, Tap Toy icon, avatar.
typedef struct WiFiPlazaPlayerCard {
    BOOL showsMessage; // the easy chat message instead of the Pokemon
    Window windows[5];
    Sprite *iconSprite;
    SpriteResource *iconResources[4];
    Sprite *avatarSprite;
    SpriteResource *avatarResources[4];
    Sprite *monSprites[12];
    SpriteResource *monPltts[4];
    SpriteResource *monChars[12];
    SpriteResource *monCells[4];
    SpriteResource *monAnims[4];
} WiFiPlazaPlayerCard;

// Touch button of the info panel.
typedef struct WiFiPlazaPanelButton {
    s16 timer;
    u8 active;
    u8 selected;
    u16 previous;
    u16 disabled;
    u16 instrumentMode; // 0: own card, 1: Tap Toy
    u16 graphic;
    Window window;
    TouchHitboxController *touch;
    void *screenAllocs[3];
    NNSG2dScreenData *screens[3];
} WiFiPlazaPanelButton;

typedef struct WiFiPlazaInfoPanel {
    u8 state;   // page fade state machine
    u8 page;    // 0 none, 1 own card, 2 player card, 3 Tap Toy, 4 own card (first)
    u8 request; // page to switch to
    u8 requestPlayer;
    u16 requestArg;
    u16 started;
    WiFiPlazaPlayerCard card;
    WiFiPlazaPanelButton button;
    NARC *narc; // NARC 0xD1
    u16 instrumentIndex;
    u8 instrumentType;
    u8 instrumentPlaying;
    u16 paletteMember;
    u16 paletteHeapID;
} WiFiPlazaInfoPanel;

struct WiFiPlazaRoomWork {
    u8 finished;
    u8 result; // WiFiPlazaRoomArgs.result (8: the plaza closes)
    u8 ownPlayerIndex;
    u8 fading;
    u8 mode : 4;
    u8 error : 4;
    u8 actionTile;
    u8 plazaClosing;
    u8 ignorePlazaClosing;
    s32 closingTimer;
    u8 playerBusy[20];
    u8 cardBlocked[20];
    WiFiPlazaWork *lobby;
    u16 *startParams; // WiFiPlazaRoomArgs.startParams
    WiFiPlazaRoomGraphics graphics;
    WiFiPlazaInfoPanel infoPanel;
    WiFiPlazaRoomMsg msg;
    WiFiPlazaMsgWindow msgWindow;
    WiFiPlazaMsgWindow signWindow;
    WiFiPlazaListMenu listMenu;
    Window errorWindow;
    WiFiPlazaYesNoMenu yesNoMenu;
    Window textWindow;
    WiFiPlazaInstrumentManager *instruments;
    BOOL panelButtonPressed;
    struct WiFiPlazaEvent *event;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaModels *models;
    WiFiPlazaMap *map;
    WiFiPlazaCamera *camera;
    struct WiFiPlazaPlayerStateManager *playerStates;
    PlayerProfile *profile;
};

BOOL WiFiPlazaRoom_Init(OverlayManager *manager, int *state);
BOOL WiFiPlazaRoom_Main(OverlayManager *manager, int *state);
BOOL WiFiPlazaRoom_Exit(OverlayManager *manager, int *state);

WiFiPlazaWork *WiFiPlazaRoom_GetLobby(WiFiPlazaRoomWork *room);
void *WiFiPlazaRoom_GetStartParams(WiFiPlazaRoomWork *room); // WiFiPlazaRoomArgs.startParams
WiFiPlazaObjManager *WiFiPlazaRoom_GetObjManager(WiFiPlazaRoomWork *room);
WiFiPlazaModels *WiFiPlazaRoom_GetModels(WiFiPlazaRoomWork *room);
WiFiPlazaMap *WiFiPlazaRoom_GetMap(WiFiPlazaRoomWork *room);
WiFiPlazaCamera *WiFiPlazaRoom_GetCamera(WiFiPlazaRoomWork *room);
struct WiFiPlazaPlayerStateManager *WiFiPlazaRoom_GetPlayerStates(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_SetMode(WiFiPlazaRoomWork *room, u32 mode);
u8 WiFiPlazaRoom_GetOwnPlayerIndex(WiFiPlazaRoomWork *room);
u8 WiFiPlazaRoom_IsFading(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_SetFinished(WiFiPlazaRoomWork *room, u8 finished);
void WiFiPlazaRoom_SetResult(WiFiPlazaRoomWork *room, u8 result);
void WiFiPlazaRoom_SetActionTile(WiFiPlazaRoomWork *room, u8 value);
u8 WiFiPlazaRoom_GetActionTile(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_IgnorePlazaClosing(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_SetPlayerBusy(WiFiPlazaRoomWork *room, u8 index, u8 value);
u8 WiFiPlazaRoom_IsPlayerBusy(WiFiPlazaRoomWork *room, u32 index);
void WiFiPlazaRoom_SetCardBlocked(WiFiPlazaRoomWork *room, u8 index, u8 value);
u8 WiFiPlazaRoom_IsCardBlocked(WiFiPlazaRoomWork *room, u32 index);

void WiFiPlazaRoom_PrintMessage(WiFiPlazaRoomWork *room, String *message);
void WiFiPlazaRoom_PrintMessageInstant(WiFiPlazaRoomWork *room, String *message);
BOOL WiFiPlazaRoom_IsMessageFinished(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_ShowWaitingIcon(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_HideWaitingIcon(WiFiPlazaRoomWork *room);
BOOL WiFiPlazaRoom_IsWaitingIconShown(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_CloseMessage(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_PrintSign(WiFiPlazaRoomWork *room, String *message);
void WiFiPlazaRoom_CreateListItems(WiFiPlazaRoomWork *room, u32 count);
void WiFiPlazaRoom_CreateListItemsWithValue(WiFiPlazaRoomWork *room, u32 count, int value);
void WiFiPlazaRoom_DeleteListItems(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_AddListItem(WiFiPlazaRoomWork *room, String *text, int value);
ListMenuItem *WiFiPlazaRoom_GetListItems(WiFiPlazaRoomWork *room);
BOOL WiFiPlazaRoom_HasListItemValue(WiFiPlazaRoomWork *room, int value);
void WiFiPlazaRoom_OpenList(WiFiPlazaRoomWork *room, const ListMenuTemplate *template, u16 cursorPos, u16 itemsAbove);
void WiFiPlazaRoom_OpenListAt(WiFiPlazaRoomWork *room, const ListMenuTemplate *template, u16 cursorPos, u16 itemsAbove, u8 x, u8 y, u8 width);
u32 WiFiPlazaRoom_ProcessList(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_CloseList(WiFiPlazaRoomWork *room, u16 *cursorPos, u16 *itemsAbove);
void WiFiPlazaRoom_SetListArrows(WiFiPlazaRoomWork *room, BOOL show);
void WiFiPlazaRoom_OpenTextWindow(WiFiPlazaRoomWork *room, u8 x, u8 y, u8 width, u8 height);
void WiFiPlazaRoom_CloseTextWindow(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_PrintTextWindow(WiFiPlazaRoomWork *room, String *string, u32 x, u32 y);
void WiFiPlazaRoom_FillTextWindowRect(WiFiPlazaRoomWork *room, u16 x, u16 y, u16 width, u16 height);
void WiFiPlazaRoom_OpenYesNoMenu(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_OpenYesNoMenuDefaultNo(WiFiPlazaRoomWork *room);
u32 WiFiPlazaRoom_GetYesNoResult(WiFiPlazaRoomWork *room); // 0 yes, 1 no / cancelled, 2 still open
void WiFiPlazaRoom_CloseYesNoMenu(WiFiPlazaRoomWork *room);
String *WiFiPlazaRoom_GetMsgString(WiFiPlazaRoomWork *room, int bank, int msgId);
void WiFiPlazaRoom_BufferInt(WiFiPlazaRoomWork *room, int value, int digits, int field, int style);
void WiFiPlazaRoom_BufferPlayerName(WiFiPlazaRoomWork *room, u32 playerIndex, u32 field);
void WiFiPlazaRoom_BufferActivityName(WiFiPlazaRoomWork *room, u32 activity, u32 field);
void WiFiPlazaRoom_BufferEventName(WiFiPlazaRoomWork *room, u32 event, u32 field);
void WiFiPlazaRoom_BufferInstrumentName(WiFiPlazaRoomWork *room, u32 instrument, u32 field);
void WiFiPlazaRoom_BufferJPGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay);
void WiFiPlazaRoom_BufferENGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay);
void WiFiPlazaRoom_BufferFRGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay);
void WiFiPlazaRoom_BufferITGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay);
void WiFiPlazaRoom_BufferDEGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay);
void WiFiPlazaRoom_BufferSPGreeting(WiFiPlazaRoomWork *room, u32 field, u32 timeOfDay);
void WiFiPlazaRoom_BufferTypeName(WiFiPlazaRoomWork *room, u32 field, u32 index);
void WiFiPlazaRoom_ShowPlayerCard(WiFiPlazaRoomWork *room, u32 playerIndex, u32 arg);
void WiFiPlazaRoom_SetPlayerCardIcon(WiFiPlazaRoomWork *room, u32 icon);
void WiFiPlazaRoom_RefreshPlayerCardTypes(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_ShowOwnCard(WiFiPlazaRoomWork *room);
u32 WiFiPlazaRoom_GetPlayerCardPlayer(WiFiPlazaRoomWork *room);
BOOL WiFiPlazaRoom_IsPlayerCardShown(WiFiPlazaRoomWork *room);
BOOL WiFiPlazaRoom_IsPlayerCardIdle(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_ShowInstrumentPanel(WiFiPlazaRoomWork *room);
BOOL WiFiPlazaRoom_IsMoverWobbling(WiFiPlazaRoomWork *room, u32 index);
void WiFiPlazaRoom_StartInfoPanel(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_SetPanelButtonDisabled(WiFiPlazaRoomWork *room, u16 disabled);
void WiFiPlazaRoom_ResetPanelButton(WiFiPlazaRoomWork *room);
BOOL WiFiPlazaRoom_TakePanelButtonPress(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_StartInstrument(WiFiPlazaRoomWork *room, u32 playerIndex);
BOOL WiFiPlazaRoom_IsInstrumentFinished(WiFiPlazaRoomWork *room, u32 playerIndex);

// Internal to the room code

// Info panel on the touch screen
typedef struct WiFiPlazaCardAvatar {
    u16 avatar;
    u16 trainerClass;
    u16 x;
    u16 y;
} WiFiPlazaCardAvatar;

typedef struct WiFiPlazaCardMonInfo {
    u8 source; // 1: the panel NARC, else NARC 215
    u8 palette;
    u16 member;
    s16 x;
    s16 y;
} WiFiPlazaCardMonInfo;

typedef struct WiFiPlazaCardMonResources {
    u16 narc;
    u16 member;
    u16 count;
} WiFiPlazaCardMonResources;

u32 WiFiPlazaRoom_GetTextSpeed(WiFiPlazaRoomWork *room);

// Lobby calls of the info panel
u32 WiFiPlazaPlayer_GetGender(struct WiFiPlazaPlayer *player);
u16 WiFiPlazaPlayer_GetAvatar(struct WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetCountry(struct WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetRegion(struct WiFiPlazaPlayer *player);
int WiFiPlazaPlayer_GetFavoriteType(struct WiFiPlazaPlayer *player, int index);
int WiFiPlazaPlayer_GetHistoryId(struct WiFiPlazaPlayer *player, int index);
int WiFiPlazaPlayer_GetHistoryValue(struct WiFiPlazaPlayer *player, int index);
u32 WiFiPlazaPlayer_GetLobbyId(struct WiFiPlazaPlayer *player);
int WiFiPlazaWork_IsSpecialVisitor(WiFiPlazaWork *lobby, int index);
int WiFiPlazaWork_HasWordSet(WiFiPlazaWork *lobby, int index);
u16 *WiFiPlazaWork_GetWordSet(WiFiPlazaWork *lobby, int index);
void WiFiPlazaWork_RequestGroupAction(WiFiPlazaWork *lobby, int index);
void WiFiPlazaWork_SendGroupAction(WiFiPlazaWork *lobby);

BOOL WiFiPlazaWork_IsEventFlagSet(WiFiPlazaWork *lobby, int bit);
u32 WiFiPlazaWork_HasAnsweredSurvey(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_IsLobbyOpen(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_IsClosing(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_GetFireworksState(WiFiPlazaWork *lobby);
BOOL WiFiPlazaWork_IsParadeStarted(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_GetEventScheduleMode5(WiFiPlazaWork *lobby);
BOOL WiFiPlazaWork_IsLightingStarted(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_GetColorEvent(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_GetLight1Event(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_GetThirdEvent(WiFiPlazaWork *lobby);
int WiFiPlazaWork_GetOwnSlot(WiFiPlazaWork *lobby);
int WiFiPlazaWork_GetScheduleValue3(WiFiPlazaWork *lobby);
int WiFiPlazaWork_GetScheduleValue2(WiFiPlazaWork *lobby);
int WiFiPlazaWork_GetGroupState(WiFiPlazaWork *lobby, int index);
int WiFiPlazaWork_GetGroupCounter(WiFiPlazaWork *lobby, int index);
int WiFiPlazaWork_GetGroupDuration(WiFiPlazaWork *lobby);
int WiFiPlazaWork_GetGroupMarkerCount(WiFiPlazaWork *lobby, int place);
void WiFiPlazaWork_SetGroupMarkerPassed(WiFiPlazaWork *lobby, int place, int value);
int WiFiPlazaWork_FindGroupPlace(WiFiPlazaWork *lobby, int index);
int WiFiPlazaWork_GetGroupActionRequest(WiFiPlazaWork *lobby, int index);
void WiFiPlazaWork_ClearGroupActionRequest(WiFiPlazaWork *lobby, int index);
void WiFiPlaza_SplitGroupPlace(u32 value, u32 *quotient, u32 *remainder);
u32 WiFiPlazaWork_GetFireworksPhase(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_GetFireworksCounter(WiFiPlazaWork *lobby);
struct WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(WiFiPlazaWork *work, u32 index);
struct WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(WiFiPlazaWork *work);
void WiFiPlazaPlayer_CopyToProfile(struct WiFiPlazaPlayer *player, PlayerProfile *profile, enum HeapID heapID);
u8 WiFiPlazaPlayer_GetTapToy(struct WiFiPlazaPlayer *player);
BOOL WiFiPlazaPlayer_HasTapToy(WiFiPlazaPlayer *player);
u32 WiFiPlazaWork_HasError(WiFiPlazaWork *work);
BOOL WiFiPlazaWork_IsClosed(WiFiPlazaWork *work);
void WiFiPlazaWork_SendTapToyPlayed(void *unused, u32 value);
u32 WiFiPlaza_GetTypeByChoice(u32 index);
BOOL WiFiPlaza_HasConnectionFailed(void);
u32 WiFiPlazaLobbyClient_GetLastError(void);
int WiFiPlaza_GetErrorMessageClass(u32 errorCode, u32 context);
u32 WiFiPlazaLobbyClient_ToErrorCode(u32 error);
BOOL WiFiPlazaLobbyClient_IsMatchActive(int index);
BOOL WiFiPlazaLobbyClient_IsMatchOpen(int index);
u32 WiFiPlazaLobbyClient_GetMatchPlayerCount(int index);

// Lobby calls of the player state handlers, the Plaza Games registration and the conversations.
BOOL WiFiPlazaWork_TakeExcessFlood(WiFiPlazaWork *lobby);
BOOL WiFiPlazaWork_TakePlayerEntered(WiFiPlazaWork *lobby, int index);
BOOL WiFiPlazaWork_TakePlayerLeft(WiFiPlazaWork *lobby, int index);
BOOL WiFiPlazaWork_TakePlayerUpdated(WiFiPlazaWork *lobby, int index);
u32 WiFiPlazaWork_GetEventScheduleHigh(WiFiPlazaWork *lobby);
void WiFiPlazaWork_SetEventScheduleHigh(WiFiPlazaWork *lobby);
BOOL WiFiPlazaWork_GetGameConnectionLost(WiFiPlazaWork *lobby);
int WiFiPlazaWork_DidAppQuitForClosing(WiFiPlazaWork *lobby);
void WiFiPlazaWork_SetMinigameActiveFlag(WiFiPlazaWork *lobby, u32 value);
void WiFiPlazaWork_PlayBGM(WiFiPlazaWork *lobby);
void WiFiPlazaWork_SetLowVolume(WiFiPlazaWork *lobby, int value);
BOOL WiFiPlazaWork_IsSlotOccupied(WiFiPlazaWork *lobby, int index);
void WiFiPlazaWork_SetActivity(WiFiPlazaWork *lobby, int value);
void WiFiPlazaWork_AddHistory(WiFiPlazaWork *lobby, int index, int value);
void WiFiPlazaWork_SetTapToy(WiFiPlazaWork *lobby, u32 value);
void WiFiPlazaWork_SetFavoriteTypes(WiFiPlazaWork *lobby, int first, int second);
int WiFiPlazaPlayer_GetActivity(WiFiPlazaPlayer *player);
int WiFiPlaza_GetChannelPlayerCount(WiFiPlazaWork *lobby);
int WiFiPlaza_ChannelSlotToSlot(WiFiPlazaWork *lobby, int index);
void WiFiPlaza_SendActivityNews(WiFiPlazaWork *lobby, int mode, int value, int player0, int player1, int player2, int player3, int flag);
int WiFiPlazaWork_HasPlayedTapToy(WiFiPlazaWork *lobby, int index);
int WiFiPlazaWork_JoinGroup(WiFiPlazaWork *lobby, int index, int choice);
void WiFiPlazaWork_LeaveGroup(WiFiPlazaWork *lobby, int index);
void WiFiPlazaWork_SendJoinGroup(WiFiPlazaWork *lobby);
BOOL WiFiPlazaWork_IsTalking(WiFiPlazaWork *lobby);
BOOL WiFiPlazaWork_WasAskedToTalk(WiFiPlazaWork *lobby);
u16 WiFiPlazaWork_GetTalkPartner(WiFiPlazaWork *lobby);
void WiFiPlazaWork_SetTalkPartnerArrived(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_HasTalkPartnerArrived(WiFiPlazaWork *lobby);
void WiFiPlazaWork_MarkActivityDone(WiFiPlazaWork *lobby, int value);
int WiFiPlazaWork_PickTapToy(WiFiPlazaWork *lobby);
BOOL WiFiPlazaLobbyClient_JoinChannel(int kind);
int WiFiPlazaLobbyClient_GetJoinChannelResult(void);
BOOL WiFiPlazaLobbyClient_LeaveChannel(void);
BOOL WiFiPlazaLobbyClient_IsNotInChannel(void);
BOOL WiFiPlazaLobbyClient_CanJoinChannel(int kind);
void IncrementGameStat119(WiFiPlazaWork *lobby);
u32 WiFiPlazaPlayer_GetLanguage(WiFiPlazaPlayer *player);
BOOL WiFiPlazaPlayer_IsLanguageValid(WiFiPlazaPlayer *player);
void IncrementGameStat47(WiFiPlazaWork *lobby);
void *WiFiPlazaWork_GetSaveData(WiFiPlazaWork *lobby);
void WiFiPlazaWork_StartGameEntryCooldown(WiFiPlazaWork *lobby);
int WiFiPlazaWork_IsGameEntryCoolingDown(WiFiPlazaWork *lobby);
void WiFiPlazaWork_ClearGameMembers(WiFiPlazaWork *lobby);
void WiFiPlazaWork_SetGameMember(WiFiPlazaWork *lobby, int id, u32 index);
void WiFiPlazaWork_SetOwnGameMember(WiFiPlazaWork *lobby, u32 index);
void WiFiPlazaWork_GetGameMembers(WiFiPlazaWork *lobby, void *dest);
int WiFiPlazaWork_GetOwnId(WiFiPlazaWork *lobby);
int WiFiPlazaPlayer_GetAvatarIndex(WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetLocalTime(WiFiPlazaPlayer *player); // packed local time, hour in the low byte
void *WiFiPlazaWork_GetOwnPlayerProfile(WiFiPlazaWork *lobby);
void WiFiPlaza_SendChatNews(WiFiPlazaWork *lobby, u32 slot0, u32 slot1);
void WiFiPlaza_SendTapToyTradeNews(WiFiPlazaWork *lobby, u32 slot0, u32 slot1, u8 value);
void WiFiPlazaWork_ResetTalk(WiFiPlazaWork *lobby);
int WiFiPlazaWork_StartTalk(WiFiPlazaWork *lobby, int slot);
void WiFiPlazaWork_AdvanceTalk(WiFiPlazaWork *lobby, int value);
void WiFiPlazaWork_CancelTalkRequest(WiFiPlazaWork *lobby);
void WiFiPlazaWork_EndTalk(WiFiPlazaWork *lobby);
u8 WiFiPlazaWork_IsFirstTalk(WiFiPlazaWork *lobby);
u8 WiFiPlazaWork_GetTalkState(WiFiPlazaWork *lobby);
u16 WiFiPlazaWork_GetTalkPartnerMessage(WiFiPlazaWork *lobby);
BOOL WiFiPlazaWork_IsTalkOver(WiFiPlazaWork *lobby);
u32 WiFiPlazaWork_GetTalkTimeoutCounter(WiFiPlazaWork *lobby);
void WiFiPlazaWork_IncPartnerTalkCount(WiFiPlazaWork *lobby);
void WiFiPlazaWork_IncTalkCount(WiFiPlazaWork *lobby, int slot);
void WiFiPlazaWork_SetTalkedWith(WiFiPlazaWork *lobby, int slot);
int WiFiPlazaWork_GetNextUndoneActivity(WiFiPlazaWork *lobby);
int WiFiPlazaLobbyClient_IdToSlot(int id);
void WiFiPlazaLobbyClient_CloseMatch(void);
BOOL WiFiPlazaLobbyClient_IsCurrentMatchClosed(void);
int WiFiPlazaLobbyClient_GetMatchFreePlaces(int slot);
s16 WiFiPlazaLobbyClient_GetMatchTimer(int slot);
int WiFiPlazaLobbyClient_GetMatchHostId(void);
u8 WiFiPlazaLobbyClient_IsMatchHost(void);

#include "wifi_plaza/wifi_plaza_player_state.h"

#endif // POKEHEARTGOLD_WIFI_PLAZA_ROOM_H
