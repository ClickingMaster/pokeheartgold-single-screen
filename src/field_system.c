#include "field_system.h"

#include "constants/field/map_load.h"
#include "constants/maps.h"
#include "constants/scrcmd.h"
#include "constants/sndseq.h"

#include "field/field_control.h"
#include "field/signpost.h"

#include "field_warp_tasks.h"
#include "main.h"
#include "map_events.h"
#include "math_util.h"
#include "overlay_01_021F1AFC.h"
#include "field/bottom_screen_app.h"
#include "overlay_124.h"
#include "overlay_35.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "unk_02035900.h"
#include "comm_player_manager.h"
#include "player_avatar_movement.h"
#include "unk_02092BB8.h"
#include "gear_phone_ring_manager.h"

FS_EXTERN_OVERLAY(intro_title);
FS_EXTERN_OVERLAY(OVY_124);
FS_EXTERN_OVERLAY(field);

const OverlayManagerTemplate gApplication_NewGameFieldsys = {
    .init = Field_NewGame_AppInit,
    .exec = Field_AppExec,
    .exit = Field_AppExit,
    .ovy_id = FS_OVERLAY_ID_NONE
};

const OverlayManagerTemplate gApplication_ContinueFieldsys = {
    .init = Field_Continue_AppInit,
    .exec = Field_AppExec,
    .exit = Field_AppExit,
    .ovy_id = FS_OVERLAY_ID_NONE
};

static FieldSystem *sFieldSysPtr;

typedef struct UnkFieldSystemInit {
    int unk0;
    int unk4;
} FieldSystemInitWork;

static BOOL FieldSystem_Main(FieldSystem *fieldSystem);

BOOL Field_Continue_AppInit(OverlayManager *man, int *unused) {
    FieldSystemInitWork *args = OverlayManager_GetArgs(man);
    sFieldSysPtr = FieldSystem_New(man);

    if (args->unk4) {
        CallFieldTask_ContinueGame_CommError(sFieldSysPtr);
    } else {
        CallFieldTask_ContinueGame_Normal(sFieldSysPtr);
    }

    args->unk4 = 0;

    return TRUE;
}

BOOL Field_NewGame_AppInit(OverlayManager *man, int *unused) {
    sFieldSysPtr = FieldSystem_New(man);
    CallFieldTask_NewGame(sFieldSysPtr);
    return TRUE;
}

BOOL Field_AppExec(OverlayManager *man, int *unused) {
    if (FieldSystem_Main(OverlayManager_GetData(man))) {
        return TRUE;
    }
    return FALSE;
}

extern OverlayManagerTemplate gApplication_TitleScreen;

BOOL Field_AppExit(OverlayManager *man, int *unused) {
    FieldSystem_Delete(man);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    return TRUE;
}

extern OverlayManagerTemplate ov01_02206378;

void FieldSystem_LoadFieldOverlayInternal(FieldSystem *fieldSystem) {
    GF_ASSERT(fieldSystem->processManager->child == NULL);
    GF_ASSERT(fieldSystem->processManager->parent == NULL);

    HandleLoadOverlay(FS_OVERLAY_ID(field), OVY_LOAD_ASYNC);

    fieldSystem->runningFieldMap = FALSE;
    fieldSystem->processManager->isPaused = FALSE;
    fieldSystem->processManager->parent = OverlayManager_New(&ov01_02206378, fieldSystem, HEAP_ID_FIELD2);
}

void FieldSystem_ClearRunningFieldMap(FieldSystem *fieldSystem) {
    fieldSystem->runningFieldMap = FALSE;
}

u8 FieldSystem_GetStartMenuInputState(FieldSystem *fieldSystem) {
    GF_ASSERT(fieldSystem->startMenuInputState == 0 || fieldSystem->startMenuInputState == 1);

    return fieldSystem->startMenuInputState;
}

void FieldSystem_SetStartMenuInputState(FieldSystem *fieldSystem, int a1) {
    GF_ASSERT(a1 == 0 || a1 == 1);
    fieldSystem->startMenuInputState = a1;
}

BOOL FieldSystem_IsFieldMapLoaded(FieldSystem *fieldSystem) {
    return fieldSystem->processManager->parent != NULL;
}

BOOL FieldSystem_IsFieldMapRunning(FieldSystem *fieldSystem) {
    return fieldSystem->processManager->parent != NULL && fieldSystem->runningFieldMap;
}

