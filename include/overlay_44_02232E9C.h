#ifndef POKEHEARTGOLD_OVY_44_02232E9C_H
#define POKEHEARTGOLD_OVY_44_02232E9C_H

#include "global.h"

#include "heap.h"
#include "list_menu.h"
#include "list_menu_2d.h"
#include "main.h"
#include "obj_char_transfer.h"
#include "overlay_42.h"
#include "render_window.h"
#include "sprite.h"
#include "unk_02009D48.h"
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

typedef struct UnkStruct_ov44_0222A758_args {
    SaveData *saveData;
    u32 choice;
    u32 partnerFriendIndex;
} UnkStruct_ov44_0222A758_args;

s32 WifiClub_HasBattleFrontier(WifiClubApp *arg0);
s32 WifiClub_IsPoffinCookingAvailable(WifiClubApp *arg0);
s32 WifiClub_ArePlazaGamesAvailable(WifiClubApp *arg0);
u32 WifiClub_GetPlayerStatus(WifiClubApp *arg0, WifiClubPlayerStatus *arg1);
s32 WifiClub_IsGroupActivityStatus(s32 arg0);
u32 WifiClub_GetGroupActivityChoice(u32 arg0);
s32 WifiClub_IsSeekingBattleStatus(u32 arg0);
s32 WifiClub_IsSeekingStatus(u32 arg0);
s32 WifiClub_IsInBattleStatus(u32 arg0);
s32 WifiClub_IsBusyStatus(u16 arg0);
s32 WifiClub_StatusNeedsTwoMons(u32 arg0);
u32 WifiClub_GetCounterpartStatus(u32 arg0);
s32 WifiClub_CountBattleReadyMons(WifiClubApp *arg0);
void WifiClub_SetLinkModeForStatus(u32 arg0);
void WifiClub_SetupGraphics(WifiClubApp *arg0);

