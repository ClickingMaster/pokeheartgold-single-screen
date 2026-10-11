#include "battle/to56_battle_task_state_pair_w20_private.h"

void ov12_0225FCC0(SysTask *task, W20TaskSmall *work) {
    u8 value;
    if (ov12_0223BE0C(work->battleSystem, work->battlerId, &value) == 1) {
        ov12_02261ED4(work->battleSystem);
    }
    if (value == 0 || (value != 0xFF && value != 1)) {
        ov12_02261EB8(work->battleSystem);
    }
    BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, value);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void ov12_0225FD14(SysTask *task, W20TaskLarge *work) {
    W20BattleSourceLocal source0;
    W20BattleParams params2;
    W20BattleSourceLocal source2;
    W20BattleSourceLocal source6;
    W20BattleParams params8;
    W20BattleSourceLocal source8;
    u8 battlerId;

    switch (work->state) {
    case 0:
        if (work->source.active && work->source.mode4C == 0 && Pokepic_GetAttr(work->pokepic, 6) == 0) {
            battlerId = work->battlerId;
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &source0, 1, 15, battlerId, battlerId, 0);
            ov12_02261B80(work->battleSystem, work->context, work->object, &source0);
            work->state = 1;
        } else {
            work->state = 4;
        }
        return;
    case 2:
        ov12_02261CA8(work->battleSystem, &work->source, &params2, work->battlerId);
        ov07_0223494C(&params2, 5);
        battlerId = work->battlerId;
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &source2, 1, 16, battlerId, battlerId, 0);
        ov12_02261B80(work->battleSystem, work->context, work->object, &source2);
        *(int *)((u8 *)work->context + 0x1A0) = 0;
        work->state++;
        return;
    case 4:
        BattleSystem_SetCriticalHpMusicFlag(work->battleSystem, 2);
        if (work->disableHpBar) {
            BattleSystem_SetHpBarDisabled(work->battleSystem);
        }
        if (work->updateG3) {
            PokepicManager_SetG3UpdateFlagsMask(BattleSystem_GetPokepicManager(work->battleSystem), 1);
        }
        ov12_02261B80(work->battleSystem, work->context, work->object, &work->source);
        work->state++;
        return;
    case 6:
        BattleSystem_SetCriticalHpMusicFlag(work->battleSystem, 0);
        if (work->disableHpBar) {
            BattleSystem_SetHpBarEnabled(work->battleSystem);
        }
        if (work->updateG3) {
            PokepicManager_ResetG3UpdateFlagsMask(BattleSystem_GetPokepicManager(work->battleSystem), 1);
        }
        if (work->source.active && work->source.mode4C == 0 && Pokepic_GetAttr(work->pokepic, 6) == 0) {
            battlerId = work->battlerId;
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &source6, 1, 15, battlerId, battlerId, 0);
            ov12_02261B80(work->battleSystem, work->context, work->object, &source6);
            work->state = 7;
        } else {
            work->state = 10;
        }
        return;
    case 8:
        ov12_02261CA8(work->battleSystem, &work->source, &params8, work->battlerId);
        ov07_02234A20(&params8, 5);
        battlerId = work->battlerId;
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &source8, 1, 16, battlerId, battlerId, 0);
        ov12_02261B80(work->battleSystem, work->context, work->object, &source8);
        *(int *)((u8 *)work->context + 0x1A0) = 1;
        work->state++;
        return;
    case 10:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->value68);
        Heap_Free(work);
        SysTask_Destroy(task);
        return;
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
        ov07_0221C394(work->object);
        if (ov07_0221C3B0(work->object) == 0) {
            ov07_0221C3C0(work->object);
            work->state++;
        }
        return;
    }
}
