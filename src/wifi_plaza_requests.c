#include "global.h"

#include "wifi_plaza_internal.h"

// Clears the 20 bytes one at a time.
void ov45_0222BD30(volatile u8 *a0) {
    u32 count = 20;
    do {
        *a0++ = 0;
        count--;
    } while (count != 0);
}

void WiFiPlazaTalkSession_Init(WiFiPlazaTalkSession *a0) {
    memset(a0, 0, sizeof(WiFiPlazaTalkSession));
}

void WiFiPlazaTalkSession_Reset(WiFiPlazaTalkSession *a0) {
    a0->unk_04 = 0;
    a0->unk_06 = 0;
    a0->unk_0a = 0;
    WiFiPlazaTalkSession_StopTimeout(a0);
}

void WiFiPlazaTalkSession_Abort(WiFiPlazaTalkSession *a0) {
    a0->unk_04 = 0;
    a0->unk_06 = 4;
    WiFiPlazaTalkSession_StopTimeout(a0);
    a0->unk_0a = 0;
}

BOOL WiFiPlazaTalkSession_HasTalkedWith(WiFiPlazaTalkSession *a0, u32 slot) {
    GF_ASSERT(slot < 20);
    return (a0->unk_00 & (1 << slot)) != 0;
}

void WiFiPlazaTalkSession_SetTalkedWith(WiFiPlazaTalkSession *a0, u32 slot) {
    GF_ASSERT(slot < 20);
    a0->unk_00 |= 1 << slot;
}

void WiFiPlazaTalkSession_ClearTalkedWith(WiFiPlazaTalkSession *a0, u32 slot) {
    GF_ASSERT(slot < 20);
    a0->unk_00 &= ~(1 << slot);
}

void WiFiPlazaTalkSession_IncTalkCount(WiFiPlazaTalkSession *a0, u32 slot) {
    int value;

    GF_ASSERT(slot < 20);
    value = a0->unk_0c[slot] + 1;
    if (value <= 6) {
        a0->unk_0c[slot] = value;
    }
}

void WiFiPlazaTalkSession_ResetTalkCount(WiFiPlazaTalkSession *a0, u32 slot) {
    GF_ASSERT(slot < 20);
    a0->unk_0c[slot] = 0;
}

void WiFiPlazaTalkSession_SetMessage(WiFiPlazaTalkSession *a0, int value) {
    a0->unk_22 = a0->unk_06;
    a0->unk_20 = value;
}

void WiFiPlazaWork_SendTalkAnswer(WiFiPlazaWork *work, int id, u16 value) {
    work->unk_1c0.unk_22 = 2;
    work->unk_1c0.unk_20 = value;
    WiFiPlazaLobbyClient_SendToPlayer(1, id, &work->unk_1c0.unk_20, 4);
}

void WiFiPlazaWork_SendTalkEnd(WiFiPlazaWork *work, int id) {
    work->unk_1c0.unk_06 = 4;
    work->unk_1c0.unk_22 = 4;
    WiFiPlazaLobbyClient_SendToPlayer(2, id, &work->unk_1c0.unk_20, 4);
}

void WiFiPlazaTalkSession_StartTimeout(WiFiPlazaTalkSession *a0) {
    a0->unk_24 = 1;
    a0->unk_26 = 0;
}

void WiFiPlazaTalkSession_StopTimeout(WiFiPlazaTalkSession *a0) {
    a0->unk_24 = 0;
    a0->unk_26 = 0;
}

// Counts unk_26 up to 900 while unk_24 is set.
void WiFiPlazaTalkSession_UpdateTimeout(WiFiPlazaTalkSession *a0) {
    if (a0->unk_24 == 1) {
        int value = a0->unk_26 + 1;
        if (value <= 900) {
            a0->unk_26 = value;
        }
    }
}

BOOL WiFiPlazaTalkSession_IsWithinTimeout(WiFiPlazaTalkSession *a0) {
    if (a0->unk_24 == 0) {
        return TRUE;
    }
    return a0->unk_26 < 900;
}

s16 WiFiPlazaTalkSession_GetTimeoutCounter(WiFiPlazaTalkSession *a0) {
    return a0->unk_26;
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

void ov45_0222C36C(void) {
}

void WiFiPlazaWork_AddLobbyEventNotice(WiFiPlazaWork *work, int which) {
    WiFiPlazaLobbyNotice notice;
    notice.time = &work->unk_d4;
    notice.which = which;
    WiFiPlazaNews_AddLobbyEventNotice(work->news, &notice);
}

void ov45_0222C388(WiFiPlazaUnk20C *a0) {
    memset(a0, 0, sizeof(WiFiPlazaUnk20C));
    MIi_CpuClear32(0xFFFFFFFF, (u32 *)a0->ids, sizeof(a0->ids));
}

void ov45_0222C3A8(WiFiPlazaUnk20C *a0) {
    a0->active = 1;
}
