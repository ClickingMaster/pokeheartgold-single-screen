#include "global.h"

#include "constants/game_stats.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"
#include "constants/wifi_plaza.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0777.h"
#include "wifi_plaza/wifi_plaza_internal.h"

#include "comm_state.h"
#include "easy_chat.h"
#include "font.h"
#include "game_stats.h"
#include "heap.h"
#include "math_util.h"
#include "msgdata.h"
#include "network_overlay_loader.h"
#include "overlay_manager.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokedex.h"
#include "pokemon.h"
#include "sav_system_info.h"
#include "save_wifi_history.h"
#include "sound.h"
#include "sound_system.h"
#include "system.h"

typedef struct WiFiPlaza {
    SaveData *save;
    WiFiPlazaWork *work;
    WiFiPlazaApps *apps;
    SysTask *vblankTask;
} WiFiPlaza;

FS_EXTERN_OVERLAY(OVY_42);

// What PPW_LobbyGetTimeInfo reports: the lobby time first.
typedef struct WiFiPlazaLobbyTime {
    s64 time;
    u8 unk_08[16];
} WiFiPlazaLobbyTime;

static void WiFiPlaza_VBlankTask(SysTask *task, void *data);
static WiFiPlazaWork *WiFiPlazaWork_New(SaveData *save, enum HeapID heapID);
static void WiFiPlazaWork_Delete(WiFiPlazaWork *work);
static void WiFiPlazaWork_Update(WiFiPlazaWork *work);
static void WiFiPlazaWork_VBlankDummy(WiFiPlazaWork *work);
static u32 WiFiPlazaPlayer_GetTrainerID(WiFiPlazaPlayer *player);
static BOOL WiFiPlazaWork_CanTalkWith(WiFiPlazaWork *work, u32 index);
static BOOL WiFiPlaza_IsNameTerminated(const u16 *name, u32 count, enum HeapID heapID);
static void WiFiPlaza_OnPlayerEnter(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work, BOOL isOwn);
static void WiFiPlaza_OnPlayerLeave(int id, WiFiPlazaWork *work);
static void WiFiPlaza_OnPlayerUpdate(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work);
static void WiFiPlaza_OnLobbyEvent(u32 event, WiFiPlazaWork *work);
static void WiFiPlaza_OnOwnRecord(const WiFiPlazaPlayer *data, u32 size, WiFiPlazaWork *work);
static void WiFiPlaza_FillPlaces(WiFiPlazaProfile *profile, WiFiPlazaPlace *out);
static void WiFiPlazaWork_CheckClosingTime(WiFiPlazaWork *work);
static void WiFiPlazaProfile_InitFromSave(WiFiPlazaProfile *profile, SaveData *save, enum HeapID heapID);
static void WiFiPlazaWork_SendOwnRecord(WiFiPlazaWork *work);
static void WiFiPlazaProfile_UpdateChecksum(WiFiPlazaProfile *profile, SaveData *save);
static BOOL WiFiPlazaProfile_IsChecksumValid(WiFiPlazaProfile *profile, SaveData *save);
static void WiFiPlazaProfile_SetFromLobbyRecord(WiFiPlazaProfile *profile, const WiFiPlazaPlayer *src, u32 size);
static void WiFiPlaza_ClearEventFlags(u32 *flags);
static void WiFiPlazaEventSchedule_Update(WiFiPlazaEventSchedule *state, u32 *flags, WiFiPlazaGroups *groups, WiFiPlazaTimer *timer);
static void WiFiPlazaEventSchedule_Init(WiFiPlazaEventSchedule *state);
static void WiFiPlazaEventSchedule_ApplyVolume(WiFiPlazaEventSchedule *state);
static void WiFiPlazaSessionState_StartCooldown(WiFiPlazaSessionState *state);
static int WiFiPlazaSessionState_IsCoolingDown(WiFiPlazaSessionState *state);
static void WiFiPlazaSessionState_UpdateCooldown(WiFiPlazaSessionState *state);
static void WiFiPlaza_NewPlayerProfile(PlayerProfile **profile, enum HeapID heapID);
static void WiFiPlaza_FreePlayerProfile(PlayerProfile **profile);
static void WiFiPlazaProfile_CopyToPlayerProfile(PlayerProfile **profile, WiFiPlazaProfile *src, enum HeapID heapID);
static void WiFiPlazaSessionState_SetAppQuitForClosing(WiFiPlazaSessionState *state);
static u8 WiFiPlazaSessionState_DidAppQuitForClosing(WiFiPlazaSessionState *state);
static void WiFiPlaza_ClearTapToyPlayedFlags(u8 *buffer);
static void WiFiPlazaTalkSession_Init(WiFiPlazaTalkSession *session);
static void WiFiPlazaTalkSession_Reset(WiFiPlazaTalkSession *session);
static void WiFiPlazaTalkSession_Abort(WiFiPlazaTalkSession *session);
static BOOL WiFiPlazaTalkSession_HasTalkedWith(WiFiPlazaTalkSession *session, u32 slot);
static void WiFiPlazaTalkSession_SetTalkedWith(WiFiPlazaTalkSession *session, u32 slot);
static void WiFiPlazaTalkSession_ClearTalkedWith(WiFiPlazaTalkSession *session, u32 slot);
static void WiFiPlazaTalkSession_IncTalkCount(WiFiPlazaTalkSession *session, u32 slot);
static void WiFiPlazaTalkSession_ResetTalkCount(WiFiPlazaTalkSession *session, u32 slot);
static void WiFiPlazaTalkSession_SetMessage(WiFiPlazaTalkSession *session, int value);
static void WiFiPlazaWork_SendTalkAnswer(WiFiPlazaWork *work, int id, u16 value);
static void WiFiPlazaWork_SendTalkEnd(WiFiPlazaWork *work, int id);
static void WiFiPlazaTalkSession_StartTimeout(WiFiPlazaTalkSession *session);
static void WiFiPlazaTalkSession_StopTimeout(WiFiPlazaTalkSession *session);
static void WiFiPlazaTalkSession_UpdateTimeout(WiFiPlazaTalkSession *session);
static BOOL WiFiPlazaTalkSession_IsWithinTimeout(WiFiPlazaTalkSession *session);
static s16 WiFiPlazaTalkSession_GetTimeoutCounter(WiFiPlazaTalkSession *session);
static void WiFiPlaza_HandleChatNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg);
static void WiFiPlaza_HandleTapToyTradeNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg);
static void WiFiPlaza_HandleMinigameEntryNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg);
static void WiFiPlaza_HandleFootprintStampNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg);
static void WiFiPlaza_HandleVisitorProfilesNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg);
static void WiFiPlaza_HandlePlazaNewsNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg);
static void WiFiPlaza_HandleMinigameResultNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg);
static void WiFiPlaza_HandleUnusedNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg);
static void WiFiPlazaWork_AddLobbyEventNotice(WiFiPlazaWork *work, int which);
static void WiFiPlazaGroups_Init(WiFiPlazaGroups *groups);
static void WiFiPlazaGroups_Start(WiFiPlazaGroups *groups);
static void WiFiPlazaGroups_Update(WiFiPlazaGroups *groups);
static int WiFiPlazaGroups_Join(WiFiPlazaGroups *groups, u32 id, u32 value);
static void WiFiPlazaGroups_Leave(WiFiPlazaGroups *groups, u32 id);
static int WiFiPlazaGroups_GetPlaceId(WiFiPlazaGroups *groups, u32 place);
static int WiFiPlazaGroups_GetState(WiFiPlazaGroups *groups, u32 group);
static int WiFiPlazaGroups_GetCounter(WiFiPlazaGroups *groups, u32 group);
static void WiFiPlazaGroups_RequestAction(WiFiPlazaGroups *groups, u32 id);
static int WiFiPlazaGroups_GetActionRequest(WiFiPlazaGroups *groups, u32 place);
static void WiFiPlazaGroups_ClearActionRequest(WiFiPlazaGroups *groups, u32 place);
static int WiFiPlazaGroups_FindPlace(WiFiPlazaGroups *groups, int id);
static void WiFiPlazaGroups_SetMarkerPassed(WiFiPlazaGroups *groups, u32 place, u32 value);
static BOOL WiFiPlazaGroups_HasPassedMarker(WiFiPlazaGroups *groups, u32 place, u32 value);
static u32 WiFiPlazaGroups_GetMarkerCount(WiFiPlazaGroups *groups, u32 place);
static void WiFiPlaza_OnTalkRequest(int id, const void *data, u32 size, void *arg);
static void WiFiPlaza_OnTalkAnswer(int id, const void *data, u32 size, void *arg);
static void WiFiPlaza_OnTalkState(int id, const void *data, u32 size, void *arg);
static void WiFiPlaza_OnTalkCancel(int id, const void *data, u32 size, void *arg);
static void WiFiPlaza_OnLobbyMessage(int id, const void *data, u32 size, void *arg);
static void WiFiPlaza_RecvTapToyPlayed(int id, const void *data, u32 size, void *arg);
static void WiFiPlaza_RecvJoinGroup(int id, const void *data, u32 size, void *arg);
static void WiFiPlaza_RecvGroupAction(int id, const void *data, u32 size, void *arg);
static void WiFiPlazaGroupWeights_Init(WiFiPlazaGroupWeights *weights);
static void WiFiPlazaGroupWeights_ReportPlace(WiFiPlazaGroupWeights *weights, u32 place, u32 slot);
static u32 WiFiPlazaGroupWeights_PickGroup(WiFiPlazaGroupWeights *weights);
static void WiFiPlaza_SetSlotFlag(u8 *flags, u32 slot, u8 value);
static u32 WiFiPlaza_GetSlotFlag(u8 *flags, u32 slot);
static void WiFiPlazaWordSet_Init(WiFiPlazaWordSet *wordSet, enum HeapID heapID);
static void WiFiPlazaWordSet_Free(WiFiPlazaWordSet *wordSet);
static void WiFiPlazaWordSet_Set(WiFiPlazaWordSet *wordSet, u32 slot, u8 flag, u32 code);
static u32 WiFiPlazaWordSet_HasWords(WiFiPlazaWordSet *wordSet, u32 slot);
static u16 *WiFiPlazaWordSet_GetWords(WiFiPlazaWordSet *wordSet, u32 slot);
static void WiFiPlaza_MakeWordsFromCode(WallpaperPasswordBank *bank, u32 code, u16 *words);
static void WiFiPlazaTimer_Init(WiFiPlazaTimer *timer, int start, int end);
static void WiFiPlazaTimer_Start(WiFiPlazaTimer *timer);
static void WiFiPlazaTimer_Update(WiFiPlazaTimer *timer);
static u32 WiFiPlazaTimer_GetCounter(WiFiPlazaTimer *timer);
static u32 WiFiPlazaTimer_GetPhase(WiFiPlazaTimer *timer);

static const WiFiPlazaTimerPhase sWiFiPlazaTimerPhases[5] = {
    { 2, 80  },
    { 1, 2   },
    { 3, 142 },
    { 1, 2   },
    { 4, 30  },
};

static void (*const sWiFiPlazaLobbyMessageHandlers[])(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) = {
    WiFiPlaza_HandleChatNews,
    WiFiPlaza_HandleTapToyTradeNews,
    WiFiPlaza_HandleMinigameEntryNews,
    WiFiPlaza_HandleFootprintStampNews,
    WiFiPlaza_HandleVisitorProfilesNews,
    WiFiPlaza_HandlePlazaNewsNews,
    WiFiPlaza_HandleUnusedNews,
    WiFiPlaza_HandleUnusedNews,
    WiFiPlaza_HandleMinigameResultNews,
};

static const s32 sWiFiPlazaGroupStartTimes[9] = { 0, 360, 720, 1200, 1560, 1920, 2400, 2760, 3120 };

