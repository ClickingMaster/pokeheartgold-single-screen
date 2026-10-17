#include "communication_player_state_internal.h"

static CommPlayerInfo *sCommPlayerInfo;

void CommPlayerInfo_Init(void *save, void *rules) {
    void *profile;
    int i;

    profile = Save_PlayerData_GetProfile(save);
    if (sCommPlayerInfo == NULL) {
        sCommPlayerInfo = Heap_Alloc(15, sizeof(CommPlayerInfo));
        MI_CpuFill8(sCommPlayerInfo, 0, sizeof(CommPlayerInfo));
        for (i = 0; i < 8; i++) {
            sCommPlayerInfo->profiles[i] = &sCommPlayerInfo->entries[i].profile;
            CommPlayerInfo_ClearPlayer(i);
        }
        sCommPlayerInfo->unk3A4 = 0;
        sCommPlayerInfo->hasNewEntries = 0;
        sCommPlayerInfo->curIdx = 0;
        sCommPlayerInfo->save = save;
        sCommPlayerInfo->rules = rules;
        PlayerProfile_Copy(profile, sCommPlayerInfo->profiles[0]);
    }
}

void CommPlayerInfo_Free(void) {
    int i;

    if (sCommPlayerInfo != NULL) {
        for (i = 0; i < 8; i++) {
            sCommPlayerInfo->profiles[i] = NULL;
        }
        if (sCommPlayerInfo != NULL) {
            Heap_Free(sCommPlayerInfo);
        }
        sCommPlayerInfo = NULL;
    }
}

BOOL CommPlayerInfo_Exists(void) {
    if (sCommPlayerInfo != NULL) {
        return TRUE;
    }
    return FALSE;
}

void CommPlayerInfo_SendLocal(void) {
    int netId;
    void *friends;
    void *friendRoster;
    void *wifi;
    void *profile;

    netId = Comm_GetLocalNetId();
    friends = Save_FriendGroup_Get(sCommPlayerInfo->save);
    friendRoster = Save_FriendRoster_Get(sCommPlayerInfo->save);
    wifi = Save_WiFiHistory_Get(sCommPlayerInfo->save);
    profile = sCommPlayerInfo->localProfile;
    if (profile == NULL) {
        profile = Save_PlayerData_GetProfile(sCommPlayerInfo->save);
    }
    PlayerProfile_Copy(profile, sCommPlayerInfo->profiles[netId]);
    OS_GetMacAddress(sCommPlayerInfo->entries[netId].mac);
    MI_CpuCopy8(Save_FriendGroup_GetName(friends, 1, 0), sCommPlayerInfo->entries[netId].groupName, 0x10);
    sCommPlayerInfo->entries[netId].country = WifiHistory_GetPlayerCountry(wifi);
    sCommPlayerInfo->entries[netId].region = WiFiHistory_GetPlayerRegion(wifi);
    sCommPlayerInfo->entries[netId].unk65 = 0;
    DWC_CreateExchangeToken(FriendRoster_GetUserData(friendRoster), sCommPlayerInfo->entries[netId].token);
    MI_CpuFill8(&sCommPlayerInfo->entries[netId], 0, 0x20);
    if (sCommPlayerInfo->rules != NULL) {
        LinkBattleRuleset_Copy(sCommPlayerInfo->rules, &sCommPlayerInfo->entries[netId]);
    }
    Comm_SendCommand(3, &sCommPlayerInfo->entries[netId], sizeof(CommPlayerInfoEntry));
}

int CommPlayerInfo_GetEntrySize(void) {
    return sizeof(CommPlayerInfoEntry);
}

void CommPlayerInfo_RecvSetUnk3A4(void) {
    if (sCommPlayerInfo != NULL) {
        sCommPlayerInfo->unk3A4 = 1;
    }
}

