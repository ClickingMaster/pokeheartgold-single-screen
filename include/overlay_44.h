#ifndef POKEHEARTGOLD_OVY_44_H
#define POKEHEARTGOLD_OVY_44_H

#include "list_menu_2d.h"
#include "overlay_44_02232E9C.h"
#include "render_window.h"
#include "unk_02009D48.h"

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
s32 WifiClubGameEntry_Main(OverlayManager *overlayManager, u32 *arg1);
s32 WifiClubGameEntry_Exit(OverlayManager *arg0);
void WifiClubGameEntry_OnJoinAnswer(WifiClubGameEntry *arg0, String *arg1);
void WifiClubGameEntry_OnStartCommand(WifiClubGameEntry *arg0);
void WifiClubGameEntry_OnVoiceChatFlags(WifiClubGameEntry *arg0, String *arg1);

// The following functions are likely static
s32 WifiClubGameEntry_Member_WaitFinalSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
void WifiClubGameEntry_VBlankCB(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_IsLocalLeader(WifiClubGameEntry *arg0);
void WifiClubGameEntry_InitScreen(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
void WifiClubGameEntry_FreeScreen(WifiClubGameEntry *arg0, enum HeapID heapID);
void WifiClubGameEntry_SetGfxBanks(void);
void InitBgAndScrnData(WifiClubGameEntryScreen *arg0, enum HeapID heapID);
void FreeBgAndScreenData(WifiClubGameEntryScreen *arg0);
void InitWindowList(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
void FreeWindowList(WifiClubGameEntryScreen *arg0, enum HeapID heapID);
void InitStringAndMsgData(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
void FreeStringAndMsgData(WifiClubGameEntryScreen *arg0);
void WifiClubGameEntry_PrintTitle(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
void InitSpritesAnd2DGfxResObjs(WifiClubGameEntryScreen *arg0, enum HeapID heapID);
void FreeSpritesAnd2DGfxResObjs(WifiClubGameEntryScreen *arg0);
void Wifi_LoadAndPrintTextToWindowWrapper(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 strno, enum HeapID heapID);
void Wifi_LoadAndPrintTextToWindow(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 strno, s32 arg3, enum HeapID heapID);
s32 WifiClubGameEntry_IsMessageFinished(WifiClubGameEntryScreen *arg0);
void WifiClubGameEntry_ClearMessage(WifiClubGameEntryScreen *arg0);
void WifiClubGameEntry_ShowApplicantInfo(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 arg2, enum HeapID heapID);
void WifiClubGameEntry_HideApplicantInfo(WifiClubGameEntryScreen *arg0);
void WifiClubGameEntry_DrawMemberList(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
void WifiClubGameEntry_DrawMember(WifiClubGameEntry *arg0, WifiClubGameEntryScreen *arg1, WifiClubGameEntryArgs *arg2, s32 arg3, enum HeapID heapID);
void WifiClubGameEntry_ClearMember(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 arg2, enum HeapID heapID);
void WifiClubGameEntry_BufferPlayerName(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 arg2, s32 arg3, enum HeapID heapID);
void WifiClubGameEntry_BufferTrainerId(WifiClubGameEntryScreen *arg0, WifiClubGameEntryArgs *arg1, s32 arg2, s32 arg3, enum HeapID heapID);
void WifiClubGameEntry_AnimateBgPalette(WifiClubGameEntryScreen *arg0);
void WifiClubGameEntry_LoadBgPaletteFrame(WifiClubGameEntryScreen *arg0, s32 arg1);
u8 WifiClubGameEntry_ToggleVoiceChat(WifiClubGameEntry *arg0, enum HeapID heapID);
void WifiClubGameEntry_CheckConnectionLost(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
void WifiClubGameEntry_CheckMatchFailed(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_IsAnyoneConnected(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_StartMatch(u32 arg0, u32 arg1);
void WifiClubGameEntry_SetPlayingStatus(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1);
s32 WifiClubGameEntry_StartVoiceChat(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_StopVoiceChat(WifiClubGameEntry *arg0);
void WifiClubGameEntry_EnableVoiceChat(WifiClubGameEntry *arg0);
void WifiClubGameEntry_DisableVoiceChat(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_CountMembers(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_AcceptEarlierMembers(WifiClubGameEntry *arg0, enum HeapID heapID);
void WifiClubGameEntry_RefusePendingApplicants(WifiClubGameEntry *arg0);
void WifiClubGameEntry_ReleaseAllMembers(WifiClubGameEntry *arg0);
void WifiClubGameEntry_SetDisconnectCallback(WifiClubGameEntry *arg0);
void WifiClubGameEntry_ClearDisconnectCallback(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_CheckMemberDisconnected(WifiClubGameEntry *arg0);
void WifiClubGameEntry_OnDisconnect(u8 arg0, WifiClubGameEntry *arg1, s32 arg2);
s32 WifiClubGameEntry_CheckMemberListChanged(WifiClubGameEntry *arg0);
void WifiClubGameEntry_RequestMemberListRedraw(WifiClubGameEntry *arg0);
void WifiClubGameEntry_AcceptMember(WifiClubGameEntry *arg0, s32 arg1, enum HeapID heapID);
void WifiClubGameEntry_AddMemberAvatar(WifiClubGameEntry *arg0, s32 arg1, enum HeapID heapID);
void WifiClubGameEntry_UpdateAvatars(WifiClubGameEntry *arg0);
void WifiClubGameEntry_DeleteAvatars(WifiClubGameEntry *arg0);
void WifiClubGameEntry_SendLeaderBusy(WifiClubGameEntry *arg0, s32 arg1);
void WifiClubGameEntry_RefuseLateApplicants(WifiClubGameEntry *arg0);
void WifiClubGameEntry_AcceptAnnouncedMembers(WifiClubGameEntry *arg0, enum HeapID heapID);
void WifiClubGameEntry_ShowWaitingIcon(WifiClubGameEntry *arg0);
void WifiClubGameEntry_HideWaitingIcon(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
void WifiClubGameEntry_AvatarNone(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1);
void WifiClubGameEntry_AvatarStartSpin(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1);
void WifiClubGameEntry_AvatarUpdateSpin(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1);
void WifiClubGameEntry_AvatarIdle(WifiClubGameEntry *arg0, WifiClubGameEntryAvatar *arg1);
void WifiClubGameEntry_SendVoiceChatFlags(WifiClubGameEntry *arg0);
int WifiClubGameEntry_IsStartSynced(WifiClubGameEntry *arg0);
void WifiClubGameEntry_StartTimeout(WifiClubGameEntry *arg0);
void WifiClubGameEntry_StopTimeout(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_HasTimedOut(WifiClubGameEntry *arg0);
s32 WifiClubGameEntry_Leader_Start(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptReadyMessage(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleInput(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptUserJoinRequest(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowAcceptYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleAcceptYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptInsufficientMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_WaitNotEnoughMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptConfirmMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowConfirmYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleConfirmYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_StartCommunicating(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_WaitMembersConnected(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_SendStartCommand(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_WaitStartSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_SendStatus(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_CheckStatuses(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_StartFinalSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_WaitFinalSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptStopFindingMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowStopSeekingYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleStopSeekingYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptDropAsLeader(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowDisbandYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleDisbandYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptCancelInvites(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_Cancel(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_OnMemberDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptMemberDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_EndAfterMemberDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptToggleVoiceChat(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_ShowVoiceChatYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Leader_HandleVoiceChatYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_Start(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitLeaderInfo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptAwaitingResponse(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitJoinAnswer(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_PrintApproved(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitApproved(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_PrintRefused(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_EndRefused(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptAwaitingMembers(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitAwaitingMessage(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitStart(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_SendStatus(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_CheckStatuses(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_StartFinalSync(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_OnDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptMemberDrop(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_EndAfterDropped(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 Wifi_PromptToggleVoiceChat_(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_ShowVoiceChatYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_HandleVoiceChatYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_PrintLeaderBusy(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_ShowApplyAgainYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_HandleApplyAgainYesNo(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_PrintContacting(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_ApplyAgain(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitReconnected(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_RebuildScreen(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_WaitFadeIn(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
s32 WifiClubGameEntry_Member_Cancel(WifiClubGameEntry *arg0, WifiClubGameEntryArgs *arg1, enum HeapID heapID);
Field2DPoint WifiClubRoom_GetMapSize(s32 arg0);
const u32 *WifiClubRoom_GetMapCells(u32 arg0);

#endif // POKEHEARTGOLD_OVY_44_H
