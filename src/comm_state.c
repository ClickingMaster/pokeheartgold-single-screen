// clang-format off
#include "comm_state_private.h"
// clang-format on

#include "communication_error.h"
#include "dwcaccount.h"
#include "overlay_00_thumb.h"
#include "overlay_13_arm_2.h"
/* overlay 45 prototypes in private header */
#include "system.h"
#include "sound_playback.h"
#include "wireless_manager.h"
#include "communication_command.h"
#include "communication_player_state.h"
#include "comm_wireless.h"
#include "link_synchronization.h"
#include "wireless_status_icon.h"
#include "network_overlay_loader.h"

typedef struct CommStateWork {
    WifiClubStatusData *wifiClubData;
    u8 selectedOrder[8];
    MATHRandContext32 mathRandContext;
    void (*stateFunc)(void);
    SaveData *saveData;
    PlayerProfile *profile;
    s32 rules;
    u32 wifiError;
    s32 wifiErrorArg1;
    s32 wifiErrorArg2;
    s32 shutdownTimer;
    s32 timer;
    u16 joinRetries;
    u8 maxEntries;
    u8 memberCount;
    u8 joinReply;
    u8 slot;
    u8 linkMode;
    u8 beaconId;
    u8 waitHostDisconnect;
    s8 matchFriendIndex;
    u8 renewTgid;
    u8 watchMembers;
    u8 reportErrors;
    u8 unk55;
    u8 errorScreen;
    u8 refuseJoins;
    u8 seenGames;
    u8 unk59[3];
    u8 peerDisconnected;
    u8 errorKind;
    u8 unk5E;
    u8 unkEF;
    s32 plazaProfileId;
    u8 plazaLobbyStarted;
} CommStateWork;

CommStateWork *gCommState = NULL;

void (*const gCommLinkedStates[3])(void) = { CommState_ParentLinked, CommState_ChildLinked, NULL };

static u8 sJoinAccepted[] = " GAME";
static u8 sJoinRequest[] = "FREAK";
static u8 sJoinRefused[] = " FULL";

s32 CommState_GetSyncPacketSize(void) {
    return 70;
}

void CommState_Init(SaveData *saveData, s8 linkMode) {
    if (gCommState == NULL) {
        GF_ASSERT(saveData);
        CommWireless_StartUp();
        gCommState = Heap_Alloc(HEAP_ID_15, sizeof(CommStateWork));
        ;
        MI_CpuClear8(gCommState, sizeof(CommStateWork));
        gCommState->timer = 50;
        gCommState->renewTgid = 1;
        gCommState->saveData = saveData;
        gCommState->profile = Save_PlayerData_GetProfile(saveData);
        gCommState->maxEntries = 2;
        gCommState->joinReply = 0;
        gCommState->refuseJoins = 0;
        gCommState->linkMode = linkMode;
        CommRandom_Init(&gCommState->mathRandContext);
        Comm_RegisterCommandTable(0, 0, 0);
        if ((linkMode != 9) && (linkMode != 17) && (linkMode != 15)) {
            WirelessStatusIcon_Create();
        }
    }
}

void CommState_Free(void) {
    if (gCommState) {
        Comm_FreeCommandTable();
        if (gCommState->wifiClubData) {
            Heap_Free(gCommState->wifiClubData);
        }
        if (CommState_IsWifi()) {
            Heap_Destroy(HEAP_ID_48);
        }
        WirelessStatusIcon_Destroy();
        CommWireless_Terminate();
        Heap_Free(gCommState);
        Heap_Destroy(HEAP_ID_15);
        gCommState = NULL;
    }
}

BOOL CommState_Exists(void) {
    return gCommState ? TRUE : FALSE;
}

void CommState_StartParentSession(SaveData *saveData, s8 linkMode, s8 beaconId, s32 arg3) {
    if (!Comm_IsActive()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, linkMode);
        gCommState->beaconId = beaconId;
        gCommState->rules = arg3;
        CommState_SetState(CommState_ParentStart, 0);
    }
}

void CommState_StartChildSession(SaveData *saveData, s8 linkMode, s8 beaconId, s32 arg3) {
    if (!Comm_IsActive()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, linkMode);
        gCommState->beaconId = beaconId;
        gCommState->rules = arg3;
        CommState_SetState(CommState_ChildStartScan, 0);
    }
}

void CommState_ConnectToSlot(s8 slot) {
    gCommState->slot = slot;
    CommState_SetState(CommState_ChildConnecting, 0);
}

void CommState_RestartScan(void) {
    Comm_ResetWorkAndParents();
    CommState_SetState(CommState_RestartScanStop, 0);
}

void CommState_ShutdownWait(void) {
    if (gCommState->shutdownTimer > 0) {
        gCommState->shutdownTimer--;
    } else if (!gCommState->shutdownTimer) {
        CommState_ShowRestartErrorAndHalt();
    }
    if (gCommState->timer) {
        gCommState->timer--;
    }
    if (CommWireless_Stop() && !gCommState->timer) {
        Comm_Shutdown();
        CommState_SetState(CommState_FreeWhenDone, 0);
    }
}

void CommState_RequestShutdown(void) {
    if (gCommState) {
        gCommState->shutdownTimer = 1800;
        CommState_SetState(CommState_ShutdownWait, 5);
    }
}

void CommState_SetEntry(u32 enabled) {
    Wireless_SetEntry(enabled);
}

s32 CommState_IsLinked(void) {
    void (*const sp0[3])(void);
    ARRAY_ASSIGN(sp0, gCommLinkedStates);
    void (*func)(void) = gCommState->stateFunc;
    if (!gCommState) {
        return 0;
    }
    for (int i = 0; sp0[i] != NULL; i++) {
        if (func == sp0[i]) {
            return 1;
        }
    }
    return 0;
}

void CommState_StartAlternateSearch(SaveData *saveData) {
    if (gCommState == NULL) {
        if (!Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800)) {
            CommState_ShowRestartErrorAndHalt();
        }
        CommState_Init(saveData, 9);
        gCommState->linkMode = 9;
        gCommState->beaconId = 0;
        CommState_SetState(CommState_AlternateInit, 0);
    }
}

void CommState_JoinParent(s8 slot) {
    gCommState->slot = slot;
    gCommState->joinRetries = 3;
    Wireless_SetEntryLocked(1);
    WirelessStatusIcon_Create();
    CommState_SetState(CommState_JoinWaitStopped, 0);
}

s32 CommState_GetJoinResult(void) {
    if (gCommState == NULL) {
        return -1;
    }
    if (gCommState->stateFunc == CommState_Joined) {
        return 1;
    }
    if (gCommState->stateFunc == CommState_JoinFailed) {
        return -1;
    } else {
        return 0;
    }
}

s32 CommState_IsAlternateLinked(void) {
    if (gCommState == NULL) {
        return 0;
    }
    if (gCommState->stateFunc == CommState_AlternateLinked) {
        return 1;
    }
    return 0;
}

void CommState_ResumeAlternateSearch(void) {
    Wireless_SetEntryLocked(0);
    if ((gCommState->watchMembers != 1) || (gCommState->reportErrors != 1) || (gCommState->linkMode == 9)) {
        CommState_SetErrorWatch(0, 0);
    }
    WirelessStatusIcon_Destroy();
    gCommState->linkMode = 9;
    gCommState->refuseJoins = 0;
    Wireless_SetMaxEntries(gCommState->maxEntries);
    if (!Comm_GetLocalNetId()) {
        Wireless_SetEntryClosed(1);
        CommState_SetState(CommState_ResumeParentWaitAlone, 15);
    } else {
        CommState_SetState(CommState_ResumeChildStop, 5);
    }
}

