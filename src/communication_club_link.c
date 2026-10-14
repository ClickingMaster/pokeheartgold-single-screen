#include "global.h"

#include "communication_club_link_internal.h"
#include "communication_player_state.h"
#include "script_manager.h"
#include "trainer_card_data.h"

// Declared with the types their call sites were matched against (no header, or a header with other types).
void FillScreenWithColor(int screen, u16 color);
int LinkSync_GetPlayerState(int index);
void CommState_FrameDummy(int, void *);

void CommClubLink_StartCardExchange(FieldSystem *fieldSystem);

void CommClubLink_InitDummy(void) {
}

void CommClubLink_Init(FieldSystem *fieldSystem) {
    if (gCommClubLink == NULL) {
        FieldComm_RegisterCommands(fieldSystem);
        gCommClubLink = Heap_Alloc((enum HeapID)15, sizeof(CommClubLink));
        MI_CpuFill8(gCommClubLink, 0, sizeof(CommClubLink));
        gCommClubLink->timer = 50;
        gCommClubLink->task = SysTask_CreateOnMainQueue(CommClubLink_Task, NULL, 10);
        gCommClubLink->fieldSystem = fieldSystem;
        gCommClubLink->party = NULL;
        CommClubLink_InitDummy();
        CommRandom_Init(gCommClubLink->random);
    }
}

void CommClubLink_Free(void) {
    int i;
    if (gCommClubLink) {
        SysTask_Destroy(gCommClubLink->task);
        for (i = 0; i < 4; i++) {
            if (gCommClubLink->cards[i]) {
                Heap_Free(gCommClubLink->cards[i]);
            }
        }
        if (gCommClubLink->party) {
            Heap_Free(gCommClubLink->party);
        }
        Heap_Free(gCommClubLink);
        gCommClubLink = NULL;
    }
}

void CommClubLink_StartParent(FieldSystem *fieldSystem, s32 linkMode, s32 beaconId) {
    if (Comm_IsActive() == 0) {
        CommState_StartParentSession(FieldSystem_GetSaveData(fieldSystem), linkMode, beaconId, fieldSystem->linkBattleRuleset, 0);
        CommClubLink_Init(fieldSystem);
        CommClubLink_SetState(CommClubLink_WaitParentConnected, 0);
    }
}

void CommClubLink_StartChild(FieldSystem *fieldSystem, u8 linkMode, u32 beaconId) {
    if (Comm_IsActive() == 0) {
        CommState_StartChildSession(FieldSystem_GetSaveData(fieldSystem), linkMode, beaconId, fieldSystem->linkBattleRuleset, 0);
        CommClubLink_Init(fieldSystem);
        CommClubLink_SetState(CommClubLink_WaitChildStarted, 0);
    }
}

void CommClubLink_ConnectToParent(u16 mode) {
    gCommClubLink->mode = mode;
    CommClubLink_SetState(CommClubLink_ConnectToSlot, 0);
}

void CommClubLink_RestartScan(void) {
    CommClubLink_SetState(CommClubLink_DoRestartScan, 0);
}

CommClubLink *gCommClubLink;

void CommClubLink_StartCardExchange(FieldSystem *fieldSystem) {
    int i;
    int netId;

    FillScreenWithColor(0, 0);
    FillScreenWithColor(1, 0);
    CommState_SetErrorWatch(1, 1);
    if (CommState_Exists()) {
        if (gCommClubLink == NULL) {
            CommClubLink_Init(fieldSystem);
            gCommClubLink->host = 1;
        } else {
            gCommClubLink->host = 0;
        }
        gCommClubLink->ready = 0;
        netId = Comm_GetLocalNetId();
        CommPlayerInfo_GetProfile(Comm_GetLocalNetId());
        for (i = 0; i < Comm_CountConnected(); i++) {
            if (gCommClubLink->cards[i] == NULL) {
                gCommClubLink->cards[i] = Heap_Alloc((enum HeapID)0, 0x66C);
            }
        }
        TrainerCardData_Build(0, 0, 0, 0xFF, gCommClubLink->fieldSystem, gCommClubLink->cards[netId]);
        LinkSync_StartSync(0x5F);
        CommClubLink_SetState(CommClubLink_SendCard, 0);
    }
}

BOOL CommClubLink_CardExchangeFinished(void) {
    if (gCommClubLink->state == CommClubLink_SendCard) {
        return FALSE;
    }
    if (gCommClubLink->state != CommClubLink_WaitCards) {
        return TRUE;
    }
    return FALSE;
}

void CommClubLink_Exit(void) {
    if (gCommClubLink) {
        CommState_SetErrorWatch(0, 0);
        CommClubLink_SetState(CommClubLink_DeletePlayers, 5);
    }
}

