#include "frontier/frontier_cmd_castle.h"

#include "global.h"

#include "constants/pokemon.h"

#include "battle/battle_setup.h"
#include "frontier/battle_castle.h"
#include "frontier/battle_castle_data.h"
#include "frontier/battle_castle_setup.h"
#include "frontier/frontier.h"
#include "frontier/frontier_map.h"
#include "frontier/frontier_script_commands.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_trainers.h"

#include "battle_frontier_communication.h"
#include "frontier_save_access.h"
#include "frontier_script_commands_misc_internal.h"
#include "frontier_stat_ids_legacy.h"
#include "heap.h"
#include "launch_application.h"
#include "party.h"
#include "pokemon.h"

FS_EXTERN_OVERLAY(OVY_83);

static void BattleCastle_FillAppArgs(BattleCastleAppArgs *args, BattleCastleData *data);
static void BattleCastle_OnAppExit(void *args);
static BOOL FrontierScriptNative_WaitCastleLink(FrontierScriptContext *ctx);
static BOOL FrontierScriptNative_WaitCastleLinkChoice(FrontierScriptContext *ctx);

static const OverlayManagerTemplate sOverlayTemplate_BattleCastlePartyApp = {
    .init = BattleCastlePartyApp_Init,
    .exec = BattleCastlePartyApp_Main,
    .exit = BattleCastlePartyApp_Exit,
    .ovy_id = FS_OVERLAY_ID(OVY_83),
};

static const OverlayManagerTemplate sBattleCastleEmptyAppTemplate = {
    .init = NULL,
    .exec = NULL,
    .exit = NULL,
    .ovy_id = FS_OVERLAY_ID(OVY_83),
};

static const OverlayManagerTemplate sOverlayTemplate_BattleCastleOpponentApp = {
    .init = BattleCastleOpponentApp_Init,
    .exec = BattleCastleOpponentApp_Main,
    .exit = BattleCastleOpponentApp_Exit,
    .ovy_id = FS_OVERLAY_ID(OVY_83),
};

static inline BOOL BattleCastle_LaunchApp(FrontierScriptContext *ctx, const OverlayManagerTemplate *template) {
    FrontierLaunchArgs *launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleCastleData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleCastleAppArgs *args = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleCastleAppArgs));
    MI_CpuFill8(args, 0, sizeof(BattleCastleAppArgs));
    args->saveData = launchArgs->saveData;
    BattleCastle_FillAppArgs(args, data);
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, template, args, 0, BattleCastle_OnAppExit);
    return TRUE;
}

BOOL FrtCmd_CastleAlloc(FrontierScriptContext *ctx) {
    u32 resume = FrontierScript_ReadVar(ctx);
    u32 type = FrontierScript_ReadVar(ctx);
    u32 slot0 = FrontierScript_ReadVar(ctx);
    u16 slot1 = FrontierScript_ReadVar(ctx);
    u16 slot2 = FrontierScript_ReadVar(ctx);
    u16 *resultVar = FrontierScript_ReadVarPtr(ctx);
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleCastleData *data = BattleCastleData_Alloc(args->saveData, resume, (u8)type, slot0, slot1, slot2, resultVar);
    Frontier_SetData(ctx->frontierSystem->frontierWork, (u32)data);
    return FALSE;
}

BOOL FrtCmd_CastleInit(FrontierScriptContext *ctx) {
    u32 resume = FrontierScript_ReadVar(ctx);
    BattleCastleData_Init(Frontier_GetData(ctx->frontierSystem->frontierWork), resume);
    return FALSE;
}