s32 CommState_IsAlternateSearchReady(void) {
    if (gCommState == NULL) {
        return 1;
    } else if (gCommState->stateFunc != CommState_AlternateWaitStopped) {
        return 1;
    }
    return 0;
}

void CommState_StopAlternateSearch(void) {
    if (gCommState) {
        CommState_SetState(CommState_ShutdownWaitStopped, 5);
    }
}

void CommState_RefuseJoins(void) {
    if (gCommState) {
        gCommState->refuseJoins = 1;
        Wireless_SetEntryLocked(1);
    }
}

void CommState_RefuseJoinsAndRestart(void) {
    if (gCommState) {
        gCommState->refuseJoins = 1;
        Wireless_SetEntryLocked(1);
        CommState_SetState(CommState_RestartIdle, 0);
    }
}

void CommState_SetUnionAppLinkMode(void) {
    gCommState->linkMode = 18;
}

void CommState_SetUnionRoomLinkMode(void) {
    gCommState->linkMode = 9;
}

void CommState_StartMysteryGiftParent(SaveData *saveData, s8 linkMode) {
    if (!Comm_IsActive()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, linkMode);
        CommState_SetState(CommState_MysteryGiftParentStart, 0);
    }
}

void CommState_StartMysteryGiftChild(SaveData *saveData, s8 linkMode) {
    if (!Comm_IsActive()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, linkMode);
        CommState_SetState(CommState_MysteryGiftChildStart, 0);
    }
}

void CommState_Main(void) {
    if (gCommState && gCommState->stateFunc) {
        gCommState->stateFunc();
    }
    if (CommState_IsWifi()) {
        WirelessStatusIcon_SetLevel(3 - DWC_GetLinkLevel());
    } else if (CommWireless_Exists()) {
        WirelessStatusIcon_SetLevel(3 - WM_GetLinkLevel());
    }
}

void CommState_SetState(void (*stateFunc)(), int timer) {
    gCommState->stateFunc = stateFunc;
    gCommState->timer = timer;
}

void CommState_EndWireless(void) {
    if (!CommState_IsWifi()) {
        if (!CommWireless_IsIdle()) {
            return;
        } else {
            Comm_EnableKeySync();
            CommWireless_SetManagerEnded(1);
        }
    }
    CommState_SetState(CommState_Idle, 0);
}

void CommState_StopWireless(void) {
    if (CommWireless_Stop()) {
        Comm_ResetWork();
        CommState_SetState(CommState_EndWireless, 0);
    }
}

void CommState_ParentStart(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        CommPlayerInfo_Init(gCommState->saveData, gCommState->rules);
        if (Comm_StartParent(1, 1, 512, 1)) {
            sub_02035FD8();
            CommState_SetState(CommState_WaitLinked, 0);
        }
    }
}

void CommState_WaitLinked(void) {
    if (Comm_IsNetIdConnected(Comm_GetLocalNetId()) != FALSE) {
        CommState_SetState(CommState_ParentLinked, 0);
    }
}

void CommState_ParentLinked(void) {
    if (!Comm_IsActive()) {
        CommState_SetState(CommState_FreeWhenDone, 0);
    }
}

void CommState_ChildStartScan(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        CommPlayerInfo_Init(gCommState->saveData, gCommState->rules);
        if (Comm_StartScan(1, 1, 512)) {
            sub_02035FD8();
            CommState_SetState(CommState_ChildScanning, 0);
        }
    }
}

void CommState_ChildScanning(void) {
    CommWireless_UpdateParents();
}

void CommState_ChildConnecting(void) {
    CommWireless_UpdateParents();
    if (Comm_ConnectToParent(gCommState->slot)) {
        CommState_SetState(CommState_ChildWaitLinked, 10);
    }
}

void CommState_ChildWaitLinked(void) {
    if (Comm_HasLinkError()) {
        CommState_SetState(CommState_ChildRescanStop, 0);
    }
    if (Comm_IsNetIdConnected(Comm_GetLocalNetId()) && Comm_GetLocalNetId()) {
        CommState_SetState(CommState_ChildLinked, 0);
    }
}

void CommState_ChildRescanStop(void) {
    CommWireless_Stop();
    CommState_SetState(CommState_ChildRescan, 2);
}

void CommState_ChildRescan(void) {
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    if (CommWireless_IsIdle() && Comm_StartScan(0, 1, 512)) {
        sub_02035FD8();
        CommState_SetState(CommState_ChildConnecting, 10);
    }
}

void CommState_ChildLinked(void) {
    if (!Comm_IsActive()) {
        CommState_SetState(CommState_FreeWhenDone, 0);
    }
}

void CommState_RestartScanStop(void) {
    CommWireless_Stop();
    CommState_SetState(CommState_RestartScanStart, 2);
}

void CommState_RestartScanStart(void) {
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    if (CommWireless_IsIdle() && Comm_StartScan(0, 1, 512)) {
        sub_02035FD8();
        CommState_SetState(CommState_ChildScanning, 10);
    }
}

void CommState_Idle(void) {
}

void CommState_FreeWhenDone(void) {
    if (!Comm_IsActive()) {
        CommState_Free();
    }
}

void CommState_ResumeParentWaitAlone(void) {
    if (Comm_CountConnected() <= 1) {
        Wireless_SetEntryClosed(0);
        sub_0203624C();
        CommState_SetState(CommState_AlternateWaitStopped, 0);
    }
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    Wireless_SetEntryClosed(0);
    sub_0203624C();
    CommState_SetState(CommState_AlternateWaitStopped, 0);
}

void CommState_ResumeChildStop(void) {
    if (CommWireless_Stop()) {
        Comm_ResetWork();
        CommState_SetState(CommState_AlternateWaitStopped, 0);
    }
}

void CommState_ShutdownWaitStopped(void) {
    if (gCommState->timer) {
        gCommState->timer--;
    }
    if (CommWireless_Stop() && !gCommState->timer) {
        Comm_Shutdown();
        CommState_SetState(CommState_FreeWhenDone, 0);
    }
}

void CommState_AlternateInit(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        Wireless_SetMaxEntries(gCommState->maxEntries);
        CommPlayerInfo_Init(gCommState->saveData, 0);
        if (Comm_StartScan(1, 1, 512)) {
            sub_02035FD8();
            CommState_SetState(CommState_AlternateScan, 64);
        }
    }
}

void CommState_AlternateScan(void) {
    CommWireless_UpdateParents();
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    if (CommWireless_Stop()) {
        CommState_SetState(CommState_AlternateStartParent, 0);
    }
}

void CommState_AlternateWaitStopped(void) {
    if (CommWireless_Stop()) {
        CommState_SetState(CommState_AlternateStartParent, 0);
    }
}

void CommState_AlternateStartParent(void) {
    if (CommWireless_IsIdle() && Comm_StartParent(0, gCommState->renewTgid, 512, 1)) {
        // Equivalent to MATH_Rand32(&gCommState->mathRandContext, 0);
        MATHRandContext32 *mathRandContext = &gCommState->mathRandContext;
        mathRandContext->x = (mathRandContext->mul * mathRandContext->x) + mathRandContext->add;
        //
        sub_02035FD8();
        gCommState->renewTgid = 0;
        CommState_SetState(&CommState_AlternateParentWait, 10000);
    }
}

void CommState_AlternateParentWait(void) {
    if (!CommWireless_HasSentBeacons()) {
        if (Comm_GetConnectedChildren()) {
            gCommState->renewTgid = 1;
            WirelessStatusIcon_Create();
            CommState_SetState(CommState_AlternateLinked, 0);
            return;
        }
        if (gCommState->timer) {
            gCommState->timer--;
            return;
        }
    }
    if (CommWireless_Stop()) {
        CommState_SetState(CommState_AlternateStartScan, 0);
    }
}

