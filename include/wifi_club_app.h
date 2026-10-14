#ifndef POKEHEARTGOLD_WIFI_CLUB_APP_H
#define POKEHEARTGOLD_WIFI_CLUB_APP_H

#include "global.h"

#include "heap.h"
#include "list_menu.h"
#include "list_menu_2d.h"
#include "main.h"
#include "obj_char_transfer.h"
#include "overlay_42.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "font_sprite.h"
#include "friend_roster.h"
#include "yes_no_prompt.h"

typedef struct WifiClubRoomLights {
    void *plttDataRaw;
    NNSG2dPaletteData *plttData;
    u8 blinkMask;
    u8 blinkPhase;
    s16 blinkTimer;
    u8 activeLight;
    u8 step;
    s16 stepTimer;
    u8 triggered;
} WifiClubRoomLights;

typedef struct WifiClubMenuItem {
    u32 strno;
    u32 value;
} WifiClubMenuItem;

typedef struct WifiClubRoom WifiClubRoom;
typedef struct WifiClubRoomPerson WifiClubRoomPerson;
typedef struct WifiClubApp WifiClubApp;

typedef s32 (*WifiClubRoomPersonUpdateFunc)(WifiClubRoom *, WifiClubRoomPerson *);
typedef void (*WifiClubRoomPersonAnimFunc)(WifiClubRoom *, WifiClubRoomPerson *);
typedef void (*WifiClubProfilePageFunc)(WifiClubApp *, enum HeapID);
typedef s32 (*WifiClubStateFunc)(WifiClubApp *, s32);

struct WifiClubRoomPerson {
    Field2DObject *object;
    Field2DObjectSprite *drawer;
    s16 timer;
    u8 talkPartnerId;
    u8 activityMode;
    u16 mode;
    u16 savedMode;
    WifiClubRoomPersonUpdateFunc update;
    void *savedUpdate;
    WifiClubRoomPersonAnimFunc animate;
};

typedef struct UnkStruct_ov44_02232C4C {
    s16 unk0;
    u8 unk2;
    u8 unk3;
} UnkStruct_ov44_02232C4C;

typedef struct WifiClubRoomGfx {
    SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    GF_2DGfxResMan *resManagers[4];
    SpriteResource *resources[4];
    SpriteResourcesHeader resourcesHeader;
    Sprite *exitArrowSprite;
    Sprite *friendMarkerSprite;
} WifiClubRoomGfx;

struct WifiClubRoom {
    s32 initialized;
    enum HeapID heapID;
    u8 unk8;
    u8 unk9;
    s16 unkA;
    u32 inputLocked;
    BgConfig *bgConfig;
    WifiClubRoomGfx gfx;
    WifiClubRoomPerson persons[48];
    WifiClubRoomPerson *player;
    WifiClubRoomLights lights;
    Field2DMap *map;
    Field2DObjectManager *objects;
    Field2DObjectSpriteManager *drawers;
    Field2DPoint cameraPos;
    Field2DBgLayer *mapBg;
    Field2DCommandQueue *commandQueue;
};

typedef struct WifiClubRoster {
    MessageFormat *msgFormat;
    void *slotScreenRaw;
    NNSG2dScreenData *slotScreen;
    void *pageScreensRaw[7];
    NNSG2dScreenData *pageScreens[7];
    void *tabCoverRaw;
    NNSG2dScreenData *tabCoverScreen;
    TouchHitboxController *slotHitboxes;
    u8 slotStates[0x20];
    u8 page;
    u8 selectedFriend;
    u8 selectAnimStep;
    u8 profileState;
    s8 profilePage;
    u8 redrawSlots;
    u8 selectAnimTimer;
    u8 prevSelectedFriend;
    u32 redrawAll;
    Window nameWindows[8];
    Window iconWindows[8][2];
    Window profileWindow;
    SpriteResource *buttonResources[4];
    Sprite *buttonSprites[3];
    ObjCharSpaceReservation closeTextCharSpace;
    TextOBJ *closeText;
    u32 buttonsShown;
    u32 touchedButton;
    u32 touchState;
    TouchHitboxController *buttonHitboxes;
} WifiClubRoster;

typedef struct WifiClubPlayerStatus {
    u16 species[6];
    u16 heldItems[6];
    u8 gameInfo[3];
    u8 status;
    u8 trainerInfo[5];
    u8 voiceChat;
    u8 voiceChatSetting;
    u8 unk23;
} WifiClubPlayerStatus;

