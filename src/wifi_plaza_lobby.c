#include "global.h"

#include "wifi_plaza_internal.h"

void *WiFiPlazaWork_GetOwnPlayerProfile(WiFiPlazaWork *work) {
    return work->ownProfileData.profile;
}

int WiFiPlazaWork_IsSpecialVisitor(WiFiPlazaWork *work, int slot) {
    return WiFiPlaza_GetSlotFlag(work->specialVisitors, slot);
}

void WiFiPlazaWork_GetSpecialVisitors(WiFiPlazaWork *work, void *dest) {
    MI_CpuCopy8(work->specialVisitors, dest, sizeof(work->specialVisitors));
}

int WiFiPlazaWork_HasWordSet(WiFiPlazaWork *work, int slot) {
    return WiFiPlazaWordSet_HasWords(&work->wordSet, slot);
}

u16 *WiFiPlazaWork_GetWordSet(WiFiPlazaWork *work, int slot) {
    return WiFiPlazaWordSet_GetWords(&work->wordSet, slot);
}

u32 WiFiPlaza_GetChannelPlayerCount(void) {
    u32 pair[2];
    WiFiPlazaLobbyClient_GetChannelPlayerList(pair);
    return pair[0];
}

int WiFiPlaza_ChannelSlotToSlot(void *unused, int channelSlot) {
    int index = WiFiPlazaLobbyClient_ChannelSlotToId(channelSlot);
    int result = -1;
    if (index != -1) {
        result = WiFiPlazaLobbyClient_IdToSlot(index);
    }
    return result;
}

void WiFiPlaza_SendChatNews(void *unused, u32 slot0, u32 slot1) {
    WiFiPlazaLobbyMessage msg;
    u32 *dst = (u32 *)&msg;
    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
    dst[4] = 0;
    msg.type = 0;
    msg.ids[0] = WiFiPlazaLobbyClient_SlotToId(slot0);
    msg.ids[1] = WiFiPlazaLobbyClient_SlotToId(slot1);
    msg.count = 2;
    WiFiPlazaLobbyClient_SendToLobby(4, &msg, sizeof(msg));
}

void WiFiPlaza_SendTapToyTradeNews(void *unused, u32 slot0, u32 slot1, u8 value) {
    WiFiPlazaLobbyMessage msg = { 0 };
    msg.type = 1;
    msg.ids[0] = WiFiPlazaLobbyClient_SlotToId(slot1);
    msg.ids[1] = WiFiPlazaLobbyClient_SlotToId(slot0);
    msg.count = 2;
    msg.unk_12 = value;
    WiFiPlazaLobbyClient_SendToLobby(4, &msg, sizeof(msg));
}

#pragma force_active on

void WiFiPlaza_SendActivityNews(void *unused, int selector, int count, int slot0, int slot1, int slot2, int slot3, int flag) {
    WiFiPlazaLobbyMessage msg = { 0 };
    u32 type;
    (void)unused;
    switch ((u32)selector) {
    case 0:
    case 1:
    case 2:
        type = 2;
        break;
    case 3:
    case 4:
        type = 3;
        break;
    case 5:
        type = 4;
        break;
    case 6:
        type = 5;
        break;
    default:
        return;
    }
    msg.type = type;
    msg.ids[0] = WiFiPlazaLobbyClient_SlotToId(slot0);
    msg.ids[1] = WiFiPlazaLobbyClient_SlotToId(slot1);
    msg.ids[2] = WiFiPlazaLobbyClient_SlotToId(slot2);
    msg.ids[3] = WiFiPlazaLobbyClient_SlotToId(slot3);
    msg.count = count;
    msg.unk_13_0 = selector;
    msg.unk_13_7 = flag;
    WiFiPlazaLobbyClient_SendToLobby(4, &msg, 20);
}

#pragma force_active reset