void CommState_AlternateStartScan(void) {
    if (CommWireless_IsIdle() && Comm_StartScan(0, 0, 512)) {
        sub_02035FD8();
        // Equivalent to u32 rand = MATH_Rand32(&gCommState->mathRandContext, 32)
        MATHRandContext32 *mathRandContext = &gCommState->mathRandContext;
        u64 r0 = (mathRandContext->mul * mathRandContext->x) + mathRandContext->add;
        mathRandContext->x = r0;
        u32 rand = (u32)(((mathRandContext->x >> 32) * 32) >> 32);
        //
        CommState_SetState(CommState_AlternateScan, rand);
    }
}

void CommState_JoinWaitStopped(void) {
    if (CommWireless_Stop()) {
        CommState_SetState(CommState_JoinStartScan, 0);
    }
}

void CommState_JoinStartScan(void) {
    if (CommWireless_IsIdle() && Comm_StartScan(0, 0, 512)) {
        sub_02035FD8();
        CommState_SetState(CommState_JoinConnect, 100);
    }
}

void CommState_JoinConnect(void) {
    if (CommWireless_GetParentMemberCount(gCommState->slot) && Comm_ConnectToParent(gCommState->slot)) {
        CommState_SetState(CommState_JoinWaitLinked, 100);
        return;
    }
    if (Comm_HasLinkError()) {
        CommState_SetState(CommState_JoinRetry, 0);
        return;
    }
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    CommState_SetState(CommState_JoinRetry, 0);
}

void CommState_JoinWaitLinked(void) {
    if (Comm_HasLinkError()) {
        CommState_SetState(CommState_JoinRetry, 0);
        return;
    }
    if (Comm_IsNetIdConnected(Comm_GetLocalNetId()) != FALSE) {
        gCommState->joinReply = 0;
        CommState_SetState(CommState_JoinWaitReply, 120);
        return;
    }
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    CommState_SetState(CommState_JoinRetry, 0);
}

void CommState_JoinWaitReply(void) {
    if (Comm_HasLinkError()) {
        CommState_SetState(CommState_JoinRetry, 0);
        return;
    }
    if (gCommState->joinReply == 2) {
        CommState_SetState(CommState_JoinFailed, 0);
        return;
    }
    if (gCommState->joinReply == 1) {
        CommPlayerInfo_SendLocal();
        CommState_SetState(CommState_Joined, 0);
        return;
    }
    if (gCommState->timer > 110) {
        Comm_SendFixedCommand(6, sJoinRequest);
    }
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    CommState_SetState(CommState_JoinRetry, 0);
}

void CommState_Joined(void) {
    if (Comm_HasLinkError()) {
        CommState_SetState(CommState_JoinRetry, 0);
    }
}

void CommState_JoinFailed(void) {
}

void CommState_JoinRetry(void) {
    gCommState->joinReply = 0;
    if (CommWireless_Stop()) {
        if (gCommState->joinRetries) {
            gCommState->joinRetries--;
            CommState_SetState(CommState_JoinStartScan, 0);
            return;
        }
        CommState_SetState(CommState_JoinFailed, 0);
    }
}

void CommState_AlternateLinked(void) {
    if (!Comm_GetConnectedChildren()) {
        if (!CommState_ReportsErrors() && CommWireless_Stop()) {
            CommState_SetState(CommState_AlternateStartScan, 0);
        }
    } else if (!Comm_GetLocalNetId()) {
        CommPlayerInfo_HostRedistribute();
    }
    if (Comm_HasLinkError() && !CommState_ReportsErrors()) {
        CommState_SetState(CommState_JoinRetry, 0);
    }
}

void CommState_RestartIdle(void) {
    if (CommWireless_IsIdle() && Comm_StartParent(0, gCommState->renewTgid, 512, 0)) {
        sub_02035FD8();
        CommState_SetState(CommState_Idle, 0);
    }
}

void sub_020388E0(void) {
    u8 sp[2];
    gCommState->linkMode = 13;
    if (!Comm_GetLocalNetId()) {
        sp[1] = 0;
        Comm_SendFixedCommand(10, &sp[1]);
        return;
    }
    sp[0] = 0;
    Comm_SendFixedCommand(10, sp);
}

void CommState_ReconnectToParent(s8 slot) {
    gCommState->slot = slot;
    gCommState->joinRetries = 3;
    CommState_SetState(CommState_ReconnectStop, 0);
}

void CommState_SetRecordMixingLinkMode(void) {
    gCommState->linkMode = 7;
}

void CommState_JoinRecordMixing(s8 slot) {
    gCommState->slot = slot;
    gCommState->linkMode = 7;
    gCommState->joinRetries = 3;
    WirelessStatusIcon_Create();
    CommState_SetState(CommState_JoinWaitStopped, 0);
}

void CommState_SetSpinTradeLinkMode(void) {
    gCommState->linkMode = 26;
}

void CommState_JoinSpinTrade(s8 slot) {
    gCommState->slot = slot;
    gCommState->linkMode = 26;
    gCommState->joinRetries = 3;
    WirelessStatusIcon_Create();
    CommState_SetState(CommState_JoinWaitStopped, 0);
}

void CommState_ReconnectStop(void) {
    if (CommWireless_Stop()) {
        CommState_SetState(CommState_ReconnectScan, 0);
    }
}

void CommState_ReconnectScan(void) {
    if (CommWireless_IsIdle()) {
        gCommState->linkMode = 13;
        if (Comm_StartScan(0, 0, 512)) {
            sub_02035FE4();
            CommState_SetState(CommState_ReconnectWaitParent, 100);
        }
    }
}

void CommState_ReconnectWaitParent(void) {
    if (CommWireless_GetParentMemberCount(gCommState->slot) && Comm_ConnectToParent(gCommState->slot)) {
        CommState_SetState(CommState_ReconnectWaitLinked, 100);
        return;
    }
    if (Comm_HasLinkError() || !gCommState->timer) {
        gCommState->joinRetries--;
        if (!gCommState->joinRetries) {
            CommState_SetState(CommState_JoinFailed, 0);
            return;
        }
        CommState_SetState(CommState_ReconnectStop, 0);
        return;
    }
    if (gCommState->timer) {
        gCommState->timer--;
    }
}

void CommState_ReconnectWaitLinked(void) {
    if (gCommState->timer > 90) {
        gCommState->timer--;
        return;
    }
    if (Comm_HasLinkError()) {
        gCommState->joinRetries--;
        if (!gCommState->joinRetries) {
            CommState_SetState(CommState_JoinFailed, 0);
            return;
        }
        CommState_SetState(CommState_ReconnectStop, 0);
        return;
    }
    if (Comm_IsNetIdConnected(Comm_GetLocalNetId()) != FALSE) {
        CommState_SetState(CommState_Joined, 0);
        return;
    }
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    gCommState->joinRetries--;
    if (!gCommState->joinRetries) {
        CommState_SetState(CommState_JoinFailed, 0);
        return;
    }
    CommState_SetState(CommState_ReconnectStop, 0);
}

void CommState_RecvJoinRequest(u8 netId, u8 size, u8 *data) {
    u32 i;
    if (!Comm_GetLocalNetId()) {
        BOOL var_r3 = TRUE;
        for (i = 0; i < 6; i++) {
            if (sJoinRequest[i] != data[i]) {
                var_r3 = FALSE;
                break;
            }
        }
        if (var_r3 && !gCommState->refuseJoins) {
            sJoinAccepted[0] = netId;
            Comm_SendFixedParentCommand(7, sJoinAccepted);
        } else {
            sJoinRefused[0] = netId;
            Comm_SendFixedParentCommand(7, sJoinRefused);
        }
    }
}