const WiFiPlazaAvatarEntry gWiFiPlazaAvatars[16] = {
    { 3,  0 },
    { 5,  0 },
    { 11, 0 },
    { 31, 0 },
    { 50, 0 },
    { 51, 0 },
    { 62, 0 },
    { 70, 0 },
    { 6,  1 },
    { 7,  1 },
    { 13, 1 },
    { 14, 1 },
    { 35, 1 },
    { 37, 1 },
    { 42, 1 },
    { 63, 1 },
};

static const WiFiPlazaLobbyCommand sWiFiPlazaLobbyCommands[] = {
    { WiFiPlaza_OnTalkRequest,    4                             },
    { WiFiPlaza_OnTalkAnswer,     4                             },
    { WiFiPlaza_OnTalkState,      4                             },
    { WiFiPlaza_OnTalkCancel,     4                             },
    { WiFiPlaza_OnLobbyMessage,   sizeof(WiFiPlazaLobbyMessage) },
    { WiFiPlaza_RecvTapToyPlayed, 4                             },
    { WiFiPlaza_RecvJoinGroup,    4                             },
    { WiFiPlaza_RecvGroupAction,  4                             },
};

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

static void WiFiPlaza_VBlankTask(SysTask *task, void *data) {
    WiFiPlaza *plaza = data;
    WiFiPlazaApps_VBlank(plaza->apps);
    WiFiPlazaWork_VBlankDummy(plaza->work);
}

static WiFiPlazaWork *WiFiPlazaWork_New(SaveData *save, enum HeapID heapID) {
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
    WiFiPlaza_ClearTapToyPlayedFlags(work->tapToyPlayed);
    WiFiPlazaLobbyClient_SetLobbyCommands(sWiFiPlazaLobbyCommands, 8, work);
    WiFiPlazaEventSchedule_Init(&work->eventSchedule);
    WiFiPlazaGroups_Init(&work->groups);
    WiFiPlazaGroupWeights_Init(&work->groupWeights);
    WiFiPlazaWordSet_Init(&work->wordSet, heapID);
    WiFiPlazaHistory_Init(&work->history, work->save);
    WiFiPlaza_NewPlayerProfile(&work->ownProfileData.profile, heapID);
    return work;
}

static void WiFiPlazaWork_Delete(WiFiPlazaWork *work) {
    WiFiPlazaPlayerSurvey survey;
    int i;
    void *surveySave;
    WiFiPlazaPlayer_GetSurvey(&work->profile.player, &survey);
    surveySave = Save_PlazaSurvey_Get(work->save);
    PlazaSurvey_SetSurveyId(surveySave, survey.surveyId);
    PlazaSurvey_SetAnswer(surveySave, survey.answer);
    WiFiPlaza_FreePlayerProfile(&work->ownProfileData.profile);
    WiFiPlazaWordSet_Free(&work->wordSet);
    WiFiPlazaLobbyClient_ClearLobbyCommands();
    WiFiPlazaNews_Delete(work->news);
    for (i = 0; i < 4; i++) {
        Heap_Free(work->profiles[i]);
    }
    WiFiPlazaLobbyClient_Free();
    Heap_Free(work);
}

// Per frame.
static void WiFiPlazaWork_Update(WiFiPlazaWork *work) {
    s64 time;
    WiFiPlazaNews_Update(work->news);
    WiFiPlazaLobbyClient_GetTime(&time);
    work->lobbyTime = time;
    WiFiPlaza_SecondsToTime(&work->currentTime, &time);
    WiFiPlazaWork_CheckClosingTime(work);
    WiFiPlazaTalkSession_UpdateTimeout(&work->talk);
    WiFiPlaza_ClearTapToyPlayedFlags(work->tapToyPlayed);
    WiFiPlaza_ClearEventFlags(work->changeFlags);
    WiFiPlazaEventSchedule_Update(&work->eventSchedule, work->changeFlags, &work->groups, &work->timer);
    WiFiPlazaSessionState_UpdateCooldown(&work->sessionState);
    WiFiPlazaGroups_Update(&work->groups);
    WiFiPlazaTimer_Update(&work->timer);
    if (WiFiPlazaHistory_IsCorrupt(&work->history) == 1) {
        work->hasError = 1;
    }
}

static void WiFiPlazaWork_VBlankDummy(WiFiPlazaWork *work) {
}

u32 WiFiPlazaWork_HasError(WiFiPlazaWork *work) {
    return work->hasError;
}

void WiFiPlazaWork_TakeExcessFlood(void *a0, void *a1, void *a2) {
    WiFiPlazaLobbyClient_TakeExcessFlood();
}

void *WiFiPlazaWork_GetNews(WiFiPlazaWork *work) {
    return work->news;
}

void *WiFiPlazaWork_GetPlaces(WiFiPlazaWork *work) {
    WiFiPlaza_FillPlaces(&work->profile, work->places);
    return work->places;
}

u32 *WiFiPlazaWork_GetCurrentTimePtr(WiFiPlazaWork *work) {
    return &work->currentTime;
}

BOOL WiFiPlazaWork_TakePlayerEntered(WiFiPlazaWork *work, int bit) {
    u32 mask = 1 << bit;
    if (work->changeFlags[0] & mask) {
        work->changeFlags[0] &= ~mask;
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaWork_TakePlayerLeft(WiFiPlazaWork *work, int bit) {
    u32 mask = 1 << bit;
    if (work->changeFlags[1] & mask) {
        work->changeFlags[1] &= ~mask;
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaWork_IsEventFlagSet(WiFiPlazaWork *work, int bit) {
    return (work->changeFlags[2] & (1 << bit)) != 0;
}

BOOL WiFiPlazaWork_TakePlayerUpdated(WiFiPlazaWork *work, int bit) {
    u32 mask = 1 << bit;
    if (work->changeFlags[3] & mask) {
        work->changeFlags[3] &= ~mask;
        return TRUE;
    }
    return FALSE;
}

void *WiFiPlazaWork_GetSaveData(void **ptr) {
    return *ptr;
}

int WiFiPlazaWork_IsArceusFootprintShown(WiFiPlazaWork *work) {
    GF_ASSERT(work != NULL);
    return WiFiPlazaLobbyClient_GetScheduleValue(4);
}

u32 WiFiPlazaWork_HasAnsweredSurvey(WiFiPlazaWork *work) {
    GF_ASSERT(work != NULL);
    return work->eventSchedule.low;
}

u32 WiFiPlazaWork_GetEventScheduleHigh(WiFiPlazaWork *work) {
    GF_ASSERT(work != NULL);
    return work->eventSchedule.high;
}

void WiFiPlazaWork_SetEventScheduleHigh(WiFiPlazaWork *work) {
    work->eventSchedule.high = 1;
}

u32 WiFiPlazaWork_IsLobbyOpen(WiFiPlazaWork *work) {
    return work->eventSchedule.flag0;
}

u32 WiFiPlazaWork_IsClosing(WiFiPlazaWork *work) {
    return work->eventSchedule.flag7;
}

BOOL WiFiPlazaWork_IsClosed(WiFiPlazaWork *work) {
    if (work->eventSchedule.flag1 == 1 && work->eventSchedule.closeTimer <= 0) {
        return TRUE;
    }
    return FALSE;
}

u32 WiFiPlazaWork_GetFireworksState(WiFiPlazaWork *work) {
    if (work->eventSchedule.fireworksDelay <= 0) {
        return work->eventSchedule.mode2;
    }
    return 0;
}

BOOL WiFiPlazaWork_IsParadeStarted(WiFiPlazaWork *work) {
    if (work->eventSchedule.flag4 == 1 && work->eventSchedule.paradeDelay <= 0) {
        return TRUE;
    }
    return FALSE;
}

u32 WiFiPlazaWork_GetEventScheduleMode5(WiFiPlazaWork *work) {
    return work->eventSchedule.mode5;
}

BOOL WiFiPlazaWork_IsLightingStarted(WiFiPlazaWork *work) {
    if (work->lightingStarted == 1 && work->eventSchedule.lightingDelay <= 0) {
        return TRUE;
    }
    return FALSE;
}

u32 WiFiPlazaWork_GetColorEvent(WiFiPlazaWork *work) {
    if (work->eventSchedule.lightingDelay <= 0) {
        return work->eventSchedule.colorEvent;
    }
    return 1;
}

u32 WiFiPlazaWork_GetLight1Event(WiFiPlazaWork *work) {
    if (work->eventSchedule.lightingDelay <= 0) {
        return work->eventSchedule.light1Event;
    }
    return 7;
}

u32 WiFiPlazaWork_GetThirdEvent(WiFiPlazaWork *work) {
    if (work->eventSchedule.lightingDelay <= 0) {
        return work->eventSchedule.thirdEvent;
    }
    return 11;
}

void WiFiPlazaWork_StartGameEntryCooldown(WiFiPlazaWork *work) {
    WiFiPlazaSessionState_StartCooldown(&work->sessionState);
}

int WiFiPlazaWork_IsGameEntryCoolingDown(WiFiPlazaWork *work) {
    return WiFiPlazaSessionState_IsCoolingDown(&work->sessionState);
}

BOOL WiFiPlazaWork_GetGameConnectionLost(WiFiPlazaWork *work) {
    return work->ownProfileData.gameConnectionLost;
}

void WiFiPlazaWork_SetGameConnectionLost(WiFiPlazaWork *work, BOOL value) {
    work->ownProfileData.gameConnectionLost = value;
}

void WiFiPlazaWork_ClearGameMembers(WiFiPlazaWork *work) {
    MIi_CpuClear32(0, (u32 *)work->gameMembers, 4);
}

void WiFiPlazaWork_SetGameMember(WiFiPlazaWork *work, int id, u32 index) {
    int value;
    GF_ASSERT(index < 4);
    value = WiFiPlazaLobbyClient_IdToSlot(id);
    GF_ASSERT(value != -1);
    work->gameMembers[index] = value;
}

void WiFiPlazaWork_SetOwnGameMember(WiFiPlazaWork *work, u32 index) {
    int id = WiFiPlazaLobbyClient_GetOwnId();
    WiFiPlazaWork_SetGameMember(work, id, index);
}

void WiFiPlazaWork_GetGameMembers(WiFiPlazaWork *work, void *dest) {
    MI_CpuCopy8(work->gameMembers, dest, 4);
}

void WiFiPlazaWork_SetAppQuitForClosing(WiFiPlazaWork *work) {
    WiFiPlazaSessionState_SetAppQuitForClosing(&work->sessionState);
}

int WiFiPlazaWork_DidAppQuitForClosing(WiFiPlazaWork *work) {
    return WiFiPlazaSessionState_DidAppQuitForClosing(&work->sessionState);
}

void WiFiPlazaWork_SetMinigameActiveFlag(WiFiPlazaWork *work, u8 value) {
    work->eventSchedule.minigameActive = value;
}

void WiFiPlazaWork_PlayBGM(WiFiPlazaWork *work) {
    if (work->eventSchedule.flag4 == 1 && work->eventSchedule.paradeDelay <= 0) {
        Sound_SetSceneAndPlayBGM(0x15, SEQ_GS_WIFIPARADE, 0);
        WiFiPlazaEventSchedule_ApplyVolume(&work->eventSchedule);
        return;
    }
    Sound_SetSceneAndPlayBGM(0x15, SEQ_GS_WIFIUNION, 0);
    WiFiPlazaEventSchedule_ApplyVolume(&work->eventSchedule);
}

void WiFiPlazaWork_SetLowVolume(WiFiPlazaWork *work, int value) {
    if (value == work->eventSchedule.lowVolume) {
        return;
    }
    work->eventSchedule.lowVolume = value;
    WiFiPlazaEventSchedule_ApplyVolume(&work->eventSchedule);
}

int WiFiPlazaWork_GetOwnSlot(void *unused) {
    return WiFiPlazaLobbyClient_IdToSlot(WiFiPlazaLobbyClient_GetOwnId());
}

void WiFiPlazaWork_GetOwnId(void) {
    WiFiPlazaLobbyClient_GetOwnId();
}

BOOL WiFiPlazaWork_IsSlotOccupied(void *unused, int index) {
    WiFiPlazaPlayerList list;
    WiFiPlazaLobbyClient_GetPlayerList(&list);
    return list.ids[index] != -1;
}

WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(WiFiPlazaWork *work, u32 index) {
    WiFiPlazaPlayerList list;
    int id;
    if (index >= 20) {
        GF_AssertFail();
    }
    WiFiPlazaLobbyClient_GetPlayerList(&list);
    id = list.ids[index];
    if (id == -1) {
        return NULL;
    }
    if (id == WiFiPlazaLobbyClient_GetOwnId()) {
        return &work->profile.player;
    }
    return WiFiPlazaLobbyClient_GetPlayer(id);
}

WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(WiFiPlazaWork *work) {
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
    }
    return &work->profile.player;
}

void WiFiPlazaWork_SetActivity(WiFiPlazaWork *work, int value) {
    if (value >= 15) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
        return;
    }
    if (work->profile.player.activity == value) {
        return;
    }
    if (value != 9 && WiFiPlazaWork_IsTalking(work) == 1) {
        if (WiFiPlazaWork_WasAskedToTalk(work) == 0) {
            WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot);
            WiFiPlazaWork_SendTalkEnd(work, work->talk.partnerSlot);
        }
        WiFiPlazaTalkSession_Abort(&work->talk);
    }
    switch (work->profile.player.activity) {
    case 2:
        WiFiPlazaHistory_Add(&work->history, 0x10, -1);
        break;
    case 3:
        WiFiPlazaHistory_Add(&work->history, 0x11, -1);
        break;
    case 4:
        WiFiPlazaHistory_Add(&work->history, 0x12, -1);
        break;
    case 5:
        WiFiPlazaHistory_Add(&work->history, 0x13, -1);
        break;
    case 6:
        WiFiPlazaHistory_Add(&work->history, 0x14, -1);
        break;
    case 7:
        WiFiPlazaHistory_Add(&work->history, 0x15, -1);
        break;
    case 8:
        WiFiPlazaHistory_Add(&work->history, 0x16, -1);
        break;
    }
    work->profile.player.activity = value;
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
    WiFiPlazaWork_SendOwnRecord(work);
}

void WiFiPlazaWork_AddHistory(WiFiPlazaWork *work, int index, int value) {
    if (index >= 24) {
        GF_AssertFail();
    }
    WiFiPlazaHistory_Add(&work->history, index, value);
    WiFiPlazaWork_SendOwnRecord(work);
}

void WiFiPlazaWork_SetTapToy(WiFiPlazaWork *work, u32 value) {
    if (value >= 27) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
        return;
    }
    work->profile.player.tapToy = value;
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
    WiFiPlazaWork_SendOwnRecord(work);
}

