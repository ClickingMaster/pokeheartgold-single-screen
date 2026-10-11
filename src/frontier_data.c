#include "frontier_data.h"

#include "global.h"

#include "frontier_data_access.h"
#include "heap.h"
#include "mail_message.h"
#include "msgdata.h"
#include "to91_frontier_download_names.h"
#include "frontier_save_access.h"

// Sub-blocks of the frontier save (Save_Frontier_GetStatic):
//   0x8E0 BattleTowerChallenge, 0x954 FrontierData, 0xABC 4 tower messages, 0xADC BattleRoomDownload

// The Battle Tower challenge in progress (BattleTowerChallenge_GetField / SetField field ids in comments).
typedef struct BattleTowerChallenge {
    u8 unk0_0 : 1;      // 7
    u8 saved : 1;       // BattleTowerChallenge_GetSaved / SetSaved
    u8 towerMode : 3;   // 0
    u8 multiAllyId : 3; // 9
    u8 unk1;
    u8 battleNumber;      // 1
    u8 unk3;              // 2, 20 points each off the Wi-Fi score
    u16 unk4;             // 3, 10 points each off the Wi-Fi score
    u16 unk6;             // 4, the last part of the Wi-Fi score
    u8 partySlots[4];     // 5
    u8 trainers[28];      // 8, 14 trainer ids
    u32 unk28;            // 10
    u8 multiAllyData[16]; // 6
} BattleTowerChallenge;   // size = 0x3C

struct FrontierData {
    u16 battlePoints;
    u8 wifiStreak;
    u8 wifiRank; // 1..10
    u32 unk4;

    union {
        u16 flags;

        struct {
            u16 pad : 4, wifiStreakActive : 1, pad2 : 11;
        } bits;
    } state;

    u16 towerRounds[6]; // per tower mode
    u16 wifiScore;
    u8 wifiTeams[2][0xA8];
}; // size = 0x168

// One trainer of a downloaded Wi-Fi Battle Room.
typedef struct BattleRoomTrainer {
    u8 team[0xA8];
    u16 name[16];
    u8 useDefaultName : 1, gender : 1, pad : 6;
    u8 trainerClass;
    u8 messages[24];
    u16 score;
} BattleRoomTrainer; // size = 0xE4

typedef struct BattleRoomDownload {
    u32 markDate;      // year, month, day, hour of the last mark, one byte each
    u8 roomMarks[250]; // one bit per (rank 1..10, room 1..200)
    u8 hasTrainers, hasLeaders;
    u8 trainersRoom, trainersRank, leadersRoom, leadersRank;
    BattleRoomTrainer trainers[7];
    Ov67DownloadRecords leaders;
} BattleRoomDownload; // size = 0xB3C

typedef struct BattleTowerOpponent {
    u32 id;
    u16 trainerClass;
    u8 pad6[2];
    u16 name[8];
    u8 messages[24];
    u8 team[0xA8];
} BattleTowerOpponent;

// asm/frontier_data_rodata.s
extern const MailMessageTemplate gTowerMessageTemplates[4];

u32 BattleRoomTrainer_sizeof(void);
void FrontierData_Reset(void *data);
void TowerMessages_Init(void *data);
void BattleRoomDownload_Init(void *data);
u8 FrontierData_GetWifiWinCount(FrontierData *data);
void sub_0202D638(FrontierData *data, u32 value);
u32 sub_0202D63C(FrontierData *data);
void BattleRoomDownload_MarkRoom(BattleRoomDownload *download, u32 rank, u32 room, u32 *date);
void BattleRoomDownload_ClearRoomMarks(BattleRoomDownload *download);
BOOL BattleRoomDownload_IsDateNewer(u32 *date, u32 *stored);
BOOL BattleRoomDownload_IsRoomMarked(BattleRoomDownload *download, u32 rank, u32 room, u32 *date);
void BattleRoomDownload_SetTrainers(BattleRoomDownload *download, void *trainers, u32 rank, u32 room);
void BattleRoomDownload_GetTrainersRoom(BattleRoomDownload *download, u8 *out);
void BattleRoomDownload_GetTrainer(BattleRoomDownload *download, BattleTowerOpponent *out, u32 index);
void BattleRoomDownload_SetLeaders(BattleRoomDownload *download, void *leaders, u32 rank, u32 room);
void *BattleRoomDownload_CopyLeaders(BattleRoomDownload *download, enum HeapID heapID);

u32 BattleRoomTrainer_sizeof(void) {
    return sizeof(BattleRoomTrainer);
}

void BattleTowerChallenge_Clear(u32 challenge) {
    MI_CpuFill8((void *)challenge, 0, sizeof(BattleTowerChallenge));
}

void FrontierData_Reset(void *data) {
    MI_CpuFill8(data, 0, sizeof(FrontierData));
    ((FrontierData *)data)->wifiRank = 1;
}

