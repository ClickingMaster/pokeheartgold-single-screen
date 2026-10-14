#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "comm_state.h"
#include "network_overlay_loader.h"
#include "wifi_plaza_internal.h"

// Overlay data of the Wi-Fi Plaza overlay.
typedef struct WiFiPlaza {
    SaveData *save;
    WiFiPlazaWork *work;
    WiFiPlazaApps *apps;
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
    Comm_InitDwc(HEAP_ID_3);
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
    plaza->apps = WiFiPlazaApps_New(args->skipPrompt, args->save, args->loginTime, plaza->work, HEAP_ID_111);
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
    WiFiPlazaWork_VBlankDummy(plaza->work);
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
    callbacks.onPlayerEnter = WiFiPlaza_OnPlayerEnter;
    callbacks.onPlayerLeave = WiFiPlaza_OnPlayerLeave;
    callbacks.onPlayerUpdate = WiFiPlaza_OnPlayerUpdate;
    callbacks.onLobbyEvent = WiFiPlaza_OnLobbyEvent;
    callbacks.onOwnRecord = WiFiPlaza_OnOwnRecord;
    WiFiPlazaLobbyClient_Init(heapID, work->save, 0x94, &callbacks, work);
    work->news = WiFiPlazaNews_New(heapID);
    for (i = 0; i < 4; i++) {
        work->profiles[i] = PlayerProfile_New(heapID);
    }
    WiFiPlazaProfile_InitFromSave(&work->profile, work->save, heapID);
    WiFiPlazaTalkSession_Init(&work->talk);
    WiFiPlaza_ClearTapToyPlayedFlags(work->unk_1e8);
    WiFiPlazaLobbyClient_SetLobbyCommands(sWiFiPlazaLobbyCommands, 8, work);
    WiFiPlazaEventSchedule_Init(&work->eventSchedule);
    WiFiPlazaGroups_Init(&work->groups);
    WiFiPlazaGroupWeights_Init(&work->groupWeights);
    WiFiPlazaWordSet_Init(&work->wordSet, heapID);
    WiFiPlazaHistory_Init(&work->history, work->save);
    WiFiPlaza_NewPlayerProfile(&work->unk_508.profile, heapID);
    return work;
}

#pragma force_active reset

void WiFiPlazaWork_Delete(WiFiPlazaWork *work) {
    WiFiPlazaPlayerSurvey vals;
    int i;
    void *x;
    WiFiPlazaPlayer_GetSurvey(&work->profile.player, &vals);
    x = Save_PlazaSurvey_Get(work->save);
    PlazaSurvey_SetSurveyId(x, vals.surveyId);
    PlazaSurvey_SetAnswer(x, vals.answer);
    WiFiPlaza_FreePlayerProfile(&work->unk_508.profile);
    WiFiPlazaWordSet_Free(&work->wordSet);
    WiFiPlazaLobbyClient_ClearLobbyCommands();
    WiFiPlazaNews_Delete(work->news);
    for (i = 0; i < 4; i++) {
        Heap_Free(work->profiles[i]);
    }
    WiFiPlazaLobbyClient_Free();
    Heap_Free(work);
}

// Per frame (WiFiPlaza_Main).
#pragma force_active on

void WiFiPlazaWork_Update(WiFiPlazaWork *work) {
    s64 time;
    WiFiPlazaNews_Update(work->news);
    WiFiPlazaLobbyClient_GetTime(&time);
    work->lobbyTime = time;
    WiFiPlaza_SecondsToTime(&work->currentTime, &time);
    WiFiPlazaWork_CheckClosingTime(work);
    WiFiPlazaTalkSession_UpdateTimeout(&work->talk);
    WiFiPlaza_ClearTapToyPlayedFlags(work->unk_1e8);
    WiFiPlaza_ClearEventFlags(work->changeFlags);
    WiFiPlazaEventSchedule_Update(&work->eventSchedule, work->changeFlags, &work->groups, &work->timer);
    WiFiPlazaUnk3A0_UpdateCooldown(&work->unk_3a0);
    WiFiPlazaGroups_Update(&work->groups);
    WiFiPlazaTimer_Update(&work->timer);
    if (WiFiPlazaHistory_IsCorrupt(&work->history) == 1) {
        work->hasError = 1;
    }
}

#pragma force_active reset
