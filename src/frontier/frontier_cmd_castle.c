#include "global.h"

#include "constants/pokemon.h"

#include "battle/battle_setup.h"
#include "frontier/battle_castle_data.h"
#include "frontier/frontier.h"
#include "frontier/frontier_map.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_trainers.h"

#include "frontier_save_access.h"
#include "heap.h"
#include "launch_application.h"
#include "party.h"
#include "pokemon.h"
#include "battle_frontier_communication.h"

// Script commands of the Battle Castle (BattleCastleData, frontier/battle_castle_data.h).

// Defined in frontier_script_commands.c.
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
u16 *FrontierScript_GetVarPtr(FrontierScriptContext *ctx, u32 index);
u16 FrontierScript_GetVar(FrontierScriptContext *ctx, u16 index);
void FrontierScript_PrintMessageEntry(FrontierScriptContext *ctx, u16 *message);

extern const OverlayManagerTemplate ov80_0223BE24;
extern const OverlayManagerTemplate gOverlayTemplate_BattleCastlePartyApp;
extern const OverlayManagerTemplate gOverlayTemplate_BattleCastleOpponentApp;

u32 BattleCastle_GetPointsStat(u8 type);
u32 FrontierStat_GetFriendIndex(u32 stat);
u32 CastleChallenge_GetSaved(void *challenge);
void OS_ResetSystem(u32 parameter);
u32 Comm_GetLocalNetId(void);
BattleSetup *BattleCastle_NewBattleSetup(BattleCastleData *data, FrontierLaunchArgs *args);
BOOL BattleCastle_IsMultiType(u8 type);
void BattleCastle_SetOpponentParty(BattleCastleData *data);
u16 BattleCastle_GetRound(BattleCastleData *data);
void BattleCastle_DrawRoundTiles(BgConfig *bgConfig, BattleCastleData *data, int a2);
void BattleCastle_SpendPoints(FrontierSave *frontierSave, u8 type, int points);

