#include "global.h"

#include "communication_player_state.h"
#include "heap.h"
#include "mail_message.h"
#include "sys_task_api.h"
#include "union_room_internal.h"
#include "comm_wireless.h"
#include "communication_core.h"
#include "comm_state.h"
#include "unk_02091564.h"

// Communication core and Union Room message functions, declared with the types their call sites were matched
// against.
BOOL Comm_GetLocalNetId(void);
BOOL Comm_SendCommand(int command, void *data, int size);
BOOL Comm_SendParentCommand(int command, void *data, int size);
UnionRoomRecord *CommWireless_GetParentInfo(int netId);
BOOL CommWireless_GetParentProfile(int netId);
u8 *CommWireless_GetRuleset(void);
void CommState_StartAlternateSearch(void);
BOOL CommState_IsAlternateLinked(void);
BOOL CommState_GetJoinResult(void);
void CommState_JoinParent(int netId);
BOOL CommState_IsAlternateSearchReady(void);
void CommState_StopAlternateSearch(void);
void CommState_ReconnectToParent(int netId);
void CommState_JoinRecordMixing(int netId);
void CommState_JoinSpinTrade(int netId);
void CommState_SetMaxEntries(int mode);
BOOL Comm_GetConnectedChildren(void);
BOOL CommState_ReportsErrors(void);
void DrawingSessionLink_RegisterCommands(int a0);
void CommRandom_Init(void *random);
SaveData *FieldSystem_GetSaveData(FieldSystem *fieldSystem);
BOOL FieldSystem_TaskIsRunning(FieldSystem *fieldSystem);
void UnionRoom_SetBeaconActivity(int command);
void UnionRoom_SetOwnChatMessage(struct UnionRoom *unionRoom, MailMessage *message);
void UnionRoom_BroadcastChatMessage(MailMessage *message);
void UnionRoom_ResetTalkReplies(struct UnionRoom *unionRoom);
void UnionRoom_ResetTalk(struct UnionRoom *unionRoom);

struct UnionRoom *UnionRoom_Start(FieldSystem *fieldSystem) {
    struct UnionRoom *unionRoom;
    if (fieldSystem == NULL) {
        GF_AssertFail();
    }
    if (fieldSystem->unionRoom != NULL) {
        return NULL;
    }
    Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_31, 0xA80);
    unionRoom = UnionRoom_New(fieldSystem);
    if (unionRoom == NULL) {
        unionRoom = fieldSystem->unionRoom;
    }
    FieldComm_RegisterCommands(fieldSystem);
    CommState_SetMaxEntries(2);
    UnionRoom_SetState(unionRoom, UnionRoom_WaitWireless, 40);
    return unionRoom;
}

void UnionRoom_Stop(FieldSystem *fieldSystem) {
    struct UnionRoom *unionRoom = fieldSystem->unionRoom;
    if (unionRoom != NULL) {
        UnionRoom_SetState(unionRoom, UnionRoom_StopSearch, 5);
    }
}

struct UnionRoom *UnionRoom_New(FieldSystem *fieldSystem) {
    struct UnionRoom *unionRoom;
    SaveData *saveData;
    if (fieldSystem->unionRoom != NULL) {
        return NULL;
    }
    saveData = FieldSystem_GetSaveData(fieldSystem);
    CommState_StartAlternateSearch();
    unionRoom = Heap_Alloc(HEAP_ID_31, sizeof(struct UnionRoom));
    MI_CpuFill8(unionRoom, 0, sizeof(struct UnionRoom));
    unionRoom->state = NULL;
    unionRoom->timer = 40;
    unionRoom->task = SysTask_CreateOnMainQueue((SysTaskFunc)Task_UnionRoom, unionRoom, 10);
    unionRoom->fieldSystem = fieldSystem;
    unionRoom->saveData = saveData;
    unionRoom->profile = Save_PlayerData_GetProfile(saveData);
    UnionRoom_ResetTalk(unionRoom);
    CommRandom_Init(unionRoom->random);
    return unionRoom;
}

void UnionRoom_WaitWireless(struct UnionRoom *unionRoom) {
    MailMessage message;
    if (CommWireless_Exists() != 0) {
        MailMsg_Init_Default(&message);
        UnionRoom_BroadcastChatMessage(&message);
        UnionRoom_SetOwnChatMessage(unionRoom, &message);
        UnionRoom_SetState(unionRoom, UnionRoom_Search, 40);
    }
}

