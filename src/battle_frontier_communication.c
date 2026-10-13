#include "global.h"

#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "frontier_save_access.h"
#include "communication_command.h"
#include "communication_core.h"

extern const u32 gFrontierLinkCommands[];
typedef struct CommDataBits {
    u8 pad[0x10];
    u8 low : 3;
    u8 flag : 1;
    u8 mid : 1;
    u8 high : 3;
} CommDataBits;
void FrontierLink_RegisterCommands(void *data);
void BattleTowerLink_ReceivePartnerInfo(int sender, int unused, const u16 *src, u8 *data);
BOOL BattleTowerLink_SendTrainers(u8 *data);
void BattleTowerLink_ReceiveTrainers(int sender, int unused, const void *src, u8 *data);
void sub_020969F8(int sender, int unused, const u16 *src, u8 *data);
BOOL FrontierLinkLobby_SendFacility(u8 *data);
void FrontierLinkLobby_ReceiveFacility(int sender, int unused, const u16 *src, u8 *data);
BOOL FrontierLinkLobby_SendChoice(u8 *data, u16 value);
void FrontierLinkLobby_ReceiveChoice(int sender, int unused, const u16 *src, u8 *data);
BOOL FrontierLinkLobby_SendParty(u8 *data, u8 first, u8 second);
void FrontierLinkLobby_ReceiveParty(int sender, int unused, const u16 *src, u8 *data);
BOOL FrontierLinkLobby_SendValue74(u8 *data, u16 value);
void FrontierLinkLobby_ReceiveValue74(int sender, int unused, const u16 *src, u8 *data);
BOOL FrontierLinkLobby_SendValue71(u8 *data, u16 value);
void FrontierLinkLobby_ReceiveValue71(int sender, int unused, const u16 *src, u8 *data);
int ov80_022385D8(int value);

void FrontierLink_RegisterCommands(void *data) {
    Comm_RegisterCommandTable(gFrontierLinkCommands, 52, data);
}

void BattleTowerLink_ReceivePartnerInfo(int sender, int unused, const u16 *src, u8 *data) {
    u16 result = 0;
    data[0x8D4]++;
    if (sender != Comm_GetLocalNetId()) {
        data[0x12] = src[0];
        *(u16 *)(data + 0x16) = src[1];
        *(u16 *)(data + 0x18) = src[2];
        *(u16 *)(data + 0x14) = src[3];
        ((CommDataBits *)data)->high = data[0x12] + 5;
        if (*(u16 *)(data + 0x2E) == *(u16 *)(data + 0x16) || *(u16 *)(data + 0x2E) == *(u16 *)(data + 0x18)) {
            result += 1;
        }
        if (*(u16 *)(data + 0x30) == *(u16 *)(data + 0x16) || *(u16 *)(data + 0x30) == *(u16 *)(data + 0x18)) {
            result += 2;
        }
        *(u16 *)(data + 0x8D8) = result;
    }
}

BOOL BattleTowerLink_SendTrainers(u8 *data) {
    MI_CpuCopy8(data + 0x3E, data + 0x83E, 0x1C);
    if (Comm_SendCommand(0x3F, data + 0x83E, 0x1C) == 1) {
        return TRUE;
    }
    return FALSE;
}

void BattleTowerLink_ReceiveTrainers(int sender, int unused, const void *src, u8 *data) {
    data[0x8D4]++;
    if (sender != Comm_GetLocalNetId() && Comm_GetLocalNetId() != 0) {
        MI_CpuCopy8(src, data + 0x3E, 0x1C);
    }
}

void sub_020969F8(int sender, int unused, const u16 *src, u8 *data) {
    *(u16 *)(data + 0x8D8) = 0;
    data[0x8D4]++;
    if (sender != Comm_GetLocalNetId()) {
        if (((CommDataBits *)data)->flag != 0 || src[0] != 0) {
            *(u16 *)(data + 0x8D8) = 1;
        }
    }
}

