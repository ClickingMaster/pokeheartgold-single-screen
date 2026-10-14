#include "global.h"

#include "constants/maps.h"

#include "bag.h"
#include "field_system.h"
#include "friend_group.h"
#include "friend_group_access.h"
#include "frontier_data_access.h"
#include "frontier_save_access.h"
#include "heap.h"
#include "launch_application.h"
#include "link_synchronization.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pokedex_util.h"
#include "save_special_ribbons.h"
#include "save_trainer_card.h"
#include "task.h"
#include "communication_core.h"
#include "frontier_opponents.h"
#include "pokemon_summary.h"

// Party menu + summary for a Battle Frontier entry (TaskManager_CallFrontierPartySelect).
typedef struct FrontierPartySelectEnv {
    u32 result;
    u32 state;
    u8 context;
    u8 summaryMode;
    u8 minMons;
    u8 maxMons;
    u8 maxLevel;
    u8 partySlot;
    u8 selectedOrder[6];
    void **runningApp;
} FrontierPartySelectEnv;

typedef struct FrontierWifiConnectionEnv {
    u32 result;
    u32 state;
    void *app;
    u8 filler_C[4];
    u16 outputVar;
    u16 arg1;
    u16 arg2;
} FrontierWifiConnectionEnv;

typedef struct FrontierLinkPartnerCheckEnv {
    u16 kind;
    u16 outputVar;
} FrontierLinkPartnerCheckEnv;

typedef struct Unk67484Inner {
    u8 filler[0x24];
    u16 x;
    u16 z;
    u16 y;
} Unk67484Inner;

typedef struct Unk67484Outer {
    u8 filler[0xA0];
    Unk67484Inner *inner;
} Unk67484Outer;

extern const u8 gFrontierSummaryPages[];

extern FrontierData *Save_FrontierData_Get(SaveData *saveData);
extern void FrontierData_SetTowerSeed(FrontierData *frontierData, u32 value);
extern u32 FrontierData_GetTowerSeed(FrontierData *frontierData);
extern BOOL Comm_HasWifiProfile(SaveData *saveData);

u32 FrontierPartySelect_LaunchPartyMenu(FrontierPartySelectEnv *env, FieldSystem *fieldSystem, enum HeapID heapId);
u32 FrontierPartySelect_WaitPartyMenu(FrontierPartySelectEnv *env, FieldSystem *fieldSystem);
u32 FrontierPartySelect_LaunchSummary(FrontierPartySelectEnv *env, FieldSystem *fieldSystem, enum HeapID heapId);
u32 FrontierPartySelect_WaitSummary(FrontierPartySelectEnv *env, FieldSystem *fieldSystem);
BOOL Task_FrontierPartySelect(TaskManager *taskManager);
void TaskManager_CallFrontierPartySelect(TaskManager *taskManager, void **runningApp, u8 context, u8 summaryMode, u8 minMons, u8 maxMons, u8 maxLevel, u8 partySlot);
u32 FrontierWifiConnection_Launch(FrontierWifiConnectionEnv *env, FieldSystem *fieldSystem);
u32 FrontierWifiConnection_Wait(FrontierWifiConnectionEnv *env, FieldSystem *fieldSystem);
BOOL Task_FrontierWifiConnection(TaskManager *taskManager);
void TaskManager_CallFrontierWifiConnection(TaskManager *taskManager, u16 arg1, u16 outputVar, u16 arg2);
BOOL Task_FrontierLinkPartnerCheck(TaskManager *taskManager);
void TaskManager_CallFrontierLinkPartnerCheck(TaskManager *taskManager, u16 kind, u16 outputVar);
u32 BattleTower_ClaimRecordMilestone(SaveData *saveData);
u32 BattleTower_GetRecordMilestone(SaveData *saveData);
void FrontierFieldSystem_AddBattleTotals(Unk67484Outer *outer, const int *delta);
u32 BattleTower_NextRandom(u32 value);
u32 BattleTower_NextSeed(u32 value);
u32 BattleTower_InitSeed(SaveData *saveData);
u32 BattleTower_AdvanceSeed(SaveData *saveData);
u32 BattleTower_RestoreRandom(SaveData *saveData);
BOOL FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(FieldSystem *fieldSystem);
void Save_DullBadges(SaveData *saveData, int days);

