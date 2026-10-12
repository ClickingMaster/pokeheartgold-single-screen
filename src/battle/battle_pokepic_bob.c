#include "battle/battle_pokepic_bob.h"

#include "battle/battle_system.h"

#include "math_util.h"
#include "pokepic.h"

static void Task_OpponentPokepicBob(SysTask *task, void *_data);

void OpponentData_StartPokepicBobTask(OpponentData *data, BattleSystem *battleSystem) {
    if (data->pokepicBobTask == NULL && !(BattleSystem_GetBattleType(battleSystem) & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_PAL_PARK))) {
        data->pokepicBobAngle = 180;
        data->pokepicBobTask = SysTask_CreateOnMainQueue(Task_OpponentPokepicBob, data, 0x3F2);
    }
}

void OpponentData_StopPokepicBobTask(OpponentData *data) {
    if (data->pokepicBobTask != NULL) {
        SysTask_Destroy(data->pokepicBobTask);
        data->pokepicBobTask = NULL;
        data->pokepicBobAngle = 0;
        Pokepic_SetAttr(data->pokepic, POKEPIC_YOFFSET, 0);
    }
}

static void Task_OpponentPokepicBob(SysTask *task, void *_data) {
    OpponentData *data = _data;

    data->pokepicBobAngle += 20;

    if (data->pokepicBobAngle >= 360) {
        data->pokepicBobAngle -= 360;
    }

    int dy = FX_Mul(GF_SinDegNoWrap(data->pokepicBobAngle), FX32_CONST(1.5)) / FX32_ONE;

    Pokepic_SetAttr(data->pokepic, POKEPIC_YOFFSET, dy);
}
