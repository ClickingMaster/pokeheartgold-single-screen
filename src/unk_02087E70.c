#include "unk_02087E70_internal.h"

int sub_02087E70(OverlayManager *manager, int *state, BOOL special) {
    BattleAppWork *work;

    StopAllSE(0);
    Heap_Create(HEAP_ID_3, HEAP_ID_109, 0x55000);
    work = sub_02087A78(manager);
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

int sub_02087EF8(OverlayManager *manager, int *state) {
    return sub_02087E70(manager, state, FALSE);
}

int sub_02087F04(OverlayManager *manager, int *state) {
    return sub_02087E70(manager, state, TRUE);
}

int sub_02087F10(OverlayManager *manager, int *state) {
    BattleAppWork *work = sub_02087A78(manager);
    work->statePtr = state;
    if (VsRecorder_Main(work, state)) {
        return TRUE;
    }
    return FALSE;
}

int sub_02087F2C(OverlayManager *manager, int *state) {
    BattleAppWork *work = sub_02087A78(manager);

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

const OverlayManagerTemplate _021028B4 = { sub_02087EF8, sub_02087F10, sub_02087F2C, OVERLAY_40_ID };
const OverlayManagerTemplate _021028C4 = { sub_02087F04, sub_02087F10, sub_02087F2C, OVERLAY_40_ID };
