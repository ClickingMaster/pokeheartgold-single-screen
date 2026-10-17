#include "wifi_club.h"

#include "global.h"

#include "application/plaza_game.h"
#include "application/plaza_game_mime_jr_top.h"
#include "application/plaza_game_swalot_plop.h"

#include "comm_state.h"
#include "encounter.h"
#include "field_system.h"
#include "frontier_launch.h"
#include "heap.h"
#include "launch_application.h"
#include "overlay_13_thumb_1.h"
#include "overlay_44.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "task.h"
#include "wifi_club_app.h"
#include "wobbuffet_pop.h"

// Wi-Fi Club: the field task that runs the Wi-Fi Club (overlay 44) and whatever
// the player chose there: a link battle (free, level 50 or level 100; single or double), a trade, the Battle Frontier,
// one of the three minigames (each behind the game entry screen of overlay 44), or the Wi-Fi settings, which reset
// the system afterwards. The Wi-Fi Club comes back after each of them until the player leaves.

FS_EXTERN_OVERLAY(OVY_44);
FS_EXTERN_OVERLAY(OVY_90);
FS_EXTERN_OVERLAY(OVY_91);
FS_EXTERN_OVERLAY(OVY_92);
FS_EXTERN_OVERLAY(OVY_93);

// What the player chose in the Wi-Fi Club (WifiClubArgs.unk4, written by overlay 44).
enum WifiClubChoice {
    WIFI_CLUB_BATTLE_SINGLE_LV50 = 1,
    WIFI_CLUB_BATTLE_SINGLE_LV100,
    WIFI_CLUB_BATTLE_SINGLE_FREE,
    WIFI_CLUB_BATTLE_DOUBLE_LV50,
    WIFI_CLUB_BATTLE_DOUBLE_LV100,
    WIFI_CLUB_BATTLE_DOUBLE_FREE,
    WIFI_CLUB_TRADE,
    WIFI_CLUB_EXIT,
    WIFI_CLUB_WIFI_SETTINGS,
    WIFI_CLUB_CANCEL,
    WIFI_CLUB_FRONTIER = 12,
    WIFI_CLUB_SWALOT_PLOP,
    WIFI_CLUB_MIME_JR_TOP,
    WIFI_CLUB_WOBBUFFET_POP,
};

// The minigames, as numbered by the game entry screen.
enum WifiClubGame {
    WIFI_CLUB_GAME_SWALOT_PLOP = 1,
    WIFI_CLUB_GAME_MIME_JR_TOP,
    WIFI_CLUB_GAME_WOBBUFFET_POP,
};

BOOL LoadOVY13(void);

enum WifiClubTaskState {
    WIFI_CLUB_TASK_INIT,
    WIFI_CLUB_TASK_START,
    WIFI_CLUB_TASK_OPEN_CLUB,
    WIFI_CLUB_TASK_HANDLE_CHOICE,
    WIFI_CLUB_TASK_START_BATTLE,
    WIFI_CLUB_TASK_END_BATTLE,
    WIFI_CLUB_TASK_START_TRADE,
    WIFI_CLUB_TASK_END_TRADE,
    WIFI_CLUB_TASK_WIFI_SETTINGS,
    WIFI_CLUB_TASK_EXIT,
    WIFI_CLUB_TASK_EXIT_EARLY,
    WIFI_CLUB_TASK_EXIT_CANCELLED,
    WIFI_CLUB_TASK_START_FRONTIER,
    WIFI_CLUB_TASK_WAIT_FRONTIER,
    WIFI_CLUB_TASK_SWALOT_PLOP_ENTRY,
    WIFI_CLUB_TASK_WAIT_SWALOT_PLOP_ENTRY,
    WIFI_CLUB_TASK_START_SWALOT_PLOP,
    WIFI_CLUB_TASK_WAIT_SWALOT_PLOP,
    WIFI_CLUB_TASK_MIME_JR_TOP_ENTRY,
    WIFI_CLUB_TASK_WAIT_MIME_JR_TOP_ENTRY,
    WIFI_CLUB_TASK_START_MIME_JR_TOP,
    WIFI_CLUB_TASK_WAIT_MIME_JR_TOP,
    WIFI_CLUB_TASK_WOBBUFFET_POP_ENTRY,
    WIFI_CLUB_TASK_WAIT_WOBBUFFET_POP_ENTRY,
    WIFI_CLUB_TASK_START_WOBBUFFET_POP,
    WIFI_CLUB_TASK_WAIT_WOBBUFFET_POP,
};