void WiFiPlazaWork_SetFavoriteTypes(WiFiPlazaWork *work, int a, int b) {
    if (a >= 18) {
        GF_AssertFail();
    }
    if (b >= 18) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
        return;
    }
    if (a < 18 && b < 18) {
        if (a == 0) {
            work->profile.player.favoriteTypes[0] = b;
            work->profile.player.favoriteTypes[1] = 0;
        } else {
            work->profile.player.favoriteTypes[0] = a;
            work->profile.player.favoriteTypes[1] = b;
        }
        WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
        WiFiPlazaWork_SendOwnRecord(work);
    }
}

void WiFiPlazaWork_SetSurveyAnswer(WiFiPlazaWork *work, u32 a, u32 b) {
    if (b >= 3) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
        return;
    }
    if (b < 3) {
        work->profile.player.survey.surveyId = a;
        work->profile.player.survey.answer = b;
        work->eventSchedule.low = 1;
        WiFiPlazaWork_MarkActivityDone(work, 6);
        WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
        WiFiPlazaWork_SendOwnRecord(work);
    }
}

void WiFiPlazaPlayer_CopyToProfile(WiFiPlazaPlayer *player, PlayerProfile *profile, enum HeapID heapID) {
    BOOL invalid;
    int ok = WiFiPlaza_IsNameTerminated(player->name, 8, heapID);
    String *b;
    String *a;
    invalid = FALSE;
    if (ok == 1) {
        Save_Profile_PlayerName_Set(profile, player->name);
        a = String_New(0x20, heapID);
        b = String_New(0x20, heapID);
        PlayerName_FlatToString(profile, b);
        if (!FontID_String_AllCharsValid((FontID)0, b, a)) {
            invalid = TRUE;
        }
        String_Delete(a);
        String_Delete(b);
    } else {
        invalid = TRUE;
    }
    if (invalid) {
        MsgData *msg = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0777_bin, heapID);
        String *name = NewString_ReadMsgData(msg, msg_0777_00064);
        PlayerName_StringToFlat(profile, name);
        String_Delete(name);
        DestroyMsgData(msg);
    }
    PlayerProfile_SetTrainerID(profile, WiFiPlazaPlayer_GetTrainerID(player));
    PlayerProfile_SetTrainerGender(profile, WiFiPlazaPlayer_GetGender(player));
    PlayerProfile_SetAvatar(profile, WiFiPlazaPlayer_GetAvatar(player));
    PlayerProfile_SetLanguage(profile, WiFiPlazaPlayer_GetLanguage(player));
    PlayerProfile_SetGameClearFlag(profile);
}

int WiFiPlazaPlayer_GetActivity(WiFiPlazaPlayer *player) {
    u8 v = player->activity;
    return v < 14 ? v : 14;
}

#pragma opt_common_subs off

u8 WiFiPlazaPlayer_GetHistoryId(WiFiPlazaPlayer *player, u32 index) {
    int count;
    int i;

    GF_ASSERT(index < 12);
    count = 0;
    for (i = 0; i < 12; i++) {
        if (player->slots[i] < 24) {
            count++;
        }
        if (count - 1 == index) {
            return player->slots[i];
        }
    }
    return 24;
}

#pragma opt_common_subs reset

void *WiFiPlazaPlayer_GetHistoryValue(WiFiPlazaPlayer *player, u32 index) {
    int seen, i;
    if (index >= 12) {
        GF_AssertFail();
    }
    seen = 0;
    for (i = 0; i < 12; i++) {
        u8 v = player->slots[i];
        if (v < 24) {
            seen++;
        }
        if (seen - 1 == index) {
            return player->slotData[i];
        }
    }
    return (void *)-1;
}

u32 WiFiPlazaPlayer_GetLobbyId(WiFiPlazaPlayer *player) {
    return player->lobbyId;
}

static u32 WiFiPlazaPlayer_GetTrainerID(WiFiPlazaPlayer *player) {
    return player->trainerId;
}

int WiFiPlazaPlayer_GetAvatarIndex(WiFiPlazaPlayer *player) {
    u16 v = WiFiPlazaPlayer_GetAvatar(player);
    int i;
    for (i = 0; i < 16; i++) {
        if (v == gWiFiPlazaAvatars[i].avatar) {
            return i;
        }
    }
    return 24;
}

void *WiFiPlazaPlayer_GetLocalTime(WiFiPlazaPlayer *player) {
    return player->localTime;
}

u32 WiFiPlazaPlayer_GetGender(WiFiPlazaPlayer *player) {
    u32 result;
    u32 i;

    if (player->gender >= 2) {
        if (WiFiPlaza_IndexToAvatar(player->avatarIndex) != 0xFFFF) {
            for (i = 0; i < 16; i++) {
                if (gWiFiPlazaAvatars[i].avatar == player->avatarIndex) {
                    result = gWiFiPlazaAvatars[i].avatar;
                }
            }
        } else {
            result = 1;
        }
    } else {
        result = player->gender;
    }
    return result;
}

u32 WiFiPlazaPlayer_GetLanguage(WiFiPlazaPlayer *player) {
    if (WiFiPlazaPlayer_IsLanguageValid(player) == TRUE) {
        return player->language;
    }
    return 2;
}

BOOL WiFiPlazaPlayer_IsLanguageValid(WiFiPlazaPlayer *player) {
    switch (player->language) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        return TRUE;
    default:
        return FALSE;
    }
}

u8 WiFiPlazaPlayer_GetRawLanguage(WiFiPlazaPlayer *player) {
    return player->language;
}

u16 WiFiPlazaPlayer_GetAvatar(WiFiPlazaPlayer *player) {
    u16 avatar = WiFiPlaza_IndexToAvatar(player->avatarIndex);
    if (avatar == 0xFFFF) {
        if (WiFiPlazaPlayer_GetGender(player) == 0) {
            return 3;
        }
        return 6;
    }
    return avatar;
}

u16 WiFiPlazaPlayer_GetCountry(WiFiPlazaPlayer *player) {
    if (player->country >= 234) {
        return 0;
    }
    if (WiFiPlaza_IsRegionValid(player->country, player->region) == 0) {
        return 0;
    }
    return player->country;
}

u8 WiFiPlazaPlayer_GetRegion(WiFiPlazaPlayer *player) {
    if (WiFiPlaza_IsRegionValid(player->country, player->region) == 0) {
        return 0;
    }
    return player->region;
}

u8 WiFiPlazaPlayer_GetTapToy(WiFiPlazaPlayer *player) {
    u8 v = player->tapToy;
    if (v == 0xFF) {
        return 0;
    }
    if (v >= 27) {
        return 0;
    }
    return v;
}

BOOL WiFiPlazaPlayer_HasTapToy(WiFiPlazaPlayer *player) {
    return player->tapToy == 0xFF ? FALSE : TRUE;
}

u16 WiFiPlazaPlayer_GetFavoriteType(WiFiPlazaPlayer *player, u32 index) {
    u16 value;
    if (index >= 2) {
        GF_AssertFail();
    }
    value = player->favoriteTypes[index];
    if (value >= 18) {
        value = 0;
    }
    return value;
}

void WiFiPlazaPlayer_GetSurvey(WiFiPlazaPlayer *player, WiFiPlazaPlayerSurvey *out) {
    *out = player->survey;
}

void *WiFiPlazaWork_GetOwnPlayerProfile(WiFiPlazaWork *work) {
    return work->ownProfileData.profile;
}

int WiFiPlazaWork_IsSpecialVisitor(WiFiPlazaWork *work, int slot) {
    return WiFiPlaza_GetSlotFlag(work->specialVisitors, slot);
}

void WiFiPlazaWork_GetSpecialVisitors(WiFiPlazaWork *work, void *dest) {
    MI_CpuCopy8(work->specialVisitors, dest, sizeof(work->specialVisitors));
}

int WiFiPlazaWork_HasWordSet(WiFiPlazaWork *work, int slot) {
    return WiFiPlazaWordSet_HasWords(&work->wordSet, slot);
}

u16 *WiFiPlazaWork_GetWordSet(WiFiPlazaWork *work, int slot) {
    return WiFiPlazaWordSet_GetWords(&work->wordSet, slot);
}

u32 WiFiPlaza_GetChannelPlayerCount(void) {
    u32 pair[2];
    WiFiPlazaLobbyClient_GetChannelPlayerList(pair);
    return pair[0];
}

int WiFiPlaza_ChannelSlotToSlot(void *unused, int channelSlot) {
    int index = WiFiPlazaLobbyClient_ChannelSlotToId(channelSlot);
    int result = -1;
    if (index != -1) {
        result = WiFiPlazaLobbyClient_IdToSlot(index);
    }
    return result;
}

void WiFiPlaza_SendChatNews(void *unused, u32 slot0, u32 slot1) {
    WiFiPlazaLobbyMessage msg;
    u32 *dst = (u32 *)&msg;
    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
    dst[4] = 0;
    msg.type = 0;
    msg.ids[0] = WiFiPlazaLobbyClient_SlotToId(slot0);
    msg.ids[1] = WiFiPlazaLobbyClient_SlotToId(slot1);
    msg.count = 2;
    WiFiPlazaLobbyClient_SendToLobby(4, &msg, sizeof(msg));
}