void UnionRoom_Search(struct UnionRoom *unionRoom) {
    if (CommState_IsAlternateLinked()) {
        _021D41CC.waitFrames = 0;
        UnionRoom_SetState(unionRoom, UnionRoom_ParentLinked, 0);
        return;
    }
    if (unionRoom->connectRequest != 0) {
        unionRoom->unk28 = 2;
        if (unionRoom->connectRequest == 1) {
            if (unionRoom->talkReply == 5) {
                CommState_JoinRecordMixing(unionRoom->partnerNetId);
            } else if (unionRoom->talkReply == 6) {
                CommState_JoinSpinTrade(unionRoom->partnerNetId);
            } else {
                CommState_JoinParent(unionRoom->partnerNetId);
            }
        } else if (unionRoom->connectRequest == 2) {
            DrawingSessionLink_RegisterCommands(0);
            CommState_ReconnectToParent(unionRoom->partnerNetId);
        }
        UnionRoom_SetState(unionRoom, UnionRoom_WaitJoin, 12);
    }
}

void UnionRoom_WaitSearchReady(struct UnionRoom *unionRoom) {
    if (CommState_IsAlternateSearchReady() == TRUE) {
        FieldComm_RegisterCommands(unionRoom->fieldSystem);
        UnionRoom_SetState(unionRoom, UnionRoom_Search, 2);
    }
}

BOOL UnionRoom_AnyOtherPlayerPresent(void) {
    int i;
    int count = 0;
    i = 1;
    for (; i < 5; i++) {
        if (CommPlayerInfo_GetProfile(i) != NULL) {
            count++;
        }
    }
    return count >= 1;
}

void UnionRoom_ParentLinked(struct UnionRoom *unionRoom) {
    u8 *profile;
    if (unionRoom->timer > 0) {
        unionRoom->timer -= 1;
        return;
    }

    _021D41CC.waitFrames++;
    profile = CommWireless_GetRuleset();
    if (Comm_GetConnectedChildren() != 0 && UnionRoom_AnyOtherPlayerPresent() == TRUE && profile[0x1C] != 4) {
        CommPlayerInfo_SendLocal();
        CommState_SetErrorWatch(1, 1);
        UnionRoom_SetBeaconActivity(11);
        UnionRoom_SetState(unionRoom, UnionRoom_ParentInSession, 0);
    }

    if (CommState_IsAlternateLinked() == 0) {
        CommState_ResumeAlternateSearch();
        UnionRoom_ResetTalk(unionRoom);
        UnionRoom_SetBeaconActivity(0);
        UnionRoom_SetState(unionRoom, UnionRoom_WaitSearchReady, 2);
    }
}

void UnionRoom_ParentInSession(struct UnionRoom *unionRoom) {
    if (CommState_ReportsErrors() != 0) {
        if (Comm_GetConnectedChildren() == 0) {
            return;
        }
    }
    if (Comm_GetConnectedChildren() == 0) {
        CommState_ResumeAlternateSearch();
        UnionRoom_ResetTalk(unionRoom);
        UnionRoom_SetBeaconActivity(0);
        UnionRoom_SetState(unionRoom, UnionRoom_WaitSearchReady, 2);
    }
}

void UnionRoom_SetState(struct UnionRoom *unionRoom, UnionRoomStateFunc state, int timer) {
    unionRoom->state = state;
    unionRoom->timer = timer;
}

void Task_UnionRoom(SysTask *task, struct UnionRoom *unionRoom) {
    struct {
        struct UnionRoom *unionRoom;
    } local;

    s32 i;

    local.unionRoom = unionRoom;
    if (local.unionRoom == NULL) {
        SysTask_Destroy(task);
        return;
    }
    for (i = 0; i < UNION_ROOM_MAX_RECORDS; i++) {
        unionRoom->records[i] = CommWireless_GetParentInfo(i);
        _021D41D8[i] = unionRoom->records[i];
    }
    if (local.unionRoom->state != NULL) {
        local.unionRoom->state(local.unionRoom);
    }
}

