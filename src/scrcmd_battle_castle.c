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
#include "unk_0205BB1C.h"
#include "unk_0205BFF0.h"
#include "unk_02088288.h"
#include "unk_02091564.h"

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

BOOL ScrCmd_637(ScriptContext *ctx) {
    u16 arg0 = ScriptReadHalfword(ctx);
    u16 arg1 = ScriptGetVar(ctx);
    u16 *result = ScriptGetVarPointer(ctx);
    Save_Frontier_GetCastleChallenge(ctx->fieldSystem->saveData);
    u32 unk0 = Save_Frontier_GetCastleFlags(ctx->fieldSystem->saveData);
    void *unk1 = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_RUNNING_APP_DATA);
    switch (arg0) {
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

BOOL ScrCmd_640(ScriptContext *ctx) {
    u16 unk = ScriptGetVar(ctx);
    BattleCastle_ResetStreak(ctx->fieldSystem->saveData, Save_Frontier_GetCastleFlags(ctx->fieldSystem->saveData), unk);
    return FALSE;
}

static void BattleCastle_ResetStreak(SaveData *saveData, u32 a1, u8 a2) {
    u8 unk;
    unk = 0;
    CastleFlags_SetField(a1, 9, a2, 0, &unk);
    if (a2 == 3) {
        FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), 108, FrontierStat_GetFriendIndex(108), 0);
    }
    FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), BattleCastle_GetWinStreakStat(a2), FrontierStat_GetFriendIndex(BattleCastle_GetWinStreakStat(a2)), 0);
    FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), BattleCastle_GetPointsStat(a2), FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(a2)), 0);
    for (s32 i = 0; i < 3; i++) {
        FrontierSave_SetStat(Save_Frontier_GetStatic(saveData), BattleCastle_GetRankStat(a2, i), FrontierStat_GetFriendIndex(BattleCastle_GetRankStat(a2, i)), 1);
    }
}

BOOL ScrCmd_638(ScriptContext *ctx) {
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

static void BattleCastle_StartSelectMons(TaskManager *taskManager, void *a1, BattleCastleChallengeType challengeType) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierSelectMonsEnv *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierSelectMonsEnv));
    MI_CpuFill8(unk, 0, sizeof(FrontierSelectMonsEnv));
    unk->challengeType = challengeType;
    unk->appArgs = a1;
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

static u32 BattleCastleSelectMons_OpenPartyMenu(FrontierSelectMonsEnv *a0, FieldSystem *fieldSystem, enum HeapID unused) {
    PartyMenuArgs *partyMenu = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
    MIi_CpuClearFast(0, (u32 *)partyMenu, sizeof(PartyMenuArgs));
    partyMenu->party = SaveArray_Party_Get(fieldSystem->saveData);
    partyMenu->bag = Save_Bag_Get(fieldSystem->saveData);
    partyMenu->mailbox = Save_Mailbox_Get(fieldSystem->saveData);
    partyMenu->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    partyMenu->panelLayout = 0;
    partyMenu->context = PARTY_MENU_CONTEXT_23;
    partyMenu->fieldSystem = fieldSystem;
    partyMenu->partySlot = a0->partySlot;
    for (u8 i = 0; i < 3; i++) {
        partyMenu->selectedOrder[i] = a0->selectedOrder[i];
    }
    partyMenu->maxLevel = 100;
    partyMenu->minMonsToSelect = 3;
    partyMenu->maxMonsToSelect = 3;
    partyMenu->menuInputStatePtr = &(fieldSystem->menuInputState);
    if (a0->challengeType == BATTLE_CASTLE_CHALLENGE_TYPE_MULTI) {
        partyMenu->minMonsToSelect = 2;
        partyMenu->maxMonsToSelect = 2;
    }
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PartyMenu, partyMenu);
    *(a0->appArgs) = partyMenu;
    return 1;
}

static u32 BattleCastleSelectMons_WaitPartyMenu(FrontierSelectMonsEnv *a0, FieldSystem *fieldSystem) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return TRUE;
    }
    PartyMenuArgs *partyMenu = *(a0->appArgs);
    switch (partyMenu->partySlot) {
    case 7:
        return 4;
    case 6:
        return 4;
    }
    MI_CpuCopy8(partyMenu->selectedOrder, a0->selectedOrder, 3);
    a0->partySlot = partyMenu->partySlot;
    Heap_Free(partyMenu);
    *(a0->appArgs) = NULL;
    return 2;
}

static u32 BattleCastleSelectMons_OpenSummary(FrontierSelectMonsEnv *a0, FieldSystem *fieldSystem, enum HeapID heapID) {
    SaveData *saveData = fieldSystem->saveData;
    PokemonSummaryArgs *unk = Heap_AllocAtEnd(heapID, sizeof(PokemonSummaryArgs));
    MI_CpuFill8(unk, 0, sizeof(PokemonSummaryArgs));
    unk->options = Save_PlayerData_GetOptionsAddr(saveData);
    unk->party = SaveArray_Party_Get(saveData);
    unk->natDexEnabled = SaveArray_IsNatDexEnabled(saveData);
    unk->unk2C = sub_02088288(saveData);
    unk->monDataType = 1;
    unk->partySlot = a0->partySlot;
    unk->partyCount = Party_GetCount(unk->party);
    unk->moveToLearn = 0;
    unk->mode = 0;
    unk->ribbons = Save_SpecialRibbons_Get(saveData);
    unk->isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(saveData);
    PokemonSummary_SetPages(unk, sBattleCastleSummaryPages);
    PokemonSummaryArgs_SetPlayer(unk, Save_PlayerData_GetProfile(saveData));
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PokemonSummary, unk);
    *(a0->appArgs) = unk;
    return 3;
}

static u32 BattleCastleSelectMons_WaitSummary(FrontierSelectMonsEnv *a0, FieldSystem *fieldSystem) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return 3;
    }
    PokemonSummaryArgs *unk = *(a0->appArgs);
    a0->partySlot = unk->partySlot;
    Heap_Free(unk);
    *(a0->appArgs) = NULL;
    return 0;
}

void BattleCastle_RecvPartnerTeam(u32 a0, u32 unused, BattleCastleTeamCheck *a2, BattleCastleTeamCheck *a3) {
    a3->numReceived++;
    if (Comm_GetLocalNetId() != a0) {
        a3->partnerTeam[0] = a2->playerTeam[0];
        a3->partnerTeam[1] = a2->playerTeam[1];
    }
}
