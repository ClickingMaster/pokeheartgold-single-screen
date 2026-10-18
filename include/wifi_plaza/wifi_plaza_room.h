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

// OVY_49: the walkable 3D room of the Wi-Fi Plaza, launched by the lobby overlay (OVY_45).
// The top screen shows the room, the touch screen an info panel (own trainer card, the card of a
// player the own avatar talks to, or the Tap Toy of the own player).

typedef struct WiFiPlazaWork WiFiPlazaWork;     // OVY_45 lobby state (include/wifi_plaza/wifi_plaza_internal.h)
typedef struct WiFiPlazaPlayer WiFiPlazaPlayer; // OVY_45 player record (include/wifi_plaza/wifi_plaza_internal.h)
typedef struct WiFiPlazaRoomWork WiFiPlazaRoomWork;

// Result of WiFiPlazaRoom_GetYesNoResult
enum WiFiPlazaYesNoResult {
    WIFI_PLAZA_YES_NO_YES,
    WIFI_PLAZA_YES_NO_NO,
    WIFI_PLAZA_YES_NO_OPEN,
};

// What the plaza does after the room closes (WiFiPlazaRoom_SetResult)
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

// What a player is doing in the plaza (WiFiPlazaWork_SetActivity, WiFiPlazaPlayer_GetActivity)
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

// Launch arguments (same layout as the OVY_45 app-0 arguments).
typedef struct WiFiPlazaRoomArgs {
    SaveData *save;
    int group;            // 0x04 room layout group (WiFiPlazaModels_Create)
    int variant;          // 0x08
    u16 startParams[6];   // 0x0C startParams[3] == 0: the info panel starts hidden
    u32 result;           // 0x18 copied from WiFiPlazaRoomWork.result on exit
    WiFiPlazaWork *lobby; // 0x1C
} WiFiPlazaRoomArgs;

// Shared 2D graphics of both screens.
typedef struct WiFiPlazaRoomGraphics {
    BgConfig *bgConfig;                    // 0x000 main bg 1, sub bgs 4-6
    SpriteList *spriteList;                // 0x004
    G2dRenderer renderer;                  // 0x008
    GF_2DGfxResMan *resManagers[4];        // 0x130 char, pltt, cell, anim
    NNSG2dCellTransferState *cellTransfer; // 0x140
    GF3DVramMan *vramManager;              // 0x144
} WiFiPlazaRoomGraphics;

// Message window on the top screen (the sign window uses the same type).
typedef struct WiFiPlazaMsgWindow {
    Window window;            // 0x00
    u32 printerId;            // 0x10
    u32 textSpeed;            // 0x14
    String *string;           // 0x18
    WaitingIcon *waitingIcon; // 0x1C
} WiFiPlazaMsgWindow;

// Scrolling list menu with up / down arrow sprites.
typedef struct WiFiPlazaListMenu {
    ListMenuTemplate template;         // 0x00
    Window window;                     // 0x20
    ListMenu *listMenu;                // 0x30
    ListMenuItem *items;               // 0x34
    u16 itemCount;                     // 0x38
    u16 totalItems;                    // 0x3A
    BOOL showArrows;                   // 0x3C
    SpriteResource *arrowResources[4]; // 0x40
    Sprite *arrows[2];                 // 0x50 up, down
} WiFiPlazaListMenu;

typedef struct WiFiPlazaYesNoMenu {
    ListMenuItem *items;       // 0x00
    ListMenuTemplate template; // 0x04
} WiFiPlazaYesNoMenu;

// Message banks (NARC msgdata, 4 files) and the expansion buffers.
typedef struct WiFiPlazaRoomMsg {
    MessageFormat *format; // 0x00
    MsgData *msgData[4];   // 0x04
    String *formatted;     // 0x14
    String *raw;           // 0x18
} WiFiPlazaRoomMsg;

// Card shown on the touch screen: name, ID, country, greeting or Pokemon, Tap Toy icon, avatar.
typedef struct WiFiPlazaPlayerCard {
    BOOL showsMessage;                  // 0x000 the easy chat message instead of the Pokemon
    Window windows[5];                  // 0x004
    Sprite *iconSprite;                 // 0x054
    SpriteResource *iconResources[4];   // 0x058
    Sprite *avatarSprite;               // 0x068
    SpriteResource *avatarResources[4]; // 0x06C
    Sprite *monSprites[12];             // 0x07C
    SpriteResource *monPltts[4];        // 0x0AC
    SpriteResource *monChars[12];       // 0x0BC
    SpriteResource *monCells[4];        // 0x0EC
    SpriteResource *monAnims[4];        // 0x0FC
} WiFiPlazaPlayerCard;

