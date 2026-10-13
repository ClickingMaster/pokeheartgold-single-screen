#include "wifi_club.h"

#include "global.h"

#include "application/plaza_game.h"

#include "comm_state.h"
#include "encounter.h"
#include "field_system.h"
#include "frontier_launch.h"
#include "heap.h"
#include "launch_application.h"
#include "overlay_13_thumb_1.h"
#include "overlay_44.h"
#include "overlay_44_02232E9C.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "task.h"

// Wi-Fi Club (ARM9 static 0x02078834-0x02078DD8): the field task that runs the Wi-Fi Club (overlay 44) and whatever
// the player chose there: a link battle (free, level 50 or level 100; single or double), a trade, the Battle Frontier,
// one of the three minigames (each behind the game entry screen of overlay 44), or the Wi-Fi settings, which reset
// the system afterwards. The Wi-Fi Club comes back after each of them until the player leaves.

FS_EXTERN_OVERLAY(OVY_90);

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

BOOL LoadOVY13(void); // network_overlay_loader.c

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

// asm/wifi_club_data.s
extern const u8 gWifiClubGameMaxPlayers[]; // player limit of the game entry screen, by game
extern const OverlayManagerTemplate gOverlayTemplate_SwalotPlop;
extern const OverlayManagerTemplate gOverlayTemplate_MimeJrTop;
extern const OverlayManagerTemplate gOverlayTemplate_WifiClubGameEntry;
extern const OverlayManagerTemplate gOverlayTemplate_WifiClub;
extern const OverlayManagerTemplate gOverlayTemplate_WobbuffetPop;

BOOL Task_WifiClub(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    WifiClubTask *task = TaskManager_GetEnvironment(taskManager);
    switch (task->state) {
    case 0:
        task->clubArgs->saveData = fieldSystem->saveData;
    case 1:
        task->state++;
        if (task->clubArgs->choice == 1 && Comm_HasWifiProfile(fieldSystem->saveData)) {
            task->state = 10;
            *task->result = 0;
        }
        break;
    case 2:
        CallApplicationAsTask(taskManager, &gOverlayTemplate_WifiClub, task->clubArgs);
        task->state++;
        break;
    case 3:
        if (Comm_HasWifiProfile(fieldSystem->saveData)) {
            SetFlag970(Save_VarsFlags_Get(fieldSystem->saveData));
        }
        switch (task->clubArgs->choice) {
        case WIFI_CLUB_BATTLE_SINGLE_FREE:
            task->maxLevel = 0;
            task->doubleBattle = FALSE;
            task->state = 4;
            break;
        case WIFI_CLUB_BATTLE_SINGLE_LV50:
            task->maxLevel = 50;
            task->doubleBattle = FALSE;
            task->state = 4;
            break;
        case WIFI_CLUB_BATTLE_SINGLE_LV100:
            task->maxLevel = 100;
            task->doubleBattle = FALSE;
            task->state = 4;
            break;
        case WIFI_CLUB_BATTLE_DOUBLE_FREE:
            task->maxLevel = 0;
            task->doubleBattle = TRUE;
            task->state = 4;
            break;
        case WIFI_CLUB_BATTLE_DOUBLE_LV50:
            task->maxLevel = 50;
            task->doubleBattle = TRUE;
            task->state = 4;
            break;
        case WIFI_CLUB_BATTLE_DOUBLE_LV100:
            task->maxLevel = 100;
            task->doubleBattle = TRUE;
            task->state = 4;
            break;
        case WIFI_CLUB_TRADE:
            task->state = 6;
            break;
        case WIFI_CLUB_CANCEL:
            *task->result = 1;
            task->state = 11;
            break;
        case WIFI_CLUB_EXIT:
            task->state = 9;
            break;
        case WIFI_CLUB_FRONTIER:
            task->state = 12;
            break;
        case WIFI_CLUB_SWALOT_PLOP:
            task->state = 14;
            break;
        case WIFI_CLUB_MIME_JR_TOP:
            task->state = 18;
            break;
        case WIFI_CLUB_WOBBUFFET_POP:
            task->state = 22;
            break;
        case WIFI_CLUB_WIFI_SETTINGS:
            task->state = 8;
            break;
        }
        break;
    case 4:
        CallTask_WifiClubBattle(taskManager, task->clubArgs->partnerFriendIndex, task->maxLevel, task->doubleBattle);
        task->state++;
        break;
    case 5:
        task->state = 2;
        break;
    case 6:
        CallTask_WirelessTrade(taskManager);
        task->state++;
        break;
    case 7:
        task->state = 2;
        break;
    case 8:
        Heap_Create(HEAP_ID_3, HEAP_ID_53, 0x40100);
        LoadOVY13();
        ov13_0221BA00(HEAP_ID_53);
        OS_ResetSystem(0);
        break;
    case 9:
    case 10:
    case 11:
        Heap_Free(task->clubArgs);
        Heap_Free(task);
        task->state++;
        return TRUE;
    case 12:
        sub_020378E4(0);
        task->launchedArgs = Frontier_LaunchApp(fieldSystem, NULL);
        task->state++;
        break;
    case 13:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            Heap_Free(task->launchedArgs);
            task->state = 2;
        }
        break;
    case 14:
        WifiClub_LaunchGameEntry(task, fieldSystem, HEAP_ID_FIELD2, WIFI_CLUB_GAME_SWALOT_PLOP);
        task->state++;
        break;
    case 15:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WifiClub_FinishGameEntry(task);
        }
        break;
    case 16:
        task->launchedArgs = WifiClub_LaunchSwalotPlop(fieldSystem, HEAP_ID_FIELD2, task->selection);
        task->state++;
        break;
    case 17:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = 2;
            WifiClub_FinishSwalotPlop(task->launchedArgs);
        }
        break;
    case 18:
        WifiClub_LaunchGameEntry(task, fieldSystem, HEAP_ID_FIELD2, WIFI_CLUB_GAME_MIME_JR_TOP);
        task->state++;
        break;
    case 19:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WifiClub_FinishGameEntry(task);
        }
        break;
    case 20:
        task->launchedArgs = WifiClub_LaunchMimeJrTop(fieldSystem, HEAP_ID_FIELD2, task->selection);
        task->state++;
        break;
    case 21:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = 2;
            WifiClub_FinishMimeJrTop(task->launchedArgs);
        }
        break;
    case 22:
        WifiClub_LaunchGameEntry(task, fieldSystem, HEAP_ID_FIELD2, WIFI_CLUB_GAME_WOBBUFFET_POP);
        task->state++;
        break;
    case 23:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = WifiClub_FinishGameEntry(task);
        }
        break;
    case 24:
        task->launchedArgs = WifiClub_LaunchWobbuffetPop(fieldSystem, HEAP_ID_FIELD2, task->selection);
        task->state++;
        break;
    case 25:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            task->state = 2;
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
    args->maxPlayers = gWifiClubGameMaxPlayers[game];
    args->result = 0;
    args->voiceChat = 0;
    args->saveData = fieldSystem->saveData;
    task->launchedArgs = args;
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_WifiClubGameEntry, args);
}