typedef struct WifiClubTask {
    WifiClubArgs *clubArgs; // 0x00
    u32 state;              // 0x04
    u16 *result;            // 0x08: script variable, 1 when the player cancelled
    u8 maxLevel;            // 0x0C: of the link battle, 0 for no limit
    u8 doubleBattle;        // 0x0D
    u8 pad0E[2];            // 0x0E
    void *launchedArgs;     // 0x10: of the game entry screen or minigame
    u32 selection;          // 0x14: picked on the game entry screen, passed on to the minigame
} WifiClubTask;

BOOL Task_WifiClub(TaskManager *taskManager);
WifiClubTask *WifiClubTask_New(void);
void WifiClub_LaunchGameEntry(WifiClubTask *task, FieldSystem *fieldSystem, enum HeapID heapID, int game);
u32 WifiClub_FinishGameEntry(WifiClubTask *task);
PlazaGameArgs *WifiClub_LaunchSwalotPlop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection);
void WifiClub_FinishSwalotPlop(PlazaGameArgs *args);
PlazaGameArgs *WifiClub_LaunchMimeJrTop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection);
void WifiClub_FinishMimeJrTop(PlazaGameArgs *args);
PlazaGameArgs *WifiClub_LaunchWobbuffetPop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection);
void WifiClub_FinishWobbuffetPop(PlazaGameArgs *args);

// player limit of the game entry screen, by game
static const u8 sWifiClubGameMaxPlayers[] = { 3, 4, 4, 4 };

static const OverlayManagerTemplate sOverlayTemplate_WifiClubGameEntry = { WifiClubGameEntry_Init, WifiClubGameEntry_Main, WifiClubGameEntry_Exit, FS_OVERLAY_ID(OVY_44) };
static const OverlayManagerTemplate sOverlayTemplate_MimeJrTop = { MimeJrTop_Init, MimeJrTop_Main, MimeJrTop_Exit, FS_OVERLAY_ID(OVY_92) };
static const OverlayManagerTemplate sOverlayTemplate_SwalotPlop = { SwalotPlop_Init, SwalotPlop_Main, SwalotPlop_Exit, FS_OVERLAY_ID(OVY_91) };
static const OverlayManagerTemplate sOverlayTemplate_WifiClub = { WifiClub_Init, WifiClub_Main, WifiClub_Exit, FS_OVERLAY_ID(OVY_44) };
static const OverlayManagerTemplate sOverlayTemplate_WobbuffetPop = { WobbuffetPopApp_Init, WobbuffetPopApp_Main, WobbuffetPopApp_Exit, FS_OVERLAY_ID(OVY_93) };

