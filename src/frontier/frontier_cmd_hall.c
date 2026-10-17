#include "frontier/frontier_cmd_hall.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "battle/battle_setup.h"
#include "frontier/battle_hall_data.h"
#include "frontier/battle_hall_setup.h"
#include "frontier/frontier.h"
#include "frontier/frontier_script_commands.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_trainers.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "launch_application.h"
#include "party.h"
#include "pokemon.h"
#include "sound_system.h"

extern const OverlayManagerTemplate gOverlayTemplate_BattleHallApp;

u32 HallChallenge_GetSaved(void *challenge);
u32 HallTypeRanks_Get(u8 cell, void *ranks);
void HallTypeRanks_Set(u8 cell, void *ranks, u8 rank);
u32 BattleHallData_GetTrainerSprite(BattleHallData *data, u8 index);
u32 BattleHallData_SendLinkMessage(BattleHallData *data, u32 kind, u32 value);
int BattleHall_GetPartySize(u8 type);
void FrontierLink_RegisterCommands(void);

static void BattleHall_OnAppExit(void *args);
static BOOL FrontierScriptNative_WaitHallLink(FrontierScriptContext *ctx);

BOOL FrtCmd_HallAlloc(FrontierScriptContext *ctx) {
    u16 resume = FrontierScript_ReadVar(ctx);
    u16 type = FrontierScript_ReadVar(ctx);
    u16 slot0 = FrontierScript_ReadVar(ctx);
    u16 slot1 = FrontierScript_ReadVar(ctx);
    BattleHallData *data = BattleHallData_Alloc(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData, resume, (u8)type, (u8)slot0, (u8)slot1);
    Frontier_SetData(ctx->frontierSystem->frontierWork, (u32)data);
    return FALSE;
}

BOOL FrtCmd_HallInit(FrontierScriptContext *ctx) {
    u16 resume = FrontierScript_ReadVar(ctx);
    BattleHallData_Init(Frontier_GetData(ctx->frontierSystem->frontierWork), resume);
    return FALSE;
}

BOOL FrtCmd_HallFree(FrontierScriptContext *ctx) {
    BattleHallData_Free(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_HallLaunchApp(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleHallData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleHallAppArgs *args = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleHallAppArgs));
    Party *party;
    Pokemon *mon;

    MI_CpuFill8(args, 0, sizeof(BattleHallAppArgs));
    args->saveData = launchArgs->saveData;
    args->mode = data->type;
    args->data = data;
    args->hallRecords = data->typeRanks[data->type];
    args->party = data->party;
    args->battleNo = data->winStreak;
    args->cursor = data->cell;
    args->partnerLevel = &data->partnerLevel;
    SaveArray_Party_Init(args->party);
    party = SaveArray_Party_Get(launchArgs->saveData);
    mon = Party_GetMonByIndex(party, data->partySlots[0]);
    Party_AddMon(args->party, mon);
    if (args->mode == 1) {
        mon = Party_GetMonByIndex(party, data->partySlots[1]);
        Party_AddMon(args->party, mon);
    } else if ((u8)(args->mode + 0xFE) <= 1) {
        // Multi battles (modes 2 and 3) also bring the partner's Pokemon
        Party_AddMon(args->party, data->partnerMon);
    }
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_BattleHallApp, args, 0, BattleHall_OnAppExit);
    return TRUE;
}

BOOL FrtCmd_HallEndBattle(FrontierScriptContext *ctx) {
    BattleHallData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = data->battleSetup;
    data->battleWon = IsBattleResultWin(setup->winFlag);
    BattleSetup_Delete(setup);
    return FALSE;
}

BOOL FrtCmd_HallStartBattle(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleHallData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = BattleHall_NewBattleSetup(data, args);
    data->battleSetup = setup;
    Sound_SetSceneAndPlayBGM(5, SEQ_GS_VS_TRAINER, 1);
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_Battle, setup, 0, NULL);
    return TRUE;
}

static void BattleHall_OnAppExit(void *args) {
    BattleHallAppArgs *appArgs = args;
    BattleHallData_ReadAppResult(appArgs->data, appArgs);
    Heap_Free(appArgs);
}

