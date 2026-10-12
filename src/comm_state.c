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
    UnkStruct_ov44_0223197C *unk0;
    u8 unk4[8];
    MATHRandContext32 mathRandContext;
    void (*stateFunc)(void);
    SaveData *saveData;
    PlayerProfile *profile;
    s32 unk30;
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
    u8 unk4F;
    u8 unk50;
    s8 unk51;
    u8 unk52;
    u8 watchMembers;
    u8 reportErrors;
    u8 unk55;
    u8 errorScreen;
    u8 refuseJoins;
    u8 seenGames;
    u8 unk59[3];
    u8 unk5C;
    u8 errorKind;
    u8 unk5E;
    u8 unkEF;
    s32 unk60;
    u8 unk64;
} CommStateWork;

CommStateWork *gCommState = NULL;

void (*const gCommLinkedStates[3])(void) = { CommState_ParentLinked, CommState_ChildLinked, NULL };

static u8 sJoinAccepted[] = " GAME";
static u8 sJoinRequest[] = "FREAK";
static u8 sJoinRefused[] = " FULL";

s32 sub_02037C94(void) {
    return 70;
}

void CommState_Init(SaveData *saveData, s8 arg1) {
    if (gCommState == NULL) {
        GF_ASSERT(saveData);
        CommWireless_StartUp();
        gCommState = Heap_Alloc(HEAP_ID_15, sizeof(CommStateWork));
        ;
        MI_CpuClear8(gCommState, sizeof(CommStateWork));
        gCommState->timer = 50;
        gCommState->unk52 = 1;
        gCommState->saveData = saveData;
        gCommState->profile = Save_PlayerData_GetProfile(saveData);
        gCommState->maxEntries = 2;
        gCommState->joinReply = 0;
        gCommState->refuseJoins = 0;
        gCommState->linkMode = arg1;
        CommRandom_Init(&gCommState->mathRandContext);
        Comm_RegisterCommandTable(0, 0, 0);
        if ((arg1 != 9) && (arg1 != 17) && (arg1 != 15)) {
            WirelessStatusIcon_Create();
        }
    }
}

void CommState_Free(void) {
    if (gCommState) {
        Comm_FreeCommandTable();
        if (gCommState->unk0) {
            Heap_Free(gCommState->unk0);
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

void CommState_StartParentSession(SaveData *saveData, s8 arg1, s8 arg2, s32 arg3) {
    if (!sub_02037474()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, arg1);
        gCommState->unk4F = arg2;
        gCommState->unk30 = arg3;
        CommState_SetState(CommState_ParentStart, 0);
    }
}

void CommState_StartChildSession(SaveData *saveData, s8 arg1, s8 arg2, s32 arg3) {
    if (!sub_02037474()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, arg1);
        gCommState->unk4F = arg2;
        gCommState->unk30 = arg3;
        CommState_SetState(CommState_ChildStartScan, 0);
    }
}

void CommState_ConnectToSlot(s8 arg0) {
    gCommState->slot = arg0;
    CommState_SetState(CommState_ChildConnecting, 0);
}

void CommState_RestartScan(void) {
    sub_02036274();
    CommState_SetState(CommState_RestartScanStop, 0);
}

void CommState_ShutdownWait(void) {
    if (gCommState->shutdownTimer > 0) {
        gCommState->shutdownTimer--;
    } else if (!gCommState->shutdownTimer) {
        sub_020399EC();
    }
    if (gCommState->timer) {
        gCommState->timer--;
    }
    if (CommWireless_Stop() && !gCommState->timer) {
        sub_02036024();
        CommState_SetState(CommState_FreeWhenDone, 0);
    }
}

void CommState_RequestShutdown(void) {
    if (gCommState) {
        gCommState->shutdownTimer = 1800;
        CommState_SetState(CommState_ShutdownWait, 5);
    }
}

void CommState_SetEntry(u32 arg0) {
    Wireless_SetEntry(arg0);
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

void sub_02037F18(SaveData *saveData) {
    if (gCommState == NULL) {
        if (!Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800)) {
            sub_020399EC();
        }
        CommState_Init(saveData, 9);
        gCommState->linkMode = 9;
        gCommState->unk4F = 0;
        CommState_SetState(CommState_AlternateInit, 0);
    }
}

void CommState_JoinParent(s8 arg0) {
    gCommState->slot = arg0;
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

void sub_02037FF0(void) {
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
        CommState_SetState(sub_02038418, 15);
    } else {
        CommState_SetState(sub_02038460, 5);
    }
}

s32 sub_02038070(void) {
    if (gCommState == NULL) {
        return 1;
    } else if (gCommState->stateFunc != CommState_AlternateWaitStopped) {
        return 1;
    }
    return 0;
}

void sub_02038094(void) {
    if (gCommState) {
        CommState_SetState(CommState_ShutdownWaitStopped, 5);
    }
}

void sub_020380B0(void) {
    if (gCommState) {
        gCommState->refuseJoins = 1;
        Wireless_SetEntryLocked(1);
    }
}

void sub_020380CC(void) {
    if (gCommState) {
        gCommState->refuseJoins = 1;
        Wireless_SetEntryLocked(1);
        CommState_SetState(sub_020388A8, 0);
    }
}

void sub_020380F4(void) {
    gCommState->linkMode = 18;
}

void sub_02038104(void) {
    gCommState->linkMode = 9;
}

void sub_02038114(SaveData *saveData, s8 arg1) {
    if (!sub_02037474()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, arg1);
        CommState_SetState(sub_02038E00, 0);
    }
}

void sub_02038148(SaveData *saveData, s8 arg1) {
    if (!sub_02037474()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, arg1);
        CommState_SetState(sub_02038E48, 0);
    }
}

void CommState_Main(void) {
    if (gCommState && gCommState->stateFunc) {
        gCommState->stateFunc();
    }
    if (CommState_IsWifi()) {
        WirelessStatusIcon_SetLevel(3 - ov00_021EC9D4());
    } else if (CommWireless_Exists()) {
        WirelessStatusIcon_SetLevel(3 - WM_GetLinkLevel());
    }
}

