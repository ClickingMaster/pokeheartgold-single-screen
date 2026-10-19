#ifndef POKEHEARTGOLD_UNION_ROOM_INTERNAL_H
#define POKEHEARTGOLD_UNION_ROOM_INTERNAL_H

#include "global.h"

#include "field_system.h"
#include "player_data.h"
#include "save.h"
#include "sys_task_api.h"

// Union Room connection manager: FieldSystem.unionRoom while the player is in the Union Room. A SysTask polls the
// other players' records every frame and runs the current state function, which drives the wireless connection.

// What a Union Room player broadcasts in its beacon besides its profile and chat message: the activity it is busy
// with and, for a parent, the members of its group.
typedef struct UnionRoomStatus {
    u32 memberIds[4];    // trainer ids of net ids 1-4
    u8 unk10[4];
    u8 unk14[4];
    u8 memberAvatars[4]; // avatar | gender << 7
    u8 activity;
} UnionRoomStatus;

// A player's beacon record as seen by the Union Room (communication layer).
typedef struct UnionRoomRecordInfo {
    u8 unk00[0x30]; // chat message, profile
    UnionRoomStatus status;
} UnionRoomRecordInfo;

typedef struct UnionRoomRecord {
    u8 unk00[0x50];
    UnionRoomRecordInfo info;
} UnionRoomRecord;

typedef void (*UnionRoomStateFunc)(struct UnionRoom *unionRoom);

#define UNION_ROOM_MAX_RECORDS 16

struct UnionRoom {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    PlayerProfile *profile;
    SysTask *task;
    UnionRoomStateFunc state;
    int timer;        // frames before the state function runs
    int partnerNetId; // the player talked to
    u32 joinStatus;
    u32 connectRequest; // connection request, 1 or 2
    u32 unk24;
    u32 unk28;
    u32 talkChoiceReceived;
    u32 talkReply;
    u32 sentTalkReply;
    u8 pad38[4];
    u32 unk3C;
    int talkChoice;
    u32 partnerCancelled;
    u8 pad48[0x110 - 0x48];
    UnionRoomRecord *records[UNION_ROOM_MAX_RECORDS]; // the players' records, refreshed every frame
    u8 random[0x18];                                  // CommRandom
    u8 pad168[0xC];
    u16 unk174;
    u8 battleChoice[2];         // per net id; 2 = cancelled the battle party selection
    MailMessage ownChatMessage; // the player's own chat message, read by the chat log
    u32 ownChatMessageIsNew;
    struct TrainerCardData *ownCard;
    struct TrainerCardData *cards[2]; // per net id, link receive buffers
};

// Mirror of the last record looked at and the last choice received (written only).
typedef struct UnionRoomScratch {
    u32 waitFrames;
    UnionRoomStatus *lastActivity;
    u32 lastChoice;
} UnionRoomScratch;

extern UnionRoomScratch gUnionRoomScratch;
extern void *gUnionRoomRecords[UNION_ROOM_MAX_RECORDS];

struct UnionRoom *UnionRoom_Start(FieldSystem *fieldSystem);
void UnionRoom_Stop(FieldSystem *fieldSystem);
struct UnionRoom *UnionRoom_New(FieldSystem *fieldSystem);
void UnionRoom_WaitWireless(struct UnionRoom *unionRoom);
void UnionRoom_Search(struct UnionRoom *unionRoom);
void UnionRoom_WaitSearchReady(struct UnionRoom *unionRoom);
BOOL UnionRoom_AnyOtherPlayerPresent(void);
void UnionRoom_ParentLinked(struct UnionRoom *unionRoom);
void UnionRoom_ParentInSession(struct UnionRoom *unionRoom);
void UnionRoom_SetState(struct UnionRoom *unionRoom, UnionRoomStateFunc state, int timer);
void Task_UnionRoom(SysTask *task, struct UnionRoom *unionRoom);
void UnionRoom_StopSearch(struct UnionRoom *unionRoom);
void UnionRoom_WaitCommEnd(struct UnionRoom *unionRoom);
void UnionRoom_WaitJoin(struct UnionRoom *unionRoom);
void UnionRoom_WaitFieldTask(struct UnionRoom *unionRoom);
void UnionRoom_WaitJoinedProfile(struct UnionRoom *unionRoom);
void UnionRoom_ChildInSession(struct UnionRoom *unionRoom);
void UnionRoom_Delete(struct UnionRoom *unionRoom);
FieldSystem *UnionRoom_GetFieldSystem(struct UnionRoom *unionRoom);
UnionRoomRecord *UnionRoom_GetRecord(struct UnionRoom *unionRoom, int netId);
int UnionRoom_GetPlayerActivityType(struct UnionRoom *unionRoom, int objId);
int UnionRoom_RequestJoinPlayer(struct UnionRoom *unionRoom, int objId, int kind);
u32 UnionRoom_GetJoinStatus(struct UnionRoom *unionRoom);
int UnionRoom_GetTalkChoice(struct UnionRoom *unionRoom);
int UnionRoom_GetTalkReply(struct UnionRoom *unionRoom);
void UnionRoom_SendTalkResponse(struct UnionRoom *unionRoom, int mode, int value);
void UnionRoom_RecvUnusedProfile(void);
void UnionRoom_RecvUnused(void);
void UnionRoom_RecvRestartSearch(int netId, int size, void *data, FieldSystem *fieldSystem);
void UnionRoom_RecvTalkReply(int netId, int size, const u8 *data, FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_UNION_ROOM_INTERNAL_H
