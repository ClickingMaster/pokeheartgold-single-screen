#ifndef POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_INTERNAL_H
#define POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_INTERNAL_H
#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "party.h"
#include "sys_task.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "comm_state.h"
typedef void (*CommClubLinkState)(void);
typedef struct CommClubLink {
    void *cards[4];
    u8 received[4];
    FieldSystem *fieldSystem;
    u8 random[24];
    CommClubLinkState state;
    SysTask *task;
    u16 timer;
    u8 mode, pad3b, paused, pad3d, host, ready;
    Party *party;
} CommClubLink;
extern CommClubLink *gCommClubLink;
void CommPlayerInfo_SendLocal(void);
void CommClub_OnParentConnected(void);
void CommClub_OnChildStarted(void);
BOOL CommWireless_IsStartedUp(void);
void CommState_ConnectToSlot(u8);
BOOL CommState_IsLinked(void);
u32 CommPlayerManager_sizeof(void);
void CommPlayerManager_Init(void *, FieldSystem *, int);
void CommPlayerManager_ResumeFieldOnce(void);
void CommPlayerInfo_HostRedistribute(void);
void CommPlayerManager_ResyncLocalPlayer(void);
void CommPlayerManager_SendLocalPosition(int);
BOOL CommPlayerManager_StepAllPlayersBack(void);
BOOL CommPlayerManager_AssignBattleSlots(void);
void Comm_SendFixedCommand(int, void *);
BOOL CommPlayerManager_GetMoveTimer(int);
void CommPlayerManager_PauseField(void);
void Colosseum_StartEntry(FieldSystem *, void (*)(int, Party *));
BOOL sub_02036010(void);
void sub_02035FD8(void);
void sub_02035FE4(void);
void CommPlayerManager_Delete(int);
void CommState_RestartScan(void);
BOOL CommWireless_IsIdle(void);
void CommState_RequestShutdown(void);
BOOL Comm_SendCommandNoCopy(int, void *, int);
void CommClubLink_Free(void);
void CommClubLink_HostShowRoom(void);
void CommClubLink_ChildShowRoom(void);
void sub_0205857C(void);
void CommClubLink_StartBattle(void);
#include "encounter.h"
#include "screen_fade.h"
#include "unk_02091564.h"
void CommRandom_Init(void *);
void CommState_StartParentSession(SaveData *, s32, s32, LinkBattleRuleset *, int);
void CommState_StartChildSession(SaveData *, u8, u32, LinkBattleRuleset *, int);
void CommState_GetSelectedOrder(void *);
void sub_02058034(void);
void CommClubLink_Init(FieldSystem *fieldSystem);
void CommClubLink_Free(void);
void CommClubLink_StartParent(FieldSystem *fieldSystem, s32 arg1, s32 arg2);
void CommClubLink_StartChild(FieldSystem *fieldSystem, u8 arg1, u32 arg2);
void CommClubLink_ConnectToParent(u16 mode);
void CommClubLink_RestartScan(void);
BOOL CommClubLink_CardExchangeFinished(void);
void CommClubLink_Exit(void);
void CommClubLink_ExitSynced(void);
void CommClubLink_Task(SysTask *task, void *unused);
void CommClubLink_SetState(CommClubLinkState state, u16 timer);
void CommClubLink_WaitParentConnected(void);
void sub_02058328(void);
void CommClubLink_WaitChildStarted(void);
void sub_02058348(void);
void CommClubLink_ConnectToSlot(void);
void CommClubLink_WaitLinked(void);
void sub_02058388(void);
void CommClubLink_HostInitRoom(void);
void CommClubLink_HostShowRoom(void);
void CommClubLink_WaitPlayersPlaced(void);
void CommClubLink_HostPlacePlayers(void);
void CommClubLink_ChildInitRoom(void);
void CommClubLink_ChildShowRoom(void);
void CommClubLink_WaitLeaveSpot(void);
void CommClubLink_OnEntryResult(int start, Party *party);
void sub_02058690(void);
void CommClubLink_StartEntry(void);
void CommClubLink_WaitOnSpot(void);
void CommClubLink_RecvOnSpot(int sender, int unused, u8 *data);
BOOL CommClubLink_IsWalkingInRoom(void);
void CommClubLink_StartBattle(void);
void CommClubLink_WaitBattleTransferMode(void);
void CommClubLink_RequestBattleTransferMode(void);
void CommClubLink_WaitBattleSync(void);
void CommClubLink_LeaveRoomForBattle(void);
void CommClubLink_DoRestartScan(void);
void CommClubLink_WaitScanIdle(void);
void CommClubLink_SetCardReceived(int index);
void *CommClubLink_GetCard(int index);
void CommClubLink_SendCard(void);
void CommClubLink_WaitCards(void);
void CommClubLink_WaitRoomTransferMode(void);
void CommClubLink_RequestRoomTransferMode(void);
void CommClubLink_WaitCardsSync(void);
void CommClubLink_EnterRoom(void);
void CommClubLink_WaitExitSync(void);
void CommClubLink_DeletePlayers(void);
void CommClubLink_Shutdown(void);
BOOL CommClubLink_IsStartingBattle(void);

