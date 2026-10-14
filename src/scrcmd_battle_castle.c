#include "scrcmd_battle_castle.h"

#include "global.h"

#include "assert.h"
#include "bag.h"
#include "field_system.h"
#include "frontier_data.h"
#include "game_stats.h"
#include "heap.h"
#include "launch_application.h"
#include "mail.h"
#include "message_format.h"
#include "party.h"
#include "party_menu.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pokedex_util.h"
#include "save.h"
#include "save_special_ribbons.h"
#include "scrcmd.h"
#include "script.h"
#include "script_manager.h"
#include "task.h"
#include "frontier_save_access.h"
#include "communication_core.h"
#include "frontier_field_system.h"
#include "field_misc_util.h"
#include "frontier_stat_ids_legacy.h"
#include "pokemon_summary.h"
#include "field_comm_cmds.h"

typedef enum BattleCastleChallengeType {
    BATTLE_CASTLE_CHALLENGE_TYPE_SINGLE,
    BATTLE_CASTLE_CHALLENGE_TYPE_DOUBLE,
    BATTLE_CASTLE_CHALLENGE_TYPE_MULTI,
} BattleCastleChallengeType;

typedef struct FrontierSelectMonsEnv {
    u32 state;
    u8 challengeType;
    u8 partySlot;
    u8 selectedOrder[3];
    u8 filler[3];
    void **appArgs;
} FrontierSelectMonsEnv;

static void BattleCastle_ResetStreak(SaveData *, u32, u8);
static void BattleCastle_StartTeamCheck(TaskManager *, u16, u16, u16 *);
static BOOL Task_BattleCastleTeamCheck(TaskManager *);
static void BattleCastle_StartSelectMons(TaskManager *, void *, BattleCastleChallengeType);
static BOOL Task_BattleCastleSelectMons(TaskManager *);
static u32 BattleCastleSelectMons_OpenPartyMenu(FrontierSelectMonsEnv *, FieldSystem *, enum HeapID);
static u32 BattleCastleSelectMons_WaitPartyMenu(FrontierSelectMonsEnv *, FieldSystem *);
static u32 BattleCastleSelectMons_OpenSummary(FrontierSelectMonsEnv *, FieldSystem *, enum HeapID);
static u32 BattleCastleSelectMons_WaitSummary(FrontierSelectMonsEnv *, FieldSystem *);

static const u8 sBattleCastleSummaryPages[] = {
    0,
    1,
    2,
    3,
    4,
};

BOOL ScrCmd_BattleCastleAction(ScriptContext *ctx) {
    u16 command = ScriptReadHalfword(ctx);
    u16 arg1 = ScriptGetVar(ctx);
    u16 *result = ScriptGetVarPointer(ctx);
    Save_Frontier_GetCastleChallenge(ctx->fieldSystem->saveData);
    u32 unk0 = Save_Frontier_GetCastleFlags(ctx->fieldSystem->saveData);
    void *unk1 = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_RUNNING_APP_DATA);
    switch (command) {
    case 0:
        *result = PartyIsValidForFrontier(arg1, ctx->fieldSystem->saveData, 0);
        break;
    case 1: // Unused
        if (arg1 == 3) {
            *result = FrontierSave_GetStat(Save_Frontier_GetStatic(ctx->fieldSystem->saveData), 108, FrontierStat_GetFriendIndex(108));
        } else {
            *result = CastleFlags_GetField(unk0, 9, arg1, 0, 0);
        }
        break;
    case 3: // Unused
        BattleCastle_ResetStreak(ctx->fieldSystem->saveData, unk0, arg1);
        break;
    case 4: {
        BattleCastleChallengeType challengeType;
        if (arg1 == 0) {
            challengeType = BATTLE_CASTLE_CHALLENGE_TYPE_SINGLE;
        } else if (arg1 == 1) {
            challengeType = BATTLE_CASTLE_CHALLENGE_TYPE_DOUBLE;
        } else {
            challengeType = BATTLE_CASTLE_CHALLENGE_TYPE_MULTI;
        }
        BattleCastle_StartSelectMons(ctx->taskman, unk1, challengeType);
        return TRUE;
    }
    default:
        GF_ASSERT(FALSE);
        *result = 0;
        break;
    }
    return FALSE;
}

