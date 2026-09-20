#include "to56_battle_r08_private.h"

void ov12_0225AC1C(BattleSystem *battleSystem, OpponentData *opponentData, const u8 *commandBytes) {
    const R4Command *command = (const R4Command *)commandBytes;
    R4Work *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(R4Work));
    int i;

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = command->command;
    work->battler = opponentData->unk194;
    work->state = 0;
    work->param = command->param;
    work->value64 = command->value;
    for (i = 0; i < 4; i++) {
        work->values20[i] = command->values04[i];
        work->values28[i] = command->values0C[i];
        work->values2C[i] = command->values10[i];
        work->values30[i] = command->values14[i];
        work->values34[i] = command->values18[i];
    }
    SysTask_CreateOnMainQueue((SysTaskFunc)ov12_02260418, work, 0);
}

void ov12_0225ACB0(BattleSystem *battleSystem, OpponentData *opponentData, const u8 *command) {
    if (*(u32 *)((u8 *)opponentData + 0x2C) == 0) {
        GF_AssertFail();
    }
    *((u8 *)opponentData + 0x72) = command[1];
    ov12_0226498C(&opponentData->hpBar, *(u32 *)((u8 *)opponentData + 0x50), 0x100);
    ov12_0226430C(battleSystem, opponentData->unk194, command[0]);
}

void ov12_0225ACE8(BattleSystem *battleSystem, OpponentData *opponentData, const u8 *command) {
    int trainer = BattleSystem_GetTrainerIndex(battleSystem, opponentData->unk194);
    R4TextWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(R4TextWork));

    work->battleSystem = battleSystem;
    work->command = command[0];
    work->battler = opponentData->unk194;
    work->printer = BattleSystem_PrintTrainerMessage(battleSystem, trainer, opponentData->unk194, command[1], BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue((SysTaskFunc)ov12_022605D0, work, 0);
}

void ov12_0225AD44(BattleSystem *battleSystem, OpponentData *opponentData, const u8 *command) {
    BattleMessage message;
    R4TextWork *work;
    MsgData *loader;

    ov12_02261390(battleSystem, opponentData, command, &message);
    loader = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(R4TextWork));
    work->battleSystem = battleSystem;
    work->command = command[0];
    work->battler = opponentData->unk194;
    work->printer = BattleSystem_PrintBattleMessage(battleSystem, loader, &message, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue((SysTaskFunc)ov12_022605D0, work, 0);
}

void ov12_0225AD9C(BattleSystem *battleSystem, OpponentData *opponentData, const u8 *command) {
    BattleMessage message;
    R4TextWork *work;
    MsgData *loader;

    ov12_02261464(battleSystem, opponentData, command, &message);
    loader = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(R4TextWork));
    work->battleSystem = battleSystem;
    work->command = command[0];
    work->battler = opponentData->unk194;
    work->printer = BattleSystem_PrintBattleMessage(battleSystem, loader, &message, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue((SysTaskFunc)ov12_022605D0, work, 0);
}

void ov12_0225ADF4(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleMessage message;
    R4TextWork *work;
    MsgData *loader;

    ov12_02261544(battleSystem, opponentData, &message);
    loader = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(R4TextWork));
    work->battleSystem = battleSystem;
    work->command = 0x22;
    work->battler = opponentData->unk194;
    work->printer = BattleSystem_PrintBattleMessage(battleSystem, loader, &message, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue((SysTaskFunc)ov12_022605D0, work, 0);
}

void ov12_0225AE48(BattleSystem *battleSystem, OpponentData *opponentData, const u8 *command) {
    BattleMessage message;
    R4TextWork *work;
    MsgData *loader;

    ov12_022615F0(battleSystem, opponentData, command, &message);
    loader = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(R4TextWork));
    work->battleSystem = battleSystem;
    work->command = command[0];
    work->battler = opponentData->unk194;
    work->printer = BattleSystem_PrintBattleMessage(battleSystem, loader, &message, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue((SysTaskFunc)ov12_022605D0, work, 0);
}
