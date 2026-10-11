#include "battle/to57_bank_battle_residual16_private.h"

void ov12_0225E250(SysTask *task, BattleWork16 *work) {
    BgConfig *bgConfig = BattleSystem_GetBgConfig(work->battleSystem);
    BattleInput *battleInput = BattleSystem_GetBattleInput(work->battleSystem);
    OpponentData *opponent = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
    int partner = BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId);
    BattleHpBar *hpBar;

    if (partner != work->battlerId) {
        hpBar = BattleSystem_GetHpBar(work->battleSystem, partner);
    } else {
        hpBar = NULL;
    }

    switch (work->state) {
    case 0:
        if (BattleInput_CheckFeedbackDone(battleInput) == 0) {
            break;
        }
        {
            MsgData *loader = BattleSystem_GetMessageLoader(work->battleSystem);
            BattleMessage msg;
            NARC *narc0;
            NARC *narc1;
            BattleMoveMenuData16 menu;
            int i;

            msg.tag = 2;
            msg.param[0] = work->battlerId | (work->battlerType << 8);
            msg.id = 0x399;
            BattleSystem_PrintBattleMessage(work->battleSystem, loader, &msg, 0);
            BattleInput_EnableBallGauge(battleInput);
            narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            for (i = 0; i < 4; i++) {
                menu.moves[i] = work->moves[i];
                menu.pp[i] = work->pp[i];
                menu.ppMax[i] = work->ppMax[i];
            }
            menu.partySlot = work->partySlot;
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 11, 0, (int *)&menu);
            NARC_Delete(narc0);
            NARC_Delete(narc1);
            work->state++;
            return;
        }
    case 1:
        work->input = BattleInput_CheckTouch(battleInput);
        if (work->input == 0xFFFFFFFF) {
            break;
        }
        PlaySE(0x5DD);
        work->state++;
        return;
    case 2:
        if (work->input != 0xFF) {
            if (!(BattleSystem_GetBattleType(work->battleSystem) & 2)) {
                BattleInput_Deadstriped_022698AC(battleInput, 0);
                ov12_02265D74(hpBar);
                ov12_02264EB4(work->hpBar);
                ov12_02262014(opponent);
            }
            BattleInput_DisableBallGauge(battleInput);
        }
        BattleController_ReplyMoveSelection(work->battleSystem, work->battlerId, work->input);
        work->state++;
        return;
    case 3:
        work->state++;
        /* fallthrough */
    default:
        if (ov12_022698B0(battleInput) == 1) {
            ov12_0223BB10(work->battleSystem, 1);
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
    (void)bgConfig;
}

void ov12_0225E404(SysTask *task, BattleWork16 *work) {
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    BattleContext *ctx = BattleSystem_GetBattleContext(work->battleSystem);
    int selected;

    if ((battleType & 0x101) || (BattleSystem_GetBattleSpecial(work->battleSystem) & 1)
        || BattleSystem_GetFieldSide(work->battleSystem, work->battlerId) == 0) {
        selected = ov10_0221BEF4(work->battleSystem, work->battlerId);
        if (selected == 0xFF) {
            return;
        }
        selected++;
    } else {
        int choices[4];
        int count = 0;
        int i = count;
        int *choice = choices;
        int data;

        for (; i < 4; i++) {
            if (!(MaskOfFlagNo(i) & work->excludedMask)) {
                *choice++ = i + 1;
                count++;
            }
        }
        selected = choices[BattleSystem_Random(work->battleSystem) % count];
        data = ov12_022506D4(work->battleSystem, ctx, work->battlerId, work->moves[selected - 1], 1, 0);
        ov12_022582B8(work->battleSystem, ctx, 11, work->battlerId, data);
    }
    BattleController_ReplyMoveSelection(work->battleSystem, work->battlerId, selected);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}