BOOL ScrCmd_BattleCastleResetStreak(ScriptContext *ctx) {
    u16 unk = ScriptGetVar(ctx);
    BattleCastle_ResetStreak(ctx->fieldSystem->saveData, Save_Frontier_GetCastleFlags(ctx->fieldSystem->saveData), unk);
    return FALSE;
}

static void BattleCastle_ResetStreak(SaveData *saveData, u32 castleFlags, u8 challengeType) {
    u8 unk;
    unk = 0;
    CastleFlags_SetField(castleFlags, 9, challengeType, 0, &unk);
    if (challengeType == 3) {
        FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), 108, FrontierStat_GetFriendIndex(108), 0);
    }
    FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), BattleCastle_GetWinStreakStat(challengeType), FrontierStat_GetFriendIndex(BattleCastle_GetWinStreakStat(challengeType)), 0);
    FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), BattleCastle_GetPointsStat(challengeType), FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(challengeType)), 0);
    for (s32 i = 0; i < 3; i++) {
        FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), BattleCastle_GetRankStat(challengeType, i), FrontierStat_GetFriendIndex(BattleCastle_GetRankStat(challengeType, i)), 1);
    }
}

BOOL ScrCmd_BattleCastleCheckTeam(ScriptContext *ctx) {
    u16 firstMon = ScriptGetVar(ctx);
    u16 secondMon = ScriptGetVar(ctx);
    u16 *result = ScriptGetVarPointer(ctx);
    BattleCastle_StartTeamCheck(ctx->taskman, firstMon, secondMon, result);
    return TRUE;
}

static void BattleCastle_StartTeamCheck(TaskManager *taskManager, u16 firstMon, u16 secondMon, u16 *result) {
    BattleCastleTeamCheck *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleCastleTeamCheck));
    memset(unk, 0, sizeof(BattleCastleTeamCheck));
    unk->playerTeam[0] = firstMon;
    unk->playerTeam[1] = secondMon;
    unk->result = result;
    FieldComm_RegisterCommands(unk);
    TaskManager_Call(taskManager, Task_BattleCastleTeamCheck, unk);
}

static BOOL Task_BattleCastleTeamCheck(TaskManager *taskManager) {
    BattleCastleTeamCheck *unk = TaskManager_GetEnvironment(taskManager);
    switch (unk->state) {
    case 0:
        if (Comm_SendCommand(133, unk, 16) == TRUE) {
            unk->state++;
        }
        break;
    case 1:
        if (unk->numReceived >= 2) {
            *unk->result = 0;
            if (unk->playerTeam[0] == unk->partnerTeam[0] || unk->playerTeam[0] == unk->partnerTeam[1]) {
                *unk->result += 1;
            }
            if (unk->playerTeam[1] == unk->partnerTeam[0] || unk->playerTeam[1] == unk->partnerTeam[1]) {
                *unk->result += 2;
            }
            unk->state++;
        }
        break;
    case 2:
        Heap_Free(unk);
        return TRUE;
    }
    return FALSE;
}

static void BattleCastle_StartSelectMons(TaskManager *taskManager, void *appArgs, BattleCastleChallengeType challengeType) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierSelectMonsEnv *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierSelectMonsEnv));
    MI_CpuFill8(unk, 0, sizeof(FrontierSelectMonsEnv));
    unk->challengeType = challengeType;
    unk->appArgs = appArgs;
    TaskManager_Call(fieldSystem->taskman, Task_BattleCastleSelectMons, unk);
}

static BOOL Task_BattleCastleSelectMons(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierSelectMonsEnv *unk = TaskManager_GetEnvironment(taskManager);
    switch (unk->state) {
    case 0:
        unk->state = BattleCastleSelectMons_OpenPartyMenu(unk, fieldSystem, HEAP_ID_FIELD2);
        break;
    case 1:
        unk->state = BattleCastleSelectMons_WaitPartyMenu(unk, fieldSystem);
        break;
    case 2:
        unk->state = BattleCastleSelectMons_OpenSummary(unk, fieldSystem, HEAP_ID_FIELD2);
        break;
    case 3:
        unk->state = BattleCastleSelectMons_WaitSummary(unk, fieldSystem);
        break;
    case 4:
        Heap_Free(unk);
        return TRUE;
    }
    return FALSE;
}

