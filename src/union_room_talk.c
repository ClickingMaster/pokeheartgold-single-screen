#include "union_room_talk.h"

#include "global.h"

#include "msgdata/msg.naix"

#include "assert.h"
#include "comm_state.h"
#include "communication_player_state.h"
#include "easy_chat.h"
#include "field_system.h"
#include "heap.h"
#include "mail_message.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "trainer_card_data.h"
#include "union_room_avatar.h"
#include "communication_core.h"

// Union Room talk (ARM9 static 0x0205A44C-0x0205AC88): the part of the Union Room object (FieldSystem.unionRoom,
// union_room.c) used while the player talks to another player: the link commands of the talk menu (choice,
// reply, cancel, battle party choice, trainer card exchange), the script messages for what a player is doing,
// the activity and chat message broadcast in the player's beacon, and the language greeting learned from a
// foreign player.

// What a Union Room player broadcasts in its beacon besides its profile and chat message: the activity it is busy
// with and, for a parent, the members of its group.
typedef struct UnionRoomStatus {
    u32 memberIds[4];    // 0x00: trainer ids of net ids 1-4
    u8 unk10[4];         // 0x10
    u8 unk14[4];         // 0x14
    u8 memberAvatars[4]; // 0x18: avatar | gender << 7
    u8 activity;         // 0x1C
} UnionRoomStatus;       // size: 0x20

// A player's beacon record as seen by the Union Room (communication layer, 0xC0 bytes each).
typedef struct UnionRoomPeerInfo {
    u8 unk00[0x30];         // chat message, profile
    UnionRoomStatus status; // 0x30
} UnionRoomPeerInfo;

typedef struct UnionRoomPeerRecord {
    u8 unk00[0x50];
    UnionRoomPeerInfo info; // 0x50
} UnionRoomPeerRecord;

// The Union Room object; only the fields used here.
struct UnionRoom {
    FieldSystem *fieldSystem;         // 0x000
    SaveData *saveData;               // 0x004
    PlayerProfile *profile;           // 0x008
    u8 unk00C[0x20 - 0x0C];           // 0x00C
    u32 connectRequest;               // 0x020
    u32 unk24;                        // 0x024
    u32 unk28;                        // 0x028
    u32 talkChoiceReceived;           // 0x02C: set by link command 0x67
    u32 talkReply;                    // 0x030: the partner's answer (link command 0x63)
    u32 sentTalkReply;                        // 0x034
    u8 unk038[0x40 - 0x38];           // 0x038
    u32 talkChoice;                   // 0x040: what the partner chose to do (link command 0x67)
    u32 partnerCancelled;             // 0x044: link command 0x68
    u8 unk048[0x110 - 0x48];          // 0x048
    UnionRoomPeerRecord *records[16]; // 0x110: the players' beacon records, NULL if absent
    u8 unk150[0x174 - 0x150];         // 0x150
    u16 unk174;                       // 0x174
    u8 battleChoice[2];               // 0x176: per net id, link command 0x65 (2: cancelled)
    MailMessage ownChatMessage;       // 0x178: the player's last chat message
    u32 ownChatMessageIsNew;          // 0x180
    TrainerCardData *ownCard;         // 0x184
    TrainerCardData *cards[2];        // 0x188: per net id, the trainer cards exchanged
}; // size: 0x190

// Beacon access of the communication layer (wireless_connection_state_8.c).
void CommWireless_SetMail(MailMessage *message);
void CommWireless_SetRuleset(UnionRoomStatus *status);
void CommWireless_RefreshBeacon(void);
void CommState_SetLinkMode13RequestMode0(void);
// Declared u32 here (they return u8): the matched code truncates the results itself.
u32 CommPlayerInfo_GetCountry(int netId);
u32 CommPlayerInfo_GetRegion(int netId);
TrainerCardData *TrainerCardData_New(enum HeapID heapID);
BOOL Comm_SendCommandNoCopy(int command, void *data, int size);