BOOL FieldSystem_IsChildAppRunning(FieldSystem *fieldSystem) {
    return fieldSystem->processManager->child != NULL;
}

void FieldSystem_LaunchApplication(FieldSystem *fieldSystem, const OverlayManagerTemplate *template, void *parentWork) {
    GF_ASSERT(fieldSystem->processManager->child == NULL);

    FieldSystem_ClearRunningFieldMap(fieldSystem);

    fieldSystem->processManager->child = OverlayManager_New(template, parentWork, HEAP_ID_FIELD2);
}

FieldSystem *FieldSystem_New(OverlayManager *man) {
    Heap_Create(HEAP_ID_3, HEAP_ID_FIELD2, 0x1C000);
    Heap_Create(HEAP_ID_3, HEAP_ID_FIELD3, 0x4000);
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570);
    FieldSystem *fieldSystem = OverlayManager_CreateAndGetData(man, sizeof(FieldSystem), HEAP_ID_FIELD2);
    MI_CpuFill8(fieldSystem, 0, sizeof(FieldSystem));
    fieldSystem->processManager = Heap_Alloc(HEAP_ID_FIELD2, sizeof(struct FieldProcessManager));

    fieldSystem->processManager->parent = NULL;
    fieldSystem->processManager->child = NULL;
    fieldSystem->processManager->isPaused = FALSE;
    fieldSystem->processManager->isDone = FALSE;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_124), OVY_LOAD_ASYNC);

    FieldSystem_Init(man, fieldSystem);

    UnloadOverlayByID(FS_OVERLAY_ID(OVY_124));

    return fieldSystem;
}

void FieldSystem_Delete(OverlayManager *man) {
    FieldSystem *fieldSystem = OverlayManager_GetData(man);
    MapMatrix_Free(fieldSystem->mapMatrix);
    Field_FreeMapEvents(fieldSystem);
    Heap_Free(fieldSystem->bagCursor);
    UnkStruct_02092BB8_Free(fieldSystem->unkA8);
    GearPhoneRingManager_Delete(fieldSystem->phoneRingManager);
    Heap_Free(fieldSystem->processManager);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_89);
    Heap_Destroy(HEAP_ID_FIELD2);
    Heap_Destroy(HEAP_ID_FIELD3);
}

static void ppOverlayManager_RunFrame_DeleteIfFinished(OverlayManager **man) {
    if (*man && OverlayManager_Run(*man)) {
        OverlayManager_Delete(*man);
        *man = NULL;
    }
}

static BOOL FieldSystem_Main(FieldSystem *fieldSystem) {
    FieldSystem_Control(fieldSystem);
    if (FieldSystem_RunTaskFrame(fieldSystem) == TRUE) {
        if (fieldSystem->unk4) {
            ov01_021F6830(fieldSystem, 0, 0);
        }
    }
    if (fieldSystem->processManager->parent) {
        ppOverlayManager_RunFrame_DeleteIfFinished(&fieldSystem->processManager->parent);
        if (!fieldSystem->processManager->parent) {
            UnloadOverlayByID(FS_OVERLAY_ID(field));
        }
    } else if (fieldSystem->processManager->child) {
        ppOverlayManager_RunFrame_DeleteIfFinished(&fieldSystem->processManager->child);
    }
    if (fieldSystem->processManager->isDone && !fieldSystem->taskman && !fieldSystem->processManager->parent && !fieldSystem->processManager->child) {
        return TRUE;
    }
    return FALSE;
}

BOOL FieldSystem_IsPlayerMovementAllowed(FieldSystem *fieldSystem) {
    return !fieldSystem->processManager->isPaused && fieldSystem->runningFieldMap && !FieldSystem_TaskIsRunning(fieldSystem);
}