void CommPlayerInfo_RecvEntry(int netId, int unused, void *packet) {
#pragma unused(unused)
    CommPlayerInfoEntry *pkt = packet;

    if (sCommPlayerInfo == NULL) {
        return;
    }
    if (!Comm_IsNetIdConnected((u16)netId)) {
        return;
    }
    MI_CpuCopy8(pkt, &sCommPlayerInfo->entries[pkt->index], sizeof(CommPlayerInfoEntry));
    sCommPlayerInfo->curIdx = pkt->index;
    if (PlayerProfile_IsNameEmpty(sCommPlayerInfo->profiles[sCommPlayerInfo->curIdx]) == TRUE) {
        return;
    }
    if (sCommPlayerInfo->status[sCommPlayerInfo->curIdx] >= 2) {
        return;
    }
    sCommPlayerInfo->status[sCommPlayerInfo->curIdx] = 1;
    if (sCommPlayerInfo->curIdx != Comm_GetLocalNetId()) {
        return;
    }
    sCommPlayerInfo->status[sCommPlayerInfo->curIdx] = 3;
}

void CommPlayerInfo_RecvPlayerEntry(int netId, int unused, void *src) {
#pragma unused(unused)
    if (sCommPlayerInfo == NULL) {
        return;
    }
    MI_CpuCopy8(src, &sCommPlayerInfo->entries[netId], sizeof(CommPlayerInfoEntry));
    CommWireless_SetMacAddress(sCommPlayerInfo->entries[netId].mac, netId);
    sCommPlayerInfo->status[netId] = 1;
    if (netId == Comm_GetLocalNetId()) {
        sCommPlayerInfo->status[netId] = 3;
        return;
    }
    sCommPlayerInfo->hasNewEntries = 1;
}

BOOL CommPlayerInfo_HostRedistribute(void) {
    int i;

    if (sCommPlayerInfo->hasNewEntries == 0) {
        return FALSE;
    }
    if (Comm_GetLocalNetId() != 0) {
        return FALSE;
    }
    if (Comm_ParentQueueHasCommand(5) == 0) {
        for (i = 0; i < 8; i++) {
            if (sCommPlayerInfo->status[i] != 0) {
                sCommPlayerInfo->entries[i].index = i;
                MI_CpuCopy8(sCommPlayerInfo->profiles[i], sCommPlayerInfo->entries[i].profile, PlayerProfile_sizeof());
                Comm_SendParentCommandNoCopyUnchecked(4, &sCommPlayerInfo->entries[i], sizeof(CommPlayerInfoEntry));
            }
        }
        Comm_SendParentCommandNoCopyUnchecked(5, NULL, 0);
        sCommPlayerInfo->hasNewEntries = 0;
        return TRUE;
    }
    return FALSE;
}

u8 CommPlayerInfo_HasNewEntries(void) {
    return sCommPlayerInfo->hasNewEntries;
}

void CommPlayerInfo_ClearPlayer(int netId) {
    PlayerProfile_Init(sCommPlayerInfo->profiles[netId]);
    sCommPlayerInfo->status[netId] = 0;
}

BOOL CommPlayerInfo_IsReceived(int netId) {
    if (sCommPlayerInfo->status[netId] == 1) {
        return TRUE;
    }
    return FALSE;
}

BOOL CommPlayerInfo_IsReceivedOrAccepted(int netId) {
    return sCommPlayerInfo->status[netId] == 2 || sCommPlayerInfo->status[netId] == 1;
}

BOOL CommPlayerInfo_IsAccepted(int netId) {
    if (sCommPlayerInfo->status[netId] == 2) {
        return TRUE;
    }
    return FALSE;
}

void CommPlayerInfo_Accept(int netId) {
    sCommPlayerInfo->status[netId] = 2;
}