void TowerMessages_Init(void *data) {
    MailMessage *messages = data;
    int i;
    for (i = 0; i < 4; i++) {
        MailMsg_Init_FromTemplate(&messages[i], &gTowerMessageTemplates[i]);
    }
}

void BattleRoomDownload_Init(void *data) {
    MI_CpuFill8(data, 0, sizeof(BattleRoomDownload));
}

u32 BattleTowerChallenge_GetField(u32 challenge, u32 field, void *out) {
    BattleTowerChallenge *c = (BattleTowerChallenge *)challenge;
    switch (field) {
    case 0:
        return c->towerMode;
    case 1:
        return c->battleNumber;
    case 2:
        return c->unk3;
    case 3:
        return c->unk4;
    case 4:
        return c->unk6;
    case 5:
        MI_CpuCopy8(c->partySlots, out, 4);
        return 0;
    case 6:
        MI_CpuCopy8(c->multiAllyData, out, 16);
        return 0;
    case 7:
        return c->unk0_0;
    case 8:
        MI_CpuCopy8(c->trainers, out, 28);
        return 0;
    case 9:
        return c->multiAllyId;
    case 10:
        return c->unk28;
    }
    return 0;
}

void BattleTowerChallenge_SetField(u32 challenge, u32 field, void *value) {
    BattleTowerChallenge *c = (BattleTowerChallenge *)challenge;
    switch (field) {
    case 0:
        c->towerMode = *(u8 *)value;
        break;
    case 1:
        c->battleNumber = *(u8 *)value;
        break;
    case 2:
        c->unk3 = *(u8 *)value;
        break;
    case 3:
        c->unk4 = *(u16 *)value;
        break;
    case 4:
        c->unk6 = *(u16 *)value;
        break;
    case 5:
        MI_CpuCopy8(value, c->partySlots, 4);
        break;
    case 6:
        MI_CpuCopy8(value, c->multiAllyData, 16);
        break;
    case 7:
        c->unk0_0 = *(u8 *)value;
        break;
    case 8:
        MI_CpuCopy8(value, c->trainers, 28);
        break;
    case 10:
        c->unk28 = *(u32 *)value;
        break;
    case 9:
        c->multiAllyId = *(u8 *)value;
        break;
    }
}

// Adds to the three Wi-Fi score counters, each capped below its type's maximum.
void sub_0202D3B0(u32 challenge, u8 a, u32 b, u32 c) {
    BattleTowerChallenge *ch = (BattleTowerChallenge *)challenge;
    int x = ch->unk3 + a;
    int y;
    int z;
    if (x < 255) {
        ch->unk3 = x;
    }
    y = ch->unk4 + b;
    if (y < 65535) {
        ch->unk4 = y;
    }
    z = ch->unk6 + c;
    if (z < 65535) {
        ch->unk6 = z;
    }
}

u32 BattleTowerChallenge_GetSaved(u32 challenge) {
    return ((BattleTowerChallenge *)challenge)->saved;
}

void BattleTowerChallenge_SetSaved(u32 challenge, u32 value) {
    ((BattleTowerChallenge *)challenge)->saved = (u8)value;
}

// action: 0 get, 1 set, 5 add, 6 subtract; capped at 9999. Returns the new amount.
u32 FrontierData_BattlePointAction(FrontierData *data, u32 param, u32 action) {
    switch (action) {
    case 1:
        if (param > 9999) {
            data->battlePoints = 9999;
        } else {
            data->battlePoints = param;
        }
        break;
    case 5: {
        int points = data->battlePoints + param;
        if (points > 9999) {
            data->battlePoints = 9999;
        } else {
            data->battlePoints = points;
        }
        break;
    }
    case 6:
        if (data->battlePoints < param) {
            data->battlePoints = 0;
        } else {
            data->battlePoints -= param;
        }
        break;
    case 0:
    case 2:
    case 3:
    case 4:
        break;
    }
    return data->battlePoints;
}

// action: 2 reset, 3 count (starts at 1 when the streak flag is not set); returns the count.
u32 sub_0202D450(FrontierData *data, u32 action) {
    switch (action) {
    case 2:
        data->wifiStreak = 0;
        data->state.flags &= ~16;
        break;
    case 3:
        if (data->state.bits.wifiStreakActive) {
            data->wifiStreak++;
        } else {
            data->wifiStreak = 1;
            data->state.flags |= 16;
        }
        break;
    }
    return data->wifiStreak;
}

// action: 0 get, 2 reset to 1, 3 up (max 10), 4 down (min 1). Returns the rank.
u32 FrontierData_WifiRankAction(FrontierData *data, u32 action) {
    switch (action) {
    case 2:
        data->wifiRank = 1;
        break;
    case 3:
        if (data->wifiRank < 10) {
            data->wifiRank++;
        }
        break;
    case 4:
        if (data->wifiRank > 1) {
            data->wifiRank--;
        }
        break;
    }
    return data->wifiRank;
}

