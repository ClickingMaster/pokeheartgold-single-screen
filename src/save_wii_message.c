#include "save_wii_message.h"

#include "global.h"

#include "math_util.h"
#include "player_data.h"
#include "save.h"
#include "save_wifi_history.h"

u32 Save_WiiMessage_sizeof(void) {
    return sizeof(WiiMessageSave);
}

void Save_WiiMessage_Init(WiiMessageSave *wiiMessage) {
    MI_CpuClear8(wiiMessage, sizeof(WiiMessageSave));
    memset(wiiMessage, 0, sizeof(wiiMessage->wiiAddress));
    wiiMessage->notificationsEnabled = 1;
    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}

void Save_WiiMessage_Reset(SaveData *saveData) {
    Save_WiiMessage_Init(SaveArray_Get(saveData, SAVE_UNK_31));
}

BOOL Save_WiiMessage_IsRegistered(SaveData *saveData) {
    WiiMessageSave *wiiMessage = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));
    if ((s8)(wiiMessage->wiiAddress[0]) != 0) {
        return TRUE;
    }
    return FALSE;
}

void Save_WiiMessage_SetWiiAddress(SaveData *saveData, char *wiiAddress) {
    WiiMessageSave *wiiMessage = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));
    strcpy(wiiMessage->wiiAddress, wiiAddress);
    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}

WiiMessageSave *Save_WiiMessage_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_UNK_31);
}

void Save_WiiMessage_SetParam(SaveData *saveData, u32 param, u32 value) {
    WiiMessageSave *wiiMessage = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));
    switch (param) {
    case WII_MESSAGE_PARAM_NOTIFICATIONS:
        wiiMessage->notificationsEnabled = value;
        break;
    case WII_MESSAGE_PARAM_REGISTRATION_CODE_PREFIX:
        wiiMessage->registrationCodePrefix = value;
        break;
    case WII_MESSAGE_PARAM_REGISTRATION_CODE:
        wiiMessage->registrationCode = value;
        break;
    case WII_MESSAGE_PARAM_PASSWORD:
        wiiMessage->password = value;
        break;
    }
    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}

u32 Save_WiiMessage_GetParam(SaveData *saveData, u32 param) {
    WiiMessageSave *wiiMessage = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));
    switch (param) {
    case WII_MESSAGE_PARAM_NOTIFICATIONS:
        return wiiMessage->notificationsEnabled;
    case WII_MESSAGE_PARAM_REGISTRATION_CODE_PREFIX:
        return wiiMessage->registrationCodePrefix;
    case WII_MESSAGE_PARAM_REGISTRATION_CODE:
        return wiiMessage->registrationCode;
    case WII_MESSAGE_PARAM_PASSWORD:
        return wiiMessage->password;
    }
    return 0;
}

static void WiiMessage_FillServerProfile(SaveData *saveData, WifiServerProfile *serverProfile) {
    SaveWiFiHistory *wifiHistory;
    WiiMessageSave *wiiMessage;
    PlayerProfile *profile;

    wifiHistory = Save_WiFiHistory_Get(saveData);
    profile = Save_PlayerData_GetProfile(saveData);
    wiiMessage = Save_WiiMessage_Get(saveData);

    MI_CpuClear8(serverProfile, sizeof(WifiServerProfile));
    serverProfile->version = GAME_VERSION;
    serverProfile->language = GAME_LANGUAGE;
    serverProfile->country = WifiHistory_GetPlayerCountry(wifiHistory);
    serverProfile->region = WiFiHistory_GetPlayerRegion(wifiHistory);
    serverProfile->otId = PlayerProfile_GetTrainerID(profile);
    CopyU16StringArray(serverProfile->name, PlayerProfile_GetNamePtr(profile));
    serverProfile->unk18 = 0;
    strcpy(serverProfile->wiiAddress, wiiMessage->wiiAddress);
    serverProfile->wiiNotificationsEnabled = Save_WiiMessage_GetParam(saveData, 0);

    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}

s32 Save_WiiMessage_InitRegistrationProfile(SaveData *saveData, WifiServerProfile *serverProfile) {
    s32 rand;

    WiiMessage_FillServerProfile(saveData, serverProfile);

    rand = LCRandom() % 1000;
    serverProfile->registrationCodePrefix = rand;
    serverProfile->registrationCode = 0xffff;

    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
    return rand;
}

void Save_WiiMessage_GetServerProfile(SaveData *saveData, WifiServerProfile *serverProfile) {
    WiiMessageSave *ptr = (WiiMessageSave *)(SaveArray_Get(saveData, SAVE_UNK_31));

    WiiMessage_FillServerProfile(saveData, serverProfile);

    serverProfile->registrationCodePrefix = ptr->registrationCodePrefix;
    serverProfile->registrationCode = ptr->registrationCode;

    SaveSubstruct_UpdateCRC(SAVE_UNK_31);
}
