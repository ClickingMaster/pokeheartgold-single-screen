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

void ov45_0222BD40(WiFiPlazaUnk1C0 *a0) {
    memset(a0, 0, sizeof(WiFiPlazaUnk1C0));
}

void ov45_0222BD4C(WiFiPlazaUnk1C0 *a0) {
    a0->unk_04 = 0;
    a0->unk_06 = 0;
    a0->unk_0a = 0;
    ov45_0222BE54(a0);
}

void ov45_0222BD5C(WiFiPlazaUnk1C0 *a0) {
    a0->unk_04 = 0;
    a0->unk_06 = 4;
    ov45_0222BE54(a0);
    a0->unk_0a = 0;
}

BOOL ov45_0222BD74(WiFiPlazaUnk1C0 *a0, u32 slot) {
    GF_ASSERT(slot < 20);
    return (a0->unk_00 & (1 << slot)) != 0;
}

void ov45_0222BD94(WiFiPlazaUnk1C0 *a0, u32 slot) {
    GF_ASSERT(slot < 20);
    a0->unk_00 |= 1 << slot;
}

void ov45_0222BDB0(WiFiPlazaUnk1C0 *a0, u32 slot) {
    GF_ASSERT(slot < 20);
    a0->unk_00 &= ~(1 << slot);
}

void ov45_0222BDCC(WiFiPlazaUnk1C0 *a0, u32 slot) {
    int value;

    GF_ASSERT(slot < 20);
    value = a0->unk_0c[slot] + 1;
    if (value <= 6) {
        a0->unk_0c[slot] = value;
    }
}

void ov45_0222BDE8(WiFiPlazaUnk1C0 *a0, u32 slot) {
    GF_ASSERT(slot < 20);
    a0->unk_0c[slot] = 0;
}

void ov45_0222BE00(WiFiPlazaUnk1C0 *a0, int value) {
    a0->unk_22 = a0->unk_06;
    a0->unk_20 = value;
}

void ov45_0222BE08(WiFiPlazaWork *work, int id, u16 value) {
    work->unk_1c0.unk_22 = 2;
    work->unk_1c0.unk_20 = value;
    WiFiPlazaLobbyClient_SendToPlayer(1, id, &work->unk_1c0.unk_20, 4);
}

void ov45_0222BE28(WiFiPlazaWork *work, int id) {
    work->unk_1c0.unk_06 = 4;
    work->unk_1c0.unk_22 = 4;
    WiFiPlazaLobbyClient_SendToPlayer(2, id, &work->unk_1c0.unk_20, 4);
}

void ov45_0222BE48(WiFiPlazaUnk1C0 *a0) {
    a0->unk_24 = 1;
    a0->unk_26 = 0;
}

void ov45_0222BE54(WiFiPlazaUnk1C0 *a0) {
    a0->unk_24 = 0;
    a0->unk_26 = 0;
}

// Counts unk_26 up to 900 while unk_24 is set.
void ov45_0222BE5C(WiFiPlazaUnk1C0 *a0) {
    if (a0->unk_24 == 1) {
        int value = a0->unk_26 + 1;
        if (value <= 900) {
            a0->unk_26 = value;
        }
    }
}

BOOL ov45_0222BE74(WiFiPlazaUnk1C0 *a0) {
    if (a0->unk_24 == 0) {
        return TRUE;
    }
    return a0->unk_26 < 900;
}

s16 ov45_0222BE94(WiFiPlazaUnk1C0 *a0) {
    return a0->unk_26;
}

// Lobby message type 0: two players.
void ov45_0222BE9C(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaPairRequest req;
    int i;
    int slot;
    if (msg->count != 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        slot = ov45_0222EC68(msg->ids[i]);
        if (slot == -1) {
            return;
        }
        WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
    }
    req.profiles[0] = work->profiles[0];
    req.profiles[1] = work->profiles[1];
    req.slots[0] = ov45_0222EC68(msg->ids[0]);
    req.slots[1] = ov45_0222EC68(msg->ids[1]);
    ov45_0222D940(work->news, &req);
}