int UnionRoom_TrySendTalkCancel(struct UnionRoom *unionRoom, int ret);
int UnionRoom_GetGroupOfMemberSlot(int slot);
int UnionRoom_GetPlayerActivityMsg(struct UnionRoom *unionRoom, int slot);
int UnionRoom_GetSentTalkReplyMsg(int a0, int gender, MessageFormat *msgFmt);
void UnionRoom_FillStatusMembers(UnionRoomStatus *status, int unused);
void UnionRoomStatus_Init(UnionRoomStatus *status);
void UnionRoom_ResetTalk(struct UnionRoom *unionRoom);

// gender-indexed message ids
extern const int _020FC7D4[2];
extern const int _020FC7DC[2];
extern const int _020FC7E4[2];
extern const int _020FC7EC[2];
extern const int _020FC7F4[2];
extern const int _020FC7FC[2];
extern const int _020FC804[2][2];
extern const int _020FC814[2][2];
extern const u16 gUnionRoomGroupFirstSlot[10];
extern const int _020FC838[3][2];
extern const int _020FC850[3][2];
extern const int _020FC868[3][2];
extern const int _020FC880[3][2];
extern const int gUnionRoomLanguageGreetings[7];
extern const int _020FC8B4[4][2];
extern const int _020FC8D4[4][2];
extern const int _020FC8F4[4][2];
extern const int _020FC914[4][2];
extern const int _020FC934[6][2];
extern const int _020FC95C[8][2];
extern const int _020FC99C[7][2];
extern const int gUnionRoomChatMsgs[20];

void UnionRoom_RecvTalkChoice(int netId, int size, u8 *data, FieldSystem *fieldSystem) {
    fieldSystem->unionRoom->talkChoiceReceived = 1;
    fieldSystem->unionRoom->talkChoice = *data;
    if (*data == 4) {
        CommState_SetLinkMode13RequestMode0();
    }
}

void UnionRoom_RecvTalkCancel(int netId, int size, u8 *data, FieldSystem *fieldSystem) {
    fieldSystem->unionRoom->partnerCancelled = 1;
}

BOOL UnionRoom_PartnerCancelled(struct UnionRoom *unionRoom) {
    return unionRoom->partnerCancelled;
}

int UnionRoom_TrySendTalkCancel(struct UnionRoom *unionRoom, int ret) {
    if (unionRoom->talkReply == 0) {
        Comm_SendCommand(0x68, NULL, 0);
        return ret;
    }
    return 0;
}

void UnionRoom_RecvTrainerCard(int netId, int size, void *data) {
    CommPlayerInfo_GetProfile(Comm_GetLocalNetId() ^ 1);
    ((TrainerCardData *)data)->received = 1;
}

TrainerCardData *UnionRoom_GetTrainerCardBuffer(int netId, FieldSystem *fieldSystem) {
    return fieldSystem->unionRoom->cards[netId];
}

void UnionRoom_RecvBattleChoice(int netId, int size, u8 *data, FieldSystem *fieldSystem) {
    fieldSystem->unionRoom->battleChoice[netId] = *data;
}

int UnionRoom_GetBattleCancel(struct UnionRoom *unionRoom) {
    int netId = Comm_GetLocalNetId();
    if (unionRoom->battleChoice[netId] == 2) {
        return 1;
    }
    if (unionRoom->battleChoice[netId ^ 1] == 2) {
        return 2;
    }
    return 0;
}

void UnionRoom_SendBattleChoice(int choice) {
    u8 data;
    data = choice;
    Comm_SendCommand(0x65, &data, 1);
}

int UnionRoom_GetGroupOfMemberSlot(int slot) {
    int i;
    const u16 *first = gUnionRoomGroupFirstSlot;
    for (i = 0; i < 10; i++, first++) {
        if (*first <= slot && *first + 4 > slot) {
            return i;
        }
    }
    return -1;
}

#pragma force_active on

