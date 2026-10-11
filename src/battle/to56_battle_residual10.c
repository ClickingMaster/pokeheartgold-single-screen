#include "battle/to56_battle_residual10_private.h"
void ov12_0225B0A0(void *battleSystem, u8 *opponentData, u8 *command) {
    W02TaskData *data = Heap_Alloc(HEAP_ID_BATTLE, sizeof(W02TaskData));
    int i;
    data->unk07 = 0;
    data->battleSystem = battleSystem;
    data->command = command[0];
    data->battlerId = opponentData[0x194];
    data->value195 = opponentData[0x195];
    for (i = 0; i < 6; i++) {
        data->payload[i] = command[i + 2];
    }
    data->mode = 0;
    SysTask_CreateOnMainQueue(ov12_02260BA0, data, 0);
}
void ov12_0225B0E8(void *battleSystem, u8 *opponentData, u8 *command) {
    W02TaskData *data = Heap_Alloc(HEAP_ID_BATTLE, sizeof(W02TaskData));
    data->unk07 = 0;
    data->battleSystem = battleSystem;
    data->command = command[0];
    data->battlerId = opponentData[0x194];
    data->value195 = opponentData[0x195];
    data->mode = 0;
    SysTask_CreateOnMainQueue(ov12_02260C58, data, 0);
}
void ov12_0225B120(void *battleSystem, u8 *opponentData, u8 *command) {
    W02TaskData *data = Heap_Alloc(HEAP_ID_BATTLE, sizeof(W02TaskData));
    int i;
    data->unk07 = 0;
    data->battleSystem = battleSystem;
    data->command = command[0];
    data->battlerId = opponentData[0x194];
    data->value195 = opponentData[0x195];
    for (i = 0; i < 6; i++) {
        data->payload[i] = command[i + 2];
    }
    data->mode = 1;
    SysTask_CreateOnMainQueue(ov12_02260BA0, data, 0);
}
void ov12_0225B16C(void *battleSystem, u8 *opponentData, u8 *command) {
    W02TaskData *data = Heap_Alloc(HEAP_ID_BATTLE, sizeof(W02TaskData));
    data->unk07 = 0;
    data->battleSystem = battleSystem;
    data->command = command[0];
    data->battlerId = opponentData[0x194];
    data->value195 = opponentData[0x195];
    data->mode = 1;
    SysTask_CreateOnMainQueue(ov12_02260C58, data, 0);
}
void ov12_0225B1A8(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleMessage msg;
    void *waitingIcon;
    MsgData *loader;
    if (opponentData->unk196 == 0) {
        loader = BattleSystem_GetMessageLoader(battleSystem);
        msg.id = 0x39B;
        msg.tag = 0;
        BattleSystem_PrintBattleMessage(battleSystem, loader, &msg, 0);
        waitingIcon = WaitingIcon_New(BattleSystem_GetWindow(battleSystem, 0), 1);
        ov12_0223BB80(battleSystem, waitingIcon);
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->unk194, 0x37);
}
