#include "vs_recorder_menu_app_internal.h"

int VsRecorderMenu_InitCommon(OverlayManager *manager, int *state, BOOL special) {
    BattleAppWork *work;

    StopAllSE(0);
    Heap_Create(HEAP_ID_3, HEAP_ID_109, 0x55000);
    work = VsRecorderApp_GetRecorderWork(manager);
    VsRecorder_MenuPop(work);
    GF_SndHandleSetPlayerVolume(1, 0x2A);
    GF_SndHandleSetPlayerVolume(7, 0x2A);
    if (special != FALSE) {
        sub_02055198(NULL, 0x482);
    }
    if (special == FALSE) {
        sub_0202AC0C(Save_Misc_Get(work->saveData), &work->value5C);
        if (work->value5C >= 7) {
            work->value5C = 0;
        }
        work->result58 = VsRecorder_GetBlendColor(work);
    } else {
        work->result58 = 0x7FDD;
    }
    VsRecorder_InitResources(work);
    return TRUE;
}

int VsRecorderMenu_Init(OverlayManager *manager, int *state) {
    return VsRecorderMenu_InitCommon(manager, state, FALSE);
}

int VsRecorderMenu_InitFromTerminal(OverlayManager *manager, int *state) {
    return VsRecorderMenu_InitCommon(manager, state, TRUE);
}

int VsRecorderMenu_Main(OverlayManager *manager, int *state) {
    BattleAppWork *work = VsRecorderApp_GetRecorderWork(manager);
    work->statePtr = state;
    if (VsRecorder_Main(work, state)) {
        return TRUE;
    }
    return FALSE;
}

int VsRecorderMenu_Exit(OverlayManager *manager, int *state) {
    BattleAppWork *work = VsRecorderApp_GetRecorderWork(manager);

    switch (*state) {
    case 0:
        VsRecorder_DestroyDecorTasks(work);
        (*state)++;
        break;
    case 1:
        VsRecorder_DeleteDecorSprites(work);
        VsRecorder_DeleteMenuRowSprites(work);
        VsRecorder_FreeMenuRowText(work);
        (*state)++;
        break;
    case 2:
        if (!OrbitCursor_Delete(work->obj6F0)) {
            (*state)++;
        }
        break;
    case 3:
        if (!OrbitCursor_Delete(work->obj6F4)) {
            (*state)++;
        }
        break;
    default:
        VsRecorder_FreeResources(work);
        Heap_Destroy(HEAP_ID_109);
        UnloadOverlayByID(OVERLAY_40_ID);
        gSystem.screensFlipped = FALSE;
        GfGfx_SwapDisplay();
        Field_SetEnvironmentSoundState_None_Unk2();
        return TRUE;
    }
    return FALSE;
}

const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenu = { VsRecorderMenu_Init, VsRecorderMenu_Main, VsRecorderMenu_Exit, OVERLAY_40_ID };
const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenuFromTerminal = { VsRecorderMenu_InitFromTerminal, VsRecorderMenu_Main, VsRecorderMenu_Exit, OVERLAY_40_ID };