int UnionRoom_GetPlayerActivityMsg(struct UnionRoom *unionRoom, int slot) {
    int index, gender;
    UnionRoomPeerInfo *info;
    UnionRoomStatus *status;
    PlayerProfile *profile;

    if (slot > 9) {
        index = UnionRoom_GetGroupOfMemberSlot(slot);
        GF_ASSERT(slot != -1);
    } else {
        index = slot;
    }
    if (unionRoom->records[index] == NULL) {
        return 40;
    }
    profile = CommWireless_GetParentProfile(index);
    info = &unionRoom->records[index]->info;
    status = (UnionRoomStatus *)((u8 *)info + 0x30); // the two offsets are added one after the other
    if (profile == NULL) {
        return 40;
    }
    if (slot > 9) {
        gender = status->memberAvatars[(slot - 10) % 4] >> 7;
    } else {
        gender = PlayerProfile_GetTrainerGender(profile);
    }
    switch (status->activity) {
    case 4:
    case 11:
        return _020FC7FC[gender];
    case 5:
        return _020FC814[(s32)LCRandom() % 2][gender];
    case 6:
        return _020FC8D4[(s32)LCRandom() % 4][gender];
    case 7:
        return _020FC804[(s32)LCRandom() % 2][gender];
    case 1:
    case 8:
        return _020FC8B4[(s32)LCRandom() % 4][gender];
    case 2:
    case 9:
        return _020FC8F4[(s32)LCRandom() % 4][gender];
    case 3:
    case 10:
    case 12:
    case 13:
        return _020FC914[(s32)LCRandom() % 4][gender];
    }
    return 40;
}

#pragma force_active off

int UnionRoom_GetPartnerHometownMsg(MessageFormat *msgFmt) {
    u8 myCountry = CommPlayerInfo_GetCountry(Comm_GetLocalNetId());
    u8 theirCountry = CommPlayerInfo_GetCountry(Comm_GetLocalNetId() ^ 1);
    u8 myCity = CommPlayerInfo_GetRegion(Comm_GetLocalNetId());
    u8 theirCity = CommPlayerInfo_GetRegion(Comm_GetLocalNetId() ^ 1);

    if (theirCountry == 0) {
        return 15;
    }
    if (theirCountry != 0) {
        BufferCountryName(msgFmt, 3, theirCountry);
        if (theirCity != 0) {
            BufferCityName(msgFmt, 4, theirCountry, theirCity);
        }
    }
    if (myCountry != theirCountry) {
        if (theirCity == 0) {
            return 13;
        }
        if (myCity == theirCity) {
            return 13;
        }
        return 12;
    }
    if (myCity != theirCity) {
        return 14;
    }
    return 15;
}

int UnionRoom_GetSentTalkReplyMsg(int a0, int gender, MessageFormat *msgFmt) {
    if (a0 != 0) {
        return _020FC99C[a0][gender];
    }
    return UnionRoom_GetPartnerHometownMsg(msgFmt);
}

int UnionRoom_GetTalkMsg(struct UnionRoom *unionRoom, int objId, int kind, MessageFormat *msgFmt) {
    PlayerProfile *profile;
    int gender;

    if (kind == 0) {
        return UnionRoom_GetPlayerActivityMsg(unionRoom, objId - 1);
    }
    profile = CommWireless_GetParentProfile(objId - 1);
    if (profile == NULL) {
        CommState_SetErrorWatch(1, 1);
        CommState_SetError(1);
        return 0;
    }
    gender = PlayerProfile_GetTrainerGender(profile);
    switch (kind) {
    case 1:
        return UnionRoom_GetSentTalkReplyMsg(unionRoom->sentTalkReply - 1, gender, msgFmt);
    case 2:
        return _020FC7D4[gender];
    case 9:
        if (unionRoom->sentTalkReply == 0) {
            return 0;
        }
        return _020FC95C[unionRoom->sentTalkReply][gender];
    case 19:
    case 20:
    case 21:
        kind -= 19;
        return _020FC880[kind][gender];
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        return _020FC934[kind - 3][gender];
    case 10:
    case 11:
    case 12:
        kind -= 10;
        return _020FC838[kind][gender];
    case 13:
    case 14:
    case 15:
        kind -= 13;
        return _020FC850[kind][gender];
    case 16:
    case 17:
    case 18:
        kind -= 16;
        return _020FC868[kind][gender];
    case 22:
        return _020FC7E4[gender];
    case 23:
        return _020FC7DC[gender];
    case 24:
        return _020FC7EC[gender];
    case 25:
        return 0xDA;
    case 26:
        return _020FC7F4[gender];
    }
    GF_AssertFail();
    return 40;
}