void WiFiPlaza_SendTapToyTradeNews(void *unused, u32 slot0, u32 slot1, u8 value) {
    WiFiPlazaLobbyMessage msg = { 0 };
    msg.type = 1;
    msg.ids[0] = WiFiPlazaLobbyClient_SlotToId(slot1);
    msg.ids[1] = WiFiPlazaLobbyClient_SlotToId(slot0);
    msg.count = 2;
    msg.tapToy = value;
    WiFiPlazaLobbyClient_SendToLobby(4, &msg, sizeof(msg));
}

void WiFiPlaza_SendActivityNews(void *unused, int selector, int count, int slot0, int slot1, int slot2, int slot3, int flag) {
    WiFiPlazaLobbyMessage msg = { 0 };
    u32 type;
    (void)unused;
    switch ((u32)selector) {
    case 0:
    case 1:
    case 2:
        type = 2;
        break;
    case 3:
    case 4:
        type = 3;
        break;
    case 5:
        type = 4;
        break;
    case 6:
        type = 5;
        break;
    default:
        return;
    }
    msg.type = type;
    msg.ids[0] = WiFiPlazaLobbyClient_SlotToId(slot0);
    msg.ids[1] = WiFiPlazaLobbyClient_SlotToId(slot1);
    msg.ids[2] = WiFiPlazaLobbyClient_SlotToId(slot2);
    msg.ids[3] = WiFiPlazaLobbyClient_SlotToId(slot3);
    msg.count = count;
    msg.option = selector;
    msg.allPlayers = flag;
    WiFiPlazaLobbyClient_SendToLobby(4, &msg, 20);
}

void WiFiPlaza_SendMinigameResultNews(void *unused, int mode, u8 kind, u32 slot0, u32 slot1, u32 slot2, u32 slot3) {
    WiFiPlazaLobbyMessage msg;
    u32 *dst = (u32 *)&msg;
    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
    dst[4] = 0;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        msg.type = 8;
        msg.ids[0] = WiFiPlazaLobbyClient_SlotToId(slot0);
        msg.ids[1] = WiFiPlazaLobbyClient_SlotToId(slot1);
        msg.ids[2] = WiFiPlazaLobbyClient_SlotToId(slot2);
        msg.ids[3] = WiFiPlazaLobbyClient_SlotToId(slot3);
        msg.count = kind;
        msg.option = mode;
        msg.allPlayers = 0;
        WiFiPlazaLobbyClient_SendToLobby(4, &msg, sizeof(msg));
        break;
    }
}

int WiFiPlazaWork_GetScheduleValue3(WiFiPlazaWork *work) {
    int value = WiFiPlazaLobbyClient_GetScheduleValue(3);
    if ((u32)value >= 5) {
        value = 0;
    }
    return value;
}

int WiFiPlazaWork_GetScheduleValue2(WiFiPlazaWork *work) {
    int value = WiFiPlazaLobbyClient_GetScheduleValue(2);
    if ((u32)value >= 5) {
        value = 0;
    }
    return value;
}

int WiFiPlazaWork_GetOpenDuration(WiFiPlazaWork *work) {
    return WiFiPlazaLobbyClient_GetScheduleValue(5) + 30;
}

u8 WiFiPlazaWork_HasPlayedTapToy(WiFiPlazaWork *work, u32 index) {
    if (index >= 20) {
        GF_AssertFail();
    }
    return work->tapToyPlayed[index];
}

void WiFiPlazaWork_SendTapToyPlayed(void *unused, u32 value) {
    u32 data[1];
    data[0] = value;
    WiFiPlazaLobbyClient_SendToLobby(5, data, sizeof(data));
}

int WiFiPlazaWork_GetGroupState(WiFiPlazaWork *work, int group) {
    return WiFiPlazaGroups_GetState(&work->groups, group);
}

int WiFiPlazaWork_GetGroupCounter(WiFiPlazaWork *work, int group) {
    return WiFiPlazaGroups_GetCounter(&work->groups, group);
}

int WiFiPlazaWork_GetGroupDuration(void) {
    return 1200;
}

int WiFiPlazaWork_FindGroupPlace(WiFiPlazaWork *work, int id) {
    return WiFiPlazaGroups_FindPlace(&work->groups, id);
}

void WiFiPlazaWork_JoinGroup(WiFiPlazaWork *work, int id, int value) {
    WiFiPlazaGroups_Join(&work->groups, id, value);
}

void WiFiPlazaWork_LeaveGroup(WiFiPlazaWork *work, int id) {
    WiFiPlazaGroups_Leave(&work->groups, id);
}

void WiFiPlazaWork_RequestGroupAction(WiFiPlazaWork *work, int id) {
    WiFiPlazaGroups_RequestAction(&work->groups, id);
}

void WiFiPlazaWork_GetGroupActionRequest(WiFiPlazaWork *work, int place) {
    WiFiPlazaGroups_GetActionRequest(&work->groups, place);
}

void WiFiPlazaWork_ClearGroupActionRequest(WiFiPlazaWork *work, int place) {
    WiFiPlazaGroups_ClearActionRequest(&work->groups, place);
}

void WiFiPlaza_SplitGroupPlace(u32 value, u32 *quotient, u32 *remainder) {
    *quotient = value / 3;
    *remainder = value % 3;
}

void WiFiPlazaWork_SetGroupMarkerPassed(WiFiPlazaWork *work, int place, int value) {
    WiFiPlazaGroups_SetMarkerPassed(&work->groups, place, value);
}

int WiFiPlazaWork_GetGroupMarkerCount(WiFiPlazaWork *work, int place) {
    return WiFiPlazaGroups_GetMarkerCount(&work->groups, place);
}

void WiFiPlazaWork_SendJoinGroup(void) {
    u32 data;
    WiFiPlazaLobbyClient_SendToLobby(6, &data, 4);
}

void WiFiPlazaWork_SendGroupAction(void) {
    u32 data;
    WiFiPlazaLobbyClient_SendToLobby(7, &data, 4);
}

void WiFiPlazaWork_ResetTalk(WiFiPlazaWork *work) {
    WiFiPlazaTalkSession_Reset(&work->talk);
}

int WiFiPlazaWork_StartTalk(WiFiPlazaWork *work, int slot) {
    if (work->talk.role) {
        return 0;
    }
    if (WiFiPlazaPlayer_GetActivity(WiFiPlazaWork_GetPlayer(work, slot)) != 1) {
        return 0;
    }
    work->talk.partnerSlot = slot;
    work->talk.state = WIFI_PLAZA_TALK_STATE_REQUESTED;
    work->talk.role = WIFI_PLAZA_TALK_ROLE_ASKER;
    WiFiPlazaTalkSession_SetMessage(&work->talk, 0);
    WiFiPlazaLobbyClient_SendToPlayer(0, WiFiPlazaLobbyClient_SlotToId(slot), &work->talk.sendMessage, 4);
    WiFiPlazaTalkSession_StartTimeout(&work->talk);
    return 1;
}

void WiFiPlazaWork_AdvanceTalk(WiFiPlazaWork *work, int value) {
    int flag = 0;
    if (work->talk.role == WIFI_PLAZA_TALK_ROLE_ASKER) {
        if (work->talk.state != WIFI_PLAZA_TALK_STATE_ASKER_TURN) {
            flag = 1;
        }
    } else if (work->talk.role == WIFI_PLAZA_TALK_ROLE_ANSWERER) {
        if (work->talk.state != WIFI_PLAZA_TALK_STATE_ANSWERER_TURN) {
            flag = 1;
        }
    }
    if (flag) {
        WiFiPlazaWork_SendTalkEnd(work, WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot));
        return;
    }
    switch (work->talk.role) {
    case WIFI_PLAZA_TALK_ROLE_ASKER:
        work->talk.state = WIFI_PLAZA_TALK_STATE_ANSWERER_TURN;
        break;
    case WIFI_PLAZA_TALK_ROLE_ANSWERER:
        work->talk.state = WIFI_PLAZA_TALK_STATE_ASKER_TURN;
        break;
    default:
        WiFiPlazaWork_SendTalkEnd(work, WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot));
        return;
    }
    WiFiPlazaTalkSession_SetMessage(&work->talk, value);
    WiFiPlazaLobbyClient_SendToPlayer(2, WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot), &work->talk.sendMessage, 4);
    WiFiPlazaTalkSession_StartTimeout(&work->talk);
}

void WiFiPlazaWork_CancelTalkRequest(WiFiPlazaWork *work) {
    int id;

    if (work->talk.state != WIFI_PLAZA_TALK_STATE_NONE && work->talk.role == WIFI_PLAZA_TALK_ROLE_ASKER) {
        work->talk.sendState = WIFI_PLAZA_TALK_STATE_NONE;
        id = WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot);
        WiFiPlazaLobbyClient_SendToPlayer(3, id, &work->talk.sendMessage, 4);
        WiFiPlazaTalkSession_Reset(&work->talk);
    }
}

void WiFiPlazaWork_EndTalk(WiFiPlazaWork *work) {
    int id;

    if (work->talk.state != WIFI_PLAZA_TALK_STATE_NONE) {
        id = WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot);
        work->talk.state = WIFI_PLAZA_TALK_STATE_ENDED;
        work->talk.sendState = WIFI_PLAZA_TALK_STATE_ENDED;
        WiFiPlazaLobbyClient_SendToPlayer(2, id, &work->talk.sendMessage, 4);
    }
}

BOOL WiFiPlazaWork_IsTalking(WiFiPlazaWork *work) {
    return work->talk.role != WIFI_PLAZA_TALK_ROLE_NONE;
}

BOOL WiFiPlazaWork_WasAskedToTalk(WiFiPlazaWork *work) {
    return work->talk.role == WIFI_PLAZA_TALK_ROLE_ANSWERER;
}

u16 WiFiPlazaWork_GetTalkPartner(WiFiPlazaWork *work) {
    return work->talk.partnerSlot;
}

u8 WiFiPlazaWork_IsFirstTalk(WiFiPlazaWork *work) {
    return work->talk.isFirstTalk;
}

u8 WiFiPlazaWork_GetTalkState(WiFiPlazaWork *work) {
    return work->talk.state;
}

u16 WiFiPlazaWork_GetTalkPartnerMessage(WiFiPlazaWork *work) {
    return work->talk.partnerMessage;
}

static BOOL WiFiPlazaWork_CanTalkWith(WiFiPlazaWork *work, u32 index) {
    GF_ASSERT(index < 20);
    if (work->talk.talkCounts[index] >= 6) {
        return FALSE;
    }
    return TRUE;
}