void WiFiPlaza_SendMinigameResultNews(void *unused, int mode, u8 kind, u32 slot0, u32 slot1, u32 slot2, u32 slot3) {
    WiFiPlazaLobbyMessage msg;
    u32 *dst = (u32 *)&msg;
    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
    dst[4] = 0;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        msg.type = 8;
        msg.ids[0] = WiFiPlazaLobbyClient_SlotToId(slot0);
        msg.ids[1] = WiFiPlazaLobbyClient_SlotToId(slot1);
        msg.ids[2] = WiFiPlazaLobbyClient_SlotToId(slot2);
        msg.ids[3] = WiFiPlazaLobbyClient_SlotToId(slot3);
        msg.count = kind;
        msg.unk_13_0 = mode;
        msg.unk_13_7 = 0;
        WiFiPlazaLobbyClient_SendToLobby(4, &msg, sizeof(msg));
        break;
    }
}

int WiFiPlazaWork_GetScheduleValue3(WiFiPlazaWork *work) {
    int value = WiFiPlazaLobbyClient_GetScheduleValue(3);
    if ((u32)value >= 5) {
        value = 0;
    }
    return value;
}

int WiFiPlazaWork_GetScheduleValue2(WiFiPlazaWork *work) {
    int value = WiFiPlazaLobbyClient_GetScheduleValue(2);
    if ((u32)value >= 5) {
        value = 0;
    }
    return value;
}

int WiFiPlazaWork_GetOpenDuration(WiFiPlazaWork *work) {
    return WiFiPlazaLobbyClient_GetScheduleValue(5) + 30;
}

u8 WiFiPlazaWork_HasPlayedTapToy(WiFiPlazaWork *work, u32 index) {
    if (index >= 20) {
        GF_AssertFail();
    }
    return work->unk_1e8[index];
}

void WiFiPlazaWork_SendTapToyPlayed(void *unused, u32 value) {
    u32 data[1];
    data[0] = value;
    WiFiPlazaLobbyClient_SendToLobby(5, data, sizeof(data));
}

int WiFiPlazaWork_GetGroupState(WiFiPlazaWork *work, int group) {
    return WiFiPlazaGroups_GetState(&work->groups, group);
}

int WiFiPlazaWork_GetGroupCounter(WiFiPlazaWork *work, int group) {
    return WiFiPlazaGroups_GetCounter(&work->groups, group);
}

int WiFiPlazaWork_GetGroupDuration(void) {
    return 1200;
}

int WiFiPlazaWork_FindGroupPlace(WiFiPlazaWork *work, int id) {
    return WiFiPlazaGroups_FindPlace(&work->groups, id);
}

void WiFiPlazaWork_JoinGroup(WiFiPlazaWork *work, int id, int value) {
    WiFiPlazaGroups_Join(&work->groups, id, value);
}

void WiFiPlazaWork_LeaveGroup(WiFiPlazaWork *work, int id) {
    WiFiPlazaGroups_Leave(&work->groups, id);
}

void WiFiPlazaWork_RequestGroupAction(WiFiPlazaWork *work, int id) {
    WiFiPlazaGroups_RequestAction(&work->groups, id);
}

void WiFiPlazaWork_GetGroupActionRequest(WiFiPlazaWork *work, int place) {
    WiFiPlazaGroups_GetActionRequest(&work->groups, place);
}

void WiFiPlazaWork_ClearGroupActionRequest(WiFiPlazaWork *work, int place) {
    WiFiPlazaGroups_ClearActionRequest(&work->groups, place);
}

void WiFiPlaza_SplitGroupPlace(u32 value, u32 *quotient, u32 *remainder) {
    *quotient = value / 3;
    *remainder = value % 3;
}

void WiFiPlazaWork_SetGroupMarkerPassed(WiFiPlazaWork *work, int place, int value) {
    WiFiPlazaGroups_SetMarkerPassed(&work->groups, place, value);
}

void WiFiPlazaWork_GetGroupMarkerCount(WiFiPlazaWork *work, int place) {
    WiFiPlazaGroups_GetMarkerCount(&work->groups, place);
}

void WiFiPlazaWork_SendJoinGroup(void) {
    u32 data;
    WiFiPlazaLobbyClient_SendToLobby(6, &data, 4);
}

void WiFiPlazaWork_SendGroupAction(void) {
    u32 data;
    WiFiPlazaLobbyClient_SendToLobby(7, &data, 4);
}

void WiFiPlazaWork_ResetTalk(WiFiPlazaWork *work) {
    WiFiPlazaTalkSession_Reset(&work->talk);
}

#pragma force_active on