u32 WifiClub_FinishGameEntry(WifiClubTask *task) {
    WifiClubGameEntryArgs *args = task->launchedArgs;
    if (args->result == 1) {
        switch ((enum WifiClubGame)args->game) {
        case WIFI_CLUB_GAME_SWALOT_PLOP:
            task->state = 16;
            break;
        case WIFI_CLUB_GAME_MIME_JR_TOP:
            task->state = 20;
            break;
        default:
        case WIFI_CLUB_GAME_WOBBUFFET_POP:
            task->state = 24;
            break;
        }
    } else {
        task->state = 1;
    }
    task->selection = args->voiceChat;
    Heap_Free(task->launchedArgs);
    return task->state;
}

PlazaGameArgs *WifiClub_LaunchSwalotPlop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection) {
    PlazaGameArgs *args = Heap_Alloc(heapID, 0x3C);
    memset(args, 0, 0x3C);
    args->unk38 = selection;
    args->fromLobby = FALSE;
    args->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_SwalotPlop, args);
    return args;
}

void WifiClub_FinishSwalotPlop(PlazaGameArgs *args) {
    Heap_Free(args);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
}

PlazaGameArgs *WifiClub_LaunchMimeJrTop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection) {
    PlazaGameArgs *args = Heap_Alloc(heapID, sizeof(PlazaGameArgs));
    memset(args, 0, sizeof(PlazaGameArgs));
    args->unk3C = selection;
    *(u32 *)&args->unk38 = 0; // read as one word: launched from the lobby
    args->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_MimeJrTop, args);
    return args;
}

void WifiClub_FinishMimeJrTop(PlazaGameArgs *args) {
    Heap_Free(args);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_90));
}

PlazaGameArgs *WifiClub_LaunchWobbuffetPop(FieldSystem *fieldSystem, enum HeapID heapID, u32 selection) {
    PlazaGameArgs *args = Heap_Alloc(heapID, 0x3C);
    MI_CpuFill8(args, 0, 0x3C);
    args->unk38 = selection;
    args->fromLobby = FALSE;
    args->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_WobbuffetPop, args);
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
