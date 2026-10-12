#include "field/bottom_screen_app.h"

#include "global.h"

#include "field/pal_pad_registration.h"

#include "assert.h"
#include "poke_overlay.h"
#include "screen_fade.h"
#include "sys_task_api.h"
#include "systask_environment.h"

FS_EXTERN_OVERLAY(OVY_27);
FS_EXTERN_OVERLAY(touch_save_app);
FS_EXTERN_OVERLAY(OVY_28);
FS_EXTERN_OVERLAY(OVY_29);
FS_EXTERN_OVERLAY(OVY_31);
FS_EXTERN_OVERLAY(OVY_32);
FS_EXTERN_OVERLAY(OVY_33);
FS_EXTERN_OVERLAY(OVY_34);

typedef void *(*BottomScreenAppInitFunc)(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
typedef void (*BottomScreenAppExitFunc)(BgConfig *bgConfig, void *app);
typedef BOOL (*BottomScreenAppIsExitDoneFunc)(BgConfig *bgConfig);

typedef struct BottomScreenAppTemplate {
    BottomScreenAppInitFunc init;
    BottomScreenAppExitFunc exit;
    BottomScreenAppIsExitDoneFunc isExitDone;
    u32 overlayId; // -1: the app is in a loaded overlay
} BottomScreenAppTemplate;

typedef struct BottomScreenAppManager {
    u8 appId;
    u8 state;
    u8 nextAppId;
    void *app;
    FieldSystem *fieldSystem;
    void *args;
} BottomScreenAppManager;

// The task data of the touchscreen choice menu (touchscreen_choice_menu.c).
typedef struct TouchscreenChoiceMenuView {
    u32 state;
    void *result;
    u8 unk_08[0x4];
    struct ScriptMenu *menu;
} TouchscreenChoiceMenuView;

// The apps' entry points, declared with the template's signatures.
void *TouchscreenMenu_Create(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void TouchscreenMenu_Destroy(BgConfig *bgConfig, void *app);
BOOL ov27_0225A2C8(BgConfig *bgConfig);
void *ov30_0225D520(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void ov30_0225D64C(BgConfig *bgConfig, void *app);
BOOL ov30_0225D6FC(BgConfig *bgConfig);
void *TouchMartApp_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void TouchMartApp_Exit(BgConfig *bgConfig, void *app);
BOOL TouchMartApp_IsExitDone(BgConfig *bgConfig);
void *TouchscreenChoiceMenu_Create(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void TouchscreenChoiceMenu_Destroy(BgConfig *bgConfig, void *app);
BOOL ov27_0225C418(BgConfig *bgConfig);
void *DowsingMchn_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void DowsingMchn_Exit(BgConfig *bgConfig, void *app);
BOOL DowsingMchn_IsExitDone(BgConfig *bgConfig);
void *NumberEntry_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void NumberEntry_Exit(BgConfig *bgConfig, void *app);
BOOL NumberEntry_IsExitDone(BgConfig *bgConfig);
void *FriendRosterApp_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void FriendRosterApp_Exit(BgConfig *bgConfig, void *app);
BOOL FriendRosterApp_IsExitDone(BgConfig *bgConfig);
void *CommClubButtons_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void CommClubButtons_Exit(BgConfig *bgConfig, void *app);
BOOL CommClubButtons_IsExitDone(BgConfig *bgConfig);
void *UnionChatLog_Init(FieldSystem *fieldSystem);
void UnionChatLog_Exit(void *data);

void TouchscreenChoiceMenu_SetCallback(void *app, void *callback, void *arg);
void TouchscreenMenu_RequestSelectedIconAnim(void *app);
void TouchscreenMenu_RequestRegisteredItemAnim(void *app, int item);
void ov35_02259D80(void *, int, int);

// Also declared by overlay_01.h, with other return types; this file keeps the types it was built with.
void FieldSystem_SwitchBottomScreenApp(FieldSystem *fieldSystem, int appId, void *args);
void FieldSystem_SetTouchscreenChoiceMenuState(FieldSystem *fieldSystem, int appId, int state, void *result);
u32 FieldSystem_GetTouchscreenChoiceMenuState(FieldSystem *fieldSystem);
u8 FieldSystem_GetBottomScreenAppId(FieldSystem *fieldSystem);
u8 FieldSystem_GetBottomScreenAppState(FieldSystem *fieldSystem);
struct ScriptMenu **FieldSystem_GetTouchscreenChoiceMenuList(FieldSystem *fieldSystem);
void FieldSystem_SetTouchscreenChoiceMenuCallback(FieldSystem *fieldSystem, void *callback, void *arg);
SysTask *BottomScreenApps_CreateTask(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem);
void BottomScreenApps_ExitApp(FieldSystem *fieldSystem);
BOOL BottomScreenApps_IsExitDone(FieldSystem *fieldSystem);
void BottomScreenApps_InitApp(BottomScreenAppManager *manager);
void BottomScreenApps_UnloadApp(BottomScreenAppManager *manager);
void Task_BottomScreenApps(SysTask *task, void *env);
void *UnionChatLogApp_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void UnionChatLogApp_Exit(BgConfig *bgConfig, void *app);
BOOL UnionChatLogApp_IsExitDone(BgConfig *bgConfig);
BOOL IsSpriteIdWithoutTalkAction(u16 spriteId);
BOOL IsScriptIdWithoutTalkAction(int scriptId);

// Facing one of these objects does not offer the touchscreen menu's talk action.
static const u16 sSpriteIdsWithoutTalkAction[] = {
    0x0196,
    0x015E,
    0x018C,
    0x0199,
    0x019A,
    0x019B,
    0x019C,
    0x019D,
    0x0193,
    0x0194,
    0x0054,
    0x0055,
    0x0056,
    0x0057,
    0x0106,
    0x017D,
    0x011F,
    0x017B,
    0x018D,
    0x018E,
    0x018F,
    0x0190,
    0x0191,
    0x0192,
    0x0120,
    0x00B7,
    0x0121,
    0x0122,
    0x0123,
    0x0124,
    0x015D,
    0x0178,
    0x00D2,
    0x017C,
    0x00EA,
    0x0106,
    0x00FB,
    0x00FC,
    0x00FD,
    0x00FE,
    0x00FF,
    0x0100,
    0x0101,
};

static const BottomScreenAppTemplate sBottomScreenApps[] = {
    [BOTTOM_SCREEN_APP_TOUCHSCREEN_MENU] = { TouchscreenMenu_Create, TouchscreenMenu_Destroy, ov27_0225A2C8, (u32)-1 },
    [BOTTOM_SCREEN_APP_TOUCH_SAVE] = { ov30_0225D520, ov30_0225D64C, ov30_0225D6FC, FS_OVERLAY_ID(touch_save_app) },
    [BOTTOM_SCREEN_APP_TOUCH_MART] = { TouchMartApp_Init, TouchMartApp_Exit, TouchMartApp_IsExitDone, FS_OVERLAY_ID(OVY_31) },
    [BOTTOM_SCREEN_APP_CHOICE_MENU] = { TouchscreenChoiceMenu_Create, TouchscreenChoiceMenu_Destroy, ov27_0225C418, (u32)-1 },
    [BOTTOM_SCREEN_APP_DOWSING_MCHN] = { DowsingMchn_Init, DowsingMchn_Exit, DowsingMchn_IsExitDone, FS_OVERLAY_ID(OVY_28) },
    [BOTTOM_SCREEN_APP_NUMBER_ENTRY] = { NumberEntry_Init, NumberEntry_Exit, NumberEntry_IsExitDone, FS_OVERLAY_ID(OVY_29) },
    [BOTTOM_SCREEN_APP_FRIEND_ROSTER] = { FriendRosterApp_Init, FriendRosterApp_Exit, FriendRosterApp_IsExitDone, FS_OVERLAY_ID(OVY_32) },
    [BOTTOM_SCREEN_APP_COMM_CLUB_BUTTONS] = { CommClubButtons_Init, CommClubButtons_Exit, CommClubButtons_IsExitDone, FS_OVERLAY_ID(OVY_33) },
    [BOTTOM_SCREEN_APP_UNION_CHAT_LOG] = { UnionChatLogApp_Init, UnionChatLogApp_Exit, UnionChatLogApp_IsExitDone, FS_OVERLAY_ID(OVY_34) },
};

void ov01_021F6830(FieldSystem *fieldSystem, int a1, int a2) {
    void *state = (void *)fieldSystem->unk4->unk14;
    if (state != NULL) {
        ov35_02259D80(state, a1, a2);
    }
}

void BottomScreen_TouchSave_Init(FieldSystem *fieldSystem) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_27), OVY_LOAD_ASYNC);
    fieldSystem->touchSaveTask = ov30_0225D520(fieldSystem->bgConfig, &fieldSystem->lastTouchMenuInput, fieldSystem, NULL);
}

void BottomScreen_TouchSave_End(FieldSystem *fieldSystem) {
    ov30_0225D64C(fieldSystem->bgConfig, fieldSystem->touchSaveTask);
}

BOOL BottomScreen_TouchSave_IsDone(FieldSystem *fieldSystem) {
    if (ov30_0225D6FC(fieldSystem->bgConfig)) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_27));
        return TRUE;
    }
    return FALSE;
}

