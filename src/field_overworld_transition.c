#include "field_overworld_transition.h"

#include "global.h"

#include "field_system.h"
#include "overlay_01.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_020552A4.h"

// Field tasks that leave the overworld (unload the field map) or bring it back, with or without a screen fade.

BOOL Task_LeaveOverworld(TaskManager *taskManager);
BOOL Task_RestoreOverworld(TaskManager *taskManager);
BOOL Task_WaitPaletteFade(TaskManager *taskManager);
BOOL Task_FadeOutAndLeaveOverworld(TaskManager *taskManager);
BOOL Task_RestoreOverworldAndFadeIn(TaskManager *taskManager);

BOOL Task_LeaveOverworld(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    if (!sub_0203DF7C(fieldSystem)) {
        return TRUE;
    }
    return FALSE;
}

void CallTask_LeaveOverworld(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    if (!sub_0203DF7C(fieldSystem)) {
        GF_AssertFail();
        return;
    }
    sub_0203DF34(fieldSystem);
    TaskManager_Call(taskManager, Task_LeaveOverworld, NULL);
}

BOOL Task_RestoreOverworld(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    if (sub_020505C8(fieldSystem)) {
        return TRUE;
    }
    return FALSE;
}

void CallTask_RestoreOverworld(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    if (sub_0203DF7C(fieldSystem)) {
        GF_AssertFail();
        return;
    }
    FieldSystem_LoadFieldOverlay(fieldSystem);
    TaskManager_Call(taskManager, Task_RestoreOverworld, NULL);
}

BOOL Task_WaitPaletteFade(TaskManager *taskManager) {
    if (IsPaletteFadeFinished()) {
        return TRUE;
    }
    return FALSE;
}

// Fades both screens to black and waits for the fade.
void PaletteFadeUntilFinished(TaskManager *taskManager) {
    if (!sub_0203DF7C(TaskManager_GetFieldSystem(taskManager))) {
        GF_AssertFail();
        return;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
    TaskManager_Call(taskManager, Task_WaitPaletteFade, NULL);
}

void CallTask_FadeFromBlack(TaskManager *taskManager) {
    if (!sub_0203DF7C(TaskManager_GetFieldSystem(taskManager))) {
        GF_AssertFail();
        return;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
    TaskManager_Call(taskManager, Task_WaitPaletteFade, NULL);
}

BOOL Task_FadeOutAndLeaveOverworld(TaskManager *taskManager) {
    u32 *state = TaskManager_GetStatePtr(taskManager);
    switch (*state) {
    case 0:
        PaletteFadeUntilFinished(taskManager);
        (*state)++;
        break;
    case 1:
        CallTask_LeaveOverworld(taskManager);
        (*state)++;
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

void CallTask_FadeOutAndLeaveOverworld(TaskManager *taskManager) {
    TaskManager_Call(taskManager, Task_FadeOutAndLeaveOverworld, NULL);
}

BOOL Task_RestoreOverworldAndFadeIn(TaskManager *taskManager) {
    u32 *state = TaskManager_GetStatePtr(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    switch (*state) {
    case 0:
        CallTask_RestoreOverworld(taskManager);
        (*state)++;
        break;
    case 1:
        FieldSystem_DrawMapNameAnimation(fieldSystem);
        CallTask_FadeFromBlack(taskManager);
        (*state)++;
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

void CallTask_RestoreOverworldAndFadeIn(TaskManager *taskManager) {
    TaskManager_Call(taskManager, Task_RestoreOverworldAndFadeIn, NULL);
}
