#ifndef POKEHEARTGOLD_OVY_44_H
#define POKEHEARTGOLD_OVY_44_H

#include "list_menu_2d.h"
#include "wifi_club_app.h"
#include "render_window.h"
#include "sprite_resource_header.h"

typedef struct WifiClubGameEntryAvatar {
    Field2DObject *object;
    Field2DObjectSprite *drawer;
    u16 state;
    s16 timer;
} WifiClubGameEntryAvatar;

typedef struct WifiClubGameEntryArgs {
    u8 game;
    u8 minPlayers;
    u8 maxPlayers;
    u8 result;
    u8 voiceChat;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    SaveData *saveData;
} WifiClubGameEntryArgs;

typedef struct WifiClubGameEntryJoinAnswer {
    s16 netId;
    s32 answer;
} WifiClubGameEntryJoinAnswer;

typedef struct WifiClubGameEntryScreen {
    BgConfig *bgConfig;
    MessageFormat *msgFmt;
    MsgData *msgData;
    String *string1;
    String *string2;
    u8 printerId;
    u8 messageDelay;
    u16 textFrameDelay;
    SpriteList *spriteList; // 0x18
    G2dRenderer g2dRenderer;
    GF_2DGfxResMan *gf2DGfxResManList[4];
    Field2DObjectManager *objects;
    Field2DObjectSpriteManager *drawers;
    WifiClubGameEntryAvatar avatars[4];
    Window windowList[5]; // 0x18C - 0x1DC
    struct ListMenu2D *listMenu2D;
    void *scrnDataRaw;
    NNSG2dScreenData *scrnData;
    WaitingIcon *waitingIcon; // 0x218;
    void *plttDataRaw;
    NNSG2dPaletteData *plttData;
    s16 paletteTimer;
    s16 paletteFrame;
} WifiClubGameEntryScreen;

typedef struct WifiClubGameEntry {
    WifiClubPlayerStatus *clubStatus;
    u8 isLeader;
    u8 state;
    u8 applicantNetId;
    u8 result;
    s32 timer;
    s32 timeoutTimer;
    u8 timeoutRunning;
    u8 voiceChatOn;
    u8 membersChanged;
    u8 joinAnswer;
    u8 disconnected[4];
    u8 disconnectedNetIds[4];
    u8 shownVoiceChatFlags[4];
    u8 voiceChatFlags[4];
    u8 memberListDirty;
    u8 lastAcceptedNetId;
    u8 leaderFriendIndex;
    u8 watchConnection;
    u8 acceptedByLeader[4];
    u8 startSyncStarted;
    u8 refusedMsg;
    WifiClubGameEntryScreen screen;
} WifiClubGameEntry;

typedef void (*WifiClubGameEntryAvatarFunc)(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1);
typedef s32 (*WifiClubGameEntryStateFunc)(WifiClubGameEntry *, WifiClubGameEntryArgs *, enum HeapID);

