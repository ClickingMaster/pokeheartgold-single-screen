#include "global.h"

#include "wifi_plaza_internal.h"

void ov45_0222A1F8(WiFiPlazaWork *work) {
}

u32 WiFiPlazaWork_HasError(WiFiPlazaWork *work) {
    return work->unk_52c;
}

#pragma force_active on

void ov45_0222A208(void *a0, void *a1, void *a2) {
    ov45_0222E9BC();
}

#pragma force_active reset

void *WiFiPlazaWork_GetNews(WiFiPlazaWork *work) {
    return work->news;
}

void *WiFiPlazaWork_GetPlaces(WiFiPlazaWork *work) {
    WiFiPlaza_FillPlaces(&work->profile, work->places);
    return work->places;
}

u32 *ov45_0222A22C(WiFiPlazaWork *work) {
    return &work->unk_d4;
}

BOOL WiFiPlazaWork_TakePlayerEntered(WiFiPlazaWork *work, int bit) {
    u32 mask = 1 << bit;
    if (work->unk_f8[0] & mask) {
        work->unk_f8[0] &= ~mask;
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaWork_TakePlayerLeft(WiFiPlazaWork *work, int bit) {
    u32 mask = 1 << bit;
    if (work->unk_f8[1] & mask) {
        work->unk_f8[1] &= ~mask;
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaWork_IsEventFlagSet(WiFiPlazaWork *work, int bit) {
    return (work->unk_f8[2] & (1 << bit)) != 0;
}

BOOL WiFiPlazaWork_TakePlayerUpdated(WiFiPlazaWork *work, int bit) {
    u32 mask = 1 << bit;
    if (work->unk_f8[3] & mask) {
        work->unk_f8[3] &= ~mask;
        return TRUE;
    }
    return FALSE;
}

void *ov45_0222A2C8(void **ptr) {
    return *ptr;
}

int ov45_0222A2CC(WiFiPlazaWork *work) {
    GF_ASSERT(work != NULL);
    return ov45_0222ECDC(4);
}

u32 ov45_0222A2E0(WiFiPlazaWork *work) {
    GF_ASSERT(work != NULL);
    return work->unk_1fc.low;
}

u32 ov45_0222A2F8(WiFiPlazaWork *work) {
    GF_ASSERT(work != NULL);
    return work->unk_1fc.high;
}

void ov45_0222A310(WiFiPlazaWork *work) {
    work->unk_1fc.high = 1;
}

u32 WiFiPlazaWork_IsLobbyOpen(WiFiPlazaWork *work) {
    return work->unk_1fc.flag0;
}

u32 WiFiPlazaWork_IsClosing(WiFiPlazaWork *work) {
    return work->unk_1fc.flag7;
}

BOOL WiFiPlazaWork_IsClosed(WiFiPlazaWork *work) {
    if (work->unk_1fc.flag1 == 1 && work->unk_1fc.unk_04 <= 0) {
        return TRUE;
    }
    return FALSE;
}

u32 WiFiPlazaWork_GetFireworksState(WiFiPlazaWork *work) {
    if (work->unk_1fc.unk_08 <= 0) {
        return work->unk_1fc.mode2;
    }
    return 0;
}

BOOL WiFiPlazaWork_IsParadeStarted(WiFiPlazaWork *work) {
    if (work->unk_1fc.flag4 == 1 && work->unk_1fc.unk_0a <= 0) {
        return TRUE;
    }
    return FALSE;
}

u32 ov45_0222A394(WiFiPlazaWork *work) {
    return work->unk_1fc.mode5;
}

BOOL WiFiPlazaWork_IsLightingStarted(WiFiPlazaWork *work) {
    if (work->unk_08 == 1 && work->unk_1fc.unk_06 <= 0) {
        return TRUE;
    }
    return FALSE;
}

u32 WiFiPlazaWork_GetColorEvent(WiFiPlazaWork *work) {
    if (work->unk_1fc.unk_06 <= 0) {
        return work->unk_1fc.unk_01;
    }
    return 1;
}

u32 WiFiPlazaWork_GetLight1Event(WiFiPlazaWork *work) {
    if (work->unk_1fc.unk_06 <= 0) {
        return work->unk_1fc.unk_02;
    }
    return 7;
}

u32 ov45_0222A3EC(WiFiPlazaWork *work) {
    if (work->unk_1fc.unk_06 <= 0) {
        return work->unk_1fc.unk_03;
    }
    return 11;
}

void ov45_0222A404(WiFiPlazaWork *work) {
    ov45_0222BCA0(&work->unk_3a0);
}

int ov45_0222A414(WiFiPlazaWork *work) {
    return ov45_0222BCA8(&work->unk_3a0);
}

void *ov45_0222A424(WiFiPlazaWork *work) {
    return work->unk_508.unk_04;
}

void ov45_0222A430(WiFiPlazaWork *work, void *value) {
    work->unk_508.unk_04 = value;
}

void WiFiPlazaWork_ClearGameMembers(WiFiPlazaWork *work) {
    MIi_CpuClear32(0, (u32 *)work->unk_3e0, 4);
}

void WiFiPlazaWork_SetGameMember(WiFiPlazaWork *work, int id, u32 index) {
    int value;
    GF_ASSERT(index < 4);
    value = WiFiPlazaLobbyClient_IdToSlot(id);
    GF_ASSERT(value != -1);
    work->unk_3e0[index] = value;
}

void WiFiPlazaWork_SetOwnGameMember(WiFiPlazaWork *work, u32 index) {
    int id = WiFiPlazaLobbyClient_GetOwnId();
    WiFiPlazaWork_SetGameMember(work, id, index);
}

void WiFiPlazaWork_GetGameMembers(WiFiPlazaWork *work, void *dest) {
    MI_CpuCopy8(work->unk_3e0, dest, 4);
}

void ov45_0222A4A8(WiFiPlazaWork *work) {
    ov45_0222BD24(&work->unk_3a0);
}

int ov45_0222A4B8(WiFiPlazaWork *work) {
    return ov45_0222BD2C(&work->unk_3a0);
}

void ov45_0222A4C8(WiFiPlazaWork *work, u8 value) {
    work->unk_1fc.unk_0c = value;
}