BOOL Task_WifiClub(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    WifiClubTask *task = TaskManager_GetEnvironment(taskManager);
    switch (task->state) {
    case WIFI_CLUB_TASK_INIT:
        task->clubArgs->saveData = fieldSystem->saveData;
    case WIFI_CLUB_TASK_START:
        task->state++;
        if (task->clubArgs->choice == 1 && Comm_HasWifiProfile(fieldSystem->saveData)) {
            task->state = WIFI_CLUB_TASK_EXIT_EARLY;
            *task->result = 0;
        }
        break;
    case WIFI_CLUB_TASK_OPEN_CLUB:
        CallApplicationAsTask(taskManager, &sOverlayTemplate_WifiClub, task->clubArgs);
        task->state++;
        break;
    case WIFI_CLUB_TASK_HANDLE_CHOICE:
        if (Comm_HasWifiProfile(fieldSystem->saveData)) {
            SetFlag970(Save_VarsFlags_Get(fieldSystem->saveData));
        }
        switch (task->clubArgs->choice) {
        case WIFI_CLUB_BATTLE_SINGLE_FREE:
            task->maxLevel = 0;
            task->doubleBattle = FALSE;
            task->state = WIFI_CLUB_TASK_START_BATTLE;
            break;
        case WIFI_CLUB_BATTLE_SINGLE_LV50:
            task->maxLevel = 50;
            task->doubleBattle = FALSE;
            task->state = WIFI_CLUB_TASK_START_BATTLE;
            break;
        case WIFI_CLUB_BATTLE_SINGLE_LV100:
            task->maxLevel = 100;
            task->doubleBattle = FALSE;
            task->state = WIFI_CLUB_TASK_START_BATTLE;
            break;
        case WIFI_CLUB_BATTLE_DOUBLE_FREE:
            task->maxLevel = 0;
            task->doubleBattle = TRUE;
            task->state = WIFI_CLUB_TASK_START_BATTLE;
            break;
        case WIFI_CLUB_BATTLE_DOUBLE_LV50:
            task->maxLevel = 50;
            task->doubleBattle = TRUE;
            task->state = WIFI_CLUB_TASK_START_BATTLE;
            break;
        case WIFI_CLUB_BATTLE_DOUBLE_LV100:
            task->maxLevel = 100;
            task->doubleBattle = TRUE;
            task->state = WIFI_CLUB_TASK_START_BATTLE;
            break;
        case WIFI_CLUB_TRADE:
            task->state = WIFI_CLUB_TASK_START_TRADE;
            break;
        case WIFI_CLUB_CANCEL:
            *task->result = 1;
            task->state = WIFI_CLUB_TASK_EXIT_CANCELLED;
            break;
        case WIFI_CLUB_EXIT:
            task->state = WIFI_CLUB_TASK_EXIT;
            break;
        case WIFI_CLUB_FRONTIER:
            task->state = WIFI_CLUB_TASK_START_FRONTIER;
            break;
        case WIFI_CLUB_SWALOT_PLOP:
            task->state = WIFI_CLUB_TASK_SWALOT_PLOP_ENTRY;
            break;
        case WIFI_CLUB_MIME_JR_TOP:
            task->state = WIFI_CLUB_TASK_MIME_JR_TOP_ENTRY;
            break;
        case WIFI_CLUB_WOBBUFFET_POP:
            task->state = WIFI_CLUB_TASK_WOBBUFFET_POP_ENTRY;
            break;
        case WIFI_CLUB_WIFI_SETTINGS:
            task->state = WIFI_CLUB_TASK_WIFI_SETTINGS;
            break;
        }
        break;
    case WIFI_CLUB_TASK_START_BATTLE:
        CallTask_WifiClubBattle(taskManager, task->clubArgs->partnerFriendIndex, task->maxLevel, task->doubleBattle);
        task->state++;
        break;
    case WIFI_CLUB_TASK_END_BATTLE:
        task->state = WIFI_CLUB_TASK_OPEN_CLUB;
        break;
    case WIFI_CLUB_TASK_START_TRADE:
        CallTask_WirelessTrade(taskManager);
        task->state++;
        break;
    case WIFI_CLUB_TASK_END_TRADE:
        task->state = WIFI_CLUB_TASK_OPEN_CLUB;
        break;
    case WIFI_CLUB_TASK_WIFI_SETTINGS:
        Heap_Create(HEAP_ID_3, HEAP_ID_53, 0x40100);
        LoadOVY13();
        RunWifiSetupApp(HEAP_ID_53);
        OS_ResetSystem(0);
        break;
    case WIFI_CLUB_TASK_EXIT:
    case WIFI_CLUB_TASK_EXIT_EARLY:
    case WIFI_CLUB_TASK_EXIT_CANCELLED:
        Heap_Free(task->clubArgs);
        Heap_Free(task);
        task->state++;
        return TRUE;
    case WIFI_CLUB_TASK_START_FRONTIER:
        Comm_SetWifiContinuousSend(0);
        task->launchedArgs = Frontier_LaunchApp(fieldSystem, NULL);
        task->state++;
        break;
    case WIFI_CLUB_TASK_WAIT_FRONTIER:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            Heap_Free(task->launchedArgs);
            task->state = WIFI_CLUB_TASK_OPEN_CLUB;
        }
        break;
    case WIFI_CLUB_TASK_SWALOT_PLOP_ENTRY:
        WifiClub_LaunchGameEntry(task, fieldSystem, HEAP_ID_FIELD2, WIFI_CLUB_GAME_SWALOT_PLOP);
        task->state++;
        break;
    case WIFI_CLUB_TASK_WAIT_SWALOT_PLOP_ENTRY:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WifiClub_FinishGameEntry(task);
        }
        break;
    case WIFI_CLUB_TASK_START_SWALOT_PLOP:
        task->launchedArgs = WifiClub_LaunchSwalotPlop(fieldSystem, HEAP_ID_FIELD2, task->selection);
        task->state++;
        break;
    case WIFI_CLUB_TASK_WAIT_SWALOT_PLOP:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WIFI_CLUB_TASK_OPEN_CLUB;
            WifiClub_FinishSwalotPlop(task->launchedArgs);
        }
        break;
    case WIFI_CLUB_TASK_MIME_JR_TOP_ENTRY:
        WifiClub_LaunchGameEntry(task, fieldSystem, HEAP_ID_FIELD2, WIFI_CLUB_GAME_MIME_JR_TOP);
        task->state++;
        break;
    case WIFI_CLUB_TASK_WAIT_MIME_JR_TOP_ENTRY:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WifiClub_FinishGameEntry(task);
        }
        break;
    case WIFI_CLUB_TASK_START_MIME_JR_TOP:
        task->launchedArgs = WifiClub_LaunchMimeJrTop(fieldSystem, HEAP_ID_FIELD2, task->selection);
        task->state++;
        break;
    case WIFI_CLUB_TASK_WAIT_MIME_JR_TOP:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WIFI_CLUB_TASK_OPEN_CLUB;
            WifiClub_FinishMimeJrTop(task->launchedArgs);
        }
        break;
    case WIFI_CLUB_TASK_WOBBUFFET_POP_ENTRY:
        WifiClub_LaunchGameEntry(task, fieldSystem, HEAP_ID_FIELD2, WIFI_CLUB_GAME_WOBBUFFET_POP);
        task->state++;
        break;
    case WIFI_CLUB_TASK_WAIT_WOBBUFFET_POP_ENTRY:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WifiClub_FinishGameEntry(task);
        }
        break;
    case WIFI_CLUB_TASK_START_WOBBUFFET_POP:
        task->launchedArgs = WifiClub_LaunchWobbuffetPop(fieldSystem, HEAP_ID_FIELD2, task->selection);
        task->state++;
        break;
    case WIFI_CLUB_TASK_WAIT_WOBBUFFET_POP:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WIFI_CLUB_TASK_OPEN_CLUB;
            WifiClub_FinishWobbuffetPop(task->launchedArgs);
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