s32 WifiClub_IsSeekingStatus(u32);
void WifiClub_PaletteCycleTask(s32 arg0, WifiClubPaletteCycle *arg1);
s32 WifiClub_Init(OverlayManager *overlayMananger, s32 *arg1);
s32 WifiClub_Main(OverlayManager *overlayManager, s32 *arg1);
void WifiClub_TeardownGraphics(WifiClubApp *arg0);
s32 WifiClub_Exit(OverlayManager *overlayManager);
void WifiClub_VBlankCB(WifiClubApp *arg0);
void WifiClub_SetVramBanks(void);
void WifiClub_InitBgs(BgConfig *arg0);
void WifiClub_AddPlayerObject(WifiClubApp *arg0, WifiClubRoomPerson *arg1);
void WifiClub_RemovePlayerObject(WifiClubApp *arg0, WifiClubRoomPerson *arg1);
WifiClubRoomPerson *WifiClub_FindPlayerObject(WifiClubApp *arg0, u32 arg1);
void WifiClub_LoadStatusIcons(BgConfig *arg0, WifiClubStatusIcons *arg1, NARC *arg2, enum HeapID arg3);
void WifiClub_FreeStatusIcons(WifiClubStatusIcons *arg0);
void WifiClub_DrawIconToBg(BgConfig *arg0, WifiClubStatusIcons *arg1, s32 arg2, u8 arg3, u8 arg4, s32 arg5, s32 arg6);
void WifiClub_DrawStatusIconToBg(BgConfig *arg0, WifiClubStatusIcons *arg1, s32 arg2, u8 arg3, u8 arg4, u32 arg5);
void WifiClub_DrawIconToWindow(Window *arg0, WifiClubStatusIcons *arg1, u8 arg2, u16 arg3, s32 arg4, s32 arg5);
void WifiClub_DrawStatusIconToWindow(Window *arg0, WifiClubStatusIcons *arg1, u8 arg2, u16 arg3, u32 arg4);
s32 WifiClub_GetIconPaletteBase(u32 arg0);
u32 WifiClub_FadeOutToEnterRoom(void);
void WifiClub_AllocStrings(WifiClubApp *arg0);
void WifiClub_FreeStrings(WifiClubApp *arg0);
void WifiClub_FreeBgConfig(BgConfig *arg0);
void WifiClub_LoadBgGraphics(WifiClubApp *arg0, NARC *arg1);
void WifiClub_InitObjTransfer(void);
void WifiClub_InitSpriteSystem(WifiClubApp *arg0, NARC *arg1);
void WifiClub_EnableObjPlane(WifiClubApp *arg0);
void WifiClub_PrintTitleBar(WifiClubApp *arg0);
void WifiClub_RemoveWindows(WifiClubApp *arg0);
s32 WifiClub_State_Start(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_CreateConnectPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ConnectPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_AskUpdateUserInfo(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_UpdateUserInfoPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ConfirmUpdateUserInfoPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_SaveUserInfoPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_StartConnecting(WifiClubApp *arg0, s32 arg1);
void WifiClub_InitOwnStatus(WifiClubApp *arg0, u32 arg1);
s32 WifiClub_CollectOnlineFriends(WifiClubApp *arg0);
s32 WifiClub_UpdateFriendStatuses(WifiClubApp *arg0);
void WifiClub_HandleWifiError(WifiClubApp *arg0);
s32 WifiClub_State_WaitErrorButton(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ErrorAcknowledged(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_GoToReconnectPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ErrorThenAskReconnect(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_CreateReconnectPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ReconnectPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Reconnect(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Connecting(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_PrepareSave(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_WriteSave(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_WaitThenReenterRoom(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ShowUserInfoSaved(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_UserInfoSavedWaitButton(WifiClubApp *arg0, s32 arg1);
void WifiClub_PrintOwnStatus(WifiClubApp *arg0);
void WifiClub_ListMenuCursorCallback(struct ListMenu *list, s32 index, u8 onInit);
void WifiClub_CreateStatusBarWindows(WifiClubApp *arg0);
s32 WifiClub_State_EnterRoom(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_IsMatchConnected(WifiClubApp *arg0);
s32 WifiClub_CheckIncomingContact(WifiClubApp *arg0);
s32 WifiClub_SyncRoomPlayers(WifiClubApp *arg0);
void WifiClub_RefreshFriendStatuses(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_UpdateRoomPlayers(WifiClubApp *arg0, enum HeapID heapID);
s32 WifiClub_State_Room(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_VoiceChatToggleMessage(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_WaitVoiceChatPartner(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_StartVoiceChat(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_VoiceChatStartMessage(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_VoiceChat(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_CreateEndVoiceChatPrompt(WifiClubApp *arg0, s32 arg1);

s32 WifiClub_State_EndVoiceChatPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_MessageThenLeaveMatch(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_MessageThenReenterRoom(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_MessageThenReturnToRoom(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Dummy30(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Dummy31(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Dummy32(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ErrorThenDisconnect(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_AskCancelSeeking(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_CreateCancelSeekingPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_CancelSeekingPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_OpenSeekMenu(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_SeekMenu(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_StartRecruiting(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_OpenSeekSubmenu(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_SeekSubmenu(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_TalkToPlayer(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_OpenApplyMenu(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_IsPartyValid(WifiClubPlayerStatus *arg0);
s32 WifiClub_GetCookingError(WifiClubApp *arg0);
void WifiClub_PlayRoomBgm(void);
BOOL WifiClub_IsRoomBgmPlaying(void);
s32 WifiClub_State_ApplyMenu(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_WaitMatchAccepted(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Dummy66(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Dummy67(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_MatchTimeout(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Dummy46(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Dummy47(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Contacted(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_CheckMatchLost(WifiClubApp *arg0);
s32 WifiClub_State_SyncStart(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_SendStatus(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ReceiveStatus(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_SendPlayerInfo(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_IsMessageFinished(WifiClubApp *arg0);
s32 WifiClub_State_StartActivity(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ReloadRoom(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_ToMessageWait(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_MessageWait(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_CloseMessage(WifiClubApp *arg0, s32 arg1);

s32 WifiClub_State_CreateDisconnectPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_DisconnectPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Disconnecting(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Disconnected(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_CreateRematchPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_RematchPrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_CreateVoiceChatModePrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_VoiceChatModePrompt(WifiClubApp *arg0, s32 arg1);
s32 WifiClub_State_Exit(WifiClubApp *arg0, s32 arg1);
void WifiClub_PrintMessage(WifiClubApp *arg0, s32 arg1, s32 arg2);
void WifiClub_ShowConnectionNotice(WifiClubApp *arg0, s32 arg1);
void WifiClub_BufferFriendName(WifiClubApp *arg0, s32 arg1);
void WifiClub_CloseMessage(WifiClubApp *arg0);
void WifiClub_SetStatusAndSend(WifiClubApp *arg0, u32 arg1);
void WifiClub_SetStatus(WifiClubApp *arg0, u32 arg1);
u8 WifiClub_ToggleVoiceChat(WifiClubApp *arg0);
u8 WifiClub_ToggleVoiceChatSetting(WifiClubApp *arg0);
u8 WifiClub_RestoreVoiceChatSetting(WifiClubApp *arg0);
void WifiClub_DeleteWaitingIcon(WifiClubApp *arg0);
void WifiClub_CancelRosterSelection(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_CancelRosterSelectionOf(WifiClubApp *arg0, s32 arg1, enum HeapID heapID);
s32 WifiClub_IsRosterScreenActive(WifiClubApp *arg0);
void WifiClub_InitRosterScreen(WifiClubApp *arg0, NARC *arg1, enum HeapID arg2);
void WifiClub_FreeRosterScreen(WifiClubApp *arg0);
u8 WifiClub_UpdateRosterScreen(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_RedrawRosterIfNeeded(WifiClubApp *arg0);
s32 WifiClub_ShouldCloseProfile(WifiClubApp *arg0, s32 arg1);
u8 WifiClub_GetSelectedRosterFriend(WifiClubApp *arg0);
void WifiClub_DeselectRosterFriend(WifiClubApp *arg0);
void WifiClub_ChangeProfilePage(WifiClubApp *arg0, s32 arg1);
void WifiClub_RosterHitboxCallback(s32 arg0, enum HeapID arg1, WifiClubApp *arg2);
void WifiClub_LoadRosterGraphics(WifiClubApp *arg0, NARC *arg1, enum HeapID arg2);
void WifiClub_FreeRosterGraphics(WifiClubApp *arg0);
void WifiClub_OffsetScreenTiles(NNSG2dScreenData *arg0, s32 arg1);
void WifiClub_SetRosterSlotState(WifiClubApp *arg0, s32 arg1, u32 arg2);
void WifiClub_ClearRosterSlotState(WifiClubApp *arg0, s32 arg1);
u8 WifiClub_GetRosterSlotState(WifiClubApp *arg0, s32 arg1);
void WifiClub_RedrawRoster(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_RequestRosterRedraw(WifiClubApp *arg0);
void WifiClub_SetRosterPagePalette(WifiClubApp *arg0);
void WifiClub_DrawRosterPage(WifiClubApp *arg0);
void WifiClub_DrawProfilePage(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_DrawProfileSummary(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_DrawProfileBattleTower(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_DrawProfileBattleFactory(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_DrawProfileBattleCastle(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_DrawProfileBattleHall(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_DrawProfileBattleArcade(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_DrawProfilePlazaGames(WifiClubApp *arg0, enum HeapID heapID);
void WifiClub_CoverFrontierPageTabs(WifiClubApp *arg0);
void WifiClub_PrintFrontierStat(WifiClubApp *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void WifiClub_GetFrontierStreakLabel(WifiClubApp *arg0, String *arg1, u32 arg2, s32 arg3);
void WifiClub_PrintCenteredNumber(WifiClubApp *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void WifiClub_DrawRosterSlotFrame(WifiClubApp *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
s32 WifiClub_GetStatusLabel(u32 arg0, u32 *color);
void WifiClub_PrintRosterSlotName(WifiClubApp *arg0, Window *arg1, s32 arg2, s32 arg3, s32 arg4);
void WifiClub_DrawRosterSlotIcons(WifiClubApp *arg0, Window *arg1, s32 arg2, u32 arg3, u32 arg4);
void WifiClub_AnimateRosterSelection(WifiClubApp *arg0);
void WifiClub_CreateProfileButtons(WifiClubApp *arg0, NARC *arg1, enum HeapID arg2);
void WifiClub_DeleteProfileButtons(WifiClubApp *arg0);
void WifiClub_ShowProfileButtons(WifiClubApp *arg0);
void WifiClub_HideProfileButtons(WifiClubApp *arg0);
s32 WifiClub_GetPressedProfileButton(WifiClubApp *arg0);
void WifiClub_ProfileButtonHitboxCallback(u32 arg0, u32 arg1, WifiClubApp *arg2);
void WifiClub_ResetProfileButton(WifiClubRoster *arg0, s32 arg1);
s32 WifiClub_UpdateProfileButton(WifiClubRoster *arg0, s32 arg1, s32 arg2, s32 arg3);

// 1
s32 WifiClub_IsGroupActivityStatus(s32);
void WifiClub_PrintMessage(WifiClubApp *, s32, s32);
void WifiClub_CloseMessage(WifiClubApp *);
s32 WifiClubRoom_GetTileBelowPlayer(WifiClubRoom *arg0);

// The following functions are static
void WifiClub_BufferRosterFriendName(WifiClubApp *arg0, s32 arg1);
WifiClubPlayerStatus *WifiClub_GetFriendStatusData(WifiClubApp *arg0, u32 arg1);
s32 WifiClub_GetFriendOnlineState(s32 arg0);
s32 WifiClub_StartMatch(WifiClubApp *arg0, s32 arg1, s32 arg2);
void WifiClub_StartSeeking(WifiClubApp *arg0, s32 arg1);
void WifiClub_StopSeeking(WifiClubApp *arg0);
YesNoPrompt *WifiClub_CreateYesNoPromptIgnoreTouch(BgConfig *arg0, const WindowTemplate *arg1, u32 arg2);
YesNoPrompt *WifiClub_CreateYesNoPrompt(BgConfig *arg0, const WindowTemplate *arg1, u32 arg2);
void WifiClubRoom_Init(WifiClubRoom *arg0, enum HeapID heapID, NARC *arg2, BgConfig *arg3, s32 arg4, u32 arg5);
void WifiClubRoom_Free(WifiClubRoom *arg0);
s32 WifiClubRoom_IsInitialized(WifiClubRoom *arg0);
s32 WifiClubRoom_Update(WifiClubRoom *arg0);
void WifiClubRoom_Draw(WifiClubRoom *arg0);
u8 WifiClubRoom_GetTalkPartnerId(WifiClubRoom *arg0);
s32 WifiClubRoom_GetTileUnderPlayer(WifiClubRoom *arg0);
void WifiClubRoom_ShowFriendMarker(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_HideFriendMarker(WifiClubRoom *arg0);
void WifiClubRoom_SetInputLocked(WifiClubRoom *arg0, s32 arg1);
s32 WifiClubRoom_IsInputLocked(WifiClubRoom *arg0);
WifiClubRoomPerson *WifiClubRoom_AddPlayer(WifiClubRoom *arg0, s16 arg1);
WifiClubRoomPerson *WifiClubRoom_AddFriend(WifiClubRoom *arg0, s16 arg1, s16 arg2);
void WifiClubRoom_StartPersonLeave(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoomPerson_Delete(WifiClubRoomPerson *arg0);
u8 WifiClubRoomPerson_GetId(WifiClubRoomPerson *arg0);
s32 WifiClubRoomPerson_GetOppositeDirection(WifiClubRoomPerson *arg0);
void WifiClubRoom_StartTalk(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s32 arg2);
void WifiClubRoom_EndTalk(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_SetPersonBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u8 arg2);
void WifiClubRoom_StartCounterLight(WifiClubRoom *arg0);
void WifiClubRoom_ContinueLightSequence(WifiClubRoom *arg0);
void WifiClubRoom_UpdateLights(WifiClubRoom *arg0);
void WifiClubRoom_DeleteAllPersons(WifiClubRoom *arg0);
s32 WifiClubRoom_IsPlayerIdle(WifiClubRoom *arg0);
s32 WifiClubRoom_IsPlayerFacingCounter(WifiClubRoom *arg0);
void WifiClubRoom_ProcessMoveCommands(WifiClubRoom *arg0);
void WifiClubRoom_UpdatePersons(WifiClubRoom *arg0);
void WifiClubRoom_AnimatePersons(WifiClubRoom *arg0);
WifiClubRoomPerson *WifiClubRoom_GetPersonInDirection(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u32 arg2);
void WifiClubRoom_CreateDrawers(WifiClubRoom *arg0, s32 arg1, enum HeapID arg2);
void WifiClubRoom_DeleteDrawers(WifiClubRoom *arg0);
void WifiClubRoom_CreateMapBg(WifiClubRoom *arg0, enum HeapID arg1, NARC *arg2, s32 arg3);
void WifiClubRoom_DeleteMapBg(WifiClubRoom *arg0);
void WifiClubRoomGfx_Init(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_Free(WifiClubRoomGfx *arg0);
void WifiClubRoomGfx_LoadResources(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_FreeResources(WifiClubRoomGfx *arg0);
void WifiClubRoomGfx_CreateSprites(WifiClubRoomGfx *arg1, enum HeapID heapID);
void WifiClubRoomGfx_DeleteSprites(WifiClubRoomGfx *arg0);
void WifiClubRoom_InitBgs(BgConfig *arg0, enum HeapID heapID);
void WifiClubRoom_FreeBgs(BgConfig *arg0);
void WifiClubRoom_LoadBgGfx(BgConfig *bgConfig, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_LoadSpriteResources(WifiClubRoomGfx *arg0, enum HeapID heapID, NARC *narc);
void WifiClubRoomGfx_FreeSpriteResources(WifiClubRoomGfx *arg0);
void WifiClubRoomGfx_CreateMarkerSprites(WifiClubRoomGfx *arg0, enum HeapID heapID);
void WifiClubRoomGfx_DeleteMarkerSprites(WifiClubRoomGfx *arg0);
void WifiClubRoom_ShowExitArrow(WifiClubRoom *arg0);
void WifiClubRoom_HideExitArrow(WifiClubRoom *arg0);
void WifiClubRoom_ShowFriendMarkerAt(WifiClubRoom *arg0, Field2DPoint arg1, s32 arg2);
void WifiClubRoom_HideFriendMarkerSprite(WifiClubRoom *arg0);
s32 WifiClubRoom_IsPlayerAtEntrance(WifiClubRoom *arg0);
void WifiClubRoom_FindFriendSpawnPos(WifiClubRoom *arg0, s16 *arg1, u32 arg2);
void WifiClubRoom_FindEntrancePos(WifiClubRoom *arg0, s16 *arg1);
WifiClubRoomPerson *WifiClubRoom_AllocPerson(WifiClubRoom *arg0);
void WifiClubRoom_CreatePersonDrawer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s32 arg2);
void WifiClubRoom_CreatePersonObject(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, s16 *arg2, s16 arg3, u16 arg4, u8 arg5, s32 arg6);
s32 WifiClubRoomPerson_CanTalk(WifiClubRoomPerson *arg0);
void WifiClubRoom_SetPersonModePlayer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_SetPersonModeWarpIn(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_SetPersonModeWander(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_SetPersonModeBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoomPerson_AnimateNone(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoomPerson_AnimateWarp(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoomPerson_AnimateHop(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdatePlayer(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateWarpIn(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateWarpOut(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateWander(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateBusy(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
s32 WifiClubRoomPerson_UpdateTalking(WifiClubRoom *arg0, WifiClubRoomPerson *arg1);
void WifiClubRoom_QueueMoveCommand(WifiClubRoom *arg0, s16 arg1, u32 arg2, u32 arg3);
void WifiClubRoom_QueuePersonCommand(WifiClubRoom *arg0, s16 arg1, WifiClubRoomPerson *arg2);
s32 WifiClubRoomPerson_IsNotWarping(WifiClubRoomPerson *arg0);
void WifiClubRoom_SetPersonActivityMode(WifiClubRoom *arg0, WifiClubRoomPerson *arg1, u8 arg2);
void WifiClubRoomLights_Init(WifiClubRoom *arg0, WifiClubRoomLights *arg1, NARC *arg2);
void WifiClubRoomLights_Free(WifiClubRoom *arg0, WifiClubRoomLights *arg1);
void WifiClubRoomLights_Update(WifiClubRoom *arg0, WifiClubRoomLights *arg1);
void WifiClubRoomLights_UpdateBlink(WifiClubRoomLights *arg0);
void WifiClubRoomLights_UpdateSequence(WifiClubRoomLights *arg0);
void *WifiClubRoomLights_GetColor(WifiClubRoomLights *arg0, s32 arg1);
void WifiClubRoomLights_EndSequence(WifiClubRoomLights *arg0);
void WifiClubRoomLights_Trigger(WifiClubRoomLights *arg0);
void WifiClubRoomLights_Start(WifiClubRoomLights *arg0, u8 arg1);
void WifiClubRoomLights_Continue(WifiClubRoomLights *arg0);
Field2DPoint WifiClubRoom_GetMapSize(s32 arg0);
const u32 *WifiClubRoom_GetMapCells(u32 arg0);

#endif // POKEHEARTGOLD_OVY_44_02232E9C_H