BOOL WiFiPlazaWork_IsTalkOver(WiFiPlazaWork *work) {
    if (work->talk.state == WIFI_PLAZA_TALK_STATE_ENDED) {
        return TRUE;
    }
    if (WiFiPlazaTalkSession_IsWithinTimeout(&work->talk) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaWork_GetTalkTimeoutCounter(WiFiPlazaWork *work) {
    WiFiPlazaTalkSession_GetTimeoutCounter(&work->talk);
}

void WiFiPlazaWork_SetTalkPartnerArrived(WiFiPlazaWork *work) {
    work->talk.partnerArrived = 1;
}

u8 WiFiPlazaWork_HasTalkPartnerArrived(WiFiPlazaWork *work) {
    return work->talk.partnerArrived;
}

void WiFiPlazaWork_IncPartnerTalkCount(WiFiPlazaWork *work) {
    if (WiFiPlazaWork_IsTalking(work) == TRUE) {
        WiFiPlazaTalkSession_IncTalkCount(&work->talk, work->talk.partnerSlot);
    }
}

void WiFiPlazaWork_IncTalkCount(WiFiPlazaWork *work, int slot) {
    WiFiPlazaTalkSession_IncTalkCount(&work->talk, slot);
}

void WiFiPlazaWork_SetTalkedWith(WiFiPlazaWork *work, int slot) {
    WiFiPlazaTalkSession_SetTalkedWith(&work->talk, slot);
}

void IncrementGameStat47(WiFiPlazaWork *work) {
    GameStats_Inc(Save_GameStats_Get(work->save), GAME_STAT_UNK47);
}

void IncrementGameStat119(WiFiPlazaWork *work) {
    GameStats_Inc(Save_GameStats_Get(work->save), GAME_STAT_UNK119);
}

void WiFiPlazaWork_MarkActivityDone(WiFiPlazaWork *work, int slot) {
    if (slot >= 8) {
        GF_AssertFail();
    }
    work->sessionState.flags[slot] = 1;
}

int WiFiPlazaWork_GetNextUndoneActivity(WiFiPlazaWork *work) {
    int i;
    int ok;
    for (i = 0; i < 8; i++) {
        if (work->sessionState.flags[i] == 0) {
            switch (i) {
            case 0:
            case 1:
            case 2:
                if (WiFiPlazaWork_GetEventScheduleMode5(work) == 1) {
                    ok = 0;
                } else {
                    ok = 1;
                }
                break;
            case 3:
            case 4:
            case 5:
                if (WiFiPlazaWork_IsClosing(work) == 1) {
                    ok = 0;
                } else {
                    ok = 1;
                }
                break;
            case 6:
                if (WiFiPlazaWork_IsLightingStarted(work) == 1) {
                    ok = 0;
                } else {
                    ok = 1;
                }
                break;
            case 7:
            default:
                ok = 1;
                break;
            }
            if (ok == 1) {
                return i;
            }
        }
    }
    return 8;
}

int WiFiPlazaWork_PickTapToy(WiFiPlazaWork *work) {
    int result = WiFiPlazaGroupWeights_PickGroup(&work->groupWeights);
    int value = WiFiPlazaWork_GetOwnSlot(work);
    if (WiFiPlazaWork_IsSpecialVisitor(work, value) == 1) {
        result += 2;
    }
    return result;
}

u32 WiFiPlazaWork_GetFireworksPhase(WiFiPlazaWork *work) {
    return WiFiPlazaTimer_GetPhase(&work->timer);
}

u32 WiFiPlazaWork_GetFireworksCounter(WiFiPlazaWork *work) {
    return WiFiPlazaTimer_GetCounter(&work->timer);
}

void WiFiPlazaWork_SetSurveyTally(WiFiPlazaWork *work, const WiFiPlazaTallyCache *tally) {
    work->ownProfileData.hasTally = TRUE;
    work->ownProfileData.tally = *tally;
}

BOOL WiFiPlazaWork_GetSurveyTally(WiFiPlazaWork *work, WiFiPlazaTallyCache *tally) {
    if (work->ownProfileData.hasTally == TRUE) {
        *tally = work->ownProfileData.tally;
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaWork_NewPlayerInfo(WiFiPlazaWork *work) {
    if (work->timer.playerInfo != NULL) {
        GF_AssertFail();
    }
    work->timer.playerInfo = WiFiPlazaPlayerInfo_New(Save_PlayerData_GetProfile(work->save), work->heapID);
}

void WiFiPlazaWork_FreePlayerInfo(WiFiPlazaWork *work) {
    if (work->timer.playerInfo != NULL) {
        WiFiPlazaPlayerInfo_Free(work->timer.playerInfo);
        work->timer.playerInfo = NULL;
    }
}

// TRUE if the name holds a terminator (0xFFFF) within count characters; the heap ID is not used.
static BOOL WiFiPlaza_IsNameTerminated(const u16 *name, u32 count, enum HeapID heapID) {
    u32 i;
    for (i = 0; i < count; i++) {
        if (name[i] == 0xffff) {
            return TRUE;
        }
    }
    return FALSE;
}

// Lobby callback: a player entered the plaza (isOwn: the own player, whose profile is synchronised).
static void WiFiPlaza_OnPlayerEnter(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work, BOOL isOwn) {
    s64 time;
    int country;
    int region;
    int slot;
    int index;
    int valid;
    if (isOwn) {
        WiFiPlazaProfile_CopyToPlayerProfile(&work->ownProfileData.profile, &work->profile, work->heapID);
        if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
            work->hasError = 1;
        }
        WiFiPlazaNews_AddPlayer(work->news, WiFiPlazaLobbyClient_IdToSlot(id), work->profile.player.gender, 3);
        work->profile.player.lobbyId = id;
        WiFiPlazaLobbyClient_GetEntryTime(id, &time);
        WiFiPlaza_SecondsToTime(&work->profile.player.entryTime, &time);
        country = WiFiPlazaPlayer_GetCountry(&work->profile.player);
        region = WiFiPlazaPlayer_GetRegion(&work->profile.player);
        valid = WiFiPlazaPlayer_IsLanguageValid(&work->profile.player);
        if (country && valid == TRUE) {
            *(WiFiPlazaTime *)&work->profile.player.localTime = WiFiPlaza_GetLocalTime((u16)country, (u8)region, work->profile.player.entryTime, work->heapID);
        } else {
            ((u8 *)&work->profile.player.localTime)[0] = 12;
            ((u8 *)&work->profile.player.localTime)[1] = 0;
            ((u8 *)&work->profile.player.localTime)[2] = 0;
        }
        WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
        WiFiPlazaWork_SendOwnRecord(work);
    } else {
        country = WiFiPlazaLobbyClient_GetOldestPlayer();
        region = WiFiPlazaLobbyClient_IdToSlot(id);
        WiFiPlazaNews_AddPlayer(work->news, region, WiFiPlazaPlayer_GetGender(player), 0);
        if (country == id) {
            WiFiPlazaNews_SetOldestPlayer(work->news, region);
        }
        if (WiFiPlazaHistory_ReplaceValue(&work->history, id, -1)) {
            WiFiPlazaWork_SendOwnRecord(work);
        }
    }
    index = WiFiPlazaLobbyClient_IdToSlot(id);
    work->changeFlags[0] |= 1 << index;
    if (WiFiPlazaLobbyClient_IsSpecialVisitor(id) == TRUE) {
        WiFiPlaza_SetSlotFlag(work->specialVisitors, index, 1);
        slot = index;
        WiFiPlazaNews_AddSpecialVisitorEvent(work->news, &slot, work->specialVisitors);
        country = WiFiPlazaLobbyClient_GetSpecialVisitorCode(id);
        if (country) {
            WiFiPlazaWordSet_Set(&work->wordSet, index, 1, country);
        } else {
            WiFiPlazaWordSet_Set(&work->wordSet, index, 0, country);
        }
    } else {
        WiFiPlaza_SetSlotFlag(work->specialVisitors, index, 0);
        WiFiPlazaWordSet_Set(&work->wordSet, index, 0, 0);
    }
}

// Lobby callback: a player left the plaza.
static void WiFiPlaza_OnPlayerLeave(int id, WiFiPlazaWork *work) {
    int slot;
    if (id == WiFiPlazaLobbyClient_GetOwnId()) {
        return;
    }
    slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (WiFiPlazaWork_IsTalking(work) == TRUE && slot == WiFiPlazaWork_GetTalkPartner(work)) {
        WiFiPlazaTalkSession_Abort(&work->talk);
    }
    if (WiFiPlazaNews_RemovePlayer(work->news, slot) != 0) {
        WiFiPlazaNews_SetOldestPlayer(work->news, WiFiPlazaLobbyClient_IdToSlot(WiFiPlazaLobbyClient_GetOldestOtherPlayer(id)));
    }
    WiFiPlaza_SetSlotFlag(work->specialVisitors, slot, 0);
    WiFiPlazaTalkSession_ResetTalkCount(&work->talk, slot);
    WiFiPlazaTalkSession_ClearTalkedWith(&work->talk, slot);
    if (WiFiPlazaHistory_ReplaceValue(&work->history, id, -1) != 0) {
        WiFiPlazaWork_SendOwnRecord(work);
    }
    WiFiPlazaWordSet_Set(&work->wordSet, slot, 0, 0);
    work->changeFlags[1] |= 1 << WiFiPlazaLobbyClient_IdToSlot(id);
}

// Lobby callback: the record of a player changed.
static void WiFiPlaza_OnPlayerUpdate(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work) {
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    work->changeFlags[3] |= 1 << slot;
    switch (WiFiPlazaPlayer_GetActivity(player)) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        WiFiPlazaTalkSession_ResetTalkCount(&work->talk, slot);
        break;
    }
    if (WiFiPlazaPlayer_HasTapToy(player) == TRUE) {
        WiFiPlazaGroupWeights_ReportPlace(&work->groupWeights, WiFiPlazaPlayer_GetTapToy(player), slot);
    }
}

// Lobby callback: a lobby event (0: the lobby opened); most of them start a timer in WiFiPlazaWork.eventSchedule.
static void WiFiPlaza_OnLobbyEvent(u32 event, WiFiPlazaWork *work) {
    WiFiPlazaLobbyTime now;
    int notify = 0;
    int which;
    PPW_LobbyGetTimeInfo(&now);
    WiFiPlaza_SecondsToTime(&work->currentTime, &now.time);
    work->lobbyTime = now.time;
    switch (event) {
    case 0:
        WiFiPlazaNews_SetOpenTime(work->news, &work->currentTime);
        work->lobbyOpenTime = work->lobbyTime;
        work->eventSchedule.flag0 = 1;
        work->changeFlags[2] |= 1;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (!work->lightingStarted) {
            work->lightingStarted = 1;
            notify = 1;
            which = 0;
            work->eventSchedule.lightingDelay = 300;
        }
    case 1:
        work->eventSchedule.colorEvent = event;
        work->changeFlags[2] |= 2;
        break;
    case 8:
    case 9:
    case 10:
        if (!work->lightingStarted) {
            work->lightingStarted = 1;
            notify = 1;
            which = 0;
            work->eventSchedule.lightingDelay = 300;
        }
    case 7:
        work->eventSchedule.light1Event = event;
        work->changeFlags[2] |= 4;
        break;
    case 12:
    case 13:
    case 14:
        if (!work->lightingStarted) {
            work->lightingStarted = 1;
            notify = 1;
            which = 0;
            work->eventSchedule.lightingDelay = 300;
        }
    case 11:
        work->eventSchedule.thirdEvent = event;
        work->changeFlags[2] |= 8;
        break;
    case 16: {
        int a;
        int b;
        notify = 1;
        which = 1;
        a = WiFiPlazaLobbyClient_GetScheduleValue(6);
        b = WiFiPlazaLobbyClient_GetScheduleValue(7);
        WiFiPlazaTimer_Init(&work->timer, a, b);
        work->eventSchedule.fireworksDelay = 300;
    } break;
    case 17:
        work->eventSchedule.mode2 = 2;
        work->changeFlags[2] |= 32;
        break;
    case 18:
        notify = 1;
        which = 2;
        work->eventSchedule.paradeDelay = 300;
        break;
    case 19:
        notify = 1;
        which = 4;
        work->eventSchedule.flag7 = 1;
        work->eventSchedule.closeTimer = 900;
        break;
    case 15:
        work->eventSchedule.mode5 = 1;
        work->changeFlags[2] |= 16;
        break;
    }
    if (notify) {
        WiFiPlazaWork_AddLobbyEventNotice(work, which);
    }
}

// Lobby callback: the own record as the lobby stores it.
static void WiFiPlaza_OnOwnRecord(const WiFiPlazaPlayer *data, u32 size, WiFiPlazaWork *work) {
    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->hasError = 1;
        return;
    }
    WiFiPlazaProfile_SetFromLobbyRecord(&work->profile, data, size);
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
}

// Fills out with the country and region of the own player, of every player in the lobby and of the 20 lobby
// location entries.
static void WiFiPlaza_FillPlaces(WiFiPlazaProfile *profile, WiFiPlazaPlace *out) {
    WiFiPlazaPlayerList list;
    int country;
    int region;
    int i;
    WiFiPlazaPlayer *player;
    memset(out, 0, 200);
    WiFiPlazaLobbyClient_GetPlayerList(&list);
    player = &profile->player;
    country = WiFiPlazaPlayer_GetCountry(player);
    region = WiFiPlazaPlayer_GetRegion(player);
    WiFiPlazaPlaces_Add(out, (u16)country, (u8)region, 0);
    for (i = 0; i < 20; i++) {
        if (list.ids[i] != -1) {
            player = WiFiPlazaLobbyClient_GetPlayer(list.ids[i]);
            country = WiFiPlazaPlayer_GetCountry(player);
            region = WiFiPlazaPlayer_GetRegion(player);
            WiFiPlazaPlaces_Add(out, (u16)country, (u8)region, 0);
        }
    }
    for (i = 0; i < 20; i++) {
        u8 entryCountry = (u8)WiFiPlazaLobbyClient_GetLocationCountry((u8)i);
        WiFiPlazaPlaces_Add(out, entryCountry, WiFiPlazaLobbyClient_GetLocationRegion((u8)i), 1);
    }
}

static void WiFiPlazaWork_CheckClosingTime(WiFiPlazaWork *work) {
    if (work->eventSchedule.flag0 == 1 && work->eventSchedule.flag7 == 0 && WiFiPlazaLobbyClient_GetState() == 2) {
        s64 duration = (u32)WiFiPlazaWork_GetOpenDuration(work);
        if (duration + work->lobbyOpenTime < work->lobbyTime) {
            work->eventSchedule.flag7 = 1;
            work->eventSchedule.closeTimer = 900;
        }
    }
}

static void WiFiPlazaProfile_InitFromSave(WiFiPlazaProfile *profile, SaveData *save, enum HeapID heapID) {
    PlayerProfile *playerProfile;
    Party *party;
    SaveWiFiHistory *history;
    Pokedex *pokedex;
    SysInfo_RTC *rtc;
    String *name;
    int count;
    int i;
    int j;
    playerProfile = Save_PlayerData_GetProfile(save);
    party = SaveArray_Party_Get(save);
    pokedex = Save_Pokedex_Get(save);
    history = Save_WiFiHistory_Get(save);
    rtc = Save_SysInfo_RTC_Get(save);
    name = PlayerProfile_GetPlayerName_NewString(playerProfile, heapID);
    CopyStringToU16Array(name, profile->player.name, 8);
    CopyStringToU16Array(name, profile->name, 8);
    String_Delete(name);
    profile->player.lobbyId = -1;
    profile->player.trainerId = PlayerProfile_GetTrainerID(playerProfile);
    count = Party_GetCount(party);
    for (i = 0; i < PARTY_SIZE; i++) {
        if (i < count) {
            Pokemon *mon = Party_GetMonByIndex(party, i);
            profile->player.species[i] = GetMonData(mon, MON_DATA_SPECIES, NULL);
            profile->player.forms[i] = GetMonData(mon, MON_DATA_FORM, NULL);
            profile->player.isEgg[i] = GetMonData(mon, MON_DATA_IS_EGG, NULL);
        } else {
            profile->player.species[i] = SPECIES_BAD_EGG; // empty party slot
        }
    }
    profile->player.gender = PlayerProfile_GetTrainerGender(playerProfile);
    profile->player.language = PlayerProfile_GetLanguage(playerProfile);
    profile->player.avatarIndex = PlayerProfile_GetAvatar(playerProfile);
    profile->player.avatarIndex = WiFiPlaza_AvatarToIndex(profile->player.avatarIndex);
    profile->player.country = WifiHistory_GetPlayerCountry(history);
    profile->player.region = WiFiHistory_GetPlayerRegion(history);
    profile->player.hasNationalDex = Pokedex_GetNatDexFlag(pokedex);
    profile->player.gameCleared = PlayerProfile_GetGameClearFlag(playerProfile);
    profile->player.tapToy = 0xff;
    profile->player.activity = 0;
    profile->player.version = GAME_VERSION;
    profile->player.unk_44 = rtc->seconds_since_nitro_epoch;
    for (i = 0; i < 12; i++) {
        profile->player.slots[i] = 24;
        profile->player.slotData[i] = (void *)-1;
    }
    for (j = 0; j < 2; j++) {
        profile->player.favoriteTypes[j] = 0;
    }
    profile->player.survey.surveyId = -1;
    profile->player.survey.answer = 3;
    WiFiPlazaProfile_UpdateChecksum(profile, save);
}

static void WiFiPlazaWork_SendOwnRecord(WiFiPlazaWork *work) {
    int i;
    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->hasError = 1;
        return;
    }
    for (i = 0; i < 12; i++) {
        work->profile.player.slots[i] = WiFiPlazaHistory_GetId(&work->history, i);
        work->profile.player.slotData[i] = (void *)WiFiPlazaHistory_GetValue(&work->history, i);
    }
    MI_CpuCopy8(work->profile.lobbyName, work->profile.player.name, sizeof(work->profile.player.name));
    WiFiPlazaLobbyClient_SetOwnPlayer(&work->profile.player);
    MI_CpuCopy8(work->profile.name, work->profile.player.name, sizeof(work->profile.player.name));
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
}