WifiClubTask *WifiClubTask_New(void) {
    WifiClubTask *task = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(WifiClubTask));
    MI_CpuFill8(task, 0, sizeof(WifiClubTask));
    task->clubArgs = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(WifiClubArgs));
    MI_CpuFill8(task->clubArgs, 0, sizeof(WifiClubArgs));
    return task;
}

void CallTask_WifiClub(TaskManager *taskManager) {
    WifiClubTask *task = WifiClubTask_New();
    task->clubArgs->choice = 2; // start mode for overlay 44
    TaskManager_Call(taskManager, Task_WifiClub, task);
}

void CallTask_WifiClubWithResult(TaskManager *taskManager, u16 *result) {
    WifiClubTask *task = WifiClubTask_New();
    task->clubArgs->choice = 1; // start mode for overlay 44; may end at once (result 0)
    task->result = result;
    TaskManager_Call(taskManager, Task_WifiClub, task);
}

void WifiClub_LaunchGameEntry(WifiClubTask *task, FieldSystem *fieldSystem, enum HeapID heapID, int game) {
    WifiClubGameEntryArgs *args = Heap_Alloc(heapID, sizeof(WifiClubGameEntryArgs));
    args->game = game;
    args->minPlayers = 2;
    args->maxPlayers = sWifiClubGameMaxPlayers[game];
    args->result = 0;
    args->voiceChat = 0;
    args->saveData = fieldSystem->saveData;
    task->launchedArgs = args;
    FieldSystem_LaunchApplication(fieldSystem, &sOverlayTemplate_WifiClubGameEntry, args);
}