typedef struct WifiClubStatusData {
    WifiClubPlayerStatus own;
    WifiClubPlayerStatus friends[0x20];
} WifiClubStatusData;

typedef struct WifiClubPaletteCycle {
    SysTask *task;
    u32 active;
    u16 basePalettes[4][0x10];
    u16 blendedPalettes[4][0x10];
    u8 unk108[0x220];
    s16 blendIndex;
    s8 reverse;
    u8 frameToggle;
    u32 targetScreens;
} WifiClubPaletteCycle;

typedef struct WifiClubStatusIcons {
    void *screenRaw;
    NNSG2dScreenData *screen;
    void *charsRaw;
    NNSG2dCharacterData *chars;
} WifiClubStatusIcons;

struct WifiClubApp {
    FriendRoster *friendRoster;
    WifiClubStatusData *clubData;
    u8 unk8[4];
    u8 onlineFriends[0x20];
    u8 prevOnlineFriends[0x20];
    u32 friendStatuses[0x20];
    u32 friendVoiceChat[0x20];
    ListMenuItem *unk14C;
    struct ListMenu *unk150;
    ListMenuItem *menuItems;
    struct ListMenu *listMenu;
    BgConfig *bgConfig;
    SaveData *saveData;
    MessageFormat *msgFormat;
    MsgData *msgData;
    MsgData *connectionMsgData;
    String *msgRaw;
    String *msgExpanded;
    String *tmpString;
    String *unk17C;
    u32 textPrinterId;
    struct ListMenu2D *yesNoMenu;
    YesNoPrompt *yesNoPrompt;
    WaitingIcon *waitingIcon;
    SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    GF_2DGfxResMan *gfxResMan[4];
    void *fontSystem;
    Window msgWindow;
    Window statusWindow;
    Window titleWindow;
    Window noticeWindow;
    Window unk310;
    Window menuWindow;
    Window unk330;
    s32 matchTimer;
    u32 unk344;
    u32 state;
    u32 exitChoice;
    u8 unk350[0x4];
    u32 startMode;
    u8 unk358[4];
    s32 timer;
    u8 unk360[4];
    u8 unk364;
    u8 unk365[3];
    u32 userInfoSaved;
    u8 unk36C[4];
    s32 contactFriend;
    u16 seekMenuCursor;
    u16 unk376;
    u8 unk378[4];
    u16 unk37C;
    u16 partnerStatus;
    u16 partnerVoiceChat;
    u16 partnerId;
    u32 isRematch;
    u8 unk388[4];
    WifiClubRoom room;
    WifiClubRoomPerson *playerObjects[0x21];
    WifiClubStatusIcons statusIcons;
    WifiClubRoster roster;
    u32 isSeeking;
    WifiClubPaletteCycle paletteCycle;
    WifiClubPlayerStatus hiddenFriendStatus;
    u16 resetBrightness;
    u16 friendCount;
};

typedef struct WifiClubArgs {
    SaveData *saveData;
    u32 choice;
    u32 partnerFriendIndex;
} WifiClubArgs;

s32 WifiClub_HasBattleFrontier(WifiClubApp *app);
s32 WifiClub_IsPoffinCookingAvailable(WifiClubApp *app);
s32 WifiClub_ArePlazaGamesAvailable(WifiClubApp *app);
u32 WifiClub_GetPlayerStatus(WifiClubApp *app, WifiClubPlayerStatus *playerStatus);
s32 WifiClub_IsGroupActivityStatus(s32 status);
u32 WifiClub_GetGroupActivityChoice(u32 status);
s32 WifiClub_IsSeekingBattleStatus(u32 status);
s32 WifiClub_IsSeekingStatus(u32 status);
s32 WifiClub_IsInBattleStatus(u32 status);
s32 WifiClub_IsBusyStatus(u16 status);
s32 WifiClub_StatusNeedsTwoMons(u32 status);
u32 WifiClub_GetCounterpartStatus(u32 status);
s32 WifiClub_CountBattleReadyMons(WifiClubApp *app);
void WifiClub_SetLinkModeForStatus(u32 status);
void WifiClub_SetupGraphics(WifiClubApp *app);