void BottomScreen_Apps_Init(FieldSystem *fieldSystem) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_27), OVY_LOAD_ASYNC);
    fieldSystem->bottomScreenTask = BottomScreenApps_CreateTask(fieldSystem->bgConfig, &fieldSystem->lastTouchMenuInput, fieldSystem);
}

void BottomScreen_Apps_End(FieldSystem *fieldSystem) {
    BottomScreenApps_ExitApp(fieldSystem);
}

BOOL BottomScreen_Apps_IsDone(FieldSystem *fieldSystem) {
    if (BottomScreenApps_IsExitDone(fieldSystem)) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_27));
        return TRUE;
    }
    return FALSE;
}

SysTask *BottomScreenApps_CreateTask(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem) {
    SysTask *task;
    BottomScreenAppManager *manager;

    task = CreateSysTaskAndEnvironment(Task_BottomScreenApps, sizeof(BottomScreenAppManager), 10, HEAP_ID_FIELD1);
    manager = SysTask_GetData(task);
    manager->appId = fieldSystem->unk1C;
    manager->state = BOTTOM_SCREEN_APP_STATE_INIT;
    manager->app = NULL;
    manager->nextAppId = 0;
    manager->fieldSystem = fieldSystem;
    manager->args = NULL;
    return task;
}

void BottomScreenApps_ExitApp(FieldSystem *fieldSystem) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    sBottomScreenApps[manager->appId].exit(fieldSystem->bgConfig, manager->app);
}

