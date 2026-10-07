#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_02261FD4.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_input.h"
#include "battle/battle_system.h"
#include "filesystem.h"
#include "heap.h"
#include "sound.h"
#include "sys_task_api.h"
#include "constants/sndseq.h"
void PlaySE(u16 seqNo);

typedef struct TargetSelectData {
    BattleSystem *battleSystem;
    BattleHpBar *hpBar;
    int input;
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 state;
    TargetPokemon targetMons[4];
    u16 moveRange;
    u8 unk32;
} TargetSelectData;

void ov12_0226311C(BattleSystem *bsys, int battlerId, u32 msg);
void ov12_0226430C(BattleSystem *bsys, int battlerId, int command);
void ov12_0225E568(SysTask *task, TargetSelectData *data);
// @fn ov12_0225E568
void ov12_0225E568(SysTask *task, TargetSelectData *data) {
    BgConfig *bgConfig = BattleSystem_GetBgConfig(data->battleSystem);
    OpponentData *opponent = BattleSystem_GetOpponentData(data->battleSystem, data->battlerId);
    BattleInput *battleInput = BattleSystem_GetBattleInput(data->battleSystem);
    int partner = BattleSystem_GetBattlerIdPartner(data->battleSystem, data->battlerId);
    BattleHpBar *hpBar;

    if (partner != data->battlerId) {
        hpBar = BattleSystem_GetHpBar(data->battleSystem, partner);
    } else {
        hpBar = NULL;
    }

    switch (data->state) {
    case 0:
        if (BattleInput_CheckFeedbackDone(battleInput) != 0) {
            NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            BattleInputTargetMenu menu;
            int i;

            for (i = 0; i < 4; i++) {
                menu.targetMons[i] = data->targetMons[i];
            }
            menu.battlerType = data->battlerType;
            menu.selectionType = ov12_02266C84(data->moveRange, data->battlerType);
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 12, 0, (int *)&menu);
            NARC_Delete(narc0);
            NARC_Delete(narc1);
            data->state++;
        }
        break;
    case 1:
        data->input = BattleInput_CheckTouch(battleInput);
        if (data->input != -1) {
            PlaySE(SEQ_SE_DP_DECIDE);
            data->state++;
        }
        break;
    case 2:
        if (data->input != 0xFF) {
            ov12_02264EB4(data->hpBar);
            ov12_02262014(opponent);
            ov12_02265D74(hpBar);
            if (data->unk32 == 1) {
                BattleInput_Deadstriped_022698AC(battleInput, 0);
            }
        }
        data->state++;
        break;
    case 3:
        data->state++;
    default:
        if (ov12_022698B0(battleInput) == 1) {
            int input = data->input;
            u32 battleType = BattleSystem_GetBattleType(data->battleSystem);
            u8 order[6];
            if (input != 0xFF) {
                ov12_0223C1A0(data->battleSystem, order);
                if (battleType & BATTLE_TYPE_DOUBLES) {
                    input = order[data->input + 1] + 1;
                } else {
                    input = order[data->input - 1] + 1;
                }
            }
            ov12_0226311C(data->battleSystem, data->battlerId, input);
            ov12_0226430C(data->battleSystem, data->battlerId, data->command);
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}
