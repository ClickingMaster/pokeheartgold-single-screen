#include "global.h"

#include "wifi_plaza_internal.h"

// Clears the 20 bytes one at a time.
void WiFiPlaza_ClearTapToyPlayedFlags(volatile u8 *buffer) {
    u32 count = 20;
    do {
        *buffer++ = 0;
        count--;
    } while (count != 0);
}

void WiFiPlazaTalkSession_Init(WiFiPlazaTalkSession *session) {
    memset(session, 0, sizeof(WiFiPlazaTalkSession));
}

void WiFiPlazaTalkSession_Reset(WiFiPlazaTalkSession *session) {
    session->partnerSlot = 0;
    session->state = 0;
    session->role = 0;
    WiFiPlazaTalkSession_StopTimeout(session);
}

void WiFiPlazaTalkSession_Abort(WiFiPlazaTalkSession *session) {
    session->partnerSlot = 0;
    session->state = 4;
    WiFiPlazaTalkSession_StopTimeout(session);
    session->role = 0;
}

BOOL WiFiPlazaTalkSession_HasTalkedWith(WiFiPlazaTalkSession *session, u32 slot) {
    GF_ASSERT(slot < 20);
    return (session->talkedWith & (1 << slot)) != 0;
}

void WiFiPlazaTalkSession_SetTalkedWith(WiFiPlazaTalkSession *session, u32 slot) {
    GF_ASSERT(slot < 20);
    session->talkedWith |= 1 << slot;
}

void WiFiPlazaTalkSession_ClearTalkedWith(WiFiPlazaTalkSession *session, u32 slot) {
    GF_ASSERT(slot < 20);
    session->talkedWith &= ~(1 << slot);
}

void WiFiPlazaTalkSession_IncTalkCount(WiFiPlazaTalkSession *session, u32 slot) {
    int value;

    GF_ASSERT(slot < 20);
    value = session->talkCounts[slot] + 1;
    if (value <= 6) {
        session->talkCounts[slot] = value;
    }
}

void WiFiPlazaTalkSession_ResetTalkCount(WiFiPlazaTalkSession *session, u32 slot) {
    GF_ASSERT(slot < 20);
    session->talkCounts[slot] = 0;
}

void WiFiPlazaTalkSession_SetMessage(WiFiPlazaTalkSession *session, int value) {
    session->sendState = session->state;
    session->sendMessage = value;
}

void WiFiPlazaWork_SendTalkAnswer(WiFiPlazaWork *work, int id, u16 value) {
    work->talk.sendState = 2;
    work->talk.sendMessage = value;
    WiFiPlazaLobbyClient_SendToPlayer(1, id, &work->talk.sendMessage, 4);
}

void WiFiPlazaWork_SendTalkEnd(WiFiPlazaWork *work, int id) {
    work->talk.state = 4;
    work->talk.sendState = 4;
    WiFiPlazaLobbyClient_SendToPlayer(2, id, &work->talk.sendMessage, 4);
}

void WiFiPlazaTalkSession_StartTimeout(WiFiPlazaTalkSession *session) {
    session->timeoutActive = 1;
    session->timeoutCounter = 0;
}

void WiFiPlazaTalkSession_StopTimeout(WiFiPlazaTalkSession *session) {
    session->timeoutActive = 0;
    session->timeoutCounter = 0;
}

// Counts timeoutCounter up to 900 while timeoutActive is set.
void WiFiPlazaTalkSession_UpdateTimeout(WiFiPlazaTalkSession *session) {
    if (session->timeoutActive == 1) {
        int value = session->timeoutCounter + 1;
        if (value <= 900) {
            session->timeoutCounter = value;
        }
    }
}

BOOL WiFiPlazaTalkSession_IsWithinTimeout(WiFiPlazaTalkSession *session) {
    if (session->timeoutActive == 0) {
        return TRUE;
    }
    return session->timeoutCounter < 900;
}

s16 WiFiPlazaTalkSession_GetTimeoutCounter(WiFiPlazaTalkSession *session) {
    return session->timeoutCounter;
}

// Lobby message type 0: two players.
void WiFiPlaza_HandleChatNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaPairRequest req;
    int i;
    int slot;
    if (msg->count != 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
        if (slot == -1) {
            return;
        }
        WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
    }
    req.profiles[0] = work->profiles[0];
    req.profiles[1] = work->profiles[1];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    WiFiPlazaNews_AddChatEvent(work->news, &req);
}