// Touch button of the info panel.
typedef struct WiFiPlazaPanelButton {
    s16 timer;                    // 0x00
    u8 active;                    // 0x02
    u8 selected;                  // 0x03 touch result
    u16 previous;                 // 0x04
    u16 disabled;                 // 0x06
    u16 instrumentMode;           // 0x08 0: own card, 1: Tap Toy
    u16 graphic;                  // 0x0A
    Window window;                // 0x0C
    TouchHitboxController *touch; // 0x1C
    void *screenAllocs[3];        // 0x20
    NNSG2dScreenData *screens[3]; // 0x2C
} WiFiPlazaPanelButton;

typedef struct WiFiPlazaInfoPanel {
    u8 state;                    // 0x000 page fade state machine
    u8 page;                     // 0x001 0 none, 1 own card, 2 player card, 3 Tap Toy, 4 own card (first)
    u8 request;                  // 0x002 page to switch to
    u8 requestPlayer;            // 0x003
    u16 requestArg;              // 0x004
    u16 started;                 // 0x006
    WiFiPlazaPlayerCard card;    // 0x008
    WiFiPlazaPanelButton button; // 0x114
    NARC *narc;                  // 0x14C NARC 0xD1
    u16 instrumentIndex;         // 0x150
    u8 instrumentType;           // 0x152
    u8 instrumentPlaying;        // 0x153
    u16 paletteMember;           // 0x154
    u16 paletteHeapID;           // 0x156
} WiFiPlazaInfoPanel;

struct WiFiPlazaRoomWork {
    u8 finished;       // 0x000
    u8 result;         // 0x001 WiFiPlazaRoomArgs.result (8: the plaza closes)
    u8 ownPlayerIndex; // 0x002
    u8 fading;         // 0x003
    u8 mode : 4;       // 0x004
    u8 error : 4;
    u8 actionTile;                                    // 0x005
    u8 plazaClosing;                                  // 0x006
    u8 ignorePlazaClosing;                            // 0x007
    s32 closingTimer;                                 // 0x008
    u8 playerBusy[20];                                // 0x00C per player
    u8 cardBlocked[20];                               // 0x020 per player
    WiFiPlazaWork *lobby;                             // 0x034
    u16 *startParams;                                 // 0x038 WiFiPlazaRoomArgs.startParams
    WiFiPlazaRoomGraphics graphics;                   // 0x03C
    WiFiPlazaInfoPanel infoPanel;                     // 0x184
    WiFiPlazaRoomMsg msg;                             // 0x2DC
    WiFiPlazaMsgWindow msgWindow;                     // 0x2F8
    WiFiPlazaMsgWindow signWindow;                    // 0x318
    WiFiPlazaListMenu listMenu;                       // 0x338
    Window errorWindow;                               // 0x390
    WiFiPlazaYesNoMenu yesNoMenu;                     // 0x3A0
    Window textWindow;                                // 0x3C4
    WiFiPlazaInstrumentManager *instruments;          // 0x3D4
    BOOL panelButtonPressed;                          // 0x3D8
    struct WiFiPlazaEvent *event;                     // 0x3DC
    WiFiPlazaObjManager *objManager;                  // 0x3E0
    WiFiPlazaModels *models;                          // 0x3E4
    WiFiPlazaMap *map;                                // 0x3E8
    WiFiPlazaCamera *camera;                          // 0x3EC
    struct WiFiPlazaPlayerStateManager *playerStates; // 0x3F0
    PlayerProfile *profile;                           // 0x3F4
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

// lobby (OVY_45) calls of the info panel, with the types it uses them with
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

// Lobby (OVY_45) calls, with the types this overlay uses them with (several return values that
// wifi_plaza_internal.h declares as void).
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

// Lobby (OVY_45) calls of the player state handlers, the Plaza Games registration and the conversations.
BOOL WiFiPlazaWork_TakeLobbyFlag131(WiFiPlazaWork *lobby);
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

// Typed player state handlers (that header includes this one first)
#include "wifi_plaza/wifi_plaza_player_state.h"

#endif // POKEHEARTGOLD_WIFI_PLAZA_ROOM_H
