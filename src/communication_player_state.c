#include "communication_player_state_internal.h"

void CommPlayerInfo_Init(void *save, void *rules) {
    void *profile;
    int i;

    profile = Save_PlayerData_GetProfile(save);
    if (gCommPlayerInfo == NULL) {
        gCommPlayerInfo = Heap_Alloc(15, sizeof(CommPlayerInfo));
        MI_CpuFill8(gCommPlayerInfo, 0, sizeof(CommPlayerInfo));
        for (i = 0; i < 8; i++) {
            gCommPlayerInfo->profiles[i] = &gCommPlayerInfo->entries[i].profile;
            CommPlayerInfo_ClearPlayer(i);
        }
        gCommPlayerInfo->unk3A4 = 0;
        gCommPlayerInfo->unk3A5 = 0;
        gCommPlayerInfo->curIdx = 0;
        gCommPlayerInfo->save = save;
        gCommPlayerInfo->rules = rules;
        PlayerProfile_Copy(profile, gCommPlayerInfo->profiles[0]);
    }
}

void CommPlayerInfo_Free(void) {
    int i;

    if (gCommPlayerInfo != NULL) {
        for (i = 0; i < 8; i++) {
            gCommPlayerInfo->profiles[i] = NULL;
        }
        if (gCommPlayerInfo != NULL) {
            Heap_Free(gCommPlayerInfo);
        }
        gCommPlayerInfo = NULL;
    }
}

BOOL CommPlayerInfo_Exists(void) {
    if (gCommPlayerInfo != NULL) {
        return TRUE;
    }
    return FALSE;
}

void CommPlayerInfo_SendLocal(void) {
    int netId;
    void *friends;
    void *r7;
    void *wifi;
    void *profile;

    netId = Comm_GetLocalNetId();
    friends = Save_FriendGroup_Get(gCommPlayerInfo->save);
    r7 = Save_FriendRoster_Get(gCommPlayerInfo->save);
    wifi = Save_WiFiHistory_Get(gCommPlayerInfo->save);
    profile = gCommPlayerInfo->unk00;
    if (profile == NULL) {
        profile = Save_PlayerData_GetProfile(gCommPlayerInfo->save);
    }
    PlayerProfile_Copy(profile, gCommPlayerInfo->profiles[netId]);
    OS_GetMacAddress(gCommPlayerInfo->entries[netId].mac);
    MI_CpuCopy8(Save_FriendGroup_GetName(friends, 1, 0), gCommPlayerInfo->entries[netId].unk4C, 0x10);
    gCommPlayerInfo->entries[netId].country = WifiHistory_GetPlayerCountry(wifi);
    gCommPlayerInfo->entries[netId].region = WiFiHistory_GetPlayerRegion(wifi);
    gCommPlayerInfo->entries[netId].unk65 = 0;
    DWC_CreateExchangeToken(FriendRoster_GetUserData(r7), gCommPlayerInfo->entries[netId].token);
    MI_CpuFill8(&gCommPlayerInfo->entries[netId], 0, 0x20);
    if (gCommPlayerInfo->rules != NULL) {
        LinkBattleRuleset_Copy(gCommPlayerInfo->rules, &gCommPlayerInfo->entries[netId]);
    }
    Comm_SendCommand(3, &gCommPlayerInfo->entries[netId], sizeof(CommPlayerInfoEntry));
}

int CommPlayerInfo_GetEntrySize(void) {
    return sizeof(CommPlayerInfoEntry);
}

void sub_02034524(void) {
    if (gCommPlayerInfo != NULL) {
        gCommPlayerInfo->unk3A4 = 1;
    }
}

void CommPlayerInfo_RecvEntry(int netId, int unused, void *packet) {
#pragma unused(unused)
    CommPlayerInfoEntry *pkt = packet;

    if (gCommPlayerInfo == NULL) {
        return;
    }
    if (!Comm_IsNetIdConnected((u16)netId)) {
        return;
    }
    MI_CpuCopy8(pkt, &gCommPlayerInfo->entries[pkt->index], sizeof(CommPlayerInfoEntry));
    gCommPlayerInfo->curIdx = pkt->index;
    if (PlayerProfile_IsNameEmpty(gCommPlayerInfo->profiles[gCommPlayerInfo->curIdx]) == TRUE) {
        return;
    }
    if (gCommPlayerInfo->status[gCommPlayerInfo->curIdx] >= 2) {
        return;
    }
    gCommPlayerInfo->status[gCommPlayerInfo->curIdx] = 1;
    if (gCommPlayerInfo->curIdx != Comm_GetLocalNetId()) {
        return;
    }
    gCommPlayerInfo->status[gCommPlayerInfo->curIdx] = 3;
}