void CommState_RecvJoinReply(u32 netId, u32 size, u8 *data) {
    u32 i;
    BOOL var_r4 = TRUE;
    for (i = 1; i < 6; i++) {
        if (sJoinAccepted[i] != data[i]) {
            var_r4 = FALSE;
            break;
        }
    }
    if (var_r4) {
        u8 temp_r4 = *data;
        if (temp_r4 == Comm_GetLocalNetId()) {
            gCommState->joinReply = 1;
        }
    } else {
        var_r4 = TRUE;
        for (i = 1; i < 6; i++) {
            if (sJoinRefused[i] != data[i]) {
                var_r4 = FALSE;
                break;
            }
        }
        if (var_r4) {
            u8 temp_r4_2 = *data;
            u8 r0 = Comm_GetLocalNetId();
            if (temp_r4_2 == r0) {
                gCommState->joinReply = 2;
            }
        }
    }
}

s32 CommState_GetJoinMessageSize(void) {
    return 6;
}

void CommState_SetMaxEntries(s8 maxEntries) {
    if (gCommState) {
        gCommState->maxEntries = maxEntries;
        Wireless_SetMaxEntries(maxEntries);
    }
}

void CommState_BeaconWatch(void) {
    CommWireless_UpdateParents();
}

void CommState_OnBeacon(u32 ggid, s32 info) {
    switch (ggid) {
    case 0x333:
        if (info == 15) {
            gCommState->seenGames = (1 | gCommState->seenGames);
        }
        break;
    case 0x400318:
        gCommState->seenGames = (1 | gCommState->seenGames);
        break;
    case 0x400131:
        gCommState->seenGames = (2 | gCommState->seenGames);
        break;
    case 0x400286:
        gCommState->seenGames = (4 | gCommState->seenGames);
        break;
    }
}

void CommState_BeaconWatchScan(void) {
    if (Comm_StartScan(1, 1, 32)) {
        sub_02035FD8();
        CommState_SetState(CommState_BeaconWatch, 0);
    }
}

void CommState_BeaconWatchInit(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 0);
        CommPlayerInfo_Init(gCommState->saveData, 0);
        Wireless_SetBeaconCallback(CommState_OnBeacon);
        CommState_SetState(CommState_BeaconWatchScan, 0);
    }
}

void CommState_StartBeaconWatch(SaveData *saveData) {
    if (!Comm_IsActive()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 0x7000);
        CommState_Init(saveData, 17);
        gCommState->beaconId = 0;
        CommState_SetState(CommState_BeaconWatchInit, 0);
    }
}

void CommState_StopBeaconWatch(void) {
    if (gCommState) {
        CommState_SetState(CommState_ShutdownWaitStopped, 5);
    }
}

u8 CommState_GetSeenGames(void) {
    return gCommState->seenGames;
}

BOOL CommState_StopForReset(void) {
    Comm_StopFrameUpdate();
    if (gCommState == NULL) {
        return TRUE;
    }
    if ((gCommState->linkMode == 24) || (gCommState->linkMode == 25) || (gCommState->linkMode == 36)) {
        WifiConnection_Shutdown();
        return TRUE;
    }
    if (CommState_IsWifi()) {
        if (gCommState->linkMode == 33) {
            CommState_SetState(CommState_PlazaLogout, 0);
        } else {
            CommState_SetState(CommState_WifiShutdown, 0);
        }
    } else {
        CommState_SetState(CommState_StopWireless, 0);
    }
    return FALSE;
}

void CommState_MysteryGiftParentStart(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        CommPlayerInfo_Init(gCommState->saveData, 0);
        if (Comm_StartParent(1, 1, 512, 1)) {
            sub_02035FD8();
            CommState_SetState(CommState_WaitLinked, 0);
        }
    }
}

void CommState_MysteryGiftChildStart(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        CommPlayerInfo_Init(gCommState->saveData, 0);
        if (Comm_StartScan(1, 1, 512)) {
            sub_02035FD8();
            CommState_SetState(CommState_ChildScanning, 0);
        }
    }
}

void CommState_WifiFailed(void) {
}

void CommState_WifiTimedOut(void) {
    if (WifiConnection_Update(0) < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
    }
}

void CommState_WifiDisconnected(void) {
}

void CommState_WifiCancelled(void) {
}

void CommState_WifiConnected(void) {
    Comm_SetWifiConnected(1);
    s32 temp_r0 = WifiConnection_Update(0);
    if ((temp_r0 >= 10) && (temp_r0 < 20)) {
        CommState_SetState(CommState_WifiCancelled, 0);
    } else if (temp_r0 < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
    } else if (temp_r0 == 23) {
        CommState_SetState(CommState_WifiTimedOut, 0);
    } else if (temp_r0 == 24) {
        if (gCommState->watchMembers) {
            CommState_SetState(CommState_WifiFailed, 0);
        } else {
            CommState_SetState(CommState_WifiDisconnected, 0);
        }
    } else if (temp_r0 == 21) {
        if (gCommState->watchMembers) {
            CommState_SetState(CommState_WifiFailed, 0);
        } else {
            CommState_SetState(CommState_WifiDisconnected, 0);
        }
    }
    if (gCommState->watchMembers && (gCommState->memberCount != Comm_CountConnected())) {
        CommState_SetState(CommState_WifiFailed, 0);
    }
}

void CommState_SetWifiError(u32 error, s32 type, s32 status) {
    if (gCommState) {
        if (error - 25 > 1) {
            error = -error;
        }
        gCommState->wifiError = error;
        gCommState->wifiErrorArg1 = type;
        gCommState->wifiErrorArg2 = status;
    }
}

void CommState_WifiLobby(void) {
    s32 temp_r0 = WifiConnection_Update(0);
    if ((temp_r0 >= 10) && (temp_r0 < 20)) {
        CommState_SetState(CommState_WifiCancelled, 0);
    } else if (temp_r0 < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
    } else if (temp_r0 == 20) {
        CommState_SetState(CommState_WifiConnected, 0);
    } else if (temp_r0 == 21) {
        CommState_SetState(CommState_WifiDisconnected, 0);
    } else if (temp_r0 == 22) {
        CommState_SetState(CommState_WifiCancelled, 0);
    } else if (temp_r0 == 24) {
        CommState_SetState(CommState_WifiDisconnected, 0);
    }
}

void CommState_WifiStartMatch(void) {
    if (WifiConnection_Update(1) < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
        return;
    }
    u32 temp_r0 = WifiConnection_StartMatchmaking(gCommState->matchFriendIndex, Comm_GetMaxChildren(gCommState->linkMode) + 1, 0) + 3;
    switch (temp_r0) {
    case 3:
        Comm_ResetWork();
        CommState_SetState(CommState_WifiLobby, 0);
        return;
    case 0:
        CommState_SetState(CommState_WifiFailed, 0);
    case 1:
    case 2:
        return;
    }
}

s32 CommState_StartWifiMatch(s32 friendIndex) {
    if (gCommState->stateFunc != CommState_WifiLobby) {
        return 0;
    }
    WifiConnection_SetReceiveCallbacks(&Comm_RecvChildFrameCallback, &Comm_RecvParentFrameCallback);
    gCommState->matchFriendIndex = friendIndex;
    CommState_SetState(CommState_WifiStartMatch, 0);
    return 1;
}

s32 CommState_GetWifiMatchStatus(void) {
    if (gCommState->stateFunc == CommState_WifiLobby) {
        return 0;
    }
    if (gCommState->stateFunc == CommState_WifiConnected) {
        return 1;
    }
    if (gCommState->stateFunc == &CommState_WifiTimedOut) {
        return 3;
    }
    if (gCommState->stateFunc == CommState_WifiDisconnected) {
        return 4;
    }
    if (gCommState->stateFunc == CommState_WifiCancelled) {
        return 5;
    }
    if (gCommState->stateFunc == CommState_PlazaMatching) {
        return 0;
    }
    if (gCommState->stateFunc == CommState_PlazaMatchConnected) {
        return 1;
    }
    if (gCommState->stateFunc == CommState_PlazaTimedOut) {
        return 3;
    }
    return 2;
}

