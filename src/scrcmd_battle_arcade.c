#include "scrcmd_battle_arcade.h"

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

typedef enum BattleArcadeChallengeType {
    BATTLE_ARCADE_CHALLENGE_TYPE_SINGLE,
    BATTLE_ARCADE_CHALLENGE_TYPE_DOUBLE,
    BATTLE_ARCADE_CHALLENGE_TYPE_MULTI,
} BattleArcadeChallengeType;

typedef struct FrontierSelectMonsEnv {
    u32 state;
    u8 challengeType;
    u8 partySlot;
    u8 selectedOrder[3];
    u8 filler[2];
    void **appArgs;
} FrontierSelectMonsEnv;

static void BattleArcade_ResetStreak(SaveData *, void *, u8);
static void BattleArcade_StartSelectMons(TaskManager *, void *, BattleArcadeChallengeType);
static BOOL Task_BattleArcadeSelectMons(TaskManager *);
static void BattleArcade_StartTeamCheck(TaskManager *, u16, u16, u16 *);
static BOOL Task_BattleArcadeTeamCheck(TaskManager *);
static u32 BattleArcadeSelectMons_OpenPartyMenu(FrontierSelectMonsEnv *, FieldSystem *, u32);
static u32 BattleArcadeSelectMons_WaitPartyMenu(FrontierSelectMonsEnv *, FieldSystem *);
static u32 BattleArcadeSelectMons_OpenSummary(FrontierSelectMonsEnv *, FieldSystem *, enum HeapID);
static u32 BattleArcadeSelectMons_WaitSummary(FrontierSelectMonsEnv *, FieldSystem *);

static const u8 sBattleArcadeSummaryPages[] = {
    0,
    1,
    2,
    3,
    4,
};

BOOL ScrCmd_BattleArcadeAction(ScriptContext *ctx) {
    u16 command = ScriptReadHalfword(ctx);
    u16 arg1 = ScriptGetVar(ctx);
    u16 *result = ScriptGetVarPointer(ctx);
    Save_Frontier_GetArcadeChallenge(ctx->fieldSystem->saveData);
    void *unk0 = Save_Frontier_GetArcadeFlags(ctx->fieldSystem->saveData);
    void *unk1 = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_RUNNING_APP_DATA);
    switch (command) {
    case 0:
        *result = PartyIsValidForFrontier(arg1, ctx->fieldSystem->saveData, 0);
        break;
    case 1: // Unused
        if (arg1 == 3) {
            *result = FrontierSave_GetStat(Save_Frontier_GetStatic(ctx->fieldSystem->saveData), 110, FrontierStat_GetFriendIndex(110));
        } else {
            *result = ArcadeFlags_GetField(unk0, 8, arg1, 0, 0);
        }
        break;
    case 3: // Unused
        BattleArcade_ResetStreak(ctx->fieldSystem->saveData, unk0, arg1);
        break;
    case 4: {
        BattleArcadeChallengeType challengeType;
        if (arg1 == 0) {
            challengeType = BATTLE_ARCADE_CHALLENGE_TYPE_SINGLE;
        } else if (arg1 == 1) {
            challengeType = BATTLE_ARCADE_CHALLENGE_TYPE_DOUBLE;
        } else {
            challengeType = BATTLE_ARCADE_CHALLENGE_TYPE_MULTI;
        }
        BattleArcade_StartSelectMons(ctx->taskman, unk1, challengeType);
        return TRUE;
    }
    default:
        GF_ASSERT(FALSE);
        *result = 0;
        break;
    }
    return FALSE;
}

BOOL ScrCmd_BattleArcadeResetStreak(ScriptContext *ctx) {
    u16 unk = ScriptGetVar(ctx);
    BattleArcade_ResetStreak(ctx->fieldSystem->saveData, Save_Frontier_GetArcadeFlags(ctx->fieldSystem->saveData), unk);
    return FALSE;
}

static void BattleArcade_ResetStreak(SaveData *saveData, void *arcadeFlags, u8 challengeType) {
    u8 unk;
    unk = 0;
    ArcadeFlags_SetField(arcadeFlags, 8, challengeType, 0, &unk);
    if (challengeType == 3) {
        FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), 110, FrontierStat_GetFriendIndex(110), 0);
    }
    FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), BattleArcade_GetWinStreakStat(challengeType), FrontierStat_GetFriendIndex(BattleArcade_GetWinStreakStat(challengeType)), 0);
}