void CommPlayerInfo_RecvPlayerEntry(int netId, int unused, void *src) {
#pragma unused(unused)
    if (gCommPlayerInfo == NULL) {
        return;
    }
    MI_CpuCopy8(src, &gCommPlayerInfo->entries[netId], sizeof(CommPlayerInfoEntry));
    CommWireless_SetMacAddress(gCommPlayerInfo->entries[netId].mac, netId);
    gCommPlayerInfo->status[netId] = 1;
    if (netId == Comm_GetLocalNetId()) {
        gCommPlayerInfo->status[netId] = 3;
        return;
    }
    gCommPlayerInfo->unk3A5 = 1;
}

BOOL CommPlayerInfo_HostRedistribute(void) {
    int i;

    if (gCommPlayerInfo->unk3A5 == 0) {
        return FALSE;
    }
    if (Comm_GetLocalNetId() != 0) {
        return FALSE;
    }
    if (sub_02037824(5) == 0) {
        for (i = 0; i < 8; i++) {
            if (gCommPlayerInfo->status[i] != 0) {
                gCommPlayerInfo->entries[i].index = i;
                MI_CpuCopy8(gCommPlayerInfo->profiles[i], gCommPlayerInfo->entries[i].profile, PlayerProfile_sizeof());
                sub_0203753C(4, &gCommPlayerInfo->entries[i], sizeof(CommPlayerInfoEntry));
            }
        }
        sub_0203753C(5, NULL, 0);
        gCommPlayerInfo->unk3A5 = 0;
        return TRUE;
    }
    return FALSE;
}

u8 CommPlayerInfo_HasNewEntries(void) {
    return gCommPlayerInfo->unk3A5;
}

void CommPlayerInfo_ClearPlayer(int netId) {
    PlayerProfile_Init(gCommPlayerInfo->profiles[netId]);
    gCommPlayerInfo->status[netId] = 0;
}

BOOL CommPlayerInfo_IsReceived(int netId) {
    if (gCommPlayerInfo->status[netId] == 1) {
        return TRUE;
    }
    return FALSE;
}

BOOL CommPlayerInfo_IsReceivedOrAccepted(int netId) {
    return gCommPlayerInfo->status[netId] == 2 || gCommPlayerInfo->status[netId] == 1;
}

BOOL CommPlayerInfo_IsAccepted(int netId) {
    if (gCommPlayerInfo->status[netId] == 2) {
        return TRUE;
    }
    return FALSE;
}

void CommPlayerInfo_Accept(int netId) {
    gCommPlayerInfo->status[netId] = 2;
}

int CommPlayerInfo_FindReceived(void) {
    int i;

    for (i = 0; i < 8; i++) {
        if (gCommPlayerInfo->status[i] == 1) {
            return i;
        }
    }
    return 0xFF;
}

int CommPlayerInfo_CountAccepted(void) {
    int count;
    int i;

    count = 0;
    for (i = 0; i < 8; i++) {
        switch (gCommPlayerInfo->status[i]) {
        case 2:
        case 3:
            count++;
        }
    }
    return count;
}

BOOL CommPlayerInfo_DropDisconnected(void) {
    int i;
    BOOL ret;

    ret = FALSE;
    if (gCommPlayerInfo != NULL) {
        if (Comm_CountConnected() == 0) {
            return ret;
        }
        for (i = 0; i < 8; i++) {
            if (!Comm_IsNetIdConnected((u16)i) && gCommPlayerInfo->status[i] != 0) {
                CommPlayerInfo_ClearPlayer(i);
                ret = TRUE;
            }
        }
    }
    return ret;
}

void *CommPlayerInfo_GetProfile(int netId) {
    if (gCommPlayerInfo == NULL) {
        return NULL;
    }
    switch (gCommPlayerInfo->status[netId]) {
    case 1:
    case 2:
    case 3:
        return gCommPlayerInfo->profiles[netId];
    }
    return NULL;
}