void CommState_WifiJoinLobby(void) {
    WifiConnection_SetReceiveCallbacks(&Comm_RecvChildFrameCallback, &Comm_RecvParentFrameCallback);
    switch (WifiConnection_StartMatchmaking(-1, 4, 1) + 4) {
    case 4:
        gCommState->peerDisconnected = 0;
        Comm_ResetWork();
        CommState_SetState(CommState_WifiLobby, 0);
        break;
    case 1:
        CommState_SetState(CommState_WifiFailed, 0);
        break;
    case 0:
        return;
    case 2:
        break;
    }
    s32 temp_r0_2 = WifiConnection_CheckError();
    if (temp_r0_2 < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
    } else if (temp_r0_2 == 26) {
        CommState_SetState(CommState_WifiFailed, 0);
    }
}

void CommState_WifiDisconnect(void) {
    Comm_SetWifiConnected(0);
    if (WifiConnection_Disconnect(gCommState->waitHostDisconnect) && WifiConnection_FinishDisconnect()) {
        CommPlayerInfo_Free();
        CommState_SetState(CommState_WifiJoinLobby, 0);
    } else if (WifiConnection_Update(0) < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
    }
}

void CommState_RecvWifiDisconnect(void) {
    if (!Comm_GetLocalNetId()) {
        gCommState->waitHostDisconnect = 0;
        CommState_SetState(CommState_WifiDisconnect, 0);
    } else {
        gCommState->waitHostDisconnect = 1;
        CommState_SetState(CommState_WifiDisconnect, 0);
    }
    gCommState->peerDisconnected = 1;
}

u8 CommState_WasWifiDisconnected(void) {
    return gCommState->peerDisconnected;
}

BOOL CommState_IsWifiJoiningLobby(void) {
    if (gCommState->stateFunc == CommState_WifiJoinLobby) {
        return TRUE;
    }
    if (gCommState->stateFunc == CommState_PlazaLobby) {
        return TRUE;
    }
    return FALSE;
}

BOOL CommState_IsWifiInLobby(void) {
    if (gCommState->stateFunc == CommState_WifiLobby) {
        return TRUE;
    }
    if (gCommState->stateFunc == CommState_WifiJoinLobby) {
        return TRUE;
    }
    if (gCommState->stateFunc == CommState_PlazaLobby) {
        return TRUE;
    }
    return FALSE;
}

u32 *CommState_GetWifiError(void) {
    GF_ASSERT(gCommState);
    return &gCommState->wifiError;
}

void CommState_WifiShutdown(void) {
    Comm_SetWifiConnected(0);
    if (WifiConnection_Disconnect(0)) {
        WifiConnection_FinishDisconnect();
        CommState_SetState(CommState_ShutdownWaitStopped, 0);
    }
    if (WifiConnection_Update(0) < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
    }
}

void CommState_EndWifiSession(void) {
    if (gCommState) {
        Sys_ClearSoftResetDisableFlag(1);
        CommPlayerInfo_Free();
        CommState_SetState(CommState_ShutdownWaitStopped, 5);
    }
}

void CommState_LeaveWifiMatch(void) {
    if (gCommState) {
        gCommState->waitHostDisconnect = 0;
        CommState_SetState(CommState_WifiDisconnect, 0);
    }
}

void CommState_EndWifiMatch(void) {
    if (gCommState) {
        CommState_SetErrorWatch(0, 1);
        if (!Comm_GetLocalNetId()) {
            gCommState->waitHostDisconnect = 0;
        } else {
            gCommState->waitHostDisconnect = 1;
        }
        CommState_SetState(CommState_WifiDisconnect, 0);
    }
}

void CommState_SendWifiDisconnect(void) {
    u8 r1 = Comm_GetLocalNetId();
    Comm_SendFixedCommand(21, &r1);
}

BOOL CommState_HasWifiFailed(void) {
    if (gCommState) {
        void (*func)() = gCommState->stateFunc;
        if (func == CommState_WifiFailed) {
            return TRUE;
        }
        if (func == CommState_WifiTimedOut && gCommState->watchMembers) {
            return TRUE;
        }
        if (func == CommState_PlazaTimedOut && gCommState->watchMembers) {
            return TRUE;
        }
    }
    return FALSE;
}

void CommState_InitWfcSession(SaveData *saveData) {
    if (gCommState == NULL) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 256);
        gCommState = Heap_Alloc(HEAP_ID_15, sizeof(CommStateWork));
        MI_CpuClear8(gCommState, sizeof(CommStateWork));
        gCommState->linkMode = 24;
        gCommState->unk55 = 1;
        gCommState->saveData = saveData;
        CommState_SetErrorWatch(0, 1);
        Sys_SetSoftResetDisableFlag(1);
    }
}

void CommState_FreeWfcSession(void) {
    if (gCommState) {
        Sys_ClearSoftResetDisableFlag(1);
        CommState_SetErrorWatch(0, 0);
        Heap_Free(gCommState);
        gCommState = NULL;
        Heap_Destroy(HEAP_ID_15);
    }
}

void CommState_InitMysteryGiftWfcSession(SaveData *saveData) {

    if (gCommState == NULL) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 256);
        gCommState = Heap_Alloc(HEAP_ID_15, sizeof(CommStateWork));
        MI_CpuClear8(gCommState, sizeof(CommStateWork));
        gCommState->linkMode = 25;
        gCommState->unk55 = 1;
        gCommState->saveData = saveData;
        CommState_SetErrorWatch(0, 1);
        Sys_SetSoftResetDisableFlag(1);
    }
}

void CommState_FreeMysteryGiftWfcSession(void) {
    if (gCommState) {
        Sys_ClearSoftResetDisableFlag(1);
        CommState_SetErrorWatch(0, 0);
        Heap_Free(gCommState);
        gCommState = NULL;
        Heap_Destroy(HEAP_ID_15);
    }
}

void CommState_InitWiiMessageWfcSession(SaveData *saveData) {
    if (gCommState == NULL) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 256);
        gCommState = Heap_Alloc(HEAP_ID_15, sizeof(CommStateWork));
        MI_CpuClear8(gCommState, sizeof(CommStateWork));
        gCommState->linkMode = 36;
        gCommState->unk55 = 1;
        gCommState->saveData = saveData;
        CommState_SetErrorWatch(0, 1);
        Sys_SetSoftResetDisableFlag(1);
    }
}

void CommState_FreeWiiMessageWfcSession(void) {
    if (gCommState) {
        Sys_ClearSoftResetDisableFlag(1);
        CommState_SetErrorWatch(0, 0);
        Heap_Free(gCommState);
        gCommState = NULL;
        Heap_Destroy(HEAP_ID_15);
    }
}

BOOL CommState_CheckDwcError(void) {
    if (gCommState == NULL) {
        return FALSE;
    }
    s32 sp4;
    s32 sp0;
    if (gCommState->linkMode == 24 || gCommState->linkMode == 36) {
        if (DWC_GetLastErrorEx(&sp4, &sp0)) {
            DWC_ClearError();
            return TRUE;
        }
    } else if ((gCommState->linkMode == 25) && DWC_GetLastErrorEx(&sp4, &sp0) && (sp0 == 7)) {
        return TRUE;
    }
    return FALSE;
}

