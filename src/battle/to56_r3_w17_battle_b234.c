#include "global.h"

#include "constants/heap.h"

#include "battle/battle_system.h"

#include "heap.h"
#include "sys_task_api.h"

#define U8_AT(p, n)  (((u8 *)(p))[n])
#define U32_AT(p, n) (*(u32 *)((u8 *)(p) + (n)))

typedef struct Ov12TaskWork16 {
    BattleSystem *battleSystem;
    void *opponentData;
    void *unk08;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 pad0F;
} Ov12TaskWork16;

typedef struct Ov12MessageTaskWork {
    BattleSystem *battleSystem;
    u8 messageId;
    u8 battlerId;
    u8 printerId;
    u8 pad07;
} Ov12MessageTaskWork;

void ov12_02260CDC(SysTask *task, void *data);
void ov12_02260D28(SysTask *task, void *data);
void ov12_022605D0(SysTask *task, void *data);
void ov12_02261928(BattleSystem *battleSystem, void *opponentData, BattleMessage *message);
void ov12_022619E4(BattleSystem *battleSystem, void *opponentData, void *command, BattleMessage *message);
void ov12_02261AD4(BattleSystem *battleSystem, void *opponentData, BattleMessage *message);

void ov12_0225B234(BattleSystem *battleSystem, void *opponentData);
void ov12_0225B26C(BattleSystem *battleSystem, void *opponentData);
void ov12_0225B2A4(BattleSystem *battleSystem, void *opponentData);
void ov12_0225B2F8(BattleSystem *battleSystem, void *opponentData, void *command);
void ov12_0225B34C(BattleSystem *battleSystem, void *opponentData);

void ov12_0225B234(BattleSystem *battleSystem, void *opponentData) {
    Ov12TaskWork16 *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(Ov12TaskWork16));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = U8_AT(opponentData, 0x94);
    work->battlerId = U8_AT(opponentData, 0x194);
    work->unk08 = (void *)U32_AT(opponentData, 0x20);
    SysTask_CreateOnMainQueue(ov12_02260CDC, work, 0);
}

void ov12_0225B26C(BattleSystem *battleSystem, void *opponentData) {
    Ov12TaskWork16 *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(Ov12TaskWork16));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = U8_AT(opponentData, 0x94);
    work->battlerId = U8_AT(opponentData, 0x194);
    work->unk08 = (void *)U32_AT(opponentData, 0x20);
    SysTask_CreateOnMainQueue(ov12_02260D28, work, 0);
}

void ov12_0225B2A4(BattleSystem *battleSystem, void *opponentData) {
    BattleMessage message;
    MsgData *msgData;
    Ov12MessageTaskWork *work;

    ov12_02261928(battleSystem, opponentData, &message);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(Ov12MessageTaskWork));
    work->battleSystem = battleSystem;
    work->messageId = 0x3B;
    work->battlerId = U8_AT(opponentData, 0x194);
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &message, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(ov12_022605D0, work, 0);
}

void ov12_0225B2F8(BattleSystem *battleSystem, void *opponentData, void *command) {
    BattleMessage message;
    MsgData *msgData;
    Ov12MessageTaskWork *work;

    ov12_022619E4(battleSystem, opponentData, command, &message);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(Ov12MessageTaskWork));
    work->battleSystem = battleSystem;
    work->messageId = 0x3C;
    work->battlerId = U8_AT(opponentData, 0x194);
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &message, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(ov12_022605D0, work, 0);
}

void ov12_0225B34C(BattleSystem *battleSystem, void *opponentData) {
    BattleMessage message;
    MsgData *msgData;
    Ov12MessageTaskWork *work;

    ov12_02261AD4(battleSystem, opponentData, &message);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(Ov12MessageTaskWork));
    work->battleSystem = battleSystem;
    work->messageId = 0x3D;
    work->battlerId = U8_AT(opponentData, 0x194);
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &message, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(ov12_022605D0, work, 0);
}