void sub_0202D4B8(FrontierData *data, u32 index, void *src) {
    if (index == 0) {
        MI_CpuCopy8(src, data->wifiTeams[1], 0xA8);
    } else {
        MI_CpuCopy8(src, data->wifiTeams[0], 0xA8);
    }
}

void sub_0202D4DC(FrontierData *data, s32 index, UnkStruct_02069528 *dst) {
    if (index == 0) {
        MI_CpuCopy8(data->wifiTeams[1], dst, 0xA8);
    } else {
        MI_CpuCopy8(data->wifiTeams[0], dst, 0xA8);
    }
}

u16 FrontierData_CalcWifiScore(FrontierData *data, u32 challenge) {
    BattleTowerChallenge *c = (BattleTowerChallenge *)challenge;
    u16 base = 1000 * (c->battleNumber - 1);
    u16 a = 10 * c->unk4;
    u16 b = 20 * c->unk3;
    u16 score;
    u16 time;
    if (a + b > 950) {
        score = 0;
    } else {
        score = 950 - (a + b);
    }
    if (c->unk6 > 970) {
        time = 0;
    } else {
        time = (1000 - c->unk6) / 30;
    }
    u16 result = (u16)(base + score + time);
    data->wifiScore = result;
    return result;
}

u16 FrontierData_GetWifiScore(FrontierData *data) {
    return data->wifiScore;
}

u8 FrontierData_GetWifiWinCount(FrontierData *data) {
    return data->wifiScore / 1000;
}

// action: 0 get, 2 reset, 3 count up (max 65534). Index 5 has no counter, index 6 uses slot 5.
u32 FrontierData_TowerRoundAction(void *ptr, u32 towerMode, u32 action) {
    FrontierData *data = ptr;
    if (towerMode == 5) {
        return 0;
    }
    if (towerMode == 6) {
        towerMode = 5;
    }
    switch (action) {
    case 2:
        data->towerRounds[towerMode] = 0;
        break;
    case 3:
        if (data->towerRounds[towerMode] < 65534) {
            data->towerRounds[towerMode]++;
        }
        break;
    }
    return data->towerRounds[towerMode];
}

u32 FrontierData_SetTowerRound(FrontierData *data, u32 towerMode, u16 round) {
    if (towerMode == 5) {
        return 0;
    }
    if (towerMode == 6) {
        towerMode = 5;
    }
    data->towerRounds[towerMode] = round;
    return data->towerRounds[towerMode];
}

// action: 0 get, 1 set, 2 clear.
u32 FrontierData_FlagAction(FrontierData *data, u16 flag, u32 action) {
    u16 mask = 1;
    u16 i;
    if (flag >= 16) {
        GF_ASSERT(FALSE);
        return 0;
    }
    for (i = 0; i < flag; i++) {
        mask <<= 1;
    }
    switch (action) {
    // Preserve the original mask calculation before reading the flags.
    case 2:
        mask ^= 0xFFFF;
        data->state.flags = mask & *(volatile u16 *)&data->state.flags;
        break;
    case 1:
        data->state.flags |= mask;
        break;
    case 0:
        return (data->state.flags >> flag) & 1;
    }
    return 0;
}

void sub_0202D638(FrontierData *data, u32 value) {
    data->unk4 = value;
}

u32 sub_0202D63C(FrontierData *data) {
    return data->unk4;
}

void Save_Frontier_SetTowerMessage(SaveData *save, int index, MailMessage *message) {
    MailMessage *messages = (MailMessage *)((u8 *)Save_Frontier_GetStatic(save) + 0xABC);
    MailMsg_Copy(&messages[index], message);
}

MailMessage *Save_Frontier_GetTowerMessage(SaveData *save, u32 index) {
    MailMessage *messages = (MailMessage *)((u8 *)Save_Frontier_GetStatic(save) + 0xABC);
    return &messages[index];
}

void BattleRoomDownload_MarkRoom(BattleRoomDownload *download, u32 rank, u32 room, u32 *date) {
    u8 mask = 1;
    if (room != 0 && room <= 200 && rank != 0 && rank <= 10) {
        u16 index = (room - 1) + (rank - 1) * 200;
        u8 bit = index % 8;
        u8 byte = index / 8;
        mask <<= bit;
        download->roomMarks[byte] |= mask;
        download->markDate = (date[0] << 24) | ((date[1] << 24) >> 8) | ((date[2] << 24) >> 16) | date[3];
    }
}

void BattleRoomDownload_ClearRoomMarks(BattleRoomDownload *download) {
    MI_CpuFill8(download->roomMarks, 0, 250);
    MI_CpuFill8(&download->markDate, 0, 4);
}