void sub_02058034(void);
void CommClubLink_Init(FieldSystem *fieldSystem);
void CommClubLink_Free(void);
void CommClubLink_StartParent(FieldSystem *fieldSystem, s32 arg1, s32 arg2);
void CommClubLink_StartChild(FieldSystem *fieldSystem, u8 arg1, u32 arg2);
void CommClubLink_ConnectToParent(u16 mode);
void CommClubLink_RestartScan(void);
BOOL CommClubLink_CardExchangeFinished(void);
void CommClubLink_Exit(void);
void CommClubLink_ExitSynced(void);
void CommClubLink_Task(SysTask *task, void *unused);
void CommClubLink_SetState(CommClubLinkState state, u16 timer);
void CommClubLink_WaitParentConnected(void);
void sub_02058328(void);
void CommClubLink_WaitChildStarted(void);
void sub_02058348(void);
void CommClubLink_ConnectToSlot(void);
void CommClubLink_WaitLinked(void);
void sub_02058388(void);
void CommClubLink_HostInitRoom(void);
void CommClubLink_HostShowRoom(void);
void CommClubLink_WaitPlayersPlaced(void);
void CommClubLink_HostPlacePlayers(void);
void CommClubLink_ChildInitRoom(void);
void CommClubLink_ChildShowRoom(void);
void CommClubLink_WaitLeaveSpot(void);
void CommClubLink_OnEntryResult(int start, Party *party);
void sub_02058690(void);
void CommClubLink_StartEntry(void);
void CommClubLink_WaitOnSpot(void);
void CommClubLink_RecvOnSpot(int sender, int unused, u8 *data);
BOOL CommClubLink_IsWalkingInRoom(void);
void CommClubLink_StartBattle(void);
void CommClubLink_WaitBattleTransferMode(void);
void CommClubLink_RequestBattleTransferMode(void);
void CommClubLink_WaitBattleSync(void);
void CommClubLink_LeaveRoomForBattle(void);
void CommClubLink_DoRestartScan(void);
void CommClubLink_WaitScanIdle(void);
void CommClubLink_SetCardReceived(int index);
void *CommClubLink_GetCard(int index);
void CommClubLink_SendCard(void);
void CommClubLink_WaitCards(void);
void CommClubLink_WaitRoomTransferMode(void);
void CommClubLink_RequestRoomTransferMode(void);
void CommClubLink_WaitCardsSync(void);
void CommClubLink_EnterRoom(void);
void CommClubLink_WaitExitSync(void);
void CommClubLink_DeletePlayers(void);
void CommClubLink_Shutdown(void);
BOOL CommClubLink_IsStartingBattle(void);

#endif