BOOL ScrCmd_BattleArcadeCheckTeam(ScriptContext *ctx) {
    u16 firstMon = ScriptGetVar(ctx);
    u16 secondMon = ScriptGetVar(ctx);
    u16 *result = ScriptGetVarPointer(ctx);
    BattleArcade_StartTeamCheck(ctx->taskman, firstMon, secondMon, result);
    return TRUE;
}

static void BattleArcade_StartTeamCheck(TaskManager *taskManager, u16 firstMon, u16 secondMon, u16 *result) {
    BattleArcadeTeamCheck *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleArcadeTeamCheck));
    memset(unk, 0, sizeof(BattleArcadeTeamCheck));
    unk->playerTeam[0] = firstMon;
    unk->playerTeam[1] = secondMon;
    unk->result = result;
    FieldComm_RegisterCommands(unk);
    TaskManager_Call(taskManager, Task_BattleArcadeTeamCheck, unk);
}

static BOOL Task_BattleArcadeTeamCheck(TaskManager *taskManager) {
    BattleArcadeTeamCheck *unk = TaskManager_GetEnvironment(taskManager);
    switch (unk->state) {
    case 0:
        if (Comm_SendCommand(134, unk, 16) == 1) {
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

static void BattleArcade_StartSelectMons(TaskManager *taskManager, void *appArgs, BattleArcadeChallengeType challengeType) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierSelectMonsEnv *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierSelectMonsEnv));
    MI_CpuFill8(unk, 0, sizeof(FrontierSelectMonsEnv));
    unk->challengeType = challengeType;
    unk->appArgs = appArgs;
    TaskManager_Call(fieldSystem->taskman, Task_BattleArcadeSelectMons, unk);
}

static BOOL Task_BattleArcadeSelectMons(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierSelectMonsEnv *unk = TaskManager_GetEnvironment(taskManager);
    switch (unk->state) {
    case 0:
        unk->state = BattleArcadeSelectMons_OpenPartyMenu(unk, fieldSystem, HEAP_ID_FIELD2);
        break;
    case 1:
        unk->state = BattleArcadeSelectMons_WaitPartyMenu(unk, fieldSystem);
        break;
    case 2:
        unk->state = BattleArcadeSelectMons_OpenSummary(unk, fieldSystem, HEAP_ID_FIELD2);
        break;
    case 3:
        unk->state = BattleArcadeSelectMons_WaitSummary(unk, fieldSystem);
        break;
    case 4:
        Heap_Free(unk);
        return TRUE;
    }
    return FALSE;
}

static u32 BattleArcadeSelectMons_OpenPartyMenu(FrontierSelectMonsEnv *env, FieldSystem *fieldSystem, u32 heapID) {
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
    if (env->challengeType == BATTLE_ARCADE_CHALLENGE_TYPE_MULTI) {
        partyMenu->minMonsToSelect = 2;
        partyMenu->maxMonsToSelect = 2;
    }
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PartyMenu, partyMenu);
    *(env->appArgs) = partyMenu;
    return 1;
}

static u32 BattleArcadeSelectMons_WaitPartyMenu(FrontierSelectMonsEnv *env, FieldSystem *fieldSystem) {
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

static u32 BattleArcadeSelectMons_OpenSummary(FrontierSelectMonsEnv *env, FieldSystem *fieldSystem, enum HeapID heapID) {
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
    unk->moveToLearn = MOVE_NONE;
    unk->mode = 0;
    unk->ribbons = Save_SpecialRibbons_Get(saveData);
    unk->isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(saveData);
    PokemonSummary_SetPages(unk, sBattleArcadeSummaryPages);
    PokemonSummaryArgs_SetPlayer(unk, Save_PlayerData_GetProfile(saveData));
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PokemonSummary, unk);
    *(env->appArgs) = unk;
    return 3;
}

static u32 BattleArcadeSelectMons_WaitSummary(FrontierSelectMonsEnv *env, FieldSystem *fieldSystem) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return 3;
    }
    PokemonSummaryArgs *pokemonSummaryArgs = *(env->appArgs);
    env->partySlot = pokemonSummaryArgs->partySlot;
    Heap_Free(pokemonSummaryArgs);
    *(env->appArgs) = NULL;
    return 0;
}

void BattleArcade_RecvPartnerTeam(u32 netId, u32 unused, BattleArcadeTeamCheck *received, BattleArcadeTeamCheck *teamCheck) {
    teamCheck->numReceived++;
    if (Comm_GetLocalNetId() != netId) {
        teamCheck->partnerTeam[0] = received->playerTeam[0];
        teamCheck->partnerTeam[1] = received->playerTeam[1];
    }
}