enum {
    HALL_ACTION_SET_TYPE = 2,
    HALL_ACTION_GET_CURSOR,
    HALL_ACTION_GET_WIN_STREAK,
    HALL_ACTION_INC_WIN_STREAK,
    HALL_ACTION_RESET_SYSTEM = 7,
    HALL_ACTION_GET_SAVED = 9,
    HALL_ACTION_SAVE,
    HALL_ACTION_NEXT_BATTLE = 14,
    HALL_ACTION_GET_SPECIES,
    HALL_ACTION_GET_TYPE = 17,
    HALL_ACTION_GET_ROUND = 19,
    HALL_ACTION_GET_TRAINER_SPRITE,
    HALL_ACTION_CLEAR_RANKS,
    HALL_ACTION_END_ROUND,
    HALL_ACTION_GET_BATTLE_NO,
    HALL_ACTION_GET_PARTNER_VALUE,
    HALL_ACTION_RESTORE_HELD_ITEMS = 27,
    HALL_ACTION_RAISE_TYPE_RANK,
    HALL_ACTION_REGISTER_LINK_COMMANDS,
    HALL_ACTION_IS_MULTI,
    HALL_ACTION_TOGGLE_BG2,
    HALL_ACTION_LOAD_TRAINERS,
    HALL_ACTION_GET_TYPE_RANK,
    HALL_ACTION_GET_WON_BATTLE_POINTS,
    HALL_ACTION_RESET_MAXED_RANKS,
    HALL_ACTION_UPDATE_LEVEL_SQRT,
    HALL_ACTION_GET_MATRON_BATTLE,
};

BOOL FrtCmd_HallAction(FrontierScriptContext *ctx) {
    u8 action;
    u8 arg;
    u16 *result;
    FrontierLaunchArgs *launchArgs;
    BattleHallData *data;

    action = FrontierScriptContext_ReadByte(ctx);
    arg = FrontierScriptContext_ReadByte(ctx);
    ctx->scriptPtr++;
    result = FrontierScript_ReadVarPtr(ctx);
    launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    data = Frontier_GetData(ctx->frontierSystem->frontierWork);

    switch (action) {
    case HALL_ACTION_SET_TYPE:
        data->type = arg;
        break;
    case HALL_ACTION_GET_CURSOR:
        *result = data->cursor;
        break;
    case HALL_ACTION_GET_WIN_STREAK:
        *result = data->winStreak;
        break;
    case HALL_ACTION_INC_WIN_STREAK:
        if (data->winStreak < 9999) {
            data->winStreak++;
        }
        break;
    case HALL_ACTION_RESET_SYSTEM:
        OS_ResetSystem(0);
        break;
    case HALL_ACTION_GET_SAVED:
        *result = HallChallenge_GetSaved(data->challenge);
        break;
    case HALL_ACTION_SAVE:
        BattleHallData_Save(data, 2);
        break;
    case HALL_ACTION_NEXT_BATTLE:
        *result = BattleHallData_NextBattle(data);
        break;
    case HALL_ACTION_GET_ROUND:
        if (data->type == 2) {
            *result = 10;
        } else {
            *result = data->round;
        }
        break;
    case HALL_ACTION_GET_TRAINER_SPRITE:
        *result = BattleHallData_GetTrainerSprite(data, arg);
        break;
    case HALL_ACTION_CLEAR_RANKS:
        BattleHallData_ClearRanks(data);
        break;
    case HALL_ACTION_END_ROUND:
        BattleHallData_EndRound(data);
        break;
    case HALL_ACTION_GET_BATTLE_NO:
        *result = BattleHallData_GetBattleNo(data);
        break;
    case HALL_ACTION_GET_PARTNER_VALUE:
        *result = data->partnerValue;
        break;
    case HALL_ACTION_RESTORE_HELD_ITEMS: {
        // Give the party back the items they held before the challenge
        Party *party = SaveArray_Party_Get(launchArgs->saveData);
        int count = BattleHall_GetPartySize(data->type);
        int i = 0;
        if (count > 0) {
            u16 *item = data->heldItems;
            do {
                Pokemon *mon = Party_GetMonByIndex(party, data->partySlots[i]);
                SetMonData(mon, MON_DATA_HELD_ITEM, item);
                i++;
                item++;
            } while (i < count);
        }
        break;
    }
    case HALL_ACTION_RAISE_TYPE_RANK: {
        u32 rank = HallTypeRanks_Get(data->cell, data->typeRanks[data->type]);
        if (rank < 10) {
            HallTypeRanks_Set(data->cell, data->typeRanks[data->type], (u8)(rank + 1));
        }
        break;
    }
    case HALL_ACTION_REGISTER_LINK_COMMANDS:
        FrontierLink_RegisterCommands();
        break;
    case HALL_ACTION_IS_MULTI:
        *result = BattleHall_IsMultiType(data->type);
        break;
    case HALL_ACTION_GET_TYPE:
        *result = data->type;
        break;
    case HALL_ACTION_TOGGLE_BG2:
        if (arg == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        } else {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        }
        break;
    case HALL_ACTION_LOAD_TRAINERS:
        FrontierTrainer_LoadRecord(&data->trainers[0], data->trainerIds[data->battleNo * 2], HEAP_ID_FIELD2, 204);
        FrontierTrainer_LoadRecord(&data->trainers[1], data->trainerIds[data->battleNo * 2 + 1], HEAP_ID_FIELD2, 204);
        break;
    case HALL_ACTION_GET_TYPE_RANK:
        *result = HallTypeRanks_Get(data->cell, data->typeRanks[data->type]);
        break;
    case HALL_ACTION_GET_WON_BATTLE_POINTS:
        *result = BattleHallData_GetWonBattlePoints(data);
        break;
    case HALL_ACTION_GET_SPECIES: {
        Party *party = SaveArray_Party_Get(launchArgs->saveData);
        Pokemon *mon = Party_GetMonByIndex(party, data->partySlots[0]);
        *result = GetMonData(mon, MON_DATA_SPECIES, NULL);
        break;
    }
    case HALL_ACTION_RESET_MAXED_RANKS:
        BattleHallData_ResetMaxedRanks(data);
        break;
    case HALL_ACTION_UPDATE_LEVEL_SQRT:
        BattleHallData_UpdateLevelSqrt(data);
        break;
    case HALL_ACTION_GET_MATRON_BATTLE:
        // The Hall Matron appears in the single battle streak
        *result = 0;
        if (data->type == 0) {
            if (data->winStreak + 1 == 50) {
                *result = 1;
            } else if (data->winStreak + 1 == 170) {
                *result = 2;
            }
        }
        break;
    case 38:
        break;
    }
    return FALSE;
}

