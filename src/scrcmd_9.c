#include "scrcmd_9.h"

#include "global.h"

#include "assert.h"
#include "error_handling.h"
#include "field_system.h"
#include "filesystem.h"
#include "frontier_data.h"
#include "game_stats.h"
#include "heap.h"
#include "launch_application.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "party.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "scrcmd.h"
#include "script.h"
#include "script_manager.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "frontier_data_access.h"
#include "frontier_save_access.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "frontier_field_system.h"
#include "unk_0204B538.h"
#include "frontier_field_support.h"
#include "battle_frontier_communication.h"

static BOOL ScrNative_WaitFrontierPartnerCheck(ScriptContext *ctx);
static u32 FrontierFieldSystem_0204A2A8(FrontierFieldSystem *frontierFsys, u8 a1);

static const u16 StatTrainerOverworlds[] = { SPRITE_SEVEN1, SPRITE_SEVEN5, SPRITE_SEVEN2, SPRITE_SEVEN3, SPRITE_SEVEN4 };

BOOL ScrCmd_FrontierFieldSystemInit(ScriptContext *ctx) {
    u16 resumeFromPrevious = ScriptReadHalfword(ctx);
    u16 towerMode = ScriptReadHalfword(ctx);
    ctx->fieldSystem->frontierFsys = FrontierFieldSystem_New(FieldSystem_GetSaveData(ctx->fieldSystem), resumeFromPrevious, towerMode);
    return FALSE;
}

BOOL ScrCmd_FrontierAssertCleared(ScriptContext *ctx) {
    FrontierFieldSystem_AssertCleared(&ctx->fieldSystem->frontierFsys);
    return FALSE;
}

BOOL ScrCmd_FrontierFieldSystemFree(ScriptContext *ctx) {
    FrontierFieldSystem_Free(ctx->fieldSystem->frontierFsys);
    ctx->fieldSystem->frontierFsys = NULL;
    return FALSE;
}

