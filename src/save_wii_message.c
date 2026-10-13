#include "save_wii_message.h"

#include "global.h"

#include "math_util.h"
#include "player_data.h"
#include "save.h"
#include "save_wifi_history.h"

u32 Save_WiiMessage_sizeof(void) {
    return sizeof(WiiMessageSave);
}

void Save_WiiMessage_Init(WiiMessageSave *a0) {
    MI_CpuClear8(a0, sizeof(WiiMessageSave));
    memset(a0, 0, sizeof(a0->wiiAddress));
    a0->notificationsEnabled = 1;
    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}

void Save_WiiMessage_Reset(SaveData *saveData) {
    Save_WiiMessage_Init(SaveArray_Get(saveData, SAVE_UNK_31));
}

BOOL Save_WiiMessage_IsRegistered(SaveData *saveData) {
    WiiMessageSave *ptr = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));
    if ((s8)(ptr->wiiAddress[0]) != 0) {
        return TRUE;
    }
    return FALSE;
}

void Save_WiiMessage_SetWiiAddress(SaveData *saveData, char *a1) {
    WiiMessageSave *ptr = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));
    strcpy(ptr->wiiAddress, a1);
    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}

WiiMessageSave *Save_WiiMessage_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_UNK_31);
}

void Save_WiiMessage_SetParam(SaveData *saveData, u32 a1, u32 a2) {
    WiiMessageSave *ptr = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));
    switch (a1) {
    case 0:
        ptr->notificationsEnabled = a2;
        break;
    case 1:
        ptr->registrationCodePrefix = a2;
        break;
    case 2:
        ptr->registrationCode = a2;
        break;
    case 3:
        ptr->password = a2;
        break;
    }
    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}

u32 Save_WiiMessage_GetParam(SaveData *saveData, u32 a1) {
    WiiMessageSave *ptr = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));
    switch (a1) {
    case 0:
        return ptr->notificationsEnabled;
    case 1:
        return ptr->registrationCodePrefix;
    case 2:
        return ptr->registrationCode;
    case 3:
        return ptr->password;
    }
    return 0;
}

static void WiiMessage_FillServerProfile(SaveData *saveData, WifiServerProfile *a1) {
    void *wifiHistory;
    WiiMessageSave *ptr2;
    PlayerProfile *profile;

    wifiHistory = Save_WiFiHistory_Get(saveData);
    profile = Save_PlayerData_GetProfile(saveData);
    ptr2 = Save_WiiMessage_Get(saveData);

    MI_CpuClear8(a1, sizeof(WifiServerProfile));
    a1->version = GAME_VERSION;
    a1->language = GAME_LANGUAGE;
    a1->country = WifiHistory_GetPlayerCountry(wifiHistory);
    a1->region = WiFiHistory_GetPlayerRegion(wifiHistory);
    a1->otId = PlayerProfile_GetTrainerID(profile);
    CopyU16StringArray(a1->name, PlayerProfile_GetNamePtr(profile));
    a1->unk18 = 0;
    strcpy(a1->wiiAddress, ptr2->wiiAddress);
    a1->wiiNotificationsEnabled = Save_WiiMessage_GetParam(saveData, 0);

    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}

s32 Save_WiiMessage_InitRegistrationProfile(SaveData *saveData, WifiServerProfile *a1) {
    s32 rand;

    WiiMessage_FillServerProfile(saveData, a1);

    rand = LCRandom() % 1000;
    a1->registrationCodePrefix = rand;
    a1->registrationCode = 0xffff;

    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
    return rand;
}

void Save_WiiMessage_GetServerProfile(SaveData *saveData, WifiServerProfile *a1) {
    WiiMessageSave *ptr = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));

    WiiMessage_FillServerProfile(saveData, a1);

    a1->registrationCodePrefix = ptr->registrationCodePrefix;
    a1->registrationCode = ptr->registrationCode;

    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}