void CommClubLink_ExitSynced(void) {
    if (gCommClubLink) {
        LinkSync_StartSync(91);
        CommClubLink_SetState(CommClubLink_WaitExitSync, 5);
    }
}

void CommClubLink_Task(SysTask *task, void *unused) {
    if (gCommClubLink == NULL) {
        SysTask_Destroy(task);
        return;
    }
    if (gCommClubLink->state && gCommClubLink->paused == 0) {
        gCommClubLink->state();
    }
}

void CommClubLink_SetState(CommClubLinkState state, u16 timer) {
    gCommClubLink->state = state;
    gCommClubLink->timer = timer;
}

void CommClubLink_WaitParentConnected(void) {
    Comm_GetLocalNetId();
    if (((BOOL(*)(void))Comm_IsNetIdConnected)()) {
        CommClub_OnParentConnected();
        CommPlayerInfo_SendLocal();
        CommClubLink_SetState(CommClubLink_IdleParentConnected, 0);
    }
}

void CommClubLink_IdleParentConnected(void) {
}

void CommClubLink_WaitChildStarted(void) {
    if (CommWireless_IsStartedUp()) {
        CommClub_OnChildStarted();
        CommClubLink_SetState(CommClubLink_IdleChildStarted, 0);
    }
}

void CommClubLink_IdleChildStarted(void) {
}

void CommClubLink_ConnectToSlot(void) {
    CommState_ConnectToSlot(gCommClubLink->mode);
    CommClubLink_SetState(CommClubLink_WaitLinked, 0);
}

void CommClubLink_WaitLinked(void) {
    if (CommState_IsLinked()) {
        CommPlayerInfo_SendLocal();
        CommClubLink_SetState(CommClubLink_IdleLinked, 0);
    }
}

void CommClubLink_IdleLinked(void) {
}

void CommClubLink_HostInitRoom(void) {
    if (LinkSync_IsSynced(98)) {
        void *data = Heap_Alloc((enum HeapID)15, CommPlayerManager_sizeof());
        CommPlayerManager_Init(data, gCommClubLink->fieldSystem, 0);
        CommPlayerManager_ResumeFieldOnce();
        Comm_DisableKeySync();
        LinkSync_StartSync(92);
        CommClubLink_SetState(CommClubLink_HostShowRoom, 0);
        return;
    }
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    gCommClubLink->timer = 30;
    LinkSync_StartSync(98);
}

void CommClubLink_HostShowRoom(void) {
    if (Comm_GetLocalNetId() == 0) {
        CommPlayerInfo_HostRedistribute();
    }
    if (LinkSync_IsSynced(92)) {
        BeginNormalPaletteFade((enum FadeMode)0, (enum FadeType)1, (enum FadeType)1, 0, 6, 1, (enum HeapID)4);
        ResetVisibleHardwareWindows((PMLCDTarget)0);
        ResetVisibleHardwareWindows((PMLCDTarget)1);
        CommPlayerManager_ResyncLocalPlayer();
        CommPlayerManager_SendLocalPosition(0);
        CommClubLink_SetState(CommClubLink_HostPlacePlayers, 1);
    }
}

void CommClubLink_WaitPlayersPlaced(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    if (LinkSync_IsSynced(30)) {
        Comm_EnableKeySync();
        CommClubLink_SetState(CommClubLink_WaitLeaveSpot, 0);
    }
}

void CommClubLink_HostPlacePlayers(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    if (Comm_GetLocalNetId() == 0) {
        if (CommPlayerManager_StepAllPlayersBack() == 0) {
            return;
        }
        LinkSync_StartSync(30);
    } else {
        LinkSync_StartSync(30);
    }
    CommClubLink_SetState(CommClubLink_WaitPlayersPlaced, 20);
}

void CommClubLink_ChildInitRoom(void) {
    if (LinkSync_IsSynced(98)) {
        void *data = Heap_Alloc((enum HeapID)15, CommPlayerManager_sizeof());
        CommPlayerManager_Init(data, gCommClubLink->fieldSystem, 0);
        CommPlayerManager_ResumeFieldOnce();
        LinkSync_StartSync(92);
        CommClubLink_SetState(CommClubLink_ChildShowRoom, 0);
        return;
    }
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    gCommClubLink->timer = 30;
    LinkSync_StartSync(98);
}

