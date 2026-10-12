#include "constants/flypoints.h"
#include "constants/trainer_class.h"

#include "assert.h"
#include "field_bgm.h"
#include "global_terminal_app_internal.h"
#include "heap.h"
#include "orbit_cursor.h"
#include "poke_overlay.h"
#include "save_misc_data.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sys_flags.h"
#include "system.h"
#include "vs_recorder_app.h"
#include "vs_recorder_app_internal.h"

FS_EXTERN_OVERLAY(OVY_40);

const OverlayManagerTemplate gOverlayTemplate_VsRecorderApp = { VsRecorderApp_InitMode0, VsRecorderApp_Main, VsRecorderApp_Exit, FS_OVERLAY_ID_NONE };
const OverlayManagerTemplate gOverlayTemplate_VsRecorderAppMode1 = { VsRecorderApp_InitMode1, VsRecorderApp_Main, VsRecorderApp_Exit, FS_OVERLAY_ID_NONE };
const OverlayManagerTemplate gOverlayTemplate_VsRecorderAppMode2 = { VsRecorderApp_InitMode2, VsRecorderApp_Main, VsRecorderApp_Exit, FS_OVERLAY_ID_NONE };
const OverlayManagerTemplate gOverlayTemplate_VsRecorderAppMode3 = { VsRecorderApp_InitMode3, VsRecorderApp_Main, VsRecorderApp_Exit, FS_OVERLAY_ID_NONE };
const OverlayManagerTemplate gOverlayTemplate_VsRecorderAppMode4 = { VsRecorderApp_InitMode4, VsRecorderApp_Main, VsRecorderApp_Exit, FS_OVERLAY_ID_NONE };
const OverlayManagerTemplate gOverlayTemplate_VsRecorderAppMode5 = { VsRecorderApp_InitMode5, VsRecorderApp_Main, VsRecorderApp_Exit, FS_OVERLAY_ID_NONE };
const OverlayManagerTemplate gOverlayTemplate_VsRecorderAppMode6 = { VsRecorderApp_InitMode6, VsRecorderApp_Main, VsRecorderApp_Exit, FS_OVERLAY_ID_NONE };

const FrontierBrainClasses gFrontierBrainTrainerClasses = {
    { TRAINERCLASS_TOWER_TYCOON, TRAINERCLASS_HALL_MATRON, TRAINERCLASS_FACTORY_HEAD, TRAINERCLASS_ARCADE_STAR, TRAINERCLASS_CASTLE_VALET }
};

const OverlayManagerTemplate *gVsRecorderAppTemplates[] = {
    &gOverlayTemplate_VsRecorderApp,
    &gOverlayTemplate_VsRecorderAppMode1,
    &gOverlayTemplate_VsRecorderAppMode2,
    &gOverlayTemplate_VsRecorderAppMode3,
    &gOverlayTemplate_VsRecorderAppMode4,
    &gOverlayTemplate_VsRecorderAppMode5,
    &gOverlayTemplate_VsRecorderAppMode6,
};

const OverlayManagerTemplate *VsRecorderApp_GetTemplate(u8 mode) {
    return gVsRecorderAppTemplates[mode];
}

BOOL sub_02087E1C(void *work) {
    VsRecorderWork *recorder = work;
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(recorder->saveData);
    return Save_VarsFlags_FlypointFlagAction(varsFlags, 2, FLYPOINT_UNION_CAVE);
}