u32 FrontierPartySelect_LaunchPartyMenu(FrontierPartySelectEnv *env, FieldSystem *fieldSystem, enum HeapID heapId) {
    PartyMenuArgs *args = Heap_AllocAtEnd(heapId, sizeof(PartyMenuArgs));
    SaveData *saveData = fieldSystem->saveData;
    u8 i;
    MI_CpuFill8(args, 0, sizeof(PartyMenuArgs));
    args->options = Save_PlayerData_GetOptionsAddr(saveData);
    args->party = SaveArray_Party_Get(saveData);
    args->bag = Save_Bag_Get(saveData);
    args->panelLayout = 0;
    args->context = env->context;
    args->minMonsToSelect = env->minMons;
    args->maxMonsToSelect = env->maxMons;
    args->maxLevel = env->maxLevel;
    args->partySlot = env->partySlot;
    args->menuInputStatePtr = &fieldSystem->menuInputState;
    for (i = 0; i < 6; i++) {
        args->selectedOrder[i] = env->selectedOrder[i];
    }
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PartyMenu, args);
    *env->runningApp = args;
    return 1;
}

u32 FrontierPartySelect_WaitPartyMenu(FrontierPartySelectEnv *env, FieldSystem *fieldSystem) {
    PartyMenuArgs *args;
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return 1;
    }
    args = *env->runningApp;
    switch (args->partySlot) {
    case 7:
        env->result = 0;
        return 4;
    case 6:
        env->result = 1;
        return 4;
    }
    MI_CpuCopy8(args->selectedOrder, env->selectedOrder, 6);
    env->partySlot = args->partySlot;
    Heap_Free(args);
    *env->runningApp = NULL;
    return 2;
}

u32 FrontierPartySelect_LaunchSummary(FrontierPartySelectEnv *env, FieldSystem *fieldSystem, enum HeapID heapId) {
    SaveData *saveData = fieldSystem->saveData;
    PokemonSummaryArgs *args = Heap_AllocAtEnd(heapId, sizeof(PokemonSummaryArgs));
    MI_CpuFill8(args, 0, sizeof(PokemonSummaryArgs));
    args->options = Save_PlayerData_GetOptionsAddr(saveData);
    args->party = SaveArray_Party_Get(saveData);
    args->natDexEnabled = SaveArray_IsNatDexEnabled(saveData);
    args->unk2C = PokemonSummary_GetDefaultUnk2C(saveData);
    args->monDataType = 1;
    args->partySlot = env->partySlot;
    args->partyCount = Party_GetCount(args->party);
    args->moveToLearn = 0;
    args->mode = env->summaryMode;
    args->ribbons = Save_SpecialRibbons_Get(saveData);
    args->menuInputStatePtr = &fieldSystem->menuInputState;
    args->isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(saveData);
    PokemonSummary_SetPages(args, gFrontierSummaryPages);
    PokemonSummaryArgs_SetPlayer(args, Save_PlayerData_GetProfile(saveData));
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PokemonSummary, args);
    *env->runningApp = args;
    return 3;
}

u32 FrontierPartySelect_WaitSummary(FrontierPartySelectEnv *env, FieldSystem *fieldSystem) {
    PokemonSummaryArgs *args;
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return 3;
    }
    args = *env->runningApp;
    env->partySlot = args->partySlot;
    Heap_Free(args);
    *env->runningApp = NULL;
    return 0;
}

BOOL Task_FrontierPartySelect(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierPartySelectEnv *env = TaskManager_GetEnvironment(taskManager);
    u32 *state = &env->state;
    switch (*state) {
    case 0:
        *state = FrontierPartySelect_LaunchPartyMenu(env, fieldSystem, HEAP_ID_FIELD2);
        break;
    case 1:
        *state = FrontierPartySelect_WaitPartyMenu(env, fieldSystem);
        break;
    case 2:
        *state = FrontierPartySelect_LaunchSummary(env, fieldSystem, HEAP_ID_FIELD2);
        break;
    case 3:
        *state = FrontierPartySelect_WaitSummary(env, fieldSystem);
        break;
    case 4:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

void TaskManager_CallFrontierPartySelect(TaskManager *taskManager, void **runningApp, u8 context, u8 summaryMode, u8 minMons, u8 maxMons, u8 maxLevel, u8 partySlot) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierPartySelectEnv *env = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierPartySelectEnv));
    MI_CpuClear8(env, sizeof(FrontierPartySelectEnv));
    env->context = context;
    env->summaryMode = summaryMode;
    env->minMons = minMons;
    env->maxMons = maxMons;
    env->maxLevel = maxLevel;
    env->partySlot = partySlot;
    env->runningApp = runningApp;
    TaskManager_Call(fieldSystem->taskman, Task_FrontierPartySelect, env);
}