void UnionRoom_StopSearch(struct UnionRoom *unionRoom) {
    int timer = unionRoom->timer;
    if (timer != 0) {
        unionRoom->timer = timer - 1;
        return;
    }
    CommState_StopAlternateSearch();
    UnionRoom_SetState(unionRoom, UnionRoom_WaitCommEnd, 0);
}

void UnionRoom_WaitCommEnd(struct UnionRoom *unionRoom) {
    if (Comm_IsActive() == 0) {
        UnionRoom_Delete(unionRoom);
    }
}

void UnionRoom_WaitJoin(struct UnionRoom *unionRoom) {
    if (CommState_GetJoinResult() == TRUE) {
        CommPlayerInfo_SendLocal();
        UnionRoom_SetState(unionRoom, UnionRoom_WaitJoinedProfile, 3);
        return;
    }
    if (Comm_GetConnectedChildren() != 0) {
        unionRoom->connectRequest = 0;
        unionRoom->joinStatus = 3;
        UnionRoom_SetState(unionRoom, UnionRoom_ParentLinked, 0);
    }
    if (CommState_GetJoinResult() != 0) {
        UnionRoom_SetState(unionRoom, UnionRoom_WaitFieldTask, 2);
        unionRoom->unk24 = 0;
        unionRoom->joinStatus = 2;
        unionRoom->connectRequest = 0;
        unionRoom->partnerCancelled = 0;
    }
}

void UnionRoom_WaitFieldTask(struct UnionRoom *unionRoom) {
    if (FieldSystem_TaskIsRunning(unionRoom->fieldSystem) == 0) {
        CommState_ResumeAlternateSearch();
        UnionRoom_ResetTalk(unionRoom);
        UnionRoom_SetBeaconActivity(0);
        UnionRoom_SetState(unionRoom, UnionRoom_WaitSearchReady, 2);
    }
}

void UnionRoom_WaitJoinedProfile(struct UnionRoom *unionRoom) {
    if (CommState_GetJoinResult() == TRUE) {
        if (CommPlayerInfo_GetProfile(Comm_GetLocalNetId()) != NULL) {
            unionRoom->connectRequest = 0;
            unionRoom->joinStatus = 1;
            unionRoom->partnerCancelled = 0;
            CommState_SetErrorWatch(1, 1);
            UnionRoom_SetState(unionRoom, UnionRoom_ChildInSession, 3);
        }
    } else if (CommState_GetJoinResult() == 0) {
        CommState_ResumeAlternateSearch();
        UnionRoom_ResetTalk(unionRoom);
        UnionRoom_SetState(unionRoom, UnionRoom_WaitSearchReady, 2);
        unionRoom->unk24 = 0;
        unionRoom->joinStatus = 2;
        unionRoom->connectRequest = 0;
        unionRoom->partnerCancelled = 0;
    }
}

void UnionRoom_ChildInSession(struct UnionRoom *unionRoom) {
    if (CommState_GetJoinResult() == 0) {
        CommState_ResumeAlternateSearch();
        UnionRoom_ResetTalk(unionRoom);
        UnionRoom_SetState(unionRoom, UnionRoom_WaitSearchReady, 2);
    }
}

void UnionRoom_Delete(struct UnionRoom *unionRoom) {
    if (unionRoom != NULL) {
        SysTask_Destroy(unionRoom->task);
        Heap_Free(unionRoom);
        Heap_Destroy(HEAP_ID_31);
    }
}

FieldSystem *UnionRoom_GetFieldSystem(struct UnionRoom *unionRoom) {
    return unionRoom->fieldSystem;
}

UnionRoomRecord *UnionRoom_GetRecord(struct UnionRoom *unionRoom, int netId) {
    return unionRoom->records[netId];
}

#pragma force_active on

// What the player standing at map object objId is doing (record + 0x80: activity, kind at +0x1C).
int UnionRoom_GetPlayerActivityType(struct UnionRoom *unionRoom, int objId) {
    UnionRoomRecord *record;
    UnionRoomRecordInfo *info;
    BOOL present = CommWireless_GetParentProfile(objId - 1);
    UnionRoom_ResetTalkReplies(unionRoom);
    if (!present) {
        return 5;
    }
    record = unionRoom->records[objId - 1];
    if (record == NULL) {
        return 5;
    }
    info = (UnionRoomRecordInfo *)((u8 *)record + 0x50);
    _021D41CC.lastActivity = (UnionRoomActivity *)((u8 *)info + 0x30);
    switch (_021D41CC.lastActivity->kind) {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 3;
    case 3:
    case 13:
        return 4;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
        goto unavailable;
    default:
        goto unsupported;
    }
unavailable:
    return 5;
unsupported:
    return 5;
}

