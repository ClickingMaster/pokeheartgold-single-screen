#include "global.h"

#include "comm_player_manager_internal.h"
#include "sys_task_api.h"
#include "communication_core.h"

// Player positions in the link lobby, 2 and 4 players; CommPlayerManager_AssignBattleSlots (comm_player_sync.c) matches the players to them.
const u16 __attribute__((aligned(4))) _020FC7A0[4] = { 4, 7, 11, 7 };
const u16 _020FC7A8[8] = { 4, 6, 11, 6, 4, 8, 11, 8 };
CommPlayerManager *gCommPlayerManager;

BOOL CommPlayerManager_Init(CommPlayerManager *manager, FieldSystem *fieldSystem, int unused) {
    int i;

    if (gCommPlayerManager != NULL) {
        return FALSE;
    }
    if (gCommPlayerManager == NULL) {
        gCommPlayerManager = manager;
        MI_CpuFill8(manager, 0, sizeof(CommPlayerManager));
    }
    gCommPlayerManager->fieldSystem = fieldSystem;
    for (i = 0; i < 8; i++) {
        gCommPlayerManager->positions[i].facing = -1;
        gCommPlayerManager->positions[i].x = 0xFFFF;
        gCommPlayerManager->positions[i].z = 0xFFFF;
        gCommPlayerManager->positions[i].speed = 2;
        gCommPlayerManager->hostPositions[i].facing = -1;
        gCommPlayerManager->hostPositions[i].x = 0xFFFF;
        gCommPlayerManager->hostPositions[i].z = 0xFFFF;
        gCommPlayerManager->hostPositions[i].speed = 2;
        gCommPlayerManager->moveEnabled[i] = 0;
        gCommPlayerManager->inputLocked[i] = 0;
    }
    gCommPlayerManager->localMoveEnabled = 0;
    gCommPlayerManager->fieldResumed = 0;
    gCommPlayerManager->task = SysTask_CreateOnMainQueue(CommPlayerManager_Task, gCommPlayerManager->fieldSystem, 200);
    Comm_EnableKeySync();
    CommPlayerManager_InitLocalPlayer();
    return TRUE;
}