BOOL FrtCmd_CastleFree(FrontierScriptContext *ctx) {
    BattleCastleData_Free(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_CastleLaunchPartyApp(FrontierScriptContext *ctx) {
    return BattleCastle_LaunchApp(ctx, &sOverlayTemplate_BattleCastlePartyApp);
}

BOOL FrtCmd_CastleLaunchEmptyApp(FrontierScriptContext *ctx) {
    return BattleCastle_LaunchApp(ctx, &sBattleCastleEmptyAppTemplate);
}

BOOL FrtCmd_CastleEndBattle(FrontierScriptContext *ctx) {
    BattleCastleData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = data->battleSetup;
    Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(data->playerParty, 0, Party_GetMonByIndex(setup->party[0], 0));
    Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(data->playerParty, 1, Party_GetMonByIndex(setup->party[0], 1));
    if (!BattleCastle_IsMultiType(data->type)) {
        Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(data->playerParty, 2, Party_GetMonByIndex(setup->party[0], 2));
    } else {
        Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(data->playerParty, 2, Party_GetMonByIndex(setup->party[2], 0));
        Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(data->playerParty, 3, Party_GetMonByIndex(setup->party[2], 1));
    }
    data->battleWon = IsBattleResultWin(setup->winFlag);
    BattleSetup_Delete(setup);
    return FALSE;
}

BOOL FrtCmd_CastleStartBattle(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleCastleData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = BattleCastle_NewBattleSetup(data, args);
    data->battleSetup = setup;
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_Battle, setup, 0, NULL);
    return TRUE;
}

BOOL FrtCmd_CastleLaunchOpponentApp(FrontierScriptContext *ctx) {
    return BattleCastle_LaunchApp(ctx, &sOverlayTemplate_BattleCastleOpponentApp);
}

static void BattleCastle_FillAppArgs(BattleCastleAppArgs *args, BattleCastleData *data) {
    int i;

    args->type = data->type;
    args->playerParty = data->playerParty;
    args->opponentParty = data->opponentParty;
    args->data = data;
    args->partnerPoints = data->partnerPoints;
    for (i = 0; i < 4; i++) {
        args->checked[i] = data->opponentChecked[i];
        args->levelChanges[i] = data->opponentLevelChanges[i];
        args->statsBought[i] = data->opponentStatsBought[i];
        args->movesBought[i] = data->opponentMovesBought[i];
    }
}

static void BattleCastle_OnAppExit(void *args) {
    BattleCastleAppArgs *appArgs = args;
    BattleCastleData_ReadAppResult(appArgs->data, appArgs);
    Heap_Free(appArgs);
}

BOOL FrtCmd_CastleSetPartyBeforeBattle(FrontierScriptContext *ctx) {
    BattleCastleData_SetPartyBeforeBattle(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_CastleGenerateOpponents(FrontierScriptContext *ctx) {
    BattleCastleData_GenerateOpponents(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

enum {
    CASTLE_ACTION_SET_TYPE = 2,
    CASTLE_ACTION_GET_SELECTION,
    CASTLE_ACTION_GET_WIN_STREAK,
    CASTLE_ACTION_INC_WIN_STREAK,
    CASTLE_ACTION_RESET_SYSTEM = 7,
    CASTLE_ACTION_GET_SAVED = 9,
    CASTLE_ACTION_SAVE,
    CASTLE_ACTION_NEXT_BATTLE = 14,
    CASTLE_ACTION_GET_OPPONENT_SPECIES,
    CASTLE_ACTION_GET_OPPONENT_MOVE,
    CASTLE_ACTION_GET_TYPE,
    CASTLE_ACTION_RESTORE_HELD_ITEMS,
    CASTLE_ACTION_GET_ROUND,
    CASTLE_ACTION_GET_TRAINER_SPRITE,
    CASTLE_ACTION_RESET_RANKS,
    CASTLE_ACTION_END_ROUND,
    CASTLE_ACTION_GET_BATTLE_NO,
    CASTLE_ACTION_GET_PARTNER_VALUE_A,
    CASTLE_ACTION_GET_MON_FAINTED,
    CASTLE_ACTION_GET_PARTNER_VALUE_B,
    CASTLE_ACTION_AWARD_POINTS,
    CASTLE_ACTION_RECORD_LINK_PARTY_PP,
    CASTLE_ACTION_REGISTER_LINK_COMMANDS,
    CASTLE_ACTION_GET_DECIDED_CHOICE,
    CASTLE_ACTION_CLEAR_CHOICES,
    CASTLE_ACTION_SET_CHOICE,
    CASTLE_ACTION_SPEND_CHOICE_POINTS,
    CASTLE_ACTION_IS_PARTNER_CHOICE,
    CASTLE_ACTION_IS_MULTI,
    CASTLE_ACTION_DRAW_ROUND_TILES,
    CASTLE_ACTION_LOAD_TRAINERS,
    CASTLE_ACTION_GET_ROUND_TABLE_VALUE,
    CASTLE_ACTION_GET_BRAIN_BATTLE = 40,
    CASTLE_ACTION_GENERATE_OPPONENTS,
    CASTLE_ACTION_SET_OPPONENT_PARTY,
};

BOOL FrtCmd_CastleAction(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *launchArgs;
    int arg;
    u32 action = FrontierScriptContext_ReadByte(ctx);
    arg = FrontierScriptContext_ReadByte(ctx);
    int moveIndex = FrontierScriptContext_ReadByte(ctx);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    BattleCastleData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u8 byteValue;
    u32 stat;
    switch (action) {
    case CASTLE_ACTION_SET_TYPE:
        data->type = arg;
        break;
    case CASTLE_ACTION_GET_SELECTION:
        *result = data->selections[arg];
        break;
    case CASTLE_ACTION_GET_WIN_STREAK:
        *result = data->winStreak;
        break;
    case CASTLE_ACTION_INC_WIN_STREAK:
        if (data->winStreak < 9999) {
            data->winStreak++;
        }
        break;
    case CASTLE_ACTION_RESET_SYSTEM:
        OS_ResetSystem(0);
        break;
    case CASTLE_ACTION_GET_SAVED:
        *result = CastleChallenge_GetSaved(data->challenge);
        break;
    case CASTLE_ACTION_SAVE:
        BattleCastleData_Save(data, 2);
        break;
    case CASTLE_ACTION_NEXT_BATTLE:
        *result = BattleCastleData_NextBattle(data);
        break;
    case CASTLE_ACTION_GET_OPPONENT_SPECIES:
        *result = data->opponentMons[arg].species;
        break;
    case CASTLE_ACTION_GET_OPPONENT_MOVE: {
        FrontierMon *opponentMon = &data->opponentMons[arg];
        *result = opponentMon->moves[moveIndex];
        break;
    }
    case CASTLE_ACTION_RESTORE_HELD_ITEMS: {
        Party *party = SaveArray_Party_Get(launchArgs->saveData);
        int i;
        u16 *heldItem = data->heldItems;
        for (i = 0; i < 3; i++) {
            SetMonData(Party_GetMonByIndex(party, data->partySlots[i]), MON_DATA_HELD_ITEM, heldItem++);
        }
        break;
    }
    case CASTLE_ACTION_GET_ROUND:
        *result = BattleCastle_GetRound(data);
        break;
    case CASTLE_ACTION_GET_TRAINER_SPRITE:
        *result = BattleCastleData_GetTrainerSprite(data, arg);
        break;
    case CASTLE_ACTION_RESET_RANKS:
        BattleCastleData_ResetRanks(data);
        break;
    case CASTLE_ACTION_END_ROUND:
        BattleCastleData_EndRound(data);
        break;
    case CASTLE_ACTION_GET_BATTLE_NO:
        *result = BattleCastleData_GetBattleNo(data);
        break;
    case CASTLE_ACTION_GET_PARTNER_VALUE_A:
        *result = data->partnerValueA;
        break;
    case CASTLE_ACTION_GET_PARTNER_VALUE_B:
        *result = data->partnerValueB;
        break;
    case CASTLE_ACTION_AWARD_POINTS:
        *result = BattleCastleData_CalcPointsEarned(data);
        BattleCastle_AddPoints(data->saveData, data->type, *result);
        break;
    case CASTLE_ACTION_RECORD_LINK_PARTY_PP:
        BattleCastleData_RecordLinkPartyPP(data);
        break;
    case CASTLE_ACTION_REGISTER_LINK_COMMANDS:
        FrontierLink_RegisterCommands(data);
        break;
    case CASTLE_ACTION_GET_DECIDED_CHOICE:
        if (data->decidedChoice >= 6) {
            *result = data->decidedChoice - 6;
        } else {
            *result = data->decidedChoice;
        }
        break;
    case CASTLE_ACTION_CLEAR_CHOICES:
        data->decidedChoice = 0;
        data->partnerChoice = 0;
        data->choice = 0;
        break;
    case CASTLE_ACTION_SET_CHOICE:
        data->choice = arg;
        break;
    case CASTLE_ACTION_SPEND_CHOICE_POINTS: {
        int spend = 0;
        if (data->decidedChoice >= 6) {
            if (Comm_GetLocalNetId() != 0) {
                spend = 1;
            }
        } else if (Comm_GetLocalNetId() == 0) {
            spend = 1;
        }
        if (spend == 1) {
            BattleCastle_SpendPoints(Save_Frontier_GetStatic(launchArgs->saveData), data->type, 50);
        } else {
            data->partnerPoints -= 50;
        }
        break;
    }
    case CASTLE_ACTION_IS_PARTNER_CHOICE:
        *result = 0;
        if (BattleCastle_IsMultiType(data->type) == TRUE) {
            if (data->decidedChoice >= 6) {
                if (Comm_GetLocalNetId() == 0) {
                    *result = 1;
                }
            } else if (Comm_GetLocalNetId() != 0) {
                *result = 1;
            }
        }
        break;
    case CASTLE_ACTION_IS_MULTI:
        *result = BattleCastle_IsMultiType(data->type);
        break;
    case CASTLE_ACTION_GET_TYPE:
        *result = data->type;
        break;
    case CASTLE_ACTION_GET_MON_FAINTED:
        *result = data->monFainted;
        break;
    case CASTLE_ACTION_DRAW_ROUND_TILES:
        BattleCastle_DrawRoundTiles(FrontierSystem_GetFrontierMap(ctx->frontierSystem)->bgConfig, data, 3);
        break;
    case CASTLE_ACTION_LOAD_TRAINERS:
        FrontierTrainer_LoadRecord(&data->trainers[0], data->trainerIds[data->battleNo], HEAP_ID_FIELD2, 204);
        FrontierTrainer_LoadRecord(&data->trainers[1], data->trainerIds[data->battleNo + 7], HEAP_ID_FIELD2, 204);
        break;
    case CASTLE_ACTION_GET_ROUND_TABLE_VALUE:
        *result = BattleCastleData_GetRoundTableValue(data);
        break;
    case 39:
        *result = CastleFlags_GetField(Save_Frontier_GetCastleFlags(launchArgs->saveData), CASTLE_FLAGS_ONCE, 0, 0, 0);
        byteValue = 1;
        CastleFlags_SetField(Save_Frontier_GetCastleFlags(launchArgs->saveData), CASTLE_FLAGS_ONCE, 0, 0, &byteValue);
        break;
    case CASTLE_ACTION_GET_BRAIN_BATTLE:
        *result = 0;
        if (data->type == FRONTIER_CHALLENGE_SINGLE) {
            u32 wins = data->winStreak + 1;
            if (wins == 21) {
                *result = 1;
            } else if (wins == 49) {
                *result = 2;
            }
        }
        break;
    case CASTLE_ACTION_GENERATE_OPPONENTS:
        FrontierMon_GenerateList(data->opponentMons, data->opponentSetIds, data->opponentIVs, data->opponentPersonalities, NULL, 4, HEAP_ID_FIELD2, 205);
        break;
    case CASTLE_ACTION_SET_OPPONENT_PARTY:
        BattleCastle_SetOpponentParty(data);
        break;
    case 43:
        *result = data->unk_013;
        data->unk_013 = 1;
        break;
    case 44: {
        u8 type = data->type;
        if (type == FRONTIER_CHALLENGE_LINK_MULTI) {
            FrontierSave *frontierSave;
            if (arg == 0) {
                frontierSave = Save_Frontier_GetStatic(launchArgs->saveData);
                stat = BattleCastle_GetPointsStat(data->type);
                data->savedPoints = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(type)));
                frontierSave = Save_Frontier_GetStatic(launchArgs->saveData);
                stat = BattleCastle_GetPointsStat(data->type);
                FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(data->type)), data->points);
            } else {
                frontierSave = Save_Frontier_GetStatic(launchArgs->saveData);
                stat = BattleCastle_GetPointsStat(data->type);
                FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(type)), data->savedPoints);
            }
        }
        break;
    }
    }
    return FALSE;
}