int UnionRoom_RequestJoinPlayer(struct UnionRoom *unionRoom, int objId, int kind) {
    u8 *record;
    UnionRoomActivity *activity;
    objId--;
    record = (u8 *)unionRoom->records[objId];
    if (record == NULL) {
        return 5;
    }
    activity = &((UnionRoomRecordInfo *)(record + 0x50))->activity;
    _021D41CC.lastActivity = activity;
    switch (activity->kind) {
    case 2:
        if (kind != 3) {
            return 5;
        }
        unionRoom->talkReply = 5;
        unionRoom->partnerNetId = objId;
        unionRoom->connectRequest = 1;
        unionRoom->unk24 = 0;
        unionRoom->joinStatus = 0;
        return 1;
    case 0:
        if (kind != 1) {
            return 5;
        }
        unionRoom->partnerNetId = objId;
        unionRoom->connectRequest = 1;
        unionRoom->unk24 = 0;
        unionRoom->joinStatus = 0;
        return 1;
    case 1:
        if (kind != 2) {
            return 5;
        }
        unionRoom->partnerNetId = objId;
        unionRoom->connectRequest = 2;
        unionRoom->unk24 = 0;
        unionRoom->joinStatus = 0;
        return 1;
    case 3:
    case 13:
        if (kind != 4) {
            return 5;
        }
        unionRoom->talkReply = 6;
        unionRoom->partnerNetId = objId;
        unionRoom->connectRequest = 1;
        unionRoom->joinStatus = 0;
        return 1;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
        return 5;
    case 9:
    case 10:
    case 12:
    default:
        return 5;
    }
}

#pragma force_active off

u32 UnionRoom_GetJoinStatus(struct UnionRoom *unionRoom) {
    return unionRoom->joinStatus;
}

int UnionRoom_GetTalkChoice(struct UnionRoom *unionRoom) {
    if (unionRoom->partnerCancelled != 0) {
        return 7;
    }
    if (Comm_CountConnected() < 2) {
        return 7;
    }
    if (Comm_GetLocalNetId() == 0) {
        if (CommState_IsAlternateLinked() == TRUE) {
            return unionRoom->talkChoice;
        }
    } else if (CommState_GetJoinResult() == TRUE) {
        return unionRoom->talkChoice;
    }
    return 7;
}

int UnionRoom_GetTalkReply(struct UnionRoom *unionRoom) {
    if (CommState_IsAlternateLinked() == TRUE) {
        return unionRoom->talkReply;
    }
    return 7;
}

void UnionRoom_SendTalkResponse(struct UnionRoom *unionRoom, int mode, int value) {
    u8 data[3];

    data[2] = value;
    switch (mode) {
    case 0:
        if (unionRoom->partnerCancelled == 0) {
            unionRoom->unk34 = data[2];
            Comm_SendCommand(0x63, &data[2], 1);
        }
        break;
    case 1:
        if (value == 0) {
            data[1] = unionRoom->talkReply;
            Comm_SendParentCommand(0x67, &data[1], 1);
            unionRoom->unk3C = value;
        } else {
            data[0] = 7;
            Comm_SendParentCommand(0x67, &data[0], 1);
            unionRoom->unk3C = value;
        }
        break;
    }
}

// Communication command callbacks (comm command table in unk_02091564.c).
void sub_0205A408(void) {
}

void sub_0205A40C(void) {
}

void UnionRoom_RecvRestartSearch(int netId, int size, void *data, FieldSystem *fieldSystem) {
    UnionRoom_SetState(fieldSystem->unionRoom, UnionRoom_Search, 2);
    UnionRoom_ResetTalk(fieldSystem->unionRoom);
}

void UnionRoom_RecvTalkReply(int netId, int size, const u8 *data, FieldSystem *fieldSystem) {
    struct UnionRoom *unionRoom = fieldSystem->unionRoom;
    if (unionRoom->partnerCancelled == 0) {
        unionRoom->talkReply = *data;
        _021D41CC.lastChoice = *data;
    }
}