void CommState_WifiLogin(void) {
    s32 temp_r0 = WifiConnection_UpdateLogin();
    gCommState->timer--;
    if (temp_r0 == 27) {
        if (gCommState->linkMode == 33) {
            if (WiFiPlazaLobbyClient_Initialize(gCommState->plazaProfileId) == 1) {
                gCommState->plazaLobbyStarted = 1;
                CommState_SetState(CommState_PlazaWaitLobby, gCommState->timer);
            } else {
                CommState_SetState(CommState_PlazaLobbyFailed, 0);
            }
        } else {
            CommState_SetState(CommState_WifiJoinLobby, 0);
        }
    } else {
        if (temp_r0) {
            CommState_SetState(CommState_WifiFailed, 0);
        }
        if (gCommState->timer <= 0) {
            CommState_SetState(CommState_WifiFailed, 0);
        }
    }
}

void CommState_WifiClubStart(void) {
    if (CommWireless_IsStartedUp()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_48, 0x35400);
        if (Comm_StartParent(1, 1, 512, 1)) {
            WifiConnection_Init(gCommState->saveData, 48, 0x2C400, Comm_GetMaxChildren(gCommState->linkMode) + 1);
            WifiConnection_SetErrorCallback(CommState_ShowErrorAndHalt);
            sub_02035FD8();
            CommState_SetState(CommState_WifiLogin, 3600);
        }
    }
}

WifiClubStatusData *CommState_StartWifiClub(SaveData *saveData, u32 dataSize) {
    Save_PlayerData_GetProfile(saveData);
    if (Comm_IsActive()) {
        return NULL;
    }
    Sys_SetSoftResetDisableFlag(1);
    Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
    CommState_Init(saveData, 23);
    gCommState->wifiClubData = Heap_Alloc(HEAP_ID_15, dataSize);
    MI_CpuClear8(gCommState->wifiClubData, dataSize);
    gCommState->beaconId = 0;
    gCommState->saveData = saveData;
    CommState_SetState(CommState_WifiClubStart, 0);
    return gCommState->wifiClubData;
}

void CommState_StartWifiPlaza(SaveData *saveData, s32 profileId) {
    Save_PlayerData_GetProfile(saveData);
    if (!Comm_IsActive()) {
        Sys_SetSoftResetDisableFlag(1);
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, 33);
        gCommState->wifiClubData = NULL;
        gCommState->plazaProfileId = profileId;
        gCommState->plazaLobbyStarted = 0;
        gCommState->beaconId = 0;
        gCommState->saveData = saveData;
        CommState_SetState(CommState_PlazaStart, 0);
    }
}

void CommState_LeaveWifiPlaza(void) {
    if (gCommState) {
        CommState_SetState(CommState_PlazaLogout, 0);
    }
}

u8 CommState_HasPlazaLobbyStarted(void) {
    if (gCommState == NULL) {
        return 0;
    }
    return gCommState->plazaLobbyStarted;
}

BOOL CommState_HasPlazaLobbyFailed(void) {
    if (gCommState && (gCommState->stateFunc == CommState_PlazaLobbyFailed)) {
        return TRUE;
    }
    return FALSE;
}

void CommState_StartPlazaMatch(s32 index) {
    GF_ASSERT(gCommState);
    GF_ASSERT(CommState_IsWifiInLobby() == 1);
    if (!WiFiPlazaLobbyClient_IsMatchActive(index)) {
        WiFiPlazaLobbyClient_HostMatch(index, 4);
        CommState_SetState(CommState_PlazaMatching, 0);
    } else {
        WiFiPlazaLobbyClient_JoinMatch(index);
        CommState_SetState(CommState_PlazaMatching, 0);
    }
}

void CommState_EndPlazaMatch(void) {
    if (WiFiPlazaLobbyClient_IsMatchHost() == 1) {
        WiFiPlazaLobbyClient_StopMatch();
    }
    WiFiPlazaLobbyClient_EndMatch();
    CommState_SetState(CommState_PlazaEndMatch, 0);
}

s32 CommState_GetPlazaMatchStatus(void) {
    if (gCommState) {
        if (gCommState->stateFunc == CommState_PlazaMatchConnected) {
            return 2;
        }
        if (gCommState->stateFunc == CommState_PlazaMatching) {
            return 1;
        }
        if (gCommState->stateFunc == CommState_PlazaEndMatch) {
            return 3;
        }
    }
    return 0;
}

WifiClubStatusData *CommState_GetWifiClubData(void) {
    return gCommState->wifiClubData;
}

void CommState_SetErrorWatch(s8 watchMembers, s8 reportErrors) {
    if (gCommState) {
        gCommState->watchMembers = watchMembers;
        gCommState->reportErrors = reportErrors;
        if (watchMembers) {
            gCommState->memberCount = Comm_CountConnected();
        } else {
            gCommState->memberCount = 0;
        }
    }
    CommWireless_SetLostWhenAlone(watchMembers);
    CommWireless_SetDetectDrops(watchMembers);
}

BOOL CommState_ReportsErrors(void) {
    if (gCommState) {
        if (gCommState->errorKind) {
            return TRUE;
        }
        return gCommState->reportErrors;
    }
    return FALSE;
}

u8 CommState_GetLinkMode(void) {
    if (gCommState) {
        return gCommState->linkMode;
    }
    return 1;
}

u8 CommState_GetBeaconId(void) {
    if (gCommState) {
        return gCommState->beaconId;
    }
    return 0;
}

void CommState_SetSelectedOrder(void *order) {
    MI_CpuCopy8(order, &gCommState->selectedOrder, 6);
}

void CommState_GetSelectedOrder(void *order) {
    MI_CpuCopy8(&gCommState->selectedOrder, order, 6);
}

int CommState_IsWifi(void) {
    CommState_GetLinkMode();
    return Comm_IsWifiLinkMode();
}

void CommState_SetErrorScreen(s8 errorScreen) {
    if (gCommState) {
        gCommState->errorScreen = errorScreen;
    }
}

u8 CommState_GetErrorScreen(void) {
    if (Sys_IsAsyncSaveInProgress()) {
        return 0;
    }
    if (gCommState) {
        return gCommState->errorScreen;
    }
    return 0;
}

void CommState_ShowErrorAndHalt(u32 errorCode) {
    ShowCommunicationError(HEAP_ID_DEFAULT, 1, errorCode);
    while (TRUE) {}
}

void CommState_ShowRestartErrorAndHalt(void) {
    ShowCommunicationError(HEAP_ID_DEFAULT, 4, 0);
    while (TRUE) {}
}

void CommState_FrameDummy(void) {
}

void CommState_CheckLinkError(void) {
    if (CommState_ReportsErrors() && (Comm_HasLinkError() || CommState_HasWifiFailed() || CommState_CheckDwcError() || gCommState->errorKind || CommState_HasPlazaLobbyFailed()) && !Sys_IsAsyncSaveInProgress() && !CommState_GetErrorScreen()) {
        Sound_Stop();
        Save_Cancel(gCommState->saveData);
        gSystem.touchpadReadAuto = TRUE;
        if (gCommState->errorKind == 3) {
            CommState_SetErrorScreen(3);
            return;
        }
        if (gCommState->errorKind == 5) {
            CommState_SetErrorScreen(5);
            return;
        }
        if ((gCommState->linkMode == 25) || (gCommState->linkMode == 15) || (gCommState->linkMode == 36)) {
            CommState_SetErrorScreen(2);
            return;
        }
        CommState_SetErrorScreen(1);
    }
}

BOOL CommState_IsLinkDown(void) {
    if (CommWireless_IsManagerEnded() || !CommState_Exists()) {
        return TRUE;
    }
    if (!CommWireless_IsInitialized()) {
        return TRUE;
    }
    if (Wireless_HasFatalError()) {
        return TRUE;
    }
    return FALSE;
}

BOOL CommState_SetError(int errorKind) {
    if (gCommState) {
        gCommState->errorKind = errorKind;
        Comm_StopFrameUpdate();
        return TRUE;
    }
    return FALSE;
}

void sub_02039AF8(void) {
    gCommState->linkMode = 29;
    WifiConnection_SetReceiveCallbacks(&Comm_RecvPeerFrameCallback, &Comm_RecvPeerFrameCallback);
}