BOOL FrtCmd_CastleGetBattleResult(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = ((BattleCastleData *)Frontier_GetData(ctx->frontierSystem->frontierWork))->battleWon;
    return FALSE;
}

BOOL FrtCmd_CastleSendLinkMessage(FrontierScriptContext *ctx) {
    u32 kind = FrontierScript_ReadVar(ctx);
    u16 value = FrontierScript_ReadVar(ctx);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = BattleCastleData_SendLinkMessage(Frontier_GetData(ctx->frontierSystem->frontierWork), kind, value);
    return TRUE;
}

BOOL FrtCmd_CastleWaitLink(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitCastleLink);
    return TRUE;
}

static BOOL FrontierScriptNative_WaitCastleLink(FrontierScriptContext *ctx) {
    FrontierScript_GetVar(ctx, ctx->data[0]);
    BattleCastleData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    if (data->linkMessageCount >= 2) {
        data->linkMessageCount = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_CastleWaitLinkChoice(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitCastleLinkChoice);
    return TRUE;
}

static BOOL FrontierScriptNative_WaitCastleLinkChoice(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_GetVarPtr(ctx, ctx->data[0]);
    BattleCastleData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    if (data->decidedChoice == 0) {
        return FALSE;
    }
    data->linkMessageCount = 0;
    *result = data->decidedChoice;
    return TRUE;
}

BOOL FrtCmd_CastleShowPoints(FrontierScriptContext *ctx) {
    BattleCastle_ShowPointsWindows(ctx->frontierSystem, Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_CastleHidePoints(FrontierScriptContext *ctx) {
    BattleCastle_HidePointsWindows(ctx->frontierSystem, Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_CastlePrintPoints(FrontierScriptContext *ctx) {
    BattleCastle_PrintPoints(ctx->frontierSystem, Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_CastleGetPoints(FrontierScriptContext *ctx) {
    u32 type = FrontierScript_ReadVar(ctx);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    FrontierSave *frontierSave = Save_Frontier_GetStatic(args->saveData);
    u32 stat = BattleCastle_GetPointsStat((u8)type);
    *result = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat((u8)type)));
    return FALSE;
}

BOOL FrtCmd_CastleSpendPoints(FrontierScriptContext *ctx) {
    u32 type = FrontierScript_ReadVar(ctx);
    u32 points = FrontierScript_ReadVar(ctx);
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleCastle_SpendPoints(Save_Frontier_GetStatic(args->saveData), (u8)type, points);
    return FALSE;
}

BOOL FrtCmd_CastleAddPoints(FrontierScriptContext *ctx) {
    u32 type = FrontierScript_ReadVar(ctx);
    u32 points = FrontierScript_ReadVar(ctx);
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleCastle_AddPoints(args->saveData, (u8)type, points);
    return FALSE;
}

BOOL FrtCmd_CastlePrintTrainerMessage(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u8 trainer = FrontierScriptContext_ReadByte(ctx);
    BattleCastleData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    if (data == NULL) {
        return FALSE;
    }
    FrontierScript_PrintMessageEntry(ctx, data->trainers[trainer].introMessage);
    return TRUE;
}