int CommPlayerInfo_FindReceived(void) {
    int i;

    for (i = 0; i < 8; i++) {
        if (sCommPlayerInfo->status[i] == 1) {
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
        switch (sCommPlayerInfo->status[i]) {
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
    if (sCommPlayerInfo != NULL) {
        if (Comm_CountConnected() == 0) {
            return ret;
        }
        for (i = 0; i < 8; i++) {
            if (!Comm_IsNetIdConnected((u16)i) && sCommPlayerInfo->status[i] != 0) {
                CommPlayerInfo_ClearPlayer(i);
                ret = TRUE;
            }
        }
    }
    return ret;
}

void *CommPlayerInfo_GetProfile(int netId) {
    if (sCommPlayerInfo == NULL) {
        return NULL;
    }
    switch (sCommPlayerInfo->status[netId]) {
    case 1:
    case 2:
    case 3:
        return sCommPlayerInfo->profiles[netId];
    }
    return NULL;
}

void *CommPlayerInfo_GetExchangeToken(int netId) {
    if (sCommPlayerInfo->status[netId] != 0) {
        return sCommPlayerInfo->entries[netId].token;
    }
    return NULL;
}

u8 CommPlayerInfo_GetFriendIndex(int a) {
    return Comm_FindFriendIndexOfPlayer(sCommPlayerInfo->save, a);
}

void *CommPlayerInfo_GetGroupName(int netId) {
    if (sCommPlayerInfo->status[netId] != 0) {
        return sCommPlayerInfo->entries[netId].groupName;
    }
    return NULL;
}

u8 CommPlayerInfo_GetCountry(int netId) {
    if (sCommPlayerInfo->status[netId] != 0) {
        return sCommPlayerInfo->entries[netId].country;
    }
    return 0;
}

u8 CommPlayerInfo_GetRegion(int netId) {
    if (sCommPlayerInfo->status[netId] != 0) {
        return sCommPlayerInfo->entries[netId].region;
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
        if (sCommPlayerInfo->status[i] == 0) {
            continue;
        }
        if (!Comm_IsNetIdConnected((u16)(i + 1))) {
            continue;
        }
        if (sCommPlayerInfo->status[i + 1] == 0) {
            continue;
        }
        for (j = 0; j < 0x20; j++) {
            if (sCommPlayerInfo->entries[i].rules[j] != sCommPlayerInfo->entries[i + 1].rules[j]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void CommPlayerInfo_AddToRecords(int mode, int delta) {
    int i;
    int parity;

    if (sCommPlayerInfo == NULL) {
        return;
    }
    if (mode != 2) {
        parity = Comm_GetBattleSlot(Comm_GetLocalNetId()) & 1;
    }
    for (i = 0; i < Comm_CountConnected(); i++) {
        if (!Comm_IsNetIdConnected((u16)i)) {
            continue;
        }
        if (sCommPlayerInfo->status[i] == 0) {
            continue;
        }
        if (mode == 0) {
            if (parity == (Comm_GetBattleSlot(i) & 1)) {
                continue;
            }
            sCommPlayerInfo->scores[i].wins += delta;
        } else if (mode == 1) {
            if (parity == (Comm_GetBattleSlot(i) & 1)) {
                continue;
            }
            sCommPlayerInfo->scores[i].losses += delta;
        } else {
            sCommPlayerInfo->scores[i].trades += delta;
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
                FriendRoster_AddWinsLossesTrades(r, val, sCommPlayerInfo->scores[i].wins, sCommPlayerInfo->scores[i].losses, sCommPlayerInfo->scores[i].trades);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        sCommPlayerInfo->scores[i].wins = 0;
        sCommPlayerInfo->scores[i].losses = 0;
        sCommPlayerInfo->scores[i].trades = 0;
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

void CommPlayerInfo_RecordTrades(void *save, int count) {
    CommPlayerInfo_AddToRecords(2, count);
    CommPlayerInfo_SaveRecords(save);
}

void CommPlayerInfo_SetProfile(void *a) {
    sCommPlayerInfo->localProfile = a;
}