static void WiFiPlazaProfile_UpdateChecksum(WiFiPlazaProfile *profile, SaveData *save) {
    profile->crc = SaveArray_CalcCRC16(save, &profile->player, sizeof(WiFiPlazaPlayer));
}

static BOOL WiFiPlazaProfile_IsChecksumValid(WiFiPlazaProfile *profile, SaveData *save) {
    u16 crc = SaveArray_CalcCRC16(save, &profile->player, sizeof(WiFiPlazaPlayer));
    if (crc != profile->crc) {
        GF_AssertFail();
        return FALSE;
    }
    return TRUE;
}

static void WiFiPlazaProfile_SetFromLobbyRecord(WiFiPlazaProfile *profile, const WiFiPlazaPlayer *src, u32 size) {
    profile->player = *src;
    if (size <= sizeof(WiFiPlazaPlayer)) {
        MI_CpuCopy8(src, &profile->player, size);
    } else {
        MI_CpuCopy8(src, &profile->player, sizeof(WiFiPlazaPlayer));
    }
    MI_CpuCopy8(src->name, profile->lobbyName, sizeof(profile->lobbyName));
    MI_CpuCopy8(profile->name, profile->player.name, sizeof(profile->player.name));
}

static void WiFiPlaza_ClearEventFlags(u32 *flags) {
    flags[2] = 0;
}

// Counts down the four lobby timers of state; each one raises a flag in flags[2] when it runs out.
static void WiFiPlazaEventSchedule_Update(WiFiPlazaEventSchedule *state, u32 *flags, WiFiPlazaGroups *groups, WiFiPlazaTimer *timer) {
    if (state->closeTimer - 1 >= 0) {
        state->closeTimer--;
        if (state->closeTimer == 840) {
            GF_SndStartFadeOutBGM(0, 127);
            state->bgmFadedOut = 1;
        }
    } else if (state->closeTimer == 0) {
        state->flag1 = 1;
        flags[2] |= 0x80;
        state->closeTimer--;
    }

    if (state->lightingDelay - 1 >= 0) {
        state->lightingDelay--;
    } else if (state->lightingDelay == 0) {
        flags[2] |= 4;
        flags[2] |= 8;
        flags[2] |= 2;
        state->lightingDelay--;
    }

    if (state->fireworksDelay - 1 >= 0) {
        state->fireworksDelay--;
    } else if (state->fireworksDelay == 0) {
        state->mode2 = 1;
        flags[2] |= 0x20;
        state->fireworksDelay--;
        WiFiPlazaTimer_Start(timer);
    }

    if (state->paradeDelay - 1 >= 0) {
        state->paradeDelay--;
    } else if (state->paradeDelay == 0) {
        WiFiPlazaGroups_Start(groups);
        if (state->minigameActive == 0) {
            PlayBGM(SEQ_GS_WIFIPARADE);
        }
        state->flag4 = 1;
        flags[2] |= 0x40;
        state->paradeDelay--;
    }
}

static void WiFiPlazaEventSchedule_Init(WiFiPlazaEventSchedule *state) {
    state->flag0 = 0;
    state->flag1 = 0;
    state->mode2 = 0;
    state->flag4 = 0;
    state->mode5 = 0;
    state->flag7 = 0;
    state->colorEvent = 1;
    state->light1Event = 7;
    state->thirdEvent = 11;
    state->closeTimer = -1;
    state->lightingDelay = -1;
    state->fireworksDelay = -1;
    state->paradeDelay = -1;
}

static void WiFiPlazaEventSchedule_ApplyVolume(WiFiPlazaEventSchedule *state) {
    if (state->lowVolume == 1) {
        GF_SndHandleSetPlayerVolume(7, 42);
    } else {
        GF_SndHandleSetPlayerVolume(7, 127);
    }
}

static void WiFiPlazaSessionState_StartCooldown(WiFiPlazaSessionState *state) {
    state->timer = 900;
}

static int WiFiPlazaSessionState_IsCoolingDown(WiFiPlazaSessionState *state) {
    return state->timer > 0;
}

static void WiFiPlazaSessionState_UpdateCooldown(WiFiPlazaSessionState *state) {
    if (state->timer > 0) {
        state->timer--;
    }
}

static void WiFiPlaza_NewPlayerProfile(PlayerProfile **profile, enum HeapID heapID) {
    *profile = PlayerProfile_New(heapID);
}

static void WiFiPlaza_FreePlayerProfile(PlayerProfile **profile) {
    Heap_Free(*profile);
}

// Copies the own lobby record into *profile (with the name the lobby has).
static void WiFiPlazaProfile_CopyToPlayerProfile(PlayerProfile **profile, WiFiPlazaProfile *src, enum HeapID heapID) {
    WiFiPlazaPlayer *player = Heap_Alloc(heapID, sizeof(WiFiPlazaPlayer));
    MIi_CpuCopyFast((const u32 *)&src->player, (u32 *)player, sizeof(WiFiPlazaPlayer));
    MI_CpuCopy8(src->lobbyName, player->name, sizeof(player->name));
    WiFiPlazaPlayer_CopyToProfile(player, *profile, heapID);
    Heap_Free(player);
}

static void WiFiPlazaSessionState_SetAppQuitForClosing(WiFiPlazaSessionState *state) {
    state->appQuitForClosing = 1;
}

static u8 WiFiPlazaSessionState_DidAppQuitForClosing(WiFiPlazaSessionState *state) {
    return state->appQuitForClosing;
}

static void WiFiPlaza_ClearTapToyPlayedFlags(u8 *buffer) {
    u32 count = 20;
    do {
        *buffer++ = 0;
        count--;
    } while (count != 0);
}

static void WiFiPlazaTalkSession_Init(WiFiPlazaTalkSession *session) {
    memset(session, 0, sizeof(WiFiPlazaTalkSession));
}

static void WiFiPlazaTalkSession_Reset(WiFiPlazaTalkSession *session) {
    session->partnerSlot = 0;
    session->state = WIFI_PLAZA_TALK_STATE_NONE;
    session->role = WIFI_PLAZA_TALK_ROLE_NONE;
    WiFiPlazaTalkSession_StopTimeout(session);
}

static void WiFiPlazaTalkSession_Abort(WiFiPlazaTalkSession *session) {
    session->partnerSlot = 0;
    session->state = WIFI_PLAZA_TALK_STATE_ENDED;
    WiFiPlazaTalkSession_StopTimeout(session);
    session->role = WIFI_PLAZA_TALK_ROLE_NONE;
}

