#include "to56_battle_r15_w09_private.h"

void ov12_0225DAD4(SysTask *task, CommandSetData *work) {
    BgConfig *bgConfig = BattleSystem_GetBgConfig(work->battleSystem);
    BattleInput *battleInput = BattleSystem_GetBattleInput(work->battleSystem);
    OpponentData *opponent = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    int partner = BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId);
    BattleHpBar *hpBar;

    if (partner != work->battlerId) {
        hpBar = BattleSystem_GetHpBar(work->battleSystem, partner);
    } else {
        hpBar = NULL;
    }

    switch (work->state) {
    case 0: {
        BattleInputMove moveDisplay;
        int i;
        for (i = 0; i < 4; i++) {
            moveDisplay.moveNo[i] = work->moves[i];
            moveDisplay.pp[i] = work->curPP[i];
            moveDisplay.ppMax[i] = work->maxPP[i];
        }
        BattleInput_LoadFightMenuText(BattleSystem_GetBattleInput(work->battleSystem), work->battlerType, &moveDisplay);
        work->state++;
    }
    case 1: {
        MsgData *loader = BattleSystem_GetMessageLoader(work->battleSystem);
        BattleMessage msg;
        if (battleType & 0x400) {
            switch (ov12_0223BB04(work->battleSystem)) {
            case 0:
                msg.tag = 2;
                msg.param[0] = work->battlerId | (work->partySlot << 8);
                msg.id = 0x399;
                BattleSystem_PrintBattleMessage(work->battleSystem, loader, &msg, 0);
                work->state = 3;
                break;
            case 1:
                msg.tag = 0;
                msg.id = 0x4CA + BattleSystem_GetTrainerGender(work->battleSystem, 0);
                work->msgIdx = BattleSystem_PrintBattleMessage(work->battleSystem, loader, &msg, BattleSystem_GetTextFrameDelay(work->battleSystem));
                work->state = 2;
                break;
            }
        } else {
            if (battleType & 0x20) {
                msg.tag = 8;
                msg.param[0] = BattleSystem_GetTrainerIndex(work->battleSystem, work->battlerId);
                msg.id = 0x39A;
            } else if (battleType & 0x200) {
                msg.tag = 8;
                msg.param[0] = BattleSystem_GetTrainerIndex(work->battleSystem, work->battlerId);
                msg.id = 0x4C6;
            } else {
                msg.tag = 2;
                msg.param[0] = work->battlerId | (work->partySlot << 8);
                msg.id = 0x399;
            }
            BattleSystem_PrintBattleMessage(work->battleSystem, loader, &msg, 0);
            work->state = 3;
        }
        break;
    }
    case 2:
        if (TextPrinterCheckActive(work->msgIdx) == FALSE) {
            work->state = 3;
        }
        break;
    case 3:
        ov12_02264E84(work->hpBar);
        ov12_02261FD4(opponent, work->battleSystem);
        work->state = 4;
    case 4:
        if (BattleInput_CheckFeedbackDone(battleInput) != 0) {
            NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            ActionSelectData action;
            action.battlerId = work->battlerId;
            action.battlerType = work->battlerType;
            action.partySlot = work->partySlot;
            action.curHP = work->curHP;
            action.maxHP = work->maxHP;
            action.ballStatusBattler = work->ballStatusBattler;
            action.waitingForPartner = ov12_0223B580(work->battleSystem, work->battlerId, work->switchingMask);
            if (opponent->unk197 != 0) {
                BattleInput_Deadstriped_022698AC(battleInput, 1);
                if (BattleSystem_GetBattleSpecial(work->battleSystem) & 1) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 6, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & 0x200) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 10, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & 0x20) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 8, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & 0x1000) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 20, 0, (int *)&action);
                } else if (action.battlerType == 4 && !(BattleSystem_GetBattleType(work->battleSystem) & 8)) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 4, 0, (int *)&action);
                } else {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 3, 0, (int *)&action);
                }
                ov12_0223BB64(work->battleSystem, 1);
            } else {
                if (BattleSystem_GetBattleSpecial(work->battleSystem) & 1) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 5, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & 0x200) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 9, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & 0x20) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 7, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & 0x1000) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 19, 0, (int *)&action);
                } else if (action.battlerType != 4) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 1, 0, (int *)&action);
                } else {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 2, 0, (int *)&action);
                }
                opponent->unk197 = 1;
            }
            BattleInput_SetPartyExpPercents(battleInput, work->expPercents);
            BattleInput_UpdateBallGaugeAnimation(battleInput, work->ballStatus[0], work->ballStatus[1]);
            BattleInput_EnableBallGauge(battleInput);
            NARC_Delete(narc0);
            NARC_Delete(narc1);
            if (hpBar != NULL) {
                ov12_02265D70(hpBar);
            }
            work->state = 5;
        }
        break;
    case 5:
        if (gSystem.newKeys & PAD_BUTTON_START) {
            int i;
            for (i = 0; i < BattleSystem_GetMaxBattlers(work->battleSystem); i++) {
                OpponentData *other = BattleSystem_GetOpponentData(work->battleSystem, i);
                ov12_02264C84(&other->hpBar);
            }
        }
        work->input = BattleInput_CheckTouch(battleInput);
        if (work->input != 0xFFFFFFFF) {
            work->unusedB = 10;
            PlaySE(0x5DD);
            work->state = 6;
        }
        break;
    case 6:
        if (BattleInput_CheckFeedbackDone(battleInput) == 1 || work->input == 1) {
            switch (work->input) {
            case 1:
                if (BattleSystem_GetBattleType(work->battleSystem) & 0x220) {
                    work->state = 7;
                }
                break;
            case 2: {
                NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
                BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
                BattleInput_Deadstriped_022698AC(battleInput, 0);
                ov12_02265D74(hpBar);
                ov12_02264EB4(work->hpBar);
                ov12_02262014(opponent);
                NARC_Delete(narc0);
                NARC_Delete(narc1);
                break;
            }
            case 3: {
                NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
                BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
                BattleInput_Deadstriped_022698AC(battleInput, 0);
                ov12_02265D74(hpBar);
                ov12_02264EB4(work->hpBar);
                ov12_02262014(opponent);
                NARC_Delete(narc0);
                NARC_Delete(narc1);
                break;
            }
            case 4: {
                NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
                if (BattleInput_GetCancelRunFlag(battleInput) == 1) {
                    ov12_02265D74(hpBar);
                    work->input = 0xFF;
                }
                BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
                NARC_Delete(narc0);
                NARC_Delete(narc1);
                break;
            }
            default:
                GF_AssertFail();
                break;
            }
            work->state = 8;
        }
        break;
    case 7:
        if (BattleInput_CheckFeedbackDone(battleInput) == 1) {
            NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
            BattleInput_Deadstriped_022698AC(battleInput, 0);
            ov12_02265D74(hpBar);
            ov12_02264EB4(work->hpBar);
            ov12_02262014(opponent);
            BattleInput_DisableBallGauge(battleInput);
            work->state = 8;
            NARC_Delete(narc0);
            NARC_Delete(narc1);
        }
        break;
    case 8:
        if (ov12_022698B0(battleInput) == 1) {
            BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, work->input);
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
    (void)bgConfig;
}