void BattleCastle_FillAppArgs(BattleCastleAppArgs *args, BattleCastleData *data);
void BattleCastle_OnAppExit(void *args);
BOOL FrontierScriptNative_WaitCastleLink(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitCastleLinkChoice(FrontierScriptContext *ctx);

BOOL FrtCmd_CastleAlloc(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleInit(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleFree(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleLaunchPartyApp(FrontierScriptContext *ctx);
BOOL FrtCmd_164(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleEndBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleStartBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleLaunchOpponentApp(FrontierScriptContext *ctx);
BOOL FrtCmd_158(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleGenerateOpponents(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleAction(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleGetBattleResult(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleSendLinkMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleWaitLink(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleWaitLinkChoice(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleShowPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleHidePoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastlePrintPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleGetPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleSpendPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleAddPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastlePrintTrainerMessage(FrontierScriptContext *ctx);

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
    return BattleCastle_LaunchApp(ctx, &gOverlayTemplate_BattleCastlePartyApp);
}

BOOL FrtCmd_164(FrontierScriptContext *ctx) {
    return BattleCastle_LaunchApp(ctx, &ov80_0223BE24);
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
    return BattleCastle_LaunchApp(ctx, &gOverlayTemplate_BattleCastleOpponentApp);
}

void BattleCastle_FillAppArgs(BattleCastleAppArgs *args, BattleCastleData *data) {
    int i;

    args->type = data->type;
    args->playerParty = data->playerParty;
    args->opponentParty = data->opponentParty;
    args->data = data;
    args->partnerPoints = data->partnerPoints;
    for (i = 0; i < 4; i++) {
        args->unk_08[i] = data->unk_370[i];
        args->unk_0C[i] = data->unk_374[i];
        args->unk_10[i] = data->unk_378[i];
        args->unk_14[i] = data->unk_37C[i];
    }
}

void BattleCastle_OnAppExit(void *args) {
    BattleCastleAppArgs *appArgs = args;
    BattleCastleData_ReadAppResult(appArgs->data, appArgs);
    Heap_Free(appArgs);
}

BOOL FrtCmd_158(FrontierScriptContext *ctx) {
    BattleCastleData_SetPartyBeforeBattle(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_CastleGenerateOpponents(FrontierScriptContext *ctx) {
    BattleCastleData_GenerateOpponents(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

#pragma force_active on
BOOL FrtCmd_CastleAction(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *launchArgs;
    int arg;
    u32 action = FrontierScriptContext_ReadByte(ctx);
    arg = FrontierScriptContext_ReadByte(ctx);
    int arg2 = FrontierScriptContext_ReadByte(ctx);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    BattleCastleData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u8 byteValue;
    u32 stat;
    switch (action) {
    case 2:
        data->type = arg;
        break;
    case 3:
        *result = data->selections[arg];
        break;
    case 4:
        *result = data->winStreak;
        break;
    case 5:
        if (data->winStreak < 9999) {
            data->winStreak++;
        }
        break;
    case 7:
        OS_ResetSystem(0);
        break;
    case 9:
        *result = CastleChallenge_GetSaved(data->challenge);
        break;
    case 10:
        BattleCastleData_Save(data, 2);
        break;
    case 14:
        *result = BattleCastleData_NextBattle(data);
        break;
    case 15:
        *result = data->opponentMons[arg].species;
        break;
    case 16: {
        FrontierMon *opponentMon = &data->opponentMons[arg];
        *result = opponentMon->moves[arg2];
        break;
    }
    case 18: {
        Party *party = SaveArray_Party_Get(launchArgs->saveData);
        int i;
        u16 *heldItem = data->heldItems;
        for (i = 0; i < 3; i++) {
            SetMonData(Party_GetMonByIndex(party, data->partySlots[i]), MON_DATA_HELD_ITEM, heldItem++);
        }
        break;
    }
    case 19:
        *result = BattleCastle_GetRound(data);
        break;
    case 20:
        *result = BattleCastleData_GetTrainerSprite(data, arg);
        break;
    case 21:
        BattleCastleData_ResetRanks(data);
        break;
    case 22:
        BattleCastleData_EndRound(data);
        break;
    case 23:
        *result = BattleCastleData_GetBattleNo(data);
        break;
    case 24:
        *result = data->unk_A10;
        break;
    case 26:
        *result = data->unk_A11;
        break;
    case 27:
        *result = BattleCastleData_CalcPointsEarned(data);
        BattleCastle_AddPoints(data->saveData, data->type, *result);
        break;
    case 28:
        BattleCastleData_RecordLinkPartyPP(data);
        break;
    case 29:
        FrontierLink_RegisterCommands(data);
        break;
    case 30:
        if (data->decidedChoice >= 6) {
            *result = data->decidedChoice - 6;
        } else {
            *result = data->decidedChoice;
        }
        break;
    case 31:
        data->decidedChoice = 0;
        data->partnerChoice = 0;
        data->choice = 0;
        break;
    case 32:
        data->choice = arg;
        break;
    case 33: {
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
    case 34:
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
    case 35:
        *result = BattleCastle_IsMultiType(data->type);
        break;
    case 17:
        *result = data->type;
        break;
    case 25:
        *result = data->monFainted;
        break;
    case 36:
        BattleCastle_DrawRoundTiles(FrontierSystem_GetFrontierMap(ctx->frontierSystem)->bgConfig, data, 3);
        break;
    case 37:
        FrontierTrainer_LoadRecord(&data->trainers[0], data->trainerIds[data->battleNo], HEAP_ID_FIELD2, 204);
        FrontierTrainer_LoadRecord(&data->trainers[1], data->trainerIds[data->battleNo + 7], HEAP_ID_FIELD2, 204);
        break;
    case 38:
        *result = ov80_02233648(data);
        break;
    case 39:
        *result = CastleFlags_GetField(Save_Frontier_GetCastleFlags(launchArgs->saveData), 10, 0, 0, 0);
        byteValue = 1;
        CastleFlags_SetField(Save_Frontier_GetCastleFlags(launchArgs->saveData), 10, 0, 0, &byteValue);
        break;
    case 40:
        *result = 0;
        if (data->type == 0) {
            u32 wins = data->winStreak + 1;
            if (wins == 21) {
                *result = 1;
            } else if (wins == 49) {
                *result = 2;
            }
        }
        break;
    case 41:
        FrontierMon_GenerateList(data->opponentMons, data->opponentSetIds, data->opponentIVs, data->opponentPersonalities, NULL, 4, HEAP_ID_FIELD2, 205);
        break;
    case 42:
        BattleCastle_SetOpponentParty(data);
        break;
    case 43:
        *result = data->unk_013;
        data->unk_013 = 1;
        break;
    case 44: {
        u8 type = data->type;
        if (type == 3) {
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
#pragma force_active reset

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

BOOL FrontierScriptNative_WaitCastleLink(FrontierScriptContext *ctx) {
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

BOOL FrontierScriptNative_WaitCastleLinkChoice(FrontierScriptContext *ctx) {
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
    FrontierScript_PrintMessageEntry(ctx, data->trainers[trainer].unk18);
    return TRUE;
}
