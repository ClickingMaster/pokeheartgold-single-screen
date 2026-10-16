#include "frontier/frontier_cmd_factory.h"

#include "global.h"

#include "constants/pokemon.h"

#include "battle/battle_setup.h"
#include "frontier/battle_factory_data.h"
#include "frontier/frontier.h"
#include "frontier/frontier_map.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_trainers.h"
#include "frontier/frontier_script_commands.h"

#include "bg_window.h"
#include "heap.h"
#include "launch_application.h"
#include "message_format.h"
#include "party.h"
#include "pokemon.h"
#include "sys_task_api.h"
#include "battle_frontier_communication.h"

// Script commands of the Battle Factory (BattleFactoryData, frontier/battle_factory_data.h).

extern const OverlayManagerTemplate gOverlayTemplate_BattleFactoryRentalApp;
extern const OverlayManagerTemplate gOverlayTemplate_BattleFactoryTradeApp;

u32 FactoryChallenge_GetSaved(void *challenge);
void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, int level);
int BattleFactory_GetPartySize(int type);
int BattleFactory_GetOpponentMonCount(int type, int a1);
BattleSetup *BattleFactory_NewBattleSetup(BattleFactoryData *data, FrontierLaunchArgs *args);
int BattleFactory_GetMonLevel(BattleFactoryData *data);
void BattleFactory_SetPartyToRentals(BattleFactoryData *data);
void BattleFactory_AddPartnerMonsToParty(BattleFactoryData *data);
BOOL BattleFactory_IsMultiType(int type);
u16 BattleFactory_GetRound(BattleFactoryData *data);

void BattleFactory_OnAppExit(void *args);
void BattleFactory_BgScrollTask(SysTask *task, void *map);
BOOL FrontierScriptNative_WaitFactoryLink(FrontierScriptContext *ctx);

BOOL FrtCmd_FactoryAlloc(FrontierScriptContext *ctx) {
    u32 resume = FrontierScript_ReadVar(ctx), type = FrontierScript_ReadVar(ctx), levelMode = FrontierScript_ReadVar(ctx);
    BattleFactoryData *data = BattleFactoryData_Alloc(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData, resume, (u8)type, (u8)levelMode);
    Frontier_SetData(ctx->frontierSystem->frontierWork, (u32)data);
    return FALSE;
}

BOOL FrtCmd_FactoryInit(FrontierScriptContext *ctx) {
    u32 resume = FrontierScript_ReadVar(ctx);
    BattleFactoryData_Init(Frontier_GetData(ctx->frontierSystem->frontierWork), resume);
    return FALSE;
}

BOOL FrtCmd_FactoryFree(FrontierScriptContext *ctx) {
    BattleFactoryData_Free(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_FactoryLaunchRentalApp(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleFactoryData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleFactoryAppLaunchArgs *args = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleFactoryAppLaunchArgs));
    MI_CpuFill8(args, 0, sizeof(BattleFactoryAppLaunchArgs));
    args->saveData = launchArgs->saveData;
    args->type = data->type;
    args->levelMode = data->levelMode;
    args->screen = 0;
    args->playerParty = data->playerParty;
    args->opponentParty = data->opponentParty;
    args->data = data;
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_BattleFactoryRentalApp, args, 0, BattleFactory_OnAppExit);
    return TRUE;
}

BOOL FrtCmd_FactoryEndBattle(FrontierScriptContext *ctx) {
    BattleFactoryData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = data->battleSetup;
    data->battleWon = IsBattleResultWin(setup->winFlag);
    BattleSetup_Delete(setup);
    return FALSE;
}

BOOL FrtCmd_FactoryStartBattle(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleFactoryData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = BattleFactory_NewBattleSetup(data, launchArgs);
    data->battleSetup = setup;
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_Battle, setup, 0, NULL);
    return TRUE;
}

BOOL FrtCmd_FactoryLaunchTradeApp(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleFactoryData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleFactoryAppLaunchArgs *args;
    int i = 0;
    u8 *p = (u8 *)data;
    u16 zero = i;
    do {
        i++;
        ((BattleFactoryData *)p)->selection[0] = zero;
        p += 2;
    } while (i < 6);
    args = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleFactoryAppLaunchArgs));
    MI_CpuFill8(args, 0, sizeof(BattleFactoryAppLaunchArgs));
    args->saveData = launchArgs->saveData;
    args->type = data->type;
    args->levelMode = data->levelMode;
    args->screen = 1;
    args->playerParty = data->playerParty;
    args->opponentParty = data->opponentParty;
    args->data = data;
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_BattleFactoryTradeApp, args, 0, BattleFactory_OnAppExit);
    return TRUE;
}