BOOL BattleRoomDownload_IsDateNewer(u32 *date, u32 *stored) {
    if (date[0] > stored[0]) {
        return TRUE;
    }
    if (date[1] > stored[1]) {
        return TRUE;
    }
    if (date[2] > stored[2]) {
        return TRUE;
    }
    return FALSE;
}

BOOL BattleRoomDownload_IsRoomMarked(BattleRoomDownload *download, u32 rank, u32 room, u32 *date) {
    u8 mask = 1;
    u32 stored[4];
    u16 index;
    u8 bit;
    u8 byte;
    if (room > 200 || rank > 10) {
        return FALSE;
    }
    u32 markDate = download->markDate;
    stored[0] = (u8)(markDate >> 24);
    stored[1] = (u8)(markDate >> 16);
    stored[2] = (u8)(markDate >> 8);
    stored[3] = (u8)markDate;
    if (BattleRoomDownload_IsDateNewer(date, stored)) {
        BattleRoomDownload_ClearRoomMarks(download);
        return FALSE;
    }
    index = (rank - 1) * 200 + (room - 1);
    bit = index % 8;
    byte = index / 8;
    mask <<= bit;
    if (download->roomMarks[byte] & mask) {
        return TRUE;
    }
    return FALSE;
}

u32 BattleRoomDownload_HasTrainers(u32 download) {
    return ((BattleRoomDownload *)download)->hasTrainers;
}

u32 BattleRoomDownload_HasLeaders(u32 download) {
    return ((BattleRoomDownload *)download)->hasLeaders;
}

void BattleRoomDownload_SetTrainers(BattleRoomDownload *download, void *trainers, u32 rank, u32 room) {
    MI_CpuCopy8(trainers, download->trainers, sizeof(download->trainers));
    download->trainersRank = rank;
    download->trainersRoom = room;
    download->hasTrainers = 1;
}

void BattleRoomDownload_GetTrainersRoom(BattleRoomDownload *download, u8 *out) {
    out[0] = download->trainersRank;
    out[1] = download->trainersRoom;
}

void BattleRoomDownload_GetTrainer(BattleRoomDownload *download, BattleTowerOpponent *out, u32 index) {
    BattleRoomTrainer *trainers = download->trainers;
    u32 offset = index * sizeof(BattleRoomTrainer);
    u8 *team = out->team;
    out->id = 10000;
    out->trainerClass = ((BattleRoomTrainer *)((u8 *)trainers + offset))->trainerClass;
    if (((BattleRoomTrainer *)((u8 *)trainers + offset))->useDefaultName) {
        MsgData *msg = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (NarcId)27, 28, (enum HeapID)11);
        ReadMsgDataIntoU16Array(msg, (((BattleRoomTrainer *)((u8 *)trainers + offset))->gender) + 33, out->name);
        DestroyMsgData(msg);
    } else {
        MI_CpuCopy8(((BattleRoomTrainer *)((u8 *)trainers + offset))->name, out->name, 16);
    }
    MI_CpuCopy8(((BattleRoomTrainer *)((u8 *)trainers + offset))->messages, out->messages, 8);
    MI_CpuCopy8(((BattleRoomTrainer *)((u8 *)trainers + offset))->messages + 8, out->messages + 8, 8);
    MI_CpuCopy8(((BattleRoomTrainer *)((u8 *)trainers + offset))->messages + 16, out->messages + 16, 8);
    MI_CpuCopy8(((BattleRoomTrainer *)((u8 *)trainers + offset))->team, team, 0xA8);
}

void BattleRoomDownload_SetLeaders(BattleRoomDownload *download, void *leaders, u32 rank, u32 room) {
    MI_CpuCopy8(leaders, download->leaders, sizeof(download->leaders));
    download->leadersRank = rank;
    download->leadersRoom = room;
    download->hasLeaders = 1;
}

void BattleRoomDownload_GetLeadersRoom(u32 addr, u8 *out) {
    BattleRoomDownload *download = (BattleRoomDownload *)addr;
    out[0] = download->leadersRank;
    out[1] = download->leadersRoom;
}

void *BattleRoomDownload_CopyLeaders(BattleRoomDownload *download, enum HeapID heapID) {
    void *out = Heap_Alloc(heapID, sizeof(download->leaders));
    MI_CpuCopy8(download->leaders, out, sizeof(download->leaders));
    return out;
}

u32 Save_BattleTowerChallenge_Get(SaveData *save) {
    return (u32)Save_Frontier_GetStatic(save) + 0x8E0;
}

FrontierData *Save_FrontierData_Get(SaveData *save) {
    return (FrontierData *)((u8 *)Save_Frontier_GetStatic(save) + 0x954);
}

u32 Save_BattleRoomDownload_Get(SaveData *save) {
    return (u32)Save_Frontier_GetStatic(save) + 0xADC;
}
