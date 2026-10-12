#ifndef POKEHEARTGOLD_UNION_ROOM_INTERNAL_H
#define POKEHEARTGOLD_UNION_ROOM_INTERNAL_H

#include "global.h"

#include "field_system.h"
#include "player_data.h"
#include "save.h"
#include "sys_task_api.h"

// Union Room connection manager (ARM9 static 0x02059DB0-0x0205A44C): FieldSystem.unk80 while the player is in the
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

typedef void (*UnionRoomStateFunc)(struct UnkStruct_02059E1C *unionRoom);

#define UNION_ROOM_MAX_RECORDS 16

struct UnkStruct_02059E1C {
    FieldSystem *fieldSystem;               // 0x00
    SaveData *saveData;                     // 0x04
    PlayerProfile *profile;                 // 0x08
    SysTask *task;                          // 0x0C
    UnionRoomStateFunc state;               // 0x10
    int timer;                              // 0x14: frames before the state function runs
    int partnerNetId;                       // 0x18: the player talked to
    u32 unk1C;                              // 0x1C
    u32 unk20;                              // 0x20: connection request, 1 or 2
    u32 unk24;                              // 0x24
    u32 unk28;                              // 0x28
    u32 unk2C;                              // 0x2C
    u32 unk30;                              // 0x30
    u32 unk34;                              // 0x34
    u8 pad38[4];                            // 0x38
    u32 unk3C;                              // 0x3C
    int unk40;                              // 0x40
    u32 unk44;                              // 0x44
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

struct UnkStruct_02059E1C *UnionRoom_Start(FieldSystem *fieldSystem);
void UnionRoom_Stop(FieldSystem *fieldSystem);
struct UnkStruct_02059E1C *UnionRoom_New(FieldSystem *fieldSystem);
void sub_02059E88(struct UnkStruct_02059E1C *unionRoom);
void sub_02059EBC(struct UnkStruct_02059E1C *unionRoom);
void sub_02059F30(struct UnkStruct_02059E1C *unionRoom);
BOOL UnionRoom_AnyOtherPlayerPresent(void);
void sub_02059F78(struct UnkStruct_02059E1C *unionRoom);
void sub_02059FF8(struct UnkStruct_02059E1C *unionRoom);
void UnionRoom_SetState(struct UnkStruct_02059E1C *unionRoom, UnionRoomStateFunc state, int timer);
void Task_UnionRoom(SysTask *task, struct UnkStruct_02059E1C *unionRoom);
void sub_0205A07C(struct UnkStruct_02059E1C *unionRoom);
void sub_0205A0A0(struct UnkStruct_02059E1C *unionRoom);
void sub_0205A0B4(struct UnkStruct_02059E1C *unionRoom);
void sub_0205A114(struct UnkStruct_02059E1C *unionRoom);
void sub_0205A144(struct UnkStruct_02059E1C *unionRoom);
void sub_0205A1AC(struct UnkStruct_02059E1C *unionRoom);
void UnionRoom_Delete(struct UnkStruct_02059E1C *unionRoom);
FieldSystem *UnionRoom_GetFieldSystem(struct UnkStruct_02059E1C *unionRoom);
UnionRoomRecord *UnionRoom_GetRecord(struct UnkStruct_02059E1C *unionRoom, int netId);
int sub_0205A200(struct UnkStruct_02059E1C *unionRoom, int objId);
int sub_0205A284(struct UnkStruct_02059E1C *unionRoom, int objId, int kind);
u32 sub_0205A358(struct UnkStruct_02059E1C *unionRoom);
int sub_0205A35C(struct UnkStruct_02059E1C *unionRoom);
int sub_0205A39C(struct UnkStruct_02059E1C *unionRoom);
void sub_0205A3B0(struct UnkStruct_02059E1C *unionRoom, int mode, int value);
void sub_0205A408(void);
void sub_0205A40C(void);
void sub_0205A410(int netId, int size, void *data, FieldSystem *fieldSystem);
void sub_0205A430(int netId, int size, const u8 *data, FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_UNION_ROOM_INTERNAL_H