BOOL BottomScreenApps_IsExitDone(FieldSystem *fieldSystem) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    if (sBottomScreenApps[manager->appId].isExitDone(fieldSystem->bgConfig) == TRUE) {
        BottomScreenApps_UnloadApp(manager);
        DestroySysTaskAndEnvironment(fieldSystem->bottomScreenTask);
        return TRUE;
    }
    return FALSE;
}

void BottomScreenApps_InitApp(BottomScreenAppManager *manager) {
    if (sBottomScreenApps[manager->appId].overlayId != (u32)-1) {
        HandleLoadOverlay(sBottomScreenApps[manager->appId].overlayId, OVY_LOAD_ASYNC);
    }
    manager->app = sBottomScreenApps[manager->appId].init(manager->fieldSystem->bgConfig, &manager->fieldSystem->lastTouchMenuInput, manager->fieldSystem, manager->args);
}

void BottomScreenApps_UnloadApp(BottomScreenAppManager *manager) {
    if (sBottomScreenApps[manager->appId].overlayId != (u32)-1) {
        UnloadOverlayByID(sBottomScreenApps[manager->appId].overlayId);
    }
}

void Task_BottomScreenApps(SysTask *task, void *env) {
    BottomScreenAppManager *manager = env;
    switch (manager->state) {
    case BOTTOM_SCREEN_APP_STATE_INIT:
        BottomScreenApps_InitApp(manager);
        manager->state = BOTTOM_SCREEN_APP_STATE_RUNNING;
        break;
    case BOTTOM_SCREEN_APP_STATE_SWITCH_REQUESTED:
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 2, 1, HEAP_ID_FIELD1);
        manager->state = BOTTOM_SCREEN_APP_STATE_FADE_OUT;
        break;
    case BOTTOM_SCREEN_APP_STATE_FADE_OUT:
        if (IsPaletteFadeFinished()) {
            manager->state = BOTTOM_SCREEN_APP_STATE_EXIT;
        }
        break;
    case BOTTOM_SCREEN_APP_STATE_EXIT:
        sBottomScreenApps[manager->appId].exit(manager->fieldSystem->bgConfig, manager->app);
        manager->state = BOTTOM_SCREEN_APP_STATE_WAIT_EXIT;
        break;
    case BOTTOM_SCREEN_APP_STATE_WAIT_EXIT:
        if (sBottomScreenApps[manager->appId].isExitDone(manager->fieldSystem->bgConfig) == TRUE) {
            BottomScreenApps_UnloadApp(manager);
            manager->appId = manager->nextAppId;
            manager->state = BOTTOM_SCREEN_APP_STATE_START_NEXT;
        }
        break;
    case BOTTOM_SCREEN_APP_STATE_START_NEXT:
        BottomScreenApps_InitApp(manager);
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 2, 1, HEAP_ID_FIELD1);
        manager->state = BOTTOM_SCREEN_APP_STATE_FADE_IN;
        break;
    case BOTTOM_SCREEN_APP_STATE_FADE_IN:
        if (IsPaletteFadeFinished()) {
            manager->state = BOTTOM_SCREEN_APP_STATE_RUNNING;
        }
        break;
    }
}

