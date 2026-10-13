#ifndef POKEHEARTGOLD_SAVE_WII_MESSAGE_H
#define POKEHEARTGOLD_SAVE_WII_MESSAGE_H

#include "save.h"

typedef struct {
    char wiiAddress[0x33];
    u8 unk33;
    u32 notificationsEnabled;
    u16 registrationCodePrefix;
    u16 registrationCode;
    u32 password;
} WiiMessageSave;

typedef struct {
    u8 version;
    u8 language;
    u8 country;
    u8 region;
    u32 otId;
    u16 name[PLAYER_NAME_LENGTH + 1];
    u32 unk18;
    u8 macAddress[0x8];
    char wiiAddress[0x33];
    u8 unk57[0x5];
    u32 wiiNotificationsEnabled;
    u16 registrationCodePrefix;
    u16 registrationCode;
} WifiServerProfile;

u32 Save_WiiMessage_sizeof(void);
void Save_WiiMessage_Init(WiiMessageSave *a0);
void Save_WiiMessage_Reset(SaveData *saveData);
BOOL Save_WiiMessage_IsRegistered(SaveData *saveData);
void Save_WiiMessage_SetWiiAddress(SaveData *saveData, char *a1);
WiiMessageSave *Save_WiiMessage_Get(SaveData *saveData);
void Save_WiiMessage_SetParam(SaveData *saveData, u32 a1, u32 a2);
u32 Save_WiiMessage_GetParam(SaveData *saveData, u32 a1);
s32 Save_WiiMessage_InitRegistrationProfile(SaveData *saveData, WifiServerProfile *a1);
void Save_WiiMessage_GetServerProfile(SaveData *saveData, WifiServerProfile *a1);

#endif // POKEHEARTGOLD_SAVE_WII_MESSAGE_H