void CommState_SetClubWifiLinkMode(void) {
    gCommState->linkMode = 35;
    WifiConnection_SetReceiveCallbacks(&Comm_RecvPeerFrameCallback, &Comm_RecvPeerFrameCallback);
}

void CommState_SetPlazaLinkMode(void) {
    gCommState->linkMode = 33;
    WifiConnection_SetReceiveCallbacks(&Comm_RecvPeerFrameCallback, &Comm_RecvPeerFrameCallback);
}

void CommState_SetWifiClubLinkMode(void) {
    gCommState->linkMode = 23;
    WifiConnection_SetReceiveCallbacks(&Comm_RecvChildFrameCallback, &Comm_RecvParentFrameCallback);
}

void CommState_SetSingleBattleWifiLinkMode(void) {
    gCommState->linkMode = 19;
    WifiConnection_SetReceiveCallbacks(&Comm_RecvChildFrameCallback, &Comm_RecvParentFrameCallback);
}

void CommState_PlazaStart(void) {
    if (CommWireless_IsStartedUp()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_48, 0x60000);
        if (Comm_StartParent(1, 1, 512, 1)) {
            WifiConnection_Init(gCommState->saveData, 48, 0x58000, Comm_GetMaxChildren(gCommState->linkMode) + 1);
            WifiConnection_SetErrorCallback(CommState_ShowErrorAndHalt);
            sub_02035FD8();
            WifiConnection_SetVoiceChatEnabled(0);
            sub_020378E4(0);
            CommState_SetState(CommState_WifiLogin, 3600);
        }
    }
}

void CommState_PlazaWaitLobby(void) {
    gCommState->timer--;
    if (gCommState->timer <= 0) {
        CommState_SetState(CommState_WifiFailed, 0);
        return;
    }
    Comm_SetWifiConnected(1);
    if (CommState_PlazaUpdate() && WiFiPlazaLobbyClient_IsInLobby()) {
        CommState_SetState(CommState_PlazaLobby, 0);
    }
}

void CommState_PlazaLobby(void) {
    if (CommState_PlazaUpdate()) {
        return;
    }
}

s32 CommState_PlazaCheckWifiResult(s32 result) {
    s32 var_r4 = 1;
    if ((result >= 10) && (result < 20)) {
        CommState_SetState(CommState_WifiCancelled, 0);
        var_r4 = 0;
    } else if (result < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
        var_r4 = 0;
    } else if (result == 23) {
        CommState_SetState(CommState_PlazaTimedOut, 0);
        var_r4 = 0;
    } else if (result == 24) {
        var_r4 = 0;
        if (gCommState->watchMembers) {
            CommState_SetState(CommState_WifiFailed, 0);
        } else {
            CommState_SetState(CommState_WifiDisconnected, 0);
        }
    }
    if (gCommState->watchMembers && (gCommState->memberCount != Comm_CountConnected())) {
        CommState_SetState(CommState_WifiFailed, 0);
        var_r4 = 0;
    }
    return var_r4;
}

s32 CommState_PlazaUpdateLobby(void) {
    s32 var_r4 = 1;
    u32 temp_r0 = WiFiPlazaLobbyClient_Update();
    switch (temp_r0) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        CommState_SetState(CommState_PlazaLobbyFailed, 0);
        var_r4 = 0;
        break;
    }
    return var_r4;
}

int CommState_PlazaUpdate(void) {
    s32 ret = CommState_PlazaCheckWifiResult(WifiConnection_Update(0));
    if (ret) {
        return CommState_PlazaUpdateLobby();
    }
    return ret;
}

void CommState_PlazaUpdateMatch(s32 *matchResult) {
    s32 temp_r0 = WifiConnection_Update(0);
    if (temp_r0 >= 20) {
        *matchResult = temp_r0;
    } else {
        *matchResult = 0;
        if (!CommState_PlazaCheckWifiResult(temp_r0)) {
            return;
        }
    }
    CommState_PlazaUpdateLobby();
}

void CommState_PlazaLogout(void) {
    WiFiPlazaLobbyClient_Shutdown();
    CommState_SetState(CommState_PlazaWaitLogout, 0);
}

void CommState_PlazaWaitLogout(void) {
    WifiConnection_Update(0);
    CommState_PlazaUpdateLobby();
    if (WiFiPlazaLobbyClient_IsDisconnected()) {
        Sys_ClearSoftResetDisableFlag(1);
        CommPlayerInfo_Free();
        CommState_SetState(CommState_ShutdownWaitStopped, 5);
        Comm_SetWifiConnected(0);
    }
}

void CommState_PlazaLobbyFailed(void) {
}

void CommState_PlazaMatchConnected(void) {
    CommState_PlazaUpdate();
    if (WiFiPlazaLobbyClient_WasMatchCancelled() == 1) {
        WiFiPlazaLobbyClient_EndMatch();
        CommState_SetState(CommState_PlazaEndMatch, 0);
    }
}

void CommState_PlazaMatching(void) {

    if (WiFiPlazaLobbyClient_WasMatchCancelled() == 1) {
        WiFiPlazaLobbyClient_EndMatch();
        CommState_SetState(CommState_PlazaEndMatch, 0);
        return;
    }
    s32 sp0;
    CommState_PlazaUpdateMatch(&sp0);
    if (sp0) {
        if (sp0 == 20) {
            CommState_SetState(CommState_PlazaMatchConnected, 0);
            return;
        }
        WiFiPlazaLobbyClient_EndMatch();
        CommState_SetState(CommState_PlazaLobby, 0);
    }
}

void CommState_PlazaEndMatch(void) {
    if (WifiConnection_Update(1) < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
        return;
    }
    if (CommState_PlazaUpdateLobby() && WifiConnection_Disconnect(0) && WifiConnection_FinishDisconnect()) {
        Comm_ResetWork();
        sub_020378E4(0);
        CommState_SetState(CommState_PlazaLobby, 0);
    }
}

void CommState_PlazaTimedOut(void) {
    if (WifiConnection_Update(0) < 0) {
        CommState_SetState(CommState_WifiFailed, 0);
    }
    CommState_PlazaUpdateLobby();
}

void CommState_SetFrontierWifiLinkMode(void) {
    gCommState->linkMode = 34;
}

void Comm_InitSyncedSave(s32 *state) {
    *state = 0;
}

s32 Comm_SyncedSave(SaveData *saveData, s32 arg1, u32 *state) {
    switch (*state) {
    case 0:
        LinkSync_StartSync(111);
        *state = 1;
    default:
        break;
    case 1:
        if (LinkSync_IsSynced(111)) {
            Save_PrepareForAsyncWrite(saveData, arg1);
            *state = 2;
        }
        break;
    case 2:
        s32 temp_r0 = Save_WriteFileAsync(saveData);
        GF_ASSERT(temp_r0 != 2);
        if (temp_r0 == 1) {
            *state = 3;
            LinkSync_StartSync(112);
        } else if (temp_r0 == 3) {
            *state = 7;
        }
        break;
    case 3:
        if (LinkSync_IsSynced(112)) {
            *state = 4;
        }
        break;
    case 4:
        temp_r0 = Save_WriteFileAsync(saveData);
        GF_ASSERT(temp_r0 != 1);
        if (temp_r0 == 2) {
            *state = 5;
        } else if (temp_r0 == 3) {
            *state = 7;
        }
        break;
    case 5:
        *state = 6;
        break;
    case 6:
        return 1;
    }
    return 0;
}