// Lobby message type 1: two players and an option.
void WiFiPlaza_HandleTapToyTradeNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaPairOptionRequest req;
    int i;
    int slot;
    if (msg->count != 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
        if (slot == -1) {
            return;
        }
        WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
    }
    req.profiles[0] = work->profiles[0];
    req.profiles[1] = work->profiles[1];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.option = msg->unk_12;
    WiFiPlazaNews_AddTapToyTradeEvent(work->news, &req);
}

// Up to four players; without the flag only the first one gets a profile.
void WiFiPlaza_HandleMinigameEntryNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupFlagRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    if ((msg->unk_13_7) == 1) {
        for (i = 0; i < 4; i++) {
            if (i < msg->count) {
                slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
                if (slot == -1) {
                    return;
                }
                WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
                profiles[i] = work->profiles[i];
            } else {
                profiles[i] = NULL;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (i == 0) {
                slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
                if (slot == -1) {
                    return;
                }
                WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
                profiles[i] = work->profiles[i];
            } else {
                profiles[i] = NULL;
            }
        }
    }
    req.option = msg->unk_13_0;
    req.count = msg->count;
    req.profiles[0] = profiles[0];
    req.profiles[1] = profiles[1];
    req.profiles[2] = profiles[2];
    req.profiles[3] = profiles[3];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.slots[2] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[2]);
    req.slots[3] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[3]);
    req.flag = msg->unk_13_7;
    WiFiPlazaNews_AddMinigameEntryEvent(work->news, &req);
}

// Up to eight players; only the first one gets a profile.
void WiFiPlaza_HandleFootprintStampNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaSingleRequest req;
    int slot;
    if (msg->count > 8 || msg->count == 0) {
        return;
    }
    slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    if (slot == -1) {
        return;
    }
    WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[0], work->heapID);
    req.option = msg->unk_13_0;
    req.count = msg->count;
    req.profile = work->profiles[0];
    req.slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    WiFiPlazaNews_AddFootprintStampEvent(work->news, &req);
}

void WiFiPlaza_HandleVisitorProfilesNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
            if (slot == -1) {
                return;
            }
            WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
            profiles[i] = work->profiles[i];
        } else {
            profiles[i] = NULL;
        }
    }
    req.count = msg->count;
    req.profiles[0] = profiles[0];
    req.profiles[1] = profiles[1];
    req.profiles[2] = profiles[2];
    req.profiles[3] = profiles[3];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.slots[2] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[2]);
    req.slots[3] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[3]);
    WiFiPlazaNews_AddVisitorProfilesEvent(work->news, &req);
}

void WiFiPlaza_HandlePlazaNewsNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
            if (slot == -1) {
                return;
            }
            WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
            profiles[i] = work->profiles[i];
        } else {
            profiles[i] = NULL;
        }
    }
    req.count = msg->count;
    req.profiles[0] = profiles[0];
    req.profiles[1] = profiles[1];
    req.profiles[2] = profiles[2];
    req.profiles[3] = profiles[3];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.slots[2] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[2]);
    req.slots[3] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[3]);
    WiFiPlazaNews_AddPlazaNewsEvent(work->news, &req);
}

void WiFiPlaza_HandleMinigameResultNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupOptionRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
            if (slot == -1) {
                return;
            }
            WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
            profiles[i] = work->profiles[i];
        } else {
            profiles[i] = NULL;
        }
    }
    req.option = msg->unk_13_0;
    req.count = msg->count;
    req.profiles[0] = profiles[0];
    req.profiles[1] = profiles[1];
    req.profiles[2] = profiles[2];
    req.profiles[3] = profiles[3];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.slots[2] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[2]);
    req.slots[3] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[3]);
    WiFiPlazaNews_AddMinigameResultEvent(work->news, &req);
}

void WiFiPlaza_HandleUnusedNews(void) {
}

void WiFiPlazaWork_AddLobbyEventNotice(WiFiPlazaWork *work, int which) {
    WiFiPlazaLobbyNotice notice;
    notice.time = &work->currentTime;
    notice.which = which;
    WiFiPlazaNews_AddLobbyEventNotice(work->news, &notice);
}

void WiFiPlazaGroups_Init(WiFiPlazaGroups *groups) {
    memset(groups, 0, sizeof(WiFiPlazaGroups));
    MIi_CpuClear32(0xFFFFFFFF, (u32 *)groups->ids, sizeof(groups->ids));
}

void WiFiPlazaGroups_Start(WiFiPlazaGroups *groups) {
    groups->active = 1;
}