int WiFiPlazaWork_StartTalk(WiFiPlazaWork *work, int slot) {
    if (work->talk.role) {
        return 0;
    }
    if (WiFiPlazaPlayer_GetActivity(WiFiPlazaWork_GetPlayer(work, slot)) != 1) {
        return 0;
    }
    work->talk.partnerSlot = slot;
    work->talk.state = 1;
    work->talk.role = 1;
    WiFiPlazaTalkSession_SetMessage(&work->talk, 0);
    WiFiPlazaLobbyClient_SendToPlayer(0, WiFiPlazaLobbyClient_SlotToId(slot), &work->talk.sendMessage, 4);
    WiFiPlazaTalkSession_StartTimeout(&work->talk);
    return 1;
}

void WiFiPlazaWork_AdvanceTalk(WiFiPlazaWork *work, int value) {
    int flag = 0;
    if (work->talk.role == 1) {
        if (work->talk.state != 2) {
            flag = 1;
        }
    } else if (work->talk.role == 2) {
        if (work->talk.state != 3) {
            flag = 1;
        }
    }
    if (flag) {
        WiFiPlazaWork_SendTalkEnd(work, WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot));
        return;
    }
    switch (work->talk.role) {
    case 1:
        work->talk.state = 3;
        break;
    case 2:
        work->talk.state = 2;
        break;
    default:
        WiFiPlazaWork_SendTalkEnd(work, WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot));
        return;
    }
    WiFiPlazaTalkSession_SetMessage(&work->talk, value);
    WiFiPlazaLobbyClient_SendToPlayer(2, WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot), &work->talk.sendMessage, 4);
    WiFiPlazaTalkSession_StartTimeout(&work->talk);
}

#pragma force_active reset

void WiFiPlazaWork_CancelTalkRequest(WiFiPlazaWork *work) {
    int id;

    if (work->talk.state != 0 && work->talk.role == 1) {
        work->talk.sendState = 0;
        id = WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot);
        WiFiPlazaLobbyClient_SendToPlayer(3, id, &work->talk.sendMessage, 4);
        WiFiPlazaTalkSession_Reset(&work->talk);
    }
}

void WiFiPlazaWork_EndTalk(WiFiPlazaWork *work) {
    int id;

    if (work->talk.state != 0) {
        id = WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot);
        work->talk.state = 4;
        work->talk.sendState = 4;
        WiFiPlazaLobbyClient_SendToPlayer(2, id, &work->talk.sendMessage, 4);
    }
}

BOOL WiFiPlazaWork_IsTalking(WiFiPlazaWork *work) {
    return work->talk.role != 0;
}

BOOL WiFiPlazaWork_WasAskedToTalk(WiFiPlazaWork *work) {
    return work->talk.role == 2;
}

u16 WiFiPlazaWork_GetTalkPartner(WiFiPlazaWork *work) {
    return work->talk.partnerSlot;
}

u8 WiFiPlazaWork_IsFirstTalk(WiFiPlazaWork *work) {
    return work->talk.isFirstTalk;
}

u8 WiFiPlazaWork_GetTalkState(WiFiPlazaWork *work) {
    return work->talk.state;
}

u16 WiFiPlazaWork_GetTalkPartnerMessage(WiFiPlazaWork *work) {
    return work->talk.partnerMessage;
}

BOOL WiFiPlazaWork_CanTalkWith(WiFiPlazaWork *work, u32 index) {
    GF_ASSERT(index < 20);
    if (work->talk.talkCounts[index] >= 6) {
        return FALSE;
    }
    return TRUE;
}

BOOL WiFiPlazaWork_IsTalkOver(WiFiPlazaWork *work) {
    if (work->talk.state == 4) {
        return TRUE;
    }
    if (WiFiPlazaTalkSession_IsWithinTimeout(&work->talk) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaWork_GetTalkTimeoutCounter(WiFiPlazaWork *work) {
    WiFiPlazaTalkSession_GetTimeoutCounter(&work->talk);
}

void WiFiPlazaWork_SetTalkField07(WiFiPlazaWork *work) {
    work->talk.unk_07 = 1;
}

u8 WiFiPlazaWork_GetTalkField07(WiFiPlazaWork *work) {
    return work->talk.unk_07;
}
