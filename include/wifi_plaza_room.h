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
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "vram_transfer_manager.h"
#include "wifi_plaza_camera.h"
#include "wifi_plaza_instrument.h"
#include "wifi_plaza_map.h"
#include "wifi_plaza_models.h"
#include "wifi_plaza_obj.h"

// OVY_49: the walkable 3D room of the Wi-Fi Plaza, launched by the lobby overlay (OVY_45).
// The top screen shows the room, the touch screen an info panel (own trainer card, the card of a
// player the own avatar talks to, or the Tap Toy of the own player).

typedef struct WiFiPlazaWork WiFiPlazaWork;     // OVY_45 lobby state (include/wifi_plaza_internal.h)
typedef struct WiFiPlazaPlayer WiFiPlazaPlayer; // OVY_45 player record (include/wifi_plaza_internal.h)
typedef struct WiFiPlazaRoomWork WiFiPlazaRoomWork;

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
BOOL ov49_0225A520(WiFiPlazaRoomWork *room, u32 index);
void WiFiPlazaRoom_StartInfoPanel(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_SetPanelButtonDisabled(WiFiPlazaRoomWork *room, u16 disabled);
void WiFiPlazaRoom_ResetPanelButton(WiFiPlazaRoomWork *room);
BOOL WiFiPlazaRoom_TakePanelButtonPress(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_StartInstrument(WiFiPlazaRoomWork *room, u32 playerIndex);
BOOL WiFiPlazaRoom_IsInstrumentFinished(WiFiPlazaRoomWork *room, u32 playerIndex);
void WiFiPlazaRoom_SetPanelButtonPressed(WiFiPlazaRoomWork *room);

// Internal to src/wifi_plaza_room.c
void WiFiPlazaRoom_VBlank(void *data);
void WiFiPlazaRoom_InitGraphics(WiFiPlazaRoomGraphics *graphics, SaveData *save, enum HeapID heapID);
void WiFiPlazaRoom_DrawSprites(WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaRoom_FreeGraphics(WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaRoom_GraphicsVBlank(WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaRoom_Setup3D(void);
void WiFiPlazaRoom_Update(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_Draw(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_UpdateMatchSigns(WiFiPlazaRoomWork *room);
void WiFiPlazaMsgWindow_Init(WiFiPlazaMsgWindow *window, WiFiPlazaRoomGraphics *graphics, SaveData *save, enum HeapID heapID);
void WiFiPlazaMsgWindow_Free(WiFiPlazaMsgWindow *window);
void WiFiPlazaMsgWindow_Print(WiFiPlazaMsgWindow *window, String *string);
void WiFiPlazaMsgWindow_PrintInstant(WiFiPlazaMsgWindow *window, String *string);
void WiFiPlazaMsgWindow_ShowWaitingIcon(WiFiPlazaMsgWindow *window);
void WiFiPlazaMsgWindow_HideWaitingIcon(WiFiPlazaMsgWindow *window);
void WiFiPlazaMsgWindow_DeleteWaitingIcon(WiFiPlazaMsgWindow *window);
BOOL WiFiPlazaMsgWindow_IsWaitingIconShown(WiFiPlazaMsgWindow *window);
BOOL WiFiPlazaMsgWindow_IsPrintFinished(WiFiPlazaMsgWindow *window);
void WiFiPlazaMsgWindow_Close(WiFiPlazaMsgWindow *window);
void WiFiPlazaSignWindow_Init(WiFiPlazaMsgWindow *window, WiFiPlazaRoomGraphics *graphics, SaveData *save, enum HeapID heapID);
void WiFiPlazaSignWindow_Free(WiFiPlazaMsgWindow *window);
void WiFiPlazaSignWindow_Print(WiFiPlazaMsgWindow *window, String *string);
void WiFiPlazaListMenu_Init(WiFiPlazaListMenu *menu, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID);
void WiFiPlazaListMenu_Free(WiFiPlazaListMenu *menu, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaListMenu_CreateItems(WiFiPlazaListMenu *menu, u32 count, enum HeapID heapID, int value);
void WiFiPlazaListMenu_DeleteItems(WiFiPlazaListMenu *menu);
void WiFiPlazaListMenu_AddItem(WiFiPlazaListMenu *menu, String *text, int value);
ListMenuItem *WiFiPlazaListMenu_GetItems(WiFiPlazaListMenu *menu);
BOOL WiFiPlazaListMenu_HasItemValue(WiFiPlazaListMenu *menu, int value);
void WiFiPlazaListMenu_Open(WiFiPlazaListMenu *menu, const ListMenuTemplate *template, WiFiPlazaRoomGraphics *graphics, u16 cursorPos, u16 itemsAbove, enum HeapID heapID, u8 x, u8 y, u8 width);
s32 WiFiPlazaListMenu_ProcessInput(WiFiPlazaListMenu *menu);
void WiFiPlazaListMenu_Close(WiFiPlazaListMenu *menu, u16 *cursorPos, u16 *itemsAbove);
void WiFiPlazaListMenu_OnCursorMove(ListMenu *listMenu, s32 index, u8 onInit);
void WiFiPlazaListMenu_SetShowArrows(WiFiPlazaListMenu *menu, BOOL show);
void WiFiPlazaListMenu_UpdateArrows(WiFiPlazaListMenu *menu);
void WiFiPlazaTextWindow_Init(Window *window, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID);
void WiFiPlazaTextWindow_Free(Window *window);
void WiFiPlazaTextWindow_Open(Window *window, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID, u8 x, u8 y, u8 width, u8 height);
void WiFiPlazaTextWindow_Close(Window *window);
void WiFiPlazaTextWindow_Print(Window *window, String *string, u32 x, u32 y);
void WiFiPlazaTextWindow_FillRect(Window *window, u16 x, u16 y, u16 width, u16 height);
void WiFiPlazaYesNoMenu_Init(WiFiPlazaYesNoMenu *menu, WiFiPlazaRoomMsg *msg, enum HeapID heapID);
void WiFiPlazaYesNoMenu_Free(WiFiPlazaYesNoMenu *menu);
void WiFiPlazaErrorWindow_Init(Window *window, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID);
void WiFiPlazaErrorWindow_Free(Window *window);
void WiFiPlazaErrorWindow_Print(Window *window, String *string);
void WiFiPlazaErrorWindow_PrintCommError(Window *window, WiFiPlazaRoomMsg *msg);
void WiFiPlazaErrorWindow_PrintLobbyError(Window *window, WiFiPlazaRoomMsg *msg, u32 error);
void WiFiPlazaErrorWindow_PrintDisconnected(Window *window, WiFiPlazaRoomMsg *msg, u32 unused);
void WiFiPlazaRoomMsg_Init(WiFiPlazaRoomMsg *msg, enum HeapID heapID);
void WiFiPlazaRoomMsg_Free(WiFiPlazaRoomMsg *msg);
String *WiFiPlazaRoomMsg_GetString(WiFiPlazaRoomMsg *msg, int bank, int msgId);
void WiFiPlazaRoomMsg_BufferInt(WiFiPlazaRoomMsg *msg, int value, int digits, int field, int style);
void WiFiPlazaRoomMsg_BufferPlayerName(WiFiPlazaRoomMsg *msg, PlayerProfile *profile, u32 field);
void WiFiPlazaRoomMsg_BufferActivityName(WiFiPlazaRoomMsg *msg, u32 activity, u32 field);
void WiFiPlazaRoomMsg_BufferEventName(WiFiPlazaRoomMsg *msg, u32 event, u32 field);
void WiFiPlazaRoomMsg_BufferInstrumentName(WiFiPlazaRoomMsg *msg, u32 instrument, u32 field);
void WiFiPlazaRoomMsg_BufferCountryName(WiFiPlazaRoomMsg *msg, u32 country, u32 field);
void WiFiPlazaRoomMsg_BufferCityName(WiFiPlazaRoomMsg *msg, u32 country, u32 city, u32 field);
void WiFiPlazaRoomMsg_BufferECWord(WiFiPlazaRoomMsg *msg, u32 field, u16 word);
void WiFiPlazaRoomMsg_ResetBuffers(WiFiPlazaRoomMsg *msg);

// Info panel on the touch screen (src/wifi_plaza_info_panel.c)
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

void WiFiPlazaInfoPanel_Init(WiFiPlazaInfoPanel *panel, u16 *startParams, WiFiPlazaRoomGraphics *graphics, PlayerProfile *profile, enum HeapID heapID);
void WiFiPlazaInfoPanel_Free(WiFiPlazaInfoPanel *panel, WiFiPlazaMsgWindow *signWindow, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaInfoPanel_Update(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomWork *room, BOOL paused, enum HeapID heapID);
u8 WiFiPlazaInfoPanel_GetState(WiFiPlazaInfoPanel *panel);
void WiFiPlazaInfoPanel_RequestPlayerCard(WiFiPlazaInfoPanel *panel, u32 playerIndex, u32 showMessage);
void WiFiPlazaInfoPanel_SetCardIcon(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, u32 icon, enum HeapID heapID);
void WiFiPlazaInfoPanel_RequestOwnCard(WiFiPlazaInfoPanel *panel);
void WiFiPlazaInfoPanel_RequestInstrument(WiFiPlazaInfoPanel *panel);
u8 WiFiPlazaInfoPanel_GetCardPlayer(WiFiPlazaInfoPanel *panel);
BOOL WiFiPlazaInfoPanel_IsPlayerCardShown(WiFiPlazaInfoPanel *panel);
BOOL WiFiPlazaInfoPanel_IsPlayerCardIdle(WiFiPlazaInfoPanel *panel);
BOOL WiFiPlazaInfoPanel_CardShowsMessage(WiFiPlazaInfoPanel *panel);
BOOL WiFiPlazaInfoPanel_HasRequest(WiFiPlazaInfoPanel *panel);
void WiFiPlazaInfoPanel_ShowPlayerCard(WiFiPlazaInfoPanel *panel, WiFiPlazaMsgWindow *signWindow, WiFiPlazaRoomGraphics *graphics, WiFiPlazaRoomMsg *msg, BOOL isOwn, enum HeapID heapID, struct WiFiPlazaPlayer *player, struct WiFiPlazaPlayer *myPlayer, int special, BOOL hasMessage, u16 *message, BOOL showIcon);
void WiFiPlazaInfoPanel_HidePlayerCard(WiFiPlazaInfoPanel *panel, WiFiPlazaMsgWindow *signWindow, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaInfoPanel_ShowOwnCard(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, WiFiPlazaRoomMsg *msg, enum HeapID heapID, struct WiFiPlazaPlayer *player);
void WiFiPlazaInfoPanel_ShowInstrument(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID, u32 type);
void WiFiPlazaInfoPanel_HideButtonPage(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaInfoPanel_SetButtonDisabled(WiFiPlazaInfoPanel *panel, u16 disabled);
SysTask *WiFiPlazaInfoPanel_StartPaletteLoad(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, u32 member, enum HeapID heapID);
void WiFiPlazaInfoPanel_LoadPaletteTask(SysTask *task, void *data);
void WiFiPlazaInfoPanel_LoadGraphics(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics, NARC *narc, u32 gender, enum HeapID heapID);
void WiFiPlazaInfoPanel_FreeGraphics(WiFiPlazaInfoPanel *panel, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaInfoPanel_LoadScreen(WiFiPlazaRoomGraphics *graphics, NARC *narc, int member, int bgId, int tileOffset, enum HeapID heapID);
void WiFiPlazaPlayerCard_Init(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID);
void WiFiPlazaPlayerCard_Free(WiFiPlazaPlayerCard *card, WiFiPlazaMsgWindow *signWindow, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaPlayerCard_Update(WiFiPlazaPlayerCard *card, WiFiPlazaMsgWindow *signWindow);
void WiFiPlazaPlayerCard_Show(WiFiPlazaInfoPanel *panel, WiFiPlazaMsgWindow *signWindow, WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, WiFiPlazaRoomMsg *msg, BOOL isOwn, NARC *narc, enum HeapID heapID, struct WiFiPlazaPlayer *player, struct WiFiPlazaPlayer *myPlayer, int special, BOOL hasMessage, u16 *message, BOOL showIcon);
void WiFiPlazaPlayerCard_LoadTypeIcons(WiFiPlazaInfoPanel *unused0, WiFiPlazaPlayerCard *unused1, WiFiPlazaRoomGraphics *graphics, enum HeapID heapID, struct WiFiPlazaPlayer *player);
void WiFiPlazaPlayerCard_Clear(WiFiPlazaPlayerCard *card, WiFiPlazaMsgWindow *unused, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaPlayerCard_ChangeIcon(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID, u32 icon);
BOOL WiFiPlazaPlayerCard_ShowsMessage(WiFiPlazaPlayerCard *card);
void WiFiPlazaPlayerCard_LoadIcon(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID, u32 icon, BOOL draw);
void WiFiPlazaPlayerCard_FreeIcon(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaPlayerCard_LoadAvatar(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID, u32 avatar);
void WiFiPlazaPlayerCard_FreeAvatar(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaInfoPanel_TintPalette(NNSG2dPaletteData *pltt);
void WiFiPlazaPlayerCard_ClearWindows(WiFiPlazaPlayerCard *card);
void WiFiPlazaPlayerCard_Print(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, int window, int msgId, u8 x, u8 y, u32 color);
void WiFiPlazaPlayerCard_PrintRightAligned(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, int window, int msgId, u8 width, u8 y, u32 color);
void WiFiPlazaPlayerCard_CopyWindow(WiFiPlazaPlayerCard *card, int window);
void WiFiPlazaPlayerCard_BufferName(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, PlayerProfile *profile);
void WiFiPlazaPlayerCard_BufferLocation(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, struct WiFiPlazaPlayer *player);
void WiFiPlazaPlayerCard_BufferMessage(WiFiPlazaPlayerCard *card, WiFiPlazaRoomMsg *msg, u16 *words);
void WiFiPlazaPlayerCard_LoadMons(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics, NARC *panelNarc, enum HeapID heapID, struct WiFiPlazaPlayer *player, struct WiFiPlazaPlayer *myPlayer);
void WiFiPlazaPlayerCard_FreeMons(WiFiPlazaPlayerCard *card, WiFiPlazaRoomGraphics *graphics);
const WiFiPlazaCardMonInfo *WiFiPlazaPlayerCard_GetMonInfo(int species, int form, int version, int gender);
void WiFiPlazaPanelButton_Init(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID);
void WiFiPlazaPanelButton_Free(WiFiPlazaPanelButton *button);
BOOL WiFiPlazaPanelButton_Update(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, NARC *narc, enum HeapID heapID);
void WiFiPlazaPanelButton_ShowOwnCard(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, WiFiPlazaRoomMsg *msg, struct WiFiPlazaPlayer *player, NARC *narc, enum HeapID heapID);
void WiFiPlazaPanelButton_ShowInstrument(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, u32 type, NARC *narc, enum HeapID heapID);
void WiFiPlazaPanelButton_Clear(WiFiPlazaPanelButton *unused, WiFiPlazaRoomGraphics *graphics);
void WiFiPlazaPanelButton_Draw(WiFiPlazaPanelButton *button, WiFiPlazaRoomGraphics *graphics, int index, NARC *narc, enum HeapID heapID);
void WiFiPlazaPanelButton_OnTouch(u32 index, u32 event, void *arg);
void WiFiPlazaPanelButton_ResetTimer(WiFiPlazaPanelButton *button);
u32 WiFiPlazaRoom_GetTextSpeed(WiFiPlazaRoomWork *room);

// lobby (OVY_45) calls of the info panel, with the types it uses them with
u32 WiFiPlazaPlayer_GetGender(struct WiFiPlazaPlayer *player);
u16 WiFiPlazaPlayer_GetAvatar(struct WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetCountry(struct WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetRegion(struct WiFiPlazaPlayer *player);
int ov45_0222AAEC(struct WiFiPlazaPlayer *player, int index);
int ov45_0222A92C(struct WiFiPlazaPlayer *player, int index);
int ov45_0222A964(struct WiFiPlazaPlayer *player, int index);
u32 ov45_0222A99C(struct WiFiPlazaPlayer *player);
int ov45_0222AB28(WiFiPlazaWork *lobby, int index);
int ov45_0222AB48(WiFiPlazaWork *lobby, int index);
u16 *ov45_0222AB58(WiFiPlazaWork *lobby, int index);
void ov45_0222ADD8(WiFiPlazaWork *lobby, int index);
void ov45_0222AE54(WiFiPlazaWork *lobby);

// Lobby (OVY_45) calls, with the types this overlay uses them with (several return values that
// wifi_plaza_internal.h declares as void).
BOOL ov45_0222A288(WiFiPlazaWork *lobby, int bit);
u32 ov45_0222A2E0(WiFiPlazaWork *lobby);
u32 ov45_0222A324(WiFiPlazaWork *lobby);
u32 ov45_0222A330(WiFiPlazaWork *lobby);
u32 ov45_0222A35C(WiFiPlazaWork *lobby);
BOOL ov45_0222A374(WiFiPlazaWork *lobby);
u32 ov45_0222A394(WiFiPlazaWork *lobby);
BOOL ov45_0222A3A0(WiFiPlazaWork *lobby);
u32 ov45_0222A3BC(WiFiPlazaWork *lobby);
u32 ov45_0222A3D4(WiFiPlazaWork *lobby);
u32 ov45_0222A3EC(WiFiPlazaWork *lobby);
int ov45_0222A53C(WiFiPlazaWork *lobby);
int ov45_0222AD2C(WiFiPlazaWork *lobby);
int ov45_0222AD3C(WiFiPlazaWork *lobby);
int ov45_0222AD80(WiFiPlazaWork *lobby, int index);
int ov45_0222AD90(WiFiPlazaWork *lobby, int index);
int ov45_0222ADA0(WiFiPlazaWork *lobby);
int ov45_0222ADA8(WiFiPlazaWork *lobby, int index);
int ov45_0222ADE8(WiFiPlazaWork *lobby, int index);
void ov45_0222ADF8(WiFiPlazaWork *lobby, int index);
void ov45_0222AE08(u32 value, u32 *quotient, u32 *remainder);
u32 ov45_0222B1DC(WiFiPlazaWork *lobby);
u32 ov45_0222B1EC(WiFiPlazaWork *lobby);
struct WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(WiFiPlazaWork *work, u32 index);
struct WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(WiFiPlazaWork *work);
void WiFiPlazaPlayer_CopyToProfile(struct WiFiPlazaPlayer *player, PlayerProfile *profile, enum HeapID heapID);
u8 ov45_0222AAC8(struct WiFiPlazaPlayer *player);
BOOL ov45_0222AADC(void); // the player-state caller passes the player through r0; see WiFiPlazaRoom_Init
u32 ov45_0222A1FC(WiFiPlazaWork *work);
BOOL ov45_0222A33C(WiFiPlazaWork *work);
void ov45_0222AD70(void *unused, u32 value);
u8 ov45_0222D7C0(u32 index);
BOOL ov45_0222D844(void);
u32 ov45_0222E7CC(void);
int ov45_0222D7CC(u32 errorCode, u32 context);
u32 ov45_0222E7FC(u32 error);
BOOL ov45_0222F274(int index);
BOOL ov45_0222F294(int index);
u32 ov45_0222F314(int index);

// Lobby (OVY_45) calls of the player state handlers, the Plaza Games registration and the conversations.
BOOL ov45_0222A208(WiFiPlazaWork *lobby);
BOOL ov45_0222A230(WiFiPlazaWork *lobby, int index);
BOOL ov45_0222A25C(WiFiPlazaWork *lobby, int index);
BOOL ov45_0222A2A0(WiFiPlazaWork *lobby, int index);
u32 ov45_0222A2F8(WiFiPlazaWork *lobby);
void ov45_0222A310(WiFiPlazaWork *lobby);
BOOL ov45_0222A424(WiFiPlazaWork *lobby);
int ov45_0222A4B8(WiFiPlazaWork *lobby);
void ov45_0222A4C8(WiFiPlazaWork *lobby, u32 value);
void ov45_0222A4D0(WiFiPlazaWork *lobby);
void ov45_0222A520(WiFiPlazaWork *lobby, int value);
BOOL ov45_0222A550(WiFiPlazaWork *lobby, int index);
void ov45_0222A5E8(WiFiPlazaWork *lobby, int value);
void ov45_0222A704(WiFiPlazaWork *lobby, int index, int value);
void ov45_0222A72C(WiFiPlazaWork *lobby, u32 value);
void ov45_0222A770(WiFiPlazaWork *lobby, int first, int second);
int ov45_0222A920(WiFiPlazaPlayer *player);
int ov45_0222AB68(WiFiPlazaWork *lobby);
int ov45_0222AB78(WiFiPlazaWork *lobby, int index);
void ov45_0222AC14(WiFiPlazaWork *lobby, int mode, int value, int player0, int player1, int player2, int player3, int flag);
int ov45_0222AD58(WiFiPlazaWork *lobby, int index);
int ov45_0222ADB8(WiFiPlazaWork *lobby, int index, int choice);
void ov45_0222ADC8(WiFiPlazaWork *lobby, int index);
void ov45_0222AE44(WiFiPlazaWork *lobby);
BOOL ov45_0222AFF8(WiFiPlazaWork *lobby);
BOOL ov45_0222B00C(WiFiPlazaWork *lobby);
u16 ov45_0222B020(WiFiPlazaWork *lobby);
void ov45_0222B0A4(WiFiPlazaWork *lobby);
u32 ov45_0222B0B0(WiFiPlazaWork *lobby);
void ov45_0222B118(WiFiPlazaWork *lobby, int value);
int ov45_0222B1B4(WiFiPlazaWork *lobby);
BOOL ov45_0222EB38(int kind);
int ov45_0222EB74(void);
BOOL ov45_0222EB94(void);
BOOL ov45_0222EBC4(void);
BOOL ov45_0222EBF0(int kind);
void IncrementGameStat119(WiFiPlazaWork *lobby);
u32 WiFiPlazaPlayer_GetLanguage(WiFiPlazaPlayer *player);
BOOL WiFiPlazaPlayer_IsLanguageValid(WiFiPlazaPlayer *player);
void IncrementGameStat47(WiFiPlazaWork *lobby);
void *ov45_0222A2C8(WiFiPlazaWork *lobby);
void ov45_0222A404(WiFiPlazaWork *lobby);
int ov45_0222A414(WiFiPlazaWork *lobby);
void ov45_0222A43C(WiFiPlazaWork *lobby);
void ov45_0222A450(WiFiPlazaWork *lobby, int id, u32 index);
void ov45_0222A480(WiFiPlazaWork *lobby, u32 index);
void ov45_0222A498(WiFiPlazaWork *lobby, void *dest);
int ov45_0222A548(WiFiPlazaWork *lobby);
int ov45_0222A9A4(WiFiPlazaPlayer *player);
u32 ov45_0222A9C8(WiFiPlazaPlayer *player); // packed local time, hour in the low byte
void *ov45_0222AB1C(WiFiPlazaWork *lobby);
void ov45_0222AB94(WiFiPlazaWork *lobby, u32 slot0, u32 slot1);
void ov45_0222ABD0(WiFiPlazaWork *lobby, u32 slot0, u32 slot1, u8 value);
void ov45_0222AE64(WiFiPlazaWork *lobby);
int ov45_0222AE74(WiFiPlazaWork *lobby, int slot);
void ov45_0222AED8(WiFiPlazaWork *lobby, int value);
void ov45_0222AF80(WiFiPlazaWork *lobby);
void ov45_0222AFC4(WiFiPlazaWork *lobby);
u8 ov45_0222B028(WiFiPlazaWork *lobby);
u8 ov45_0222B034(WiFiPlazaWork *lobby);
u16 ov45_0222B040(WiFiPlazaWork *lobby);
BOOL ov45_0222B06C(WiFiPlazaWork *lobby);
u32 ov45_0222B094(WiFiPlazaWork *lobby);
void ov45_0222B0BC(WiFiPlazaWork *lobby);
void ov45_0222B0D8(WiFiPlazaWork *lobby, int slot);
void ov45_0222B0E8(WiFiPlazaWork *lobby, int slot);
int ov45_0222B134(WiFiPlazaWork *lobby);
int ov45_0222EC68(int id);
void ov45_0222F1BC(void);
BOOL ov45_0222F218(void);
int ov45_0222F2D4(int slot);
s16 ov45_0222F3E8(int slot);
int ov45_0222F430(void);
u8 ov45_0222F464(void);

// Typed player state handlers (that header includes this one first)
#include "wifi_plaza_player_state.h"

#endif // POKEHEARTGOLD_WIFI_PLAZA_ROOM_H