u32 FrontierWifiConnection_Launch(FrontierWifiConnectionEnv *env, FieldSystem *fieldSystem) {
    if (Comm_HasWifiProfile(fieldSystem->saveData)) {
        env->app = NintendoWifiConnection_LaunchApp(fieldSystem, env->arg1, env->arg2);
        return 1;
    }
    env->result = 1;
    return 2;
}

u32 FrontierWifiConnection_Wait(FrontierWifiConnectionEnv *env, FieldSystem *fieldSystem) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return 1;
    }
    env->result = ((u32 *)env->app)[8];
    Heap_Free(env->app);
    return 2;
}

BOOL Task_FrontierWifiConnection(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierWifiConnectionEnv *env = TaskManager_GetEnvironment(taskManager);
    switch (env->state) {
    case 0:
        env->state = FrontierWifiConnection_Launch(env, fieldSystem);
        break;
    case 1:
        env->state = FrontierWifiConnection_Wait(env, fieldSystem);
        break;
    case 2:
        *GetVarPointer(fieldSystem, env->outputVar) = env->result;
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

void TaskManager_CallFrontierWifiConnection(TaskManager *taskManager, u16 arg1, u16 outputVar, u16 arg2) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierWifiConnectionEnv *env = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierWifiConnectionEnv));
    MI_CpuClear8(env, sizeof(FrontierWifiConnectionEnv));
    env->arg1 = arg1;
    env->arg2 = arg2;
    env->outputVar = outputVar;
    TaskManager_Call(fieldSystem->taskman, Task_FrontierWifiConnection, env);
}

BOOL Task_FrontierLinkPartnerCheck(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierLinkPartnerCheckEnv *env = TaskManager_GetEnvironment(taskManager);
    u16 *linkData = LinkSync_GetPacket(1 - Comm_GetLocalNetId());
    u16 *output;
    if (linkData == NULL) {
        return FALSE;
    }
    output = GetVarPointer(fieldSystem, env->outputVar);
    switch (env->kind) {
    case 0:
        *output = FrontierFieldSystem_ReceiveLinkAllyData(fieldSystem, linkData);
        break;
    case 1:
        *output = FrontierFieldSystem_ReceiveRoundTrainers(fieldSystem, linkData);
        break;
    case 2:
        *output = FrontierFieldSystem_IsLinkFlagSetByEither(fieldSystem, linkData);
        break;
    }
    Heap_Free(env);
    return TRUE;
}

void TaskManager_CallFrontierLinkPartnerCheck(TaskManager *taskManager, u16 kind, u16 outputVar) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FrontierLinkPartnerCheckEnv *env = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierLinkPartnerCheckEnv));
    MI_CpuClear8(env, sizeof(FrontierLinkPartnerCheckEnv));
    env->kind = kind;
    env->outputVar = outputVar;
    TaskManager_Call(fieldSystem->taskman, Task_FrontierLinkPartnerCheck, env);
}

u32 BattleTower_ClaimRecordMilestone(SaveData *saveData) {
    FrontierData *frontierData;
    u32 stat;
    u8 flag13;
    u8 flag0;
    u8 flag1;

    stat = FrontierSave_GetStat(Save_Frontier_GetStatic(saveData), 0, 0xFF);
    if (stat < 20) {
        return 0;
    }
    frontierData = Save_FrontierData_Get(saveData);
    flag13 = FrontierData_FlagAction(frontierData, 13, 0);
    flag0 = FrontierData_FlagAction(frontierData, 0, 0);
    flag1 = FrontierData_FlagAction(frontierData, 1, 0);
    FrontierData_FlagAction(frontierData, 14, 0);
    FrontierData_FlagAction(frontierData, 2, 0);
    FrontierData_FlagAction(frontierData, 3, 0);
    if (flag13 != 0 && flag0 != 0 && flag1 != 0) {
        return 0;
    }
    if (flag13 == 0) {
        FrontierData_FlagAction(frontierData, 13, 1);
        return 1;
    }
    if (stat < 50) {
        return 0;
    }
    if (flag0 == 0) {
        FrontierData_FlagAction(frontierData, 0, 1);
        return 2;
    }
    if (stat < 100 || flag1 != 0) {
        return 0;
    }
    FrontierData_FlagAction(frontierData, 1, 1);
    return 3;
}