void FieldSystem_Control(FieldSystem *fieldSystem) {
    FieldInput fieldInput;

    BOOL movementAllowed = FieldSystem_IsPlayerMovementAllowed(fieldSystem);

    if (movementAllowed) {
        PlayerAvatar_UpdateMovement(fieldSystem->playerAvatar);
        FieldInput_Update(&fieldInput, fieldSystem, gSystem.newKeys, gSystem.heldKeys);
    }

    int loadType = fieldSystem->mapLoadType;

    if (fieldSystem->location->mapId == MAP_BATTLE_TOWER) {
        loadType = 0;
    }

    switch (loadType) {
    case 3:
        if (movementAllowed) {
            if (CommPlayerManager_IsLocalPlayerConnected()) {
                if (FieldInput_Process_Colosseum(&fieldInput, fieldSystem) == TRUE) {
                    movementAllowed = FALSE;
                }
            } else {
                movementAllowed = FALSE;
            }
        }
        sub_020573F0(fieldSystem, movementAllowed);
        break;
    case 2:
        if (movementAllowed && FieldInput_Process_UnionRoom(&fieldInput, fieldSystem) != TRUE) {
            PlayerAvatar_MoveControl(fieldSystem->playerAvatar, fieldSystem->mapLoadManager, -1, fieldInput.newKeys, fieldInput.heldKeys, 0);
        }
        break;
    case 4:
        if (movementAllowed) {
            if (FieldInput_Process_BattleTower(&fieldInput, fieldSystem) == TRUE) {
                FieldDrawMapName_Reset(fieldSystem->unk4->drawMapNameInfo);
                FieldSystem_SetAndExecuteSignpostWindowCommand(fieldSystem, MAPSIGNCOMMAND_HIDE);
                Field_PlayerAvatar_TryLeavePoketchPose(fieldSystem->playerAvatar);
                ov01_021F6830(fieldSystem, 0, 1);
                break;
            }
            if (gSystem.newKeys & PAD_BUTTON_A) {
                FieldDrawMapName_Reset(fieldSystem->unk4->drawMapNameInfo);
            }
            movementAllowed = 0;
            if (sub_0203E324()) {
                movementAllowed = ov35_02259DB8();
            }
            PlayerAvatar_MoveControl(fieldSystem->playerAvatar, fieldSystem->mapLoadManager, -1, fieldInput.newKeys, fieldInput.heldKeys, movementAllowed);
        }
        break;
    default:
        if (movementAllowed) {
            u32 newEvent = FieldInput_Process(&fieldInput, fieldSystem);
            if (newEvent) {
                FieldDrawMapName_Reset(fieldSystem->unk4->drawMapNameInfo);
                FieldSystem_SetAndExecuteSignpostWindowCommand(fieldSystem, MAPSIGNCOMMAND_HIDE);
                PlayerAvatar_ResetMovement(fieldSystem->playerAvatar);
                Field_PlayerAvatar_TryLeavePoketchPose(fieldSystem->playerAvatar);
                ov01_021F6830(fieldSystem, 0, 1);
                if (newEvent != 2) {
                    GearPhoneRingManager_ResetIfActive(fieldSystem->phoneRingManager);
                }
            } else {
                if (gSystem.newKeys & PAD_BUTTON_A) {
                    FieldDrawMapName_Reset(fieldSystem->unk4->drawMapNameInfo);
                }
                u32 flag = 0;
                if (sub_0203E324()) {
                    flag = ov35_02259DB8();
                }
                PlayerAvatar_MoveControl(fieldSystem->playerAvatar, fieldSystem->mapLoadManager, -1, fieldInput.newKeys, fieldInput.heldKeys, flag);
            }
        }
        break;
    }
}

void FieldSystem_PauseProcesses() {
    sFieldSysPtr->processManager->isPaused = TRUE;
    sub_02037504();
}

void FieldSystem_ResumeProcesses() {
    sFieldSysPtr->processManager->isPaused = FALSE;
    sub_020374E4();
}

int sub_0203E324() {
    if (sFieldSysPtr->unk4 == NULL) {
        return 0;
    }
    return sFieldSysPtr->unk4->unk14;
}

void FieldSystem_SetSavedBottomScreenAppId(FieldSystem *fieldSystem, int a1) {
    fieldSystem->savedBottomScreenAppId = a1;
}

BgConfig *FieldSystem_GetBgConfigPtr(FieldSystem *fieldSystem) {
    return fieldSystem->bgConfig;
}

SaveData *FieldSystem_GetSaveData(FieldSystem *fieldSystem) {
    return fieldSystem->saveData;
}

void Task_AntipiracyRandom() {
    LCRandom();
    LCRandom();
}

void Field_SetEnvironmentSoundState_None_Unk2() {
    sFieldSysPtr->environmentSoundState = ENVIRONMENT_SOUND_NONE_UNK2;
}