u32 UnionRoom_GetPartnerGameVersion(void) {
    PlayerProfile *profile = CommPlayerInfo_GetProfile(Comm_GetLocalNetId() ^ 1);
    GF_ASSERT(profile);
    return PlayerProfile_GetVersion(profile);
}

// The parent lists the players connected to it (net ids 1-4) as its group.
void UnionRoom_FillStatusMembers(UnionRoomStatus *status, int unused) {
    PlayerProfile *profile;
    int netId;
    int i;
    for (netId = 1; netId < 5; netId++) {
        i = netId - 1;
        profile = CommPlayerInfo_GetProfile(netId);
        if (profile != NULL) {
            if (Comm_GetLocalNetId() == 0) {
                status->memberIds[i] = PlayerProfile_GetTrainerID(profile);
                status->memberAvatars[i] = PlayerProfile_GetAvatar(profile) | (PlayerProfile_GetTrainerGender(profile) << 7);
            }
        } else {
            status->memberIds[i] = 0;
            status->memberAvatars[i] = 0;
        }
    }
}

void UnionRoom_SetBeaconActivity(int activity) {
    UnionRoomStatus status;
    MI_CpuFill8(&status, 0, sizeof(status));
    switch (activity) {
    case 11:
        UnionRoom_FillStatusMembers(&status, 2);
        break;
    case 5:
    case 6:
    case 7:
        UnionRoom_FillStatusMembers(&status, 2);
        break;
    case 8:
        UnionRoom_FillStatusMembers(&status, 5);
        break;
    case 1:
        UnionRoom_FillStatusMembers(&status, 4);
        break;
    case 9:
        UnionRoom_FillStatusMembers(&status, 5);
        break;
    case 2:
        UnionRoom_FillStatusMembers(&status, 4);
        break;
    case 10:
    case 12:
        UnionRoom_FillStatusMembers(&status, 5);
        break;
    case 3:
    case 13:
        UnionRoom_FillStatusMembers(&status, 4);
        break;
    }
    status.activity = activity;
    CommWireless_SetRuleset(&status);
    CommWireless_RefreshBeacon();
}

int UnionRoom_GetOwnChatMessageMsg(struct UnionRoom *unionRoom, MessageFormat *msgFmt) {
    int i;
    int count = 0;
    int msgNo;
    u16 word;
    for (i = 0; i < 10; i++) {
        if (unionRoom->records[i] != NULL) {
            count++;
        }
    }
    if (count != 0) {
        return 0xA6;
    }
    if (!MailMsg_IsInit(&unionRoom->ownChatMessage)) {
        return 0xA7;
    }
    if (MailMsg_GetMsgBank(&unionRoom->ownChatMessage) != 4) {
        int avatar = PlayerProfile_GetAvatar(unionRoom->profile);
        int gender = PlayerProfile_GetTrainerGender(unionRoom->profile);
        BufferTrainerClassName(msgFmt, 0, GetUnionRoomAvatarAttrBySprite(gender, avatar, 2));
        return 0xA8;
    }
    msgNo = MailMsg_GetMsgNo(&unionRoom->ownChatMessage);
    if (msgNo >= 20) {
        msgNo = 0;
    }
    word = MailMsg_GetFieldI(&unionRoom->ownChatMessage, 0);
    if (word != 0xFFFF) {
        BufferECWord(msgFmt, 0, word);
    }
    return gUnionRoomChatMsgs[msgNo];
}

void UnionRoomStatus_Init(UnionRoomStatus *status) {
    int i;
    status->activity = 0;
    for (i = 0; i < 4; i++) {
        status->memberIds[i] = 0;
        status->memberAvatars[i] = 0;
        status->unk10[i] = 0;
        status->unk14[i] = 0;
    }
}

void UnionRoom_SetOwnChatMessage(struct UnionRoom *unionRoom, MailMessage *message) {
    MailMsg_Copy(&unionRoom->ownChatMessage, message);
    unionRoom->ownChatMessageIsNew = 1;
}

MailMessage *UnionRoom_TakeOwnChatMessage(struct UnionRoom *unionRoom) {
    if (unionRoom->ownChatMessageIsNew == 0) {
        return NULL;
    }
    unionRoom->ownChatMessageIsNew = 0;
    return &unionRoom->ownChatMessage;
}

