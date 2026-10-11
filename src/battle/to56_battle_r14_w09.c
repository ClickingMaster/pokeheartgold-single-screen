#include "to56_battle_r14_w09_private.h"

void ov12_0225CE28(SysTask *task, TrainerEncounterData *work) {
    s16 x1, y1, x2, y2;
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);

    if (work->enterFrameCount < 33 && work->state >= 2 && (work->battlerType == 0 || work->battlerType == 2)) {
        BgSetPosTextAndCommit(BattleSystem_GetBgConfig(work->battleSystem), 3, BG_POS_OP_ADD_X, 4);
        work->enterFrameCount++;
    }

    switch (work->state) {
    case 0:
        if (ManagedSprite_GetNumFrames(work->managedSprite) > 2) {
            ManagedSprite_SetAnimationFrame(work->managedSprite, 0);
            ManagedSprite_SetAnim(work->managedSprite, 2);
        }
        work->delay = 28;
        work->state = 1;
    case 1:
        if (--work->delay) {
            break;
        }
        work->state = 2;
    case 2:
        ManagedSprite_GetPositionXY(work->terrain->managedSprite, &x1, &y1);
        ManagedSprite_GetPositionXY(work->managedSprite, &x2, &y2);
        if (work->face == 2) {
            if (work->battlerType == 1 || work->battlerType == 3) {
                if (x1 < 192) {
                    ManagedSprite_OffsetPositionXY(work->terrain->managedSprite, 8, 0);
                } else {
                    ManagedSprite_SetPositionXY(work->terrain->managedSprite, 192, 88);
                }
            }
            ManagedSprite_GetPositionXY(work->terrain->managedSprite, &x1, &y1);
            if (work->battlerType == 1 || (work->battlerType == 3 && (battleType == 3 || battleType == 0x83 || battleType == 7))) {
                ManagedSprite_SetPositionXY(work->managedSprite, x1, y2);
            } else if (work->battlerType == 3) {
                x1 = x2 - x1;
                x1 -= 24;
                ManagedSprite_OffsetPositionXY(work->managedSprite, -x1, 0);
            } else if (work->battlerType == 5) {
                x1 = x1 - x2;
                x1 -= 16;
                ManagedSprite_OffsetPositionXY(work->managedSprite, x1, 0);
            }
            ManagedSprite_GetPositionXY(work->managedSprite, &x2, &y2);
            if (x2 >= work->targetX) {
                ManagedSprite_SetPositionXY(work->managedSprite, work->targetX, y2);
                if (work->battlerType == 1 || work->battlerType == 3) {
                    ManagedSprite_SetPositionXY(work->terrain->managedSprite, 192, 88);
                }
                if (ManagedSprite_GetNumFrames(work->managedSprite) > 1) {
                    ManagedSprite_SetAnimationFrame(work->managedSprite, 0);
                    ManagedSprite_SetAnim(work->managedSprite, 1);
                    work->ballFlashStarted = 0;
                    work->state = 3;
                } else {
                    work->state = 4;
                }
            }
        } else {
            if (work->battlerType == 0 || work->battlerType == 2) {
                if (x1 > 64) {
                    ManagedSprite_OffsetPositionXY(work->terrain->managedSprite, -8, 0);
                } else {
                    ManagedSprite_SetPositionXY(work->terrain->managedSprite, 64, 136);
                }
            }
            ManagedSprite_GetPositionXY(work->terrain->managedSprite, &x1, &y1);
            if (work->battlerType == 0 || (work->battlerType == 2 && (battleType == 3 || battleType == 0x83 || battleType == 0x13 || battleType == 7))) {
                ManagedSprite_SetPositionXY(work->managedSprite, x1, y2);
            } else if (work->battlerType == 2) {
                x1 = x1 - x2;
                x1 -= 24;
                ManagedSprite_OffsetPositionXY(work->managedSprite, x1, 0);
            } else if (work->battlerType == 4) {
                x1 = x2 - x1;
                x1 -= 16;
                ManagedSprite_OffsetPositionXY(work->managedSprite, -x1, 0);
            }
            ManagedSprite_GetPositionXY(work->managedSprite, &x2, &y2);
            if (work->pokepic != NULL) {
                Pokepic_SetAttr(work->pokepic, 0, x2);
            }
            if (x2 <= work->targetX) {
                if (work->pokepic != NULL) {
                    Pokepic_Delete(work->pokepic);
                }
                ManagedSprite_SetPositionXY(work->managedSprite, work->targetX, y2);
                if (work->battlerType == 0 || work->battlerType == 2) {
                    ManagedSprite_SetPositionXY(work->terrain->managedSprite, 64, 136);
                }
                work->state = 4;
            }
        }
        break;
    case 3:
        if (ManagedSprite_IsAnimated(work->managedSprite) == 1) {
            if (ov12_02261DC8(work, work->managedSprite) == 0) {
                break;
            }
        }
        work->state = 4;
    case 4:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void ov12_0225D138(SysTask *task, TrainerThrowBallData *work) {
    s16 x, y;
    int animFrame;
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    UnkStruct_134 ball;

    switch (work->state) {
    case 0:
        if (work->ballTypeIn == 0) {
            work->state = 1;
            break;
        }
        switch (work->ballTypeIn) {
        default:
        case 1:
            ball.unk0 = 15;
            work->ballTargetState = 3;
            ball.ball = 5;
            break;
        case 2:
            ball.unk0 = 12;
            work->ballTargetState = 0;
            ball.ball = 0x401;
            break;
        case 3:
            ball.unk0 = 13;
            work->ballTargetState = 0;
            ball.ball = 0x402;
            break;
        case 4:
            ball.unk0 = 15;
            work->ballTargetState = 3;
            ball.ball = 0x400;
            break;
        }
        ball.heapID = HEAP_ID_BATTLE;
        ball.unk8 = 4;
        ball.unkC = work->battlerId;
        ball.spriteSystem = BattleSystem_GetSpriteSystem(work->battleSystem);
        ball.paletteData = BattleSystem_GetPaletteData(work->battleSystem);
        ball.unk18 = 0;
        ball.battleSystem = work->battleSystem;
        work->opponentData->ballData = UnkBallData_New(&ball);
        UnkBallData_SetDrawPriority(work->opponentData->ballData, 100);
        UnkBallData_SetPriority(work->opponentData->ballData, 2);
        UnkBallData_SetAnimateSprite(work->opponentData->ballData, 0);
        ManagedSprite_SetAnimationFrame(work->opponentData->managedSprite, 0);
        ManagedSprite_SetAnim(work->opponentData->managedSprite, 1);
        ManagedSprite_SetAnimateFlag(work->opponentData->managedSprite, TRUE);
        work->state = 3;
        break;
    case 1:
        if (work->face == 2) {
            ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, 5, 0);
            ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
            if (x >= 160 && UnkBallData_GetMode(work->opponentData->ballData) != 0) {
                UnkBallData_SetDrawFlag(work->opponentData->ballData, 1);
                UnkBallData_SetMode(work->opponentData->ballData, 0);
                if ((battleType & 2) && !(battleType & 8)) {
                    OpponentData *partner = BattleSystem_GetOpponentData(work->battleSystem, BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId));
                    UnkBallData_SetDrawFlag(partner->ballData, 1);
                    UnkBallData_SetMode(partner->ballData, 0);
                    UnkBallData_SetDelay(partner->ballData, 12);
                }
            } else if (x >= 296) {
                SpriteManager *mgr = BattleSystem_GetSpriteManager(work->battleSystem);
                Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
                work->opponentData->managedSprite = NULL;
                SpriteManager_UnloadCharObjById(mgr, 20015 + work->opponentData->battlerType);
                SpriteManager_UnloadPlttObjById(mgr, 20010 + work->opponentData->battlerType);
                SpriteManager_UnloadCellObjById(mgr, 20007 + work->opponentData->battlerType);
                SpriteManager_UnloadAnimObjById(mgr, 20007 + work->opponentData->battlerType);
                work->state = 6;
            }
        } else {
            ManagedSprite_SetAnimationFrame(work->opponentData->managedSprite, 0);
            ManagedSprite_SetAnim(work->opponentData->managedSprite, 1);
            ManagedSprite_SetAnimateFlag(work->opponentData->managedSprite, TRUE);
            work->state = 2;
        }
        break;
    case 2:
        ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, -5, 0);
        ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
        if (work->opponentData->ballData) {
            animFrame = ManagedSprite_GetAnimationFrame(work->opponentData->managedSprite);
            if (ov12_0226D1E8[work->backSpriteIdx][animFrame][0] != 0x7FFF) {
                UnkBallData_SetDrawFlag(work->opponentData->ballData, 1);
                UnkBallData_SetPosition(work->opponentData->ballData, x + ov12_0226D1E8[work->backSpriteIdx][animFrame][0], y + ov12_0226D1EA[work->backSpriteIdx][animFrame][0]);
                if (animFrame == 3 && UnkBallData_GetMode(work->opponentData->ballData) != 0) {
                    UnkBallData_SetMode(work->opponentData->ballData, 0);
                    UnkBallData_SetAnimateSprite(work->opponentData->ballData, 1);
                    if ((battleType & 2) && !(battleType & 8)) {
                        OpponentData *partner = BattleSystem_GetOpponentData(work->battleSystem, BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId));
                        UnkBallData_SetPosition(partner->ballData, x + ov12_0226D1E8[work->backSpriteIdx][animFrame][0], y + ov12_0226D1EA[work->backSpriteIdx][animFrame][0]);
                        UnkBallData_SetDrawFlag(partner->ballData, 1);
                        UnkBallData_SetMode(partner->ballData, 0);
                        UnkBallData_SetAnimateSprite(partner->ballData, 1);
                    }
                }
            }
        }
        if (x <= -40) {
            SpriteManager *mgr = BattleSystem_GetSpriteManager(work->battleSystem);
            Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
            work->opponentData->managedSprite = NULL;
            SpriteManager_UnloadCharObjById(mgr, 20015 + work->opponentData->battlerType);
            SpriteManager_UnloadPlttObjById(mgr, 20010 + work->opponentData->battlerType);
            SpriteManager_UnloadCellObjById(mgr, 20007 + work->opponentData->battlerType);
            SpriteManager_UnloadAnimObjById(mgr, 20007 + work->opponentData->battlerType);
            work->state = 6;
        }
        break;
    case 3:
        ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
        animFrame = ManagedSprite_GetAnimationFrame(work->opponentData->managedSprite);
        if (animFrame == 4) {
            work->delay = 8;
            work->state = 4;
        } else if (work->opponentData->ballData && ov12_0226D1E8[work->backSpriteIdx][animFrame][0] != 0x7FFF) {
            UnkBallData_SetPosition(work->opponentData->ballData, x + ov12_0226D1E8[work->backSpriteIdx][animFrame][0], y + ov12_0226D1EA[work->backSpriteIdx][animFrame][0]);
            if (animFrame == 3 && UnkBallData_GetMode(work->opponentData->ballData) != work->ballTargetState) {
                UnkBallData_SetMode(work->opponentData->ballData, work->ballTargetState);
                UnkBallData_SetPriority(work->opponentData->ballData, 1);
                UnkBallData_SetAnimateSprite(work->opponentData->ballData, 1);
                if (work->ballTargetState != 3) {
                    PlaySE(0x70A);
                }
            }
        }
        break;
    case 4:
        if (--work->delay == 0) {
            if (work->ballTargetState == 3) {
                work->state = 6;
            } else {
                work->state = 5;
            }
        }
        break;
    case 5:
        if (UnkBallData_IsActive(work->opponentData->ballData) != 1) {
            UnkBallData_Delete(work->opponentData->ballData);
            work->opponentData->ballData = NULL;
            work->state = 6;
        }
        break;
    case 6:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void ov12_0225D644(SysTask *task, TrainerThrowBallData *work) {
    s16 x, y;
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    switch (work->state) {
    case 0:
        if (work->face == 2) {
            ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, 5, 0);
            ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
            if (x >= 296) {
                SpriteManager *mgr = BattleSystem_GetSpriteManager(work->battleSystem);
                Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
                work->opponentData->managedSprite = NULL;
                SpriteManager_UnloadCharObjById(mgr, 20015 + work->opponentData->battlerType);
                SpriteManager_UnloadPlttObjById(mgr, 20010 + work->opponentData->battlerType);
                SpriteManager_UnloadCellObjById(mgr, 20007 + work->opponentData->battlerType);
                SpriteManager_UnloadAnimObjById(mgr, 20007 + work->opponentData->battlerType);
                work->state = 2;
            }
        } else {
            ManagedSprite_SetPriority(work->opponentData->managedSprite, 1);
            ManagedSprite_SetAnimationFrame(work->opponentData->managedSprite, 0);
            ManagedSprite_SetAnim(work->opponentData->managedSprite, 1);
            ManagedSprite_SetAnimateFlag(work->opponentData->managedSprite, TRUE);
            work->state = 1;
        }
        break;
    case 1:
        ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, -5, 0);
        ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
        if (work->opponentData->ballData && (battleType & 2) && !(battleType & 8)) {
            int frame = ManagedSprite_GetAnimationFrame(work->opponentData->managedSprite);
            if (ov12_0226D1E8[work->backSpriteIdx][frame][0] != 0x7FFF) {
                OpponentData *partner = BattleSystem_GetOpponentData(work->battleSystem, BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId));
                if (frame == 3 && UnkBallData_GetMode(partner->ballData) != 0) {
                    UnkBallData_SetPosition(partner->ballData, x + ov12_0226D1E8[work->backSpriteIdx][frame][0], y + ov12_0226D1EA[work->backSpriteIdx][frame][0]);
                    UnkBallData_SetDrawFlag(partner->ballData, 1);
                    UnkBallData_SetMode(partner->ballData, 0);
                    UnkBallData_SetAnimateSprite(partner->ballData, 1);
                    UnkBallData_SetPriority(partner->ballData, 1);
                }
            }
        }
        if (x <= -40) {
            SpriteManager *mgr = BattleSystem_GetSpriteManager(work->battleSystem);
            Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
            work->opponentData->managedSprite = NULL;
            SpriteManager_UnloadCharObjById(mgr, 20015 + work->opponentData->battlerType);
            SpriteManager_UnloadPlttObjById(mgr, 20010 + work->opponentData->battlerType);
            SpriteManager_UnloadCellObjById(mgr, 20007 + work->opponentData->battlerType);
            SpriteManager_UnloadAnimObjById(mgr, 20007 + work->opponentData->battlerType);
            work->state = 2;
        }
        break;
    case 2:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}
