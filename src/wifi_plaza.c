#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "unk_02037C94.h"
#include "unk_020915B0.h"
#include "wifi_plaza_internal.h"

// Overlay data of the Wi-Fi Plaza overlay.
typedef struct WiFiPlaza {
    SaveData *save;
    WiFiPlazaWork *work;
    void *apps;
    SysTask *vblankTask;
} WiFiPlaza;

FS_EXTERN_OVERLAY(OVY_42);

BOOL LoadOVY38(void);
void UnloadOVY38(void);

BOOL WiFiPlaza_Init(OverlayManager *man, int *state) {
    WiFiPlaza *plaza;
    WiFiPlazaArgs *args;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_42), OVY_LOAD_ASYNC);
    LoadDwcOverlay();
    LoadOVY38();
    sub_02039FD8(HEAP_ID_3);
    Heap_Create(HEAP_ID_3, HEAP_ID_111, 0x5000);
    plaza = OverlayManager_CreateAndGetData(man, sizeof(WiFiPlaza), HEAP_ID_111);
    {
        u8 *dst = (u8 *)plaza;
        u32 count = sizeof(WiFiPlaza);
        do {
            *dst++ = 0;
            count--;
        } while (count != 0);
    }
    args = OverlayManager_GetArgs(man);
    {
        SaveData *save = args->save;
        plaza->save = save;
        plaza->work = WiFiPlazaWork_New(save, HEAP_ID_111);
    }
    plaza->apps = WiFiPlazaApps_New(args->unk_08, args->save, args->unk_00, plaza->work, HEAP_ID_111);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    plaza->vblankTask = SysTask_CreateOnVWaitQueue(WiFiPlaza_VBlankTask, plaza, 0);
    WiFiPlazaApps_StartFirst(plaza->apps);
    return TRUE;
}

BOOL WiFiPlaza_Main(OverlayManager *man, int *state) {
    WiFiPlaza *plaza = OverlayManager_GetData(man);
    int done = WiFiPlazaApps_Run(plaza->apps);
    WiFiPlazaWork_Update(plaza->work);
    if (done == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlaza_Exit(OverlayManager *man, int *state) {
    WiFiPlaza *plaza;
    OverlayManager *manager = man;
    plaza = OverlayManager_GetData(man);
    SysTask_Destroy(plaza->vblankTask);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    WiFiPlazaApps_Delete(plaza->apps);
    WiFiPlazaWork_Delete(plaza->work);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_111);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
    UnloadOVY38();
    UnloadDwcOverlay();
    return TRUE;
}

void WiFiPlaza_VBlankTask(SysTask *task, void *data) {
    WiFiPlaza *plaza = data;
    WiFiPlazaApps_VBlank(plaza->apps);
    ov45_0222A1F8(plaza->work);
}

// The original objects of WiFiPlazaWork_New and WiFiPlazaWork_Update were force-active.
#pragma force_active on

WiFiPlazaWork *WiFiPlazaWork_New(SaveData *save, enum HeapID heapID) {
    WiFiPlazaCallbacks callbacks;
    int i;
    WiFiPlazaWork *work = Heap_Alloc(heapID, sizeof(WiFiPlazaWork));
    memset(work, 0, sizeof(WiFiPlazaWork));
    work->save = save;
    work->heapID = heapID;
    callbacks.func0 = ov45_0222B2B4;
    callbacks.func1 = ov45_0222B470;
    callbacks.func2 = ov45_0222B530;
    callbacks.func3 = ov45_0222B5A0;
    callbacks.func4 = ov45_0222B75C;
    ov45_0222E5D4(heapID, work->save, 0x94, &callbacks, work);
    work->unk_04 = ov45_0222D860(heapID);
    for (i = 0; i < 4; i++) {
        work->profiles[i] = PlayerProfile_New(heapID);
    }
    WiFiPlazaProfile_InitFromSave(&work->profile, work->save, heapID);
    ov45_0222BD40(&work->unk_1c0);
    ov45_0222BD30(work->unk_1e8);
    ov45_0222EE20(ov45_02254AC4, 8, work);
    ov45_0222BC3C(&work->unk_1fc);
    ov45_0222C388(work->unk_20c);
    ov45_0222C8AC(work->unk_3ac);
    WiFiPlazaWordSet_Init(work->wordSet, heapID);
    ov45_0222CB44(work->unk_4bc, work->save);
    ov45_0222BCC8(&work->unk_508, heapID);
    return work;
}

#pragma force_active reset

void WiFiPlazaWork_Delete(WiFiPlazaWork *work) {
    WiFiPlazaPlayerUnk8C vals;
    int i;
    void *x;
    ov45_0222AB0C(&work->profile.player, &vals);
    x = Save_PlazaSurvey_Get(work->save);
    PlazaSurvey_SetSurveyId(x, vals.unk_00);
    PlazaSurvey_SetAnswer(x, vals.unk_04);
    ov45_0222BCD8(&work->unk_508);
    WiFiPlazaWordSet_Free(work->wordSet);
    ov45_0222EE80();
    ov45_0222D890(work->unk_04);
    for (i = 0; i < 4; i++) {
        Heap_Free(work->profiles[i]);
    }
    ov45_0222E688();
    Heap_Free(work);
}

// Per frame (WiFiPlaza_Main).
#pragma force_active on

void WiFiPlazaWork_Update(WiFiPlazaWork *work) {
    s64 time;
    ov45_0222D8A4(work->unk_04);
    ov45_0222ECB8(&time);
    work->unk_d8 = time;
    ov45_0222D500(&work->unk_d4, &time);
    ov45_0222B840(work);
    ov45_0222BE5C(&work->unk_1c0);
    ov45_0222BD30(work->unk_1e8);
    ov45_0222BB58(work->unk_f8);
    ov45_0222BB60(&work->unk_1fc, work->unk_f8, work->unk_20c, work->unk_49c);
    ov45_0222BCB8(work->unk_3a0);
    ov45_0222C3B0(work->unk_20c);
    ov45_0222CAA0(work->unk_49c);
    if (ov45_0222CCDC(work->unk_4bc) == 1) {
        work->unk_52c = 1;
    }
}

#pragma force_active reset