// Buffers both players' names and the partner's language, and lets the player learn the greeting in that language.
void UnionRoom_BufferPartnerLanguage(MessageFormat *msgFmt, u16 mode, u16 objId, PlayerProfile *profile, SaveEasyChat *easyChat) {
    MsgData *msgData;
    PlayerProfile *partner;
    int language;
    int msgNo;
    String *str;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x2E2, HEAP_ID_FIELD1);
    if (mode == 0) {
        partner = CommWireless_GetParentProfile(objId - 1);
    } else {
        partner = CommPlayerInfo_GetProfile(Comm_GetLocalNetId() ^ 1);
    }
    if (partner == NULL) {
        DestroyMsgData(msgData);
        return;
    }
    BufferPlayersName(msgFmt, 0, partner);
    BufferPlayersName(msgFmt, 1, profile);
    language = PlayerProfile_GetLanguage(partner);
    if (language >= 1 && language <= 7) {
        u16 idx = language - 1;
        if (idx < 7 && gUnionRoomLanguageGreetings[idx] >= 0) {
            Save_EasyChat_SetGreetingFlag(easyChat, gUnionRoomLanguageGreetings[idx]);
        }
    }
    switch (language) {
    case 1:
        msgNo = 0xD3;
        break;
    case 2:
        msgNo = 0xD4;
        break;
    case 3:
        msgNo = 0xD5;
        break;
    case 4:
        msgNo = 0xD6;
        break;
    case 5:
        msgNo = 0xD7;
        break;
    case 7:
        msgNo = 0xD8;
        break;
    default:
        msgNo = 0xD9;
        break;
    }
    str = NewString_ReadMsgData(msgData, msgNo);
    BufferString(msgFmt, 2, str, 0, 1, language);
    Heap_Free(str);
    DestroyMsgData(msgData);
}

void UnionRoom_BroadcastChatMessage(MailMessage *message) {
    UnionRoomStatus status;
    UnionRoomStatus_Init(&status);
    status.activity = 0;
    CommWireless_SetMail(message);
    CommWireless_SetRuleset(&status);
    CommWireless_RefreshBeacon();
}

void UnionRoom_ResetTalkReplies(struct UnionRoom *unionRoom) {
    unionRoom->talkReply = 0;
    unionRoom->talkChoice = 0;
    unionRoom->partnerCancelled = 0;
}

void UnionRoom_ResetTalk(struct UnionRoom *unionRoom) {
    unionRoom->connectRequest = 0;
    unionRoom->unk24 = 0;
    unionRoom->talkChoiceReceived = 0;
    unionRoom->talkReply = 0;
    unionRoom->talkChoice = 0;
    unionRoom->unk174 = 0;
    unionRoom->ownChatMessageIsNew = 0;
    unionRoom->partnerCancelled = 0;
}

// Allocates the trainer cards to exchange and builds the player's own; returns the partner's receive buffer.
TrainerCardData *UnionRoom_AllocTrainerCards(struct UnionRoom *unionRoom) {
    int gender;
    int trainerClass;
    unionRoom->ownCard = TrainerCardData_New(HEAP_ID_DEFAULT);
    unionRoom->cards[0] = TrainerCardData_New(HEAP_ID_DEFAULT);
    unionRoom->cards[1] = TrainerCardData_New(HEAP_ID_DEFAULT);
    gender = PlayerProfile_GetTrainerGender(unionRoom->profile);
    trainerClass = GetUnionRoomAvatarAttrBySprite(gender, PlayerProfile_GetAvatar(unionRoom->profile), 0);
    TrainerCardData_Build(0, 0, 0, trainerClass, unionRoom->fieldSystem, (TrainerCardAppArgs *)unionRoom->ownCard);
    return unionRoom->cards[Comm_GetLocalNetId() ^ 1];
}

void UnionRoom_FreeTrainerCards(struct UnionRoom *unionRoom) {
    Heap_Free(unionRoom->cards[0]);
    Heap_Free(unionRoom->cards[1]);
    Heap_Free(unionRoom->ownCard);
}

BOOL UnionRoom_SendTrainerCard(struct UnionRoom *unionRoom) {
    return Comm_SendCommandNoCopy(0x69, unionRoom->ownCard, sizeof(TrainerCardData));
}