// Ignored unless the current app is running; args is passed to the next app's init.
void FieldSystem_SwitchBottomScreenApp(FieldSystem *fieldSystem, int appId, void *args) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    if (manager->state == BOTTOM_SCREEN_APP_STATE_RUNNING) {
        manager->state = BOTTOM_SCREEN_APP_STATE_SWITCH_REQUESTED;
        manager->nextAppId = appId;
        manager->args = args;
    }
}

// The choice menu app: state 3 shows YES/NO, 7 the list; the answer is written to *result.
void FieldSystem_SetTouchscreenChoiceMenuState(FieldSystem *fieldSystem, int appId, int state, void *result) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    TouchscreenChoiceMenuView *menu = SysTask_GetData(manager->app);
    if (appId != FieldSystem_GetBottomScreenAppId(fieldSystem)) {
        GF_AssertFail();
    }
    menu->state = state;
    menu->result = result;
}

u32 FieldSystem_GetTouchscreenChoiceMenuState(FieldSystem *fieldSystem) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    TouchscreenChoiceMenuView *menu = SysTask_GetData(manager->app);
    return menu->state;
}

u8 FieldSystem_GetBottomScreenAppId(FieldSystem *fieldSystem) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    return manager->appId;
}

u8 FieldSystem_GetBottomScreenAppState(FieldSystem *fieldSystem) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    return manager->state;
}

struct ScriptMenu **FieldSystem_GetTouchscreenChoiceMenuList(FieldSystem *fieldSystem) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    TouchscreenChoiceMenuView *menu = SysTask_GetData(manager->app);
    return &menu->menu;
}

void FieldSystem_SetTouchscreenChoiceMenuCallback(FieldSystem *fieldSystem, void *callback, void *arg) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    TouchscreenChoiceMenu_SetCallback(manager->app, callback, arg);
}

void FieldSystem_RequestTouchscreenMenuSelectedIconAnim(FieldSystem *fieldSystem) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    TouchscreenMenu_RequestSelectedIconAnim(manager->app);
}