BOOL FrontierLinkLobby_SendFacility(u8 *data) {
    int stat;
    FrontierSave *frontier;
    *(u16 *)(data + 8) = data[0xA0];
    stat = ov80_022385D8(data[0xA0]);
    frontier = Save_Frontier_GetStatic(*(SaveData **)data);
    *(u16 *)(data + 0xA) = FrontierSave_GetStat(frontier, stat, FrontierStat_GetFriendIndex(stat));
    if (Comm_SendCommand(0x39, data + 8, 0x28) == 1) {
        return TRUE;
    }
    return FALSE;
}

void FrontierLinkLobby_ReceiveFacility(int sender, int unused, const u16 *src, u8 *data) {
    data[0x6F]++;
    if (sender != Comm_GetLocalNetId()) {
        data[0x6E] = src[0];
        *(u16 *)(data + 0x72) = src[1];
    }
}

BOOL FrontierLinkLobby_SendChoice(u8 *data, u16 value) {
    *(u16 *)(data + 8) = value;
    if (Comm_SendCommand(0x3A, data + 8, 0x28) == 1) {
        return TRUE;
    }
    return FALSE;
}

void FrontierLinkLobby_ReceiveChoice(int sender, int unused, const u16 *src, u8 *data) {
    data[0x6F]++;
    if (sender != Comm_GetLocalNetId()) {
        data[0x59] = src[0];
    }
}

BOOL FrontierLinkLobby_SendParty(u8 *data, u8 first, u8 second) {
    Party *party = SaveArray_Party_Get(*(SaveData **)data);
    Pokemon *mon;
    data[0x6A] = first;
    data[0x6B] = second;
    if (first == 0xFF) {
        *(u16 *)(data + 0x76) = 0;
        *(u16 *)(data + 0x7E) = 0;
        *(u16 *)(data + 0x78) = 0;
        *(u16 *)(data + 0x80) = 0;
    } else {
        mon = Party_GetMonByIndex(party, first);
        *(u16 *)(data + 0x76) = GetMonData(mon, MON_DATA_SPECIES, NULL);
        *(u16 *)(data + 0x7E) = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
        mon = Party_GetMonByIndex(party, second);
        *(u16 *)(data + 0x78) = GetMonData(mon, MON_DATA_SPECIES, NULL);
        *(u16 *)(data + 0x80) = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    *(u16 *)(data + 8) = *(u16 *)(data + 0x76);
    *(u16 *)(data + 0xA) = *(u16 *)(data + 0x7E);
    *(u16 *)(data + 0xC) = *(u16 *)(data + 0x78);
    *(u16 *)(data + 0xE) = *(u16 *)(data + 0x80);
    if (Comm_SendCommand(0x3B, data + 8, 0x28) == 1) {
        return TRUE;
    }
    return FALSE;
}

void FrontierLinkLobby_ReceiveParty(int sender, int unused, const u16 *src, u8 *data) {
    data[0x6F]++;
    if (sender != Comm_GetLocalNetId()) {
        *(u16 *)(data + 0x86) = src[0];
        *(u16 *)(data + 0x8E) = src[1];
        *(u16 *)(data + 0x88) = src[2];
        *(u16 *)(data + 0x90) = src[3];
    }
}

BOOL FrontierLinkLobby_SendValue74(u8 *data, u16 value) {
    *(u16 *)(data + 8) = value;
    if (Comm_SendCommand(0x3C, data + 8, 0x28) == 1) {
        return TRUE;
    }
    return FALSE;
}

void FrontierLinkLobby_ReceiveValue74(int sender, int unused, const u16 *src, u8 *data) {
    data[0x6F]++;
    if (sender != Comm_GetLocalNetId()) {
        *(u16 *)(data + 0x74) = src[0];
    }
}

BOOL FrontierLinkLobby_SendValue71(u8 *data, u16 value) {
    *(u16 *)(data + 8) = value;
    if (Comm_SendCommand(0x3D, data + 8, 0x28) == 1) {
        return TRUE;
    }
    return FALSE;
}

void FrontierLinkLobby_ReceiveValue71(int sender, int unused, const u16 *src, u8 *data) {
    data[0x6F]++;
    if (sender != Comm_GetLocalNetId()) {
        data[0x71] = src[0];
    }
}