void CommClubLink_ChildShowRoom(void) {
    if (Comm_GetLocalNetId() == 0) {
        CommPlayerInfo_HostRedistribute();
    }
    if (LinkSync_IsSynced(92)) {
        u8 flag;
        CommPlayerManager_ResyncLocalPlayer();
        CommPlayerManager_SendLocalPosition(0);
        flag = 1;
        Comm_SendFixedCommand(94, &flag);
        BeginNormalPaletteFade((enum FadeMode)0, (enum FadeType)1, (enum FadeType)1, 0, 6, 1, (enum HeapID)4);
        ResetVisibleHardwareWindows((PMLCDTarget)0);
        ResetVisibleHardwareWindows((PMLCDTarget)1);
        CommClubLink_SetState(CommClubLink_WaitOnSpot, 0);
    }
}

void CommClubLink_CheckPartnerState5E(void) {
    int i;

    for (i = 0; i < Comm_CountConnected(); i++) {
        if (i != Comm_GetLocalNetId() && LinkSync_GetPlayerState(i) == 0x5e && gCommClubLink->fieldSystem->taskman == NULL && gCommClubLink->fieldSystem->startMenuState == 0) {
            int j;
            for (j = 0; j < 4; j++) {
                if (gCommClubLink->cards[j] != NULL) {
                    Heap_Free(gCommClubLink->cards[j]);
                    gCommClubLink->cards[j] = NULL;
                }
            }
            StartMapSceneScript(gCommClubLink->fieldSystem, 0x238e, 0);
        }
    }
    CommState_FrameDummy(4, gCommClubLink->fieldSystem->bgConfig);
}

void CommClubLink_WaitLeaveSpot(void) {
    if (CommPlayerManager_AssignBattleSlots() == 0) {
        u8 flag;
        gCommClubLink->ready = 0;
        flag = 1;
        Comm_SendFixedCommand(94, &flag);
        CommClubLink_SetState(CommClubLink_WaitOnSpot, 0);
    }
    CommClubLink_CheckPartnerState5E();
}

void CommClubLink_OnEntryResult(int start, Party *party) {
    if (party) {
        gCommClubLink->party = SaveArray_Party_Alloc((enum HeapID)11);
        Party_Copy(party, gCommClubLink->party);
    }
    if (start) {
        CommClubLink_SetState(CommClubLink_LeaveRoomForBattle, 3);
        return;
    }
    {
        u8 flag = 3;
        Comm_SendFixedCommand(94, &flag);
        CommClubLink_SetState(CommClubLink_WaitLeaveSpot, 0);
    }
}

void CommClubLink_GoToWaitLeaveSpot(void) {
    CommClubLink_SetState(CommClubLink_WaitLeaveSpot, 0);
}

void CommClubLink_StartEntry(void) {
    if (Comm_IsSendingMoveKeys() == 0 && CommPlayerManager_GetMoveTimer(Comm_GetLocalNetId()) == 0) {
        if (gCommClubLink->timer) {
            gCommClubLink->timer--;
            return;
        }
        CommPlayerManager_PauseField();
        Colosseum_StartEntry(gCommClubLink->fieldSystem, CommClubLink_OnEntryResult);
        CommClubLink_SetState(CommClubLink_GoToWaitLeaveSpot, 0);
    }
}

void CommClubLink_WaitOnSpot(void) {
    if (gCommClubLink->ready) {
        u8 flag;
        CommClubLink_SetState(CommClubLink_StartEntry, 5);
        flag = 0;
        Comm_SendFixedCommand(94, &flag);
    }
    CommClubLink_CheckPartnerState5E();
}

void CommClubLink_RecvOnSpot(int sender, int unused, u8 *data) {
    if (*data == Comm_GetLocalNetId()) {
        gCommClubLink->ready = 1;
    }
}

BOOL CommClubLink_IsWalkingInRoom(void) {
    if (gCommClubLink && (gCommClubLink->state == CommClubLink_WaitOnSpot || gCommClubLink->state == CommClubLink_WaitLeaveSpot)) {
        return TRUE;
    }
    return FALSE;
}

void CommClubLink_StartBattle(void) {
    int battleType;
    u8 data[8];
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    battleType = 5;
    switch (CommState_GetLinkMode()) {
    case 4:
    case 5:
        battleType = 15;
        break;
    case 2:
    case 38:
        battleType = 7;
        break;
    }
    CommState_GetSelectedOrder(data);
    if (gCommClubLink->party == NULL) {
        FieldSystem_StartCommClubLinkBattle(gCommClubLink->fieldSystem, data, battleType);
    } else {
        FieldSystem_StartCommClubLinkBattleWithParty(gCommClubLink->fieldSystem, gCommClubLink->party, battleType);
        Heap_Free(gCommClubLink->party);
        gCommClubLink->party = NULL;
    }
    CommClubLink_Free();
}

