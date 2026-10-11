#include "to57_new49_overlay08_bank00_input_private.h"

int BattlePartyMenu_HandleLearnMoveInput(Ov08N49Work *work) {
    int result = BattlePartyMenu_CheckTouchHitbox(work, sBattlePartyLearnMoveHitboxes);
    if (result == -1) {
        result = BattleDpadCursor_HandleInput(work->smallState);
        if (result == -2) {
            result = 6;
        }
    } else {
        BattlePartyMenu_DeactivateCursor(work);
    }
    switch (result) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        work->root->selected = result;
        work->result = result;
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, (u8)(result + 0x17));
        work->state = 12;
        return 0x16;
    case 5:
        if (work->modeHigh == 0) {
            break;
        }
        work->modeLow ^= 1;
        work->result = result;
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 0x12);
        work->state = 11;
        return 0x16;
    case 6:
        work->root->selected = 4;
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 6);
        work->state = 0x19;
        return 0x16;
    }
    return 0x13;
}

int BattlePartyMenu_HandleForgetMoveInput(Ov08N49Work *work) {
    int result = BattlePartyMenu_CheckTouchHitbox(work, sBattlePartyForgetMoveHitboxes);
    if (result == -1) {
        result = BattleDpadCursor_HandleInput(work->smallState);
        if (result == -2) {
            result = 2;
        }
    } else {
        BattlePartyMenu_DeactivateCursor(work);
    }
    switch (result) {
    case 0:
        PlaySE(0x5DD);
        if (work->modeLow == 0) {
            BattlePartyMenu_StartButtonAnim(work, 0x1C);
        } else {
            BattlePartyMenu_StartButtonAnim(work, 0x1D);
        }
        if (BattlePartyMenu_IsSelectedMoveHM(work) == 1) {
            BattlePartyMenu_PrintHMCantBeForgotten(work);
            if (work->modeLow == 0) {
                BattlePartyMenu_ClearBattleMoveInfo(work);
            } else {
                BattlePartyMenu_ClearContestMoveInfo(work);
            }
            work->state = 0x14;
        } else {
            work->state = 0x19;
        }
        return 0x16;
    case 1:
        if (work->modeHigh == 0) {
            break;
        }
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 0x12);
        work->modeLow ^= 1;
        work->field208E = result;
        work->state = 0xC;
        return 0x16;
    case 2:
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 6);
        work->field208E = 0;
        work->state = 0xB;
        return 0x16;
    }
    return 0x14;
}

int BattlePartyMenu_HandleRestorePPInput(Ov08N49Work *work) {
    Ov08N49Root *root = work->root;
    int result = BattlePartyMenu_CheckTouchHitbox(work, sBattlePartyRestorePPHitboxes);
    if (result == -1) {
        result = BattleDpadCursor_HandleInput(work->smallState);
        if (result == -2) {
            result = 4;
        }
    } else {
        BattlePartyMenu_DeactivateCursor(work);
    }
    switch (result) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (*(u16 *)((u8 *)work + 0x34 + root->battler * 0x50 + result * 8) == 0) {
            break;
        }
        work->root->selected = result;
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, (u8)(result + 0x13));
        if (BattleSystem_RecoverStatus(root->unk08, (int)root->unk28, root->data2C[root->battler], result, root->item) == 1) {
            work->field207C = 0;
            work->state = 0x17;
            return 0x16;
        }
        ReadMsgDataIntoString(work->msgData, 0x51, work->string);
        BattlePartyMenu_ShowMessage(work);
        work->root->battler = 6;
        work->state = 0x19;
        return 0x11;
    case 4:
        PlaySE(0x5DD);
        BattlePartyMenu_StartButtonAnim(work, 6);
        work->state = 6;
        return 0x16;
    }
    return 0x15;
}