BOOL ScrCmd_BattleTowerAction(ScriptContext *ctx) {
    void **unk;
    u16 id = ScriptReadHalfword(ctx);
    u16 arg = ScriptGetVar(ctx);
    u16 resultVarId = ScriptReadHalfword(ctx);
    u16 *result = GetVarPointer(ctx->fieldSystem, resultVarId);
    FrontierFieldSystem *frontierFsys = ctx->fieldSystem->frontierFsys;
    switch (id) {
    case 1:
        if (arg == 0) {
            *result = PartyIsValidForFrontier(frontierFsys->numMons, ctx->fieldSystem->saveData, TRUE);
        } else {
            *result = PartyIsValidForFrontier(arg, ctx->fieldSystem->saveData, TRUE);
        }
        break;
    case 2:
        ResetSystem();
        break;
    case 3:
        FrontierFieldSystem_ClearSavedChallenge(Save_BattleTowerChallenge_Get(ctx->fieldSystem->saveData));
        break;
    case 4:
        *result = FrontierFieldSystem_GetSavedChallenge(Save_BattleTowerChallenge_Get(ctx->fieldSystem->saveData));
        break;
    case 5:
        FrontierFieldSystem_SetReturnWarp(ctx->fieldSystem);
        break;
    case 6:
        FrontierFieldSystem_ClearReturnWarpFlag(ctx->fieldSystem);
        break;
    case 8: // unused
        *result = FrontierFieldSystem_GetWinStreak(ctx->fieldSystem->saveData, arg);
        break;
    case 9:
        *result = FrontierFieldSystem_WifiRankAction(NULL, ctx->fieldSystem->saveData, 2);
        break;
    case 10:
        *result = FrontierFieldSystem_WifiRankAction(NULL, ctx->fieldSystem->saveData, 0);
        break;
    case 11:
        sub_0204A730(ctx->fieldSystem->saveData, arg);
        break;
    case 12:
        *result = sub_0204A750(ctx->fieldSystem->saveData);
        break;
    case 14:
        *result = FrontierFieldSystem_ForfeitSavedChallenge(ctx->fieldSystem->saveData);
        break;
    case 15:
        *result = Save_BattleRoomDownload_HasTrainers(ctx->fieldSystem->saveData);
        break;
    case 16:
        TaskManager_CallFrontierWifiConnection(ctx->taskman, arg, resultVarId, *result);
        return TRUE;
    case 30:
        unk = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_RUNNING_APP_DATA);
        FrontierFieldSystem_StartPartySelect(frontierFsys, ctx->taskman, unk);
        return TRUE;
    case 31:
        unk = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_RUNNING_APP_DATA);
        *result = FrontierFieldSystem_ReadPartySelection(frontierFsys, unk, ctx->fieldSystem->saveData);
        break;
    case 32:
        *result = FrontierFieldSystem_PartyHasDuplicateSpeciesOrItems(frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 35:
        *result = FrontierFieldSystem_HasWonSet(frontierFsys);
        break;
    case 37: // unused
        FrontierFieldSystem_RecordSetLost(frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 38: // unused
        FrontierFieldSystem_RecordSetWon(frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 39:
        FrontierFieldSystem_SaveChallenge(frontierFsys);
        break;
    case 56:
        FrontierFieldSystem_GenerateStatTrainers(frontierFsys);
        break;
    case 41: // unused
        *result = FrontierFieldSystem_GetTrainerOverworld(frontierFsys, arg);
        break;
    case 43:
        *result = FrontierFieldSystem_GetBattleTowerMode(frontierFsys);
        break;
    case 45:
        *result = FrontierFieldSystem_GetPalmerDefeated(frontierFsys);
        break;
    case 47: // unused
        FrontierFieldSystem_0204B0E0(frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 48:
        *result = FrontierFieldSystem_TryGivePalmerRibbons(frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 49:
        *result = FrontierFieldSystem_TryGiveOtherTowerRibbons(frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 50:
        frontierFsys->multiBattleAllyID = arg;
        break;
    case 51:
        *result = frontierFsys->multiBattleAllyID;
        break;
    case 52:
        FrontierFieldSystem_SetRandomFrontierTrainers(frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 53:
        *result = frontierFsys->partyMonIndexes[arg];
        break;
    case 54:
        *result = FrontierFieldSystem_WifiRankAction(frontierFsys, ctx->fieldSystem->saveData, 1);
        break;
    case 55:
        *result = FrontierFieldSystem_0204A2A8(frontierFsys, arg);
        break;
    case 57:
        *result = FrontierFieldSystem_InitChallengeRandom(frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 100:
        if (frontierFsys == NULL) {
            *result = TRUE;
        } else {
            *result = FALSE;
        }
        break;
    case 58:
        MI_CpuClear8(frontierFsys->unk884, sizeof(frontierFsys->unk884));
        break;
    default:
        GF_ASSERT(FALSE);
        *result = 0;
        break;
    }
    return FALSE;
}

BOOL ScrCmd_BattleTowerGetAllyMonSpeciesAndMove(ScriptContext *ctx) {
    FrontierFieldSystem *frontierFsys = ctx->fieldSystem->frontierFsys;
    u16 multiBattleAllyIndex = ScriptGetVar(ctx);
    u16 allyMonIndex = ScriptGetVar(ctx);
    u16 *speciesPtr = ScriptGetVarPointer(ctx);
    u16 *movePtr = ScriptGetVarPointer(ctx);
    *speciesPtr = frontierFsys->frontierStatTrainers[multiBattleAllyIndex].frontierMons[allyMonIndex].species;
    *movePtr = frontierFsys->frontierStatTrainers[multiBattleAllyIndex].frontierMons[allyMonIndex].moves[0];
    return FALSE;
}

BOOL ScrCmd_BattleTowerClaimRecordMilestone(ScriptContext *ctx) {
    u16 *unk = ScriptGetVarPointer(ctx);
    *unk = BattleTower_ClaimRecordMilestone(ctx->fieldSystem->saveData);
    return FALSE;
}

BOOL ScrCmd_BattleTowerGetRecordMilestone(ScriptContext *ctx) {
    u16 *unk = ScriptGetVarPointer(ctx);
    *unk = BattleTower_GetRecordMilestone(ctx->fieldSystem->saveData);
    return FALSE;
}

BOOL ScrCmd_BattleTowerSendLinkData(ScriptContext *ctx) {
    u16 unk7 = ScriptGetVar(ctx);
    u16 unk4 = ScriptGetVar(ctx);
    u16 *unkPtr = ScriptGetVarPointer(ctx);
    FrontierFieldSystem *frontierFsys = ctx->fieldSystem->frontierFsys;
    *unkPtr = FALSE;
    u32 unk0;
    switch (unk7) {
    case 0:
        unk0 = 62;
        FrontierFieldSystem_PackLinkAllyData(ctx->fieldSystem->frontierFsys, ctx->fieldSystem->saveData);
        break;
    case 1:
        unk0 = 63;
        FrontierFieldSystem_PackRoundTrainers(ctx->fieldSystem->frontierFsys);
        break;
    case 2:
        unk0 = 64;
        FrontierFieldSystem_0204B720(ctx->fieldSystem->frontierFsys, unk4);
        break;
    }
    if (sub_0205C298(ctx->fieldSystem->saveData) == 1) {
        if (LinkSync_SendPacket(Comm_GetLocalNetId(), frontierFsys->linkSendBuffer) == 1) {
            *unkPtr = TRUE;
        } else {
            return TRUE;
        }
    } else {
        FrontierLink_RegisterCommands((void *)frontierFsys);
        if (Comm_SendCommand(unk0, frontierFsys->linkSendBuffer, sizeof(frontierFsys->linkSendBuffer)) == 1) {
            *unkPtr = TRUE;
        }
    }
    return FALSE;
}

BOOL ScrCmd_BattleTowerCheckLinkPartner(ScriptContext *ctx) {
    FrontierFieldSystem *frontierFsys = ctx->fieldSystem->frontierFsys;
    u16 unk6 = ScriptGetVar(ctx);
    u16 unk7 = ScriptReadHalfword(ctx);
    if (sub_0205C298(ctx->fieldSystem->saveData) == 1) {
        TaskManager_CallFrontierLinkPartnerCheck(ctx->fieldSystem->taskman, unk6, unk7);
    } else {
        frontierFsys->resultVar = unk7;
        frontierFsys->unk8D5 = unk6;
        SetupNativeScript(ctx, ScrNative_WaitFrontierPartnerCheck);
    }
    return TRUE;
}

static BOOL ScrNative_WaitFrontierPartnerCheck(ScriptContext *ctx) {
    FrontierFieldSystem *frontierFsys = ctx->fieldSystem->frontierFsys;
    u16 *unkPtr = GetVarPointer(ctx->fieldSystem, frontierFsys->resultVar);
    u32 unk = frontierFsys->unk8D5 == 1 ? 1 : 2;
    if (frontierFsys->linkReceiveCount == unk) {
        frontierFsys->linkReceiveCount = 0;
        *unkPtr = frontierFsys->unk8D8;
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL ScrCmd_GetBattleRoomLeadersRoom(ScriptContext *ctx) {
    u16 *unk4 = ScriptGetVarPointer(ctx);
    u16 *unk6 = ScriptGetVarPointer(ctx);
    u8 unk[2];
    BattleRoomDownload_GetLeadersRoom(Save_BattleRoomDownload_Get(ctx->fieldSystem->saveData), unk);
    *unk4 = unk[0];
    *unk6 = unk[1];
    return FALSE;
}

BOOL ScrCmd_BattleRoomHasLeaders(ScriptContext *ctx) {
    u16 *unk = ScriptGetVarPointer(ctx);
    *unk = BattleRoomDownload_HasLeaders(Save_BattleRoomDownload_Get(ctx->fieldSystem->saveData));
    return FALSE;
}

static u32 FrontierFieldSystem_0204A2A8(FrontierFieldSystem *frontierFsys, u8 a1) {
    if (a1 == 2) {
        return frontierFsys->multiBattleAllyID;
    } else if (a1 == 1) {
        if (frontierFsys->towerMode == TOWER_MODE_MULTI) {
            return StatTrainerOverworlds[frontierFsys->multiBattleAllyID];
        }
        return frontierFsys->linkAllyGender ? SPRITE_HEROINE : SPRITE_HERO;
    } else {
        return frontierFsys->trainerGender ? SPRITE_HEROINE : SPRITE_HERO;
    }
}

BOOL ScrCmd_GetBattlePoints(ScriptContext *ctx) {
    SaveData *saveData = ctx->fieldSystem->saveData;
    u16 *unk = ScriptGetVarPointer(ctx);
    *unk = FrontierData_BattlePointAction(Save_FrontierData_Get(saveData), 0, 0);
    return FALSE;
}

BOOL ScrCmd_GiveBattlePoints(ScriptContext *ctx) {
    SaveData *saveData = ctx->fieldSystem->saveData;
    u16 battlePoints = ScriptGetVar(ctx);
    GameStats_Add(Save_GameStats_Get(ctx->fieldSystem->saveData), GAME_STAT_BATTLE_POINTS_RECEIVED, battlePoints);
    FrontierData_BattlePointAction(Save_FrontierData_Get(saveData), battlePoints, 5);
    return FALSE;
}

BOOL ScrCmd_TakeBattlePoints(ScriptContext *ctx) {
    SaveData *saveData = ctx->fieldSystem->saveData;
    u16 battlePoints = ScriptGetVar(ctx);
    GameStats_Add(Save_GameStats_Get(ctx->fieldSystem->saveData), GAME_STAT_BATTLE_POINTS_SPENT, battlePoints);
    FrontierData_BattlePointAction(Save_FrontierData_Get(saveData), battlePoints, 6);
    return FALSE;
}

BOOL ScrCmd_CheckBattlePoints(ScriptContext *ctx) {
    SaveData *saveData = ctx->fieldSystem->saveData;
    u16 unk = ScriptGetVar(ctx);
    u16 *unkPtr = ScriptGetVarPointer(ctx);
    if (FrontierData_BattlePointAction(Save_FrontierData_Get(saveData), 0, 0) < unk) {
        *unkPtr = FALSE;
    } else {
        *unkPtr = TRUE;
    }
    return FALSE;
}