void WifiClubGameEntry_Init(OverlayManager *overlayMananger);
s32 WifiClubGameEntry_Main(OverlayManager *overlayManager, u32 *state);
s32 WifiClubGameEntry_Exit(OverlayManager *man);
void WifiClubGameEntry_OnJoinAnswer(WifiClubGameEntry *gameEntry, String *string);
void WifiClubGameEntry_OnStartCommand(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_OnVoiceChatFlags(WifiClubGameEntry *gameEntry, String *string);

// The following functions are likely static
s32 WifiClubGameEntry_Member_WaitFinalSync(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
void WifiClubGameEntry_VBlankCB(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_IsLocalLeader(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_InitScreen(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
void WifiClubGameEntry_FreeScreen(WifiClubGameEntry *gameEntry, enum HeapID heapID);
void WifiClubGameEntry_SetGfxBanks(void);
void InitBgAndScrnData(WifiClubGameEntryScreen *screen, enum HeapID heapID);
void FreeBgAndScreenData(WifiClubGameEntryScreen *screen);
void InitWindowList(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, enum HeapID heapID);
void FreeWindowList(WifiClubGameEntryScreen *screen, enum HeapID heapID);
void InitStringAndMsgData(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, enum HeapID heapID);
void FreeStringAndMsgData(WifiClubGameEntryScreen *screen);
void WifiClubGameEntry_PrintTitle(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, enum HeapID heapID);
void InitSpritesAnd2DGfxResObjs(WifiClubGameEntryScreen *screen, enum HeapID heapID);
void FreeSpritesAnd2DGfxResObjs(WifiClubGameEntryScreen *screen);
void Wifi_LoadAndPrintTextToWindowWrapper(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, s32 strno, enum HeapID heapID);
void Wifi_LoadAndPrintTextToWindow(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, s32 strno, s32 messageDelay, enum HeapID heapID);
s32 WifiClubGameEntry_IsMessageFinished(WifiClubGameEntryScreen *screen);
void WifiClubGameEntry_ClearMessage(WifiClubGameEntryScreen *screen);
void WifiClubGameEntry_ShowApplicantInfo(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, s32 netId, enum HeapID heapID);
void WifiClubGameEntry_HideApplicantInfo(WifiClubGameEntryScreen *screen);
void WifiClubGameEntry_DrawMemberList(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
void WifiClubGameEntry_DrawMember(WifiClubGameEntry *gameEntry, WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, s32 netId, enum HeapID heapID);
void WifiClubGameEntry_ClearMember(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, s32 netId, enum HeapID heapID);
void WifiClubGameEntry_BufferPlayerName(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, s32 netId, s32 idx, enum HeapID heapID);
void WifiClubGameEntry_BufferTrainerId(WifiClubGameEntryScreen *screen, WifiClubGameEntryArgs *args, s32 netId, s32 idx, enum HeapID heapID);
void WifiClubGameEntry_AnimateBgPalette(WifiClubGameEntryScreen *screen);
void WifiClubGameEntry_LoadBgPaletteFrame(WifiClubGameEntryScreen *screen, s32 frame);
u8 WifiClubGameEntry_ToggleVoiceChat(WifiClubGameEntry *gameEntry, enum HeapID heapID);
void WifiClubGameEntry_CheckConnectionLost(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
void WifiClubGameEntry_CheckMatchFailed(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_IsAnyoneConnected(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_StartMatch(u32 friendIndex, u32 game);
void WifiClubGameEntry_SetPlayingStatus(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args);
s32 WifiClubGameEntry_StartVoiceChat(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_StopVoiceChat(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_EnableVoiceChat(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_DisableVoiceChat(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_CountMembers(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_AcceptEarlierMembers(WifiClubGameEntry *gameEntry, enum HeapID heapID);
void WifiClubGameEntry_RefusePendingApplicants(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_ReleaseAllMembers(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_SetDisconnectCallback(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_ClearDisconnectCallback(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_CheckMemberDisconnected(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_OnDisconnect(u8 netId, WifiClubGameEntry *gameEntry, s32 arg2);
s32 WifiClubGameEntry_CheckMemberListChanged(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_RequestMemberListRedraw(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_AcceptMember(WifiClubGameEntry *gameEntry, s32 netId, enum HeapID heapID);
void WifiClubGameEntry_AddMemberAvatar(WifiClubGameEntry *gameEntry, s32 netId, enum HeapID heapID);
void WifiClubGameEntry_UpdateAvatars(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_DeleteAvatars(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_SendLeaderBusy(WifiClubGameEntry *gameEntry, s32 netId);
void WifiClubGameEntry_RefuseLateApplicants(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_AcceptAnnouncedMembers(WifiClubGameEntry *gameEntry, enum HeapID heapID);
void WifiClubGameEntry_ShowWaitingIcon(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_HideWaitingIcon(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
void WifiClubGameEntry_AvatarNone(WifiClubGameEntry *gameEntry, WifiClubGameEntryAvatar *avatar);
void WifiClubGameEntry_AvatarStartSpin(WifiClubGameEntry *gameEntry, WifiClubGameEntryAvatar *avatar);
void WifiClubGameEntry_AvatarUpdateSpin(WifiClubGameEntry *gameEntry, WifiClubGameEntryAvatar *avatar);
void WifiClubGameEntry_AvatarIdle(WifiClubGameEntry *gameEntry, WifiClubGameEntryAvatar *avatar);
void WifiClubGameEntry_SendVoiceChatFlags(WifiClubGameEntry *gameEntry);
int WifiClubGameEntry_IsStartSynced(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_StartTimeout(WifiClubGameEntry *gameEntry);
void WifiClubGameEntry_StopTimeout(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_HasTimedOut(WifiClubGameEntry *gameEntry);
s32 WifiClubGameEntry_Leader_Start(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptReadyMessage(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleInput(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptUserJoinRequest(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowAcceptYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleAcceptYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptInsufficientMembers(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_WaitNotEnoughMembers(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptConfirmMembers(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowConfirmYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleConfirmYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_StartCommunicating(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_WaitMembersConnected(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_SendStartCommand(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_WaitStartSync(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_SendStatus(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_CheckStatuses(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_StartFinalSync(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_WaitFinalSync(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptStopFindingMembers(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowStopSeekingYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleStopSeekingYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptDropAsLeader(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowDisbandYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleDisbandYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptCancelInvites(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_Cancel(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_OnMemberDropped(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptMemberDropped(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_EndAfterMemberDropped(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptToggleVoiceChat(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowVoiceChatYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleVoiceChatYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_Start(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitLeaderInfo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptAwaitingResponse(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitJoinAnswer(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_PrintApproved(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitApproved(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_PrintRefused(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_EndRefused(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptAwaitingMembers(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitAwaitingMessage(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitStart(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_SendStatus(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_CheckStatuses(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_StartFinalSync(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_OnDropped(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptMemberDrop(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_EndAfterDropped(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 Wifi_PromptToggleVoiceChat_(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_ShowVoiceChatYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_HandleVoiceChatYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_PrintLeaderBusy(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_ShowApplyAgainYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_HandleApplyAgainYesNo(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_PrintContacting(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_ApplyAgain(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitReconnected(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_RebuildScreen(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitFadeIn(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
s32 WifiClubGameEntry_Member_Cancel(WifiClubGameEntry *gameEntry, WifiClubGameEntryArgs *args, enum HeapID heapID);
Field2DPoint WifiClubRoom_GetMapSize(s32 mapIndex);
const u32 *WifiClubRoom_GetMapCells(u32 mapIndex);

#endif // POKEHEARTGOLD_OVY_44_H