void FieldSystem_RequestTouchscreenMenuRegisteredItemAnim(FieldSystem *fieldSystem, int item) {
    BottomScreenAppManager *manager = SysTask_GetData(fieldSystem->bottomScreenTask);
    if (item <= 0 || item >= 3) {
        GF_AssertFail();
    }
    TouchscreenMenu_RequestRegisteredItemAnim(manager->app, item);
}

// The chat log is kept in the view photo slot (0xDC), which the Union Room never uses for a photo.
void *UnionChatLogApp_Init(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args) {
    fieldSystem->viewPhotoTask = UnionChatLog_Init(fieldSystem);
    return fieldSystem->viewPhotoTask;
}

void UnionChatLogApp_Exit(BgConfig *bgConfig, void *app) {
    UnionChatLog_Exit(app);
}

BOOL UnionChatLogApp_IsExitDone(BgConfig *bgConfig) {
    return TRUE;
}

BOOL IsSpriteIdWithoutTalkAction(u16 spriteId) {
    u32 i;
    const u16 *p = sSpriteIdsWithoutTalkAction;
    for (i = 0; i < NELEMS(sSpriteIdsWithoutTalkAction); i++, p++) {
        if (spriteId == *p) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL IsScriptIdWithoutTalkAction(int scriptId) {
    if (scriptId >= 7000 && scriptId <= 8799) {
        return TRUE;
    }
    if (scriptId >= 2800 && scriptId <= 2999) {
        return TRUE;
    }
    if (scriptId >= 10000 && scriptId <= 10099) {
        return TRUE;
    }
    if (scriptId >= 10100 && scriptId <= 10149) {
        return TRUE;
    }
    return FALSE;
}

// Touch helpers of the Pal Pad registration task (pal_pad_registration.c).
BOOL PalPadRegistration_IsFriendRosterAppRunning(PalPadRegistration *registration) {
    u8 appId = FieldSystem_GetBottomScreenAppId(registration->fieldSystem);
    u8 state = FieldSystem_GetBottomScreenAppState(registration->fieldSystem);
    if (appId == BOTTOM_SCREEN_APP_FRIEND_ROSTER && state == BOTTOM_SCREEN_APP_STATE_RUNNING) {
        return TRUE;
    }
    return FALSE;
}

BOOL PalPadRegistration_TryShowTouchYesNo(PalPadRegistration *registration) {
    u8 appId = FieldSystem_GetBottomScreenAppId(registration->fieldSystem);
    u8 state = FieldSystem_GetBottomScreenAppState(registration->fieldSystem);
    if (appId == BOTTOM_SCREEN_APP_CHOICE_MENU && state == BOTTOM_SCREEN_APP_STATE_RUNNING) {
        // 3: show the YES/NO buttons
        FieldSystem_SetTouchscreenChoiceMenuState(registration->fieldSystem, BOTTOM_SCREEN_APP_CHOICE_MENU, 3, &registration->yesNoAnswer);
        return TRUE;
    }
    return FALSE;
}

BOOL PalPadRegistration_IsTouchYesNoDone(PalPadRegistration *registration) {
    u8 appId = FieldSystem_GetBottomScreenAppId(registration->fieldSystem);
    u32 state = FieldSystem_GetTouchscreenChoiceMenuState(registration->fieldSystem);
    if (appId == BOTTOM_SCREEN_APP_CHOICE_MENU && state == 6) { // 6: answered
        return TRUE;
    }
    return FALSE;
}

// Returns FALSE once the player answered (registration->yesNoAnswer).
BOOL PalPadRegistration_RunTouchYesNo(PalPadRegistration *registration) {
    switch (registration->yesNoState) {
    case 0:
        FieldSystem_SwitchBottomScreenApp(registration->fieldSystem, BOTTOM_SCREEN_APP_CHOICE_MENU, NULL);
        registration->yesNoState++;
        break;
    case 1:
        if (PalPadRegistration_TryShowTouchYesNo(registration) == TRUE) {
            registration->yesNoState++;
        }
        break;
    case 2:
        if (PalPadRegistration_IsTouchYesNoDone(registration) == TRUE) {
            registration->yesNoState = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}