BOOL FrtCmd_HallGetBattleResult(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = ((BattleHallData *)Frontier_GetData(ctx->frontierSystem->frontierWork))->battleWon;
    return FALSE;
}

BOOL FrtCmd_HallSendLinkMessage(FrontierScriptContext *ctx) {
    u16 kind = FrontierScript_ReadVar(ctx);
    u16 value = FrontierScript_ReadVar(ctx);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = BattleHallData_SendLinkMessage(Frontier_GetData(ctx->frontierSystem->frontierWork), kind, value);
    return TRUE;
}

BOOL FrtCmd_HallWaitLink(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitHallLink);
    return TRUE;
}

static BOOL FrontierScriptNative_WaitHallLink(FrontierScriptContext *ctx) {
    BattleHallData *data;
    FrontierScript_GetVar(ctx, ctx->data[0]);
    data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    if (data->linkMessageCount >= 2) {
        data->linkMessageCount = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_HallPrintTrainerMessage(FrontierScriptContext *ctx) {
    u8 trainer;
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    trainer = FrontierScriptContext_ReadByte(ctx);
    {
        BattleHallData *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
        if (data == NULL) {
            return FALSE;
        }
        FrontierScript_PrintMessageEntry(ctx, data->trainers[trainer].introMessage);
    }
    return TRUE;
}

BOOL FrtCmd_HallUpdateSpeciesRecord(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    BattleHallData_UpdateSpeciesRecord(Frontier_GetData(ctx->frontierSystem->frontierWork), result);
    return TRUE;
}

BOOL FrtCmd_Dummy199(FrontierScriptContext *ctx) {
    return FALSE;
}