// Lobby message type 1: two players and an option.
void ov45_0222BF18(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaPairOptionRequest req;
    int i;
    int slot;
    if (msg->count != 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        slot = ov45_0222EC68(msg->ids[i]);
        if (slot == -1) {
            return;
        }
        WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
    }
    req.profiles[0] = work->profiles[0];
    req.profiles[1] = work->profiles[1];
    req.slots[0] = ov45_0222EC68(msg->ids[0]);
    req.slots[1] = ov45_0222EC68(msg->ids[1]);
    req.option = msg->unk_12;
    ov45_0222D990(work->news, &req);
}

// Up to four players; without the flag only the first one gets a profile.
void ov45_0222BF98(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
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
                slot = ov45_0222EC68(msg->ids[i]);
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
                slot = ov45_0222EC68(msg->ids[i]);
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
    req.slots[0] = ov45_0222EC68(msg->ids[0]);
    req.slots[1] = ov45_0222EC68(msg->ids[1]);
    req.slots[2] = ov45_0222EC68(msg->ids[2]);
    req.slots[3] = ov45_0222EC68(msg->ids[3]);
    req.flag = msg->unk_13_7;
    ov45_0222D9EC(work->news, &req);
}

// Up to eight players; only the first one gets a profile.
void ov45_0222C0CC(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaSingleRequest req;
    int slot;
    if (msg->count > 8 || msg->count == 0) {
        return;
    }
    slot = ov45_0222EC68(msg->ids[0]);
    if (slot == -1) {
        return;
    }
    WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[0], work->heapID);
    req.option = msg->unk_13_0;
    req.count = msg->count;
    req.profile = work->profiles[0];
    req.slot = ov45_0222EC68(msg->ids[0]);
    ov45_0222DA80(work->news, &req);
}

void ov45_0222C130(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = ov45_0222EC68(msg->ids[i]);
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
    req.slots[0] = ov45_0222EC68(msg->ids[0]);
    req.slots[1] = ov45_0222EC68(msg->ids[1]);
    req.slots[2] = ov45_0222EC68(msg->ids[2]);
    req.slots[3] = ov45_0222EC68(msg->ids[3]);
    ov45_0222DAE0(work->news, &req);
}

void ov45_0222C1EC(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = ov45_0222EC68(msg->ids[i]);
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
    req.slots[0] = ov45_0222EC68(msg->ids[0]);
    req.slots[1] = ov45_0222EC68(msg->ids[1]);
    req.slots[2] = ov45_0222EC68(msg->ids[2]);
    req.slots[3] = ov45_0222EC68(msg->ids[3]);
    ov45_0222DB3C(work->news, &req);
}

void ov45_0222C2A8(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupOptionRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = ov45_0222EC68(msg->ids[i]);
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
    req.slots[0] = ov45_0222EC68(msg->ids[0]);
    req.slots[1] = ov45_0222EC68(msg->ids[1]);
    req.slots[2] = ov45_0222EC68(msg->ids[2]);
    req.slots[3] = ov45_0222EC68(msg->ids[3]);
    ov45_0222DC64(work->news, &req);
}

void ov45_0222C36C(void) {
}

void ov45_0222C370(WiFiPlazaWork *work, int which) {
    WiFiPlazaLobbyNotice notice;
    notice.time = &work->unk_d4;
    notice.which = which;
    ov45_0222DB98(work->news, &notice);
}

void ov45_0222C388(WiFiPlazaUnk20C *a0) {
    memset(a0, 0, sizeof(WiFiPlazaUnk20C));
    MIi_CpuClear32(0xFFFFFFFF, (u32 *)a0->ids, sizeof(a0->ids));
}

void ov45_0222C3A8(WiFiPlazaUnk20C *a0) {
    a0->active = 1;
}