void Comm_RecordPlayerLocations(SaveWiFiHistory *wifiHistory) {
    s32 temp_r7 = Comm_CountConnected();
    if (CommPlayerInfo_GetProfile(0)) {
        for (int i = 0; i < temp_r7; i++) {
            if (i != Comm_GetLocalNetId()) {
                Comm_MarkLocationSeen(wifiHistory, CommPlayerInfo_GetCountry(i), CommPlayerInfo_GetRegion(i), PlayerProfile_GetLanguage(CommPlayerInfo_GetProfile(i)));
            }
        }
    }
}

void Comm_MarkLocationSeen(SaveWiFiHistory *history, s32 country, s32 region, u8 language) {
    if (!WiFiHistory_GetLocationSeenState(history, country, region)) {
        WiFiHistory_SetLocationSeenState(history, country, region, 1);
    }
}

s32 Comm_InitDwc(enum HeapID heapID) {
    void *temp_r0 = Heap_Alloc(heapID, 1824);
    s32 temp_r5 = DWC_Init(((u32)temp_r0 + 31) & ~31); // TODO
    Heap_Free(temp_r0);
    return temp_r5;
}

s32 Comm_LoadAndInitDwc(enum HeapID heapID) {
    LoadDwcOverlay();
    LoadOVY38();
    s32 temp_r4 = Comm_InitDwc(heapID);
    UnloadDwcOverlay();
    UnloadOVY38();
    return temp_r4;
}

void Comm_EnsureDwcUserData(WiFiList *wifiList) {

    DWCUserData *userData = FriendRoster_GetUserData(wifiList);
    if (!DWC_CheckUserData(userData)) {
        DWC_CreateUserData(userData, 0x4144414A);
        DWC_ClearDirtyFlag(userData);
    }
}

int Comm_GetOwnGsProfileId(WiFiList *wifiList) {
    DWCUserData *userData = FriendRoster_GetUserData(wifiList);
    DWCFriendData friendData;
    DWC_CreateExchangeToken(userData, &friendData);
    return DWC_GetGsProfileId(userData, &friendData);
}

BOOL Comm_HasWifiProfile(SaveData *saveData) {
    DWCUserData *userData = FriendRoster_GetUserData(Save_FriendRoster_Get(saveData));
    if (DWC_CheckHasProfile(userData) && DWC_CheckValidConsole(userData)) {
        return TRUE;
    }
    return FALSE;
}

s32 Comm_FindFriendSlot(SaveData *saveData, DWCFriendData *exchangeToken, s32 *slot) {
    int i;
    DWCUserData *userData = FriendRoster_GetUserData(Save_FriendRoster_Get(saveData));
    DWCFriendData *friendData = FriendRoster_GetDwcFriendData(Save_FriendRoster_Get(saveData), 0);
    *slot = -1;
    if (!DWC_IsValidFriendData(exchangeToken)) {
        return 3;
    }
    for (i = 0; i < 32; i++) {
        if (DWC_IsEqualFriendData(exchangeToken, &friendData[i])) {
            *slot = i;
            return 0;
        }
        if (DWC_GetGsProfileId(userData, exchangeToken) > 0) {
            if (DWC_GetGsProfileId(userData, exchangeToken) == DWC_GetGsProfileId(userData, &friendData[i])) {
                *slot = i;
                return 1;
            }
        }
        if ((*slot < 0) && !DWC_IsValidFriendData(&friendData[i])) {
            *slot = i;
        }
    }
    return 2;
}

int Comm_FindFriendSlotByKey(SaveData *saveData, u64 friendKey, int *slot) {
    int i;
    DWCUserData *userData = FriendRoster_GetUserData(Save_FriendRoster_Get(saveData));
    DWCFriendData *friendData = FriendRoster_GetDwcFriendData(Save_FriendRoster_Get(saveData), 0);
    if (!DWC_CheckFriendKey(userData, friendKey)) {
        return 3;
    }
    DWCFriendData newFriendData;
    DWC_CreateFriendKeyToken(&newFriendData, friendKey);
    if (DWC_GetGsProfileId(userData, &newFriendData) <= 0) {
        return 3;
    }
    *slot = -1;
    for (i = 0; i < 32; i++) {
        if (DWC_GetGsProfileId(userData, &newFriendData) == DWC_GetGsProfileId(userData, &friendData[i])) {
            *slot = i;
            return 0;
        }
        if ((*slot < 0) && !DWC_IsValidFriendData(&friendData[i])) {
            *slot = i;
        }
    }
    return 2;
}

s32 Comm_RegisterLinkedFriends(SaveData *saveData, s32 *results, enum HeapID heapID) {
    s32 spC = 0;
    DWCFriendData *friendData = FriendRoster_GetDwcFriendData(Save_FriendRoster_Get(saveData), 0);
    for (int i = 0; i < Comm_CountConnected(); i++) {
        if (i != Comm_GetLocalNetId()) {
            DWCFriendData *temp_r0 = CommPlayerInfo_GetExchangeToken(i);
            if (temp_r0) {
                s32 sp10;
                results[i] = Comm_FindFriendSlot(saveData, temp_r0, &sp10);
                GF_ASSERT(results[i] != 3);
                if (results[i] == 0) {
                    Comm_StoreFriendFromPlayer(saveData, i, sp10, heapID, 2);
                    CommPlayerInfo_SaveRecords(saveData);
                } else if (results[i] == 1) {
                    if (!CommState_IsWifi()) {
                        Comm_StoreFriendFromPlayer(saveData, i, sp10, heapID, 1);
                        MI_CpuCopy8(temp_r0, &friendData[sp10], sizeof(DWCFriendData));
                        CommPlayerInfo_SaveRecords(saveData);
                    }
                } else if (results[i] == 2) {
                    spC = 1;
                }
            }
        }
    }
    return spC;
}

void Comm_StoreFriendFromPlayer(SaveData *saveData, s32 netId, s32 slot, enum HeapID heapID, u32 mode) {
    WiFiList *wifiList = Save_FriendRoster_Get(saveData);
    DWCFriendData *friendData = FriendRoster_GetDwcFriendData(wifiList, slot);
    PlayerProfile *playerProfile = CommPlayerInfo_GetProfile(netId);
    if (mode != 2) {
        MI_CpuCopy8(CommPlayerInfo_GetExchangeToken(netId), friendData, 12);
    }
    if (mode == 0) {
        String *string = PlayerProfile_GetPlayerName_NewString(playerProfile, heapID);
        FriendRoster_SetPlayerName(wifiList, slot, string);
        String_Delete(string);
        FriendRoster_SetEntryField(wifiList, slot, 8, PlayerProfile_GetTrainerGender(playerProfile));
        FriendRoster_SetEntryField(wifiList, slot, 0, PlayerProfile_GetTrainerID(playerProfile));
    } else if (((mode - 1) <= 1) && (FriendRoster_GetEntryField(wifiList, slot, 8) == 2)) {
        FriendRoster_SetEntryField(wifiList, slot, 8, PlayerProfile_GetTrainerGender(playerProfile));
        FriendRoster_SetEntryField(wifiList, slot, 0, PlayerProfile_GetTrainerID(playerProfile));
    }
    String *string = String_New(120, heapID);
    CopyU16ArrayToString(string, CommPlayerInfo_GetGroupName(netId));
    FriendRoster_SetGroupName(wifiList, slot, string);
    String_Delete(string);
    FriendRoster_SetEntryField(wifiList, slot, 7, PlayerProfile_GetAvatar(playerProfile));
    CommPlayerInfo_SaveRecords(saveData);
}

s32 Comm_FindFriendIndexOfPlayer(SaveData *saveData, s32 netId) {
    int i;
    DWCFriendData *friendData = CommPlayerInfo_GetExchangeToken(netId);
    WiFiList *r5 = Save_FriendRoster_Get(saveData);

    for (i = 0; i < 32; i++) {
        if (DWC_IsEqualFriendData(friendData, FriendRoster_GetDwcFriendData(r5, i))) {
            return i;
        }
    }
    return 32;
}