void CommClubLink_WaitBattleTransferMode(void) {
    if (Comm_IsTransferMode1()) {
        if (gCommClubLink->timer) {
            gCommClubLink->timer--;
        }
        if (gCommClubLink->timer == 90) {
            LinkSync_StartSync(4);
        }
        if (LinkSync_IsSynced(4)) {
            CommClubLink_SetState(CommClubLink_StartBattle, 0);
        }
    }
}

void CommClubLink_RequestBattleTransferMode(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    Comm_RequestTransferMode1();
    CommClubLink_SetState(CommClubLink_WaitBattleTransferMode, 120);
}

void CommClubLink_WaitBattleSync(void) {
    if (LinkSync_IsSynced(3)) {
        CommClubLink_SetState(CommClubLink_RequestBattleTransferMode, 2);
    }
}

void CommClubLink_LeaveRoomForBattle(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    CommPlayerManager_Delete(0);
    LinkSync_StartSync(3);
    CommClubLink_SetState(CommClubLink_WaitBattleSync, 0);
}

void CommClubLink_DoRestartScan(void) {
    CommState_RestartScan();
    CommClubLink_SetState(CommClubLink_WaitScanIdle, 2);
}

void CommClubLink_WaitScanIdle(void) {
    if (CommWireless_IsIdle()) {
        CommClubLink_SetState(CommClubLink_WaitLinked, 10);
    }
}

void CommClubLink_SetCardReceived(int index) {
    gCommClubLink->received[index] = 1;
}

void *CommClubLink_GetCard(int index) {
    GF_ASSERT(index < 4);
    return gCommClubLink->cards[index];
}

void CommClubLink_SendCard(void) {
    int id = Comm_GetLocalNetId();
    if (LinkSync_IsSynced(95)) {
        Comm_SendCommandNoCopy(88, gCommClubLink->cards[id], 0x66c);
        CommClubLink_SetState(CommClubLink_WaitCards, 0);
    }
}

void CommClubLink_WaitCards(void) {
    int i;
    for (i = 0; i < Comm_CountConnected(); i++) {
        if (gCommClubLink->received[i] == 0) {
            return;
        }
    }
    LinkSync_StartSync(97);
    CommClubLink_SetState(CommClubLink_WaitCardsSync, 0);
}

void CommClubLink_WaitRoomTransferMode(void) {
    if (Comm_IsTransferMode1() == 0) {
        if (gCommClubLink->timer) {
            gCommClubLink->timer--;
        }
        if (gCommClubLink->timer == 90) {
            LinkSync_StartSync(5);
        }
        if (LinkSync_IsSynced(5)) {
            CommClubLink_SetState(CommClubLink_EnterRoom, 0);
        }
    }
}

void CommClubLink_RequestRoomTransferMode(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    Comm_RequestTransferMode0();
    CommClubLink_SetState(CommClubLink_WaitRoomTransferMode, 120);
}

void CommClubLink_WaitCardsSync(void) {
    if (LinkSync_IsSynced(97)) {
        CommClubLink_SetState(CommClubLink_RequestRoomTransferMode, 2);
    }
}

void CommClubLink_EnterRoom(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    LinkSync_StartSync(98);
    if (gCommClubLink->host) {
        CommClubLink_SetState(CommClubLink_HostInitRoom, 30);
        return;
    }
    CommClubLink_SetState(CommClubLink_ChildInitRoom, 30);
}

void CommClubLink_WaitExitSync(void) {
    if (LinkSync_IsSynced(91)) {
        CommState_SetErrorWatch(0, 0);
        CommPlayerManager_Delete(1);
        CommClubLink_SetState(CommClubLink_Shutdown, 5);
    }
}

void CommClubLink_DeletePlayers(void) {
    CommPlayerManager_Delete(1);
    CommClubLink_SetState(CommClubLink_Shutdown, 5);
}

void CommClubLink_Shutdown(void) {
    if (gCommClubLink->timer) {
        gCommClubLink->timer--;
        return;
    }
    CommState_RequestShutdown();
    CommClubLink_SetState(CommClubLink_Free, 0);
}

BOOL CommClubLink_IsStartingBattle(void) {
    if (gCommClubLink == NULL) {
        return FALSE;
    }
    if (gCommClubLink->state == CommClubLink_LeaveRoomForBattle || gCommClubLink->state == CommClubLink_WaitBattleSync || gCommClubLink->state == CommClubLink_RequestBattleTransferMode || gCommClubLink->state == CommClubLink_WaitBattleTransferMode || gCommClubLink->state == CommClubLink_StartBattle) {
        return TRUE;
    }
    return FALSE;
}
