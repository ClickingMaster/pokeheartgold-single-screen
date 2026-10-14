#include "global.h"

#include "wifi_plaza_internal.h"

void WiFiPlazaWork_VBlankDummy(WiFiPlazaWork *work) {
}

u32 WiFiPlazaWork_HasError(WiFiPlazaWork *work) {
    return work->hasError;
}

#pragma force_active on

void WiFiPlazaWork_TakeLobbyFlag131(void *a0, void *a1, void *a2) {
    WiFiPlazaLobbyClient_TakeFlag131();
}

#pragma force_active reset

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

int WiFiPlazaWork_GetScheduleValue4(WiFiPlazaWork *work) {
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
    WiFiPlazaUnk3A0_StartCooldown(&work->unk_3a0);
}

int WiFiPlazaWork_IsGameEntryCoolingDown(WiFiPlazaWork *work) {
    return WiFiPlazaUnk3A0_IsCoolingDown(&work->unk_3a0);
}

void *WiFiPlazaWork_GetField508_04(WiFiPlazaWork *work) {
    return work->unk_508.unk_04;
}

void WiFiPlazaWork_SetField508_04(WiFiPlazaWork *work, void *value) {
    work->unk_508.unk_04 = value;
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
    WiFiPlazaUnk3A0_SetAppQuitForClosing(&work->unk_3a0);
}

int WiFiPlazaWork_DidAppQuitForClosing(WiFiPlazaWork *work) {
    return WiFiPlazaUnk3A0_DidAppQuitForClosing(&work->unk_3a0);
}

void WiFiPlazaWork_SetMinigameActiveFlag(WiFiPlazaWork *work, u8 value) {
    work->eventSchedule.unk_0c = value;
}