u32 BattleTower_GetRecordMilestone(SaveData *saveData) {
    FrontierData *frontierData;
    u32 stat;
    u8 flag13;
    u8 flag0;
    u8 flag1;
    u8 flag14;
    u8 flag2;
    u8 flag3;

    stat = FrontierSave_GetStat(Save_Frontier_GetStatic(saveData), 0, 0xFF);
    if (stat < 20) {
        return 0;
    }
    frontierData = Save_FrontierData_Get(saveData);
    flag13 = FrontierData_FlagAction(frontierData, 13, 0);
    flag0 = FrontierData_FlagAction(frontierData, 0, 0);
    flag1 = FrontierData_FlagAction(frontierData, 1, 0);
    flag14 = FrontierData_FlagAction(frontierData, 14, 0);
    flag2 = FrontierData_FlagAction(frontierData, 2, 0);
    flag3 = FrontierData_FlagAction(frontierData, 3, 0);
    if (flag13 != 0 && flag0 != 0 && flag1 != 0) {
        return 0;
    }
    if (flag13 == 0) {
        if (flag14 != 0) {
            return 4;
        }
        return 1;
    }
    if (stat < 50) {
        return 0;
    }
    if (flag0 == 0) {
        if (flag2 != 0) {
            return 5;
        }
        return 2;
    }
    if (stat < 100) {
        return 0;
    }
    if (flag1 != 0) {
        return 0;
    }
    if (flag3 != 0) {
        return 6;
    }
    return 3;
}

void FrontierFieldSystem_AddBattleTotals(Unk67484Outer *outer, const int *delta) {
    Unk67484Inner *inner = outer->inner;
    inner->x += delta[0];
    inner->y += delta[1];
    inner->z += delta[2];
}

u32 BattleTower_NextRandom(u32 value) {
    return value * 0x02E90EDD + 1;
}

u32 BattleTower_NextSeed(u32 value) {
    return value * 0x5D588B65 + 1;
}

u32 BattleTower_InitSeed(SaveData *saveData) {
    u32 value = BattleTower_NextSeed(Save_FriendGroup_GetCurrentSeed(Save_FriendGroup_Get(saveData)));
    FrontierData_SetTowerSeed(Save_FrontierData_Get(saveData), value);
    return value;
}

u32 BattleTower_AdvanceSeed(SaveData *saveData) {
    FrontierData *frontierData = Save_FrontierData_Get(saveData);
    u32 value = BattleTower_NextSeed(FrontierData_GetTowerSeed(frontierData));
    u32 next;
    FrontierData_SetTowerSeed(frontierData, value);
    next = BattleTower_NextRandom(value);
    BattleTowerChallenge_SetField(Save_BattleTowerChallenge_Get(saveData), 10, &next);
    return next;
}

u32 BattleTower_RestoreRandom(SaveData *saveData) {
    FrontierData *frontierData = Save_FrontierData_Get(saveData);
    u32 unk = Save_BattleTowerChallenge_Get(saveData);
    u32 value = BattleTower_NextRandom(FrontierData_GetTowerSeed(frontierData));
    s32 i;
    s32 count;
    count = FrontierData_TowerRoundAction(frontierData, (u16)BattleTowerChallenge_GetField(unk, 0, 0), 0) * 24;
    i = 0;
    for (i = 0; i < count; i++) {
        value = BattleTower_NextRandom(value);
    }
    BattleTowerChallenge_SetField(Save_BattleTowerChallenge_Get(saveData), 10, &value);
    return value;
}

BOOL FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(FieldSystem *fieldSystem) {
    return fieldSystem->location->mapId == MAP_BATTLE_TOWER_PARTNER_ROOM;
}

void Save_DullBadges(SaveData *saveData, int days) {
    int *arr;
    PlayerProfile *profile;
    u8 i;
    if (days <= 0) {
        return;
    }
    arr = TrainerCard_GetBadgeShininessArr(Save_TrainerCard_Get(saveData));
    profile = Save_PlayerData_GetProfile(saveData);
    for (i = 0; i < 8; i++) {
        int shininess;
        if (!PlayerProfile_TestBadgeFlag(profile, i)) {
            continue;
        }
        shininess = GetShininessOfBadgeI(i, arr);
        if (shininess > 0 && shininess < 0xc8) {
            shininess = shininess - days * 10;
        } else {
            GF_ASSERT(shininess < 0xc8);
            shininess = 0;
        }
        if (shininess < 0) {
            shininess = 0;
        }
        SetShininessOfBadgeI(i, shininess, arr);
    }
}
