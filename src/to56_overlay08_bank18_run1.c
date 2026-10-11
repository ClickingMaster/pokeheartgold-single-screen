#include "../include/to56_overlay08_bank18_private.h"
#pragma force_active on

void BattlePartyMenu_ShowMonMenuSprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x80, 0x48);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x88, 0x50);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x90, 0x50);
}

void BattlePartyMenu_ShowSummarySprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    BattlePartyMenu_ShowStatusIcon(rec->flags17, work->sprites[13 + work->data->battler], 0xC6, 0x14);
    BattlePartyMenu_ShowMonTypeIcons(work, rec, sBattlePartySummaryTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[6], 0x14, 0x84);
}

void BattlePartyMenu_ShowCheckMovesSprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    u32 i;
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    BattlePartyMenu_ShowStatusIcon(rec->flags17, work->sprites[13 + work->data->battler], 0xC6, 0x14);
    BattlePartyMenu_ShowMonTypeIcons(work, rec, sBattlePartyCheckMovesTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    for (i = 0; i < 4; i++) {
        if (rec->moves[i].move != 0) {
            BattlePartyMenu_SetTypeIcon(work, work->sprites[21 + i], 0xB010 + i, rec->moves[i].value4);
            BattlePartyMenu_ShowSpriteAt(work->sprites[21 + i], sBattlePartyCheckMovesMoveIconPos[i].x, sBattlePartyCheckMovesMoveIconPos[i].y);
        }
    }
}

void BattlePartyMenu_ShowMoveDetailSprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    BattlePartyMenu_ShowStatusIcon(rec->flags17, work->sprites[13 + work->data->battler], 0xC6, 0x14);
    BattlePartyMenu_ShowMonTypeIcons(work, rec, sBattlePartyMoveDetailTypeIconPos);
    BattlePartyMenu_ShowSpriteAt(work->sprites[21 + work->data->slot], 0x88, 0x30);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    BattlePartyMenu_SetMoveCategoryIcon(work, work->sprites[26], rec->moves[work->data->slot].value5);
    BattlePartyMenu_ShowSpriteAt(work->sprites[26], 0x18, 0x58);
}

void BattlePartyMenu_ShowLearnMoveSprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    BattlePartyMenu_ShowMonTypeIcons(work, rec, sBattlePartyLearnMoveTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    BattlePartyMenu_ShowMoveIcons(work);
}

void BattlePartyMenu_ShowForgetMoveSprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    BattlePartyMenu_ShowMonTypeIcons(work, rec, sBattlePartyForgetMoveTypeIconPos);
    BattlePartyMenu_ShowSpriteAt(work->sprites[21 + work->data->slot], 0x88, 0x30);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    if (work->data->slot < 4) {
        BattlePartyMenu_SetMoveCategoryIcon(work, work->sprites[26], rec->moves[work->data->slot].value5);
    } else {
        BattlePartyMenu_SetMoveCategoryIcon(work, work->sprites[26], GetMoveAttr(work->data->move, MOVEATTR_CLASS));
    }
    BattlePartyMenu_ShowSpriteAt(work->sprites[26], 0x18, 0x58);
}

void BattlePartyMenu_ShowRestorePPSprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    BattlePartyMenu_ShowMonTypeIcons(work, rec, sBattlePartyLearnMoveTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    BattlePartyMenu_ShowMoveIcons(work);
}

void BattlePartyMenu_ShowContestForgetMoveSprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    BattlePartyMenu_ShowSpriteAt(work->sprites[21 + work->data->slot], 0x88, 0x48);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    BattlePartyMenu_ShowConditionMarkers(work);
}

void BattlePartyMenu_ShowContestLearnMoveSprites(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    BattlePartyMenu_ShowSpriteAt(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    BattlePartyMenu_ShowHeldItemIcon(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    BattlePartyMenu_ShowCapsuleIcon(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    BattlePartyMenu_ShowMoveIcons(work);
    BattlePartyMenu_ShowConditionMarkers(work);
}

void BattlePartyMenu_ShowMoveTypeIcons(Ov08Work *work) {
    u16 i;
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    for (i = 0; i < 4; i++) {
        if (rec->moves[i].move != 0) {
            BattlePartyMenu_SetTypeIcon(work, work->sprites[21 + i], 0xB010 + i, rec->moves[i].value4);
            BattlePartyMenu_ShowSpriteAt(work->sprites[21 + i], sBattlePartyLearnMoveMoveIconPos[i].x, sBattlePartyLearnMoveMoveIconPos[i].y);
        }
    }
    if (work->data->move != 0) {
        BattlePartyMenu_SetTypeIcon(work, work->sprites[25], 0xB014, (u8)GetMoveAttr(work->data->move, MOVEATTR_TYPE));
        BattlePartyMenu_ShowSpriteAt(work->sprites[25], 0x58, 0xB0);
    }
}

void BattlePartyMenu_ShowMoveContestTypeIcons(Ov08Work *work) {
    u16 i;
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    for (i = 0; i < 4; i++) {
        if (rec->moves[i].move != 0) {
            BattlePartyMenu_SetTypeIcon(work, work->sprites[21 + i], 0xB010 + i, (u8)(GetMoveAttr(rec->moves[i].move, MOVEATTR_CONTEST_TYPE) + 0x12));
            BattlePartyMenu_ShowSpriteAt(work->sprites[21 + i], sBattlePartyLearnMoveMoveIconPos[i].x, sBattlePartyLearnMoveMoveIconPos[i].y);
        }
    }
    if (work->data->move != 0) {
        BattlePartyMenu_SetTypeIcon(work, work->sprites[25], 0xB014, (u8)(GetMoveAttr(work->data->move, MOVEATTR_CONTEST_TYPE) + 0x12));
        BattlePartyMenu_ShowSpriteAt(work->sprites[25], 0x58, 0xB0);
    }
}