BOOL VsRecorderApp_IsFrontierBrainClass(u32 trainerClass) {
    FrontierBrainClasses classes = gFrontierBrainTrainerClasses;
    u32 i;
    for (i = 0; i < 5; i++) {
        if (trainerClass == classes.trainerClass[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

int VsRecorderMenu_InitCommon(OverlayManager *manager, int *state, BOOL fromTerminal) {
    VsRecorderWork *recorder;

    StopAllSE(0);
    Heap_Create(HEAP_ID_3, HEAP_ID_109, 0x55000);
    recorder = VsRecorderApp_GetRecorderWork(manager);
    VsRecorder_MenuPop(recorder);
    GF_SndHandleSetPlayerVolume(1, 0x2A);
    GF_SndHandleSetPlayerVolume(7, 0x2A);
    if (fromTerminal != FALSE) {
        sub_02055198(NULL, 0x482);
    }
    if (fromTerminal == FALSE) {
        sub_0202AC0C(Save_Misc_Get(recorder->saveData), &recorder->colorScheme);
        if (recorder->colorScheme >= 7) {
            recorder->colorScheme = 0;
        }
        recorder->blendColor = VsRecorder_GetBlendColor(recorder);
    } else {
        recorder->blendColor = 0x7FDD;
    }
    VsRecorder_InitResources(recorder);
    return TRUE;
}

int VsRecorderMenu_Init(OverlayManager *manager, int *state) {
    return VsRecorderMenu_InitCommon(manager, state, FALSE);
}

int VsRecorderMenu_InitFromTerminal(OverlayManager *manager, int *state) {
    return VsRecorderMenu_InitCommon(manager, state, TRUE);
}

int VsRecorderMenu_Main(OverlayManager *manager, int *state) {
    VsRecorderWork *recorder = VsRecorderApp_GetRecorderWork(manager);
    recorder->screenPtr = state;
    if (VsRecorder_Main(recorder, state)) {
        return TRUE;
    }
    return FALSE;
}

int VsRecorderMenu_Exit(OverlayManager *manager, int *state) {
    VsRecorderWork *recorder = VsRecorderApp_GetRecorderWork(manager);

    switch (*state) {
    case 0:
        VsRecorder_DestroyDecorTasks(recorder);
        (*state)++;
        break;
    case 1:
        VsRecorder_DeleteDecorSprites(recorder);
        VsRecorder_DeleteMenuRowSprites(recorder);
        VsRecorder_FreeMenuRowText(recorder);
        (*state)++;
        break;
    case 2:
        if (!OrbitCursor_Delete(recorder->orbitCursors[0])) {
            (*state)++;
        }
        break;
    case 3:
        if (!OrbitCursor_Delete(recorder->orbitCursors[1])) {
            (*state)++;
        }
        break;
    default:
        VsRecorder_FreeResources(recorder);
        Heap_Destroy(HEAP_ID_109);
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_40));
        gSystem.screensFlipped = FALSE;
        GfGfx_SwapDisplay();
        Field_SetEnvironmentSoundState_None_Unk2();
        return TRUE;
    }
    return FALSE;
}

// MWCC emits these two (unreferenced in this TU, one section each) in reverse order of definition
const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenuFromTerminal = { VsRecorderMenu_InitFromTerminal, VsRecorderMenu_Main, VsRecorderMenu_Exit, FS_OVERLAY_ID(OVY_40) };
const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenu = { VsRecorderMenu_Init, VsRecorderMenu_Main, VsRecorderMenu_Exit, FS_OVERLAY_ID(OVY_40) };

const VsRecorderMenuEntry *VsRecorderApp_GetRootMenu(int mode) {
    if (mode >= 7) {
        GF_ASSERT(mode < 7);
        return *(const VsRecorderMenuEntry **)(_02103754 + 0x64);
    }
    return _021037B8[mode].menu;
}

const VsRecorderMenuEntry *VsRecorderApp_GetStartMenu(VsRecorderWork *recorder, int mode) {
    if (mode == 0 && sub_02087E1C(recorder) == 0) {
        return _0210357C;
    }
    if (mode == 3 && sub_02087E1C(recorder) == 0) {
        return _02102DC0;
    }
    return VsRecorderApp_GetRootMenu(mode);
}

void VsRecorderApp_CountMenuChoices(VsRecorderWork *recorder) {
    int i = 0;
    const VsRecorderMenuEntry *entry = recorder->menu;

    recorder->choiceCount = 0;
    while (i < 5) {
        if (entry->enabled) {
            recorder->choiceCount++;
        }
        i++;
        entry++;
    }
}
