#include "field_overworld_transition.h"

#include "global.h"

#include "field_overworld_restore.h"
#include "field_system.h"
#include "overlay_01.h"
#include "screen_fade.h"
#include "task.h"

// Field tasks that leave the overworld (unload the field map) or bring it back, with or without a screen fade.

enum FadeOutAndLeaveOverworldState {
    FADE_OUT_AND_LEAVE_FADE,
    FADE_OUT_AND_LEAVE_LEAVE,
    FADE_OUT_AND_LEAVE_DONE,
};

enum RestoreOverworldAndFadeInState {
    RESTORE_AND_FADE_IN_RESTORE,
    RESTORE_AND_FADE_IN_FADE,
    RESTORE_AND_FADE_IN_DONE,
};

static BOOL Task_LeaveOverworld(TaskManager *taskManager);
static BOOL Task_RestoreOverworld(TaskManager *taskManager);
static BOOL Task_WaitPaletteFade(TaskManager *taskManager);
static BOOL Task_FadeOutAndLeaveOverworld(TaskManager *taskManager);
static BOOL Task_RestoreOverworldAndFadeIn(TaskManager *taskManager);

static BOOL Task_LeaveOverworld(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    if (!FieldSystem_IsFieldMapLoaded(fieldSystem)) {
        return TRUE;
    }
    return FALSE;
}

void CallTask_LeaveOverworld(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    if (!FieldSystem_IsFieldMapLoaded(fieldSystem)) {
        GF_AssertFail();
        return;
    }
    FieldSystem_ClearRunningFieldMap(fieldSystem);
    TaskManager_Call(taskManager, Task_LeaveOverworld, NULL);
}

static BOOL Task_RestoreOverworld(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    if (FieldSystem_IsOverworldRestored(fieldSystem)) {
        return TRUE;
    }
    return FALSE;
}

void CallTask_RestoreOverworld(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    if (FieldSystem_IsFieldMapLoaded(fieldSystem)) {
        GF_AssertFail();
        return;
    }
    FieldSystem_LoadFieldOverlay(fieldSystem);
    TaskManager_Call(taskManager, Task_RestoreOverworld, NULL);
}

static BOOL Task_WaitPaletteFade(TaskManager *taskManager) {
    if (IsPaletteFadeFinished()) {
        return TRUE;
    }
    return FALSE;
}

// Fades both screens to black and waits for the fade.
void PaletteFadeUntilFinished(TaskManager *taskManager) {
    if (!FieldSystem_IsFieldMapLoaded(TaskManager_GetFieldSystem(taskManager))) {
        GF_AssertFail();
        return;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
    TaskManager_Call(taskManager, Task_WaitPaletteFade, NULL);
}

void CallTask_FadeFromBlack(TaskManager *taskManager) {
    if (!FieldSystem_IsFieldMapLoaded(TaskManager_GetFieldSystem(taskManager))) {
        GF_AssertFail();
        return;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
    TaskManager_Call(taskManager, Task_WaitPaletteFade, NULL);
}

static BOOL Task_FadeOutAndLeaveOverworld(TaskManager *taskManager) {
    u32 *state = TaskManager_GetStatePtr(taskManager);
    switch (*state) {
    case FADE_OUT_AND_LEAVE_FADE:
        PaletteFadeUntilFinished(taskManager);
        (*state)++;
        break;
    case FADE_OUT_AND_LEAVE_LEAVE:
        CallTask_LeaveOverworld(taskManager);
        (*state)++;
        break;
    case FADE_OUT_AND_LEAVE_DONE:
        return TRUE;
    }
    return FALSE;
}

void CallTask_FadeOutAndLeaveOverworld(TaskManager *taskManager) {
    TaskManager_Call(taskManager, Task_FadeOutAndLeaveOverworld, NULL);
}

static BOOL Task_RestoreOverworldAndFadeIn(TaskManager *taskManager) {
    u32 *state = TaskManager_GetStatePtr(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    switch (*state) {
    case RESTORE_AND_FADE_IN_RESTORE:
        CallTask_RestoreOverworld(taskManager);
        (*state)++;
        break;
    case RESTORE_AND_FADE_IN_FADE:
        FieldSystem_DrawMapNameAnimation(fieldSystem);
        CallTask_FadeFromBlack(taskManager);
        (*state)++;
        break;
    case RESTORE_AND_FADE_IN_DONE:
        return TRUE;
    }
    return FALSE;
}

void CallTask_RestoreOverworldAndFadeIn(TaskManager *taskManager) {
    TaskManager_Call(taskManager, Task_RestoreOverworldAndFadeIn, NULL);
}
