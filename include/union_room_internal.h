#ifndef POKEHEARTGOLD_UNION_ROOM_INTERNAL_H
#define POKEHEARTGOLD_UNION_ROOM_INTERNAL_H

#include "global.h"

#include "field_system.h"
#include "player_data.h"
#include "save.h"
#include "sys_task_api.h"

// Union Room connection manager (ARM9 static 0x02059DB0-0x0205A44C): FieldSystem.unionRoom while the player is in the
// Union Room. A SysTask polls the other players' records every frame and runs the current state function, which
// drives the wireless connection (searching, connecting as parent or child, disconnecting). The rest of the object
// (talk messages, 0x0205A44C+) is handled by the Union Room talk code, which keeps its own view of this struct
// with the same unkXX field names.

// A player's record as polled by the Union Room task; the activity kind is what the player is doing.
typedef struct UnionRoomActivity {
    u8 unk00[0x1C];
    u8 kind; // 0x1C
} UnionRoomActivity;

typedef struct UnionRoomRecordInfo {
    u8 unk00[0x30];
    UnionRoomActivity activity; // 0x30
} UnionRoomRecordInfo;

typedef struct UnionRoomRecord {
    u8 unk00[0x50];
    UnionRoomRecordInfo info; // 0x50
} UnionRoomRecord;

typedef void (*UnionRoomStateFunc)(struct UnionRoom *unionRoom);

#define UNION_ROOM_MAX_RECORDS 16

struct UnionRoom {
    FieldSystem *fieldSystem;               // 0x00
    SaveData *saveData;                     // 0x04
    PlayerProfile *profile;                 // 0x08
    SysTask *task;                          // 0x0C
    UnionRoomStateFunc state;               // 0x10
    int timer;                              // 0x14: frames before the state function runs
    int partnerNetId;                       // 0x18: the player talked to
    u32 joinStatus;                         // 0x1C
    u32 connectRequest;                     // 0x20: connection request, 1 or 2
    u32 unk24;                              // 0x24
    u32 unk28;                              // 0x28
    u32 talkChoiceReceived;                 // 0x2C
    u32 talkReply;                          // 0x30
    u32 sentTalkReply;                              // 0x34
    u8 pad38[4];                            // 0x38
    u32 unk3C;                              // 0x3C
    int talkChoice;                         // 0x40
    u32 partnerCancelled;                   // 0x44
    u8 pad48[0x110 - 0x48];                 // 0x48
    UnionRoomRecord *records[UNION_ROOM_MAX_RECORDS]; // 0x110: the players' records, refreshed every frame
    u8 random[0x18];                        // 0x150: CommRandom (communication_core_internal.h)
    u8 pad168[0xC];                         // 0x168
    u16 unk174;                             // 0x174
    u8 battleChoice[2];                     // 0x176: per net id; 2 = cancelled the battle party selection
    MailMessage ownChatMessage;             // 0x178: the player's own chat message, read by the chat log
    u32 ownChatMessageIsNew;                // 0x180
    struct TrainerCardData *ownCard;        // 0x184
    struct TrainerCardData *cards[2];       // 0x188: per net id, link receive buffers
}; // size: 0x190

// Mirror of the last record looked at and the last choice received (written only).
typedef struct UnionRoomScratch {
    u32 waitFrames;
    UnionRoomActivity *lastActivity;
    u32 lastChoice;
} UnionRoomScratch;

extern UnionRoomScratch _021D41CC;
extern void *_021D41D8[UNION_ROOM_MAX_RECORDS];

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
