#include <nitro/rtc.h>

#include "global.h"

#include "location_gmm_dat.h"
#include "comm_state.h"
#include "wifi_connection.h"
#include "wifi_plaza_internal.h"

BOOL CommState_HasPlazaLobbyFailed(void);

#pragma force_active on

// out = left + right (hour, minute, second; the hour wraps at 24).
void WiFiPlazaTime_Add(const WiFiPlazaTime *left, const WiFiPlazaTime *right, WiFiPlazaTime *out) {
    WiFiPlazaTime a = *left, b = *right;
    a.field.second += b.field.second;
    a.field.minute += a.field.second / 60;
    out->field.second = a.field.second % 60;
    a.field.minute += b.field.minute;
    a.field.hour += a.field.minute / 60;
    out->field.minute = a.field.minute % 60;
    a.field.hour += b.field.hour;
    out->field.hour = a.field.hour % 24;
}

// out = first - second (hour, minute, second; the hour wraps at 24).
void WiFiPlazaTime_Subtract(const WiFiPlazaTime *first, const WiFiPlazaTime *second, WiFiPlazaTime *out) {
    WiFiPlazaTime a = *first, b = *second;
    int delta;
    u8 value;
    value = b.value[2];
    delta = a.signedValue[2] - (s8)value;
    if (delta < 0) {
        int borrow = (-delta) / 60 + 1;
        a.value[2] = (u8)(a.value[2] + borrow * 60);
        a.value[1] = (u8)(a.value[1] - borrow);
    }
    out->value[2] = (u8)(a.value[2] - value);
    value = b.value[1];
    delta = a.signedValue[1] - (s8)value;
    if (delta < 0) {
        int borrow = (-delta) / 60 + 1;
        a.value[1] = (u8)(a.value[1] + borrow * 60);
        a.value[0] = (u8)(a.value[0] - borrow);
    }
    out->value[1] = (u8)(a.value[1] - value);
    delta = (s8)a.value[0] - (s8)b.value[0];
    if (delta < 0) {
        int borrow = (-delta) / 24 + 1;
        a.value[0] = (u8)(a.value[0] + borrow * 24);
    }
    out->value[0] = (u8)(a.value[0] - b.value[0]);
}

#pragma force_active reset

// Adds a country and region to the list (the first free entry, or the entry of the same place unless value is 1).
void WiFiPlazaPlaces_Add(WiFiPlazaPlace *places, u32 country, u32 region, u32 value) {
    s32 i;
    BOOL use;
    if (country == 0) {
        return;
    }
    if (region > (u32)LocationGmmDatRegionCountGetByCountryMsgNo(country)) {
        return;
    }
    use = FALSE;
    for (i = 0; i < 50; i++) {
        if (places[i].occupied == 0) {
            use = TRUE;
        } else if (places[i].country == country && places[i].region == region) {
            if (value == 1) {
                return;
            }
            use = TRUE;
        }
        if (use) {
            places[i].country = country;
            places[i].region = region;
            places[i].value = value;
            places[i].occupied = 1;
            return;
        }
    }
}

u16 WiFiPlazaPlaces_GetCountry(WiFiPlazaPlace *places, u32 index) {
    if (index >= 50) {
        GF_AssertFail();
    }
    places += index;
    if (places->occupied != 1) {
        GF_AssertFail();
    }
    return places->country;
}

u8 WiFiPlazaPlaces_GetRegion(WiFiPlazaPlace *places, u32 index) {
    if (index >= 50) {
        GF_AssertFail();
    }
    if (places[index].occupied != 1) {
        GF_AssertFail();
    }
    return places[index].region;
}

u8 WiFiPlazaPlaces_GetValue(WiFiPlazaPlace *places, u32 index) {
    if (index >= 50) {
        GF_AssertFail();
    }
    places += index;
    if (places->occupied != 1) {
        GF_AssertFail();
    }
    return places->value;
}

u8 WiFiPlazaPlaces_IsOccupied(WiFiPlazaPlace *places, u32 index) {
    if (index >= 50) {
        GF_AssertFail();
    }
    return places[index].occupied;
}

// Turns off the diffuse, ambient, specular and emission flags of the materials of the first model of file.
void WiFiPlaza_DisableModelLighting(void *file) {
    NNSG3dResMdlSet *set = NNS_G3dGetMdlSet(file);
    NNSG3dResMdl *model = NNS_G3dGetMdlByIdx(set, 0);
    NNSi_G3dModifyMatFlag(model, FALSE, NNS_G3D_MATFLAG_DIFFUSE);
    NNSi_G3dModifyMatFlag(model, FALSE, NNS_G3D_MATFLAG_AMBIENT);
    NNSi_G3dModifyMatFlag(model, FALSE, NNS_G3D_MATFLAG_SPECULAR);
    NNSi_G3dModifyMatFlag(model, FALSE, NNS_G3D_MATFLAG_EMISSION);
}

// TRUE if region exists in country (region 0 for a country without regions).
BOOL WiFiPlaza_IsRegionValid(u32 country, u32 region) {
    u32 count = LocationGmmDatRegionCountGetByCountryMsgNo(country);
    if (count == 0 && region == 0) {
        return TRUE;
    }
    if (region >= 1 && region <= count) {
        return TRUE;
    }
    return FALSE;
}

u8 WiFiPlaza_GetTypeByChoice(u32 index) {
    return ov45_02254BB4[index];
}

// The error message class of a connection error (WifiConnection_ClassifyErrorCode, with the plaza's own errors 25 and
// 26).
int WiFiPlaza_GetErrorMessageClass(u32 errorCode, u32 context) {
    int class = WifiConnection_ClassifyErrorCode(errorCode, context);
    if (class == 11 || errorCode == 25) {
        if (context != 2) {
            return 14;
        }
        return 11;
    }
    if (errorCode == 26) {
        return 13;
    }
    if (class < 0) {
        class = 11;
    }
    return class;
}

// FALSE for the errors that end the connection.
BOOL WiFiPlaza_IsErrorRecoverable(u32 errorCode, u32 context) {
    int class = WifiConnection_ClassifyErrorCode(errorCode, context);
    if (errorCode == 25) {
        class = 11;
    } else if (errorCode == 26) {
        class = 12;
    }
    switch (class) {
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return FALSE;
    default:
        return TRUE;
    }
}

BOOL WiFiPlaza_HasConnectionFailed(void) {
    if (CommState_HasWifiFailed() || CommState_HasPlazaLobbyFailed()) {
        return TRUE;
    }
    return FALSE;
}

WiFiPlazaNews *WiFiPlazaNews_New(enum HeapID heapID) {
    WiFiPlazaNews *news = Heap_Alloc(heapID, sizeof(WiFiPlazaNews));
    memset(news, 0, sizeof(WiFiPlazaNews));
    WiFiPlazaNewsSlots_Init(&news->slots);
    WiFiPlazaNewsEventList_Init(&news->events, heapID);
    return news;
}

void WiFiPlazaNews_Delete(WiFiPlazaNews *news) {
    WiFiPlazaNewsEventList_Free(&news->events);
    Heap_Free(news);
}

void WiFiPlazaNews_Update(WiFiPlazaNews *news) {
    WiFiPlazaNewsEventList_Update(&news->events);
    WiFiPlazaNewsSlots_SetChange(&news->slots, 0);
}