void *CommPlayerInfo_GetExchangeToken(int netId) {
    if (gCommPlayerInfo->status[netId] != 0) {
        return gCommPlayerInfo->entries[netId].token;
    }
    return NULL;
}

u8 sub_02034870(int a) {
    return Comm_FindFriendIndexOfPlayer(gCommPlayerInfo->save, a);
}

void *CommPlayerInfo_GetGroupName(int netId) {
    if (gCommPlayerInfo->status[netId] != 0) {
        return gCommPlayerInfo->entries[netId].unk4C;
    }
    return NULL;
}

u8 CommPlayerInfo_GetCountry(int netId) {
    if (gCommPlayerInfo->status[netId] != 0) {
        return gCommPlayerInfo->entries[netId].country;
    }
    return 0;
}

u8 CommPlayerInfo_GetRegion(int netId) {
    if (gCommPlayerInfo->status[netId] != 0) {
        return gCommPlayerInfo->entries[netId].region;
    }
    return 0;
}

BOOL CommPlayerInfo_RulesetsMatch(void) {
    int i;
    int j;

    for (i = 0; i < 7; i++) {
        if (!Comm_IsNetIdConnected((u16)i)) {
            continue;
        }
        if (gCommPlayerInfo->status[i] == 0) {
            continue;
        }
        if (!Comm_IsNetIdConnected((u16)(i + 1))) {
            continue;
        }
        if (gCommPlayerInfo->status[i + 1] == 0) {
            continue;
        }
        for (j = 0; j < 0x20; j++) {
            if (gCommPlayerInfo->entries[i].rules[j] != gCommPlayerInfo->entries[i + 1].rules[j]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void CommPlayerInfo_AddToRecords(int mode, int delta) {
    int i;
    int parity;

    if (gCommPlayerInfo == NULL) {
        return;
    }
    if (mode != 2) {
        parity = sub_020378AC(Comm_GetLocalNetId()) & 1;
    }
    for (i = 0; i < Comm_CountConnected(); i++) {
        if (!Comm_IsNetIdConnected((u16)i)) {
            continue;
        }
        if (gCommPlayerInfo->status[i] == 0) {
            continue;
        }
        if (mode == 0) {
            if (parity == (sub_020378AC(i) & 1)) {
                continue;
            }
            gCommPlayerInfo->scores[i].wins += delta;
        } else if (mode == 1) {
            if (parity == (sub_020378AC(i) & 1)) {
                continue;
            }
            gCommPlayerInfo->scores[i].losses += delta;
        } else {
            gCommPlayerInfo->scores[i].trades += delta;
        }
    }
}

void CommPlayerInfo_SaveRecords(void *save) {
    void *r;
    int val;
    int i;
    void *tok;

    r = Save_FriendRoster_Get(save);
    for (i = 0; i < Comm_CountConnected(); i++) {
        tok = CommPlayerInfo_GetExchangeToken(i);
        if (tok != NULL) {
            int res = Comm_FindFriendSlot(save, tok, &val);
            switch (res) {
            case 0:
            case 1:
                if (val < 0) {
                    GF_AssertFail();
                }
                sub_0202C4F0(r, val, gCommPlayerInfo->scores[i].wins, gCommPlayerInfo->scores[i].losses, gCommPlayerInfo->scores[i].trades);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        gCommPlayerInfo->scores[i].wins = 0;
        gCommPlayerInfo->scores[i].losses = 0;
        gCommPlayerInfo->scores[i].trades = 0;
    }
}

void CommPlayerInfo_RecordBattleResult(void *save, int b) {
    if (b == 1) {
        CommPlayerInfo_AddToRecords(0, 1);
    } else if (b == -1) {
        CommPlayerInfo_AddToRecords(1, 1);
    }
    CommPlayerInfo_SaveRecords(save);
}

void CommPlayerInfo_RecordTrades(void *save, int b) {
    CommPlayerInfo_AddToRecords(2, b);
    CommPlayerInfo_SaveRecords(save);
}

void CommPlayerInfo_SetProfile(void *a) {
    gCommPlayerInfo->unk00 = a;
}