static u32 BattleCastleSelectMons_OpenPartyMenu(FrontierSelectMonsEnv *env, FieldSystem *fieldSystem, enum HeapID unused) {
    PartyMenuArgs *partyMenu = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
    MIi_CpuClearFast(0, (u32 *)partyMenu, sizeof(PartyMenuArgs));
    partyMenu->party = SaveArray_Party_Get(fieldSystem->saveData);
    partyMenu->bag = Save_Bag_Get(fieldSystem->saveData);
    partyMenu->mailbox = Save_Mailbox_Get(fieldSystem->saveData);
    partyMenu->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    partyMenu->panelLayout = 0;
    partyMenu->context = PARTY_MENU_CONTEXT_23;
    partyMenu->fieldSystem = fieldSystem;
    partyMenu->partySlot = env->partySlot;
    for (u8 i = 0; i < 3; i++) {
        partyMenu->selectedOrder[i] = env->selectedOrder[i];
    }
    partyMenu->maxLevel = 100;
    partyMenu->minMonsToSelect = 3;
    partyMenu->maxMonsToSelect = 3;
    partyMenu->menuInputStatePtr = &(fieldSystem->menuInputState);
    if (env->challengeType == BATTLE_CASTLE_CHALLENGE_TYPE_MULTI) {
        partyMenu->minMonsToSelect = 2;
        partyMenu->maxMonsToSelect = 2;
    }
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PartyMenu, partyMenu);
    *(env->appArgs) = partyMenu;
    return 1;
}

static u32 BattleCastleSelectMons_WaitPartyMenu(FrontierSelectMonsEnv *env, FieldSystem *fieldSystem) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return TRUE;
    }
    PartyMenuArgs *partyMenu = *(env->appArgs);
    switch (partyMenu->partySlot) {
    case 7:
        return 4;
    case 6:
        return 4;
    }
    MI_CpuCopy8(partyMenu->selectedOrder, env->selectedOrder, 3);
    env->partySlot = partyMenu->partySlot;
    Heap_Free(partyMenu);
    *(env->appArgs) = NULL;
    return 2;
}

static u32 BattleCastleSelectMons_OpenSummary(FrontierSelectMonsEnv *env, FieldSystem *fieldSystem, enum HeapID heapID) {
    SaveData *saveData = fieldSystem->saveData;
    PokemonSummaryArgs *unk = Heap_AllocAtEnd(heapID, sizeof(PokemonSummaryArgs));
    MI_CpuFill8(unk, 0, sizeof(PokemonSummaryArgs));
    unk->options = Save_PlayerData_GetOptionsAddr(saveData);
    unk->party = SaveArray_Party_Get(saveData);
    unk->natDexEnabled = SaveArray_IsNatDexEnabled(saveData);
    unk->unk2C = sub_02088288(saveData);
    unk->monDataType = 1;
    unk->partySlot = env->partySlot;
    unk->partyCount = Party_GetCount(unk->party);
    unk->moveToLearn = 0;
    unk->mode = 0;
    unk->ribbons = Save_SpecialRibbons_Get(saveData);
    unk->isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(saveData);
    PokemonSummary_SetPages(unk, sBattleCastleSummaryPages);
    PokemonSummaryArgs_SetPlayer(unk, Save_PlayerData_GetProfile(saveData));
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PokemonSummary, unk);
    *(env->appArgs) = unk;
    return 3;
}

static u32 BattleCastleSelectMons_WaitSummary(FrontierSelectMonsEnv *env, FieldSystem *fieldSystem) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return 3;
    }
    PokemonSummaryArgs *unk = *(env->appArgs);
    env->partySlot = unk->partySlot;
    Heap_Free(unk);
    *(env->appArgs) = NULL;
    return 0;
}

void BattleCastle_RecvPartnerTeam(u32 netId, u32 unused, BattleCastleTeamCheck *received, BattleCastleTeamCheck *teamCheck) {
    teamCheck->numReceived++;
    if (Comm_GetLocalNetId() != netId) {
        teamCheck->partnerTeam[0] = received->playerTeam[0];
        teamCheck->partnerTeam[1] = received->playerTeam[1];
    }
}