void BattleFactory_OnAppExit(void *args) {
    BattleFactory_StoreAppResult(((BattleFactoryAppLaunchArgs *)args)->data, args);
    Heap_Free(args);
}

BOOL FrtCmd_FactorySetRentalParty(FrontierScriptContext *ctx) {
    BattleFactory_SetRentalParty(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_FactoryGenerateOpponent(FrontierScriptContext *ctx) {
    BattleFactory_GenerateOpponentMons(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_FactoryTradeMon(FrontierScriptContext *ctx) {
    BattleFactory_TradeMon(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_FactorySetOpponentParty(FrontierScriptContext *ctx) {
    BattleFactory_SetOpponentParty(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

enum {
    FACTORY_ACTION_SET_LEVEL_MODE = 1,
    FACTORY_ACTION_SET_TYPE,
    FACTORY_ACTION_GET_SELECTION,
    FACTORY_ACTION_GET_WIN_STREAK,
    FACTORY_ACTION_INC_WIN_STREAK,
    FACTORY_ACTION_RESET_SYSTEM = 7,
    FACTORY_ACTION_GET_SAVED = 9,
    FACTORY_ACTION_SAVE,
    FACTORY_ACTION_NEXT_BATTLE = 14,
    FACTORY_ACTION_GET_OPPONENT_SPECIES,
    FACTORY_ACTION_GET_OPPONENT_MOVE,
    FACTORY_ACTION_GET_OPPONENT_TYPE,
    FACTORY_ACTION_GET_OPPONENT_MAIN_TYPE,
    FACTORY_ACTION_GET_ROUND,
    FACTORY_ACTION_GET_TRAINER_SPRITE,
    FACTORY_ACTION_COMPLETE_ROUND = 22,
    FACTORY_ACTION_GET_BATTLE_NO,
    FACTORY_ACTION_GET_PARTNER_VALUE_A,
    FACTORY_ACTION_GET_PARTNER_VALUE_B = 26,
    FACTORY_ACTION_REGISTER_LINK_COMMANDS,
    FACTORY_ACTION_IS_MULTI,
    FACTORY_ACTION_GET_TYPE,
    FACTORY_ACTION_SET_BG_PALETTE,
    FACTORY_ACTION_START_BG_SCROLL,
    FACTORY_ACTION_STOP_BG_SCROLL,
    FACTORY_ACTION_LOAD_TRAINERS,
    FACTORY_ACTION_BUFFER_PARTY_NAMES,
    FACTORY_ACTION_GET_WON_BATTLE_POINTS,
    FACTORY_ACTION_INC_TRADE_COUNT,
    FACTORY_ACTION_GET_BRAIN_BATTLE,
    FACTORY_ACTION_ADD_PARTNER_MONS,
    FACTORY_ACTION_GENERATE_OPPONENTS,
    FACTORY_ACTION_SET_PARTY_TO_RENTALS,
};

BOOL FrtCmd_FactoryAction(FrontierScriptContext *ctx) {
    u8 action;
    u8 arg;
    BattleFactoryData *data;
    FrontierMap *map;
    int moveIndex;
    u16 *var;

    action = FrontierScriptContext_ReadByte(ctx);
    arg = FrontierScriptContext_ReadByte(ctx);
    moveIndex = FrontierScriptContext_ReadByte(ctx);
    var = FrontierScript_ReadVarPtr(ctx);
    data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    map = FrontierSystem_GetFrontierMap(ctx->frontierSystem);

    switch (action) {
    case 0:
        data->unk_007 = arg;
        break;
    case FACTORY_ACTION_SET_LEVEL_MODE:
        data->levelMode = arg;
        break;
    case FACTORY_ACTION_SET_TYPE:
        data->type = arg;
        break;
    case FACTORY_ACTION_GET_SELECTION:
        *var = data->selection[arg];
        break;
    case FACTORY_ACTION_GET_WIN_STREAK:
        *var = data->winStreak;
        break;
    case FACTORY_ACTION_INC_WIN_STREAK: {
        u16 winStreak = data->winStreak;
        if (winStreak < 9999) {
            data->winStreak = winStreak + 1;
        }
        break;
    }
    case FACTORY_ACTION_RESET_SYSTEM:
        OS_ResetSystem(0);
        break;
    case FACTORY_ACTION_GET_SAVED:
        *var = FactoryChallenge_GetSaved(data->challenge);
        break;
    case FACTORY_ACTION_SAVE:
        BattleFactoryData_SaveRecord(data, 2);
        break;
    case FACTORY_ACTION_NEXT_BATTLE:
        *var = BattleFactory_IncBattleNo(data);
        break;
    case FACTORY_ACTION_GET_OPPONENT_SPECIES:
        *var = data->opponentMons[arg].species;
        break;
    case FACTORY_ACTION_GET_OPPONENT_MOVE:
        *var = data->opponentMons[arg].moves[moveIndex];
        break;
    case FACTORY_ACTION_GET_OPPONENT_TYPE: {
        Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);
        FrontierMon_ToPokemon(&data->opponentMons[arg], mon, BattleFactory_GetMonLevel(data));
        *var = GetMonData(mon, MON_DATA_TYPE_1, NULL);
        Heap_Free(mon);
        break;
    }
    // the type most of the opponent's Pokemon have, 0xFF when no type is shared
    case FACTORY_ACTION_GET_OPPONENT_MAIN_TYPE: {
        int count;
        int i;
        int types[18];
        int j;
        int zero;
        int *p;
        Pokemon *mon;
        int type1;
        int type2;
        int best;

        count = BattleFactory_GetOpponentMonCount(data->type, 1);
        j = 0;
        zero = j;
        p = types;
        do {
            j++;
            *p = zero;
            p++;
        } while (j < 18);
        mon = AllocMonZeroed(HEAP_ID_FIELD2);
        for (i = 0; i < count; i++) {
            FrontierMon_ToPokemon(&data->opponentMons[i], mon, BattleFactory_GetMonLevel(data));
            type1 = GetMonData(mon, MON_DATA_TYPE_1, NULL);
            type2 = GetMonData(mon, MON_DATA_TYPE_2, NULL);
            if (type1 == type2) {
                type2 = 0xFF;
            }
            types[type1]++;
            if (type2 != 0xFF) {
                types[type2]++;
            }
        }
        Heap_Free(mon);
        best = 0;
        for (j = 0; j < 18; j++) {
            if (types[best] < types[j]) {
                best = j;
            }
        }
        if (types[best] <= 1) {
            *var = 0xFF;
        } else {
            *var = best;
        }
        break;
    }
    case FACTORY_ACTION_GET_ROUND:
        *var = BattleFactory_GetRound(data);
        break;
    case FACTORY_ACTION_GET_TRAINER_SPRITE:
        *var = BattleFactory_GetTrainerSprite(data, arg);
        break;
    case 21:
        BattleFactoryData_SaveRecordMode1(data);
        break;
    case FACTORY_ACTION_COMPLETE_ROUND:
        BattleFactory_CompleteRound(data);
        break;
    case FACTORY_ACTION_GET_BATTLE_NO:
        *var = BattleFactory_GetBattleNo(data);
        break;
    case FACTORY_ACTION_GET_PARTNER_VALUE_A:
        *var = data->partnerValueA;
        break;
    case FACTORY_ACTION_GET_PARTNER_VALUE_B:
        *var = data->partnerValueB;
        break;
    case FACTORY_ACTION_REGISTER_LINK_COMMANDS:
        FrontierLink_RegisterCommands(data);
        break;
    case FACTORY_ACTION_IS_MULTI:
        *var = BattleFactory_IsMultiType(data->type);
        break;
    case FACTORY_ACTION_GET_TYPE:
        *var = data->type;
        break;
    case FACTORY_ACTION_SET_BG_PALETTE:
        BgTilemapRectChangePalette(map->bgConfig, 3, 3, 10, 26, 11, arg);
        ScheduleBgTilemapBufferTransfer(map->bgConfig, 3);
        break;
    case FACTORY_ACTION_START_BG_SCROLL:
        data->scrollTask = SysTask_CreateOnMainQueue(BattleFactory_BgScrollTask, FrontierSystem_GetFrontierMap(ctx->frontierSystem), 5);
        break;
    case FACTORY_ACTION_STOP_BG_SCROLL:
        if (data->scrollTask != NULL) {
            SysTask_Destroy(data->scrollTask);
            data->scrollTask = NULL;
        }
        break;
    case FACTORY_ACTION_LOAD_TRAINERS:
        FrontierTrainer_LoadRecord(&data->trainers[0], data->trainerIds[data->battleNo], HEAP_ID_FIELD2, NARC_a_2_0_2);
        FrontierTrainer_LoadRecord(&data->trainers[1], data->trainerIds[data->battleNo + 7], HEAP_ID_FIELD2, NARC_a_2_0_2);
        break;
    case FACTORY_ACTION_BUFFER_PARTY_NAMES: {
        int count = BattleFactory_GetPartySize(data->type);
        int i;
        for (i = 0; i < count; i++) {
            BufferBoxMonSpeciesName(ctx->frontierSystem->msgFormat, i, Mon_GetBoxMon(Party_GetMonByIndex(data->playerParty, i)));
        }
        break;
    }
    case FACTORY_ACTION_GET_WON_BATTLE_POINTS:
        *var = BattleFactory_GetWonBattlePoints(data);
        break;
    case FACTORY_ACTION_INC_TRADE_COUNT:
        BattleFactory_IncTradeCount(data);
        break;
    // 1 before the Factory Head's first battle (21st), 2 before the second (49th)
    case FACTORY_ACTION_GET_BRAIN_BATTLE:
        *var = 0;
        if (data->type == 0) {
            int battle = data->winStreak + 1;
            if (battle == 21) {
                *var = 1;
            } else if (battle == 49) {
                *var = 2;
            }
        }
        break;
    case FACTORY_ACTION_ADD_PARTNER_MONS:
        BattleFactory_AddPartnerMonsToParty(data);
        break;
    case FACTORY_ACTION_GENERATE_OPPONENTS:
        FrontierMon_GenerateList(data->opponentMons, data->opponentSetIds, data->opponentIVs, data->opponentPersonalities, NULL, 4, HEAP_ID_FIELD2, NARC_a_2_0_3);
        break;
    case FACTORY_ACTION_SET_PARTY_TO_RENTALS:
        BattleFactory_SetPartyToRentals(data);
        break;
    case 41:
        *var = data->unk_00B;
        data->unk_00B = 1;
        break;
    }
    return FALSE;
}

void BattleFactory_BgScrollTask(SysTask *task, void *map) {
    (void)task;
    if (Bg_GetYpos(((FrontierMap *)map)->bgConfig, GF_BG_LYR_MAIN_2) >= 0xFF) {
        ScheduleSetBgPosText(((FrontierMap *)map)->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
    } else {
        ScheduleSetBgPosText(((FrontierMap *)map)->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_ADD_Y, 1);
    }
}

BOOL FrtCmd_FactoryGetBattleResult(FrontierScriptContext *ctx) {
    u16 *var = FrontierScript_ReadVarPtr(ctx);
    *var = (u16)((BattleFactoryData *)Frontier_GetData(ctx->frontierSystem->frontierWork))->battleWon;
    return FALSE;
}

BOOL FrtCmd_FactorySendBuffer(FrontierScriptContext *ctx) {
    u32 command = FrontierScript_ReadVar(ctx), value = FrontierScript_ReadVar(ctx);
    u16 *var = FrontierScript_ReadVarPtr(ctx);
    *var = BattleFactory_SendBuffer(Frontier_GetData(ctx->frontierSystem->frontierWork), command, value);
    return TRUE;
}

BOOL FrtCmd_FactoryReceiveBuffer(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitFactoryLink);
    return TRUE;
}

BOOL FrontierScriptNative_WaitFactoryLink(FrontierScriptContext *ctx) {
    BattleFactoryData *data;
    FrontierScript_GetVar(ctx, ctx->data[0]);
    data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    if (data->linkMessageCount >= 2) {
        data->linkMessageCount = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_FactoryPrintMsg(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u8 index = FrontierScript_ReadByte(ctx);
    BattleFactoryData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    if (data == NULL) {
        return FALSE;
    }
    FrontierScript_PrintMessageEntry(ctx, data->trainers[index].introMessage);
    return TRUE;
}