static BOOL WiFiPlazaTalkSession_HasTalkedWith(WiFiPlazaTalkSession *session, u32 slot) {
    GF_ASSERT(slot < 20);
    return (session->talkedWith & (1 << slot)) != 0;
}

static void WiFiPlazaTalkSession_SetTalkedWith(WiFiPlazaTalkSession *session, u32 slot) {
    GF_ASSERT(slot < 20);
    session->talkedWith |= 1 << slot;
}

static void WiFiPlazaTalkSession_ClearTalkedWith(WiFiPlazaTalkSession *session, u32 slot) {
    GF_ASSERT(slot < 20);
    session->talkedWith &= ~(1 << slot);
}

static void WiFiPlazaTalkSession_IncTalkCount(WiFiPlazaTalkSession *session, u32 slot) {
    int value;

    GF_ASSERT(slot < 20);
    value = session->talkCounts[slot] + 1;
    if (value <= 6) {
        session->talkCounts[slot] = value;
    }
}

static void WiFiPlazaTalkSession_ResetTalkCount(WiFiPlazaTalkSession *session, u32 slot) {
    GF_ASSERT(slot < 20);
    session->talkCounts[slot] = 0;
}

static void WiFiPlazaTalkSession_SetMessage(WiFiPlazaTalkSession *session, int value) {
    session->sendState = session->state;
    session->sendMessage = value;
}

static void WiFiPlazaWork_SendTalkAnswer(WiFiPlazaWork *work, int id, u16 value) {
    work->talk.sendState = WIFI_PLAZA_TALK_STATE_ASKER_TURN;
    work->talk.sendMessage = value;
    WiFiPlazaLobbyClient_SendToPlayer(1, id, &work->talk.sendMessage, 4);
}

static void WiFiPlazaWork_SendTalkEnd(WiFiPlazaWork *work, int id) {
    work->talk.state = WIFI_PLAZA_TALK_STATE_ENDED;
    work->talk.sendState = WIFI_PLAZA_TALK_STATE_ENDED;
    WiFiPlazaLobbyClient_SendToPlayer(2, id, &work->talk.sendMessage, 4);
}

static void WiFiPlazaTalkSession_StartTimeout(WiFiPlazaTalkSession *session) {
    session->timeoutActive = 1;
    session->timeoutCounter = 0;
}

static void WiFiPlazaTalkSession_StopTimeout(WiFiPlazaTalkSession *session) {
    session->timeoutActive = 0;
    session->timeoutCounter = 0;
}

// Counts timeoutCounter up to 900 while timeoutActive is set.
static void WiFiPlazaTalkSession_UpdateTimeout(WiFiPlazaTalkSession *session) {
    if (session->timeoutActive == 1) {
        int value = session->timeoutCounter + 1;
        if (value <= 900) {
            session->timeoutCounter = value;
        }
    }
}

static BOOL WiFiPlazaTalkSession_IsWithinTimeout(WiFiPlazaTalkSession *session) {
    if (session->timeoutActive == 0) {
        return TRUE;
    }
    return session->timeoutCounter < 900;
}

static s16 WiFiPlazaTalkSession_GetTimeoutCounter(WiFiPlazaTalkSession *session) {
    return session->timeoutCounter;
}

// Lobby message type 0: two players.
static void WiFiPlaza_HandleChatNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaPairRequest req;
    int i;
    int slot;
    if (msg->count != 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
        if (slot == -1) {
            return;
        }
        WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
    }
    req.profiles[0] = work->profiles[0];
    req.profiles[1] = work->profiles[1];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    WiFiPlazaNews_AddChatEvent(work->news, &req);
}

// Lobby message type 1: two players and an option.
static void WiFiPlaza_HandleTapToyTradeNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaPairOptionRequest req;
    int i;
    int slot;
    if (msg->count != 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
        if (slot == -1) {
            return;
        }
        WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
    }
    req.profiles[0] = work->profiles[0];
    req.profiles[1] = work->profiles[1];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.option = msg->tapToy;
    WiFiPlazaNews_AddTapToyTradeEvent(work->news, &req);
}

// Up to four players; without the flag only the first one gets a profile.
static void WiFiPlaza_HandleMinigameEntryNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupFlagRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    if ((msg->allPlayers) == 1) {
        for (i = 0; i < 4; i++) {
            if (i < msg->count) {
                slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
                if (slot == -1) {
                    return;
                }
                WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
                profiles[i] = work->profiles[i];
            } else {
                profiles[i] = NULL;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (i == 0) {
                slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
                if (slot == -1) {
                    return;
                }
                WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
                profiles[i] = work->profiles[i];
            } else {
                profiles[i] = NULL;
            }
        }
    }
    req.option = msg->option;
    req.count = msg->count;
    req.profiles[0] = profiles[0];
    req.profiles[1] = profiles[1];
    req.profiles[2] = profiles[2];
    req.profiles[3] = profiles[3];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.slots[2] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[2]);
    req.slots[3] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[3]);
    req.flag = msg->allPlayers;
    WiFiPlazaNews_AddMinigameEntryEvent(work->news, &req);
}

// Up to eight players; only the first one gets a profile.
static void WiFiPlaza_HandleFootprintStampNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) {
    WiFiPlazaSingleRequest req;
    int slot;
    if (msg->count > 8 || msg->count == 0) {
        return;
    }
    slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    if (slot == -1) {
        return;
    }
    WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[0], work->heapID);
    req.option = msg->option;
    req.count = msg->count;
    req.profile = work->profiles[0];
    req.slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    WiFiPlazaNews_AddFootprintStampEvent(work->news, &req);
}

static void WiFiPlaza_HandleVisitorProfilesNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
            if (slot == -1) {
                return;
            }
            WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
            profiles[i] = work->profiles[i];
        } else {
            profiles[i] = NULL;
        }
    }
    req.count = msg->count;
    req.profiles[0] = profiles[0];
    req.profiles[1] = profiles[1];
    req.profiles[2] = profiles[2];
    req.profiles[3] = profiles[3];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.slots[2] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[2]);
    req.slots[3] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[3]);
    WiFiPlazaNews_AddVisitorProfilesEvent(work->news, &req);
}

static void WiFiPlaza_HandlePlazaNewsNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
            if (slot == -1) {
                return;
            }
            WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
            profiles[i] = work->profiles[i];
        } else {
            profiles[i] = NULL;
        }
    }
    req.count = msg->count;
    req.profiles[0] = profiles[0];
    req.profiles[1] = profiles[1];
    req.profiles[2] = profiles[2];
    req.profiles[3] = profiles[3];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.slots[2] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[2]);
    req.slots[3] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[3]);
    WiFiPlazaNews_AddPlazaNewsEvent(work->news, &req);
}

static void WiFiPlaza_HandleMinigameResultNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) {
    PlayerProfile *profiles[4];
    WiFiPlazaGroupOptionRequest req;
    int i;
    int slot;
    if (msg->count > 4 || msg->count == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (i < msg->count) {
            slot = WiFiPlazaLobbyClient_IdToSlot(msg->ids[i]);
            if (slot == -1) {
                return;
            }
            WiFiPlazaPlayer_CopyToProfile(WiFiPlazaWork_GetPlayer(work, slot), work->profiles[i], work->heapID);
            profiles[i] = work->profiles[i];
        } else {
            profiles[i] = NULL;
        }
    }
    req.option = msg->option;
    req.count = msg->count;
    req.profiles[0] = profiles[0];
    req.profiles[1] = profiles[1];
    req.profiles[2] = profiles[2];
    req.profiles[3] = profiles[3];
    req.slots[0] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[0]);
    req.slots[1] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[1]);
    req.slots[2] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[2]);
    req.slots[3] = WiFiPlazaLobbyClient_IdToSlot(msg->ids[3]);
    WiFiPlazaNews_AddMinigameResultEvent(work->news, &req);
}

static void WiFiPlaza_HandleUnusedNews(WiFiPlazaWork *work, const WiFiPlazaLobbyMessage *msg) {
}

static void WiFiPlazaWork_AddLobbyEventNotice(WiFiPlazaWork *work, int which) {
    WiFiPlazaLobbyNotice notice;
    notice.time = &work->currentTime;
    notice.which = which;
    WiFiPlazaNews_AddLobbyEventNotice(work->news, &notice);
}

static void WiFiPlazaGroups_Init(WiFiPlazaGroups *groups) {
    memset(groups, 0, sizeof(WiFiPlazaGroups));
    MIi_CpuClear32(0xFFFFFFFF, (u32 *)groups->ids, sizeof(groups->ids));
}

static void WiFiPlazaGroups_Start(WiFiPlazaGroups *groups) {
    groups->active = 1;
}

// Advances the plaza time; group i starts counting (state 1) once the time reaches sWiFiPlazaGroupStartTimes[i] and ends (state 2)
// after 1200 more updates.
static void WiFiPlazaGroups_Update(WiFiPlazaGroups *groups) {
    int i;
    if (groups->active != 0) {
        for (i = 0; i < 9; i++) {
            if (groups->time == sWiFiPlazaGroupStartTimes[i]) {
                groups->states[i] = 1;
            }
            if (groups->states[i] == 1) {
                if (groups->counters[i] + 1 <= 1200) {
                    groups->counters[i]++;
                } else {
                    groups->states[i] = 2;
                }
            }
        }
        if (groups->time + 1 <= 4320) {
            groups->time++;
        }
    }
}

// Puts id into the first free place of a group that has not ended and whose markerCounts count allows value; returns the
// place (group * 3 + seat) or -1.
static int WiFiPlazaGroups_Join(WiFiPlazaGroups *groups, u32 id, u32 value) {
    int seat;
    int base;
    int group;
    if (WiFiPlazaGroups_FindPlace(groups, (s32)id) == -1) {
        base = 0;
        group = 0;
        for (; group < 9; group++, base += 3) {
            if (WiFiPlazaGroups_GetState(groups, group) != 2) {
                for (seat = 0; seat < 3; seat++) {
                    if (WiFiPlazaGroups_HasPassedMarker(groups, seat + base, value) == 0 && WiFiPlazaGroups_GetPlaceId(groups, seat + base) == -1) {
                        groups->ids[group][seat] = id;
                        return seat + base;
                    }
                }
            }
        }
    }
    return -1;
}

static void WiFiPlazaGroups_Leave(WiFiPlazaGroups *groups, u32 id) {
    int place = WiFiPlazaGroups_FindPlace(groups, id);
    if (place != -1) {
        u32 group = (u32)place / 3, seat = (u32)place % 3;
        groups->ids[group][seat] = (u32)-1;
    }
}

static int WiFiPlazaGroups_GetPlaceId(WiFiPlazaGroups *groups, u32 place) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->ids[group][seat];
    return *slot;
}

static int WiFiPlazaGroups_GetState(WiFiPlazaGroups *groups, u32 group) {
    if (group >= 9) {
        GF_AssertFail();
    }
    return groups->states[group];
}

static int WiFiPlazaGroups_GetCounter(WiFiPlazaGroups *groups, u32 group) {
    if (group >= 9) {
        GF_AssertFail();
    }
    return groups->counters[group];
}

static void WiFiPlazaGroups_RequestAction(WiFiPlazaGroups *groups, u32 id) {
    u32 place = WiFiPlazaGroups_FindPlace(groups, id);
    if (place != (u32)-1) {
        groups->actionRequests[place / 3][place % 3] = 1;
    }
}

static int WiFiPlazaGroups_GetActionRequest(WiFiPlazaGroups *groups, u32 place) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->actionRequests[group][seat];
    return *slot;
}

static void WiFiPlazaGroups_ClearActionRequest(WiFiPlazaGroups *groups, u32 place) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->actionRequests[group][seat];
    *slot = 0;
}