u32 WifiClub_FinishGameEntry(WifiClubTask *task) {
    WifiClubGameEntryArgs *args = task->launchedArgs;
    if (args->result == 1) {
        switch ((enum WifiClubGame)args->game) {
        case WIFI_CLUB_GAME_SWALOT_PLOP:
            task->state = WIFI_CLUB_TASK_START_SWALOT_PLOP;
            break;
        case WIFI_CLUB_GAME_MIME_JR_TOP:
            task->state = WIFI_CLUB_TASK_START_MIME_JR_TOP;
            break;
        default:
        case WIFI_CLUB_GAME_WOBBUFFET_POP:
            task->state = WIFI_CLUB_TASK_START_WOBBUFFET_POP;
            break;
        }
    } else {
        task->state = WIFI_CLUB_TASK_START;
    }
    task->selection = args->voiceChat;
    Heap_Free(task->launchedArgs);
    return task->state;
}

PlazaGameArgs *WifiClub_LaunchSwalotPlop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection) {
    PlazaGameArgs *args = Heap_Alloc(heapID, 0x3C);
    memset(args, 0, 0x3C);
    args->voiceChat = selection;
    args->fromLobby = FALSE;
    args->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &sOverlayTemplate_SwalotPlop, args);
    return args;
}

void WifiClub_FinishSwalotPlop(PlazaGameArgs *args) {
    Heap_Free(args);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
}

PlazaGameArgs *WifiClub_LaunchMimeJrTop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection) {
    PlazaGameArgs *args = Heap_Alloc(heapID, sizeof(PlazaGameArgs));
    memset(args, 0, sizeof(PlazaGameArgs));
    args->mimeJrTopVoiceChat = selection;
    *(u32 *)&args->voiceChat = 0; // read as one word: launched from the lobby
    args->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &sOverlayTemplate_MimeJrTop, args);
    return args;
}

void WifiClub_FinishMimeJrTop(PlazaGameArgs *args) {
    Heap_Free(args);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
}

PlazaGameArgs *WifiClub_LaunchWobbuffetPop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection) {
    PlazaGameArgs *args = Heap_Alloc(heapID, 0x3C);
    MI_CpuFill8(args, 0, 0x3C);
    args->voiceChat = selection;
    args->fromLobby = FALSE;
    args->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &sOverlayTemplate_WobbuffetPop, args);
    return args;
}

void WifiClub_FinishWobbuffetPop(PlazaGameArgs *args) {
    Heap_Free(args);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
}

// Connection filter of the Wi-Fi Club: TRUE when this player's activity and the other player's activity form one of
// the matching pairs.
BOOL WifiClub_IsMatchingActivity(s32 netId) {
    WifiClubStatusData *status = CommState_GetWifiClubData();
    u8 own = status->own.status;
    u8 other = status->friends[netId].status;
    if (own == 0xC && other == 5) {
        return TRUE;
    }
    if (own == 0xD && other == 6) {
        return TRUE;
    }
    if (own == 0xE && other == 7) {
        return TRUE;
    }
    if (own == 9 && other == 2) {
        return TRUE;
    }
    if (own == 0xA && other == 3) {
        return TRUE;
    }
    if (own == 0xB && other == 4) {
        return TRUE;
    }
    if (own == 0xF && other == 8) {
        return TRUE;
    }
    if (own == 0x13 && other == 0x12) {
        return TRUE;
    }
    if (own == 0x15 && other == 0x14) {
        return TRUE;
    }
    if (own == 0x17 && other == 0x16) {
        return TRUE;
    }
    if (own == 0x19 && other == 0x18) {
        return TRUE;
    }
    if (own == 0x1B && other == 0x1A) {
        return TRUE;
    }
    if (own == 0x10 && other == 1) {
        return TRUE;
    }
    return FALSE;
}