s32 WifiClub_IsSeekingStatus(u32);
void WifiClub_PaletteCycleTask(s32 task, WifiClubPaletteCycle *paletteCycle);
s32 WifiClub_Init(OverlayManager *overlayMananger, s32 *state);
s32 WifiClub_Main(OverlayManager *overlayManager, s32 *state);
void WifiClub_TeardownGraphics(WifiClubApp *app);
s32 WifiClub_Exit(OverlayManager *overlayManager);
void WifiClub_VBlankCB(WifiClubApp *app);
void WifiClub_SetVramBanks(void);
void WifiClub_InitBgs(BgConfig *bgConfig);
void WifiClub_AddPlayerObject(WifiClubApp *app, WifiClubRoomPerson *person);
void WifiClub_RemovePlayerObject(WifiClubApp *app, WifiClubRoomPerson *person);
WifiClubRoomPerson *WifiClub_FindPlayerObject(WifiClubApp *app, u32 personId);
void WifiClub_LoadStatusIcons(BgConfig *bgConfig, WifiClubStatusIcons *statusIcons, NARC *narc, enum HeapID heapID);
void WifiClub_FreeStatusIcons(WifiClubStatusIcons *statusIcons);
void WifiClub_DrawIconToBg(BgConfig *bgConfig, WifiClubStatusIcons *statusIcons, s32 bgId, u8 x, u8 y, s32 iconIndex, s32 paletteOffset);
void WifiClub_DrawStatusIconToBg(BgConfig *bgConfig, WifiClubStatusIcons *statusIcons, s32 bgId, u8 x, u8 y, u32 status);
void WifiClub_DrawIconToWindow(Window *window, WifiClubStatusIcons *statusIcons, u8 x, u16 y, s32 iconIndex, s32 paletteOffset);
void WifiClub_DrawStatusIconToWindow(Window *window, WifiClubStatusIcons *statusIcons, u8 x, u16 y, u32 status);
s32 WifiClub_GetIconPaletteBase(u32 bgId);
u32 WifiClub_FadeOutToEnterRoom(void);
void WifiClub_AllocStrings(WifiClubApp *app);
void WifiClub_FreeStrings(WifiClubApp *app);
void WifiClub_FreeBgConfig(BgConfig *bgConfig);
void WifiClub_LoadBgGraphics(WifiClubApp *app, NARC *narc);
void WifiClub_InitObjTransfer(void);
void WifiClub_InitSpriteSystem(WifiClubApp *app, NARC *narc);
void WifiClub_EnableObjPlane(WifiClubApp *app);
void WifiClub_PrintTitleBar(WifiClubApp *app);
void WifiClub_RemoveWindows(WifiClubApp *app);
s32 WifiClub_State_Start(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_CreateConnectPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ConnectPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_AskUpdateUserInfo(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_UpdateUserInfoPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ConfirmUpdateUserInfoPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_SaveUserInfoPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_StartConnecting(WifiClubApp *app, s32 mainState);
void WifiClub_InitOwnStatus(WifiClubApp *app, u32 status);
s32 WifiClub_CollectOnlineFriends(WifiClubApp *app);
s32 WifiClub_UpdateFriendStatuses(WifiClubApp *app);
void WifiClub_HandleWifiError(WifiClubApp *app);
s32 WifiClub_State_WaitErrorButton(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ErrorAcknowledged(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_GoToReconnectPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ErrorThenAskReconnect(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_CreateReconnectPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ReconnectPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Reconnect(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Connecting(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_PrepareSave(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_WriteSave(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_WaitThenReenterRoom(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ShowUserInfoSaved(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_UserInfoSavedWaitButton(WifiClubApp *app, s32 mainState);
void WifiClub_PrintOwnStatus(WifiClubApp *app);
void WifiClub_ListMenuCursorCallback(struct ListMenu *list, s32 index, u8 onInit);
void WifiClub_CreateStatusBarWindows(WifiClubApp *app);
s32 WifiClub_State_EnterRoom(WifiClubApp *app, s32 mainState);
s32 WifiClub_IsMatchConnected(WifiClubApp *app);
s32 WifiClub_CheckIncomingContact(WifiClubApp *app);
s32 WifiClub_SyncRoomPlayers(WifiClubApp *app);
void WifiClub_RefreshFriendStatuses(WifiClubApp *app, enum HeapID heapID);
void WifiClub_UpdateRoomPlayers(WifiClubApp *app, enum HeapID heapID);
s32 WifiClub_State_Room(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_VoiceChatToggleMessage(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_WaitVoiceChatPartner(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_StartVoiceChat(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_VoiceChatStartMessage(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_VoiceChat(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_CreateEndVoiceChatPrompt(WifiClubApp *app, s32 mainState);

s32 WifiClub_State_EndVoiceChatPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_MessageThenLeaveMatch(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_MessageThenReenterRoom(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_MessageThenReturnToRoom(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Dummy30(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Dummy31(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Dummy32(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ErrorThenDisconnect(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_AskCancelSeeking(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_CreateCancelSeekingPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_CancelSeekingPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_OpenSeekMenu(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_SeekMenu(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_StartRecruiting(WifiClubApp *app, s32 mainState);
s32 WifiClub_OpenSeekSubmenu(WifiClubApp *app, s32 menuChoice);
s32 WifiClub_State_SeekSubmenu(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_TalkToPlayer(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_OpenApplyMenu(WifiClubApp *app, s32 mainState);
s32 WifiClub_IsPartyValid(WifiClubPlayerStatus *playerStatus);
s32 WifiClub_GetCookingError(WifiClubApp *app);
void WifiClub_PlayRoomBgm(void);
BOOL WifiClub_IsRoomBgmPlaying(void);
s32 WifiClub_State_ApplyMenu(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_WaitMatchAccepted(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Dummy66(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Dummy67(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_MatchTimeout(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Dummy46(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Dummy47(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Contacted(WifiClubApp *app, s32 mainState);
s32 WifiClub_CheckMatchLost(WifiClubApp *app);
s32 WifiClub_State_SyncStart(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_SendStatus(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ReceiveStatus(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_SendPlayerInfo(WifiClubApp *app, s32 mainState);
s32 WifiClub_IsMessageFinished(WifiClubApp *app);
s32 WifiClub_State_StartActivity(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ReloadRoom(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_ToMessageWait(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_MessageWait(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_CloseMessage(WifiClubApp *app, s32 mainState);

s32 WifiClub_State_CreateDisconnectPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_DisconnectPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Disconnecting(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Disconnected(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_CreateRematchPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_RematchPrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_CreateVoiceChatModePrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_VoiceChatModePrompt(WifiClubApp *app, s32 mainState);
s32 WifiClub_State_Exit(WifiClubApp *app, s32 mainState);
void WifiClub_PrintMessage(WifiClubApp *app, s32 msgId, s32 isConnectionMsg);
void WifiClub_ShowConnectionNotice(WifiClubApp *app, s32 msgId);
void WifiClub_BufferFriendName(WifiClubApp *app, s32 friendIndex);
void WifiClub_CloseMessage(WifiClubApp *app);
void WifiClub_SetStatusAndSend(WifiClubApp *app, u32 status);
void WifiClub_SetStatus(WifiClubApp *app, u32 status);
u8 WifiClub_ToggleVoiceChat(WifiClubApp *app);
u8 WifiClub_ToggleVoiceChatSetting(WifiClubApp *app);
u8 WifiClub_RestoreVoiceChatSetting(WifiClubApp *app);
void WifiClub_DeleteWaitingIcon(WifiClubApp *app);
void WifiClub_CancelRosterSelection(WifiClubApp *app, enum HeapID heapID);
void WifiClub_CancelRosterSelectionOf(WifiClubApp *app, s32 friendId, enum HeapID heapID);
s32 WifiClub_IsRosterScreenActive(WifiClubApp *app);
void WifiClub_InitRosterScreen(WifiClubApp *app, NARC *narc, enum HeapID heapID);
void WifiClub_FreeRosterScreen(WifiClubApp *app);
u8 WifiClub_UpdateRosterScreen(WifiClubApp *app, enum HeapID heapID);
void WifiClub_RedrawRosterIfNeeded(WifiClubApp *app);
s32 WifiClub_ShouldCloseProfile(WifiClubApp *app, s32 pressedButton);
u8 WifiClub_GetSelectedRosterFriend(WifiClubApp *app);
void WifiClub_DeselectRosterFriend(WifiClubApp *app);
void WifiClub_ChangeProfilePage(WifiClubApp *app, s32 pressedButton);
void WifiClub_RosterHitboxCallback(s32 index, enum HeapID event, WifiClubApp *app);
void WifiClub_LoadRosterGraphics(WifiClubApp *app, NARC *narc, enum HeapID heapID);
void WifiClub_FreeRosterGraphics(WifiClubApp *app);
void WifiClub_OffsetScreenTiles(NNSG2dScreenData *screenData, s32 tileOffset);
void WifiClub_SetRosterSlotState(WifiClubApp *app, s32 friendId, u32 slotState);
void WifiClub_ClearRosterSlotState(WifiClubApp *app, s32 friendId);
u8 WifiClub_GetRosterSlotState(WifiClubApp *app, s32 friendId);
void WifiClub_RedrawRoster(WifiClubApp *app, enum HeapID heapID);
void WifiClub_RequestRosterRedraw(WifiClubApp *app);
void WifiClub_SetRosterPagePalette(WifiClubApp *app);
void WifiClub_DrawRosterPage(WifiClubApp *app);
void WifiClub_DrawProfilePage(WifiClubApp *app, enum HeapID heapID);
void WifiClub_DrawProfileSummary(WifiClubApp *app, enum HeapID heapID);
void WifiClub_DrawProfileBattleTower(WifiClubApp *app, enum HeapID heapID);
void WifiClub_DrawProfileBattleFactory(WifiClubApp *app, enum HeapID heapID);
void WifiClub_DrawProfileBattleCastle(WifiClubApp *app, enum HeapID heapID);
void WifiClub_DrawProfileBattleHall(WifiClubApp *app, enum HeapID heapID);
void WifiClub_DrawProfileBattleArcade(WifiClubApp *app, enum HeapID heapID);
void WifiClub_DrawProfilePlazaGames(WifiClubApp *app, enum HeapID heapID);
void WifiClub_CoverFrontierPageTabs(WifiClubApp *app);
void WifiClub_PrintFrontierStat(WifiClubApp *app, s32 msgId, s32 stat, s32 friendIndex, s32 x, s32 y);
void WifiClub_GetFrontierStreakLabel(WifiClubApp *app, String *dest, u32 facility, s32 friendIndex);
void WifiClub_PrintCenteredNumber(WifiClubApp *app, s32 msgId, s32 value, s32 centerX, s32 y);
void WifiClub_DrawRosterSlotFrame(WifiClubApp *app, u8 x, u8 y, u8 frameStyle, u8 animStep);
s32 WifiClub_GetStatusLabel(u32 status, u32 *color);
void WifiClub_PrintRosterSlotName(WifiClubApp *app, Window *window, s32 friendId, s32 animStep, s32 slot);
void WifiClub_DrawRosterSlotIcons(WifiClubApp *app, Window *windows, s32 friendId, u32 animStep, u32 slot);
void WifiClub_AnimateRosterSelection(WifiClubApp *app);
void WifiClub_CreateProfileButtons(WifiClubApp *app, NARC *narc, enum HeapID heapID);
void WifiClub_DeleteProfileButtons(WifiClubApp *app);
void WifiClub_ShowProfileButtons(WifiClubApp *app);
void WifiClub_HideProfileButtons(WifiClubApp *app);
s32 WifiClub_GetPressedProfileButton(WifiClubApp *app);
void WifiClub_ProfileButtonHitboxCallback(u32 index, u32 event, WifiClubApp *app);
void WifiClub_ResetProfileButton(WifiClubRoster *roster, s32 button);
s32 WifiClub_UpdateProfileButton(WifiClubRoster *roster, s32 button, s32 touchedButton, s32 touchState);

// 1
s32 WifiClub_IsGroupActivityStatus(s32);
void WifiClub_PrintMessage(WifiClubApp *, s32, s32);
void WifiClub_CloseMessage(WifiClubApp *);
s32 WifiClubRoom_GetTileBelowPlayer(WifiClubRoom *room);

// The following functions are static
void WifiClub_BufferRosterFriendName(WifiClubApp *app, s32 friendIndex);
WifiClubPlayerStatus *WifiClub_GetFriendStatusData(WifiClubApp *app, u32 friendIndex);
s32 WifiClub_GetFriendOnlineState(s32 friendIndex);
s32 WifiClub_StartMatch(WifiClubApp *app, s32 friendIndex, s32 status);
void WifiClub_StartSeeking(WifiClubApp *app, s32 showMessage);
void WifiClub_StopSeeking(WifiClubApp *app);
YesNoPrompt *WifiClub_CreateYesNoPromptIgnoreTouch(BgConfig *bgConfig, const WindowTemplate *windowTemplate, u32 initialCursorPos);
YesNoPrompt *WifiClub_CreateYesNoPrompt(BgConfig *bgConfig, const WindowTemplate *windowTemplate, u32 initialCursorPos);
void WifiClubRoom_Init(WifiClubRoom *room, enum HeapID heapID, NARC *narc, BgConfig *bgConfig, s32 playerSpriteId, u32 friendCount);
void WifiClubRoom_Free(WifiClubRoom *room);
s32 WifiClubRoom_IsInitialized(WifiClubRoom *room);
s32 WifiClubRoom_Update(WifiClubRoom *room);
void WifiClubRoom_Draw(WifiClubRoom *room);
u8 WifiClubRoom_GetTalkPartnerId(WifiClubRoom *room);
s32 WifiClubRoom_GetTileAtPlayer(WifiClubRoom *room);
void WifiClubRoom_ShowFriendMarker(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoom_HideFriendMarker(WifiClubRoom *room);
void WifiClubRoom_SetInputLocked(WifiClubRoom *room, s32 locked);
s32 WifiClubRoom_IsInputLocked(WifiClubRoom *room);
WifiClubRoomPerson *WifiClubRoom_AddPlayer(WifiClubRoom *room, s16 spriteId);
WifiClubRoomPerson *WifiClubRoom_AddFriend(WifiClubRoom *room, s16 spriteId, s16 id);
void WifiClubRoom_StartPersonLeave(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoomPerson_Delete(WifiClubRoomPerson *person);
u8 WifiClubRoomPerson_GetId(WifiClubRoomPerson *person);
s32 WifiClubRoomPerson_GetOppositeDirection(WifiClubRoomPerson *person);
void WifiClubRoom_StartTalk(WifiClubRoom *room, WifiClubRoomPerson *person, s32 direction);
void WifiClubRoom_EndTalk(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoom_SetPersonBusy(WifiClubRoom *room, WifiClubRoomPerson *person, u8 activityMode);
void WifiClubRoom_StartCounterLight(WifiClubRoom *room);
void WifiClubRoom_ContinueLightSequence(WifiClubRoom *room);
void WifiClubRoom_UpdateLights(WifiClubRoom *room);
void WifiClubRoom_DeleteAllPersons(WifiClubRoom *room);
s32 WifiClubRoom_IsPlayerIdle(WifiClubRoom *room);
s32 WifiClubRoom_IsPlayerFacingCounter(WifiClubRoom *room);
void WifiClubRoom_ProcessMoveCommands(WifiClubRoom *room);
void WifiClubRoom_UpdatePersons(WifiClubRoom *room);
void WifiClubRoom_AnimatePersons(WifiClubRoom *room);
WifiClubRoomPerson *WifiClubRoom_GetPersonInDirection(WifiClubRoom *room, WifiClubRoomPerson *person, u32 direction);
void WifiClubRoom_CreateDrawers(WifiClubRoom *room, s32 spriteId, enum HeapID heapID);
void WifiClubRoom_DeleteDrawers(WifiClubRoom *room);
void WifiClubRoom_CreateMapBg(WifiClubRoom *room, enum HeapID heapID, NARC *narc, s32 mapIndex);
void WifiClubRoom_DeleteMapBg(WifiClubRoom *room);
void WifiClubRoomGfx_Init(WifiClubRoomGfx *gfx, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_Free(WifiClubRoomGfx *gfx);
void WifiClubRoomGfx_LoadResources(WifiClubRoomGfx *gfx, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_FreeResources(WifiClubRoomGfx *gfx);
void WifiClubRoomGfx_CreateSprites(WifiClubRoomGfx *gfx, enum HeapID heapID);
void WifiClubRoomGfx_DeleteSprites(WifiClubRoomGfx *gfx);
void WifiClubRoom_InitBgs(BgConfig *bgConfig, enum HeapID heapID);
void WifiClubRoom_FreeBgs(BgConfig *bgConfig);
void WifiClubRoom_LoadBgGfx(BgConfig *bgConfig, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_LoadSpriteResources(WifiClubRoomGfx *gfx, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_FreeSpriteResources(WifiClubRoomGfx *gfx);
void WifiClubRoomGfx_CreateMarkerSprites(WifiClubRoomGfx *gfx, enum HeapID heapID);
void WifiClubRoomGfx_DeleteMarkerSprites(WifiClubRoomGfx *gfx);
void WifiClubRoom_ShowExitArrow(WifiClubRoom *room);
void WifiClubRoom_HideExitArrow(WifiClubRoom *room);
void WifiClubRoom_ShowFriendMarkerAt(WifiClubRoom *room, Field2DPoint pos, s32 drawPriority);
void WifiClubRoom_HideFriendMarkerSprite(WifiClubRoom *room);
s32 WifiClubRoom_IsPlayerAtEntrance(WifiClubRoom *room);
void WifiClubRoom_FindFriendSpawnPos(WifiClubRoom *room, s16 *pos, u32 spawnIndex);
void WifiClubRoom_FindEntrancePos(WifiClubRoom *room, s16 *pos);
WifiClubRoomPerson *WifiClubRoom_AllocPerson(WifiClubRoom *room);
void WifiClubRoom_CreatePersonDrawer(WifiClubRoom *room, WifiClubRoomPerson *person, s32 priorityMode);
void WifiClubRoom_CreatePersonObject(WifiClubRoom *room, WifiClubRoomPerson *person, s16 *pos, s16 id, u16 spriteId, u8 direction, s32 action);
s32 WifiClubRoomPerson_CanTalk(WifiClubRoomPerson *person);
void WifiClubRoom_SetPersonModePlayer(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoom_SetPersonModeWarpIn(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoom_SetPersonModeWander(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoom_SetPersonModeBusy(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoomPerson_AnimateNone(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoomPerson_AnimateWarp(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoomPerson_AnimateHop(WifiClubRoom *room, WifiClubRoomPerson *person);
s32 WifiClubRoomPerson_UpdatePlayer(WifiClubRoom *room, WifiClubRoomPerson *person);
s32 WifiClubRoomPerson_UpdateWarpIn(WifiClubRoom *room, WifiClubRoomPerson *person);
s32 WifiClubRoomPerson_UpdateWarpOut(WifiClubRoom *room, WifiClubRoomPerson *person);
s32 WifiClubRoomPerson_UpdateWander(WifiClubRoom *room, WifiClubRoomPerson *person);
s32 WifiClubRoomPerson_UpdateBusy(WifiClubRoom *room, WifiClubRoomPerson *person);
s32 WifiClubRoomPerson_UpdateTalking(WifiClubRoom *room, WifiClubRoomPerson *person);
void WifiClubRoom_QueueMoveCommand(WifiClubRoom *room, s16 action, u32 direction, u32 objectId);
void WifiClubRoom_QueuePersonCommand(WifiClubRoom *room, s16 action, WifiClubRoomPerson *person);
s32 WifiClubRoomPerson_IsNotWarping(WifiClubRoomPerson *person);
void WifiClubRoom_SetPersonActivityMode(WifiClubRoom *room, WifiClubRoomPerson *person, u8 activityMode);
void WifiClubRoomLights_Init(WifiClubRoom *room, WifiClubRoomLights *lights, NARC *narc);
void WifiClubRoomLights_Free(WifiClubRoom *room, WifiClubRoomLights *lights);
void WifiClubRoomLights_Update(WifiClubRoom *room, WifiClubRoomLights *lights);
void WifiClubRoomLights_UpdateBlink(WifiClubRoomLights *lights);
void WifiClubRoomLights_UpdateSequence(WifiClubRoomLights *lights);
void *WifiClubRoomLights_GetColor(WifiClubRoomLights *lights, s32 colorIndex);
void WifiClubRoomLights_EndSequence(WifiClubRoomLights *lights);
void WifiClubRoomLights_Trigger(WifiClubRoomLights *lights);
void WifiClubRoomLights_Start(WifiClubRoomLights *lights, u8 light);
void WifiClubRoomLights_Continue(WifiClubRoomLights *lights);
Field2DPoint WifiClubRoom_GetMapSize(s32 mapIndex);
const u32 *WifiClubRoom_GetMapCells(u32 mapIndex);

#endif // POKEHEARTGOLD_WIFI_CLUB_APP_H