void CommState_SetState(void (*arg0)(), int arg1) {
    gCommState->stateFunc = arg0;
    gCommState->timer = arg1;
}

void CommState_EndWireless(void) {
    if (!CommState_IsWifi()) {
        if (!CommWireless_IsIdle()) {
            return;
        } else {
            sub_020374E4();
            CommWireless_SetManagerEnded(1);
        }
    }
    CommState_SetState(CommState_Idle, 0);
}

void CommState_StopWireless(void) {
    if (CommWireless_Stop()) {
        sub_0203622C();
        CommState_SetState(CommState_EndWireless, 0);
    }
}

void CommState_ParentStart(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        CommPlayerInfo_Init(gCommState->saveData, gCommState->unk30);
        if (sub_02035ED8(1, 1, 512, 1)) {
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
    if (!sub_02037474()) {
        CommState_SetState(CommState_FreeWhenDone, 0);
    }
}

void CommState_ChildStartScan(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        CommPlayerInfo_Init(gCommState->saveData, gCommState->unk30);
        if (sub_02035F14(1, 1, 512)) {
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
    if (sub_020360E4(gCommState->slot)) {
        CommState_SetState(CommState_ChildWaitLinked, 10);
    }
}

void CommState_ChildWaitLinked(void) {
    if (sub_02037700()) {
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
    if (CommWireless_IsIdle() && sub_02035F14(0, 1, 512)) {
        sub_02035FD8();
        CommState_SetState(CommState_ChildConnecting, 10);
    }
}

void CommState_ChildLinked(void) {
    if (!sub_02037474()) {
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
    if (CommWireless_IsIdle() && sub_02035F14(0, 1, 512)) {
        sub_02035FD8();
        CommState_SetState(CommState_ChildScanning, 10);
    }
}

void CommState_Idle(void) {
}

void CommState_FreeWhenDone(void) {
    if (!sub_02037474()) {
        CommState_Free();
    }
}

void sub_02038418(void) {
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

void sub_02038460(void) {
    if (CommWireless_Stop()) {
        sub_0203622C();
        CommState_SetState(CommState_AlternateWaitStopped, 0);
    }
}

void CommState_ShutdownWaitStopped(void) {
    if (gCommState->timer) {
        gCommState->timer--;
    }
    if (CommWireless_Stop() && !gCommState->timer) {
        sub_02036024();
        CommState_SetState(CommState_FreeWhenDone, 0);
    }
}

void CommState_AlternateInit(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        Wireless_SetMaxEntries(gCommState->maxEntries);
        CommPlayerInfo_Init(gCommState->saveData, 0);
        if (sub_02035F14(1, 1, 512)) {
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
    if (CommWireless_IsIdle() && sub_02035ED8(0, gCommState->unk52, 512, 1)) {
        // Equivalent to MATH_Rand32(&gCommState->mathRandContext, 0);
        MATHRandContext32 *mathRandContext = &gCommState->mathRandContext;
        mathRandContext->x = (mathRandContext->mul * mathRandContext->x) + mathRandContext->add;
        //
        sub_02035FD8();
        gCommState->unk52 = 0;
        CommState_SetState(&CommState_AlternateParentWait, 10000);
    }
}

void CommState_AlternateParentWait(void) {
    if (!CommWireless_HasSentBeacons()) {
        if (sub_020376F8()) {
            gCommState->unk52 = 1;
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
    if (CommWireless_IsIdle() && sub_02035F14(0, 0, 512)) {
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
    if (CommWireless_IsIdle() && sub_02035F14(0, 0, 512)) {
        sub_02035FD8();
        CommState_SetState(CommState_JoinConnect, 100);
    }
}

void CommState_JoinConnect(void) {
    if (CommWireless_GetParentMemberCount(gCommState->slot) && sub_020360E4(gCommState->slot)) {
        CommState_SetState(CommState_JoinWaitLinked, 100);
        return;
    }
    if (sub_02037700()) {
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
    if (sub_02037700()) {
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
    if (sub_02037700()) {
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
        sub_020376E0(6, sJoinRequest);
    }
    if (gCommState->timer) {
        gCommState->timer--;
        return;
    }
    CommState_SetState(CommState_JoinRetry, 0);
}

void CommState_Joined(void) {
    if (sub_02037700()) {
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
    if (!sub_020376F8()) {
        if (!CommState_ReportsErrors() && CommWireless_Stop()) {
            CommState_SetState(CommState_AlternateStartScan, 0);
        }
    } else if (!Comm_GetLocalNetId()) {
        CommPlayerInfo_HostRedistribute();
    }
    if (sub_02037700() && !CommState_ReportsErrors()) {
        CommState_SetState(CommState_JoinRetry, 0);
    }
}

void sub_020388A8(void) {
    if (CommWireless_IsIdle() && sub_02035ED8(0, gCommState->unk52, 512, 0)) {
        sub_02035FD8();
        CommState_SetState(CommState_Idle, 0);
    }
}

void sub_020388E0(void) {
    u8 sp[2];
    gCommState->linkMode = 13;
    if (!Comm_GetLocalNetId()) {
        sp[1] = 0;
        sub_020376E0(10, &sp[1]);
        return;
    }
    sp[0] = 0;
    sub_020376E0(10, sp);
}

void sub_02038918(s8 arg0) {
    gCommState->slot = arg0;
    gCommState->joinRetries = 3;
    CommState_SetState(sub_020389BC, 0);
}

void sub_0203893C(void) {
    gCommState->linkMode = 7;
}

void sub_0203894C(s8 arg0) {
    gCommState->slot = arg0;
    gCommState->linkMode = 7;
    gCommState->joinRetries = 3;
    WirelessStatusIcon_Create();
    CommState_SetState(CommState_JoinWaitStopped, 0);
}

void sub_0203897C(void) {
    gCommState->linkMode = 26;
}

void sub_0203898C(s8 arg0) {
    gCommState->slot = arg0;
    gCommState->linkMode = 26;
    gCommState->joinRetries = 3;
    WirelessStatusIcon_Create();
    CommState_SetState(CommState_JoinWaitStopped, 0);
}

void sub_020389BC(void) {
    if (CommWireless_Stop()) {
        CommState_SetState(sub_020389D4, 0);
    }
}

void sub_020389D4(void) {
    if (CommWireless_IsIdle()) {
        gCommState->linkMode = 13;
        if (sub_02035F14(0, 0, 512)) {
            sub_02035FE4();
            CommState_SetState(sub_02038A10, 100);
        }
    }
}

void sub_02038A10(void) {
    if (CommWireless_GetParentMemberCount(gCommState->slot) && sub_020360E4(gCommState->slot)) {
        CommState_SetState(sub_02038A94, 100);
        return;
    }
    if (sub_02037700() || !gCommState->timer) {
        gCommState->joinRetries--;
        if (!gCommState->joinRetries) {
            CommState_SetState(CommState_JoinFailed, 0);
            return;
        }
        CommState_SetState(sub_020389BC, 0);
        return;
    }
    if (gCommState->timer) {
        gCommState->timer--;
    }
}

void sub_02038A94(void) {
    if (gCommState->timer > 90) {
        gCommState->timer--;
        return;
    }
    if (sub_02037700()) {
        gCommState->joinRetries--;
        if (!gCommState->joinRetries) {
            CommState_SetState(CommState_JoinFailed, 0);
            return;
        }
        CommState_SetState(sub_020389BC, 0);
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
    CommState_SetState(sub_020389BC, 0);
}

void CommState_RecvJoinRequest(u8 arg0, u8 arg1, u8 *arg2) {
    u32 i;
    if (!Comm_GetLocalNetId()) {
        BOOL var_r3 = TRUE;
        for (i = 0; i < 6; i++) {
            if (sJoinRequest[i] != arg2[i]) {
                var_r3 = FALSE;
                break;
            }
        }
        if (var_r3 && !gCommState->refuseJoins) {
            sJoinAccepted[0] = arg0;
            sub_02037184(7, sJoinAccepted);
        } else {
            sJoinRefused[0] = arg0;
            sub_02037184(7, sJoinRefused);
        }
    }
}

void CommState_RecvJoinReply(u32 arg0, u32 arg1, u8 *arg2) {
    u32 i;
    BOOL var_r4 = TRUE;
    for (i = 1; i < 6; i++) {
        if (sJoinAccepted[i] != arg2[i]) {
            var_r4 = FALSE;
            break;
        }
    }
    if (var_r4) {
        u8 temp_r4 = *arg2;
        if (temp_r4 == Comm_GetLocalNetId()) {
            gCommState->joinReply = 1;
        }
    } else {
        var_r4 = TRUE;
        for (i = 1; i < 6; i++) {
            if (sJoinRefused[i] != arg2[i]) {
                var_r4 = FALSE;
                break;
            }
        }
        if (var_r4) {
            u8 temp_r4_2 = *arg2;
            u8 r0 = Comm_GetLocalNetId();
            if (temp_r4_2 == r0) {
                gCommState->joinReply = 2;
            }
        }
    }
}

s32 sub_02038C18(void) {
    return 6;
}

void CommState_SetMaxEntries(s8 arg0) {
    if (gCommState) {
        gCommState->maxEntries = arg0;
        Wireless_SetMaxEntries(arg0);
    }
}

void sub_02038C34(void) {
    CommWireless_UpdateParents();
}

void CommState_OnBeacon(u32 arg0, s32 arg1) {
    switch (arg0) {
    case 0x333:
        if (arg1 == 15) {
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

void sub_02038CC4(void) {
    if (sub_02035F14(1, 1, 32)) {
        sub_02035FD8();
        CommState_SetState(sub_02038C34, 0);
    }
}

void sub_02038CE8(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 0);
        CommPlayerInfo_Init(gCommState->saveData, 0);
        Wireless_SetBeaconCallback(CommState_OnBeacon);
        CommState_SetState(sub_02038CC4, 0);
    }
}

void sub_02038D28(SaveData *arg0) {
    if (!sub_02037474()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 0x7000);
        CommState_Init(arg0, 17);
        gCommState->unk4F = 0;
        CommState_SetState(sub_02038CE8, 0);
    }
}

void sub_02038D64(void) {
    if (gCommState) {
        CommState_SetState(CommState_ShutdownWaitStopped, 5);
    }
}

u8 CommState_GetSeenGames(void) {
    return gCommState->seenGames;
}

BOOL sub_02038D90(void) {
    sub_02037988();
    if (gCommState == NULL) {
        return TRUE;
    }
    if ((gCommState->linkMode == 24) || (gCommState->linkMode == 25) || (gCommState->linkMode == 36)) {
        WifiConnection_Shutdown();
        return TRUE;
    }
    if (CommState_IsWifi()) {
        if (gCommState->linkMode == 33) {
            CommState_SetState(sub_02039D78, 0);
        } else {
            CommState_SetState(sub_020392F4, 0);
        }
    } else {
        CommState_SetState(CommState_StopWireless, 0);
    }
    return FALSE;
}

void sub_02038E00(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        CommPlayerInfo_Init(gCommState->saveData, 0);
        if (sub_02035ED8(1, 1, 512, 1)) {
            sub_02035FD8();
            CommState_SetState(CommState_WaitLinked, 0);
        }
    }
}

void sub_02038E48(void) {
    if (CommWireless_IsStartedUp()) {
        CommWireless_Init(gCommState->profile, 1);
        CommPlayerInfo_Init(gCommState->saveData, 0);
        if (sub_02035F14(1, 1, 512)) {
            sub_02035FD8();
            CommState_SetState(CommState_ChildScanning, 0);
        }
    }
}

void sub_02038E90(void) {
}

void sub_02038E94(void) {
    if (WifiConnection_Update(0) < 0) {
        CommState_SetState(sub_02038E90, 0);
    }
}

void sub_02038EB0(void) {
}

void sub_02038EB4(void) {
}

void sub_02038EB8(void) {
    sub_0203786C(1);
    s32 temp_r0 = WifiConnection_Update(0);
    if ((temp_r0 >= 10) && (temp_r0 < 20)) {
        CommState_SetState(sub_02038EB4, 0);
    } else if (temp_r0 < 0) {
        CommState_SetState(sub_02038E90, 0);
    } else if (temp_r0 == 23) {
        CommState_SetState(sub_02038E94, 0);
    } else if (temp_r0 == 24) {
        if (gCommState->watchMembers) {
            CommState_SetState(sub_02038E90, 0);
        } else {
            CommState_SetState(sub_02038EB0, 0);
        }
    } else if (temp_r0 == 21) {
        if (gCommState->watchMembers) {
            CommState_SetState(sub_02038E90, 0);
        } else {
            CommState_SetState(sub_02038EB0, 0);
        }
    }
    if (gCommState->watchMembers && (gCommState->memberCount != Comm_CountConnected())) {
        CommState_SetState(sub_02038E90, 0);
    }
}

void CommState_SetWifiError(u32 arg0, s32 arg1, s32 arg2) {
    if (gCommState) {
        if (arg0 - 25 > 1) {
            arg0 = -arg0;
        }
        gCommState->wifiError = arg0;
        gCommState->wifiErrorArg1 = arg1;
        gCommState->wifiErrorArg2 = arg2;
    }
}

void sub_02038F9C(void) {
    s32 temp_r0 = WifiConnection_Update(0);
    if ((temp_r0 >= 10) && (temp_r0 < 20)) {
        CommState_SetState(sub_02038EB4, 0);
    } else if (temp_r0 < 0) {
        CommState_SetState(sub_02038E90, 0);
    } else if (temp_r0 == 20) {
        CommState_SetState(sub_02038EB8, 0);
    } else if (temp_r0 == 21) {
        CommState_SetState(sub_02038EB0, 0);
    } else if (temp_r0 == 22) {
        CommState_SetState(sub_02038EB4, 0);
    } else if (temp_r0 == 24) {
        CommState_SetState(sub_02038EB0, 0);
    }
}

void sub_0203900C(void) {
    if (WifiConnection_Update(1) < 0) {
        CommState_SetState(sub_02038E90, 0);
        return;
    }
    u32 temp_r0 = WifiConnection_StartMatchmaking(gCommState->unk51, sub_02033FC4(gCommState->linkMode) + 1, 0) + 3;
    switch (temp_r0) {
    case 3:
        sub_0203622C();
        CommState_SetState(sub_02038F9C, 0);
        return;
    case 0:
        CommState_SetState(sub_02038E90, 0);
    case 1:
    case 2:
        return;
    }
}

s32 sub_02039080(s32 arg0) {
    if (gCommState->stateFunc != sub_02038F9C) {
        return 0;
    }
    WifiConnection_SetReceiveCallbacks(&sub_02036904, &sub_020367A8);
    gCommState->unk51 = arg0;
    CommState_SetState(sub_0203900C, 0);
    return 1;
}

s32 sub_020390C4(void) {
    if (gCommState->stateFunc == sub_02038F9C) {
        return 0;
    }
    if (gCommState->stateFunc == sub_02038EB8) {
        return 1;
    }
    if (gCommState->stateFunc == &sub_02038E94) {
        return 3;
    }
    if (gCommState->stateFunc == sub_02038EB0) {
        return 4;
    }
    if (gCommState->stateFunc == sub_02038EB4) {
        return 5;
    }
    if (gCommState->stateFunc == sub_02039DE4) {
        return 0;
    }
    if (gCommState->stateFunc == sub_02039DC4) {
        return 1;
    }
    if (gCommState->stateFunc == sub_02039E7C) {
        return 3;
    }
    return 2;
}

void sub_02039144(void) {
    WifiConnection_SetReceiveCallbacks(&sub_02036904, &sub_020367A8);
    switch (WifiConnection_StartMatchmaking(-1, 4, 1) + 4) {
    case 4:
        gCommState->unk5C = 0;
        sub_0203622C();
        CommState_SetState(sub_02038F9C, 0);
        break;
    case 1:
        CommState_SetState(sub_02038E90, 0);
        break;
    case 0:
        return;
    case 2:
        break;
    }
    s32 temp_r0_2 = WifiConnection_CheckError();
    if (temp_r0_2 < 0) {
        CommState_SetState(sub_02038E90, 0);
    } else if (temp_r0_2 == 26) {
        CommState_SetState(sub_02038E90, 0);
    }
}

void sub_020391D0(void) {
    sub_0203786C(0);
    if (WifiConnection_Disconnect(gCommState->unk50) && WifiConnection_FinishDisconnect()) {
        CommPlayerInfo_Free();
        CommState_SetState(sub_02039144, 0);
    } else if (WifiConnection_Update(0) < 0) {
        CommState_SetState(sub_02038E90, 0);
    }
}

void sub_02039220(void) {
    if (!Comm_GetLocalNetId()) {
        gCommState->unk50 = 0;
        CommState_SetState(sub_020391D0, 0);
    } else {
        gCommState->unk50 = 1;
        CommState_SetState(sub_020391D0, 0);
    }
    gCommState->unk5C = 1;
}

u8 sub_02039264(void) {
    return gCommState->unk5C;
}

BOOL sub_02039274(void) {
    if (gCommState->stateFunc == sub_02039144) {
        return TRUE;
    }
    if (gCommState->stateFunc == sub_02039C60) {
        return TRUE;
    }
    return FALSE;
}

BOOL sub_020392A0(void) {
    if (gCommState->stateFunc == sub_02038F9C) {
        return TRUE;
    }
    if (gCommState->stateFunc == sub_02039144) {
        return TRUE;
    }
    if (gCommState->stateFunc == sub_02039C60) {
        return TRUE;
    }
    return FALSE;
}

u32 *CommState_GetWifiError(void) {
    GF_ASSERT(gCommState);
    return &gCommState->wifiError;
}

void sub_020392F4(void) {
    sub_0203786C(0);
    if (WifiConnection_Disconnect(0)) {
        WifiConnection_FinishDisconnect();
        CommState_SetState(CommState_ShutdownWaitStopped, 0);
    }
    if (WifiConnection_Update(0) < 0) {
        CommState_SetState(sub_02038E90, 0);
    }
}

void sub_02039330(void) {
    if (gCommState) {
        sub_0201A738(1);
        CommPlayerInfo_Free();
        CommState_SetState(CommState_ShutdownWaitStopped, 5);
    }
}

void sub_02039358(void) {
    if (gCommState) {
        gCommState->unk50 = 0;
        CommState_SetState(sub_020391D0, 0);
    }
}

void sub_02039378(void) {
    if (gCommState) {
        CommState_SetErrorWatch(0, 1);
        if (!Comm_GetLocalNetId()) {
            gCommState->unk50 = 0;
        } else {
            gCommState->unk50 = 1;
        }
        CommState_SetState(sub_020391D0, 0);
    }
}

void sub_020393B4(void) {
    u8 r1 = Comm_GetLocalNetId();
    sub_020376E0(21, &r1);
}

BOOL sub_020393C8(void) {
    if (gCommState) {
        void (*func)() = gCommState->stateFunc;
        if (func == sub_02038E90) {
            return TRUE;
        }
        if (func == sub_02038E94 && gCommState->watchMembers) {
            return TRUE;
        }
        if (func == sub_02039E7C && gCommState->watchMembers) {
            return TRUE;
        }
    }
    return FALSE;
}

void sub_02039418(SaveData *saveData) {
    if (gCommState == NULL) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 256);
        gCommState = Heap_Alloc(HEAP_ID_15, sizeof(CommStateWork));
        MI_CpuClear8(gCommState, sizeof(CommStateWork));
        gCommState->linkMode = 24;
        gCommState->unk55 = 1;
        gCommState->saveData = saveData;
        CommState_SetErrorWatch(0, 1);
        sub_0201A728(1);
    }
}

void sub_0203946C(void) {
    if (gCommState) {
        sub_0201A738(1);
        CommState_SetErrorWatch(0, 0);
        Heap_Free(gCommState);
        gCommState = NULL;
        Heap_Destroy(HEAP_ID_15);
    }
}

void sub_020394A0(SaveData *saveData) {

    if (gCommState == NULL) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 256);
        gCommState = Heap_Alloc(HEAP_ID_15, sizeof(CommStateWork));
        MI_CpuClear8(gCommState, sizeof(CommStateWork));
        gCommState->linkMode = 25;
        gCommState->unk55 = 1;
        gCommState->saveData = saveData;
        CommState_SetErrorWatch(0, 1);
        sub_0201A728(1);
    }
}

void sub_020394F4(void) {
    if (gCommState) {
        sub_0201A738(1);
        CommState_SetErrorWatch(0, 0);
        Heap_Free(gCommState);
        gCommState = NULL;
        Heap_Destroy(HEAP_ID_15);
    }
}

void sub_02039528(SaveData *saveData) {
    if (gCommState == NULL) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 256);
        gCommState = Heap_Alloc(HEAP_ID_15, sizeof(CommStateWork));
        MI_CpuClear8(gCommState, sizeof(CommStateWork));
        gCommState->linkMode = 36;
        gCommState->unk55 = 1;
        gCommState->saveData = saveData;
        CommState_SetErrorWatch(0, 1);
        sub_0201A728(1);
    }
}

void sub_0203957C(void) {
    if (gCommState) {
        sub_0201A738(1);
        CommState_SetErrorWatch(0, 0);
        Heap_Free(gCommState);
        gCommState = NULL;
        Heap_Destroy(HEAP_ID_15);
    }
}

BOOL sub_020395B0(void) {
    if (gCommState == NULL) {
        return FALSE;
    }
    s32 sp4;
    s32 sp0;
    if (gCommState->linkMode == 24 || gCommState->linkMode == 36) {
        if (ov00_021EC11C(&sp4, &sp0)) {
            ov00_021EC210();
            return TRUE;
        }
    } else if ((gCommState->linkMode == 25) && ov00_021EC11C(&sp4, &sp0) && (sp0 == 7)) {
        return TRUE;
    }
    return FALSE;
}

void sub_0203960C(void) {
    s32 temp_r0 = WifiConnection_UpdateLogin();
    gCommState->timer--;
    if (temp_r0 == 27) {
        if (gCommState->linkMode == 33) {
            if (ov45_0222E804(gCommState->unk60) == 1) {
                gCommState->unk64 = 1;
                CommState_SetState(sub_02039C14, gCommState->timer);
            } else {
                CommState_SetState(sub_02039DC0, 0);
            }
        } else {
            CommState_SetState(sub_02039144, 0);
        }
    } else {
        if (temp_r0) {
            CommState_SetState(sub_02038E90, 0);
        }
        if (gCommState->timer <= 0) {
            CommState_SetState(sub_02038E90, 0);
        }
    }
}

void sub_02039694(void) {
    if (CommWireless_IsStartedUp()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_48, 0x35400);
        if (sub_02035ED8(1, 1, 512, 1)) {
            WifiConnection_Init(gCommState->saveData, 48, 0x2C400, sub_02033FC4(gCommState->linkMode) + 1);
            WifiConnection_SetErrorCallback(CommState_ShowErrorAndHalt);
            sub_02035FD8();
            CommState_SetState(sub_0203960C, 3600);
        }
    }
}

UnkStruct_ov44_0223197C *sub_020396FC(SaveData *saveData, u32 arg1) {
    Save_PlayerData_GetProfile(saveData);
    if (sub_02037474()) {
        return NULL;
    }
    sub_0201A728(1);
    Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
    CommState_Init(saveData, 23);
    gCommState->unk0 = Heap_Alloc(HEAP_ID_15, arg1);
    MI_CpuClear8(gCommState->unk0, arg1);
    gCommState->unk4F = 0;
    gCommState->saveData = saveData;
    CommState_SetState(sub_02039694, 0);
    return gCommState->unk0;
}

void sub_0203976C(SaveData *saveData, s32 arg1) {
    Save_PlayerData_GetProfile(saveData);
    if (!sub_02037474()) {
        sub_0201A728(1);
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_15, 28800);
        CommState_Init(saveData, 33);
        gCommState->unk0 = NULL;
        gCommState->unk60 = arg1;
        gCommState->unk64 = 0;
        gCommState->unk4F = 0;
        gCommState->saveData = saveData;
        CommState_SetState(sub_02039BA0, 0);
    }
}

void sub_020397C8(void) {
    if (gCommState) {
        CommState_SetState(sub_02039D78, 0);
    }
}

u8 sub_020397E4(void) {
    if (gCommState == NULL) {
        return 0;
    }
    return gCommState->unk64;
}

BOOL sub_020397FC(void) {
    if (gCommState && (gCommState->stateFunc == sub_02039DC0)) {
        return TRUE;
    }
    return FALSE;
}

void sub_0203981C(s32 arg0) {
    GF_ASSERT(gCommState);
    GF_ASSERT(sub_020392A0() == 1);
    if (!ov45_0222F274(arg0)) {
        ov45_0222F028(arg0, 4);
        CommState_SetState(sub_02039DE4, 0);
    } else {
        ov45_0222F350(arg0);
        CommState_SetState(sub_02039DE4, 0);
    }
}

void sub_0203986C(void) {
    if (ov45_0222F464() == 1) {
        ov45_0222F154();
    }
    ov45_0222F70C();
    CommState_SetState(sub_02039E30, 0);
}

s32 sub_0203988C(void) {
    if (gCommState) {
        if (gCommState->stateFunc == sub_02039DC4) {
            return 2;
        }
        if (gCommState->stateFunc == sub_02039DE4) {
            return 1;
        }
        if (gCommState->stateFunc == sub_02039E30) {
            return 3;
        }
    }
    return 0;
}

UnkStruct_ov44_0223197C *sub_020398C8(void) {
    return gCommState->unk0;
}

void CommState_SetErrorWatch(s8 arg0, s8 arg1) {
    if (gCommState) {
        gCommState->watchMembers = arg0;
        gCommState->reportErrors = arg1;
        if (arg0) {
            gCommState->memberCount = Comm_CountConnected();
        } else {
            gCommState->memberCount = 0;
        }
    }
    CommWireless_SetLostWhenAlone(arg0);
    CommWireless_SetDetectDrops(arg0);
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

u8 sub_02039954(void) {
    if (gCommState) {
        return gCommState->unk4F;
    }
    return 0;
}

void sub_0203996C(void *arg0) {
    MI_CpuCopy8(arg0, &gCommState->unk4, 6);
}

void sub_02039980(void *arg0) {
    MI_CpuCopy8(&gCommState->unk4, arg0, 6);
}

int CommState_IsWifi(void) {
    CommState_GetLinkMode();
    return sub_02034044();
}

void CommState_SetErrorScreen(s8 arg0) {
    if (gCommState) {
        gCommState->errorScreen = arg0;
    }
}

u8 CommState_GetErrorScreen(void) {
    if (sub_0201A79C()) {
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

void sub_020399EC(void) {
    ShowCommunicationError(HEAP_ID_DEFAULT, 4, 0);
    while (TRUE) {}
}

void sub_020399FC(void) {
}

void CommState_CheckLinkError(void) {
    if (CommState_ReportsErrors() && (sub_02037700() || sub_020393C8() || sub_020395B0() || gCommState->errorKind || sub_020397FC()) && !sub_0201A79C() && !CommState_GetErrorScreen()) {
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

BOOL sub_02039AA4(void) {
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

BOOL CommState_SetError(int arg0) {
    if (gCommState) {
        gCommState->errorKind = arg0;
        sub_02037988();
        return TRUE;
    }
    return FALSE;
}

void sub_02039AF8(void) {
    gCommState->linkMode = 29;
    WifiConnection_SetReceiveCallbacks(&sub_020369CC, &sub_020369CC);
}

void sub_02039B18(void) {
    gCommState->linkMode = 35;
    WifiConnection_SetReceiveCallbacks(&sub_020369CC, &sub_020369CC);
}

void sub_02039B38(void) {
    gCommState->linkMode = 33;
    WifiConnection_SetReceiveCallbacks(&sub_020369CC, &sub_020369CC);
}

void sub_02039B58(void) {
    gCommState->linkMode = 23;
    WifiConnection_SetReceiveCallbacks(&sub_02036904, &sub_020367A8);
}

void sub_02039B7C(void) {
    gCommState->linkMode = 19;
    WifiConnection_SetReceiveCallbacks(&sub_02036904, &sub_020367A8);
}

void sub_02039BA0(void) {
    if (CommWireless_IsStartedUp()) {
        Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_48, 0x60000);
        if (sub_02035ED8(1, 1, 512, 1)) {
            WifiConnection_Init(gCommState->saveData, 48, 0x58000, sub_02033FC4(gCommState->linkMode) + 1);
            WifiConnection_SetErrorCallback(CommState_ShowErrorAndHalt);
            sub_02035FD8();
            ov00_021E70B8(0);
            sub_020378E4(0);
            CommState_SetState(sub_0203960C, 3600);
        }
    }
}

void sub_02039C14(void) {
    gCommState->timer--;
    if (gCommState->timer <= 0) {
        CommState_SetState(sub_02038E90, 0);
        return;
    }
    sub_0203786C(1);
    if (sub_02039D3C() && ov45_0222E8F4()) {
        CommState_SetState(sub_02039C60, 0);
    }
}

void sub_02039C60(void) {
    if (sub_02039D3C()) {
        return;
    }
}

s32 sub_02039C6C(s32 arg0) {
    s32 var_r4 = 1;
    if ((arg0 >= 10) && (arg0 < 20)) {
        CommState_SetState(sub_02038EB4, 0);
        var_r4 = 0;
    } else if (arg0 < 0) {
        CommState_SetState(sub_02038E90, 0);
        var_r4 = 0;
    } else if (arg0 == 23) {
        CommState_SetState(sub_02039E7C, 0);
        var_r4 = 0;
    } else if (arg0 == 24) {
        var_r4 = 0;
        if (gCommState->watchMembers) {
            CommState_SetState(sub_02038E90, 0);
        } else {
            CommState_SetState(sub_02038EB0, 0);
        }
    }
    if (gCommState->watchMembers && (gCommState->memberCount != Comm_CountConnected())) {
        CommState_SetState(sub_02038E90, 0);
        var_r4 = 0;
    }
    return var_r4;
}

s32 sub_02039D08(void) {
    s32 var_r4 = 1;
    u32 temp_r0 = WiFiPlazaLobbyClient_Update();
    switch (temp_r0) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        CommState_SetState(sub_02039DC0, 0);
        var_r4 = 0;
        break;
    }
    return var_r4;
}

int sub_02039D3C(void) {
    s32 ret = sub_02039C6C(WifiConnection_Update(0));
    if (ret) {
        return sub_02039D08();
    }
    return ret;
}

void sub_02039D54(s32 *arg0) {
    s32 temp_r0 = WifiConnection_Update(0);
    if (temp_r0 >= 20) {
        *arg0 = temp_r0;
    } else {
        *arg0 = 0;
        if (!sub_02039C6C(temp_r0)) {
            return;
        }
    }
    sub_02039D08();
}

void sub_02039D78(void) {
    ov45_0222E91C();
    CommState_SetState(sub_02039D8C, 0);
}

void sub_02039D8C(void) {
    WifiConnection_Update(0);
    sub_02039D08();
    if (ov45_0222E944()) {
        sub_0201A738(1);
        CommPlayerInfo_Free();
        CommState_SetState(CommState_ShutdownWaitStopped, 5);
        sub_0203786C(0);
    }
}

void sub_02039DC0(void) {
}

void sub_02039DC4(void) {
    sub_02039D3C();
    if (ov45_0222F254() == 1) {
        ov45_0222F70C();
        CommState_SetState(sub_02039E30, 0);
    }
}

void sub_02039DE4(void) {

    if (ov45_0222F254() == 1) {
        ov45_0222F70C();
        CommState_SetState(sub_02039E30, 0);
        return;
    }
    s32 sp0;
    sub_02039D54(&sp0);
    if (sp0) {
        if (sp0 == 20) {
            CommState_SetState(sub_02039DC4, 0);
            return;
        }
        ov45_0222F70C();
        CommState_SetState(sub_02039C60, 0);
    }
}

void sub_02039E30(void) {
    if (WifiConnection_Update(1) < 0) {
        CommState_SetState(sub_02038E90, 0);
        return;
    }
    if (sub_02039D08() && WifiConnection_Disconnect(0) && WifiConnection_FinishDisconnect()) {
        sub_0203622C();
        sub_020378E4(0);
        CommState_SetState(sub_02039C60, 0);
    }
}

void sub_02039E7C(void) {
    if (WifiConnection_Update(0) < 0) {
        CommState_SetState(sub_02038E90, 0);
    }
    sub_02039D08();
}

void sub_02039E9C(void) {
    gCommState->linkMode = 34;
}

void sub_02039EAC(s32 *arg0) {
    *arg0 = 0;
}

s32 sub_02039EB4(SaveData *saveData, s32 arg1, u32 *arg2) {
    switch (*arg2) {
    case 0:
        LinkSync_StartSync(111);
        *arg2 = 1;
    default:
        break;
    case 1:
        if (LinkSync_IsSynced(111)) {
            Save_PrepareForAsyncWrite(saveData, arg1);
            *arg2 = 2;
        }
        break;
    case 2:
        s32 temp_r0 = Save_WriteFileAsync(saveData);
        GF_ASSERT(temp_r0 != 2);
        if (temp_r0 == 1) {
            *arg2 = 3;
            LinkSync_StartSync(112);
        } else if (temp_r0 == 3) {
            *arg2 = 7;
        }
        break;
    case 3:
        if (LinkSync_IsSynced(112)) {
            *arg2 = 4;
        }
        break;
    case 4:
        temp_r0 = Save_WriteFileAsync(saveData);
        GF_ASSERT(temp_r0 != 1);
        if (temp_r0 == 2) {
            *arg2 = 5;
        } else if (temp_r0 == 3) {
            *arg2 = 7;
        }
        break;
    case 5:
        *arg2 = 6;
        break;
    case 6:
        return 1;
    }
    return 0;
}

void sub_02039F68(SaveWiFiHistory *arg0) {
    s32 temp_r7 = Comm_CountConnected();
    if (CommPlayerInfo_GetProfile(0)) {
        for (int i = 0; i < temp_r7; i++) {
            if (i != Comm_GetLocalNetId()) {
                sub_02039FB8(arg0, CommPlayerInfo_GetCountry(i), CommPlayerInfo_GetRegion(i), PlayerProfile_GetLanguage(CommPlayerInfo_GetProfile(i)));
            }
        }
    }
}

void sub_02039FB8(SaveWiFiHistory *arg0, s32 arg1, s32 arg2, u8 arg3) {
    if (!WiFiHistory_GetLocationSeenState(arg0, arg1, arg2)) {
        WiFiHistory_SetLocationSeenState(arg0, arg1, arg2, 1);
    }
}

s32 sub_02039FD8(enum HeapID heapID) {
    void *temp_r0 = Heap_Alloc(heapID, 1824);
    s32 temp_r5 = DWC_Init(((u32)temp_r0 + 31) & ~31); // TODO
    Heap_Free(temp_r0);
    return temp_r5;
}

s32 sub_02039FFC(enum HeapID heapID) {
    LoadDwcOverlay();
    LoadOVY38();
    s32 temp_r4 = sub_02039FD8(heapID);
    UnloadDwcOverlay();
    UnloadOVY38();
    return temp_r4;
}

void sub_0203A01C(WiFiList *arg0) {

    DWCUserData *userData = FriendRoster_GetUserData(arg0);
    if (!DWC_CheckUserData(userData)) {
        DWC_CreateUserData(userData, 0x4144414A);
        DWC_ClearDirtyFlag(userData);
    }
}

int sub_0203A040(WiFiList *arg0) {
    DWCUserData *userData = FriendRoster_GetUserData(arg0);
    DWCFriendData friendData;
    DWC_CreateExchangeToken(userData, &friendData);
    return DWC_GetGsProfileId(userData, &friendData);
}

BOOL sub_0203A05C(SaveData *saveData) {
    DWCUserData *userData = FriendRoster_GetUserData(Save_FriendRoster_Get(saveData));
    if (DWC_CheckHasProfile(userData) && DWC_CheckValidConsole(userData)) {
        return TRUE;
    }
    return FALSE;
}

s32 sub_0203A084(SaveData *saveData, DWCFriendData *arg1, s32 *arg2) {
    int i;
    DWCUserData *userData = FriendRoster_GetUserData(Save_FriendRoster_Get(saveData));
    DWCFriendData *friendData = FriendRoster_GetDwcFriendData(Save_FriendRoster_Get(saveData), 0);
    *arg2 = -1;
    if (!DWC_IsValidFriendData(arg1)) {
        return 3;
    }
    for (i = 0; i < 32; i++) {
        if (DWC_IsEqualFriendData(arg1, &friendData[i])) {
            *arg2 = i;
            return 0;
        }
        if (DWC_GetGsProfileId(userData, arg1) > 0) {
            if (DWC_GetGsProfileId(userData, arg1) == DWC_GetGsProfileId(userData, &friendData[i])) {
                *arg2 = i;
                return 1;
            }
        }
        if ((*arg2 < 0) && !DWC_IsValidFriendData(&friendData[i])) {
            *arg2 = i;
        }
    }
    return 2;
}

int sub_0203A128(SaveData *saveData, u64 arg1, int *arg2) {
    int i;
    DWCUserData *userData = FriendRoster_GetUserData(Save_FriendRoster_Get(saveData));
    DWCFriendData *friendData = FriendRoster_GetDwcFriendData(Save_FriendRoster_Get(saveData), 0);
    if (!DWC_CheckFriendKey(userData, arg1)) {
        return 3;
    }
    DWCFriendData newFriendData;
    DWC_CreateFriendKeyToken(&newFriendData, arg1);
    if (DWC_GetGsProfileId(userData, &newFriendData) <= 0) {
        return 3;
    }
    *arg2 = -1;
    for (i = 0; i < 32; i++) {
        if (DWC_GetGsProfileId(userData, &newFriendData) == DWC_GetGsProfileId(userData, &friendData[i])) {
            *arg2 = i;
            return 0;
        }
        if ((*arg2 < 0) && !DWC_IsValidFriendData(&friendData[i])) {
            *arg2 = i;
        }
    }
    return 2;
}

s32 sub_0203A1C4(SaveData *saveData, s32 *arg1, enum HeapID heapID) {
    s32 spC = 0;
    DWCFriendData *friendData = FriendRoster_GetDwcFriendData(Save_FriendRoster_Get(saveData), 0);
    for (int i = 0; i < Comm_CountConnected(); i++) {
        if (i != Comm_GetLocalNetId()) {
            DWCFriendData *temp_r0 = CommPlayerInfo_GetExchangeToken(i);
            if (temp_r0) {
                s32 sp10;
                arg1[i] = sub_0203A084(saveData, temp_r0, &sp10);
                GF_ASSERT(arg1[i] != 3);
                if (arg1[i] == 0) {
                    sub_0203A280(saveData, i, sp10, heapID, 2);
                    CommPlayerInfo_SaveRecords(saveData);
                } else if (arg1[i] == 1) {
                    if (!CommState_IsWifi()) {
                        sub_0203A280(saveData, i, sp10, heapID, 1);
                        MI_CpuCopy8(temp_r0, &friendData[sp10], sizeof(DWCFriendData));
                        CommPlayerInfo_SaveRecords(saveData);
                    }
                } else if (arg1[i] == 2) {
                    spC = 1;
                }
            }
        }
    }
    return spC;
}

void sub_0203A280(SaveData *saveData, s32 arg1, s32 arg2, enum HeapID heapID, u32 arg4) {
    WiFiList *wifiList = Save_FriendRoster_Get(saveData);
    DWCFriendData *friendData = FriendRoster_GetDwcFriendData(wifiList, arg2);
    PlayerProfile *playerProfile = CommPlayerInfo_GetProfile(arg1);
    if (arg4 != 2) {
        MI_CpuCopy8(CommPlayerInfo_GetExchangeToken(arg1), friendData, 12);
    }
    if (arg4 == 0) {
        String *string = PlayerProfile_GetPlayerName_NewString(playerProfile, heapID);
        sub_0202C270(wifiList, arg2, string);
        String_Delete(string);
        FriendRoster_SetEntryField(wifiList, arg2, 8, PlayerProfile_GetTrainerGender(playerProfile));
        FriendRoster_SetEntryField(wifiList, arg2, 0, PlayerProfile_GetTrainerID(playerProfile));
    } else if (((arg4 - 1) <= 1) && (FriendRoster_GetEntryField(wifiList, arg2, 8) == 2)) {
        FriendRoster_SetEntryField(wifiList, arg2, 8, PlayerProfile_GetTrainerGender(playerProfile));
        FriendRoster_SetEntryField(wifiList, arg2, 0, PlayerProfile_GetTrainerID(playerProfile));
    }
    String *string = String_New(120, heapID);
    CopyU16ArrayToString(string, CommPlayerInfo_GetGroupName(arg1));
    sub_0202C2B4(wifiList, arg2, string);
    String_Delete(string);
    FriendRoster_SetEntryField(wifiList, arg2, 7, PlayerProfile_GetAvatar(playerProfile));
    CommPlayerInfo_SaveRecords(saveData);
}

s32 sub_0203A378(SaveData *saveData, s32 arg1) {
    int i;
    DWCFriendData *friendData = CommPlayerInfo_GetExchangeToken(arg1);
    WiFiList *r5 = Save_FriendRoster_Get(saveData);

    for (i = 0; i < 32; i++) {
        if (DWC_IsEqualFriendData(friendData, FriendRoster_GetDwcFriendData(r5, i))) {
            return i;
        }
    }
    return 32;
}