// The place (group * 3 + seat) of id, or -1.
static int WiFiPlazaGroups_FindPlace(WiFiPlazaGroups *groups, int id) {
    int i;
    int j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 3; j++) {
            if (id == groups->ids[i][j]) {
                return i * 3 + j;
            }
        }
    }
    return -1;
}

static void WiFiPlazaGroups_SetMarkerPassed(WiFiPlazaGroups *groups, u32 place, u32 value) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->markerCounts[group][seat];
    *slot = value + 1;
}

static BOOL WiFiPlazaGroups_HasPassedMarker(WiFiPlazaGroups *groups, u32 place, u32 value) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->markerCounts[group][seat];
    if (*slot <= value) {
        return FALSE;
    }
    return TRUE;
}

static u32 WiFiPlazaGroups_GetMarkerCount(WiFiPlazaGroups *groups, u32 place) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->markerCounts[group][seat];
    return *slot;
}

// Receive callbacks of the exchange with one other player:
// id is the sender, data the received words.

// Message 0: the other player asks for an exchange; answered with WiFiPlazaWork_SendTalkAnswer (TRUE = accepted).
static void WiFiPlaza_OnTalkRequest(int id, const void *data, u32 size, void *arg) {
    WiFiPlazaWork *work = arg;
    const u16 *words = data;
    u32 slot = WiFiPlazaLobbyClient_IdToSlot(id);
    BOOL ready;
    BOOL busy;

    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->hasError = 1;
    }
    ready = WiFiPlazaPlayer_GetActivity(&work->profile.player);
    busy = WiFiPlazaWork_IsTalking(work);
    if (ready != TRUE || slot == (u32)-1 || words[1] != 1 || busy == TRUE || WiFiPlazaWork_CanTalkWith(work, slot) == FALSE) {
        WiFiPlazaWork_SendTalkAnswer(work, id, FALSE);
    } else {
        WiFiPlazaWork_SendTalkAnswer(work, id, TRUE);
        if (WiFiPlazaTalkSession_HasTalkedWith(&work->talk, slot) == FALSE) {
            work->talk.isFirstTalk = TRUE;
        } else {
            work->talk.isFirstTalk = FALSE;
        }
        work->talk.partnerSlot = slot;
        work->talk.state = WIFI_PLAZA_TALK_STATE_ASKER_TURN;
        work->talk.role = WIFI_PLAZA_TALK_ROLE_ANSWERER;
        work->talk.partnerArrived = 0;
        WiFiPlazaTalkSession_StartTimeout(&work->talk);
    }
}

// Message 1: the answer of the player asked.
static void WiFiPlaza_OnTalkAnswer(int id, const void *data, u32 size, void *arg) {
    WiFiPlazaWork *work = arg;
    const u16 *words = data;
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (work->talk.role != WIFI_PLAZA_TALK_ROLE_NONE && work->talk.partnerSlot == slot) {
        if (*words == 1) {
            if (WiFiPlazaTalkSession_HasTalkedWith(&work->talk, slot) == FALSE) {
                work->talk.isFirstTalk = TRUE;
            } else {
                work->talk.isFirstTalk = FALSE;
            }
            work->talk.state = WIFI_PLAZA_TALK_STATE_ASKER_TURN;
        } else {
            WiFiPlazaTalkSession_Reset(&work->talk);
        }
        WiFiPlazaTalkSession_StopTimeout(&work->talk);
    }
}

// Message 2: the state of the other player (2 or 3), or the end of the exchange.
static void WiFiPlaza_OnTalkState(int id, const void *data, u32 size, void *arg) {
    WiFiPlazaWork *work = arg;
    const u16 *words = data;
    int slot;
    if (work->talk.role != WIFI_PLAZA_TALK_ROLE_NONE) {
        slot = WiFiPlazaLobbyClient_IdToSlot(id);
        if (work->talk.partnerSlot == slot) {
            if ((u16)(words[1] + 0xFFFE) <= 1) { // 2 or 3
                work->talk.state = words[1];
                work->talk.partnerMessage = words[0];
                WiFiPlazaTalkSession_StopTimeout(&work->talk);
                return;
            }
            WiFiPlazaTalkSession_Abort(&work->talk);
            return;
        }
        if (slot == -1) {
            WiFiPlazaTalkSession_Abort(&work->talk);
        }
    }
}

static void WiFiPlaza_OnTalkCancel(int id, const void *data, u32 size, void *arg) {
    WiFiPlazaWork *work = arg;
    if (work->talk.state != WIFI_PLAZA_TALK_STATE_NONE) {
        int slot = WiFiPlazaLobbyClient_IdToSlot(id);
        if (work->talk.partnerSlot == slot) {
            WiFiPlazaTalkSession_Reset(&work->talk);
        }
    }
}

// A lobby message: handled by sWiFiPlazaLobbyMessageHandlers[type].
static void WiFiPlaza_OnLobbyMessage(int id, const void *data, u32 size, void *arg) {
    WiFiPlazaWork *work = arg;
    const WiFiPlazaLobbyMessage *msg = data;
    sWiFiPlazaLobbyMessageHandlers[msg->type](work, msg);
}

// Receive callbacks: the sender id marks its lobby slot in WiFiPlazaWork.tapToyPlayed or joins a
// group of WiFiPlazaWork.groups.
static void WiFiPlaza_RecvTapToyPlayed(int id, const void *data, u32 size, void *arg) {
    WiFiPlazaWork *work = arg;
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (slot != -1) {
        work->tapToyPlayed[slot] = 1;
    }
}

static void WiFiPlaza_RecvJoinGroup(int id, const void *data, u32 size, void *arg) {
    WiFiPlazaWork *work = arg;
    int slot = WiFiPlazaLobbyClient_IdToSlot(id), value;
    if (slot != -1) {
        for (value = 0; value < 3; value++) {
            if (WiFiPlazaGroups_Join(&work->groups, slot, value) != -1) {
                break;
            }
        }
    }
}

static void WiFiPlaza_RecvGroupAction(int id, const void *data, u32 size, void *arg) {
    WiFiPlazaWork *work = arg;
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (slot != -1) {
        WiFiPlazaGroups_RequestAction(&work->groups, slot);
    }
}

static void WiFiPlazaGroupWeights_Init(WiFiPlazaGroupWeights *weights) {
    int i;
    memset(weights, 0, sizeof(WiFiPlazaGroupWeights));
    for (i = 0; i < 9; i++) {
        weights->weights[i] = 100;
    }
}

// The first time the player of a lobby slot reports a place, the weight of that place's group is halved.
static void WiFiPlazaGroupWeights_ReportPlace(WiFiPlazaGroupWeights *weights, u32 place, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (place >= 27) {
        GF_AssertFail();
    }
    if (weights->seen[slot] == 0) {
        weights->weights[place / 3] >>= 1;
        weights->seen[slot] = 1;
    }
}

// A random group by weight; returns its first place (group * 3).
static u32 WiFiPlazaGroupWeights_PickGroup(WiFiPlazaGroupWeights *weights) {
    int sum = 0;
    int i = 0;
    u32 target;
    u32 accum;
    while (i < 9) {
        sum += weights->weights[i];
        i++;
    }
    target = MTRandom() % (u32)sum;
    accum = 0;
    i = 0;
    while (i < 9) {
        if (accum <= target && accum + weights->weights[i] > target) {
            return i * 3;
        }
        accum += weights->weights[i];
        i++;
    }
    GF_AssertFail();
    return 0;
}

static void WiFiPlaza_SetSlotFlag(u8 *flags, u32 slot, u8 value) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20) {
        flags[slot] = value;
    }
}

static u32 WiFiPlaza_GetSlotFlag(u8 *flags, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20) {
        return flags[slot];
    }
    return 0;
}

static void WiFiPlazaWordSet_Init(WiFiPlazaWordSet *wordSet, enum HeapID heapID) {
    MI_CpuFill8(wordSet, 0, sizeof(WiFiPlazaWordSet));
    wordSet->bank = WallpaperPasswordBank_Create(heapID);
}

static void WiFiPlazaWordSet_Free(WiFiPlazaWordSet *wordSet) {
    WallpaperPasswordBank_Delete(wordSet->bank);
}

// Sets the flag of a lobby slot; flag 1 also makes its four words from code.
static void WiFiPlazaWordSet_Set(WiFiPlazaWordSet *wordSet, u32 slot, u8 flag, u32 code) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20) {
        wordSet->flags[slot] = flag;
        if (flag == 1) {
            WiFiPlaza_MakeWordsFromCode(wordSet->bank, code, wordSet->words[slot]);
        }
    }
}

static u32 WiFiPlazaWordSet_HasWords(WiFiPlazaWordSet *wordSet, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20) {
        return wordSet->flags[slot];
    }
    return 0;
}

static u16 *WiFiPlazaWordSet_GetWords(WiFiPlazaWordSet *wordSet, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    if (slot < 20 && wordSet->flags[slot]) {
        return wordSet->words[slot];
    }
    return NULL;
}

// Four words of the bank, each picked by the sum of two neighbouring bytes of code.
static void WiFiPlaza_MakeWordsFromCode(WallpaperPasswordBank *bank, u32 code, u16 *words) {
    union {
        u32 word;
        u8 bytes[4];
    } value;

    u32 count;
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    count = WallpaperPasswordBank_GetCount(bank);
    value.word = code;
    a = value.bytes[0];
    d = value.bytes[3];
    words[0] = WallpaperPasswordBank_GetWordAtIndex(bank, (d + a) % count);
    b = value.bytes[1];
    words[1] = WallpaperPasswordBank_GetWordAtIndex(bank, (a + b) % count);
    c = value.bytes[2];
    words[2] = WallpaperPasswordBank_GetWordAtIndex(bank, (b + c) % count);
    words[3] = WallpaperPasswordBank_GetWordAtIndex(bank, (c + d) % count);
}

static void WiFiPlazaTimer_Init(WiFiPlazaTimer *timer, int start, int end) {
    timer->active = 0;
    timer->elapsed = 0;
    timer->duration = (end - start) * 30;
}

static void WiFiPlazaTimer_Start(WiFiPlazaTimer *timer) {
    timer->active = 1;
    timer->elapsed = 0;
    WiFiPlazaLobbyClient_GetTime(&timer->start);
}

// Runs the timer (30 per second of WiFiPlazaLobbyClient_GetTime time) and steps its phase through sWiFiPlazaTimerPhases; phase 5 for 120
// updates at the end.
static void WiFiPlazaTimer_Update(WiFiPlazaTimer *timer) {
    s64 now;
    s64 delta;
    int accum;
    u32 i;
    if (timer->active != 0) {
        WiFiPlazaLobbyClient_GetTime(&now);
        delta = (now - timer->start) * 30;
        if ((s64)timer->elapsed < delta) {
            timer->elapsed = (s32)delta;
        }
        if (timer->elapsed < timer->duration) {
            u32 value;
            timer->elapsed++;
            value = (timer->elapsed << 8) / timer->duration;
            accum = 0;
            for (i = 0; i < 5; i++) {
                accum += sWiFiPlazaTimerPhases[i].length;
                if ((u32)accum >= value) {
                    if (timer->phase != sWiFiPlazaTimerPhases[i].phase) {
                        timer->phase = sWiFiPlazaTimerPhases[i].phase;
                        timer->counter = 0;
                    }
                    break;
                }
            }
        } else {
            if (timer->phase != 5) {
                timer->phase = 5;
                timer->counter = 0;
            }
            if (timer->counter >= 120) {
                timer->phase = 0;
                timer->active = 0;
            }
        }
        timer->counter++;
    }
}

static u32 WiFiPlazaTimer_GetCounter(WiFiPlazaTimer *timer) {
    return timer->counter;
}

static u32 WiFiPlazaTimer_GetPhase(WiFiPlazaTimer *timer) {
    return timer->phase;
}
